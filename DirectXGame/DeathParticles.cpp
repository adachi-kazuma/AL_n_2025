#include "DeathParticles.h"
#include "GameScene.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	//assert(camera);
	camera_ =camera;
	/*assert(model);*/
	model_ = model;



	//ワールド変換
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		worldTransform.Initialize();
		worldTransform.translation_ = position;
		
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}
void DeathParticles::Update() {
	if (isFinished_) {
		return;
	}
	
	
	for (uint32_t i = 0; i < 8;i++) {
		//基本的
		Vector3 velocity = {0.5f, 0, 0};
		//回転角
		float angle = kAngleUnit * i;
		//z周り
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		//基本回転
		velocity = Transform(velocity, matrixRotation);
		//移動
		worldTransforms_[i].translation_ += velocity;
		
		//持続時間になったら
		if (counter_ >= kDuration) {
			counter_ = kDuration;
			isFinished_ = true;
		}
		
	}

	// カウンター
	counter_ += 1.0f / 60.0f;
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
	for (WorldTransform& worldTransform : worldTransforms_) {
		//
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}
}
void DeathParticles::Draw() {
	if (isFinished_) {
		return;
	}
	for (WorldTransform& worldTransform : worldTransforms_) {
		// 3Dモデルを描画
		model_->Draw(worldTransform, * camera_, &objectColor_);
	}
	
}