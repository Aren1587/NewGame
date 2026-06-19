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

	static constexpr float MOVE_POW = 3.0f;

	// カメラスピード（移動）
	static constexpr float SPEED_MOVE = 10.0f;

	// カメラスピード(度)
	static constexpr float SPEED_ANGLE_DEG = 1.0f;

	// カメラの初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f, 300.0f, -500.0f };

	// カメラの初期角度
	static constexpr VECTOR DEFAULT_ANGLES = { 15.0f * DX_PI_F / 180.0f, 0.0f, 0.0f };

	//// カメラの初期位置
	//static constexpr VECTOR DEFAULT_POS = { 360.0f, 320.0f, -360.0f };

	//// カメラの初期角度
	//static constexpr VECTOR DEFAULT_ANGLES = { 26.0f * DX_PI_F / 180.0f, -28.0f * DX_PI_F / 180.0f, 0.0f };

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 3.0f;

	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 15000.0f;

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
	void SetFarClip(float farClip) { farClip_ = farClip; }
	void SetPos(VECTOR pos) { pos_ = pos; }
	void SetCollision(bool is) { isCollision_ = is; }

	bool GetIsMove() { return isMove_; }
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

	float farClip_ = 0;

	bool isMove_;

	// カメラモード別の制御処理
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);

	bool isCollision_ = false;
};

