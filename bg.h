//--------------------------------------------------
//
// ACG制作 ( bg.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _BG_H_		//このマクロ定義がされてなかったら
#define _BG_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//マクロ定義
//-------------------------
#define MOVE_BG		(3)		//動く背景の数
#define NUM_BG		(2)		//背景の種類

//-------------------------
//ゲームの状態
//-------------------------
typedef enum
{
	BATTLEBG_NORMAL = 0,		//通常戦
	BATTLEBG_BOSS,				//ボス戦
	BATTLEBG_APPEAR,			//出現状態
	BATTLEBG_MAX
}BATTLEBG;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void SetBG(BATTLEBG battle);
BATTLEBG GetBG(void);

#endif // !_BG_H_