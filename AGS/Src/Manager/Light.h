#pragma once

#include <DxLib.h>

class Light
{
public:
	Light();
	~Light();

	void Init();
	void InitPointLight();
	void UpdateLight();
	void DrawLight();

	void CreateLight();
	void DeleteLight();

	void CreateSetLight();

	void IsPointLightPow(const VECTOR& pos);

	const VECTOR& GetLightPos() { return pointLightPos_; }

private:
	struct LightInfo
	{
		bool isActive;   // ライトを使うかどうか
		int handle;      // ライトハンドル
		float lightPow;
		VECTOR pos;
	};

	// 配列サイズ
	static constexpr int LIGHT_LENGTH = 1;

	// 2つ分用意
	LightInfo lights_[LIGHT_LENGTH];

	VECTOR pointLightPos_;	//ライトの位置

	float minusLight;

	float lightPow_;
};