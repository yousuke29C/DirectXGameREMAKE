#include "Model.h"
#include "WorldTransform.h"


///(summary)
///���L�����̒e
/// </summary>
class PlayerBullet {
public:
	///(summary)
	///������
	/// </summary>
	/// <param name = "model">���f��</param>
	/// <param name = "position">�������W</param>
	void Initialize(Model* model, const Vector3& position);

	///(summary)
	///�X�V
	/// </summary>
	void Update();
	///(summary)
	///�`��
	/// </summary>
	/// <param name = "ViewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& ViewProjection);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};
