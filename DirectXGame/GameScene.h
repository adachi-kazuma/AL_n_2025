#pragma once
#include "KamataEngine.h"
#include <vector>

using namespace KamataEngine;

// ゲームシーン
class GameScene {

public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	~GameScene();

	bool isDebugCameraActive_ = false;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	DebugCamera* debugCamera_ = nullptr;

	// 3Dモデルデータ
	KamataEngine::Model* modelBlock_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
};