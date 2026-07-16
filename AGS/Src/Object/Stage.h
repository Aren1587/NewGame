#pragma once
#include <memory>

class ModelMaterial;
class ModelRenderer;

class Stage
{
public:
	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetModelId() { return modelId_; }
	int GetGoalModelId() { return goalModelId_; }

	void SetLightHitPos(VECTOR pos) { lightHitPos_ = pos; }
private:

	std::unique_ptr<ModelMaterial> vertexMaterial_;
	std::unique_ptr<ModelRenderer> vertexRenderer_;

	static constexpr VECTOR STAGE_INIT_POS = { 0.0f, -100.0f, 0.0f };
	static constexpr VECTOR GOAL_INIT_POS = { -2500.0f, 0.0f, 9050.0f };

	VECTOR lightHitPos_;

	// ３ＤモデルのハンドルID
	int modelId_;
	int goalModelId_;

	int backModelId_;

	int skyImg;

	// 時間
	float time_;

	float dissolveTime_;

	// シェーダ
	int shader_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4Size_;

	// 定数バッファハンドル
	int constBuf_;


	// シェーダ
	int shaderVS_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4SizeVS_;

	// 定数バッファハンドル
	int constBufVS_;

	const int CONSTANT_BUF_SLOT_BEGIN_VS = 1;
};

