#include <Windows.h>
#include <KamataEngine.h>
#include "GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	

	using namespace KamataEngine;

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	GameScene* gameScene = new GameScene();

	gameScene->Initialize();
	KamataEngine::Initialize();


	//メインループ
	while (true) 
	{

		

		//エンジンの更新
		if (KamataEngine::Update()) 
		{
			break;
		}

		//ゲームシーンの更新
		gameScene->Update();

		//描画更新
		dxCommon->PreDraw();

		//ゲームシーンの描画
		gameScene->Draw();

		//描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの解放
	delete gameScene;

	// nullptrの代入
	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}
