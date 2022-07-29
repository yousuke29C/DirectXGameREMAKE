#include "Enemy.h"
#include "Function.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
    // NULL�|�C���^�`�F�b�N
    assert(model);

    // �����o�ϐ��ɑ��
    model_ = model;
    textureHandle_ = textureHandle;

    // �V���O���g���C���X�^���X���擾����
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();

    // ���[���h�ϊ��̏�����
    worldTransform_.Initialize();
}

void Enemy::Update() {

    // �L�����N�^�[�̈ړ��x�N�g��
    Vector3 move = { 0,0,0 };

    //�L�����N�^�[�̈ړ��̑���
    const float EnemySpeed = 0.3f;
    const float EnemyYSpeed = 0.01f;
    const float accessPhaseSpeed = 0.3f; // �ڋ߃t�F�[�Y�̃X�s�[�h
    const float eliminationPhaseSpeed = 0.3f; // ���E�t�F�[�Y�̑��x

    // ���W�ړ�(�x�N�g���̉��Z)
    /*move.z -= EnemySpeed;
    move.y += EnemyYSpeed;*/

    /* worldTransform_.translation_ += move;*/

    // �s��X�V
    CreateMatrixUpdate(worldTransform_);

    switch (phase_) {
    case Enemy::Phase::Approach:
    default:
        // �ڋ߃t�F�[�Y�̍X�V
        AccessPhaseUpdate();
        break;
    case Enemy::Phase::Leave:
        // ���E�t�F�[�Y�̍X�V
        EliminationPhaseUpdate();
        break;
    }

    // �G�̍U������
    Fire();

    // �e�X�V
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Update();
    }
}

void Enemy::Draw(ViewProjection viewProjection_) {
    model_->Draw(worldTransform_, viewProjection_, textureHandle_);

    // �e�`��
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Draw(viewProjection_);
    }
}

void Enemy::Fire() {
    // �e�̑��x
    const float kBulletSpeed = 1.0f;
    Vector3 velocity(0, 0, kBulletSpeed);

    // ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
    velocity = CreateVector(velocity, worldTransform_);

    // ���L�����̍��W���R�s�[
    Vector3 position = worldTransform_.translation_;

    // �e�𐶐����A������
    std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
    newBullet->Initialize(model_, position, velocity);

    // ����o�^����
    bullets_.push_back(std::move(newBullet));
}

// �ڋ߃t�F�[�Y�̍X�V
void Enemy::AccessPhaseUpdate() {
    // �ړ� (�x�N�g�������Z)
    worldTransform_.translation_ -= {0.0, 0.0, 0.05};
    //�K��̈ʒu�ɓ��B�����痣�E
    if (worldTransform_.translation_.z < -10.0f) {
        phase_ = Enemy::Phase::Leave;
    }
}

// ���E�t�F�[�Y�̍X�V
void Enemy::EliminationPhaseUpdate() {
    // �ړ��i�x�N�g�������Z�j
    worldTransform_.translation_ += {0.05, 0.05, 0};
}
