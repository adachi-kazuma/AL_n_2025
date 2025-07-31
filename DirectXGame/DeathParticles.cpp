#include "DeathParticles.h"
#include "GameScene.h"
#include "algorithm"

using namespace KamataEngine;
using namespace MathUtility;

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	//ワールド変換
	for (WorldTransform& worldTransform : worldTransform_)
	{
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

}
void DeathParticles::Update() {

	for (WorldTransform& worldTransform : worldTransform_) {
		// アフィン変換行列の生成
		worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
		worldTransform_.TransferMatrix();
	}
}