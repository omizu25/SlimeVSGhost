//--------------------------------------------------
//
// STG制作 ( result.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _RESULT_H_		//このマクロ定義がされてなかったら
#define _RESULT_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//マクロ定義
//-------------------------
#define MAX_ROUTE		(5)		//ルートの最大数

//-------------------------
//ルートの結末を定義
//-------------------------
typedef enum
{
	ROUTE_NONE = -1,	//まだルートが決まってない
	ROUTE_TRUE = 0,		//トゥルーエンド(通常)
	ROUTE_HAPPY,		//ハッピーエンド(ボスだけ倒す)
	ROUTE_ALL_KILL,		//オールキルエンド(全員倒す)
	ROUTE_NO_KILL,		//NOキルエンド(ボスすら倒さない)
	ROUTE_ESCAPE,		//ボスに逃げられる(時間切れ)
	ROUTE_BAD,			//バッドエンド(死)
	ROUTE_HAPPYBAD,		//ハッピーエンドでの死
	ROUTE_MAX
}ROUTE;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(ROUTE Route);
ROUTE GetResult(void);

#endif // !_RESULT_H_