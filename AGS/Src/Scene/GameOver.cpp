#include "GameClear.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "GameOver.h"

GameOver::GameOver(void)
{
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
	pauseMenu_ = new PauseMenu();

	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "Over.png").c_str());

}

void GameOver::Update(void)
{
	// ÉVÅ[ÉìëJà⁄
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOver::Draw(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0.5f, 0.0, imgTitle_, true);

	DrawFont("Space/AÇ≈ñﬂÇÈ");
}

void GameOver::Release(void)
{
	DeleteGraph(imgTitle_);
}
