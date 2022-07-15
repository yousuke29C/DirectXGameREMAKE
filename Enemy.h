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
		void Initialize(Model* model, uint32_t textureHandle2);

		/// <summary>
		///�X�V
		/// </summary>
		void Update();

		/// <summary>
		///�`��
		/// </summary>
		void Draw(ViewProjection viewProjection_);

		//�s���t�F�[�Y
		enum class Phase {
			Approach, //�ڋ߂���
			Leave, //���E����
		};
		/// <summary>
		///�e����
		/// </summary>
		void Fire();

	private:
		//���[���h�ϊ��f�[�^
		WorldTransform worldtransform_;
		//���f��
		Model* model_ = nullptr;
		//�e�N�X�`���n���h��
		uint32_t textureHandle2_ = 0u;

		Input* input_ = nullptr;

		DebugText* debugText_ = nullptr;

		//�t�F�[�Y
		Phase phase_ = Phase::Approach;
		void AccessPhaseUpdate();

		void EliminationPhaseUpdate();
		//�e
		std::list<std::unique_ptr<EnemyBullet>> bullets_;
};