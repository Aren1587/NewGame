#pragma once

#include <DxLib.h>
#include <memory>

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
	std::unique_ptr<ModelMaterial> vertexMaterial_;
	std::unique_ptr<ModelRenderer> vertexRenderer_;

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

	float time_;

	VECTOR lightPos_;
	float lightRadius_;
	float notLightRadius_;
	bool isExpand_;

	void Move();

};
