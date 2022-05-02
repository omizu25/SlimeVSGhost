//--------------------------------------------------
//
// アクションゲーム制作 ( map.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _MAP_H_		//このマクロ定義がされてなかったら
#define _MAP_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//マップの種類を定義
//--------------------------------------------------
typedef enum
{
	MAPTYPE_PURPLE = 0,		//紫
	MAPTYPE_BROWN,			//茶色
	MAXTYPE_BLUE,			//青
	MAPTYPE_MAX
}MAPTYPE;

//--------------------------------------------------
//マップの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		move;		//移動量
	MAPTYPE			type;		//種類
}Map;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
Map *GetMap(void);

#endif // !_Map_H_