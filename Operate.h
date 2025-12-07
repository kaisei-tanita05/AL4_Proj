#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "UpData.h"

class Operate {
public:
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	~Operate();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	// ビュープロジェクション
	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransformOparate_;

	KamataEngine::WorldTransform worldTransformSpace_;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	// 02_13 12枚目
	Fade* fade_ = nullptr;

	// 02_13 27枚目 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	UpData* upData = nullptr;

	KamataEngine::Model* modelArrow_ = nullptr;

	KamataEngine::Model* modelSpace_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	float arrowScaleTimer_ = 0.0f;     // 拡縮用のタイマー
	float arrowBaseScale_ = 3.0f;      // 基本スケール
	float arrowScaleAmplitude_ = 1.0f; // 拡縮の振れ幅
	float arrowScaleSpeed_ = 2.0f;     // 拡縮の速さ（周期)
};
