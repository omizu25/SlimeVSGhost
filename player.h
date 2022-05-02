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
//ジャンプの状態を定義
//--------------------------------------------------
typedef enum
{
	JUMPSTATE_NONE = 0,		//何もしていない状態
	JUMPSTATE_JUMP,			//ジャンプ状態
	JUMPSTATE_LAND,			//着地状態
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
	PLAYERSTATE_STAR,			//無敵状態
	PLAYERSTATE_DEATH,			//死亡状態
	PLAYERSTATE_PV,				//PV状態
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
	float			fRot;				//回転
	float			fLength;			//対角線の長さ
	float			fAngle;				//対角線の角度
	float			fCol;				//カラー
	int				nCounterState;		//状態管理カウンター
	int				nCounterMotion;		//モーションカウンター
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
void SetPlayer(PLAYERSTATE state);
Player *GetPlayer(void);
void HitPlayer(int nDamage);

#endif // !_PLAYER_H_