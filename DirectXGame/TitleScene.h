#pragma once
#include "KamataEngine.h"
#include"Fade.h"
class TitleScene {
public:
	// シーンのフェーズ
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};
	~TitleScene();
	bool finished_ = false;
	bool IsFinished() const { return finished_; }
	void Update();

	void Initialize();

	void Draw();
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	float rotate = 0.0f;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;

};
