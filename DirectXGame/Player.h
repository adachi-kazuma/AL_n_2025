#pragma once
#include "KamataEngine.h"

// player
class Player {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// 変換データ
	KamataEngine::WorldTransform worldTransform_ = {};

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// hundle
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
};