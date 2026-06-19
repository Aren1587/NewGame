#include "PauseMenu.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "../Scene/SceneBase.h"
#include "../Application.h"

void PauseMenu::Toggle()
{
    if (isActive_)
    {
        SceneManager::GetInstance().SetIsPause(false);
    }

    isActive_ = !isActive_;
}

void PauseMenu::Update()
{
    if (!isActive_) 
    {
        SceneManager::GetInstance().SetIsPause(false);
        return;
    }

    SceneManager::GetInstance().SetIsPause(true);

    auto& ins = InputManager::GetInstance();

    // パッド取得
    InputManager::JOYPAD_IN_STATE padState =
        ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

    auto trigger = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);
    auto f = trigger.z;


    // 通常メニュー状態
    if (state_ == MENU_STATE::MAIN)
    {
        // --- 移動 ---
        if (ins.IsTrgDown(KEY_INPUT_UP) ||
            ins.IsTrgDown(KEY_INPUT_W) ||
            f > 0.0f)
        {
            if (!isTrigger_) selectedIndex_--;
            if (selectedIndex_ < 0) selectedIndex_ = static_cast<int>(items_.size()) - 1;
        }
        else if (ins.IsTrgDown(KEY_INPUT_DOWN) ||
            ins.IsTrgDown(KEY_INPUT_S) ||
            f < 0.0f)
        {
            if (!isTrigger_) selectedIndex_++;
            if (selectedIndex_ >= static_cast<int>(items_.size())) selectedIndex_ = 0;
        }

        MENU_ITEM item = static_cast<MENU_ITEM>(selectedIndex_);

        // --- 決定 ---
        if (ins.IsTrgDown(KEY_INPUT_RETURN) ||
            ins.IsTrgDown(KEY_INPUT_SPACE) ||
            ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
        {
            switch (item)
            {
            case MENU_ITEM::RESUME: // 再開
                SceneManager::GetInstance().SetIsPause(false);
                isActive_ = false;
                break;

            case MENU_ITEM::BACK_TO_TITLE: // タイトル
                SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
                SceneManager::GetInstance().SetIsPause(false);
                break;

            case MENU_ITEM::EXIT: // 終了 → 確認状態へ
                if (isConfirm_)
                {
                    SceneManager::GetInstance().SetIsPause(false);
                    isActive_ = false;
                    break;
                }
                state_ = MENU_STATE::CONFIRM;
                selectedIndex_ = 1; // 「いいえ」を初期位置
                break;
            }
        }
    }


    // 終了確認状態
    else if (state_ == MENU_STATE::CONFIRM)
    {
        // 左右移動（2択）
        if (ins.IsTrgDown(KEY_INPUT_UP) || ins.IsTrgDown(KEY_INPUT_A) || trigger.x < 0)
        {
            if (!isTrigger_) selectedIndex_ = 0; // はい
        }
        else if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_D) || trigger.x > 0)
        {
            if (!isTrigger_) selectedIndex_ = 1; // いいえ
        }

        // 決定
        if (ins.IsTrgDown(KEY_INPUT_RETURN) ||
            ins.IsTrgDown(KEY_INPUT_SPACE) ||
            ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
        {
            if (selectedIndex_ == 0)
            {
                // はい → 終了
                Application::GetInstance().GameEnd();
            }
            else
            {
                // いいえ → メインメニューに戻る
                state_ = MENU_STATE::MAIN;
                selectedIndex_ = 0;
                if (isConfirm_) 
                {
                    SceneManager::GetInstance().SetIsPause(false);
                    isActive_ = false;
                }
            }
        }
    }

    // トリガーフラグ更新
    isTrigger_ = (trigger.x != 0.0f) || (trigger.y != 0.0f) || (trigger.z != 0.0f);
}

void PauseMenu::Draw()
{
    if (!isActive_) return;

    // 半透明背景
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    const char* text = "PAUSE";

    int w = GetDrawStringWidthToHandle(text, strlen(text), SceneBase::fontTitle_);
    int x = (Application::SCREEN_SIZE_X - w) / 2;
    int y = 150;

    DrawStringToHandle(x, y, text, GetColor(255, 255, 255), SceneBase::fontTitle_);


    // 通常メニューの描画
    if (state_ == MENU_STATE::MAIN)
    {
        int baseY = 300;

        // 一番長い文字幅を調べる
        int maxWidth = 0;
        for (int i = 0; i < items_.size(); i++)
        {
            std::string text = "→ " + items_[i];   // カーソル込みで計算
            int w = GetDrawStringWidthToHandle(
                text.c_str(),
                text.length(),
                SceneBase::fontTitle_
            );
            if (w > maxWidth) maxWidth = w;
        }

        // 画面中央に「全体」を置く
        int baseX = (Application::SCREEN_SIZE_X - maxWidth) / 2;

        // 全項目を描く
        for (int i = 0; i < items_.size(); i++)
        {
            const char* cursor = (i == selectedIndex_) ? "→ " : "   ";
            std::string text = std::string(cursor) + items_[i];

            int y = baseY + i * 50;

            int color = (i == selectedIndex_) ?
                GetColor(255, 255, 0) :
                GetColor(200, 200, 200);

            DrawStringToHandle(
                baseX,
                y,
                text.c_str(),
                color,
                SceneBase::fontTitle_
            );
        }
    }
    else if (state_ == MENU_STATE::CONFIRM) // 終了確認ウィンドウの描画
    {
        // 確認メッセージ（中央）
        const char* msg = "ゲームを終了しますか？";

        int msgW = GetDrawStringWidthToHandle(
            msg,
            strlen(msg),
            SceneBase::fontTitle_
        );

        int msgX = (Application::SCREEN_SIZE_X - msgW) / 2;
        int msgY = 260;

        DrawStringToHandle(
            msgX,
            msgY,
            msg,
            GetColor(255, 255, 255),
            SceneBase::fontTitle_
        );

        // はい、いいえの左揃え計算
        const char* labels[2] = { "はい", "いいえ" };

        int maxWidth = 0;
        for (int i = 0; i < 2; i++)
        {
            std::string t = "→ ";
            t += labels[i];

            int w = GetDrawStringWidthToHandle(
                t.c_str(),
                t.length(),
                SceneBase::fontTitle_
            );

            if (w > maxWidth) maxWidth = w;
        }

        int baseX = (Application::SCREEN_SIZE_X - maxWidth) / 2;
        int baseY = 350;

        // 描画
        for (int i = 0; i < 2; i++)
        {
            const char* cursor = (selectedIndex_ == i) ? "→ " : "   ";

            std::string text = std::string(cursor) + labels[i];

            int color = (selectedIndex_ == i) ?
                GetColor(255, 255, 0) :
                GetColor(200, 200, 200);

            DrawStringToHandle(
                baseX,
                baseY + i * 50,
                text.c_str(),
                color,
                SceneBase::fontTitle_
            );
        }
    }

}
