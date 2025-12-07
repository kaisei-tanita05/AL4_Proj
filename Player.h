#pragma once
#include <KamataEngine.h>
#include "UpData.h"
#include "Math.h"
#include <algorithm>
#include <cassert>
#include <numbers>
#include "MapChipField.h"
#include "enemyBullet.h"

class MapChipField;

class Enemy;


class Player {

public:

	//左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	//地面ブロックとの当たり判定
	enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };

	//振る舞い
	enum class Behavior { 
		kUnknown = -1,
		kRoot,//通常状態
		kAttack,//攻撃
		kPari, //パリィ
	};

	//攻撃フェーズ
	enum class AttackPhase {
		kUnknown = -1, // 無効な状態
		kAnticipation, // 予備動作
		kAction,       // 前進動作
		kRecovery,     // 余韻動作
	};

	//パリィフェーズ
	enum class PariPahase {
		kUnknown = -1,
		kAnticipation, // 予備動作
		kAction,       // 前進動作
		kRecovery,     // 余韻動作
	};




	void Initialize(KamataEngine::Model* model_, KamataEngine::Model* modelAttack, KamataEngine::Camera* camera_, const KamataEngine::Vector3& position);

	void UpDate();

	void Draw();

	// getter(02_06スライド11枚目で追加)
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// 02_06スライド28枚目で追加
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	// 02_07 スライド4枚目
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// 02_10 10枚目 ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// AABBを取得
	AABB GetAABB();

	// 02_10 21枚目 衝突応答
	void OnCollision(const Enemy* enemy);

	void OnCollision2(const enemyBullet* enemyBullet_);

	// 02_12 11枚目 デスフラグ
	bool IsDead() const { return isDead_; }

	bool IsHit() const { return isHit_; }

	// 通常行動更新
	// 6枚目 通常行動更新
	void BehavoirRootUpdate();

	//8枚目 攻撃行動更新
	void BehaviorAttackUpdate();

	//void BehaviorPariUpData();

	//16枚目 通常行動初期化
	void BehaviorRootInitialize();

	//16枚目 攻撃行動初期化
	void BehaviorAttackInitialize();


	//void BehaviorPariInitialize();

	/// <summary>
	/// プレイヤーの位置を設定
	/// </summary>
	/// <param name="position">設定する位置</param>
	void SetPosition(const KamataEngine::Vector3& position) { worldTransform_.translation_ = position; }

	bool IsAttack() const { return behavior_ == Behavior::kAttack && attackPhase_ == AttackPhase::kAction; }

	void SetDead() { isDead_ = true; }

	void SetHit() { isHit_ = true; }

private:

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Model* modelAttack_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	UpData* upData = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.1f;

	// 02_05 顔の向き
	LRDirection lrDirection_ = LRDirection::kRight;

	// 02_05 非入力時の摩擦係数
	static inline const float kAttenuation = 0.05f;

	// 速度制限
	//  02_05 最高速度
	static inline const float kLimitRunSpeed = 0.3f;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;

	// 02_05 ジャンプ定数もろもろ

	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;

	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;

	// 02_07 マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.04f;

	// 着地時の速度減算衰率
	static inline const float kAttenuationLanding = 0.0f;

	// 02_08スライド21枚目 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;

	// 02_08スライド27枚目 着地時の速度減衰率
	static inline const float kAttenuationWall = 0.2f;

	void InputMove();

	// マップチップとの当たり判定情報
	// 02_07 スライド12枚目
	struct CollisionMapInfo {
		// 天井衝突フラグ
		bool ceiling = false;
		// 着地フラグ
		bool landing = false;
		// 壁接触フラグ
		bool hitWall = false;
		// 移動量
		KamataEngine::Vector3 move;
	};

	// 02_07 スライド13枚目
	void CheckMapCollision(CollisionMapInfo& info);

	// 02_07 スライド14枚目
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	

	// 02_07 スライド17枚目
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	// 02_08スライド14枚目 設置状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 02_08 スライド27枚目 壁接触している場合の処理
	void UpdateOnWall(const CollisionMapInfo& info);

	//デスフラグ
	bool isDead_ = false;

	// playerが見えないブロックにあたった時のフラグ
	bool isHit_ = false;

	// ゴールをしたかのフラグ
	bool isGoal_ = false;

	//振るまい
	Behavior behavior_ = Behavior::kRoot;

	//次の振るまいリクエスト
	Behavior behaviorRequest_ = Behavior::kUnknown;

	//攻撃ギミックの経過時間カウンター
	uint32_t attackParameter_ = 0;

	//攻撃フェーズ
	AttackPhase attackPhase_ = AttackPhase::kUnknown;

	//パリィの経過時間カウンター
	uint32_t pariParameter_ = 0;

	//パリィフェーズ
	PariPahase parikPhase_ = PariPahase::kUnknown;

	//26枚目 予備動作の時間
	static inline const uint32_t kAnticipationTime = 8;
	//26枚目 前進動作の時間
	static inline const uint32_t kActionTime = 5;
	//26枚目 余韻動作の時間
	static inline const uint32_t kRecoveryTime = 12;
	KamataEngine::WorldTransform worldTransformAttack_;

	bool isOnGround_ = false;    // 接地しているかどうか
	
	
};
