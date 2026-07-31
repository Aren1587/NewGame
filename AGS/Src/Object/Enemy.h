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

	enum class MOVE_TYPE
	{

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

	static constexpr float RADIUS_MAX_SIZE = 6000.0f;
	static constexpr float RADIUS_SIZE = 200.0f;
	static constexpr float RADIUS_SPEED = 600.0f;

	std::unique_ptr<ModelMaterial> vertexMaterial_;
	std::unique_ptr<ModelRenderer> vertexRenderer_;

	static constexpr VECTOR SPOT1_POS = { -60.0f, 300.0f, 4775.0f };
	static constexpr VECTOR SPOT2_POS = { 0.0f, 300.0f, 0.0f };
	static constexpr VECTOR SPOT3_POS = { 3060.0f, 300.0f, 4970.0f };

	static constexpr float SPEED = 5.0f;

	static constexpr float STOP_DISTANCE = 2.0f;

	// アニメーションコントローラ
	AnimationController* animationController_;

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

	bool isCollision_ = false;

	VECTOR lightPos_;
	float lightRadius_;
	float notLightRadius_;
	bool isExpand_;

	void Move();
};

