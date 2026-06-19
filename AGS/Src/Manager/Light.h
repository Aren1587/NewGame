#pragma once

#include <DxLib.h>

class Light
{
public:

	Light();
	~Light();

	static constexpr float MIN_LIGHT_POW = 0.01f;
	static constexpr float INIT_LIGHT_POW = 0.08f;

	// 配列サイズ
	static constexpr int LIGHT_LENGTH = 1;

	void Init();
	void InitPointLight();
	void UpdateLight(void);
	void DrawLight(void);

	void CreateLight(void);
	void DeleteLight(void);

	void CreateSetLight();

	void IsPointLightPow(VECTOR pos);

	VECTOR GetLightPos() { return pointLightPos_; }

private:
	struct LightInfo
	{
		bool isActive;   // ライトを使うかどうか
		int handle;      // ライトハンドル
		float lightPow;
		VECTOR pos;
	};

	// 2つ分用意
	LightInfo lights_[LIGHT_LENGTH];

	VECTOR pointLightPos_;	//ライトの位置

	float minusLight = 0.00004f;

	float lightPow_;
};