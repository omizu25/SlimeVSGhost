//--------------------------------------------------
//
// ACG制作 ( number.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _NUMBER_H_		//このマクロ定義がされてなかったら
#define _NUMBER_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(void);
void SetNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, int nRank);
void TexNumber(int nNumber, int nDigit);
void RankNumber(int nRank);
void MoveNumber(D3DXVECTOR3 move, int nNumber, int nDigit, int nRank);

#endif // !_NUMBER_H_