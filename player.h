//--------------------------------------------------
//
// アクションゲーム制作 ( player.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//プレイヤーの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//現在の位置
	D3DXVECTOR3		posOld;				//前回の位置
	D3DXVECTOR3		move;				//移動量
	bool			bjump;				//ジャンプ状態 [してる  : true してない  : false]
	int				nCounterAnim;		//アニメーションカウンター
	int				nPatternAnim;		//アニメーションパターンNo.
	int				nDirectionMove;		//向き [0  : 右向き 1  : 左向き]
}Player;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player GetPlayer(void);


#endif // !_PLAYER_H_