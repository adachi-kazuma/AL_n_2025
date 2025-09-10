#define NOMINMAX
#include "Enemy.h"
#include "Mymath.h"
#include "cassert"
#include <algorithm>
#include <numbers>
#include "MapChipFiled.h"

using namespace KamataEngine;
using namespace MathUtility;

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,  const Vector3& position) {
	// モデル・カメラのポインタを保存
	model_ = model;
	camera_ = camera;

	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();

	// 初期座標
	//worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	//速度設定
	velocity_ = {-kWalkSpeed, 0, 0};
	//アニメーション
	walkTimer_ = 0.0f;
}

void Enemy::Update() 
{
	// 移動
	worldTransform_.translation_ += velocity_;

	walkTimer_ += 1.0f / 60.0f;
	//回転
	worldTransform_.rotation_.x = std::sin(walkTimer_ * 5.0f);
	//アニメーション

	// アフィン変換行列の生成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	
	
}

void Enemy::Draw() {
	if (model_ != nullptr) {
		model_->Draw(worldTransform_, *camera_);
	}
}
Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::OnCollision(const Player* player)
{ 
	(void)player; 
}
AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
