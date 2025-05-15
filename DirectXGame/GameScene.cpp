#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	worldTransform_.Initialize();
	camera_.Initialize();

	// 3Dモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("cube");

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			// ブロックの模様
			if ((i + j) % 2 == 0) {
				continue;
			}

			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}

	// デバックカメラ
	debugCamera_ = new DebugCamera(1280, 780);
}

void GameScene::Update() {

	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			// アフィン変換行列の生成
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数bバッファに転送
			worldTransformBlock->TransferMatrix();
		}
	}

	// デバックカメラ処理
	debugCamera_->Update();

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		// デバックカメラの更新
		debugCamera_->Update();
		// デバックカメラのビュー行列
		camera_.matView = debugCamera_->GetCamera().matView;
		// デバックカメラのプロジェクション行列
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();

	} else {
		// ビュープロジェクション行列の転送と更新
		camera_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// ブロック描画
	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	Model::PostDraw();
}

GameScene::~GameScene() {
	// 解放
	delete modelBlock_;

	for (std::vector<WorldTransform*>& worldtransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldtransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	delete debugCamera_;
}