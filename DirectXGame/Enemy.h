#pragma once
#include "KamataEngine.h"
#include "MapChipFiled.h"
#include "MyMath.h"


using namespace KamataEngine;


class Player;
class MapChipField;
class Enemy {
public:

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	// マップチップ
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;


	void Update();

    void Draw();

	public:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
    //マップチップfeled
    MapChipField* mapChipField_ = nullptr;
	//アニメーションがあったら
	static inline const float kWalkMotionAngleStart = 60.0f;
	static inline const float kWalkMotionAngleEnd =  -60.0f;
	static inline const float kWalkMotionAngleTime = 0.3f;
	//経過時間
	float walkTimer_ = 0.0f;
	Vector3 GetWorldPosition();
	AABB GetAABB();
	// 衝突
	void OnCollision(const Player* player);
	private:
	//補講
	static inline const float kWalkSpeed = 0.1f;
	//補講の速度
	Vector3 velocity_ = {};
};
