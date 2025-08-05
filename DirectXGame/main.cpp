#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

enum class Scene {
	kUnknown = 0,

	kTitle,
	kGame,
};
Scene scene = Scene::kUnknown;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()){
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameScene;
			gameScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}
};
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	}
};
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
};
    // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// エンジンの初期化
	KamataEngine::Initialize(L"GC2D_01_アダチカズマ_AL3");

	// gameSceneのインスタンス
	//GameScene* gameScene = nullptr;
	//シーン

	// gameSceneの初期化
	//gameScene->Initialize();
	//TitleScene* titleScene = nullptr;
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();
	//titleScene->Update();
	//titleScene->Draw();
	
	
	
	

	while (true) {

		// 更新
		if (KamataEngine::Update()) {
			break;
		}

		// 更新
		//gameScene->Update();
		UpdateScene();
		ChangeScene();
		// 描画開始
		dxCommon->PreDraw();

		// 描画
		//gameScene->Draw();
		
		DrawScene();
		// 描画終了
		dxCommon->PostDraw();
	}

	// gameSceneの解放
	delete gameScene;
	// nullptrの代入
	//gameScene = nullptr;

	delete titleScene;
	//delete gameScene;
	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}