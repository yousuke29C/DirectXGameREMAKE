#include "Enemy.h"
#include <cassert>
#include "Function.h"
#include"EnemyBullet.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle2)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle2_ = textureHandle2;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldtransform_.Initialize();
}

void Enemy::Update()


{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	Vector3 move = { 0,0,0 };
	Vector3 move2 = { 0,0,0 };

	//�G�̈ړ��̑���
	const float kEnemySpeed = 0.02f;

	move = { 0.0f,0.0f,kEnemySpeed };
	move2 = { kEnemySpeed ,kEnemySpeed ,0.0f };

	/*worldtransform_.translation_ += move;*/


	//�s��X�V
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
		//�ړ�(�x�N�g�������Z)
		worldtransform_.translation_ -= move;
		//����̈ʒu�ɓ��B�����痣�E
		if (worldtransform_.translation_.z < -10.0f) {
			phase_ = Phase::Leave;
		}
		break;
		case Enemy::Phase::Leave:
			//�ړ��i�x�N�g�������Z�j
			worldtransform_.translation_ -= move2;
			break;
	}
	Fire();
	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}
void Enemy::Fire() {
	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = bvector(velocity, worldtransform_);
	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldtransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle2_);
}
