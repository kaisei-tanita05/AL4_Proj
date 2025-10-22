#include "GameScene.h"

using namespace KamataEngine;

void GameScene::CreateHitEffect(const Vector3& position) {

	HitEffect* newHitEffect = HitEffect::Create(position);

	hitEffects_.push_back(newHitEffect);
}

GameScene::~GameScene() {
	delete sprite_;

	delete debugCamera_;

	delete model_;

	delete blockModel_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	delete skydome_;

	delete modelSkydome_;

	delete mapChipField_;

	// 02_10 6枚目 敵クラス削除
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	// 02_11_17枚目
	delete deathParticles_;
	delete deathParticle_model_;

	// 02_16 17枚目
	for (HitEffect* hitEffect : hitEffects_) {
		delete hitEffect;
	}

	delete pauseMenuBackGround_;

	delete pauseMenu_;

	delete pauseMenuRetryButton1_;

	delete pauseMenuRetryButton2_;

	delete pauseMenuTitleButton1_;

	delete pauseMenuTitleButton2_;

	delete pauseMenuBackGame_;

	delete pauseMenuBackGame2_;

	delete chooseTexture_;
}


void GameScene::Initialize() 
{
	//ここにインゲームの初期化処理を書く


	////スプライトインスタンスの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	model_ = Model::Create();

	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	// ゲーム内の地面ブロック
	blockModel_ = Model::CreateFromOBJ("block2");

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	camera_.farZ = 1000.0f;

	// 02_03天球
	// skydome生成
	skydome_ = new Skydome();
	// 初期化
	modelSkydome_ = Model::CreateFromOBJ("skyDome", true);

	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField;

	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	// 自キャラの生成
	player_ = new Player();

	modelPlayer_ = Model::CreateFromOBJ("player2", true);

	modelAttack_ = Model::CreateFromOBJ("attack_effect", true);

	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");

	// 02_16
	particle_model_ = Model::CreateFromOBJ("particle");

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	player_->SetMapChipField(mapChipField_);

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, modelAttack_, &camera_, playerPosition);

	CController_ = new CameraController(); // 生成

	CController_->Initialize(&camera_); // 初期化

	CController_->SetTarget(player_); // 追従対象セット

	CController_->Reset(); // リセット

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CController_->SetMovableArea(cameraArea);


	enemy_model_ = Model::CreateFromOBJ("idiotFace");


	// 02_10 5枚目（for文の中身全部）
	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();

		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(30 + i * 6, 18);

		newEnemy->Initialize(enemy_model_, &camera_, enemyPosition);
		newEnemy->SetGameScene(this);
		enemies_.push_back(newEnemy);
	}

	// 02_12 4枚目 ゲームプレイフェーズから開始
	phase_ = Phase::kFadeIn;

	// 02_13 27枚目
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	HitEffect::SetModel(particle_model_);
	HitEffect::SetCamera(&camera_);

	upData = new UpData();

#pragma region ポーズメニュー
	// ポーズメニュー
	pauseMenuBackgroundHandle_ = TextureManager::Load("Texture/backGround.png");

	pauseMenuRestartButton1Handle_ = TextureManager::Load("Texture/Retry.png");
	pauseMenuRestartButton2Handle_ = TextureManager::Load("Texture/Retry2.png");

	pauseMenuTitleButton1Handle_ = TextureManager::Load("Texture/backTitle.png");
	pauseMenuTitleButton2Handle_ = TextureManager::Load("Texture/backTitle2.png");

	pauseMenuHandle_ = TextureManager::Load("Texture/pause_Menu.png");

	pauseMenuBackGameHandle_ = TextureManager::Load("Texture/backGame.png");
	pauseMenuBackGame2Handle_ = TextureManager::Load("Texture/backGame2.png");

	chooseTextureHandle_ = TextureManager::Load("Texture/choose.png");

	// スプライトの生成
	pauseMenuBackGround_ = Sprite::Create(pauseMenuBackgroundHandle_, {0, 0});

	pauseMenuRetryButton1_ = Sprite::Create(pauseMenuRestartButton1Handle_, {420, 250});
	pauseMenuRetryButton1_->SetSize({373, 208});

	pauseMenuRetryButton2_ = Sprite::Create(pauseMenuRestartButton2Handle_, {420, 250});
	pauseMenuRetryButton2_->SetSize({373, 208});

	pauseMenuTitleButton1_ = Sprite::Create(pauseMenuTitleButton1Handle_, {420, 350});
	pauseMenuTitleButton1_->SetSize({373, 208});

	pauseMenuTitleButton2_ = Sprite::Create(pauseMenuTitleButton2Handle_, {420, 350});
	pauseMenuTitleButton2_->SetSize({373, 208});

	pauseMenu_ = Sprite::Create(pauseMenuHandle_, {320, -15});
	pauseMenu_->SetSize({666, 208});

	pauseMenuBackGame_ = Sprite::Create(pauseMenuBackGameHandle_, {420, 450});
	pauseMenuBackGame_->SetSize({373, 208});

	pauseMenuBackGame2_ = Sprite::Create(pauseMenuBackGame2Handle_, {420, 450});
	pauseMenuBackGame2_->SetSize({373, 208});

	chooseTexture_ = Sprite::Create(chooseTextureHandle_, {200, 300});
	chooseTexture_->SetSize({373, 208});

#pragma endregion
}

// 02_10 10枚目
Vector3 GameScene::GetWorldPosition() const {

	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}


AABB GameScene::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:
		// 02_12 13枚目 if文から中身まで全部実装
		// Initialize関数のいきなりパーティクル発生処理は消す
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticle_model_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:

		break;
	}
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();     // 横の数
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal(); // 縦の数

	// 要素数を変更する

	worldTransformBlocks_.resize(numBlockVirtical);

	blockTypes_.resize(numBlockVirtical);

	trap2Visibility_.resize(numBlockVirtical); // ★追加

	// キューブの生成
	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		worldTransformBlocks_[y].resize(numBlockHorizontal);
		blockTypes_[y].resize(numBlockHorizontal, MapChipType::kBlank);
		trap2Visibility_[y].resize(numBlockHorizontal, false); // ★全て非表示で初期化
	}
	// ブロックの生成
	for (uint32_t y = 0; y < numBlockVirtical; ++y) {
		for (uint32_t x = 0; x < numBlockHorizontal; ++x) {
			MapChipType type = mapChipField_->GetMapChipTypeByIndex(x, y);
			if (type == MapChipType::kBlank) {
				continue;
			}

			WorldTransform* worldTransform = new WorldTransform();
			worldTransform->Initialize();
			worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);

			worldTransformBlocks_[y][x] = worldTransform;

			blockTypes_[y][x] = type;
		}
	}
}

void GameScene::Update() 
{
	//ここにインゲームの更新処理を書く
	//  デスフラグの立ったエフェクトを削除
	hitEffects_.remove_if([](HitEffect* hitEffect) {
		if (hitEffect->IsDead()) {
			delete hitEffect;

			return true;
		}
		return false;
	});

	// 02_15 7枚目 デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	ChangePhase();

	switch (phase_) {

	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kPlay;
		}

		skydome_->Update();
		CController_->Updata();
		//		worldTransformSkydome_.UpdateMatrix();
		//		cameraController->Update();

		// 自キャラの更新
		player_->UpDate();

		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

		// UpdateCamera();
#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// UpdateBlocks();
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				// アフィン変換～DirectXに転送
				upData->WorldTransformUpData(*worldTransformBlock);
			}
		}
		break;

	case Phase::kPlay:
		// ゲームプレイフェーズの処理

		//   skydome生成
		skydome_->Update();

		CController_->Updata();

		//  自キャラの更新
		player_->UpDate();

		// プレイヤーが下に落ちすぎたら死亡扱いにしてフェーズを変更
		if (player_->GetWorldPosition().y < -6.0f) { // 閾値は環境に応じて調整
			player_->SetDead();                      // プレイヤーを死亡状態にする関数

			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticle_model_, &camera_, deathParticlesPosition);
		}

		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送AL3_02_02*/
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送AL3_02_02*/

			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				// アフィン変換行列の生成
				upData->WorldTransformUpData(*worldTransformBlock);
			}
		}

		CheckAllCollisions();

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}
		break;

	case Phase::kDeath:
		// デス演出フェーズ

		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
		}

		//    skydome生成
		skydome_->Update();
		CController_->Updata();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

		// 02_11 18枚目 デスパーティクルあれば更新
		if (deathParticles_) {
			deathParticles_->Update();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		skydome_->Update();
		CController_->Updata();

		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

		break;
	}

	// =========================
	// ポーズ処理
	// =========================
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		pauseMenuActive_ = !pauseMenuActive_;
	}

	if (pauseMenuActive_) {
		// 上下キーで選択移動
		if (Input::GetInstance()->TriggerKey(DIK_UP)) {
			pauseSelection_ = (pauseSelection_ + 2) % 3; // 0←→2循環
		}
		if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
			pauseSelection_ = (pauseSelection_ + 1) % 3;
		}

		// Enterキーで決定
		if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
			switch (pauseSelection_) {
			case 0:               // リトライ
				finished_ = true; // Scene側で再生成される
				break;
			case 1: // タイトル
				finished_ = true;
				player_->SetDead(); // 死んだ扱いにしてGameOverへ飛ばさないように
				break;
			case 2: // 続行
				pauseMenuActive_ = false;
				break;
			}
		}
		return; // ポーズ中はゲーム更新しない
	}
}



void GameScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	if (!player_->IsDead()) {
		player_->Draw();
	}

	// 天球描画
	skydome_->Draw();

	// ブロックの描画
	for (uint32_t y = 0; y < worldTransformBlocks_.size(); ++y) {
		for (uint32_t x = 0; x < worldTransformBlocks_[y].size(); ++x) {
			WorldTransform* worldTransform = worldTransformBlocks_[y][x];
			if (!worldTransform) {
				continue;
			}

			MapChipType type = blockTypes_[y][x];

			switch (type) {
			case MapChipType::kBlock:
				blockModel_->Draw(*worldTransform, camera_);

				break;
			}
		}
	}

	// 02_09 12枚目 敵更新
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 02_11 18枚目 デスパーティクルあれば描画
	if (player_->IsDead()) {
		if (deathParticles_) {
			deathParticles_->Draw();
		}
	}

	for (HitEffect* hitEffect : hitEffects_) {
		hitEffect->Draw();
	}

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// =========================
	// ポーズメニュー描画
	// =========================
	if (pauseMenuActive_) {
		// ポーズメニュー
		pauseMenuBackGround_->Draw();

		pauseMenu_->Draw();

		//
		switch (pauseSelection_) {

			break;

		case 0: // リトライ
			pauseMenuRetryButton2_->Draw();
			pauseMenuTitleButton1_->Draw();
			pauseMenuBackGame_->Draw();
			chooseTexture_->Draw();
			break;
		case 1: // タイトル
			pauseMenuRetryButton1_->Draw();
			pauseMenuTitleButton2_->Draw();
			pauseMenuBackGame_->Draw();
			chooseTexture_->Draw();
			break;
		case 2:
			pauseMenuRetryButton1_->Draw();
			pauseMenuTitleButton1_->Draw();
			pauseMenuBackGame2_->Draw();
			chooseTexture_->Draw();
			break;
		}
	}

	// スプライト描画後処理
	Sprite::PostDraw();

	// 02_13 28枚目
	fade_->Draw();
}


// 02_10 16枚目
void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {

			// コリジョン無効の敵はスキップ
			if (enemy->IsCollisionDisabled())
				continue;

			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}
	}
#pragma endregion
}
