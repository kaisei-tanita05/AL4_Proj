#include <Windows.h>
#include <KamataEngine.h>
#include "GameScene.h"
#include "GameClear.h"
#include "GameOver.h"
#include "Player.h"
#include "TitleScene.h"
#include "Operate.h"


	using namespace KamataEngine;


	TitleScene* titleScene = nullptr;
    GameScene* gameScene = nullptr;
    GameOver* gameOverScene = nullptr;
    GameClear* gameClearScene = nullptr;
    Operate* operateScene = nullptr;

	// 02_12 25枚目(Scene sceneまで)
	enum class Scene {
		kUnknown = 0,
		kTitle,
		kOperate,
		kGame,
		kGameOver,
		kGameClear,
	};

	// 現在シーン（型）
    Scene scene = Scene::kUnknown;

	void ChangeScene() {

		switch (scene) {
		case Scene::kTitle:
			if (titleScene->IsFinished()) {
				// シーン変更
				scene = Scene::kOperate;
				delete titleScene;
				titleScene = nullptr;
				operateScene = new Operate;
				operateScene->Initialize();
			}
			break;

		case Scene::kOperate:
			if (operateScene->IsFinished()) {
				// シーン変更
				scene = Scene::kGame;
				delete operateScene;
				operateScene = nullptr;
				gameScene = new GameScene;
				gameScene->Initialize();
			}
			break;

		case Scene::kGame:
			// 02_12 30枚目
			if (gameScene->IsFinished()) {
				Player* player_ = gameScene->GetPlayer();

				// ポーズメニューからリトライ or タイトル選択
				if (gameScene->IsPauseActive()) {
					if (gameScene->GetPauseSelection() == 0) {
						// リトライ
						delete gameScene;
						gameScene = new GameScene;
						gameScene->Initialize();
					} else if (gameScene->GetPauseSelection() == 1) {
						// タイトル戻り
						scene = Scene::kTitle;
						delete gameScene;
						gameScene = nullptr;
						titleScene = new TitleScene;
						titleScene->Initialize();
					}
				}
				// 死亡 or ゴール時の遷移
				else {
					if (player_->IsDead()) {
						scene = Scene::kGameOver;
						delete gameScene;
						gameScene = nullptr;
						gameOverScene = new GameOver;
						gameOverScene->Initialize();
					}
				}
			}
			break;

		case Scene::kGameOver:
			if (gameOverScene->IsFinished()) {
				scene = Scene::kTitle;
				delete gameOverScene;
				gameOverScene = nullptr;
				titleScene = new TitleScene;
				titleScene->Initialize();
			}

			break;

		case Scene::kGameClear:
			
			break;
		}
	}

	// 02_12 31枚目
    void UpDataScene() {

	    switch (scene) {
	    case Scene::kTitle:
		    titleScene->Update();
		    break;
	    case Scene::kOperate:
		    operateScene->Update();
		    break;
	    case Scene::kGame:
		    gameScene->Update();
		    break;
	    case Scene::kGameOver:
		    gameOverScene->Update();
		    break;
	    case Scene::kGameClear:
		  //  gameClearScene->Update();
		    break;
	    }
    }

    // 02_12 32枚目
    void DrawScene() {
	    switch (scene) {
	    case Scene::kTitle:
		    titleScene->Draw();
		    break;
	    case Scene::kOperate:
		    operateScene->Draw();
		    break;
	    case Scene::kGame:
		    gameScene->Draw();
		    break;
	    case Scene::kGameOver:
		    gameOverScene->Draw();
		    break;
	    case Scene::kGameClear:
		   // gameClearScene->Draw();
		    break;
	    }
    }

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize();

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	//メインループ
	while (true) 
	{

		

		//エンジンの更新
		if (KamataEngine::Update()) 
		{
			break;
		}

		// シーン切り替え
		ChangeScene();

		// 現在シーン更新
		UpDataScene();

		//描画更新
		dxCommon->PreDraw();

		//ゲームシーンの描画
		DrawScene();

		//描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの解放
	// ゲームシーンの解放
	// 02_12 35枚目 各種解放
	delete titleScene;
	delete gameScene;
	delete gameOverScene;
	delete gameClearScene;
	delete operateScene;

	// nullptrの代入
	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}
