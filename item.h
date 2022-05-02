//--------------------------------------------------
//
// ACG制作 ( item.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _ITEM_H_		//このマクロ定義がされてなかったら
#define _ITEM_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define ITEM_SIZE		(70)		//アイテムのサイズ

//--------------------------------------------------
//アイテムの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	float			fWidth;			//幅
	float			fHeight;		//高さ
	bool			bUse;			//使用しているかどうか
}Item;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos);
Item *GetItem(void);
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif // !_Item_H_