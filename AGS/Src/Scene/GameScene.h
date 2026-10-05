#pragma once
#include <DxLib.h>
#include "../Application.h"
#include "SceneBase.h"

class SceneManager;
class Stage;
class Enemy;

class GameScene : public SceneBase
{
public:
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	bool CollisionCamera(void);

private:
	constexpr static float GRAVITY = 0.08f;
	constexpr static float CIRCLE_RADIUS = 80.0f;

	// ステージインスタンス
	Stage* stage_; 

	Enemy* enemy_;

	VECTOR circlePos_ = { 0,0,0 };
	float pow_ = 20.0f;
	float powdddd_ = 20.0f;

	float lightPow_ = 0.0f;

	bool isCollision_ = false;

	// 移動方向
	VECTOR moveDir_;

	VECTOR movePow;

	bool isShooting = false;

	int seId_;
	int footSeId_;
	int enemyFootSeId_;

	VECTOR startPos_;  // 発射開始位置
	float maxDistance = 1000.0f;  // これ以上飛んだら停止
	bool isMove;

	bool isEnemyPlayerCollision = false;

	// 当たり判定
	void Collision(void);

	// 自身とステージの当たり判定
	bool StageCollision(void);
	// 自身と敵の当たり判定
	bool EnemyCollision(void);

	void CircleCollisionSet(void);
	void CircleMove(void);
	
	void DistanceSoundVolume(const VECTOR& soundPos, const VECTOR& soundPos2, int soundId);

	// 追加するメンバ変数（クラス内）
	VECTOR vel = VGet(0.0f, 0.0f, 0.0f); // 初期速度
	float gravity = -0.5f;             // 重力（Y方向下向き）
	float bounce = 0.6f;               // 反発係数（0〜1）小さいほどすぐ止まる
	float friction = 0.98f;            // 空気抵抗 or 摩擦（1.0＝減速なし）

};