#include "../Manager/InputManager.h"
#include "../Application.h"
#include "PauseMenu.h"
#include "SceneBase.h"

int SceneBase::fontTitle_ = -1;
int SceneBase::fontPress_ = -1;
int SceneBase::fontUI_ = -1;

SceneBase::SceneBase(void)
{
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}

void SceneBase::Release(void)
{
}

void SceneBase::UpdateBase(void)
{
	auto& ins = InputManager::GetInstance();

	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	rePress = newPress;
	newPress = ins.IsNew(KEY_INPUT_ESCAPE);

	// Escapeキーでポーズ切り替え
	if ((rePress == 0 && newPress == 1))
	{
		switch (state_)
		{
		case PauseMenu::MENU_STATE::MAIN:
			pauseMenu_->Toggle();
			break;
		case PauseMenu::MENU_STATE::CONFIRM:
			pauseMenu_->GameExitConfirm();
			break;
		default:
			break;
		}
	}

	if (pauseMenu_->IsActive())
	{
		pauseMenu_->Update();
		return; // ポーズ中はゲーム更新停止
	}

	Update();
}

void SceneBase::InitFonts()
{
	fontTitle_ = CreateFontToHandle(
		"ＭＳ ゴシック", 48, 4,
		DX_FONTTYPE_ANTIALIASING_EDGE, -1, 2
	);

	fontPress_ = CreateFontToHandle(
		"メイリオ", 28, 3,
		DX_FONTTYPE_ANTIALIASING_EDGE, -1, 2
	);

	fontUI_ = CreateFontToHandle(
		"ＭＳ ゴシック", 20, 2,
		DX_FONTTYPE_ANTIALIASING_EDGE, -1, 1
	);
}

void SceneBase::ReleaseFonts()
{
	DeleteFontToHandle(fontTitle_);
	DeleteFontToHandle(fontPress_);
	DeleteFontToHandle(fontUI_);
}

void SceneBase::DrawFont(const char* text)
{
	//const char* text = "Space/Aでスタート";

	int w = GetDrawStringWidthToHandle(text, strlen(text), fontPress_);
	int x = (Application::SCREEN_SIZE_X - w) / 2;
	int y = Application::SCREEN_SIZE_Y - 60;

	DrawStringToHandle(x, y, text, GetColor(255, 255, 255), fontPress_);
}
