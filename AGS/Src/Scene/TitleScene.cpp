#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "PauseMenu.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:
	imgTitle_(-1)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "TitleImage.png").c_str());

	state_ = PauseMenu::MENU_STATE::CONFIRM;
}

void TitleScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::Draw(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0.5f, 0.0, imgTitle_, true);

	// 点滅処理
	float time = GetNowHiPerformanceCount() / 1000000.0f;

	// 0.0 ～ 1.0
	float alpha = (sinf(time * 3.0f) + 1.0f) * 0.5f;

	// 140 ～ 255 の範囲で明滅
	int alphaValue = 140 + (int)(alpha * 115.0f);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

	DrawFont("Space/Aでスタート");

	// ブレンド設定を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	pauseMenu_->Draw();
}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);
}
