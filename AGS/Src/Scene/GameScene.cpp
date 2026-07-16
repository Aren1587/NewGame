#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Stage.h"
#include "../Object/Enemy.h"
#include "../Manager/Camera.h"
#include "../Manager/Light.h"
#include "PauseMenu.h"
#include "GameScene.h"
#include <algorithm>

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	stage_ = new Stage();
	stage_->Init();

	enemy_ = new Enemy();
	enemy_->Init();

	// カメラをフリーモードにする
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FREE);

	lightPow_ = 0.001f;

	seId_ = LoadSoundMem((Application::PATH_SE + "Aura.mp3").c_str());

	pauseMenu_ = new PauseMenu();

	isMove = false;

	rePress = newPress = 0;

	footSeId_ = LoadSoundMem((Application::PATH_SE + "FootStep.mp3").c_str());
	ChangeVolumeSoundMem(200.0f, footSeId_);

	enemyFootSeId_ = LoadSoundMem((Application::PATH_SE + "EnemyFootStep.mp3").c_str());
	ChangeVolumeSoundMem(200.0f, enemyFootSeId_);

	state_ = PauseMenu::MENU_STATE::MAIN;
}

void GameScene::Update(void)
{
	auto& ins = InputManager::GetInstance();

	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	rePress = newPress;
	newPress = ins.IsNew(KEY_INPUT_ESCAPE);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP))
	{
		//printfDx("Pad TOP Trigger\n");
	}
	if (ins.IsTrgDown(KEY_INPUT_UP))
	{
		//printfDx("Keyboard UP Trigger\n");
	}

	// Escapeキーでポーズ切り替え
	if ((rePress == 0 && newPress == 1))
	{
		pauseMenu_->Toggle();
	}

	if (pauseMenu_->IsActive())
	{
		pauseMenu_->Update();
		return; // ポーズ中はゲーム更新停止
	}

	if (SceneManager::GetInstance().GetCamera().GetIsMove())
	{
		// 足音ループ
		if (!CheckSoundMem(footSeId_))
		{
			PlaySoundMem(footSeId_, DX_PLAYTYPE_BACK);
		}
	}
	else if(CheckSoundMem(footSeId_))
	{
		if (!CheckSoundMem(footSeId_))
		{
			StopSoundMem(footSeId_);
		}
	}

	stage_->Update();
	enemy_->Update();

	// 距離に応じて音量を調整
	DistanceSoundVolume(SceneManager::GetInstance().GetCamera().GetPos(), enemy_->GetPos(), enemyFootSeId_);

	// 足音ループ
	if (!CheckSoundMem(enemyFootSeId_))
	{
		PlaySoundMem(enemyFootSeId_, DX_PLAYTYPE_BACK);
	}
	

	// 発射キー
	if ((CheckHitKey(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		&& !isMove)
	{
		auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
		auto cameraAngle = SceneManager::GetInstance().GetCamera().GetAngle();

		isCollision_ = false;
		isShooting = true;
		startPos_ = cameraPos;

		MATRIX mat = MGetIdent();

		mat = MGetIdent();
		mat = MMult(mat, MGetRotX(cameraAngle.x * 0.5f));
		mat = MMult(mat, MGetRotY(cameraAngle.y));
		moveDir_ = VTransform({ 0.0f,0.0f,1.0f }, mat);

		circlePos_ = VAdd(cameraPos, moveDir_);

		pow = 0.0f;
		powdddd = 20.0f;  // 移動速度
		
	}

	lightPow_ -= 0.001f * SceneManager::GetInstance().GetDeltaTime();

	if (isMove)
	{
		auto& camera = SceneManager::GetInstance().GetCamera();

		// 投げた位置から今の球までの距離を算出
		float dist = VSize(VSub(circlePos_, startPos_));

		// FarClipを距離に応じて設定（最小値を確保）
		camera.SetFarClip(dist);
	}

	Collision();
	CollisionCamera();
}

void GameScene::Draw(void)
{
	stage_->Draw();

	if(isMove)
	{
		// ライトを一時的に無効化
		SetUseLighting(FALSE);
		// 球を描画（常に明るく見える）
		DrawSphere3D(circlePos_, CIRCLE_RADIUS, 10, 0xff0000, 0xffffff, TRUE);
		// ライトを元に戻す
		SetUseLighting(TRUE);
	}
	auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
#ifdef _DEBUG
	DrawFormatString(
		0, 40, 0xFFFFFF, "球座標：(%.2f, %.2f, %.2f)",
		circlePos_.x, circlePos_.y, circlePos_.z);

	DrawFormatString(
		0, 80, 0xFFFFFF, "カメラ座標：(%.2f, %.2f, %.2f)",
		cameraPos.x, cameraPos.y, cameraPos.z);
#endif // DEBUG

	auto ePos = enemy_->GetPos();
	VECTOR diff = VSub(ePos, cameraPos);

	enemy_->Draw();
	SetUseZBuffer3D(TRUE);

	pauseMenu_->Draw();

	auto iu = ConvWorldPosToScreenPos(circlePos_);
}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;
	stage_ = nullptr;

	enemy_->Release();
	delete enemy_;
	enemy_ = nullptr;

	StopSoundMem(seId_);
	DeleteSoundMem(seId_);

	StopSoundMem(footSeId_);
	DeleteSoundMem(footSeId_);

	StopSoundMem(enemyFootSeId_);
	DeleteSoundMem(enemyFootSeId_);
}

bool GameScene::CollisionCamera(void)
{
	auto& camera = SceneManager::GetInstance().GetCamera();

	// Camera の次の座標を取得
	VECTOR nextPos = camera.Move();

	// 衝突判定
	int stageModelId = stage_->GetGoalModelId();
	auto info = MV1CollCheck_Sphere(stageModelId, -1, nextPos, camera.CAMERA_RADIUS);
	if (info.HitNum != 0)
	{
		// 衝突なし → 座標を確定
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
		return true;
	}

	stageModelId = stage_->GetModelId();
	info = MV1CollCheck_Sphere(stageModelId, -1, nextPos, camera.CAMERA_RADIUS);

	if (info.HitNum == 0)
	{
		// 衝突なし → 座標を確定
		camera.ApplyMove(nextPos);
		return true;
	}
	else
	{
		// 衝突あり → 更新なし
	}

	return false;
}

void GameScene::Collision(void)
{
	bool isEnemyHit = EnemyCollision();
	if (isEnemyHit) return;

	bool isStageCollision = StageCollision();
	if (isStageCollision)
	{
		if (!isCollision_)
		{
			CircleCollisionSet();
		}
	}
	else
	{
		CircleMove();
	}
}

bool GameScene::StageCollision(void)
{
	// ステージモデルID
	int stageModelId = stage_->GetModelId();

	auto info = MV1CollCheck_Sphere(
		stageModelId, -1, circlePos_, CIRCLE_RADIUS);

	if (info.HitNum > 0)
	{
		//shot->Blast();
		int a = 0;
		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);
		return true;
	}

	return false;
}

bool GameScene::EnemyCollision(void)
{
	auto cPos = SceneManager::GetInstance().GetCamera().GetPos();
	cPos.y -= 300.0f;

	if (AsoUtility::IsHitSpheres(cPos, 100.0f, enemy_->GetPos(), 100.0f))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		return true;
	}

	return false;
}

void GameScene::CircleCollisionSet(void)
{
	// 接地
	//sceneMana.SetPointLightPos(circlePos_);
	SceneManager::GetInstance().GetLight().IsPointLightPow(circlePos_);
	enemy_->SetTargetPos(circlePos_);
	isCollision_ = true;
	SceneManager::GetInstance().GetCamera().SetFarClip(1800.0f);
	isMove = false;

	// 距離に応じた音量調整
	DistanceSoundVolume(SceneManager::GetInstance().GetCamera().GetPos(), circlePos_, seId_);
	PlaySoundMem(seId_, DX_PLAYTYPE_BACK);
}

void GameScene::DistanceSoundVolume(const VECTOR& soundPos, const VECTOR& soundPos2, int soundId)
{
	// 距離に応じた音量調整
	VECTOR pos1 = soundPos;
	VECTOR pos2 = soundPos2;
	float distance = VSize(VSub(soundPos, soundPos2));

	const float MAX_DISTANCE = 2000.0f; // 聞こえる最大距離
	const int MAX_VOLUME = 255;
	const int MIN_VOLUME = 0;

	float t = 1.0f - (distance / MAX_DISTANCE);
	t = std::clamp(t, 0.0f, 1.0f);
	int volume = static_cast<int>(t * MAX_VOLUME);

	ChangeVolumeSoundMem(volume, soundId);
}

void GameScene::CircleMove(void)
{
	circlePos_ = VAdd(circlePos_, VScale(moveDir_, powdddd * SceneManager::GetInstance().GetDeltaTime()));

	circlePos_.x += movePow.x * SceneManager::GetInstance().GetDeltaTime();
	pow -= GRAVITY * SceneManager::GetInstance().GetDeltaTime();

	circlePos_.y += pow * SceneManager::GetInstance().GetDeltaTime();

	isMove = true;

	if (circlePos_.y < 0)
	{
		isMove = false;
	}
}
