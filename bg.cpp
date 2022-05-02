//--------------------------------------------------
//
// ACG制作 ( bg.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "bg.h"
#include "block.h"
#include "input.h"
#include "player.h"
#include "setup.h"

//--------------------------------------------------
//背景の構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	float			fWidth;			//幅
	float			fHeight;		//高さ
}BG;

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_BG		(2)		//背景の最大数

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(int i);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//頂点バッファのポインタ
static BG							s_aBG[MAX_BG];			//背景の情報

//--------------------------------------------------
//背景の初期化処理
//--------------------------------------------------
void InitBG(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BG; i++)
	{
		//構造体の初期化処理
		InitStruct(i);

		//頂点座標の設定処理
		SetMiddlepos(pVtx, s_aBG[i].pos, s_aBG[i].fWidth, s_aBG[i].fHeight);

		//rhwの初期化処理
		Initrhw(pVtx);

		if (i == 0)
		{
			//頂点カラーの設定処理
			Setcol(pVtx, 0.615f, 0.215f, 0.341f, 1.0f);
		}
		else
		{
			//頂点カラーの設定処理
			Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャの初期化処理
		Inittex(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進め
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//背景の終了処理
//--------------------------------------------------
void UninitBG(void)
{
	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//背景の更新処理
//--------------------------------------------------
void UpdateBG(void)
{
	
}

//--------------------------------------------------
//背景の描画処理
//--------------------------------------------------
void DrawBG(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < MAX_BG; i++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			i * 4,						//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}

//--------------------------------------------------
//構造体の初期化処理
//--------------------------------------------------
static void InitStruct(int i)
{
	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight;

	if (i == 0)
	{
		fHeight = SCREEN_HEIGHT * 0.5f;
	}
	else
	{
		fHeight = SCREEN_HEIGHT / MAX_Y_BLOCK;
	}
	
	s_aBG[i].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);
	s_aBG[i].fWidth = fWidth;
	s_aBG[i].fHeight = fHeight;
}