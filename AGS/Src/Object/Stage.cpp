#include <DxLib.h>
#include <memory>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"

#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
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
	MV1SetScale(modelId_, { 1.0f, 1.0f, 1.0f });
	//MV1SetScale(backModelId_, { 1.0f, 1.0f, 1.0f });

	// ３Ｄモデルの位置(引数は、３Ｄ座標)
	MV1SetPosition(modelId_, STAGE_INIT_POS);
	MV1SetPosition(goalModelId_, GOAL_INIT_POS);
	//MV1SetPosition(backModelId_, { 0.0f, 0.0f, 0.0f });

	// ３Ｄモデルの向き(引数は、x, y, zの回転量。単位はラジアン。)
	MV1SetRotationXYZ(modelId_, { 0.0f, 0.0f, 0.0f });
	MV1SetRotationXYZ(goalModelId_, { 0.0f, 0.0f, 0.0f });

	// 衝突判定情報(コライダ)の作成
	MV1SetupCollInfo(modelId_);
	MV1SetupCollInfo(goalModelId_);

	SetUseBackCulling(FALSE);

	// シェーダ
	lightPos_ = VGet(0, 0, 0);
	lightRadius_ = 0.0f;
	isExpand_ = false;

	vertexMaterial_ = std::make_unique<ModelMaterial>(
		"LightVS.cso", 1,
		"LightPS.cso", 1);

	time_ = 1.0f;

	vertexMaterial_->AddConstBufPS({ 0.0f, 0.0f, 0.0f, time_ });

	vertexRenderer_ = std::make_unique<ModelRenderer>(modelId_, *vertexMaterial_);
}

void Stage::Update(void)
{
	if (isExpand_)
	{
		lightRadius_ += 600.0f * SceneManager::GetInstance().GetDeltaTime() * 0.05f;

		if (lightRadius_ > 3000.0f)
		{
			lightRadius_ = 3000.0f;
			isExpand_ = false;
		}
	}

	vertexMaterial_->SetConstBufPS(0, { lightPos_.x, lightPos_.y, lightPos_.z, lightRadius_ });
}

void Stage::Draw(void)
{
	DrawGraph(0, 0, skyImg, true);

	// ロードされた３Ｄモデルを画面に描画
	vertexRenderer_->Draw();
	////MV1DrawModel(modelId_);
	//MV1DrawModel(goalModelId_);
	//MV1DrawModel(backModelId_);
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
	isExpand_ = true;
}
