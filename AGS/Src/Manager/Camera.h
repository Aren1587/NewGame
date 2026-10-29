#pragma once

#include <DxLib.h>

class Camera
{
public:
	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT, // 定点カメラ
		FREE, // フリーモード
	};

	// カメラ半径
	static constexpr float CAMERA_RADIUS = 30.0f;

	Camera(void);
	~Camera(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画前のカメラ設定
	void SetBeforeDraw(void);

	// デバッグ
	void DrawDebug(void);

	// 解放
	void Release(void);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	const VECTOR& GetAngle(void) const { return angles_; }
	const VECTOR& GetPos(void) const { return pos_; }

	void SetFarClip(const float& farClip) { farClip_ = farClip; }
	void SetPos(const VECTOR& pos) { pos_ = pos; }
	void SetCollision(const bool& is) { isCollision_ = is; }

	const bool& GetIsMove(void) { return isMove_; }
	void SetIsMove(bool isMove) { isMove_ = isMove; }

	VECTOR Move();                     // 入力から次座標を計算して返す
	void ApplyMove(const VECTOR& newPos); // 確定的に pos_ に反映

private:
	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// 移動方向
	VECTOR moveDir_;

	float farClip_;

	bool isMove_;

	bool isCollision_;

	// カメラモード別の制御処理
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);

};

