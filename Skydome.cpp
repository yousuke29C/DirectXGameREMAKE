#include "Skydome.h"

void Skydome::Initialize()
{
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//�X�P�[�����O�̐ݒ�
	worldTransform_.scale_ = { 250.0f,250.0f,250.0f };
	//�X�P�[�����O�s��
	worldTransform_.matWorld_ = CreateMatScale(worldTransform_.scale_);
	//�s��̓]��
	worldTransform_.TransferMatrix();

	model_ = Model::CreateFromOBJ("skydome", true);
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewprojection_)
{
	//3D���f���֐�
	model_->Draw(worldTransform_, viewprojection_);
}