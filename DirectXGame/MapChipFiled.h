#include "KamataEngine.h"
#include <KamataEngine.h>

#pragma once

enum class MapChipType {

	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {

	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {

public:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

	//インデックスセット
	struct  IndexSet 
	{
		uint32_t xIndex;
		uint32_t yIndex;
	};
	//座標からマップチップ番号
	IndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position);

	//範囲矩形
	struct  Rect {
		float left; // 左
		float right; //右
		float bottom; //下
		float top; //上
	};

	//ブロックの範囲取得
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

public:
	uint32_t GetNumBlockVertical() const { return kNumBlockVertical; }
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	// マップチップリセット関数
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);
};