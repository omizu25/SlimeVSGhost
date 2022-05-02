//--------------------------------------------------
//
// ACG制作 ( time.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _TIME_H_		//このマクロ定義がされてなかったら
#define _TIME_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
int GetTime(void);

#endif // !_TIME_H_