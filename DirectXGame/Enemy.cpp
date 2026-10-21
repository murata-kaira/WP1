#define NOMINMAX
#include "Enemy.h"
#include "MapChipField.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {

	assert(model);

	model_ = model;

	camera_ = camera;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	basePosition_ = position;

	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	walkTimer_ += 1.0f / 60.0f;

	// 上下に往復移動する障害物敵
	worldTransform_.translation_.y = basePosition_.y + std::sin(walkTimer_ * kMoveSpeed) * kMoveAmplitude;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player; }
