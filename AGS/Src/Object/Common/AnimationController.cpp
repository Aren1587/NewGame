#include <DxLib.h>
#include <string>
#include "AnimationController.h"
#include "../../Manager/SceneManager.h"



AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;
	playType_ = -1;
}

AnimationController::~AnimationController(void)
{
}


void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;

	Add(type, speed, animation);
}


void AnimationController::AddInFbx(int type, float speed, int animIndex)
{

	Animation animation;
	animation.animIndex = animIndex;

	animation.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 動的配列に追加
		animations_.emplace(type, animation);
	}
}

void AnimationController::Play(int type, bool isLoop)
{
	if (playType_ == type)
	{
		return;
	}

	if (playType_ != -1)
	{
		// モデルからアニメーションを外す
		MV1DetachAnim(modelId_, playAnim_.attachNo);
	}
	// アニメーション種別を変更
	playType_ = type;
	playAnim_ = animations_[type];

	// 初期化
	playAnim_.step = 0.0f;
	isLoop_ = isLoop;
	
	// モデルと同じファイルからアニメーションをアタッチする
	playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
		
	// アニメーション総時間の取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	playAnim_.speed = playAnim_.totalTime / playAnim_.speed;
	
#if 0
	// モデルにアニメーションを付ける
	if (playAnim_.model == -1)
	{
		// モデルと同じファイルからアニメーションをアタッチする
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	}
	else
	{
		// 別のモデルファイルからアニメーションをアタッチする
		// DxModelViewerを確認すること(大体0か1)
		int animIdx = 0;
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	}
#endif
}

void AnimationController::Update(void)
{
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 再生
	playAnim_.step += (deltaTime * playAnim_.speed);

	// アニメーション設定
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);

	// ループ再生
	if (IsEnd())
	{
		playAnim_.step = 0.0f;
	}
}

void AnimationController::Release(void)
{
	// ロードした外部FBXのモデル（アニメーション）のメモリの解放
	//for (const std::pair<int, Animation>& pair : animations_)
	for (auto& pair : animations_)	// 簡潔に書ける方法
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}

	animations_.clear();
}

bool AnimationController::IsEnd(void) const
{
	return  isLoop_ && playAnim_.step > playAnim_.totalTime;
}

bool AnimationController::IsNotLoopEnd(void) const
{
	return  !isLoop_ && playAnim_.step > playAnim_.totalTime;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

void AnimationController::Debug(void)
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", playAnim_.step);
}

void AnimationController::Add(int type, float speed, Animation& animation)
{
}
