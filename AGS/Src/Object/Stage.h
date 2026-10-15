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

	void StartLight(const VECTOR& pos);

private:
	std::unique_ptr<ModelMaterial> vertexMaterial_;
	std::unique_ptr<ModelRenderer> vertexRenderer_;

	std::unique_ptr<ModelMaterial> goalVertexMaterial_;
	std::unique_ptr<ModelRenderer> goalVertexRenderer_;

	VECTOR lightHitPos_;

	// ３ＤモデルのハンドルID
	int modelId_;
	int goalModelId_;

	int backModelId_;

	int skyImg;

#pragma region シェーダ系
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
	//------
	// シェーダ
	int goalShader_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int goalConstBufFloat4Size_;

	// 定数バッファハンドル
	int goalConstBuf_;

	// シェーダ
	int goalShaderVS_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int goalConstBufFloat4SizeVS_;

	// 定数バッファハンドル
	int goalConstBufVS_;
#pragma endregion

	VECTOR lightPos_;
	float lightRadius_;
	float notLightRadius_;
	bool isExpand_;

};

