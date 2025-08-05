#pragma once
#include "KamataEngine.h"
class TitleScene {
public:
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


};
