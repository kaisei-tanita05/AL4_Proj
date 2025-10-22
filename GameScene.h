#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "HitEffect.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "UpData.h"
#include <KamataEngine.h>

//ゲームシーン
class GameScene 
{
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	~GameScene();

	void GenerateBlocks();

	// 02_10 16枚目 衝突判定と応答
	void CheckAllCollisions();

	// 02_12 9枚目
	void ChangePhase();

	// 02_12 26枚目	デスフラグのgetter
	bool IsFinished() const { return finished_; }

	// エフェクトを生成
	void CreateHitEffect(const Vector3& position);

	// 02_10 10枚目 ワールド座標を取得
	Vector3 GetWorldPosition() const;

	// AABBを取得
	AABB GetAABB();

	Player* GetPlayer() const { return player_; } // プレイヤー取得関数を追加

	bool IsPauseActive() const { return pauseMenuActive_; }
	int GetPauseSelection() const { return pauseSelection_; }

	private:
	// 02_12 4枚目 ゲームのフェーズ（型）
	enum class Phase {
		kFadeIn,  // フェードイン 02_13 28枚目で追加
		kPlay,    // ゲームプレイ
		kDeath,   // デス演出
		kFadeOut, // フェードアウト 02_13 28枚目で追加
		kClear,   // クリア
	};

	// 02_12 4枚目 ゲームの現在フェーズ（変数）
	Phase phase_;

	////テクスチャーハンドル
	uint32_t textureHandle_ = 0;

	Sprite* sprite_ = nullptr;

	//////3Dモデル
	Model* model_ = nullptr;

	// ブロックの3Dモデル
	Model* blockModel_ = nullptr;

	Model* goalModel_ = nullptr;

	WorldTransform worldTransform_;

	////カメラ
	Camera camera_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	std::vector<std::vector<MapChipType>> blockTypes_;

	// kTrap2ブロックの可視状態を記録する2次元配列
	std::vector<std::vector<bool>> trap2Visibility_;

	DebugCamera* debugCamera_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	// 02_09 10枚目 エネミークラス
	//Enemy* enemy_ = nullptr;

	// Math* math_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	Model* modelPlayer_ = nullptr;

	Model* modelAttack_ = nullptr;

	Model* enemy_model_ = nullptr;

	Model* modelGoal_ = nullptr;

	Model* modelArrow_ = nullptr;

	WorldTransform worldTransformArrow_;

	MapChipField* mapChipField_;

	CameraController* CController_ = nullptr;

	std::list<Enemy*> enemies_;

	DeathParticles* deathParticles_ = nullptr;

	// 02_11 16枚目
	Model* deathParticle_model_ = nullptr;

	// 02_12 26枚目
	bool finished_ = false;

	// 02_13 28枚目
	Fade* fade_ = nullptr;

	UpData* upData = nullptr;

	// 02_16 17枚目
	std::list<HitEffect*> hitEffects_;

	// 02_16
	Model* particle_model_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// ポーズメニュー作成
	// ポーズメニュー作成
	bool pauseMenuActive_ = false;

	// メニュー選択
	int pauseSelection_ = 0;

	Sprite* pauseMenuBackGround_ = nullptr;

	Sprite* pauseMenu_ = nullptr;

	Sprite* pauseMenuRetryButton1_ = nullptr;

	Sprite* pauseMenuRetryButton2_ = nullptr;

	Sprite* pauseMenuTitleButton1_ = nullptr;

	Sprite* pauseMenuTitleButton2_ = nullptr;

	Sprite* pauseMenuBackGame_ = nullptr;

	Sprite* pauseMenuBackGame2_ = nullptr;

	Sprite* chooseTexture_ = nullptr;

	// テクスチャハンドル
	int32_t pauseMenuBackgroundHandle_ = 0;

	int32_t pauseMenuHandle_ = 0;

	int32_t pauseMenuRestartButton1Handle_ = 0;
	int32_t pauseMenuRestartButton2Handle_ = 0;

	int32_t pauseMenuTitleButton1Handle_ = 0;
	int32_t pauseMenuTitleButton2Handle_ = 0;

	int32_t pauseMenuBackGameHandle_ = 0;
	int32_t pauseMenuBackGame2Handle_ = 0;

	int32_t chooseTextureHandle_ = 0;
};