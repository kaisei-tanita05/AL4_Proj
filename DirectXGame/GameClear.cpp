#include "GameClear.h"
#include <numbers>

using namespace KamataEngine;

GameClear::~GameClear() {
	// delete modelPlayer_;
	delete modelTitle_;
	delete fade_;
}

void GameClear::Initialize() {
	// modelTitle_ = Model::CreateFromOBJ("titleFont", true);
	modelClear_ = Model::CreateFromOBJ("ClearFont", true);

	// カメラ初期化
	camera_.Initialize();

	worldTransformGameClear_.Initialize();

	const float kPlayerClear = 2.0f;

	worldTransformGameClear_.scale_ = {kPlayerClear, kPlayerClear, kPlayerClear};

	fade_ = new Fade();
	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 5.0f);
}

void GameClear::Update() {

	// 02_13 27枚目
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeClearMove);

	float angle = counter_ / kTimeClearMove * 2.0f * std::numbers::pi_v<float>;

	// worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	worldTransformGameClear_.translation_.y = std::sin(angle) + 10.0f;

	// ==== 追加：タイトル拡縮演出 ====

	// スケール値をsin波で上下させる
	const float kBaseScale = 2.0f;      // 基本スケール
	const float kScaleAmplitude = 0.3f; // 拡縮の幅（±値）
	const float kScaleSpeed = 2.0f;     // 速度

	float scaleAnim = std::sin(counter_ * kScaleSpeed * std::numbers::pi_v<float>);
	float scaleValue = kBaseScale + scaleAnim * kScaleAmplitude;

	// worldTransformTitle_.scale_ = {scaleValue, scaleValue, scaleValue};

	worldTransformGameClear_.scale_ = {scaleValue, scaleValue, scaleValue};

	camera_.TransferMatrix();

	// upData->WorldTransformUpData(worldTransformTitle_);
	upData->WorldTransformUpData(worldTransformGameClear_);
}

void GameClear::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	// modelTitle_->Draw(worldTransformTitle_, camera_);
	modelClear_->Draw(worldTransformGameClear_, camera_);
	Model::PostDraw();

	// 02_13 13枚目
	fade_->Draw();
}

Vector3 GameClear::GetWorldPosition() const {

	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB GameClear::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}