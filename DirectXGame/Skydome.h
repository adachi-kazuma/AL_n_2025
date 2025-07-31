#include "KamataEngine.h"

#pragma once
class Skydome {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	void Update();

	void Draw();

private:
	// world変換データ
	KamataEngine ::WorldTransform worldTransform_;
	// モデル
	KamataEngine ::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
};