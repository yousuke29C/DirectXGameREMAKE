#include "worldtransform.h"
#include <cmath>
/// <summary>
/// 単位行列作成
/// </summary>
/// <returns>単位行列</returns>
Matrix4 CreateMatIdentity();

/// <summary>
/// スケーリング行列の作成
/// </summary>
/// <param name="scale">WorldTransformのscale_</param>
/// <returns>作成したスケーリング行列</returns>
Matrix4 CreateMatScale(Vector3 scale);

/// <summary>
/// Z軸の回転行列作成
/// </summary>
/// <param name="rotation">WorldTransformのrotation_</param>
/// <returns>Z軸周りの回転行列</returns>
Matrix4 CreateMatRotationZ(Vector3 rotation);

/// <summary>
/// X軸の回転行列作成
/// </summary>
/// <param name="rotation">WorldTransformのrotation_</param>
/// <returns>X軸の回転行列</returns>
Matrix4 CreateMatRotationX(Vector3 rotation);


/// <summary>
/// Y軸の回転行列
/// </summary>
/// <param name="rotation">WorldTransformのrotation_</param>
/// <returns>Y軸の回転行列</returns>
Matrix4 CreateMatRotationY(Vector3 rotation);


/// <summary>
/// 平行移動
/// </summary>
/// <param name="translation">WorldTransformのTranslation_</param>
/// <returns>平行移動</returns>
Matrix4 CreateMatTranslation(Vector3 translation);

Vector3 bvector(Vector3& velocity, Matrix4& mat);

// 速度ベクトルを自機の向きに合わせて回転させる
Vector3 CreateVector(Vector3 velocity, WorldTransform& worldTransform);

void CreateMatrixUpdate(WorldTransform& worldTransform_);

Vector3 Vector3sub(Vector3& v1, Vector3& v2);

Vector3 Normalize(Vector3& vec);

float length(Vector3& vec);