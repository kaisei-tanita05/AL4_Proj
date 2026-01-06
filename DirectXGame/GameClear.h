#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "UpData.h"



class GameClear {

public:
	// 02_12 27枚目 シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	~GameClear();

	void Initialize();

	void Update();

	void Draw();

	// 02_12 26枚目
	bool IsFinished() const { return finished_; }

	// AABBを取得
	AABB GetAABB();

	KamataEngine::Vector3 GetWorldPosition() const;

private:
	static inline const float kTimeClearMove = 2.0f;

	// ビュープロジェクション
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransformGameClear_;
	KamataEngine::WorldTransform worldTransformTitle_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* modelTitle_ = nullptr;
	// Model* modelPlayer_ = nullptr;

	KamataEngine::Model* modelClear_ = nullptr;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	// 02_13 12枚目
	Fade* fade_ = nullptr;

	// 02_13 27枚目 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	UpData* upData = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};
