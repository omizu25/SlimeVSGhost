//--------------------------------------------------
//
// ACG制作 ( item.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "enemy.h"
#include "item.h"
#include "player.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_ITEM			(256)			//アイテムの最大数
#define INHALE_DIV			(0.05f)			//吸い込みの分割数
#define INHALE_DISTANCE		(350.0f)		//吸い込む距離
#define MAX_INERTIA			(0.75f)			//慣性の最大値
#define STER_MOVE			(10.0f)			//スターの移動量

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(Item *pItem);
static void UpdateOffScreen(Item *pItem);
static void UpdateCollision(Item *pItem);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[ITEMTYPE_MAX];		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//頂点バッファのポインタ
static Item							s_aItem[MAX_ITEM];				//アイテムの情報

//--------------------------------------------------
//アイテムの初期化処理
//--------------------------------------------------
void InitItem(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ITEMTYPE_MAX; i++)
	{//メモリのクリア
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block005.png",
		&s_pTexture[ITEMTYPE_BLOCK]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Star002.png",
		&s_pTexture[ITEMTYPE_STAR]);

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
		Item *pItem = &s_aItem[i];		//アイテムの情報

		//構造体の初期化処理
		InitStruct(pItem);

		//全ての初期化処理
		InitAll(pVtx);

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
	for (int i = 0; i < ITEMTYPE_MAX; i++)
	{
		if (s_pTexture[i] != NULL)
		{//テクスチャの破棄
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
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

		//位置を更新
		pItem->pos += pItem->move;
		
		if (pItem->type == ITEMTYPE_BLOCK)
		{//ブロックの時
			//慣性・移動量を更新 (減衰させる)
			pItem->move.x += (0.0f - pItem->move.x) * MAX_INERTIA;
		}

		//画面外処理
		UpdateOffScreen(pItem);

		//当たり判定処理
		UpdateCollision(pItem);

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

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (s_aItem[i].bUse)
		{//アイテムが使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexture[s_aItem[i].type]);

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
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, bool bDirection)
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
		pItem->type = type;
		pItem->fWidth = ITEM_SIZE * 0.5f;
		pItem->fHeight = ITEM_SIZE * 0.5f;
		pItem->bDirection = bDirection;
		pItem->bUse = true;		//使用している状態にする

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetMiddlepos(pVtx, pos, pItem->fWidth, pItem->fHeight);

		switch (pItem->type)
		{
		case ITEMTYPE_BLOCK:		//ブロック
			pItem->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;

		case ITEMTYPE_STAR:			//スター

			if (bDirection)
			{//右向き
				pItem->move = D3DXVECTOR3(STER_MOVE, 0.0f, 0.0f);
			}
			else
			{//左向き
				pItem->move = D3DXVECTOR3(-STER_MOVE, 0.0f, 0.0f);
			}

		default:
			break;
		}

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

		if (!pItem->bUse || pItem->type == ITEMTYPE_STAR)
		{//アイテムが使用されていない、指定の種類じゃない
			continue;
		}

		//アイテムが使用されている、指定の種類

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
//アイテムの吸い込み処理
//--------------------------------------------------
void InhaleItem(D3DXVECTOR3 pos, ATTACKSTATE *pAttack, float fWidth, float fHeight, bool bDirection)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];		//アイテムの情報

		if (!pItem->bUse || pItem->type == ITEMTYPE_STAR)
		{//アイテムが使用されていない、指定の種類じゃない
			continue;
		}

		//アイテムが使用されている、指定の種類

		if (bDirection != pItem->bDirection)
		{//向きが逆な時
			if (pos.y >= pItem->pos.y && pos.y - fHeight <= pItem->pos.y)
			{//高さがプレイヤーの範囲内
				if ((!bDirection && pos.x >= pItem->pos.x) ||
					(bDirection && pos.x <= pItem->pos.x))
				{//左向きでプレイヤーより左にある、右向きでプレイヤーより右にある
					float Difference = pos.x - pItem->pos.x;

					if ((Difference <= 0.0f && Difference >= -INHALE_DISTANCE) ||
						(Difference >= 0.0f && Difference <= INHALE_DISTANCE))
					{//差が範囲内
						pItem->move.x += Difference * INHALE_DIV;

						float fLeft = pItem->pos.x - pItem->fWidth;
						float fRight = pItem->pos.x + pItem->fWidth;

						if ((pos.x - fWidth <= fRight) && (pos.x + fWidth >= fLeft))
						{//アイテムとプレイヤーが重なった
							pItem->bUse = false;		//使用していない状態にする
							pItem->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							*pAttack = ATTACKSTATE_STORE;
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------
//構造体の初期化処理
//--------------------------------------------------
static void InitStruct(Item *pItem)
{
	pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pItem->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pItem->type = ITEMTYPE_BLOCK;
	pItem->fWidth = 0.0f;
	pItem->fHeight = 0.0f;
	pItem->bDirection = false;		//左向き
	pItem->bUse = false;			//使用していない状態にする
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

//--------------------------------------------------
//当たり判定処理
//--------------------------------------------------
static void UpdateCollision(Item *pItem)
{
	Player *pPlayer = GetPlayer();		//プレイヤーの取得

	if (pItem->pos.y <= (pPlayer->pos.y + pItem->fHeight) &&
		pItem->pos.y >= (pPlayer->pos.y - pPlayer->fHeight - pItem->fHeight) &&
		pItem->pos.x <= (pPlayer->pos.x + pPlayer->fWidth + pItem->fWidth) &&
		pItem->pos.x >= (pPlayer->pos.x - pPlayer->fWidth - pItem->fWidth) &&
		pPlayer->state == PLAYERSTATE_NORMAL && pPlayer->attack != ATTACKSTATE_IN &&
		pItem->type == ITEMTYPE_BLOCK && pItem->move.x == 0.0f)
	{//プレイヤーにブロックが当たった時

		//プレイヤーのヒット処理
		HitPlayer(5);

		pItem->bUse = false;		//使用していない状態にする
	}

	Enemy *pEnemy = GetEnemy();		//敵の取得

	for (int i = 0; i < ENEMYTYPE_MAX; i++, pEnemy++)
	{
		if (!pEnemy->bUse)
		{//敵が使用されていない
			continue;
		}

		//敵が使用されている

		if (pItem->pos.y <= (pEnemy->pos.y + pItem->fHeight) &&
			pItem->pos.y >= (pEnemy->pos.y - pEnemy->fHeight - pItem->fHeight) &&
			pItem->pos.x <= (pEnemy->pos.x + pEnemy->fWidth + pItem->fWidth) &&
			pItem->pos.x >= (pEnemy->pos.x - pEnemy->fWidth - pItem->fWidth) &&
			pEnemy->state == ENEMYSTATE_NORMAL && pItem->type == ITEMTYPE_STAR)
		{//敵にスターが当たった時

			//敵のヒット処理
			HitEnemy(i, 10);

			pItem->bUse = false;		//使用していない状態にする
		}
	}
}