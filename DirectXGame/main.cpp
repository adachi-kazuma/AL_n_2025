#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// エンジンの初期化
	KamataEngine::Initialize(L"GC2D_01_アダチカズマ_AL3");

	// gameSceneのインスタンス
	GameScene* gameScene = new GameScene();

	// gameSceneの初期化
	gameScene->Initialize();

	while (true) {

		// 更新
		if (KamataEngine::Update()) {
			break;
		}

		// 更新
		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();

		// 描画
		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// gameSceneの解放
	delete gameScene;
	// nullptrの代入
	gameScene = nullptr;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}