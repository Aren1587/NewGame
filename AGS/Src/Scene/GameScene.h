#pragma once
#include <DxLib.h>
#include <memory>
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
	// ステージインスタンス
	std::unique_ptr<Stage> stage_;

	std::unique_ptr<Enemy> enemy_;

	VECTOR circlePos_;
	float pow_;

	float lightPow_;

	bool isCollision_;

	// 移動方向
	VECTOR moveDir_;

	VECTOR movePow_;

	bool isShooting_;

	int seId_;
	int footSeId_;
	int enemyFootSeId_;

	VECTOR startPos_;	// 発射開始位置
	bool isMove_;

	// 当たり判定
	void Collision(void);

	// 自身とステージの当たり判定
	bool StageCollision(void);
	// 自身と敵の当たり判定
	bool EnemyCollision(void);

	void CircleCollisionSet(void);
	void CircleMove(void);
	
	void DistanceSoundVolume(const VECTOR& soundPos, const VECTOR& soundPos2, int soundId);
};