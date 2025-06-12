#pragma once
#include "KamataEngine.h"

class Skydome {
public:

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	//3D
	Skydome* modelSkydome_ = nullptr;

	void  Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	void Update();

	void Draw();

	~Skydome();
};