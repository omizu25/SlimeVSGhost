//--------------------------------------------------
//
// ACG制作 ( player.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define REFLECT_BOUND		(0.8f)		//バウンドの跳ね返り量

//--------------------------------------------------
//ジャンプの状態を定義
//--------------------------------------------------
typedef enum
{
	JUMPSTATE_NONE = 0,		//何もしていない状態
	JUMPSTATE_JUMP,			//ジャンプ状態
	JUMPSTATE_BOUND,		//バウンド状態
	JUMPSTATE_MAX
}JUMPSTATE;

//--------------------------------------------------
//プレイヤーの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//現在の位置
	D3DXVECTOR3		posOld;				//前回の位置
	D3DXVECTOR3		move;				//移動量
	JUMPSTATE		jump;				//ジャンプ状態
	float			fWidth;				//幅
	float			fHeight;			//高さ
	int				nCounterState;		//状態管理カウンター
}Player;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
bool GetTexUsePlayer(void);

#endif // !_PLAYER_H_