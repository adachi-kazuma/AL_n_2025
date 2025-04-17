#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;
    // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	KamataEngine::Initialize(L"GC2D_01_アダチ_カズマ_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// インスタンス
	GameScene* gameScene = new GameScene();
	// シーン書記官
	gameScene->Initialize();
	//メインループ
	while (true) {
		if (KamataEngine::Update()){
			break;
		}
		// 描画
		dxCommon->PreDraw();
		// シーン　更新
		gameScene->Update();
		// シーン　描画
		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
		
		
	}
	// 解放
	delete gameScene;
		// nullptr
	gameScene = nullptr;
	// エンジン終了
	KamataEngine::Finalize();
	return 0;
}

