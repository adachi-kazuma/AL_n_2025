#include "Skydome.h"
#include "cassert"
void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) { 

	assert(model);
	model_ = model;
	assert(camera);
	camera_ = camera;

	worldTransform_.Initialize();

	/*modelSkydome_ = Model::CreateFromOBJ("Skydome", true);*/
}

void Skydome::Update() { 
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() {
	model_->Draw(worldTransform_, *camera_); }

Skydome::~Skydome() { 
	delete modelSkydome_; 
}
