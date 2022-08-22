#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}



void RailCamera::Update()
{
	//�ړ�
	worldTransform_.translation_ += Vector3(0, 0, 0.1f);
	//���[���h�g�����X�t�H�[�����X�V
	worldTransform_.matWorld_ = CreateMatIdentity();
	worldTransform_.matWorld_ *= CreateMatScale(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatRotationX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatRotationY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatRotationZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= CreateMatTranslation(worldTransform_.translation_);
	//���[���h�s��̕��s�ړ�����
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];




	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = MyMath::Math_(forward, worldTransform_.matWorld_);

	//���_����O���ɓK���ȋ����i��
	forward += viewProjection_.eye;
	viewProjection_.target = forward;

	//���[���h���
	Vector3 up(0, 1, 0);

	//���[���J�����̉�]
	viewProjection_.up = MyMath::Math_(up, worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V����
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	//�f�o�b�O�p�\��
	debugText_->SetPos(20, 100);
	debugText_->Printf(
		"RailCamera Pos:(%f,%f,%f)", viewProjection_.eye.x,
		viewProjection_.eye.y,
		viewProjection_.eye.z);
}