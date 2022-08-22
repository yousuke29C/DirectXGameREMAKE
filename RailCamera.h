#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
#include "Function.h"

///<summary>
///���[���J����
///</summary>
class RailCamera
{
public:
	///<summary>
	///������
	///</summary>
	void Initialize(const Vector3& position, const Vector3& rotation);

	///<summary>
	///�X�V
	///</summary>
	void Update();

	///<summary>
	///�r���[�v���W�F�N�V�������擾
	///</summary>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//���[���h�s����擾
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
};
