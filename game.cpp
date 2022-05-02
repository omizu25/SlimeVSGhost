//--------------------------------------------------
//
// ACG制作 ( game.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "bg.h"
#include "block.h"
#include "colon.h"
#include "effect.h"
#include "enemy.h"
#include "fade.h"
#include "game.h"
#include "gauge.h"
#include "input.h"
#include "item.h"
#include "number.h"
#include "pause.h"
#include "player.h"
#include "result.h"
#include "time.h"

#include <assert.h>

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static GAMESTATE		s_gameState = GAMESTATE_NONE;		//ゲームの状態
static int				s_nCounterState;					//状態管理カウンター
static bool				s_bPause = false;					//ポーズ中かどうか [してる  : true してない  : false]

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void SetPop(void);

//--------------------------------------------------
//ゲームの初期化処理
//--------------------------------------------------
void InitGame(void)
{
	//ポーズの初期化処理
	InitPause();

	//背景の初期化処理
	InitBG();

	//ゲージの初期化処理
	InitGauge();

	//数の初期化処理
	InitNumber();

	//コロンの初期化処理
	InitColon();

	//タイムの初期化処理
	InitTime();

	//ブロックの初期化処理
	InitBlock();

	//アイテムの初期化処理
	InitItem();

	//敵の初期化処理
	InitEnemy();

	//ポップ処理
	SetPop();

	//エフェクトの初期化処理
	//InitEffect();

	//プレイヤーの初期化処理
	InitPlayer();

	s_gameState = GAMESTATE_NORMAL;		//通常状態に設定

	s_nCounterState = 0;				//カウンターの初期化

	s_bPause = false;					//ポーズ解除

	//リザルトの設定処理
	SetResult(RESULT_NONE);
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

	//アイテムの終了処理
	UninitItem();

	//敵の終了処理
	UninitEnemy();

	//エフェクトの終了処理
	//UninitEffect();

	//プレイヤーの終了処理
	UninitPlayer();

	//ゲージの終了処理
	UninitGauge();

	//数の終了処理
	UninitNumber();

	//コロンの終了処理
	UninitColon();
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
		case GAMESTATE_NONE:		//何もしていない状態

			break;

		case GAMESTATE_NORMAL:		//通常状態

			//タイムの加算処理
			AddTime(1);

			//背景の更新処理
			UpdateBG();

			//ブロックの更新処理
			UpdateBlock();

			//アイテムの更新処理
			UpdateItem();

			//敵の更新処理
			UpdateEnemy();

			//エフェクトの更新処理
			//UpdateEffect();

			//プレイヤーの更新処理
			UpdatePlayer();

			//ゲージの更新処理
			UpdateGauge();

			//数の更新処理
			UpdateNumber();

			//コロンの更新処理
			UpdateColon();

			break;

		case GAMESTATE_END:			//終了状態

			//ゲージの更新処理
			UpdateGauge();

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

	//アイテムの描画処理
	DrawItem();

	//敵の描画処理
	DrawEnemy();

	//エフェクト描画処理
	//DrawEffect();

	//プレイヤーの描画処理
	DrawPlayer();

	//ゲージの描画処理
	DrawGauge();

	//数の描画処理
	DrawNumber();

	//コロンの描画処理
	DrawColon();

	if (s_bPause)
	{//ポーズ中
#ifdef DEBUG

		//ポーズの描画処理
		DrawPause();

#endif // DEBUG
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

//--------------------------------------------------
//ポップ処理
//--------------------------------------------------
static void SetPop(void)
{
	float fEnemyWidth = ENEMY_WIDTH * 0.5f;
	float fBlockHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK);

	D3DXVECTOR3 posBoy  = D3DXVECTOR3(fEnemyWidth, fBlockHeight * TOP_BLOCK, 0.0f);
	D3DXVECTOR3 posGirl = D3DXVECTOR3(SCREEN_WIDTH - fEnemyWidth, fBlockHeight * BOTTOM_BLOCK, 0.0f);

	//敵の設定処理
	SetEnemy(posBoy, ENEMYTYPE_BOY);
	SetEnemy(posGirl, ENEMYTYPE_GIRL);
}