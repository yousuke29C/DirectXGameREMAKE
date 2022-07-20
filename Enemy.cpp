#include "Enemy.h"
#include <cassert>
#include "Function.h"

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
		// �ڋ߃t�F�[�Y�̍X�V
		AccessPhaseUpdate();
		break;
	case Enemy::Phase::Leave:
		// ���E�t�F�[�Y�̍X�V
		EliminationPhaseUpdate();
		break;
	}
}
// �ڋ߃t�F�[�Y�̍X�V
void Enemy::AccessPhaseUpdate() {

	Vector3 move = { 0,0,0 };
	//�G�̈ړ��̑���
	const float kEnemySpeed = 0.02f;

	move = { 0.0f,0.0f,kEnemySpeed };
	

	// �ړ� (�x�N�g�������Z)
	worldtransform_.translation_ -= move;
	// ����̈ʒu�ɓ��B�����痣�E
	if (worldtransform_.translation_.z < -10.0f) {
		phase_ = Enemy::Phase::Leave;
	}
}

// ���E�t�F�[�Y�̍X�V
void Enemy::EliminationPhaseUpdate() {
	Vector3 move2 = { 0,0,0 };
	//�G�̈ړ��̑���
	const float kEnemySpeed = 0.02f;
	move2 = { kEnemySpeed ,kEnemySpeed ,0.0f };
	// �ړ��i�x�N�g�������Z�j
	worldtransform_.translation_ -= move2;
}


void Enemy::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle2_);
}
