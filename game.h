//--------------------------------------------------
//
// ACG制作 ( game.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _GAME_H_		//このマクロ定義がされてなかったら
#define _GAME_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//-------------------------
//ゲームの状態
//-------------------------
typedef enum
{
	GAMESTATE_NONE = 0,		//何もしていない状態
	GAMESTATE_START,		//開始状態(ゲーム開始中)
	GAMESTATE_NORMAL,		//通常状態(ゲーム進行中)
	GAMESTATE_END,			//終了状態(ゲーム終了時)
	GAMESTATE_MAX
}GAMESTATE;

//-------------------------
//プロトタイプ宣言
//-------------------------
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounterState);
void SetEnablePause(bool bPause);

#endif // !_GAME_H_