#include "GameOver.h"
#include "Math.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;

GameOver::~GameOver() {

	// delete modelPlayer_;
	// delete modelTitle_;
	delete modelGameOver_;
	delete modelIdiotFace_;
	delete fade_;
}

void GameOver::Initialize() {

	// modelPlayer_ = Model::CreateFromOBJ("player2");
	modelGameOver_ = Model::CreateFromOBJ("gameOverFont");
	modelIdiotFace_ = Model::CreateFromOBJ("idiotFace");

	// カメラ初期化
	camera_.Initialize();

	// const float kPlayerTitle = 2.0f;

	// worldTransformTitle_.Initialize();

	// worldTransformTitle_.scale_ = {kPlayerTitle, kPlayerTitle, kPlayerTitle};

	// const float kPlayerScale = 10.0f;

	// worldTransformPlayer_.Initialize();

	// worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};

	// worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;

	// worldTransformPlayer_.translation_.x = -2.0f;

	// worldTransformPlayer_.translation_.y = -10.0f;

	worldTransformGameOver_.Initialize();

	const float kPlayerGameOver = 2.0f;

	worldTransformGameOver_.scale_ = {kPlayerGameOver, kPlayerGameOver, kPlayerGameOver};

	worldTransformGameOver_.translation_.z = 30.0f; // 遠くに配置

	const float kIdiotFace = 2.0f;

	worldTransformIdiotFace_.Initialize();

	worldTransformIdiotFace_.scale_ = {kIdiotFace, kIdiotFace, kIdiotFace};

	const float kIdiotFaceScale = 10.0f;

	worldTransformIdiotFace_.scale_ = {kIdiotFaceScale, kIdiotFaceScale, kIdiotFaceScale};

	worldTransformIdiotFace_.rotation_.y = 0.95f * std::numbers::pi_v<float>;

	worldTransformIdiotFace_.translation_.x = -2.0f;

	worldTransformIdiotFace_.translation_.y = -10.0f;

	fade_ = new Fade();
	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 5.0f);
}

void GameOver::Update() {

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
	counter_ = std::fmod(counter_, kTimeGameOverMove);

	float angle = counter_ / kTimeGameOverMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformGameOver_.translation_.y = std::sin(angle) + 10.0f;

	// 回転（常に続ける）
	worldTransformGameOver_.rotation_.z += 0.1f;

	// === ▼ 回転接近 ▼ ===
	// 一定距離まで近づいてきたら停止
	if (isApproaching_) {
		// Z軸方向に近づける（例：前方方向 -Z に動く）
		worldTransformGameOver_.translation_.z -= 0.2f;

		// 一定距離まで来たら停止
		if (worldTransformGameOver_.translation_.z <= 10.0f) {
			worldTransformGameOver_.translation_.z = 10.0f;
			isApproaching_ = false;
		}
	}

	// idiotFaceがチラチラこっちを向く動き
	lookTimer_ += 1.0f / 60.0f;                     // 1フレーム分タイマー加算
	lookTimer_ = std::fmod(lookTimer_, kLookCycle); // 周期でループ

	switch (lookState_) {
	case LookState::kIdle:
		idleCooldown_ -= 1.0f / 60.0f;
		if (idleCooldown_ <= 0.0f) {
			lookState_ = LookState::kLookStart;
			lookTimer_ = 0.0f;
		}
		break;

	case LookState::kLookStart: {
		float t = (std::min)(lookTimer_ / kLookStartTime, 1.0f);
		// 横（kLookAngle）→正面（0.0f）へイージング
		worldTransformIdiotFace_.rotation_.y = kLookAngle * (1.0f - t);
		if (lookTimer_ >= kLookStartTime) {
			lookState_ = LookState::kLooking;
			lookTimer_ = 0.0f;
		}
		break;
	}

	case LookState::kLooking:
		worldTransformIdiotFace_.rotation_.y = 0.0f; // 正面を向いて静止
		if (lookTimer_ >= kLookingTime) {
			lookState_ = LookState::kLookEnd;
			lookTimer_ = 0.0f;
		}
		break;

	case LookState::kLookEnd: {
		float t = (std::min)(lookTimer_ / kLookEndTime, 1.0f);
		// 正面 → 横（kLookAngle）へイージング
		worldTransformIdiotFace_.rotation_.y = kLookAngle * t;
		if (lookTimer_ >= kLookEndTime) {
			lookState_ = LookState::kIdle;
			lookTimer_ = 0.0f;

			// 次のチラ見までのクールタイム（ランダム）
			idleCooldown_ = kIdleCooldownMin + (float)(rand()) / RAND_MAX * (kIdleCooldownMax - kIdleCooldownMin);
		}
		break;
	}
	}

	camera_.TransferMatrix();

	upData->WorldTransformUpData(worldTransformGameOver_);

	// アフィン変換～DirectXに転送(タイトル座標)
	// upData->WorldTransformUpData(worldTransformPlayer_);

	upData->WorldTransformUpData(worldTransformIdiotFace_);
}

void GameOver::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelGameOver_->Draw(worldTransformGameOver_, camera_);
	// modelPlayer_->Draw(worldTransformPlayer_, camera_);
	modelIdiotFace_->Draw(worldTransformIdiotFace_, camera_);
	Model::PostDraw();

	// 02_13 13枚目
	fade_->Draw();
}