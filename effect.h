//--------------------------------------------------
//
// ACG制作 ( effect.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _EFFECT_H_		//このマクロ定義がされてなかったら
#define _EFFECT_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//エフェクトの種類を定義
//--------------------------------------------------
typedef enum
{
	EFFECTTYPE_IN = 0,			//吸い込み
	EFFECTTYPE_STAR,			//スター
	EFFECTTYPE_MOVE,			//移動
	EFFECTTYPE_EXPLOSION,		//爆発
	EFFECTTYPE_MAX
}EFFECTTYPE;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(EFFECTTYPE type);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, EFFECTTYPE type);
void SetParticle(D3DXVECTOR3 pos, EFFECTTYPE type, bool bDirection);

#endif // !_EFFECT_H_