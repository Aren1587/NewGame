#pragma once
#include "PauseMenu.h"

class PauseMenu;

class SceneBase
{

public:
	enum PAUSE_SCENE
	{
		MAIN,       // pause 通常メニュー
		CONFIRM     // 終了確認
	};

	static int fontTitle_;
	static int fontPress_;
	static int fontUI_;

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	void UpdateBase(void);

	static void InitFonts();
	static void ReleaseFonts();

	void DrawFont(const char* text);
protected:
	PauseMenu* pauseMenu_;
	int rePress, newPress;

	PauseMenu::MENU_STATE state_;
};
