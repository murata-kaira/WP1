#include "CameraController.h"
#include "Player.h"

using namespace KamataEngine;
using namespace MathUtility;

void CameraController::Initialize() { camera_.Initialize(); }

void CameraController::SetViewingGoal(bool viewingGoal) {
	if (viewingGoal && !isViewingGoal_) {
		// ゴール確認モードに入った瞬間に手動移動オフセットをリセット
		goalViewManualOffset_ = {};
	}
	isViewingGoal_ = viewingGoal;
}

void CameraController::Update() {

	if (isViewingGoal_) {
		// WASDキーでゴール確認中のカメラを手動で動かせるようにする
		Input* input = Input::GetInstance();
		if (input->PushKey(DIK_A)) {
			goalViewManualOffset_.x -= kGoalViewManualMoveSpeed;
		}
		if (input->PushKey(DIK_D)) {
			goalViewManualOffset_.x += kGoalViewManualMoveSpeed;
		}
		if (input->PushKey(DIK_W)) {
			goalViewManualOffset_.y += kGoalViewManualMoveSpeed;
		}
		if (input->PushKey(DIK_S)) {
			goalViewManualOffset_.y -= kGoalViewManualMoveSpeed;
		}

		// ゴール確認中はカメラをゴール位置（＋手動移動分）へなめらかに移動させる
		const Vector3 goalCameraPosition = goalPosition_ + target0ffset_ + goalViewManualOffset_;
		camera_.translation_.x = Lerp(camera_.translation_.x, goalCameraPosition.x, kGoalViewInterpolationRate);
		camera_.translation_.y = Lerp(camera_.translation_.y, goalCameraPosition.y, kGoalViewInterpolationRate);
		camera_.translation_.z = Lerp(camera_.translation_.z, goalCameraPosition.z, kGoalViewInterpolationRate);
		camera_.UpdateMatrix();
		return;
	}

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	targetPosition_ = targetWorldTransform.translation_ + target0ffset_;

	const Vector3& targetVelocity = target_->GetVelocity();
	targetPosition_ = targetWorldTransform.translation_ + targetVelocity * kVelocityBias;

	camera_.translation_ = targetWorldTransform.translation_ + target0ffset_;

	camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);

	camera_.translation_.x = max(camera_.translation_.x, camera_.translation_.x + targetMargin.left);
	camera_.translation_.x = min(camera_.translation_.x, camera_.translation_.x + targetMargin.right);
	camera_.translation_.y = max(camera_.translation_.y, camera_.translation_.y + targetMargin.bottom);
	camera_.translation_.y = min(camera_.translation_.y, camera_.translation_.y + targetMargin.top);
	 
	camera_.translation_.x = max(camera_.translation_.x,movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = max(camera_.translation_.y, movableArea_.bottom);
	camera_.translation_.y = min(camera_.translation_.y, movableArea_.top);


	camera_.UpdateMatrix();
}

void CameraController::Reset() {

	const KamataEngine::WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	camera_.translation_ = targetWorldTransform.translation_ + target0ffset_;
}
