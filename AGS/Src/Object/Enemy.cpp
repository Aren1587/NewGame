#include "Enemy.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "Common/AnimationController.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"

namespace
{
	constexpr float RADIUS_MAX_SIZE = 2000.0f;
	constexpr float RADIUS_SIZE = 400.0f;
	constexpr float RADIUS_SPEED = 400.0f;

	constexpr VECTOR SPOT1_POS = { -60.0f, 300.0f, 4775.0f };
	constexpr VECTOR SPOT2_POS = { 0.0f, 300.0f, 0.0f };
	constexpr VECTOR SPOT3_POS = { 3060.0f, 300.0f, 4970.0f };

	constexpr float MOVE_SPEED = 5.0f;

	constexpr float STOP_DISTANCE = 2.0f;

	constexpr float TIME_OFFSET = 0.05f;
}

Enemy::Enemy(void)
	:
	vertexMaterial_(nullptr),
	vertexRenderer_(nullptr),
	animationController_(nullptr),
	pos_(AsoUtility::VECTOR_ZERO),
	angles_(AsoUtility::VECTOR_ZERO),
	modelId_(-1),
	scales_(AsoUtility::VECTOR_ONE),
	moveDir_(AsoUtility::VECTOR_ZERO),
	targetPos_(AsoUtility::VECTOR_ZERO),
	isMoveSpot_(false),
	isCollision_(false),
	time_(0.0f),
	lightPos_(AsoUtility::VECTOR_ZERO),
	lightRadius_(0.0f),
	notLightRadius_(0.0f),
	isExpand_(false)
{
}

Enemy::~Enemy(void)
{
	Release();
}

void Enemy::Init(void)
{
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Enemy/BlueDemon.mv1").c_str());

	MV1SetScale(modelId_, { 1.0f, 1.0f, 1.0f });

	// アニメーションの設定
	animationController_ = std::make_unique<AnimationController>(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		if (i == static_cast<int>(ANIM_TYPE::WAVE)) 
		{
			animationController_->AddInFbx(i, 50.0f, i);
			continue;
		}
		animationController_->AddInFbx(i, 30.0f, i);
	}
	
	pos_ = AsoUtility::VECTOR_ZERO;
	MV1SetPosition(modelId_, pos_);
	MV1SetRotationXYZ(modelId_, AsoUtility::VECTOR_ZERO);

	// シェーダ
	vertexMaterial_ = std::make_unique<ModelMaterial>(
		"EnemyVS.cso", 1,
		"EnemyPS.cso", 2);

	// エネミーのサイズに合わせてライト半径の最大値を調整
	// BlueDemonモデルは小さいので、RADIUS_MAX_SIZEをもっと小さくする
	vertexMaterial_->AddConstBufPS({ 0.0f, 0.0f, 0.0f, lightRadius_ });
	vertexMaterial_->AddConstBufPS({ notLightRadius_, RADIUS_MAX_SIZE, 0.0f, 0.0f });
	vertexMaterial_->AddConstBufVS({ 1, 1, 1.0f, 0.0f });

	vertexRenderer_ = std::make_unique<ModelRenderer>(modelId_, *vertexMaterial_);
}

void Enemy::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime() * TIME_OFFSET;

	Move();
	// アニメーション再生
	animationController_->Update();

	if (isExpand_)
	{
		lightRadius_ += RADIUS_SPEED * SceneManager::GetInstance().GetDeltaTime() * TIME_OFFSET;

		if (lightRadius_ > RADIUS_MAX_SIZE)
		{
			lightRadius_ = RADIUS_MAX_SIZE;
			isExpand_ = false;
		}
	}

	if (lightRadius_ >= RADIUS_SIZE)
	{
		notLightRadius_ += RADIUS_SPEED * SceneManager::GetInstance().GetDeltaTime() * TIME_OFFSET;
	}

	vertexMaterial_->SetConstBufPS(0, { lightPos_.x, lightPos_.y, lightPos_.z, lightRadius_ });
	vertexMaterial_->SetConstBufPS(1, { notLightRadius_, RADIUS_MAX_SIZE, 0.0f, 0.0f });
	vertexMaterial_->SetConstBufVS(0, { 1, 1, 1.0f, time_ });
}

void Enemy::Draw(void)
{
	MV1DrawModel(modelId_);
	//vertexRenderer_->Draw();
}

void Enemy::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);
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
			VECTOR moveVec = VScale(VNorm(diff), MOVE_SPEED * dt);
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

	// モデルが反対向きの場合は補正
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

void Enemy::StartLight(const VECTOR& pos)
{
	lightPos_ = pos;
	lightRadius_ = 0.0f;
	notLightRadius_ = 0.0f;
	isExpand_ = true;
}