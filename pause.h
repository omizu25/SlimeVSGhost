//--------------------------------------------------
//
// ACG制作 ( pause.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PAUSE_H_		//このマクロ定義がされてなかったら
#define _PAUSE_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//マクロ定義
//-------------------------
#define MAX_PAUSE				(5)			//ポーズの最大数
#define NUM_PAUSE				(2)			//ポーズの種類
#define PAUSE_WIDTH				(200)		//ポーズの幅の半分
#define PAUSE_HEIGHT			(200)		//ポーズの高さの半分
#define PAUSEMENU_WIDTH			(215)		//ポーズの選択肢の幅
#define PAUSEMENU_HEIGHT		(70)		//ポーズの選択肢の高さ

//-------------------------
//ポーズのモードを定義
//-------------------------
typedef enum
{
	PAUSEMENU_GAME = 0,		//ゲームに戻る
	PAUSEMENU_RETRY,		//ゲームをやり直す
	PAUSEMENU_TITLE,		//タイトル画面に戻る
	PAUSEMENU_MAX
}PAUSEMENU;

//-------------------------
//ポーズの選択肢の状態(点滅具合)を定義
//-------------------------
typedef enum
{
	PAUSESTATE_IN = 0,		//見える
	PAUSESTATE_OUT,			//見えない
	PAUSESTATE_MAX
}PAUSESTATE;

//-------------------------
//ポーズの構造体を定義
//-------------------------
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXCOLOR		col;		//色
	PAUSESTATE		state;		//状態状態(点滅具合)
}Pause;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif // !_PAUSE_H_