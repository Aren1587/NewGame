#pragma once
#include <DxLib.h>

class PauseMenu
{
public:
    enum class MENU_STATE
    {
        MAIN,       // pause 通常メニュー
        CONFIRM     // 終了確認
    };

    enum class MENU_ITEM
    {
        RESUME = 0,     // 再開
        BACK_TO_TITLE,  // タイトルへ戻る
        EXIT,           // 終了
        MAX
    };

    PauseMenu();
    ~PauseMenu() = default;

    // ポーズの開閉
    void Toggle();

    // ウィンドウ×ボタン等から、終了確認を直接開く
    void GameExitConfirm();

    bool IsActive() const { return isActive_; }

    // 入力処理
    void Update();

    // 描画処理（ゲーム画面の上に重ねる）
    void Draw();

    // 現在選択中のメニューを取得
    int GetSelectedIndex() const { return selectedIndex_; }

    void SetActive(bool f) { isActive_ = f; }
    void SetState(MENU_STATE s) { state_ = s; }
    void SetSelectedIndex(int i) { selectedIndex_ = i; }

    // スティックが倒れっぱなしの状態として扱う（開いた直後の誤爆防止用）
    void ResetInput() { isStickHeld_ = true; }

private:
    // 終了確認の選択肢（描画順と同じ）
    enum class CONFIRM_ITEM
    {
        YES = 0,
        NO,
        MAX
    };

    // スティック入力のスナップショット
    struct StickState
    {
        float x;
        float z;
        bool tilted;    // 何らかの方向に倒れているか
    };

    MENU_STATE state_;
    bool isActive_;
    bool isDirectConfirm_;  // 終了確認から直接開いたか（「いいえ」でメニュー自体を閉じる）
    int  selectedIndex_;
    bool isStickHeld_;      // 前フレームでスティックが倒れていたか（エッジ検出用）

    void Open();
    void Close();

    void StateMainUpdate();
    void StateConfirmUpdate();

    void DrawMain() const;
    void DrawConfirm() const;

    StickState ReadStick() const;
};