//--------------------------------------------------
//
// アクションゲーム制作 ( block.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _BLOCK_H_		//このマクロ定義がされてなかったら
#define _BLOCK_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_X_BLOCK					(32)				//ブロックの横の最大数
#define MAX_Y_BLOCK					(18)				//ブロックの縦の最大数
#define MAX_BLOCK		(MAX_Y_BLOCK * MAX_X_BLOCK)		//ブロックの最大数

//--------------------------------------------------
//ブロックの種類を定義
//--------------------------------------------------
typedef enum
{
	BLOCKTYPE_BROWN = 0,		//茶色
	BLOCKTYPE_ORANGE,			//オレンジ
	BLOCKTYPE_NONE,				//使用しない
	BLOCKTYPE_MAX
}BLOCKTYPE;

//--------------------------------------------------
//ブロックの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	float			fWidth;			//幅
	float			fHeight;		//高さ
	BLOCKTYPE		type;			//種類
	bool			bUse;			//使用しているかどうか
}Block;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);
Block *GetBlock(void);
bool GetTexUseBlock(void);
bool GetCollisionUse(void);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif // !_Block_H_