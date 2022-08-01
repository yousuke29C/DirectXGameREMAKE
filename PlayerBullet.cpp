#include "PlayerBullet.h"
#include "Function.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;

	// �����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	// ���W���ړ�������i�P�t���[�����̈ړ��ʂ𑫂����ށj
	worldTransform_.translation_ += velocity_;

	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// �s��X�V
	CreateMatrixUpdate(worldTransform_);
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() {
	isDead_ = true;
}
Vector3  PlayerBullet::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}