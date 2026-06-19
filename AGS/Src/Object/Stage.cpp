#include <DxLib.h>
#include "../Application.h"
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
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	DrawGraph(0, 0, skyImg, true);

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(modelId_);
	MV1DrawModel(goalModelId_);
	MV1DrawModel(backModelId_);
}

void Stage::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);
	MV1DeleteModel(goalModelId_);
	MV1DeleteModel(backModelId_);
}
