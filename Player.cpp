#include "Player.h"
#include "Function.h"
#include <cassert>

/// <summary>
/// ������
/// �������烂�f���f�[�^�ƃe�N�X�`���f�[�^���󂯎��
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldtransform_.Initialize();
}

void Player::Update()
{
	Vector3 move = { 0,0,0 };

	//�L�����N�^�[�̈ړ��̑���
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	//�L�����N�^�[���񏈗�
	//�����������ňړ��x�N�g����ύX
	const float kCharacterRootSpeed = 0.2f;

	if (input_->PushKey(DIK_U)) {
		worldtransform_.rotation_.y -= kCharacterRootSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldtransform_.rotation_.y += kCharacterRootSpeed;
	}

	//�L�[�{�[�h���͂ɂ��ړ�����
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//�͈͂𒴂��Ȃ�����
	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, +kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, +kMoveLimitY);


	worldtransform_.translation_ += move;

	//�s��X�V
	worldtransform_.matWorld_ = CreateMatIdentity();
	worldtransform_.matWorld_ *= CreateMatScale(worldtransform_.scale_);
	worldtransform_.matWorld_ *= CreateMatRotationX(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationY(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatRotationZ(worldtransform_.rotation_);
	worldtransform_.matWorld_ *= CreateMatTranslation(worldtransform_.translation_);
	worldtransform_.TransferMatrix();

	Attack();

	if (bullet_) {
		bullet_->Update();
	}
}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//�e�̕`��
	if (bullet_) {
		bullet_->Draw(viewProjection_);
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE)) {
		//�e�𐶐����A������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldtransform_.translation_);

		//�e��o�^����
		bullet_ = newBullet;
	}
}
