#include "Enemy.h"
#include <cassert>
#include "Function.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldtransform_.Initialize();
}

void Enemy::Update()
{
	Vector3 move = { 0,0,0 };
	Vector3 move2 = { 0,0,0 };

	//敵の移動の速さ
	const float kEnemySpeed = 0.02f;

	move = { 0.0f,0.0f,kEnemySpeed };
	move2 = { kEnemySpeed ,kEnemySpeed ,0.0f };

	/*worldtransform_.translation_ += move;*/


	//行列更新
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		//移動(ベクトルを加算)
		worldtransform_.translation_ -= move;
		//既定の位置に到達したら離脱
		if (worldtransform_.translation_.z < -10.0f) {
			phase_ = Phase::Leave;
		}
		break;
		case Enemy::Phase::Leave:
			//移動（ベクトルを加算）
			worldtransform_.translation_ -= move2;
			break;
	}

}

void Enemy::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);
}
