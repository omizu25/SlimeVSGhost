//--------------------------------------------------
//
// ACG制作 ( colon.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _COLON_H_		//このマクロ定義がされてなかったら
#define _COLON_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitColon(void);
void UninitColon(void);
void UpdateColon(void);
void DrawColon(void);
void SetColon(D3DXVECTOR3 pos, float fWidth, float fHeight, int nRank);
void RankColon(D3DXCOLOR col, int nRank);
void MoveColon(D3DXVECTOR3 move, int nRank);

#endif // !_COLON_H_