//--------------------------------------------------
//
// ACG制作 ( block.h )
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
#define TOP_BLOCK					(7)					//上の段のブロック
#define MIDDLE_BLOCK				(12)				//真ん中の段のブロック
#define BOTTOM_BLOCK				(17)				//下の段のブロック

//--------------------------------------------------
//ブロックの種類を定義
//--------------------------------------------------
typedef enum
{
	BLOCKTYPE_BLUE = 0,			//青
	BLOCKTYPE_LIGHT_BLUE,		//水色
	BLOCKTYPE_B_THROUGH,		//青の通り抜ける
	BLOCKTYPE_L_THROUGH,		//水色の通り抜ける
	BLOCKTYPE_NONE,
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
	bool			bCollision;		//当たるかどうか
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
void CollisionTopBlock(D3DXVECTOR3 *pPos, float fWidth, float fHeight);

#endif // !_Block_H_