//--------------------------------------------------
//
// ACG制作 ( ranking.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "colon.h"
#include "fade.h"
#include "input.h"
#include "number.h"
#include "ranking.h"
#include "setup.h"
#include "time.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define FILE_NAME			"data/Ranking.txt"		//ランキングのパス
#define MAX_RANKING					(5)				//ランキングの最大数
#define MAX_DATA			(MAX_RANKING + 1)		//データ(箱)の最大数
#define NUMBER_WIDTH				(60.0f)			//数の幅
#define NUMBER_HEIGHT				(100.0f)		//数の高さ
#define WIDTH_INTERVAL				(40.0f)			//幅の間隔
#define HEIGHT_INTERVAL				(20.0f)			//高さの間隔

//--------------------------------------------------
//ランキングの状態(点滅具合)を定義
//--------------------------------------------------
typedef enum
{
	RANKINGSTATE_IN = 0,		//見える
	RANKINGSTATE_OUT,			//見えない
	RANKINGSTATE_MAX
}RANKINGSTATE;

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//頂点バッファのポインタ
static int							s_aRanking[MAX_RANKING];		//ランキングの情報
static D3DXVECTOR3					s_pos;							//位置
static RANKINGSTATE					s_state;						//状態
static int							s_nRankingUpdate;				//更新ランクNo.

//--------------------------------------------------
//ランキングの初期化処理
//--------------------------------------------------
void InitRanking(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, pos, fWidth, fHeight);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 1.0f, 1.0f, 0.5f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//数の初期化処理
	InitNumber();

	//コロンの初期化処理
	InitColon();

	//位置を初期化する
	s_pos = D3DXVECTOR3(800.0f, 100.0f, 0.0f);

	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		int nTime = s_aRanking[i];
		int aNumber[MAX_TIME];
		float fHeightInterval = (NUMBER_HEIGHT * i) + (HEIGHT_INTERVAL * i);

		for (int j = 0; j < MAX_TIME; j++)
		{//１桁ずつに分ける
			aNumber[j] = nTime % 10;
			nTime /= 10;

			float fWidthInterval = (NUMBER_WIDTH * j) + (WIDTH_INTERVAL * (j / 2));

			//数の設定処理
			SetNumber(s_pos + D3DXVECTOR3(-fWidthInterval, fHeightInterval, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[j], j);
			
			if (j == 2 || j == 4)
			{
				//コロンの設定処理
				SetColon(s_pos + D3DXVECTOR3(-fWidthInterval + WIDTH_INTERVAL, fHeightInterval, 0.0f), WIDTH_INTERVAL, NUMBER_HEIGHT * 0.5f);
			}
		}
	}
}

//--------------------------------------------------
//ランキングの終了処理
//--------------------------------------------------
void UninitRanking(void)
{
	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	//数の終了処理
	UninitNumber();

	//コロンの終了処理
	UninitColon();
}

//--------------------------------------------------
//ランキングの更新処理
//--------------------------------------------------
void UpdateRanking(void)
{	
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
	{//決定キー(ENTERキー)が押されたかどうか
		//フェード設定
		SetFade(MODE_TITLE);
	}

	//if (s_nRankingUpdate != -1)
	//{//新しいスコアがある時
	//	if (s_state == RANKINGSTATE_IN)
	//	{//フェードイン状態
	//		s_aRanking[s_nRankingUpdate].col.a -= 0.025f;		//ポリゴンを透明にしていく

	//		if (s_aRanking[s_nRankingUpdate].col.a <= 0.0f)
	//		{
	//			s_aRanking[s_nRankingUpdate].col.a = 0.0f;
	//			s_state = RANKINGSTATE_OUT;		//フェードアウト状態に
	//		}
	//	}
	//	else if (s_state == RANKINGSTATE_OUT)
	//	{//フェードアウト状態
	//		s_aRanking[s_nRankingUpdate].col.a += 0.025f;		//ポリゴンを不透明にしていく

	//		if (s_aRanking[s_nRankingUpdate].col.a >= 1.0f)
	//		{
	//			s_aRanking[s_nRankingUpdate].col.a = 1.0f;
	//			s_state = RANKINGSTATE_IN;		//フェードイン状態に
	//		}
	//	}

	//	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//	//頂点情報をロックし、頂点情報へのポインタを取得
	//	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	//該当の位置まで進める
	//	pVtx += (s_nRankingUpdate *  MAX_TIME * 4);

	//	for (int i = 0; i < MAX_RANKING; i++)
	//	{//８桁まで
	//		//頂点カラーの設定
	//		pVtx[0].col = s_aRanking[s_nRankingUpdate].col;
	//		pVtx[1].col = s_aRanking[s_nRankingUpdate].col;
	//		pVtx[2].col = s_aRanking[s_nRankingUpdate].col;
	//		pVtx[3].col = s_aRanking[s_nRankingUpdate].col;

	//		pVtx += 4;		//頂点データのポインタを４つ分進める
	//	}
	//	//頂点バッファをアンロックする
	//	s_pVtxBuffRankScore->Unlock();
	//}

	//数の更新処理
	UpdateNumber();

	//コロンの更新処理
	UpdateColon();
}

//--------------------------------------------------
//ランキングの描画処理
//--------------------------------------------------
void DrawRanking(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//数の描画処理
	DrawNumber();

	//コロンの描画処理
	DrawColon();
}

//--------------------------------------------------
//ランキングのリセット
//--------------------------------------------------
void ResetRanking(void)
{
	FILE *pRankingFile;			//ファイルポインタを宣言

	s_nRankingUpdate = -1;		//更新ランクNo.の初期化

	//ファイルを開く
	pRankingFile = fopen(FILE_NAME, "r");

	if (pRankingFile != NULL)
	{//ファイルが開いた場合
		for (int i = 0; i < MAX_RANKING; i++)
		{//ランキングの読み込み
			fscanf(pRankingFile, "%d", &s_aRanking[i]);
		}

		//ファイルを閉じる
		fclose(pRankingFile);
	}
	else
	{//ファイルが開かない場合
		//フェードの設定
		SetFade(MODE_TITLE);

		assert(false);
	}
}

//--------------------------------------------------
//ランキングのセーブ
//--------------------------------------------------
void SaveRanking(void)
{
	FILE *pRankingFile;			//ファイルポインタを宣言

	//ファイルを開く
	pRankingFile = fopen(FILE_NAME, "w");

	if (pRankingFile != NULL)
	{//ファイルが開いた場合
		for (int i = 0; i < MAX_RANKING; i++)
		{//ランキングの書き込み
			fprintf(pRankingFile, "%d \n\n", s_aRanking[i]);
		}

		//ファイルを閉じる
		fclose(pRankingFile);
	}
	else
	{//ファイルが開かない場合
		//フェードの設定
		SetFade(MODE_TITLE);

		assert(false);
	}
}

//--------------------------------------------------
//ランキングの設定処理
//--------------------------------------------------
void SetRanking(int nRankTime)
{
	int nSwap;					//入れ替え用 
	int aData[MAX_DATA];		//データ(箱)

	for (int i = 0; i < MAX_RANKING; i++)
	{//代入
		aData[i] = s_aRanking[i];
	}

	//今回のタイム
	aData[MAX_RANKING] = nRankTime;

	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		//僕が最小値です！！！
		nSwap = aData[i];

		for (int j = i + 1; j < MAX_DATA; j++)
		{
			if (nSwap > aData[j])
			{//相手が小さかったら交代
				nSwap = aData[j];
			}
		}

		for (int j = i + 1; j < MAX_DATA; j++)
		{
			if (nSwap == aData[j])
			{//最小値を探して交代
				aData[j] = aData[i];
				break;
			}
		}

		//私が最小値だ！！！
		aData[i] = nSwap;
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//代入
		s_aRanking[i] = aData[i];
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		s_nRankingUpdate = -1;

		if (nRankTime == s_aRanking[i])
		{//指定のスコアを探す
			s_nRankingUpdate = i;
			break;
		}
	}
}