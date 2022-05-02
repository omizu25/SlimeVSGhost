//--------------------------------------------------
//
// ACG制作 ( item.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _ITEM_H_		//このマクロ定義がされてなかったら
#define _ITEM_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "player.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define ITEM_SIZE		(70)		//アイテムのサイズ

//-------------------------
//敵の種類を定義
//-------------------------
typedef enum
{
	ITEMTYPE_BLOCK = 0,		//ブロック
	ITEMTYPE_STAR,			//スター
	ITEMTYPE_MAX
}ITEMTYPE;

//--------------------------------------------------
//アイテムの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	D3DXVECTOR3		move;			//移動量
	ITEMTYPE		type;			//種類
	float			fWidth;			//幅
	float			fHeight;		//高さ
	float			fRot;			//回転
	float			fLength;		//対角線の長さ
	float			fAngle;			//対角線の角度
	bool			bDirection;		//向き [false  : 左向き true  : 右向き]
	bool			bUse;			//使用しているかどうか
}Item;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, bool bDirection);
Item *GetItem(void);
bool GetDeath(void);
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fHeight);
void InhaleItem(D3DXVECTOR3 pos, ATTACKSTATE *pAttack, float fWidth, float fHeight, bool bDirection);

#endif // !_Item_H_