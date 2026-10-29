#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/ModelMaterial.h"

class ModelRenderer
{

public:

	// コンストラクタ
	ModelRenderer(int& modelId_, ModelMaterial& modelMaterial);

	// デストラクタ
	~ModelRenderer(void);

	// 描画
	void Draw(void);

private:

	// モデルのハンドルID
	int& modelId_;

	// モデルマテリアル
	ModelMaterial& modelMaterial_;

	// シェーダ設定(頂点)
	void SetReserveVS(void);

	// シェーダ設定(ピクセル)
	void SetReservePS(void);

};
