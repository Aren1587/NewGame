#include "Enemy.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "Common/AnimationController.h"


Enemy::Enemy(void)
	:
	animationController_(nullptr),
	modelId_(-1),
	angles_(AsoUtility::VECTOR_ZERO),
	moveDir_(AsoUtility::VECTOR_ZERO),
	pos_(AsoUtility::VECTOR_ZERO),
	scales_(AsoUtility::VECTOR_ONE),
	targetPos_(AsoUtility::VECTOR_ZERO),
	isMoveSpot_(false)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::Init(void)
{
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/BlueDemon.mv1").c_str());

	MV1SetWireFrameDrawFlag(modelId_, TRUE);

	// アニメーションの設定
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		if (i == static_cast<int>(ANIM_TYPE::WAVE)) 
		{
			animationController_->AddInFbx(i, 50.0f, i);
			continue;
		}
		animationController_->AddInFbx(i, 30.0f, i);
	}
	animationController_->Play(static_cast<int>(ANIM_TYPE::WAVE));
	pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, { 0.0f, 0.0f, 0.0f });
}

void Enemy::Update(void)
{
	Move();
	// アニメーション再生
	animationController_->Update();
}

void Enemy::Draw(void)
{
	MV1DrawModel(modelId_);
}

void Enemy::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);

	animationController_->Release();
	delete animationController_;
}

VECTOR Enemy::GetHeadPos(void)
{
	return MV1GetFramePosition(modelId_, MV1SearchFrame(modelId_, "Head"));
}

void Enemy::Move()
{
	// デルタタイム
	float dt = SceneManager::GetInstance().GetDeltaTime();

	VECTOR diff;

	if (isMoveSpot_)
	{
		// yは無視（平面）
		diff = VSub(targetPos_, pos_);
		diff.y = 0.0f;
		float dist = VSize(diff);

		if (dist > STOP_DISTANCE)
		{
			VECTOR moveVec = VScale(VNorm(diff), SPEED * dt);
			pos_ = VAdd(pos_, moveVec);
		}
		else
		{
			// 到着
			isMoveSpot_ = false;
			isCollision_ = false;
		}

		if(isCollision_)
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::RUN));
		}
		else
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::WAVE));
		}
	}
	else
	{
		// ランダム移動（自動徘徊）
		static const VECTOR moveSpots[] = { SPOT1_POS, SPOT2_POS, SPOT3_POS };

		int r = GetRand(_countof(moveSpots) - 1);
		targetPos_ = moveSpots[r];
		targetPos_.y = 0.0f;

		isMoveSpot_ = true;
		return; // 次フレームから追い始める
	}

	// 回転処理（ターゲット方向を向く）
	float angleY = atan2f(diff.x, diff.z);

	// モデルが反対向きなら補正
	const float angleOffset = DX_PI_F;
	float finalAngleY = angleY + angleOffset;

	MATRIX rotMat = MGetRotY(finalAngleY);
	rotMat.m[3][0] = pos_.x;
	rotMat.m[3][1] = pos_.y;
	rotMat.m[3][2] = pos_.z;
	MV1SetMatrix(modelId_, rotMat);
}

// 敵モデルをカメラに向かせる（線形補完 + 行列セットまで）
void Enemy::LookAtCameraAndSetMatrix(int modelId, VECTOR& pos, float& currentAngleY, const VECTOR& cameraPos, float lerpSpeed)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	// デルタタイム
	float dt = SceneManager::GetInstance().GetDeltaTime();
	float t = lerpSpeed * dt; // 補間速度

	// 敵→カメラの方向ベクトル
	VECTOR diff = VSub(cameraPos, pos);
	diff.y = 0.0f; // 水平回転のみ

	VECTOR targetDir = AsoUtility::VNormalize(diff);

	// 現在の角度を atan2 で取得（X/Z平面）
	float targetAngleY = atan2f(targetDir.x, targetDir.z);

	// モデルの正面が逆向きの場合は補正
	const float angleOffset = DX_PI_F;
	targetAngleY += angleOffset;

	// 線形補完
	currentAngleY = AsoUtility::Lerp(currentAngleY, targetAngleY, t);

	// 行列セット
	MATRIX rotMat = MGetRotY(currentAngleY);
	rotMat.m[3][0] = pos.x;
	rotMat.m[3][1] = pos.y;
	rotMat.m[3][2] = pos.z;

	MV1SetMatrix(modelId, rotMat);
}