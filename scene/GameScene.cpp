﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include <random>
#include "Function.h"
#define PI 3.14

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

float Degree(const float& degree) {
	float n = degree * PI / 180;
	return n;
}
void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自弾と敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& bulletB : enemyBullets) {
		// 自弾の座標
		posB = bulletA ->GetWorldPosition();
		// 敵弾の座標
		posA = bulletB->GetWorldPosition();
		//AとBの距離を求める
		Vector3 len = MyMath::Vector3sub(posA, posB);
		float distance = MyMath::length(len);
		//半径
		float radius = bulletB->GetRadius() + bulletA->GetRadius()
		// 自弾と敵弾の交差判定
		if (distance <= radius) {
			// 自キャラの衝突時コールバックを呼び出す
			bulletB->OnCollision();
			// 敵弾のの衝突時コールバックを呼び出す
			bulletA->OnCollision();
		}
	}
}
#pragma endregion

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		// 敵弾の座標
		posB = enemyBullet_->GetWorldPosition();

		//AとBの距離を求める
		Vector3 len = MyMath::Vector3sub(posA, posB);
		float distance = MyMath::length(len);
		//半径
		float radius = player_->GetRadius();
		// 球と球の交差判定
		if (distance <= radius) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾のの衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		// 敵弾の座標
		posB = playerBullet_->GetWorldPosition();

		//AとBの距離を求める
		Vector3 len = MyMath::Vector3sub(posA, posB);
		float distance = MyMath::length(len);
		//半径
		float radius = enemy_->GetRadius();
		// 球と球の交差判定
		if (distance <= radius) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾のの衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion
}

//乱数シード生成器
std::random_device seed_gen;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine(seed_gen());
//乱数範囲の指定
std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
std::uniform_real_distribution<float> rotDist(0.0f, 3.14f);



//Matrix4 CreateMatRot(Vector3 Rotation) {
//	Matrix4 Rot = CreateMatIdentity();
//	Rot *= CreateMatRotationZ(worldTransform.rotation_);
//	Rot *= CreateMatRotationY(worldTransform.rotation_);
//	Rot *= CreateMatRotationX(worldTransform.rotation_);
//
//	return Rot;
//}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandle2_ = TextureManager::Load("gensin1.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//ビュープロジェクション
	viewProjection_.Initialize();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(600, 400);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵の生成
	enemy_ = new Enemy();

	//敵キャラの初期化
	enemy_->Initialize(model_, textureHandle2_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

}

void GameScene::Update() {
	//デバックカメラの更新
	debugCamera_->Update();

	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= 0.5f;
		}
		if (input_->PushKey(DIK_RIGHT)) {
			move.x += 0.5f;
		}
	}

	//自キャラの更新
	player_->Update();

	//敵の更新
	enemy_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>



	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵の描画
	enemy_->Draw(viewProjection_);
	////3Dモデル
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}