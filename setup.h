//--------------------------------------------------
//
// ACG制作 ( setup.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _SETUP_H_		//このマクロ定義がされてなかったら
#define _SETUP_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//プロトタイプ宣言
//-------------------------
void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
void Setcol(VERTEX_2D *pVtx, float Red, float Green, float Blue, float Alpha);
void Settex(VERTEX_2D *pVtx, float ULeft, float URight, float VTop, float VBottom);
void Initpos(VERTEX_2D *pVtx);
void Initrhw(VERTEX_2D *pVtx);
void Initcol(VERTEX_2D *pVtx);
void Inittex(VERTEX_2D *pVtx);
void InitAll(VERTEX_2D *pVtx);

#endif // !_SETUP_H_