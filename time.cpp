//--------------------------------------------------
//
// ACG制作 ( time.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "colon.h"
#include "game.h"
#include "number.h"
#include "result.h"
#include "time.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define NUMBER_WIDTH		(30.0f)		//数の幅
#define NUMBER_HEIGHT		(50.0f)		//数の高さ
#define WIDTH_INTERVAL		(20.0f)		//幅の間隔

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static D3DXVECTOR3		s_pos;			//位置
static int				s_nTime;		//タイムの値

//--------------------------------------------------
//タイムの初期化処理
//--------------------------------------------------
void InitTime(void)
{
	float fWidth = SCREEN_WIDTH / MAX_X_BLOCK;
	float fHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK) * 0.5f;

	//位置を初期化する
	s_pos = D3DXVECTOR3(fWidth * 30.0f, fHeight * 2.0f, 0.0f);

	s_nTime = 0;

	int nTime = s_nTime;
	int aNumber[MAX_TIME];

	for (int i = 0; i < MAX_TIME; i++)
	{//１桁ずつに分ける
		aNumber[i] = nTime % 10;
		nTime /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * (i / 2));

		//数の設定処理
		SetRightNumber(D3DXVECTOR3(s_pos.x - fInterval, s_pos.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, 0);

		if ((i % 2 == 0) && (i != 0))
		{//２の倍数
			//コロンの設定処理
			SetColon(D3DXVECTOR3(s_pos.x - fInterval + WIDTH_INTERVAL, s_pos.y, 0.0f), WIDTH_INTERVAL, NUMBER_HEIGHT * 0.5f, 0);
		}
	}
}

//--------------------------------------------------
//タイムの加算処理
//--------------------------------------------------
void AddTime(int nValue)
{
	s_nTime += nValue;

	if (s_nTime % 10000 == 6000)
	{//１分ごと
		s_nTime += 4000;
	}

	if (s_nTime % 1000000 >= 300000)
	{//３０分以上
		//リザルトの設定処理
		SetResult(RESULT_LOSE);

		//ゲームの設定処理
		SetGameState(GAMESTATE_END, 45);
	}

	int nTime = s_nTime;
	int i = 0;
	int aNumber[MAX_TIME];

	while (nTime != 0)
	{//１桁ずつに分ける
		aNumber[i] = nTime % 10;
		nTime /= 10;

		//数のテクスチャの処理
		TexNumber(aNumber[i], i);

		i++;
	}
}

//--------------------------------------------------
//タイムの取得
//--------------------------------------------------
int GetTime(void)
{
	return s_nTime;
}