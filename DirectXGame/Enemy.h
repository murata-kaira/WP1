#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "Player.h"

class MapChipField;
class Player;
class Enemy {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void OnCollision(const Player* player);

	 KamataEngine::Vector3 GetWorldPosition();
	
private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	float walkTimer_ = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 上下移動用
	KamataEngine::Vector3 basePosition_ = {}; // 上下移動の中心座標
	static inline const float kMoveAmplitude = 2.0f;   // 上下移動の振れ幅
	static inline const float kMoveSpeed = 2.0f;       // 上下移動の速さ
};
