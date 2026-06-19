#include "SceneManager.h"
#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::Init()
{
	lightPow_ = 0.001f;
}

void Light::InitPointLight()
{
	int lightID = -1;

	pointLightPos_ = { 0.0f, 40.0f, 180.0f };
	/*ChangeLightTypePoint(
		pointLightPos_, 400.0f, 0.000f, 0.001f, 0.000f);*/

	lightID = ChangeLightTypePoint(pointLightPos_, 400.0f, 0.000f, 0.001f, 0.000f);
	SetLightEnable(true);
}

void Light::UpdateLight(void)
{
	bool allInactive = true;

	for (auto& L : lights_)
	{
		if (L.lightPow < MIN_LIGHT_POW)
		{
			// 徐々に暗く
			L.lightPow += minusLight * SceneManager::GetInstance().GetDeltaTime();

			allInactive = false;
		}
		else
		{
			// 真っ暗
			L.lightPow = INIT_LIGHT_POW;
			continue;
		}

		float pow = L.lightPow;

		SetLightPositionHandle(L.handle, L.pos);

		auto col = GetColorF(L.lightPow, L.lightPow, L.lightPow, 1.0f);

		SetLightDifColorHandle(L.handle, col);
		SetLightSpcColorHandle(L.handle, col);

		if (!L.isActive)
		{
			SetLightEnableHandle(L.handle, TRUE);
			L.isActive = true;
		}
	}

	// 二つとも暗いか
	if (allInactive)
	{
		SceneManager::GetInstance().SetCameraNear(100.0f);
	}
}

void Light::DrawLight(void)
{
	SetLightPosition(lights_[0].pos);
	SetLightRangeAtten(400.0f, 0.000001f, lights_[0].lightPow, 0.0000001f);
}

void Light::CreateLight(void)
{
	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		lights_[i].isActive = false;

		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// 元の標準ポイントライトと同じパラメータ
		lights_[i].handle = CreatePointLightHandle(
			pos,
			400.0f,            // Range
			0.000001f,         // Atten0
			0.01f,             // Atten1
			0.0000001f         // Atten2
		);

		lights_[i].lightPow = 0.8f;

		// 最初は無効化
		SetLightEnableHandle(lights_[i].handle, TRUE);
	}
}

void Light::DeleteLight(void)
{
	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		lights_[i].handle = -1;
		lights_[i].isActive = false;
		lights_[i].lightPow = 0.0f;
	}
}

void Light::CreateSetLight()
{
	// 標準ライトを使わない
	SetLightEnable(true);

	// Ambient（環境光）だけ残す
	//SetLightAmbientHandle(0, GetColorF(0.2f, 0.2f, 0.2f));
}

void Light::IsPointLightPow(VECTOR pos)
{
	int answer = 0;

	auto max = lights_[0].lightPow;

	for (int i = 1; i < LIGHT_LENGTH; i++)
	{
		// max以上だったら
		if (max <= lights_[i].lightPow)
		{
			max = lights_[i].lightPow;
			answer = i;
		}
	}

	// 
	lights_[answer].lightPow = 0.0000001f;
	lights_[answer].pos = pos;
}
