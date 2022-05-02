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
#define MAX_MOVE					(10.0f)			//移動量の最大値
#define RANKING_WIDTH				(600.0f)		//ランクの幅
#define RANKING_HEIGHT				(80.0f)			//ランクの高さ
#define END_WIDTH					(800.0f)		//終わりの幅
#define END_HEIGHT					(60.0f)			//終わりの高さ
#define RANK_WIDTH					(80.0f)			//ランクの幅
#define RANK_HEIGHT					(80.0f)			//ランクの高さ
#define NUMBER_WIDTH				(60.0f)			//数の幅
#define NUMBER_HEIGHT				(80.0f)			//数の高さ
#define NEW_WIDTH					(160.0f)		//新しい奴の幅
#define NEW_HEIGHT					(80.0f)			//新しい奴の高さ
#define WIDTH_INTERVAL				(40.0f)			//幅の間隔
#define HEIGHT_INTERVAL				(20.0f)			//高さの間隔

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void UpdateMove(void);
static bool UpdateWherePos(void);
static void UpdatePos(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;		//背景の頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureEnd = NULL;		//終わりのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffEnd = NULL;		//終わりの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureRank = NULL;		//位のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffRank = NULL;		//位の頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureNew = NULL;		//新しい奴のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffNew = NULL;		//新しい奴の頂点バッファのポインタ
static int							s_aScore[MAX_RANKING];		//ランキングのスコア
static D3DXVECTOR3					s_pos;						//位置
static float						s_fWidth[MAX_RANKING];		//新しい奴の位置
static D3DXVECTOR3					s_move[MAX_RANKING];		//移動量
static int							s_nRankUpdate;				//更新ランクNo.

//--------------------------------------------------
//ランキングの初期化処理
//--------------------------------------------------
void InitRanking(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\ranking001.png",
		&s_pTexture);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title009.png",
		&s_pTextureEnd);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\ranking000.png",
		&s_pTextureRank);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\ranking002.png",
		&s_pTextureNew);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffEnd,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRank,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffNew,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	/* ↓背景↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, pos, fWidth, fHeight);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 0.85f, 0.85f, 0.85f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuffBG->Unlock();

	/* ↓ranking↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, D3DXVECTOR3(fWidth, RANKING_HEIGHT * 0.75f, 0.0f), RANKING_WIDTH * 0.5f, RANKING_HEIGHT * 0.5f);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの初期化処理
	Initcol(pVtx);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/* ↓終わり↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthEnd = END_WIDTH * 0.5f;
	float fHeightEnd = END_HEIGHT * 0.5f;
	D3DXVECTOR3 posEnd = D3DXVECTOR3(pos.x, SCREEN_HEIGHT - END_HEIGHT * 0.75f, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, posEnd, fWidthEnd, fHeightEnd);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの初期化処理
	Initcol(pVtx);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuffEnd->Unlock();

	/* ↓数関連↓ */

	//数の初期化処理
	InitNumber();

	//コロンの初期化処理
	InitColon();

	//s_nRankUpdate = 1;

	//位置を初期化する
	s_pos = D3DXVECTOR3(2000.0f, 175.0f, 0.0f);

	for (int i = 0; i < MAX_RANKING; i++)
	{
		s_move[i] = D3DXVECTOR3(-MAX_MOVE, 0.0f, 0.0f);
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		int nTime = s_aScore[i];
		int aNumber[MAX_TIME];
		float fHeightInterval = (NUMBER_HEIGHT * i) + (HEIGHT_INTERVAL * i);
		float fWidthInterval;
		s_fWidth[i] = s_pos.x - (WIDTH_INTERVAL * i);

		for (int j = 0; j < MAX_TIME; j++)
		{//１桁ずつに分ける
			aNumber[j] = nTime % 10;
			nTime /= 10;

			fWidthInterval = (NUMBER_WIDTH * j) + (WIDTH_INTERVAL * (j / 2)) + (WIDTH_INTERVAL * i);

			//数の設定処理
			SetNumber(s_pos + D3DXVECTOR3(-fWidthInterval, fHeightInterval, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[j], j, i);

			if ((j % 2 == 0) && (j != 0))
			{//２の倍数
				//コロンの設定処理
				SetColon(s_pos + D3DXVECTOR3(-fWidthInterval + WIDTH_INTERVAL, fHeightInterval, 0.0f), WIDTH_INTERVAL, NUMBER_HEIGHT * 0.5f, i);
			}
		}

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		D3DXVECTOR3 posRank = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, s_pos.y, 0.0f);

		//頂点座標の設定処理
		SetMiddlepos(pVtx, posRank + D3DXVECTOR3(0.0f, fHeightInterval, 0.0f), RANK_WIDTH * 0.5f, RANK_HEIGHT * 0.5f);

		//rhwの初期化処理
		Initrhw(pVtx);

		//頂点カラーの設定処理
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ座標の初期化処理
		Inittex(pVtx);

		//頂点バッファをアンロックする
		s_pVtxBuffRank->Unlock();

		//数の設定処理
		SetNumber(posRank + D3DXVECTOR3(-RANK_WIDTH, fHeightInterval, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, i + 1, 0, -2);
	}

	/* ↓New↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffNew->Lock(0, 0, (void**)&pVtx, 0);

	float fHeightNew;

	if (s_nRankUpdate == -1)
	{//Newがない
		//位置を初期化する
		fHeightNew = 0.0f;
	}
	else
	{//Newがある
		//位置を初期化する
		fHeightNew = (NUMBER_HEIGHT * s_nRankUpdate) + (HEIGHT_INTERVAL * s_nRankUpdate);
	}

	//頂点座標の設定処理
	SetMiddlepos(pVtx, s_pos + D3DXVECTOR3(NEW_WIDTH, fHeightNew, 0.0f), NEW_WIDTH * 0.5f, NEW_HEIGHT * 0.5f);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuffNew->Unlock();
}

//--------------------------------------------------
//ランキングの終了処理
//--------------------------------------------------
void UninitRanking(void)
{
	if (s_pVtxBuffBG != NULL)
	{//背景の頂点バッファの破棄
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	if (s_pTexture != NULL)
	{//テクスチャの破棄
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pTextureEnd != NULL)
	{//終わりのテクスチャの破棄
		s_pTextureEnd->Release();
		s_pTextureEnd = NULL;
	}

	if (s_pVtxBuffEnd != NULL)
	{//終わりの頂点バッファの破棄
		s_pVtxBuffEnd->Release();
		s_pVtxBuffEnd = NULL;
	}

	if (s_pTextureRank != NULL)
	{//位のテクスチャの破棄
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	if (s_pVtxBuffRank != NULL)
	{//位の頂点バッファの破棄
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	if (s_pTextureNew != NULL)
	{//新しい奴のテクスチャの破棄
		s_pTextureNew->Release();
		s_pTextureNew = NULL;
	}

	if (s_pVtxBuffNew != NULL)
	{//新しい奴の頂点バッファの破棄
		s_pVtxBuffNew->Release();
		s_pVtxBuffNew = NULL;
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
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_B) ||
		GetJoypadTrigger(JOYKEY_B))
	{//決定キー(ENTERキー)が押されたかどうか

		//posがどこにいるのか処理
		if (UpdateWherePos())
		{//posが既定の位置にいる
			//フェードの設定
			SetFade(MODE_TITLE);
		}
		else
		{//posが既定の位置にいない
			//頂点処理
			UpdatePos();
		}
	}

	//移動処理
	UpdateMove();

	//頂点処理
	UpdatePos();

	//数のランク処理
	RankNumber(s_nRankUpdate);

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
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffEnd, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureEnd);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureRank);

	for (int i = 0; i < MAX_RANKING; i++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			i * 4,						//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffNew, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureNew);

	if (s_nRankUpdate != -1)
	{//Newがある
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,							//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}

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

	s_nRankUpdate = -1;		//更新ランクNo.の初期化

	//ファイルを開く
	pRankingFile = fopen(FILE_NAME, "r");

	if (pRankingFile != NULL)
	{//ファイルが開いた場合
		for (int i = 0; i < MAX_RANKING; i++)
		{//ランキングの読み込み
			fscanf(pRankingFile, "%d", &s_aScore[i]);
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
			fprintf(pRankingFile, "%d \n\n", s_aScore[i]);
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
		aData[i] = s_aScore[i];
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
		s_aScore[i] = aData[i];
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		s_nRankUpdate = -1;

		if (nRankTime == s_aScore[i])
		{//指定のスコアを探す
			s_nRankUpdate = i;
			break;
		}
	}
}

//--------------------------------------------------
//ランキングの更新の初期化処理
//--------------------------------------------------
void InitRankUpdate(void)
{
	s_nRankUpdate = -1;
}

//--------------------------------------------------
//移動処理
//--------------------------------------------------
static void UpdateMove(void)
{
	//位置を更新
	s_pos.x += s_move[s_nRankUpdate].x;

	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		//位置の更新
		s_fWidth[i] += s_move[i].x;

		if (s_fWidth[i] <= 900.0f)
		{//既定の値を越した
			s_fWidth[i] = 900.0f;
			s_move[i].x = 0.0f;
		}
	}
}

//--------------------------------------------------
//posがどこにいるのか処理
//--------------------------------------------------
static bool UpdateWherePos(void)
{
	bool bWherePos = true;

	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		if (s_fWidth[i] > 900.0f)
		{//既定の値を越してない
			bWherePos = false;
		}

		s_move[i].x = -(s_fWidth[i] - 900.0f);
	}

	if (!bWherePos)
	{
		//位置を更新
		s_pos.x = 900.0f;
	}

	return bWherePos;
}

//--------------------------------------------------
//頂点処理
//--------------------------------------------------
static void UpdatePos(void)
{
	for (int i = 0; i < MAX_RANKING; i++)
	{//５位まで
		int nTime = s_aScore[i];
		int aNumber[MAX_TIME];
		float fHeightInterval = (NUMBER_HEIGHT * i) + (HEIGHT_INTERVAL * i);
		float fWidthInterval;

		for (int j = 0; j < MAX_TIME; j++)
		{//１桁ずつに分ける
			aNumber[j] = nTime % 10;
			nTime /= 10;

			fWidthInterval = (NUMBER_WIDTH * j) + (WIDTH_INTERVAL * (j / 2));

			//数の頂点処理
			PosNumber(D3DXVECTOR3(s_fWidth[i] - fWidthInterval, s_pos.y + fHeightInterval, 0.0f), aNumber[j], j, i);

			if ((j % 2 == 0) && (j != 0))
			{//２の倍数
				//コロンの移動処理
				MoveColon(D3DXVECTOR3(s_move[i].x * 0.5f, s_move[i].y, 0.0f), i);
			}
		}
	}

	/* ↓New↓ */

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffNew->Lock(0, 0, (void**)&pVtx, 0);

	float fHeightNew = (NUMBER_HEIGHT * s_nRankUpdate) + (HEIGHT_INTERVAL * s_nRankUpdate);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, s_pos + D3DXVECTOR3(NEW_WIDTH - (WIDTH_INTERVAL * s_nRankUpdate), fHeightNew, 0.0f), NEW_WIDTH * 0.5f, NEW_HEIGHT * 0.5f);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuffNew->Unlock();
}