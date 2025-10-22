#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "UpData.h"

using namespace KamataEngine;

class GameOver {
public:
	// 02_12 27枚目 シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// チラ見状態
	enum class LookState {
		kIdle,      // 待機（横向き）
		kLookStart, // 振り向き開始
		kLooking,   // 振り向いたまま静止
		kLookEnd    // 振り向きをやめる
	};

	~GameOver();

	void Initialize();

	void Update();

	void Draw();

	// 02_12 26枚目
	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeGameOverMove = 2.0f;

	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransformGameOver_;
	WorldTransform worldTransformPlayer_;
	WorldTransform worldTransformIdiotFace_;

	Model* modelPlayer_ = nullptr;
	Model* modelGameOver_ = nullptr;
	Model* modelIdiotFace_ = nullptr;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	// 02_13 12枚目
	Fade* fade_ = nullptr;

	// 02_13 27枚目 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	UpData* upData = nullptr;

	bool isApproaching_ = true; // 近づいているかどうかのフラグ

	LookState lookState_ = LookState::kIdle;

	float lookTimer_ = 0.0f;                        // チラ見用タイマー
	static inline const float kLookCycle = 2.0f;    // 1周期あたりの時間（秒）
	static inline const float kMaxLookAngle = 0.4f; // 最大角度（ラジアン）※約23度

	// パラメータ
	static inline const float kLookAngle = 0.8f;     // 約45度
	static inline const float kLookStartTime = 0.2f; // 向きを変えるまでの時間
	static inline const float kLookingTime = 0.5f;   // 見続ける時間
	static inline const float kLookEndTime = 0.3f;   // 元に戻るまでの時間
	static inline const float kIdleCooldownMin = 1.5f;
	static inline const float kIdleCooldownMax = 3.5f;

	float idleCooldown_ = 0.0f; // 次のチラ見までの待機時間
};
