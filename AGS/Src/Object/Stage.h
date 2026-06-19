#pragma once
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
private:

	static constexpr VECTOR STAGE_INIT_POS = { 0.0f, -100.0f, 0.0f };
	static constexpr VECTOR GOAL_INIT_POS = { -2500.0f, 0.0f, 9050.0f };

	// ３ＤモデルのハンドルID
	int modelId_;
	int goalModelId_;

	int backModelId_;

	int skyImg;
};

