#pragma once
#include "kamataEngine.h"
#include "Mymath.h"
#include <array>
#include <numbers>
class DeathParticles {

	public:
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// hundle
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
	//存在時間
	static inline const float kDuration = 0.5f;
	//移動の速さ
	static inline const float kSpeed = 0.1f;
	//分割
	static inline const float kAngleUnit = 2.0f * std::numbers::pi_v<float> / kNumParticles;
	//色
	KamataEngine::ObjectColor objectColor_;
	//値
	KamataEngine::Vector4 color_;
	//フラグ
	bool isFinished_ = false;
	float counter_ = 0.0f;
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();


	void Draw();





};







