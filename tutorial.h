//--------------------------------------------------
//
// ACG制作 ( tutorial.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _TUTORIAL_H_		//このマクロ定義がされてなかったら
#define _TUTORIAL_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif // !_TUTORIAL_H_