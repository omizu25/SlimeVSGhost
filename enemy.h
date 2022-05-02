//--------------------------------------------------
//
// ACG制作 ( enemy.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _ENEMY_H_		//このマクロ定義がされてなかったら
#define _ENEMY_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define ENEMY_WIDTH			(100.0f)		//敵の幅
#define ENEMY_HEIGHT		(100.0f)		//敵の高さ

//-------------------------
//敵の種類を定義
//-------------------------
typedef enum
{
	ENEMYTYPE_BOY = 0,		//男の子 (リリ)
	ENEMYTYPE_GIRL,			//女の子 (ルル)
	ENEMYTYPE_MAX
}ENEMYTYPE;

//-------------------------
//敵の状態を定義
//-------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//通常状態
	ENEMYSTATE_DAMAGE,			//ダメージ状態
	ENEMYSTATE_DEATH,			//死亡状態
	ENEMYSTATE_PV,				//PV状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//-------------------------
//敵のポップ場所を定義
//-------------------------
typedef enum
{
	ENEMYPOP_TOP = 0,		//上の段
	ENEMYPOP_MIDDLE,		//真ん中の段
	ENEMYPOP_BOTTOM,		//下の段
	ENEMYPOP_MAX
}ENEMYPOP;

//-------------------------
//敵の構造体を定義
//-------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//位置
	D3DXVECTOR3		posOld;				//前回の位置
	D3DXVECTOR3		move;				//移動量
	D3DXCOLOR		col;				//色
	ENEMYTYPE		type;				//種類
	ENEMYSTATE		state;				//状態
	ENEMYPOP		pop;				//ポップ場所
	float			fWidth;				//幅
	float			fHeight;			//高さ
	int				nCounterState;		//状態管理カウンター
	int				nCounterStop;		//ストップカウンター
	int				nCounterAnim;		//アニメーションカウンター
	int				nPatternAnim;		//アニメーションパターンNo.
	int				nLife;				//体力
	bool			bDirection;			//向き [false  : 左向き true  : 右向き]
	bool			bUse;				//使用しているかどうか
}Enemy;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type);
void SetEnemyState(ENEMYSTATE state);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);

#endif // !_ENEMY_H_