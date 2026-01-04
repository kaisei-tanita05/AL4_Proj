#pragma once
#include "HitEffect.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Math.h"
#include "UpData.h"
#include <list>


// 02_10 20枚目
class Player;
class GameScene;
class EnemyBullet;
    /// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	// 02_15 13枚目 振るまい
	enum class Behavior {
		kUnknown = -1, // 無効な状態
		kWalk,         // 歩行状態
		kDefeated,     // やられ状態
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="camera_"></param>
	/// <param name="position"></param>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void UpDate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 02_10 スライド14枚目
	AABB GetAABB();
	// 02_10 スライド14枚目 ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	// 02_10 スライド20枚目 衝突応答
	void OnCollision(const Player* player);

	void OnCollision2(const EnemyBullet* bullet);

	// 02_15 6枚目
	bool IsDead() const { return isDead_; }

	// 02_15 20枚目
	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

	// 02_16 19
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// 位置を設定するメソッドを追加
	void SetPosition(const KamataEngine::Vector3& position) { position_ = position; }

	// 位置を取得するメソッドを追加
	const KamataEngine::Vector3& GetPosition() const { return position_; }

	void SetStop(bool flag) { isStop_ = flag; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	UpData* upData = nullptr;

	// テクスチャハンドル
	//  uint32_t textureHandle_ = 0u;

	KamataEngine::Camera* camera_ = nullptr;

	// 02_09 15枚目
	static inline const float kWalkSpeed = 0.02f;
	// 02_09 15枚目
	KamataEngine::Vector3 velocity_ = {};

	// 02_09 19枚目
	// 最初の角度
	static inline const float kWalkMotionAngleStart = -10.0f;

	// 02_09 19枚目
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 40.0f;

	// 02_09 19枚目
	static inline const float kWalkMotionTime = 1.0f;
	// 02_09 20枚目
	float walkTimer = 0.0f;

	// 02_10 14枚目 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	KamataEngine::Vector3 position_; // 敵の位置を保持するメンバ変数

	//死亡フラグ
	bool isDead_ = false;

	// 02_15 13枚目
	Behavior behavior_ = Behavior::kWalk;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	//死んだときの演出
	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f; // カウンター

	// 02_15 20枚目
	bool isCollisionDisabled_ = false;

	//ゲームシーンで動かすために含める
	GameScene* gameScene_ = nullptr;

	// 弾を撃つ間隔
	static inline const float kShootInterval = 2.0f;
	float shootTimer_ = 0.0f;

	bool isStop_ = false;
};
