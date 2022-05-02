//--------------------------------------------------
//
// ACG制作 ( ranking.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _RANKING_H_		//このマクロ定義がされてなかったら
#define _RANKING_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SaveRanking(void);
void SetRanking(int nRankTime);
void InitRankUpdate(void);

#endif // !_RANKING_H_