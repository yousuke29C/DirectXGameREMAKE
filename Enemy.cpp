#include "Enemy.h"
#include "Function.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
    // NULLポインタチェック
    assert(model);

    // メンバ変数に代入
    model_ = model;
    textureHandle_ = textureHandle;

    worldTransform_.rotation_ = { 0, 3.0f, 0 };

    // シングルトンインスタンスを取得する
    input_ = Input::GetInstance();
    debugText_ = DebugText::GetInstance();

    // ワールド変換の初期化
    worldTransform_.Initialize();

    // 接近フェーズの初期化
    ApproachInitialize();
}

void Enemy::Update() {
    // デスフラグの立った弾を削除
    bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
        return bullet->IsDead();
        });

    // キャラクターの移動ベクトル
    Vector3 move = { 0,0,0 };

    //キャラクターの移動の速さ
    const float EnemySpeed = 0.3f;
    const float EnemyYSpeed = 0.01f;
    const float accessPhaseSpeed = 0.3f; // 接近フェーズのスピード
    const float eliminationPhaseSpeed = 0.3f; // 離脱フェーズの速度

    // 座標移動(ベクトルの加算)
    /*move.z -= EnemySpeed;
    move.y += EnemyYSpeed;*/

    /* worldTransform_.translation_ += move;*/

    // 行列更新
    CreateMatrixUpdate(worldTransform_);

    switch (phase_) {
    case Enemy::Phase::Approach:
    default:
        // 接近フェーズの更新
        AccessPhaseUpdate();
        break;
    case Enemy::Phase::Leave:
        // 離脱フェーズの更新
        EliminationPhaseUpdate();
        break;
    }

    // 弾更新
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Update();
    }
}

void Enemy::Draw(ViewProjection viewProjection_) {
    model_->Draw(worldTransform_, viewProjection_, textureHandle_);

    // 弾描画
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Draw(viewProjection_);
    }
}

void Enemy::Fire() {
    // 弾の速度
    const float kBulletSpeed = 1.0f;
    Vector3 velocity(0, 0, kBulletSpeed);

    // 速度ベクトルを自機の向きに合わせて回転させる
    velocity = CreateVector(velocity, worldTransform_);

    // 自キャラの座標をコピー
    Vector3 position = worldTransform_.translation_;

    // 弾を生成し、初期化
    std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
    newBullet->Initialize(model_, position, velocity);

    // 球を登録する
    bullets_.push_back(std::move(newBullet));
}

// 接近フェーズの更新
void Enemy::AccessPhaseUpdate() {
    // 移動 (ベクトルを加算)
    worldTransform_.translation_ -= {0.0, 0.0, 0.05};
    //規定の位置に到達したら離脱
    if (worldTransform_.translation_.z < -10.0f) {
        phase_ = Enemy::Phase::Leave;
    }
    // 発射タイマーカウントダウン
    fireTimer--;
    // 指定時間に達した
    if (fireTimer <= 0) {
        // 弾を発射
        Fire();
        // 発射タイマーを初期化
        fireTimer = kFireInterval;
    }
}

// 接近フェーズの初期化
void Enemy::ApproachInitialize() {
    // 発射タイマーを初期化
    fireTimer = kFireInterval;

}

// 離脱フェーズの更新
void Enemy::EliminationPhaseUpdate() {
    // 移動（ベクトルを加算）
    worldTransform_.translation_ += {0.05, 0.05, 0};
    // 発射タイマーカウントダウン
    fireTimer--;
    // 指定時間に達した
    if (fireTimer <= 0) {
        // 弾を発射
        Fire();
        // 発射タイマーを初期化
        fireTimer = kFireInterval;
    }
}