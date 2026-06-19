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
	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "ppap.png").c_str());

	pauseMenu_ = new PauseMenu();

	state_ = PauseMenu::MENU_STATE::CONFIRM;

	pauseMenu_ = new PauseMenu();
	//pauseMenu_->SetActive(true);                // 表示ON
	pauseMenu_->SetState(PauseMenu::MENU_STATE::CONFIRM);
	pauseMenu_->SetSelectedIndex(1);            // 「いいえ」
	pauseMenu_->ResetInput();                   // isT = true 相当
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

	//DrawString((Application::SCREEN_SIZE_X - GetDrawStringWidth("Space/Aでスタート", strlen("Space?Aでスタート"))) / 2, Application::SCREEN_SIZE_Y - 30, "Space/Aでスタート", GetColor(255, 255, 255));
	DrawFont("Space/Aでスタート");

	pauseMenu_->Draw();
}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);

	delete pauseMenu_;
}
