//--------------------------------------------------
//
// ACG制作 ( gauge.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "setup.h"
#include "gauge.h"

#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_GAUGE			(32)			//ゲージの最大数
#define GAUGE_WIDTH			(2.0f)			//ゲージの幅

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffFrame = NULL;		//フレームの頂点バッファのポインタ
static Gauge						s_Gauge[MAX_GAUGE];			//ゲージの情報
static GaugeFrame					s_Frame[MAX_GAUGE];			//フレームの情報

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitGaugeStruct(Gauge *pGauge);
static void InitFrameStruct(GaugeFrame *pFrame);
static void SetPos(VERTEX_2D *pVtx, Gauge *pGauge);
static void SetCol(VERTEX_2D *pVtx, Gauge *pGauge);
static void SetFrame(Gauge *pGauge);
static void UpdateDecrease(void);

//--------------------------------------------------
//ゲージの初期化処理
//--------------------------------------------------
void InitGauge(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_GAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_GAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffFrame,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_Gauge[i];

		//ゲージの構造体の初期化処理
		InitGaugeStruct(pGauge);

		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進め
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffFrame->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_GAUGE; i++)
	{
		GaugeFrame *pFrame = &s_Frame[i];

		//構造体の初期化処理
		InitFrameStruct(pFrame);

		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進め
	}

	//頂点バッファをアンロックする
	s_pVtxBuffFrame->Unlock();
}

//--------------------------------------------------
//ゲージの終了処理
//--------------------------------------------------
void UninitGauge(void)
{
	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//ゲージの更新処理
//--------------------------------------------------
void UpdateGauge(void)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_Gauge[i];

		if (!pGauge->bUse)
		{//ゲージが使用されていない
			continue;
		}

		//ゲージが使用されている

		//減少量の処理
		UpdateDecrease();

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetPos(pVtx, pGauge);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//ゲージの描画処理
//--------------------------------------------------
void DrawGauge(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffFrame, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < MAX_GAUGE; i++)
	{
		if (s_Frame[i].bUse)
		{//フレームが使用されている
		 //ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				i * 4,						//描画する最初の頂点インデックス
				2);							//プリミティブ(ポリゴン)数
		}
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < MAX_GAUGE; i++)
	{
		if (s_Gauge[i].bUse)
		{//ゲージが使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				i * 4,						//描画する最初の頂点インデックス
				2);							//プリミティブ(ポリゴン)数
		}
	}
}

//--------------------------------------------------
//ゲージの設定処理
//--------------------------------------------------
void SetGauge(GAUGEUSE use, int nLife)
{
	for (int i = 0; i < GAUGETYPE_MAX; i++)
	{
		for (int j = 0; j < MAX_GAUGE; j++)
		{
			Gauge *pGauge = &s_Gauge[j];

			if (pGauge->bUse)
			{//ゲージが使用されている
				continue;
			}

			//ゲージが使用されていない

			pGauge->use = use;
			pGauge->type = (GAUGETYPE)i;
			pGauge->bUse = true;
			pGauge->nCounter = 0;
			pGauge->fWidth = GAUGE_WIDTH * nLife;

			float fWidth = SCREEN_WIDTH / MAX_X_BLOCK;
			float fHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK) * 0.5f;

			pGauge->fHeight = fHeight;

			switch (use)
			{
			case GAUGEUSE_PLAYER:		//プレイヤー

				pGauge->pos = D3DXVECTOR3(fWidth * (MAX_X_BLOCK * 0.093f), fHeight * 2.0f, 0.0f);

				break;

			case GAUGEUSE_BOY:			//男の子 (リリ)
			case GAUGEUSE_GIRL:			//女の子 (ルル)

				pGauge->pos = D3DXVECTOR3(fWidth * (MAX_X_BLOCK * 0.5f), fHeight * 2.0f, 0.0f);

				break;

			default:
				assert(false);
				break;
			}

			VERTEX_2D *pVtx;		//頂点情報へのポインタ

			//頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (j * 4);		//該当の位置まで進める

			//頂点座標の設定処理
			SetPos(pVtx, pGauge);
			
			//色の設定処理
			SetCol(pVtx, pGauge);

			//頂点バッファをアンロックする
			s_pVtxBuff->Unlock();

			if ((GAUGETYPE)i == GAUGETYPE_MAXIMUM)
			{//指定の種類の時
				//フレームの設定処理
				SetFrame(pGauge);
			}

			break;		//ここでfor文を抜ける
		}
	}
}

//--------------------------------------------------
//ゲージの減算処理
//--------------------------------------------------
void SubGauge(GAUGEUSE use, int nLife)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_Gauge[i];

		if (!pGauge->bUse || pGauge->type != GAUGETYPE_REMAINING || pGauge->use != use)
		{//ゲージが使用されていない、指定の種類じゃない、使用者じゃない
			continue;
		}

		//ゲージが使用されている

		pGauge->fWidth = GAUGE_WIDTH * nLife;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetPos(pVtx, pGauge);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;		//ここでfor文を抜ける
	}
}

//--------------------------------------------------
//ゲージの構造体の初期化処理
//--------------------------------------------------
static void InitGaugeStruct(Gauge *pGauge)
{
	pGauge->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pGauge->use = GAUGEUSE_PLAYER;
	pGauge->type = GAUGETYPE_REMAINING;
	pGauge->fWidth = 0.0f;
	pGauge->fHeight = 0.0f;
	pGauge->nCounter = 0;
	pGauge->bUse = false;
}

//--------------------------------------------------
//フレームの構造体の初期化処理
//--------------------------------------------------
static void InitFrameStruct(GaugeFrame *pFrame)
{
	pFrame->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pFrame->use = GAUGEUSE_PLAYER;
	pFrame->fWidth = 0.0f;
	pFrame->fHeight = 0.0f;
	pFrame->bUse = false;
}

//--------------------------------------------------
//頂点座標の設定処理
//--------------------------------------------------
static void SetPos(VERTEX_2D *pVtx, Gauge *pGauge)
{
	switch (pGauge->use)
	{
	case GAUGEUSE_PLAYER:		//プレイヤー
	case GAUGEUSE_GIRL:			//女の子 (ルル)

		//頂点座標の設定処理
		SetLeftpos(pVtx, pGauge->pos, pGauge->fWidth, pGauge->fHeight);

		break;

	case GAUGEUSE_BOY:			//男の子 (リリ)

		//頂点座標の設定処理
		SetRightpos(pVtx, pGauge->pos, pGauge->fWidth, pGauge->fHeight);

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//色の設定処理
//--------------------------------------------------
static void SetCol(VERTEX_2D *pVtx, Gauge *pGauge)
{
	switch (pGauge->type)
	{
	case GAUGETYPE_MAXIMUM:			//最大値

		//頂点カラーの設定処理
		Setcol(pVtx, 0.5f, 0.5f, 0.5f, 1.0f);

		break;

	case GAUGETYPE_DECREASE:		//減少量

		//頂点カラーの設定処理
		Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

		break;

	case GAUGETYPE_REMAINING:		//残り

		switch (pGauge->use)
		{
		case GAUGEUSE_PLAYER:		//プレイヤー

			//頂点カラーの設定処理
			Setcol(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

			break;

		case GAUGEUSE_BOY:			//男の子 (リリ)

			//頂点カラーの設定処理
			Setcol(pVtx, 0.0f, 0.0f, 1.0f, 1.0f);

			break;

		case GAUGEUSE_GIRL:			//女の子 (ルル)

			//頂点カラーの設定処理
			Setcol(pVtx, 1.0f, 0.3f, 0.75f, 1.0f);

			break;

		default:
			assert(false);
			break;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//フレームの設定処理
//--------------------------------------------------
static void SetFrame(Gauge *pGauge)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		GaugeFrame *pFrame = &s_Frame[i];

		if (pFrame->bUse)
		{//フレームが使用されている
			continue;
		}

		assert(i <= 3);

		//フレームが使用されていない

		switch (pGauge->use)
		{
		case GAUGEUSE_PLAYER:		//プレイヤー
		case GAUGEUSE_GIRL:			//女の子 (ルル)

			pFrame->pos = D3DXVECTOR3(pGauge->pos.x + (pGauge->fWidth * 0.5f), pGauge->pos.y, 0.0f);

			break;

		case GAUGEUSE_BOY:			//男の子 (リリ)

			pFrame->pos = D3DXVECTOR3(pGauge->pos.x - (pGauge->fWidth * 0.5f), pGauge->pos.y, 0.0f);

			break;

		default:
			assert(false);
			break;
		}

		pFrame->use = pGauge->use;
		pFrame->fWidth = pGauge->fWidth * 0.51f;
		pFrame->fHeight = pGauge->fHeight * 1.2f;
		pFrame->bUse = true;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffFrame->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetMiddlepos(pVtx, pFrame->pos, pFrame->fWidth, pFrame->fHeight);

		//頂点カラーの設定処理
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		//頂点バッファをアンロックする
		s_pVtxBuffFrame->Unlock();

		break;
	}
}

//--------------------------------------------------
//減少量の処理
//--------------------------------------------------
static void UpdateDecrease(void)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pRemaining = &s_Gauge[i];

		if (!pRemaining->bUse || pRemaining->type != GAUGETYPE_REMAINING)
		{//ゲージが使用されていない、指定の種類じゃない
			continue;
		}

		//ゲージが使用されている

		for (int j = 0; j < MAX_GAUGE; j++)
		{
			Gauge *pDecrease = &s_Gauge[j];

			if (!pDecrease->bUse || pDecrease->type != GAUGETYPE_DECREASE || pDecrease->use != pRemaining->use)
			{//ゲージが使用されていない、指定の種類じゃない、使用者が違う
				continue;
			}

			//ゲージが使用されている

			if (pDecrease->fWidth == pRemaining->fWidth)
			{//横幅同じ
  				pDecrease->nCounter = 0;
				break;
			}
			
			//横幅違う

			pDecrease->nCounter++;

			if (pDecrease->nCounter >= 180)
			{//カウンターが超したら
				float Difference = pDecrease->fWidth - pRemaining->fWidth;
				
				pDecrease->fWidth -= Difference * 0.0075f;

				if (Difference <= 0.5f)
				{//差が小さくなったら
					pDecrease->nCounter = 0;
					pDecrease->fWidth = pRemaining->fWidth;
				}
			}
		}
	}
}