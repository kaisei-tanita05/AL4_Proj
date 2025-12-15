#include "enemyBullet.h"
#include"Player.h"

using namespace KamataEngine;

void EnemyBullet::Initialize(Model* model, Camera* camera, Vector3& position) {
	assert(model);

	model_ = model;

	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	isAlive_ = true;
}

void EnemyBullet::UpDate() {
	if (!isAlive_) {
		return;
	}

	worldTransform_.translation_ += bulletSpeed;

	upData->WorldTransformUpData(worldTransform_);

	// 範囲外で消滅（例: y座標が一定より下）
	if (worldTransform_.translation_.x < -500.0f) {
		isAlive_ = false;
	}
}

AABB EnemyBullet::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 EnemyBullet::GetWorldPosition() const {

	//upData->WorldTransformUpData(worldTransform_);
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



void EnemyBullet::Draw() 
{
	if (isAlive_) {
		model_->Draw(worldTransform_, *camera_);
	}
}