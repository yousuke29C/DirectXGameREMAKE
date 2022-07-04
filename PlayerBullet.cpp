#include "PlayerBullet.h"
#include "Function.h"
#include <cassert>



void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
    textureHandle_ = TextureManager::Load("mario.jpg");

    //���[���h�ϊ��̏�����
    worldtransform_.Initialize();

    //�����Ŏ󂯎�����������W���Z�b�g
    worldtransform_.translation_ = position;

}

void PlayerBullet::Update() {
    //�s��X�V
    worldtransform_.matWorld_ = CreateMatIdentity();
    worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
    worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
    worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
    worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
    worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
    worldtransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

    model_->Draw(worldtransform_, viewProjection, textureHandle_);
}