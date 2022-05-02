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
//背景の構造体を定義
//-------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	float			fWidth;			//幅
	float			fHeight;		//高さ
}BG;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif // !_BG_H_