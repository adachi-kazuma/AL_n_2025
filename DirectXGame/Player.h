#pragma once
#include "KamataEngine.h"
#include "CameraController.h"
#include "MapChipFiled.h"
#include "MyMath.h"
    // player
using namespace KamataEngine;


class MapChipField;
class Enemy;
class Player {
public:
	enum class LRdirection {
		kRight,
		kLeft,
	};

	// LRDirection
	LRdirection lrDirection_ = LRdirection::kRight;
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	// 更新
	void Update();

	// 描画
	void Draw();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	KamataEngine::Vector3 velocity_ = {};
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; };
	//マップチップ
	void SetMapChipField(MapChipField* mapChipField){ mapChipField_ = mapChipField;}
	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Enemy* enemy);



private:
	// 変換データ
	KamataEngine::WorldTransform worldTransform_ = {};
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// hundle
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
	// 加速度
	static inline const float kAcceleration = 0.05f;
	static inline const float kAttenuation = 0.1f;

	// 速度制限
	static inline const float kLimitRunSpeed = 2.0f;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 設置状態フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 10.0f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 1.0f;

	//マップチップfeled
	MapChipField* mapChipField_ = nullptr;

	//キャラクター
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	//マップチップの当たり判定
	struct  CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 Move;
	};

	//1移動
	void InputMove();

	//2マップチップ衝突判定
	void CheckMapCollision(CollisionMapInfo& info);
	//上方向
	void CheckMapCollisionUp(CollisionMapInfo& info);
	// 下方向
	void CheckMapCollisionDown(CollisionMapInfo& info);
	// 右上方向
	void CheckMapCollisionRight(CollisionMapInfo& info);
	// 左上方向
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	//3判定結果を反映
	void CheckMapMove(const CollisionMapInfo& info);

	//4天井に接触
	void CheckMapCeiling(const CollisionMapInfo& info);

	//5壁に接触
	void CheckMapWall(const CollisionMapInfo& info);
	//6設置状態の切り替え
	void CheckMapLanding(const CollisionMapInfo& info);

	//7旋回制御
	void AnimateTurn();

	//角
	enum  Corner {
		kRightBottom, //右下
		KLeftBottom, //左シフト
		KRightTop,
		KLeftTop,

		kNumCorner //要素数
	};
	//指定した角
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	//隙間
	static inline const float kBlank = 0.1f;

	//着地時の速度減退
	static inline const float kAttenuationLanding = 0.5f;
	// 微小
	static inline const float kGroundSearchHeight = 0.1f;
	//着地時の速度減退
	static inline const float kAttenuationWall = 0.5f;
};
