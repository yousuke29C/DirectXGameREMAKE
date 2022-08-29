#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	input_ = Input::GetInstance();
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	/*worldTransform_.scale_ = { 1,1,1 };*/
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}



void RailCamera::Update()
{
	//移動
	worldTransform_.translation_ += Vector3(0, 0, 0.01f);
	//ワールドトランスフォームを更新
	worldTransform_.matWorld_ = CreateMatIdentity();
	worldTransform_.matWorld_ *= CreateMatScale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= CreateMatRotationX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatRotationY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatRotationZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatTranslation(worldTransform_.translation_);
	//ワールド行列の平行移動成分
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];




	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = bvector(forward, worldTransform_.matWorld_);

	//視点から前方に適当な距離進んだ
	forward += viewProjection_.eye;
	viewProjection_.target = forward;

	//ワールド上方
	Vector3 up(0, 1, 0);

	//レールカメラの回転
	viewProjection_.up = bvector(up, worldTransform_.matWorld_);

	//ビュープロジェクション
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	//デバッグ用表示
	debugText_->SetPos(20, 100);
	debugText_->Printf(
		"RailCamera Pos:(%f,%f,%f)", viewProjection_.eye.x,
		viewProjection_.eye.y,
		viewProjection_.eye.z);
}