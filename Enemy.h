#include <3d/Model.h>
#include <input.h>
#include <DebugText.h>


///<summary>
///�G
/// </summary>
class Enemy {
	public:
		/// <summary>
		///������
		/// </summary>
		/// <param name="model">���f��</param>
		/// <param name="textureHandle">�e�N�X�`���n���h��</param>
		void Initialize(Model* model, uint32_t textureHandle);

		/// <summary>
		///�X�V
		/// </summary>
		void Update();

		/// <summary>
		///�`��
		/// </summary>
		void Draw(ViewProjection viewProjection_);

	private:
		//���[���h�ϊ��f�[�^
		WorldTransform worldtransform_;
		//���f��
		Model* model_ = nullptr;
		//�e�N�X�`���n���h��
		uint32_t textureHandle_ = 0u;

		Input* input_ = nullptr;

		DebugText* debugText_ = nullptr;

};