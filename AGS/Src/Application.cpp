#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_MODEL = PATH_DATA + "Model/";
const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";
const std::string Application::PATH_SE = PATH_DATA + "SE/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::DeleteInstance(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("2416030_野本アレン");

	// ウィンドウサイズ
	GetWindowSize(&w, &h);
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	SetGlobalAmbientLight(GetColorF(0.0f,0.0f,0.0f,1.0f));

	SetLightEnable(FALSE); // 既存ライトOFF
	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.0f, 1.0f)); // 環境光ゼロ
	SetBackgroundColor(0, 0, 0); // 背景も黒

	// ScreenFlip を実行しても垂直同期信号を待たない
	//SetWaitVSyncFlag(FALSE);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	//SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	// 乱数の初期化
	srand((unsigned int)time(NULL));

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	// 背景色（黒）
	SetBackgroundColor(0, 0, 0);
}

void Application::Run(void)
{

	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && !isGameEnd_)
	{

		inputManager.Update();
		sceneManager.Update();

		sceneManager.Draw();

		ScreenFlip();

	}

}

void Application::Destroy(void)
{

	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	// 入力制御解放
	InputManager::GetInstance().Destroy();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
	isGameEnd_ = false;
}
