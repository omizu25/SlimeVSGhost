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
//攻撃の状態を定義
//--------------------------------------------------
typedef enum
{
	ATTACKSTATE_NONE = 0,		//何もしていない状態
	ATTACKSTATE_IN,				//吸い込み状態
	ATTACKSTATE_STORE,			//蓄えている状態
	ATTACKSTATE_OUT,			//吐き出し状態
	ATTACKSTATE_MAX
}ATTACKSTATE;

//-------------------------
//プレイヤーの状態を定義
//-------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//通常状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//--------------------------------------------------
//プレイヤーの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//現在の位置
	D3DXVECTOR3		posOld;				//前回の位置
	D3DXVECTOR3		move;				//移動量
	JUMPSTATE		jump;				//ジャンプ状態
	ATTACKSTATE		attack;				//攻撃状態
	PLAYERSTATE		state;				//状態
	float			fWidth;				//幅
	float			fHeight;			//高さ
	int				nCounterState;		//状態管理カウンター
	int				nCounterAttack;		//攻撃カウンター
	int				nLife;				//寿命
	bool			bDirection;			//向き [false  : 左向き true  : 右向き]
}Player;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);
Player *GetPlayer(void);
bool GetTexUsePlayer(void);

#endif // !_PLAYER_H_