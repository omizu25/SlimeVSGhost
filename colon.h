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
void SetColon(D3DXVECTOR3 pos, float fWidth, float fHeight);

#endif // !_COLON_H_