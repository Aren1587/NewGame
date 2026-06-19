#pragma once
#include <DxLib.h>
#include <vector>
#include <string>

class PauseMenu
{
public:
    enum class MENU_STATE {
        MAIN,       // pause 通常メニュー
        CONFIRM     // 終了確認
    };

    enum class MENU_ITEM
    {
        RESUME = 0,         // 再開
        BACK_TO_TITLE,  // タイトルへ戻る
        EXIT            // 終了
    };

    PauseMenu()
        : isActive_(false), selectedIndex_(0)
    {
        items_ = { "再開", "タイトルに戻る", "終了" };
        isTrigger_ = false;
    }

    void Toggle();
    void GameExitConfirm() { state_ = MENU_STATE::CONFIRM; isConfirm_ = true; isActive_ = !isActive_; }
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
    void ResetInput() { isTrigger_ = true; }

private:

    MENU_STATE state_ = MENU_STATE::MAIN;
    bool isConfirm_ = false;

    bool isActive_;
    int selectedIndex_;
    std::vector<std::string> items_;
    bool isTrigger_;
};
