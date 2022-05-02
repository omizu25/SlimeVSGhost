//--------------------------------------------------
//
// ACG制作 ( time.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "game.h"
#include "result.h"
#include "setup.h"
#include "time.h"

//-------------------------
//マクロ定義
//-------------------------
#define MAX_TIME			(6)			//タイムの最大桁数
#define MAX_TEX				(10)		//texの最大数
#define TIME_WIDTH			(30.0f)		//タイムの幅
#define TIME_HEIGHT			(25.0f)		//タイムの高さ
#define WIDTH_INTERVAL		(35.0f)		//幅の間隔

//-------------------------
//スタティック変数
//-------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//頂点バッファのポインタ
static D3DXVECTOR3					s_pos;					//タイムの位置
static int							s_nTime;				//タイムの値

//-------------------------
//タイムの初期化処理
//-------------------------
void InitTime(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number000.png",
		&s_pTexture);

	float fWidth = SCREEN_WIDTH / MAX_X_BLOCK;
	float fHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK) * 0.5f;

	//位置を初期化する
	s_pos = D3DXVECTOR3(fWidth * 30.0f, fHeight * 2.0f, 0.0f);

	s_nTime = 0;		//値を初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//もろもろの設定
	for (int i = 0; i < MAX_TIME; i++)
	{
		float fInterval = (WIDTH_INTERVAL * 0.3f) * (i / 2);

		//頂点座標の設定処理処理
		SetRightpos(pVtx, D3DXVECTOR3(s_pos.x - (WIDTH_INTERVAL * i) - fInterval, s_pos.y, 0.0f), TIME_WIDTH, TIME_HEIGHT);

		//rhwの初期化処理
		Initrhw(pVtx);

		//頂点カラーの設定処理
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		float fTex = 1.0f / MAX_TEX;

		//テクスチャ座標の設定
		Settex(pVtx, 0.0f, fTex, 0.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//-------------------------
//タイムの終了処理
//-------------------------
void UninitTime(void)
{
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
}

//-------------------------
//タイムの更新処理
//-------------------------
void UpdateTime(void)
{
	s_nTime++;

	//繰り上げ処理
	if (s_nTime % 100 >= 60)
	{//１秒ごと
		s_nTime += 40;
	}

	if (s_nTime % 10000 >= 6000)
	{//１分ごと
		s_nTime += 4000;
	}

	if (s_nTime % 1000000 >= 300000)
	{//３０分ごと
		//リザルトの設定処理
		SetResult(RESULT_LOSE);

		//ゲームの設定処理
		SetGameState(GAMESTATE_END);
	}

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nTime = s_nTime;
	int i = 0;
	float aTex[MAX_TIME];
	int aNumber[MAX_TIME];

	while (nTime != 0)
	{//１桁ずつに分ける
		float fTex = 1.0f / MAX_TEX;

		aNumber[i] = nTime % 10;
		aTex[i] = aNumber[i] * fTex;
		nTime /= 10;

		//テクスチャ座標の設定
		Settex(pVtx, 0.0f + aTex[i], fTex + aTex[i], 0.0f, 1.0f);

		i++;
		pVtx += 4;		//頂点データのポインタを４つ分進め
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//-------------------------
//タイムの描画処理
//-------------------------
void DrawTime(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_TIME; i++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			i * 4,						//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}

//-------------------------
//タイムの取得処理
//-------------------------
int GetTime(void)
{
	return s_nTime;		//今のタイムを授ける
}