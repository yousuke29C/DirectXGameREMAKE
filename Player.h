#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class RailCamera;

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name = "model">���f��</param>
	/// <param mame = "textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjectione">�r���[�v���W�F�N�V�����i�Q�Ɠn���j</param>
	void Draw(ViewProjection viewProjection_);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// �e���X�g���擾
	const std::list < std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	float GetRadius();

	void SetParent(WorldTransform* worldTransform) {
		worldTransform_.parent_ = worldTransform;
	}
private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// ���͏������邽��
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	const float radius_ = 1.0f;

	RailCamera* camera_ = nullptr;
};