#include "PauseMenu.h"
#include <algorithm>
#include <string>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Scene/SceneBase.h"
#include "../Application.h"

namespace
{
    // レイアウト
    constexpr int BACK_ALPHA = 180;
    constexpr int TITLE_Y = 150;
    constexpr int MAIN_LIST_Y = 300;
    constexpr int CONFIRM_MSG_Y = 260;
    constexpr int CONFIRM_LIST_Y = 350;
    constexpr int LINE_HEIGHT = 50;

    // 文字列
    constexpr const char* TITLE_TEXT = "PAUSE";
    constexpr const char* CONFIRM_MSG = "ゲームを終了しますか？";
    constexpr const char* CURSOR_ON = "→ ";
    constexpr const char* CURSOR_OFF = "   ";

    // MENU_ITEM / CONFIRM_ITEM の並びと一致させる
    const char* const MAIN_LABELS[] = { "再開", "タイトルに戻る", "終了" };
    const char* const CONFIRM_LABELS[] = { "はい", "いいえ" };

    constexpr int MAIN_COUNT = static_cast<int>(PauseMenu::MENU_ITEM::MAX);
    constexpr int CONFIRM_COUNT = 2;

    static_assert(sizeof(MAIN_LABELS) / sizeof(MAIN_LABELS[0]) == MAIN_COUNT,
        "MAIN_LABELS と MENU_ITEM の数が一致していません");
    static_assert(sizeof(CONFIRM_LABELS) / sizeof(CONFIRM_LABELS[0]) == CONFIRM_COUNT,
        "CONFIRM_LABELS の数が不正です");

    // 描画ヘルパー
    int TextWidth(const std::string& s)
    {
        return GetDrawStringWidthToHandle(
            s.c_str(), static_cast<int>(s.length()), SceneBase::fontTitle_);
    }

    // 画面中央揃えで1行描く
    void DrawCenteredString(int y, const std::string& s, int color)
    {
        const int x = (Application::SCREEN_SIZE_X - TextWidth(s)) / 2;
        DrawStringToHandle(x, y, s.c_str(), color, SceneBase::fontTitle_);
    }

    // 「リスト全体」を画面中央に置き、各項目は左揃えで縦に並べる
    void DrawOptionList(const char* const* labels, int count, int selected, int baseY)
    {
        const int colorSelected = GetColor(255, 255, 0);
        const int colorNormal = GetColor(200, 200, 200);

        // 一番長い幅（カーソル込み）を基準にする
        int maxWidth = 0;
        for (int i = 0; i < count; i++)
        {
            maxWidth = (std::max)(maxWidth, TextWidth(std::string(CURSOR_ON) + labels[i]));
        }
        const int baseX = (Application::SCREEN_SIZE_X - maxWidth) / 2;

        for (int i = 0; i < count; i++)
        {
            const bool isSelected = (i == selected);
            const std::string text = std::string(isSelected ? CURSOR_ON : CURSOR_OFF) + labels[i];

            DrawStringToHandle(
                baseX,
                baseY + i * LINE_HEIGHT,
                text.c_str(),
                isSelected ? colorSelected : colorNormal,
                SceneBase::fontTitle_);
        }
    }

    // 入力ヘルパー
    bool IsDecidePressed()
    {
        auto& ins = InputManager::GetInstance();
        return ins.IsTrgDown(KEY_INPUT_RETURN)
            || ins.IsTrgDown(KEY_INPUT_SPACE)
            || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN);
    }
}

PauseMenu::PauseMenu()
    : 
    state_(MENU_STATE::MAIN),
    isActive_(false),
    isDirectConfirm_(false),
    selectedIndex_(0),
    isStickHeld_(false)
{
}

void PauseMenu::Toggle()
{
    if (isActive_)
    {
        Close();
    }
    else
    {
        Open();
    }
}

void PauseMenu::GameExitConfirm()
{
    Open();
    state_ = MENU_STATE::CONFIRM;
    selectedIndex_ = static_cast<int>(CONFIRM_ITEM::NO);   // 誤爆防止で「いいえ」から
    isDirectConfirm_ = true;
}

void PauseMenu::Open()
{
    isActive_ = true;
    isDirectConfirm_ = false;
    state_ = MENU_STATE::MAIN;
    selectedIndex_ = 0;
}

void PauseMenu::Close()
{
    isActive_ = false;
    isDirectConfirm_ = false;
    state_ = MENU_STATE::MAIN;
    selectedIndex_ = 0;
    SceneManager::GetInstance().SetIsPause(false);
}

void PauseMenu::Update()
{
    SceneManager::GetInstance().SetIsPause(isActive_);
    if (!isActive_)
    {
        return;
    }

    switch (state_)
    {
    case MENU_STATE::MAIN:    StateMainUpdate();    break;
    case MENU_STATE::CONFIRM: StateConfirmUpdate(); break;
    }
}

PauseMenu::StickState PauseMenu::ReadStick() const
{
    auto& ins = InputManager::GetInstance();

    InputManager::JOYPAD_IN_STATE padState =
        ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

    const auto dir = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);

    StickState s;
    s.x = dir.x;
    s.z = dir.z;
    s.tilted = (dir.x != 0.0f) || (dir.y != 0.0f) || (dir.z != 0.0f);
    return s;
}

void PauseMenu::StateMainUpdate()
{
    auto& ins = InputManager::GetInstance();
    const StickState stick = ReadStick();

    // 移動量（-1: 上, +1: 下）。スティックは倒し始めの1回だけ反応させる
    int move = 0;
    if (ins.IsTrgDown(KEY_INPUT_UP) || ins.IsTrgDown(KEY_INPUT_W))
    {
        move = -1;
    }
    else if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_S))
    {
        move = 1;
    }
    else if (!isStickHeld_)
    {
        if (stick.z > 0.0f)      move = -1;
        else if (stick.z < 0.0f) move = 1;
    }
    isStickHeld_ = stick.tilted;

    if (move != 0)
    {
        selectedIndex_ = (selectedIndex_ + move + MAIN_COUNT) % MAIN_COUNT;
    }

    if (!IsDecidePressed())
    {
        return;
    }

    switch (static_cast<MENU_ITEM>(selectedIndex_))
    {
    case MENU_ITEM::RESUME:
        Close();
        break;

    case MENU_ITEM::BACK_TO_TITLE:
        // シーン切替後に自身が破棄されても安全なよう、メンバ操作を先に済ませる
        Close();
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
        break;

    case MENU_ITEM::EXIT:
        state_ = MENU_STATE::CONFIRM;
        selectedIndex_ = static_cast<int>(CONFIRM_ITEM::NO);   // 「いいえ」を初期位置
        break;

    default:
        break;
    }
}

void PauseMenu::StateConfirmUpdate()
{
    auto& ins = InputManager::GetInstance();
    const StickState stick = ReadStick();

    // 選択（上 or 左 = はい, 下 or 右 = いいえ）
    if (ins.IsTrgDown(KEY_INPUT_UP) || ins.IsTrgDown(KEY_INPUT_A))
    {
        selectedIndex_ = static_cast<int>(CONFIRM_ITEM::YES);
    }
    else if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_D))
    {
        selectedIndex_ = static_cast<int>(CONFIRM_ITEM::NO);
    }
    else if (!isStickHeld_)
    {
        if (stick.x < 0.0f || stick.z > 0.0f)      selectedIndex_ = static_cast<int>(CONFIRM_ITEM::YES);
        else if (stick.x > 0.0f || stick.z < 0.0f) selectedIndex_ = static_cast<int>(CONFIRM_ITEM::NO);
    }
    isStickHeld_ = stick.tilted;

    if (!IsDecidePressed())
    {
        return;
    }

    if (selectedIndex_ == static_cast<int>(CONFIRM_ITEM::YES))
    {
        Application::GetInstance().GameEnd();
        return;
    }

    // 「いいえ」
    if (isDirectConfirm_)
    {
        // 終了確認から直接開いた場合は、ポーズごと閉じる
        Close();
    }
    else
    {
        // メインメニューに戻る
        state_ = MENU_STATE::MAIN;
        selectedIndex_ = 0;
    }
}

void PauseMenu::Draw()
{
    if (!isActive_)
    {
        return;
    }

    // 半透明背景
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, BACK_ALPHA);
    DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawCenteredString(TITLE_Y, TITLE_TEXT, GetColor(255, 255, 255));

    switch (state_)
    {
    case MENU_STATE::MAIN:    DrawMain();    break;
    case MENU_STATE::CONFIRM: DrawConfirm(); break;
    }
}

void PauseMenu::DrawMain() const
{
    DrawOptionList(MAIN_LABELS, MAIN_COUNT, selectedIndex_, MAIN_LIST_Y);
}

void PauseMenu::DrawConfirm() const
{
    DrawCenteredString(CONFIRM_MSG_Y, CONFIRM_MSG, GetColor(255, 255, 255));
    DrawOptionList(CONFIRM_LABELS, CONFIRM_COUNT, selectedIndex_, CONFIRM_LIST_Y);
}