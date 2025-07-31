#pragma once
#include "CameraController.h"
#include "KamataEngine.h"
#include "MapChipFiled.h"
#include "Skydome.h"
#include "player.h"
#include <vector>
#include "Enemy.h"

using namespace KamataEngine;

// ゲームシーン
class GameScene {

	MapChipField* mapChipFiled_;
	std::list<Enemy*> enemies_;
	void CheckAllCollisions();

public:
	void GenerateBlocks();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	~GameScene();

	bool isDebugCameraActive_ = false;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	Skydome* Skydome_ = nullptr;
	Player* player_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	CameraController* cameraController_ = nullptr;
	Enemy* enemy_ = nullptr;
	// 3Dモデルデータ
	KamataEngine::Model* modelBlock_ = nullptr;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;

private:
	uint32_t skydomeHandle_ = 0;
};