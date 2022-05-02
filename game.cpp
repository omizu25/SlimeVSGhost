//--------------------------------------------------
//
// ACG制作 ( game.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "bg.h"
#include "block.h"
#include "enemy.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "pause.h"
#include "player.h"

#include <assert.h>

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static GAMESTATE		s_gameState = GAMESTATE_NONE;		//ゲームの状態
static int				s_nCounterState;					//状態管理カウンター
static bool				s_bPause = false;					//ポーズ中かどうか [してる  : true してない  : false]

//--------------------------------------------------
//ゲームの初期化処理
//--------------------------------------------------
void InitGame(void)
{
	//ポーズの初期化処理
	InitPause();

	//背景の初期化処理
	InitBG();

	//ブロックの初期化処理
	InitBlock();

	//敵の初期化処理
	InitEnemy();

	float fYellowWidth = ENEMY_WIDTH * 0.5f;
	float fPurpleWidth = SCREEN_WIDTH - (ENEMY_WIDTH * 0.5f);
	float fYellowHeught = (SCREEN_HEIGHT / MAX_Y_BLOCK) * TOP_BLOCK;
	float fPurpleHeught = (SCREEN_HEIGHT / MAX_Y_BLOCK) * BOTTOM_BLOCK;

	//敵の設定処理
	SetEnemy(D3DXVECTOR3(fYellowWidth, fYellowHeught, 0.0f), ENEMYTYPE_YELLOW);
	SetEnemy(D3DXVECTOR3(fPurpleWidth, fPurpleHeught, 0.0f), ENEMYTYPE_PURPLE);

	//プレイヤーの初期化処理
	InitPlayer();

	s_gameState = GAMESTATE_NORMAL;		//通常状態に設定

	s_nCounterState = 0;				//カウンターの初期化

	s_bPause = false;					//ポーズ解除
}

//--------------------------------------------------
//ゲームの終了処理
//--------------------------------------------------
void UninitGame(void)
{
	//ポーズの終了処理
	UninitPause();

	//背景の終了処理
	UninitBG();

	//ブロックの終了処理
	UninitBlock();

	//敵の終了処理
	UninitEnemy();

	//プレイヤーの終了処理
	UninitPlayer();
}

//--------------------------------------------------
//ゲームの更新処理
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{//ポーズキー(Pキー)が押されたかどうか
		s_bPause = !s_bPause;

		//ポーズの初期化
		InitPause();
	}

	if (!s_bPause)
	{//ポーズしてない
		switch (s_gameState)
		{
		case GAMESTATE_NORMAL:		//通常状態

			//背景の更新処理
			UpdateBG();

			//ブロックの更新処理
			UpdateBlock();

			//敵の更新処理
			UpdateEnemy();

			//プレイヤーの更新処理
			UpdatePlayer();

			break;

		case GAMESTATE_END:			//終了状態
			s_nCounterState++;

			if (s_nCounterState >= 45)
			{
				s_gameState = GAMESTATE_NONE;		//何もしていない状態に設定

				//フェードの設定
				SetFade(MODE_RESULT);
			}

			break;

		default:
			assert(false);
			break;
		}
	}
	else
	{//ポーズ中
		//ポーズの更新処理
		UpdatePause();
	}
}

//--------------------------------------------------
//ゲームの描画処理
//--------------------------------------------------
void DrawGame(void)
{
	//背景の描画処理
	DrawBG();

	//ブロックの描画処理
	DrawBlock();

	//敵の描画処理
	DrawEnemy();

	//プレイヤーの描画処理
	DrawPlayer();

	if (s_bPause)
	{//ポーズ中
		//ポーズの描画処理
		//DrawPause();
	}
}

//--------------------------------------------------
//ゲームの設定処理
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_gameState = state;

	s_nCounterState = 0;
}

//--------------------------------------------------
//ポーズの有効無効設定
//--------------------------------------------------
void SetEnablePause(bool bPause)
{
	s_bPause = bPause;
}