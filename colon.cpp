//--------------------------------------------------
//
// ACG制作 ( colon.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "game.h"
#include "number.h"
#include "result.h"
#include "setup.h"
#include "colon.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_COLON		(64)		//コロンの最大数

//--------------------------------------------------
//コロンの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	float			fWidth;			//幅
	float			fHeight;		//高さ
	int				nRank;			//順位
	bool			bUse;			//使用しているかどうか
}Colon;

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static Colon						s_aColon[MAX_COLON];		//コロンの情報

//--------------------------------------------------
//コロンの初期化処理
//--------------------------------------------------
void InitColon(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\time003.png",
		&s_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_COLON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//メモリのクリア
	memset(&s_aColon[0], NULL, sizeof(s_aColon));

	for (int i = 0; i < MAX_COLON; i++)
	{
		//頂点情報の初期化処理
		Initpos(pVtx);

		//rhwの初期化処理
		Initrhw(pVtx);

		//頂点カラーの設定
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ座標の初期化処理
		Inittex(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//コロンの終了処理
//--------------------------------------------------
void UninitColon(void)
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

//--------------------------------------------------
//コロンの更新処理
//--------------------------------------------------
void UpdateColon(void)
{
	
}

//--------------------------------------------------
//コロンの描画処理
//--------------------------------------------------
void DrawColon(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_COLON; i++)
	{
		if (s_aColon[i].bUse)
		{//数が使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				i * 4,						//描画する最初の頂点インデックス
				2);							//プリミティブ(ポリゴン)数
		}
	}
}

//--------------------------------------------------
//コロンの設定処理
//--------------------------------------------------
void SetColon(D3DXVECTOR3 pos, float fWidth, float fHeight, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (pColon->bUse)
		{//コロンが使用されている
			continue;
		}

		//コロンが使用されていない

		pColon->pos = pos;
		pColon->fWidth = fWidth;
		pColon->fHeight = fHeight;
		pColon->nRank = nRank;
		pColon->bUse = true;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetRightpos(pVtx, pos, fWidth, fHeight);

		if (nRank == -1)
		{//指定のランク
			//頂点カラーの設定処理
			Setcol(pVtx, 0.0f, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			//頂点カラーの設定処理
			Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//コロンのランク処理
//--------------------------------------------------
void RankColon(D3DXCOLOR col, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (!pColon->bUse || pColon->nRank != nRank)
		{//コロンが使用されていない、順位が違う
			continue;
		}

		//コロンが使用されている、順位が同じ

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点カラーの設定処理
		Setcol(pVtx, col.r, col.g, col.b, col.a);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//コロンの移動処理
//--------------------------------------------------
void MoveColon(D3DXVECTOR3 move, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (!pColon->bUse || pColon->nRank != nRank)
		{//コロンが使用されていない、順位が違う
			continue;
		}

		//コロンが使用されている、順位が同じ

		//位置を更新
		pColon->pos.x += move.x;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetRightpos(pVtx, pColon->pos, pColon->fWidth, pColon->fHeight);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//コロンの頂点処理
//--------------------------------------------------
void PosColon(D3DXVECTOR3 pos, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (!pColon->bUse || pColon->nRank != nRank)
		{//コロンが使用されていない、順位が違う
			continue;
		}

		//コロンが使用されている、順位が同じ

		//位置を更新
		pColon->pos.x += pos.x;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetRightpos(pVtx, pos, pColon->fWidth, pColon->fHeight);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}