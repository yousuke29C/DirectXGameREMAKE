#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Function.h"
///<summary>
///�V��
///<summary>
class Skydome {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewproject_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//3D���f��
	Model* model_ = nullptr;
};