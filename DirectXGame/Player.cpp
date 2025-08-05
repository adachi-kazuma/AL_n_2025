#define NOMINMAX
#include "Player.h"
#include "Mymath.h"
#include "cassert"
#include <algorithm>
#include <numbers>
#include "MapChipFiled.h"


//using namespace KamataEngine;
using namespace MathUtility;
void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	// ポインタチェック
	assert(model);
	// メンバ変数に記録
	model_ = model;
	// textureHandle_ = textureHandle;
	camera_ = camera;
	assert(camera);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
Vector3 Player::GetWorldPosition() 
{
	Vector3 worldPos;



	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos; 
}

AABB Player::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;


	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};

	return AABB();
}

void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy;
	//velocity_ += Vector3(0.0f, 1.0f, 0.0f);
	isDead_ = true;
}

    //移動入力
void Player::InputMove() {
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 逆入力はブレーキ
					velocity_.x += (1.0f - kAttenuation);
				}
				// 切り替え
				if (lrDirection_ != LRdirection::kRight) {
					lrDirection_ = LRdirection::kRight;
					// 旋回開始時の角度の記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する。
					turnTimer_ = kTimeTurn;
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 逆入力はブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				// 切り替え
				if (lrDirection_ != LRdirection::kLeft) {
					lrDirection_ = LRdirection::kLeft;
					// 旋回開始時の角度の記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する。
					turnTimer_ = kTimeTurn;
				}
			}

			// acceleration
			velocity_ += acceleration;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {

			velocity_.x *= (1.0f, -kAcceleration);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}

	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}
//2マップ衝突判定
void Player::CheckMapCollision(CollisionMapInfo& info) 
{ 
	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}
//上
void Player::CheckMapCollisionUp(CollisionMapInfo& info)
{
	//上昇あり
	if (info.Move.y <= 0) 
	{
		return;
	}
	//移動4
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.Move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	//真上の当たり判定
	bool hit = false;
	//左上
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	// 隣接セル
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	//右上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	// 隣接セル
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	//ブロックにヒット
	if (hit) {
		// めり込みを排除
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.Move + Vector3(0, kHeight / 2.0f, 0));
		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexsetNow;
		indexsetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexsetNow.yIndex != indexSet.yIndex) {
			// めり込みブロック
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.Move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			// 天井に当たった
			info.ceiling = true;
		}
	}

}
// 下
void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	// 下昇あり
	if (info.Move.y >= 0) {
		return;
	}
	// 移動4
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.Move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真下の当たり判定
	bool hit = false;
	// 左下
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	// 隣接セル
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	
	// 右下の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	//隣接セル
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock)
	{
		hit = true;
	}
	// ブロックにヒット
	if (hit) {
		// めり込みを排除
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.Move + Vector3(0, -kHeight / 2.0f, 0));
		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexsetNow;
		indexsetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		if (indexsetNow.yIndex != indexSet.yIndex) {
			// めり込みブロック
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.Move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			// 床に当たった
			info.landing = true;
		}
	}
}
	// 右
	void Player::CheckMapCollisionRight(CollisionMapInfo & info) {
		// 右移動あり
		if (info.Move.x <= 0) {
			return;
		}
		// 移動4
		std::array<Vector3, kNumCorner> positionsNew;

		for (uint32_t i = 0; i < positionsNew.size(); ++i) {
			positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.Move, static_cast<Corner>(i));
		}
		MapChipType mapChipType;
	    MapChipType mapChipTypeNext;
		// 真上の当たり判定
		bool hit = false;
		// 右上
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KRightTop]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	    mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		// 右上の判定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	    mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	    // 隣接セル
	    if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		    hit = true;
	    }
		// ブロックにヒット
		if (hit) {
			// めり込みを排除
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.Move + Vector3(+kWidth / 2.0f, 0.0));
			// めり込みブロック
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.Move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			// 壁に当たった
			info.hitWall = true;
		}
    }
    // 左
    void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
	    // 左移動あり
	    if (info.Move.x >= 0) {
		    return;
	    }
	    // 移動4
	    std::array<Vector3, kNumCorner> positionsNew;

	    for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		    positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.Move, static_cast<Corner>(i));
	    }
	    MapChipType mapChipType;
	    MapChipType mapChipTypeNext;
	    // 真上の当たり判定
	    bool hit = false;
	    // 左上
	    MapChipField::IndexSet indexSet;
	    indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KLeftTop]);
	    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	    mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex+ 1, indexSet.yIndex);
	    if (mapChipType == MapChipType::kBlock) {
		    hit = true;
	    }
	    // 左上の判定
	    indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KLeftBottom]);
	    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	    mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	    if (mapChipType == MapChipType::kBlock) {
		    hit = true;
	    }
	    // ブロックにヒット
	    if (hit) {
		    // めり込みを排除
		    indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.Move + Vector3(-kWidth / 2.0f, 0.0));
		    // めり込みブロック
		    MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		    info.Move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
		    // 壁に当たった
		    info.hitWall = true;
	    }
    }
    //3判定結果を反映
void Player::CheckMapMove(const CollisionMapInfo& info)
{
 //移動
	worldTransform_.translation_ += info.Move;
}
//4天井に接触
void Player::CheckMapCeiling(const CollisionMapInfo& info) 
{
	//天井に当たった
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}
//壁に接触
void Player::CheckMapWall(const CollisionMapInfo& info) {
	//壁接触による減速
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}
// 6設置状態の切り替え
void Player::CheckMapLanding(const CollisionMapInfo& info) 
{
	if (onGround_) 
	{
		//設置状態の処理
		
		//ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} 
		else 
		{
			// 移動4
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.Move, static_cast<Corner>(i));
			}
			MapChipType mapChipType;
			/*MapChipType mapChipTypeNext;*/
			// 真下の当たり判定
			bool hit = false;
			// 左下
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[KLeftBottom] + Vector3(0, -kGroundSearchHeight,0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, - kGroundSearchHeight,0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			//落下なら空中状態に切り替え
			if (!hit) 
			{
				//空中状態に切り替え
				onGround_ = false;
			}
		}
	}
	else
	{
		//空中状態の処理

		//着地フラグ
		if (info.landing) {
			//着地状態に切り替え
			onGround_ = true;
			//着地にX速度
			velocity_.x *= (1.0f - kAttenuationLanding);
			//Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}

}


// 旋回制御
void Player::AnimateTurn() {
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
	}
	// 左右の字キャラ角度テーブル
	float destinationRotationYTable[] = {
	    std::numbers::pi_v<float> / 2.0f,
	    std::numbers::pi_v<float> * 3.0f / 2.0f,
	};
	// 状態に応じた角度の取得
	float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
	// 字キャラの角度の設定
	worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
}
//指定した角
KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner)
{ 
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}
void Player::Update() {
	InputMove();
	AnimateTurn();

	//2移動量加味
	//衝突情報を加味
	CollisionMapInfo collisionMapInfo;
	//移動量に移動の速度
	collisionMapInfo.Move = velocity_;

	//マップチップ衝突チェック
	CheckMapCollision(collisionMapInfo);
	// 3判定結果を反映
	CheckMapMove(collisionMapInfo);
	//4天井に接触している
	CheckMapCeiling(collisionMapInfo);
	//5壁に接触している
	CheckMapWall(collisionMapInfo);
	//6設置状態
	CheckMapLanding(collisionMapInfo);
	
	// アフィン変換行列の生成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 定数bバッファに転送
	worldTransform_.TransferMatrix();
}
void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
}