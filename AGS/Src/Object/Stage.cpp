#include <DxLib.h>
#include <memory>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "Stage.h"

namespace
{
	static constexpr float RADIUS_MAX_SIZE = 2000.0f;
	static constexpr float RADIUS_SIZE = 400.0f;
	static constexpr float RADIUS_SPEED = 400.0f;

	static constexpr VECTOR STAGE_INIT_POS = { 0.0f, -100.0f, 0.0f };
	static constexpr VECTOR GOAL_INIT_POS = { -2500.0f, 0.0f, 9050.0f };

	const int CONSTANT_BUF_SLOT_BEGIN_VS = 1;
}

Stage::Stage(void)
	:
	lightHitPos_({}),
	modelId_(-1),
	goalModelId_(-1),
	backModelId_(-1),
	skyImg(-1),
	shader_(-1),
	constBufFloat4Size_(0),
	constBuf_(-1),
	shaderVS_(-1),
	constBufFloat4SizeVS_(0),
	constBufVS_(-1),
	goalShader_(-1),
	goalConstBufFloat4Size_(0),
	goalConstBuf_(-1),
	goalShaderVS_(-1),
	goalConstBufFloat4SizeVS_(0),
	goalConstBufVS_(-1),
	lightPos_({}),
	lightRadius_(0.0f),
	notLightRadius_(0.0f),
	isExpand_(false)
{
}

Stage::~Stage(void)
{
	Release();
}

void Stage::Init(void)
{
	// 外部ファイルの３Ｄモデルをロード
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Stage/Stage.mv1").c_str());

	goalModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Stage/Goal.mv1").c_str());

	MV1SetWireFrameDrawFlag(modelId_, TRUE);
	MV1SetWireFrameDrawFlag(goalModelId_, TRUE);

	// ３Ｄモデルの大きさを設定(引数は、x, y, zの倍率)
	MV1SetScale(modelId_, AsoUtility::VECTOR_ONE);

	// ３Ｄモデルの位置(引数は、３Ｄ座標)
	MV1SetPosition(modelId_, STAGE_INIT_POS);
	MV1SetPosition(goalModelId_, GOAL_INIT_POS);

	// ３Ｄモデルの向き(引数は、x, y, zの回転量。単位はラジアン。)
	MV1SetRotationXYZ(modelId_, AsoUtility::VECTOR_ZERO);
	MV1SetRotationXYZ(goalModelId_, AsoUtility::VECTOR_ZERO);

	// 衝突判定情報(コライダ)の作成
	MV1SetupCollInfo(modelId_);
	MV1SetupCollInfo(goalModelId_);

	SetUseBackCulling(FALSE);

	// シェーダ
	vertexMaterial_ = std::make_unique<ModelMaterial>(
		"LightVS.cso", 1,
		"LightPS.cso", 2);

	vertexMaterial_->AddConstBufPS({ 0.0f, 0.0f, 0.0f, lightRadius_ });
	vertexMaterial_->AddConstBufPS({ notLightRadius_, RADIUS_MAX_SIZE, 0.0f, 0.0f });

	vertexRenderer_ = std::make_unique<ModelRenderer>(modelId_, *vertexMaterial_);

	// -------------------

	goalVertexMaterial_ = std::make_unique<ModelMaterial>(
		"GoalVS.cso", 1,
		"GoalPS.cso", 2);

	goalVertexMaterial_->AddConstBufPS({ 0.0f, 0.0f, 0.0f, lightRadius_ });
	goalVertexMaterial_->AddConstBufPS({ notLightRadius_, RADIUS_MAX_SIZE, 0.0f, 0.0f });

	goalVertexRenderer_ = std::make_unique<ModelRenderer>(goalModelId_, *goalVertexMaterial_);
}

void Stage::Update(void)
{
	if (isExpand_)
	{
		lightRadius_ += RADIUS_SPEED * SceneManager::GetInstance().GetDeltaTime() * 0.05f;

		if (lightRadius_ > RADIUS_MAX_SIZE)
		{
			lightRadius_ = RADIUS_MAX_SIZE;
			isExpand_ = false;
		}
	}

	if (lightRadius_ >= RADIUS_SIZE)
	{
		notLightRadius_ += RADIUS_SPEED * SceneManager::GetInstance().GetDeltaTime() * 0.05f;
	}

	vertexMaterial_->SetConstBufPS(0, { lightPos_.x, lightPos_.y, lightPos_.z, lightRadius_ });
	vertexMaterial_->SetConstBufPS(1, { notLightRadius_, RADIUS_MAX_SIZE, 0.0f, 0.0f });

	goalVertexMaterial_->SetConstBufPS(0, { lightPos_.x, lightPos_.y, lightPos_.z, lightRadius_ });
	goalVertexMaterial_->SetConstBufPS(1, { notLightRadius_, RADIUS_MAX_SIZE, 0.0f, 0.0f });
}

void Stage::Draw(void)
{
	// ロードされた３Ｄモデルを画面に描画
	vertexRenderer_->Draw();
	goalVertexRenderer_->Draw();
}

void Stage::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);
	MV1DeleteModel(goalModelId_);
	MV1DeleteModel(backModelId_);
}

void Stage::StartLight(const VECTOR& pos)
{
	lightPos_ = pos;
	lightRadius_ = 0.0f;
	notLightRadius_ = 0.0f;
	isExpand_ = true;
}
