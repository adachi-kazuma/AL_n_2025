#pragma once
#include "kamataEngine.h"
#include "Mymath.h"
#include <array>

class DeathParticles {

	public:
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransform_;


	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();





};







