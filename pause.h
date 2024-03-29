//--------------------------------------------------
//
// ACG制作 ( pause.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PAUSE_H_		//このマクロ定義がされてなかったら
#define _PAUSE_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif // !_PAUSE_H_