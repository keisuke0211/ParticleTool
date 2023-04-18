//========================================
// 
// パーティクルの計算のヘッダファイル
// author KEISUKE OOTONO
// 
//========================================
// *** particle_calc.h ***
//========================================
#ifndef _PARTICLE_CALC_H_
#define _PARTICLE_CALC_H_  
#include "../main.h"
#include "particle.h"
#include "particle_type.h"
//****************************************
// 構造体の定義
//****************************************

// ベクトル
struct Vector3 {
	double x, y, z;
	// コンストラクタ
	Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
	// ベクトルの大きさを返す関数
	double length() const {
		return sqrt(x * x + y * y + z * z);
	}
	// ベクトルを単位ベクトルに正規化する関数
	void normalize() {
		double len = length();
		x /= len;
		y /= len;
		z /= len;
	}
};
//****************************************
// プロトタイプ宣言
//****************************************
// 速度計算
void SpeedCalc(Particle *pPar, Particle_Sub *pParSub,ParticleType *pPType, int nCntPar);
// 向きベクトルを法線ベクトルに変換する関数
Vector3 directionToNormal(const Vector3& dir);
// 3次元ベクトルの外積を計算する関数
Vector3 cross(const Vector3& v1, const Vector3& v2);
#endif