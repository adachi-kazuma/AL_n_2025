#include "Player.h"

using namespace KamataEngine;

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {

	// ポインタチェック
	assert(model);

	// メンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	assert(camera);
	worldTransform_.Initialize();
}

void Player::Update() {

	// 行列を転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}