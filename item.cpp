//--------------------------------------------------
//
// ACG制作 ( item.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "item.h"
#include "player.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_ITEM		(256)		//アイテムの最大数

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(int i);
static void UpdateOffScreen(Item *pItem);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//頂点バッファのポインタ
static Item							s_aItem[MAX_ITEM];		//アイテムの情報

//--------------------------------------------------
//アイテムの初期化処理
//--------------------------------------------------
void InitItem(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block005.png",
		&s_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//アイテムの情報の初期化設定
	for (int i = 0; i < MAX_ITEM; i++)
	{
		//構造体の初期化処理
		InitStruct(i);

		//頂点座標の設定処理
		SetMiddlepos(pVtx, s_aItem[i].pos, s_aItem[i].fWidth, s_aItem[i].fHeight);

		//rhwの設定処理
		Setrhw(pVtx);

		//頂点カラーの設定処理
		Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//アイテムの終了処理
//--------------------------------------------------
void UninitItem(void)
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
//アイテムの更新処理
//--------------------------------------------------
void UpdateItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];		//アイテムの情報

		if (!pItem->bUse)
		{//アイテムが使用されていない
			continue;
		}

		//アイテムが使用されている

		//画面外処理
		UpdateOffScreen(pItem);

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetMiddlepos(pVtx, pItem->pos, pItem->fWidth, pItem->fHeight);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//アイテムの描画処理
//--------------------------------------------------
void DrawItem(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (s_aItem[i].bUse)
		{//アイテムが使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				i * 4,						//描画する最初の頂点インデックス
				2);							//プリミティブ(ポリゴン)数
		}
	}
}

//--------------------------------------------------
//アイテムの設定処理
//--------------------------------------------------
void SetItem(D3DXVECTOR3 pos)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];

		if (pItem->bUse)
		{//アイテムが使用されている
			continue;
		}

		//アイテムが使用されていない

		pItem->pos = pos;
		pItem->fWidth = ITEM_SIZE * 0.5f;
		pItem->fHeight = ITEM_SIZE * 0.5f;
		pItem->bUse = true;		//使用している状態にする

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetMiddlepos(pVtx, pos, pItem->fWidth, pItem->fHeight);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;		//ここでfor文を抜ける
	}
}

//--------------------------------------------------
//アイテムの取得処理
//--------------------------------------------------
Item *GetItem(void)
{
	return &s_aItem[0];		//アイテムの情報を授ける
}

//--------------------------------------------------
//アイテムの当たり判定処理
//--------------------------------------------------
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];		//アイテムの情報

		if (!pItem->bUse)
		{//アイテムが使用されていない
			continue;
		}

		//アイテムが使用されている

		float fLeft = pItem->pos.x - pItem->fWidth;
		float fRight = pItem->pos.x + pItem->fWidth;
		float fTop = pItem->pos.y - pItem->fHeight;
		float fBottom = pItem->pos.y + pItem->fHeight;

		if ((pPosOld->y > fTop) && (pPosOld->y - fHeight < fBottom))
		{//前回のYがアイテムの範囲内
			if ((pPosOld->x + fWidth <= fLeft) && (pPos->x + fWidth > fLeft))
			{//アイテムの左端
				pItem->pos.x = pPos->x + fWidth + pItem->fWidth;
			}
			else if ((pPosOld->x - fWidth >= fRight) && (pPos->x - fWidth < fRight))
			{//アイテムの右端
				pItem->pos.x = pPos->x - fWidth - pItem->fWidth;
			}
		}
	}
}

//--------------------------------------------------
//構造体の初期化処理
//--------------------------------------------------
static void InitStruct(int i)
{
	s_aItem[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aItem[i].fWidth = 0.0f;
	s_aItem[i].fHeight = 0.0f;
	s_aItem[i].bUse = false;		//使用していない状態にする
}

//--------------------------------------------------
//画面外処理
//--------------------------------------------------
static void UpdateOffScreen(Item *pItem)
{
	float ItemSize = ITEM_SIZE * 0.5f;

	//画面端処理
	if (pItem->pos.x >= SCREEN_WIDTH + ItemSize || pItem->pos.x <= -ItemSize)
	{//右端か左端
		pItem->bUse = false;		//使用していない状態にする
	}
}