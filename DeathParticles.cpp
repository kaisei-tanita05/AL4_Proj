#include "DeathParticles.h"

using namespace KamataEngine;

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// 02_11_13枚目 モデルとカメラを退避
	model_ = model;
	camera_ = camera;

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		worldTransforms_[i].Initialize();
		worldTransforms_[i].translation_ = position;

		particles_[i].angle = kAngleUnit_ * i; // 各パーティクルに固有の角度を設定
		particles_[i].radius = 0.0f;           // 半径は最初は0
	}

	// 02_11_31枚目
	objectColor_.Initialize();

	// 02_11_31枚目
	color_ = {1, 1, 1, 1};

	counter_ = 0.0f;
	isFinished_ = false;

	center_ = position; // 死亡位置を保存
}

void DeathParticles::Update() {

	// 02_11_27枚目  終了なら何もしない
	if (isFinished_) {
		return;
	}

	// 02_11_26枚目  カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	// 02_11_26枚目  存続時間の上限に達したら
	if (counter_ >= kDuration_) {
		counter_ = kDuration_;
		// 終了扱いにする
		isFinished_ = true;
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		// 角度を回転させる（毎フレーム少しずつ）
		particles_[i].angle += 0.2f; // 回転速度（調整可）

		// 半径を増やす（外に広がる）
		particles_[i].radius += kSpeed_; // kSpeed_ を広がる速度として再利用

		// 角度と半径から新しい座標を計算（Z=0の平面）
		float x = particles_[i].radius * std::cos(particles_[i].angle);
		float y = particles_[i].radius * std::sin(particles_[i].angle);

		// プレイヤー死亡地点を中心にする
		worldTransforms_[i].translation_.x = center_.x + x;
		worldTransforms_[i].translation_.y = center_.y + y;
		worldTransforms_[i].translation_.z = center_.z; // Z方向には動かさない（必要なら加える）
	}

	// 02_11_32枚目
	color_.w = std::clamp(1.0f - counter_ / kDuration_, 0.0f, 1.0f);

	// 02_11_32枚目 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);

	// 02_11_12枚目
	for (auto& worldTransform : worldTransforms_) {
		// ワールド行列更新（アフィン変換～DirectXに転送）
		upData->WorldTransformUpData(worldTransform);
	}
}

void DeathParticles::Draw() {

	// 02_11_27枚目  終了なら何もしない
	if (isFinished_) {
		return;
	}

	// 02_11_13枚目
	for (auto& worldTransform : worldTransforms_) {
		// 02_11_33枚目で&objectColor_を追加
		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
}