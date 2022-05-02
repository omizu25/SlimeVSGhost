//--------------------------------------------------
//
// ACG制作 ( bg.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "bg.h"
#include "input.h"
#include "player.h"

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//頂点バッファのポインタ
static D3DXVECTOR3					s_pos;					//位置

//--------------------------------------------------
//背景の初期化処理
//--------------------------------------------------
void InitBG(void)
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

	s_pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, s_pos, fWidth, fHeight);

	//rhwの設定処理
	Setrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 1.0f, 1.0f, 0.5f, 1.0f);

	//テクスチャ座標の設定処理
	Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

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

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数
}