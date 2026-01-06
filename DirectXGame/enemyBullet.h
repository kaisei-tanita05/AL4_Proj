#pragma once
#include "Math.h"
#include <KamataEngine.h>
#include "UpData.h"


class Player;
class GameScene;

class EnemyBullet {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	void UpDate();

	void Draw();

	KamataEngine::Vector3 GetWorldPosition() const;

	AABB GetAABB();

	void OnCollision(const Player* player);

	bool IsDead() const { return !isAlive_; }

	void SetDead() { isAlive_ = true; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	void SetPosition(const KamataEngine::Vector3& position) {
		worldTransform_.translation_ = position;
	}

	const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }

	void SetSpeed(const KamataEngine::Vector3& speed) { bulletSpeed = speed; }

	const KamataEngine::Vector3& GetVelocity() const { return bulletSpeed; }
	void SetVelocity(const KamataEngine::Vector3& v) { bulletSpeed= v; }

	void SetReflected(bool reflected) { isReflected_ = reflected; }
	bool IsReflected() const { return isReflected_; }

private:

	//ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	//モデル
	KamataEngine::Model* model_ = nullptr;

	UpData* upData = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 bulletSpeed = {};

	static inline const float kWidth = 0.6f;
	static inline const float kHeight = 0.6f;

	//KamataEngine::Vector3 position_;

	bool isAlive_ = false;

	GameScene* gameScene_ = nullptr;

	 bool isReflected_ = false;
};
