#pragma once
#include "UpData.h"
#include <KamataEngine.h>
#include <cstdint>

/// <summary>
/// ヒット演出用エフェクト
/// </summary>
class HitEffect {

public:
	enum class State {
		kSpread, // 拡大中
		kFade,   // フェードアウト中
		kDead    // 死亡
	};

	// 02_16 10枚目(SetModel,SetCamera)
	static void SetModel(KamataEngine::Model* model) { model_ = model; }

	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }

	void Initialize(const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	static HitEffect* Create(const KamataEngine::Vector3& position);

	bool IsDead() const { return state_ == State::kDead; }

private:
	// モデル(借りてくる用)
	static KamataEngine::Model* model_;

	// カメラ(借りてくる用)
	static KamataEngine::Camera* camera_;

	UpData* upData = nullptr;

	// 円のワールドトランスフォーム
	KamataEngine::WorldTransform circleWorldTransform_;

	State state_ = State::kSpread;

	KamataEngine::ObjectColor objectColor_;

	// カウンター
	uint32_t counter_ = 0;

	// 拡大アニメーションの時間
	static inline const uint32_t kSpreadTime = 10;

	// フェードアウトアニメーションの時間
	static inline const uint32_t kFadeTime = 20;

	// 楕円エフェクトの数
	static const inline uint32_t kellipseEffectNum = 2;

	// 楕円のワールドトランスフォーム
	std::array<KamataEngine::WorldTransform, kellipseEffectNum> ellipseWorldTransforms_;
};
