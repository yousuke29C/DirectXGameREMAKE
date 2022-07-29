#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

class Enemy
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
	/// �e�̔���
	/// </summary>
	void Fire();

	// �s���t�F�[�Y
	enum class Phase {
		Approach, // �ڋ߂���
		Leave,    // ���E����
	};

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
	// �t�F�[�Y
	Phase phase_ = Phase::Approach;
	// �e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// �ڋ߃t�F�[�Y�̍X�V
	void AccessPhaseUpdate();
	// ���E�t�F�[�Y�̍X�V
	void EliminationPhaseUpdate();
};