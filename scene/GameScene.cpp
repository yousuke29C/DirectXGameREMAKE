#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "Function.h"
#include <random>
#define PI 3.14

//関数
float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

float Degree(const float& degree) {
	float n = degree * PI / 180;
	return n;
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
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//-------オブジェクト100個-------//
	////ワールドトランスフォームの初期化
	//for (WorldTransform& worldTransform : worldTransforms_) {
	//	worldTransform.Initialize();

	//	//X,Y,Z方向のスケーリングを設定
	//	worldTransform.scale_ = { 1,1,1 };
	//	//X,Y,Z軸周りの回転角を設定
	//	worldTransform.rotation_ = { rotDist(engine), rotDist(engine), rotDist(engine) };
	//	//X,Y,Z軸周りの平行移動を設定
	//	worldTransform.translation_ = { posDist(engine),posDist(engine),posDist(engine) };

	//	// スケーリング、回転、平行移動行列を作成する
	//	Matrix4 matScale = CreateMatScale(worldTransform.scale_); // スケーリング行列作成

	//	Matrix4 matRotZ = CreateMatRotationZ(worldTransform.rotation_);
	//	Matrix4 matRotY = CreateMatRotationY(worldTransform.rotation_);
	//	Matrix4 matRotX = CreateMatRotationX(worldTransform.rotation_);

	//	Matrix4 lat = CreateMatTranslation(worldTransform.translation_);

		////スケーリング・回転・平行移動を合成した行列を計算
		//worldTransform.matWorld_ = CreateMatIdentity();
		//worldTransform.matWorld_ *= matScale; // worldTransform.matWorld_ *= CreateMatScale(worldTransform.scale_); これでも可
		//worldTransform.matWorld_ *= matRotZ;
		//worldTransform.matWorld_ *= matRotY;
		//worldTransform.matWorld_ *= matRotX;
		//worldTransform.matWorld_ *= CreateMatTranslation(worldTransform.translation_);

		//行列の転送
		//worldTransform.TransferMatrix();
	//}

	//ワールドトランスフォームの初期化

	//大本(0番)
	worldTransforms_[Partid::kRoot].Initialize();
	// int* num = &a;
	//脊髄(1番)
	worldTransforms_[Partid::kSpine].Initialize();
	worldTransforms_[Partid::kSpine].parent_ = &worldTransforms_[Partid::kRoot];
	worldTransforms_[Partid::kSpine].translation_ = { 0,4.5f,0 };

	//Chest
	worldTransforms_[Partid::kChest].Initialize();
	worldTransforms_[Partid::kChest].parent_ = &worldTransforms_[Partid::kSpine];
	worldTransforms_[Partid::kChest].translation_ = { 0,4.5f,0 };

	//Head
	worldTransforms_[Partid::kHead].Initialize();
	worldTransforms_[Partid::kHead].parent_ = &worldTransforms_[Partid::kChest];
	worldTransforms_[Partid::kHead].translation_ = { 0,4.5f,0 };

	//ArmL
	worldTransforms_[Partid::kArmL].Initialize();
	worldTransforms_[Partid::kArmL].parent_ = &worldTransforms_[Partid::kChest];
	worldTransforms_[Partid::kArmL].translation_ = { 4.5f,0,0 };

	//ArmR
	worldTransforms_[Partid::kArmR].Initialize();
	worldTransforms_[Partid::kArmR].parent_ = &worldTransforms_[Partid::kChest];
	worldTransforms_[Partid::kArmR].translation_ = { -4.5f,0,0 };

	//Hip
	worldTransforms_[Partid::kHip].Initialize();
	worldTransforms_[Partid::kHip].parent_ = &worldTransforms_[Partid::kSpine];
	worldTransforms_[Partid::kHip].translation_ = { 0,0,0 };

	//LegL
	worldTransforms_[Partid::kLegL].Initialize();
	worldTransforms_[Partid::kLegL].parent_ = &worldTransforms_[Partid::kHip];
	worldTransforms_[Partid::kLegL].translation_ = { 4.5f,-4.5f,0 };

	//LegR
	worldTransforms_[Partid::kLegR].Initialize();
	worldTransforms_[Partid::kLegR].parent_ = &worldTransforms_[Partid::kHip];
	worldTransforms_[Partid::kLegR].translation_ = { -4.5f,-4.5f,0 };

	//X,Y,Z方向のスケーリングを設定
	worldTransforms_[0].scale_ = { 1,1,1 };
	//X,Y,Z軸周りの回転角を設定
	worldTransforms_[0].rotation_ = { 0, 0, 0 };
	//X,Y,Z軸周りの平行移動を設定
	worldTransforms_[0].translation_ = { 0,0,0 };

	//// スケーリング、回転、平行移動行列を作成する
	//Matrix4 matScale = CreateMatScale(worldTransforms_[0].scale_); // スケーリング行列作成

	//Matrix4 matRotZ = CreateMatRotationZ(worldTransforms_[0].rotation_);
	//Matrix4 matRotY = CreateMatRotationY(worldTransforms_[0].rotation_);
	//Matrix4 matRotX = CreateMatRotationX(worldTransforms_[0].rotation_);

	//Matrix4 lat = CreateMatTranslation(worldTransforms_[0].translation_);

	////スケーリング・回転・平行移動を合成した行列を計算
	//worldTransforms_[0].matWorld_ = CreateMatIdentity();
	//worldTransforms_[0].matWorld_ *= matScale; // worldTransform.matWorld_ *= CreateMatScale(worldTransform.scale_); これでも可
	//worldTransforms_[0].matWorld_ *= matRotZ;
	//worldTransforms_[0].matWorld_ *= matRotY;
	//worldTransforms_[0].matWorld_ *= matRotX;
	//worldTransforms_[0].matWorld_ *= CreateMatTranslation(worldTransforms_[0].translation_);

	////行列の転送
	//worldTransforms_[0].TransferMatrix();
	////カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };

	////カメラ注視点座標を設定
	//viewProjection_.target = { 10,0,0 };

	////カメラ上方向ベクトルを設定(右上45度指定)
	//viewProjection_.up = { cosf(3.14f / 4.0f),sinf(3.14f / 4.0f),0.0f };

	//カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = 3.14f / 18.0f;

	////アスペクトル比を設定
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

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

	//Matrix4 matIdentity;
	//matIdentity.m[0][0] = 1;
	//matIdentity.m[1][1] = 1;
	//matIdentity.m[2][2] = 1;
	//matIdentity.m[3][3] = 1;

	///*{
	//	1,0,0,0
	//	0,1,0,0
	//	0,0,1,0
	//	0,0,0,1
	//}*/

	////スケーリング行列を宣言
	//Matrix4 matscale = matIdentity;

	//worldTransforms_[100].scale_ = { 1,1,1 };
	//worldTransforms_[100].rotation_ = { rotDist(engine), rotDist(engine), rotDist(engine)};
	////X,Y,Z軸の平行移動を設定
	//worldTransforms_[100].translation_ = { posDist(engine),posDist(engine),posDist(engine) };

	////合成用回転行列を宣言
	//Matrix4 matRot = matIdentity;

	////各軸用回転行列を宣言
	//Matrix4 matRotX = matIdentity;
	//Matrix4 matRotY = matIdentity;
	//Matrix4 matRotZ = matIdentity;

	////matscale.m[0][0] = worldTransforms_[100].scale_.x;
	////matscale.m[1][1] = worldTransforms_[100].scale_.y;
	////matscale.m[2][2] = worldTransforms_[100].scale_.z;
	////matscale.m[3][3] = 1;

	////Matrix4 matRotZ = matIdentity;

	///*matRotZ.m[0][0] = cos(worldTransforms_[100].rotation_.z);
	//matRotZ.m[0][1] = sin(worldTransforms_[100].rotation_.z);
	//matRotZ.m[1][0] = -sin(worldTransforms_[100].rotation_.z);
	//matRotZ.m[1][1] = cos(worldTransforms_[100].rotation_.z);

	//worldTransforms_[100].matWorld_ = matIdentity;
	//worldTransforms_[100].matWorld_ *= matRotZ;

	//worldTransforms_[100].TransferMatrix();*/

	////Matrix4 matRotX = matIdentity;

	///*matRotX.m[1][1] = cos(worldTransforms_[100].rotation_.x);
	//matRotX.m[1][2] = sin(worldTransforms_[100].rotation_.x);
	//matRotX.m[2][1] = -sin(worldTransforms_[100].rotation_.x);
	//matRotX.m[2][2] = cos(worldTransforms_[100].rotation_.x);

	//worldTransforms_[100].matWorld_ = matIdentity;
	//worldTransforms_[100].matWorld_ *= matRotX;

	//worldTransforms_[100].TransferMatrix();*/

	////Matrix4 matRotY = matIdentity;

	///*matRotY.m[0][0] = cos(worldTransforms_[100].rotation_.y);
	//matRotY.m[0][2] = -sin(worldTransforms_[100].rotation_.y);
	//matRotY.m[2][0] = sin(worldTransforms_[100].rotation_.y);
	//matRotY.m[2][2] = cos(worldTransforms_[100].rotation_.y);

	//worldTransforms_[100].matWorld_ = matIdentity;
	//worldTransforms_[100].matWorld_ *= matRotY;

	//worldTransforms_[100].TransferMatrix();*/

	//matRotZ.m[0][0] = cos(worldTransforms_[100].rotation_.z);
	//matRotZ.m[0][1] = sin(worldTransforms_[100].rotation_.z);
	//matRotZ.m[1][0] = -sin(worldTransforms_[100].rotation_.z);
	//matRotZ.m[1][1] = cos(worldTransforms_[100].rotation_.z);

	//matRotX.m[1][1] = cos(worldTransforms_[100].rotation_.x);
	//matRotX.m[1][2] = sin(worldTransforms_[100].rotation_.x);
	//matRotX.m[2][1] = -sin(worldTransforms_[100].rotation_.x);
	//matRotX.m[2][2] = cos(worldTransforms_[100].rotation_.x);

	//matRotY.m[0][0] = cos(worldTransforms_[100].rotation_.y);
	//matRotY.m[0][2] = -sin(worldTransforms_[100].rotation_.y);
	//matRotY.m[2][0] = sin(worldTransforms_[100].rotation_.y);
	//matRotY.m[2][2] = cos(worldTransforms_[100].rotation_.y);

	////各軸の回転行列を合成
	//matRot *= matRotZ;
	//matRot *= matRotX;
	//matRot *= matRotY;

	////平行移動行列を宣言
	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	//matTrans.m[3][0] = worldTransforms_[100].translation_.x;
	//matTrans.m[3][1] = worldTransforms_[100].translation_.y;
	//matTrans.m[3][2] = worldTransforms_[100].translation_.z;
	//matTrans.m[3][3] = 1;

	////行列の合成
	//worldTransforms_[100].matWorld_ = matIdentity;
	//worldTransforms_[100].matWorld_ *= matscale;
	//worldTransforms_[100].matWorld_ *= matRot;
	//worldTransforms_[100].matWorld_ *= matTrans;

	////行列の転送
	//worldTransforms_[100].TransferMatrix();

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

		worldTransforms_[Partid::kRoot].translation_ += move;

		debugText_->SetPos(50, 130);
		debugText_->Printf("translation[0] : %f,%f,%f",
			worldTransforms_[Partid::kRoot].translation_.x,
			worldTransforms_[Partid::kRoot].translation_.y,
			worldTransforms_[Partid::kRoot].translation_.z);
	}

	//大元から更新していく
	for (int i = 0; i < kNumPartid; i++) {
		Matrix4 matScale = CreateMatScale(worldTransforms_[i].scale_); // スケーリング行列作成

		Matrix4 matRotZ = CreateMatRotationZ(worldTransforms_[i].rotation_);
		Matrix4 matRotY = CreateMatRotationY(worldTransforms_[i].rotation_);
		Matrix4 matRotX = CreateMatRotationX(worldTransforms_[i].rotation_);

		Matrix4 lat = CreateMatTranslation(worldTransforms_[i].translation_);

		//スケーリング・回転・平行移動を合成した行列を計算
		worldTransforms_[i].matWorld_ = CreateMatIdentity();
		worldTransforms_[i].matWorld_ *= matScale; // worldTransform.matWorld_ *= CreateMatScale(worldTransform.scale_); これでも可
		worldTransforms_[i].matWorld_ *= matRotZ;
		worldTransforms_[i].matWorld_ *= matRotY;
		worldTransforms_[i].matWorld_ *= matRotX;
		worldTransforms_[i].matWorld_ *= CreateMatTranslation(worldTransforms_[i].translation_);

		// 子供のワールド行列に親のワールド行列かける parent_ = nullではないとき
		if (worldTransforms_[i].parent_ != nullptr) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}

		//行列の転送
		worldTransforms_[i].TransferMatrix();
	}

	//上半身回転処理
	{
		Vector3 turn = { 0,0.1f,0 };
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransforms_[Partid::kChest].rotation_ += turn;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransforms_[Partid::kChest].rotation_ -= turn;
		}
	}

	//下半身回転処理
	{
		Vector3 turn = { 0,0.1f,0 };
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			worldTransforms_[Partid::kHip].rotation_ += turn;
		}
		else if (input_->PushKey(DIK_K)) {
			worldTransforms_[Partid::kHip].rotation_ -= turn;
		}
	}
	////-------クリップ距離変更処理-------////
	//{
	//	//上下キーニアクリップ距離を増減
	//	if (input_->PushKey(DIK_UP)) {
	//		viewProjection_.nearZ += 0.1f;
	//	}
	//	else if (input_->PushKey(DIK_DOWN)) {
	//		viewProjection_.nearZ -= 0.1f;
	//	}

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
	//	debugText_->SetPos(50, 130);
	//	debugText_->Printf("nearZ : %f", viewProjection_.nearZ);
	//}


	////--------Fov変更処理-------//
	//{
	//	//上キーで視野角を広げる
	//	if (input_->PushKey(DIK_UP)) {
	//		viewProjection_.fovAngleY += 0.01f;
	//		if (viewProjection_.fovAngleY >= 3.14f) {
	//			viewProjection_.fovAngleY = 3.14f;
	//		}
	//	}
	//	else if(input_->PushKey(DIK_DOWN)) {
	//		viewProjection_.fovAngleY -= 0.01f;
	//		if (viewProjection_.fovAngleY <= 0.01f) {
	//			viewProjection_.fovAngleY = 0.01f;
	//		}
	//	}

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
	//	debugText_->SetPos(50, 110);
	//	debugText_->Printf("fovAngleY(Degree):%f",180 / 3.14f *(viewProjection_.fovAngleY));
	//}

	////-------視点移動処理-------//
	//{

	//	Vector3 cameramove = Vector3();

	//	//視点の移動の速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		cameramove.z += kEyeSpeed;
	//	}
	//	else if (input_->PushKey(DIK_S)) {
	//		cameramove.z -= kEyeSpeed;
	//	}

	//	//視点移動(ベクトルの加算)
	//	viewProjection_.eye += cameramove;
	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
	//	debugText_->SetPos(50, 50);
	//	debugText_->Printf("eye:(%f,%f,%f)",
	//					   viewProjection_.eye.x,
	//					   viewProjection_.eye.y,
	//					   viewProjection_.eye.z);
	//}

	////-------注視点移動処理-------//
	//{

	//	Vector3 targetmove = Vector3();

	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		targetmove.x -= kTargetSpeed;
	//	}
	//	else if (input_->PushKey(DIK_RIGHT)) {
	//		targetmove.x += kTargetSpeed;
	//	}
	//	//注視点移動(ベクトルの加算)
	//	viewProjection_.target += targetmove;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf("target:(%f,%f,%f)",
	//					   viewProjection_.target.x,
	//					   viewProjection_.target.y,
	//					   viewProjection_.target.z);
	//}

	////-------上方向回転処理-------//
	//{
	//	//上方向の回転の速さ[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, 3.14f * 2.0f);
	//	}

	//	//上方向ベクトルを計算(半径1の円周上の座標)
	//	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック用表示
	//	debugText_->SetPos(50, 90);
	//	debugText_->Printf("up:(%f,%f,%f)",
	//					   viewProjection_.up.x,
	//					   viewProjection_.up.y,
	//					   viewProjection_.up.z);
	//}
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

	for (int i = 0; i < kNumPartid; i++) {
		if (i == 0) {
			continue;
		}
		if (i == 1) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}


	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	/*PrimitiveDrawer::GetInstance()->DrawLine3d({ 0,-10,0 }, { 0,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 1,-10,0 }, { 1,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 2,-10,0 }, { 2,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 3,-10,0 }, { 3,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 4,-10,0 }, { 4,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 5,-10,0 }, { 5,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 6,-10,0 }, { 6,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 7,-10,0 }, { 7,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 8,-10,0 }, { 8,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 9,-10,0 }, { 9,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 10,-10,0 }, { 10,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -1,-10,0 }, { -1,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -2,-10,0 }, { -2,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -3,-10,0 }, { -3,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -4,-10,0 }, { -4,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -5,-10,0 }, { -5,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -6,-10,0 }, { -6,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -7,-10,0 }, { -7,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -8,-10,0 }, { -8,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -9,-10,0 }, { -9,10,0 }, { 1,0,0,1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ -10,-10,0 }, { -10,10,0 }, { 1,0,0,1 });*/




	////3Dモデル
	//model_->Draw(worldTransforms_[100], viewProjection_, textureHandle_);

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