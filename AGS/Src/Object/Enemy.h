#pragma once

#include <DxLib.h>
#include <memory>
#include <vector>

class ModelMaterial;
class ModelRenderer;
class AnimationController;

class Enemy
{
public:
	// アニメーション種別
	enum class ANIM_TYPE
	{
		DEATH,
		DUCK,
		HIT_REACT,
		IDLE,
		JUMP,
		JUMP_IDLE,
		JUMP_LAND,
		NO,
		PUNCH,
		RUN,
		WALK,
		WAVE,
		WEAPON,
		YES,
		MAX,
	};

	enum class MOVE_SPOT
	{
		SPOT1,
		SPOT2,
		SPOT3,
		MAX,
	};

	// コンストラクタ
	Enemy(void);
	// デストラクタ
	~Enemy(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void SetTargetPos(VECTOR pos) { targetPos_ = pos; isMoveSpot_ = true; isCollision_ = true; }
	VECTOR GetPos(void) { return pos_; }
	VECTOR GetHeadPos(void);

	void LookAtCameraAndSetMatrix(int modelId, VECTOR& pos, float& currentAngleY, const VECTOR& cameraPos, float lerpSpeed);

	void StartLight(const VECTOR& pos);

private:
	// 4FRAME 用(ボーン影響 4 本まで)
	std::unique_ptr<ModelMaterial> material4_;
	std::unique_ptr<ModelRenderer> renderer4_;

	// 8FRAME 用(ボーン影響 8 本まで)
	std::unique_ptr<ModelMaterial> material8_;
	std::unique_ptr<ModelRenderer> renderer8_;

	// アニメーションコントローラ
	std::unique_ptr<AnimationController> animationController_;

	// モデルのハンドルID
	int modelId_;

	// 座標
	VECTOR pos_;

	// 角度
	VECTOR angles_;

	// 大きさ
	VECTOR scales_;

	// 移動方向
	VECTOR moveDir_;

	VECTOR targetPos_;

	bool isMoveSpot_;

	bool isCollision_;

	// 光の輪
	VECTOR lightPos_;
	float lightRadius_;
	float notLightRadius_;
	bool isExpand_;

	// メッシュごとに 8FRAME 用か
	std::vector<bool> meshUse8_;
	bool hasMesh4_;
	bool hasMesh8_;

	void Move();

	// PS の定数バッファを更新
	void UpdateMaterial(ModelMaterial& material);

	// 指定グループのメッシュだけを表示にする
	void SetMeshGroupVisible(bool use8);
};