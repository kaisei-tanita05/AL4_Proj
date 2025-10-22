#include "operate.h"

Operate::~Operate() {
	delete modelArrow_;
	delete modelSpace_;
	delete fade_;
}

void Operate::Initialize() {

	modelArrow_ = Model::CreateFromOBJ("Arrow", true);

	modelSpace_ = Model::CreateFromOBJ("Space", true);

	// カメラ初期化
	camera_.Initialize();

	worldTransformOparate_.Initialize();

	worldTransformSpace_.Initialize();

	const float kArrow = 2.0f;

	worldTransformOparate_.scale_ = {kArrow, kArrow, kArrow};

	worldTransformOparate_.translation_.y = -9.0f;

	const float kSpace = 2.0f;

	worldTransformSpace_.scale_ = {kSpace, kSpace, kSpace};

	worldTransformSpace_.translation_.y = -12.0f;

	fade_ = new Fade();
	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 3.0f);
}

void Operate::Update() {

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

	camera_.TransferMatrix();

	camera_.TransferMatrix();

	// ★ 拡縮アニメーション処理
	arrowScaleTimer_ += 0.016f * arrowScaleSpeed_; // タイマー更新（適宜FPSに合わせて調整）
	float scaleOffset = sinf(arrowScaleTimer_) * arrowScaleAmplitude_;
	float currentScale = arrowBaseScale_ + scaleOffset;
	worldTransformOparate_.scale_ = {currentScale, currentScale, currentScale};

	upData->WorldTransformUpData(worldTransformOparate_);

	upData->WorldTransformUpData(worldTransformSpace_);
}

void Operate::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelArrow_->Draw(worldTransformOparate_, camera_);

	modelSpace_->Draw(worldTransformSpace_, camera_);

	Model::PostDraw();

	// 02_13 13枚目
	fade_->Draw();
}