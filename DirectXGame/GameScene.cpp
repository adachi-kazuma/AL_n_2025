#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;

// デストラクタ
GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete modelSkydome_;
	/*delete enemy_;*/
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockline) {
			delete worldTransformBlock;
		}
	}

	for (Enemy* enemy : enemies_) 
	{
		delete enemy;
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete mapChipFiled_;
	delete cameraController_;
}

// 初期化処理
void GameScene::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	
	// スプライトインスタンスの生成
	model_ = Model::CreateFromOBJ("block", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);
	debugCamera_ = new DebugCamera(1280, 720);

	mapChipFiled_ = new MapChipField;
	mapChipFiled_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	camera_.Initialize();


	// 自キャラにの生成
	player_ = new Player();

	Skydome_ = new Skydome();
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	//敵
	//enemy_ = new Enemy();
	//Vector3 enemyPosition = mapChipFiled_->GetMapChipPositionByIndex(5, 18);

	Vector3 playerPosition = mapChipFiled_->GetMapChipPositionByIndex(1, 18);

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipFiled_);
	//敵
	/*enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);
	enemy_->SetMapChipField(mapChipFiled_);*/
	//敵の福島
	for (int32_t i = 0; i < 3; i++) 
	{
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipFiled_->GetMapChipPositionByIndex(18 + i, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}
	// 背景
	Skydome_->Initialize(modelSkydome_, &camera_);
	CameraController::Rect cameraArea = {12.0f, 100 -12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

}
 

// 更新処理
void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}
	cameraController_->Update();
	debugCamera_->Update();
	CheckAllCollisions();

	//enemy_->Update();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.TransferMatrix();
	} else {
	/*	camera_.UpdateMatrix();*/
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
	}
	//敵
	for (Enemy* enemy : enemies_)
	{
		enemy->Update();
	}

}

// 描画処理
void GameScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// ここに描画処理

	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockline : worldTransformBlocks_) {
		for (KamataEngine::WorldTransform* worldTransformBlock : worldTransformBlockline) {
			if (!worldTransformBlock) {
				continue;
			}
			model_->Draw(*worldTransformBlock, camera_);
		}
	}
	// 自キャラの描画
	player_->Draw();
	//enemy_->Draw();
	Skydome_->Draw();
	// 敵
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// スプライト描画後処理
	Model::PostDraw();
}

void GameScene::CheckAllCollisions() {
	#pragma region 自キャラ
	AABB aabb1, aabb2;
	
	aabb1 = player_->GetAABB();

	for(Enemy* enemy : enemies_) {
		aabb2 = enemy->GetAABB();

		if (IsCollision(aabb1,aabb2)) {
			player_->OnCollision(enemy);
			enemy_->OnCollision(player_);
		}
	}
	

	#pragma endregion


}

void GameScene::GenerateBlocks() {
	const uint32_t kNumBlockVirtical = mapChipFiled_->GetNumBlockVertical();
	const uint32_t kNumBlockHorizon = mapChipFiled_->GetNumBlockHorizontal();

	// const float kBlockWidth = 2.0f;
	// const float kBlockheight = 2.0f;

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {

		worldTransformBlocks_[i].resize(kNumBlockHorizon);
	}
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizon; ++j) {
			if (mapChipFiled_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransform->translation_ = mapChipFiled_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}