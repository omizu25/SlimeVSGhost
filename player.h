//--------------------------------------------------
//
// STG制作 ( player.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//マクロ定義
//-------------------------
#define PLAYER_WIDTH		(150)		//プレイヤーの幅
#define PLAYER_HEIGHT		(120)		//プレイヤーの高さ

//-------------------------
//プレイヤーの構造体を定義
//-------------------------
typedef struct
{
	D3DXVECTOR3			pos;			//位置
	D3DXVECTOR3			move;			//移動量
	D3DXVECTOR3			rot;			//向き
	float				fLength;		//対角線の長さ
	float				fAngle;			//対角線の角度
	int					nLife;			//体力
	int					nStock;			//残機
	bool				bDisp;			//表示しているかどうか
}Player;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

#endif // !_PLAYER_H_