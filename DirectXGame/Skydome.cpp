#include "Skydome.h"
#include "cassert"
using namespace KamataEngine;
void Skydome::Initialize(Model* model, Camera* camera) {
	assert(model);
	model_ = model;
	assert(camera);
	camera_ = camera;
	worldTransform_.Initialize();
};
void Skydome::Update() {};
void Skydome::Draw() { model_->Draw(worldTransform_, *camera_); };