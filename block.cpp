//--------------------------------------------------
//
// ACG制作 ( block.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "input.h"
#include "player.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define FILE_DATA				(1024)					//ファイルのデータ数
#define FILE_NAME		"data/test_map.txt"				//マップのパス
#define MAX_TEX			(BLOCKTYPE_MAX - 1)				//テクスチャの最大数

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(int i);
static void InitType(void);
static void DownCollision(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static Block						s_aBlock[MAX_BLOCK];		//ブロックの情報
static bool							s_bThrough;					//ブロックが通り抜けるか [false :通り抜けない true : 通り抜ける]

//--------------------------------------------------
//ブロックの初期化処理
//--------------------------------------------------
void InitBlock(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_bThrough = false;

	for (int i = 0; i < MAX_TEX; i++)
	{//メモリのクリア
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block009.png",
		&s_pTexture[BLOCKTYPE_BLUE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block011.png",
		&s_pTexture[BLOCKTYPE_LIGHT_BLUE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block010.png",
		&s_pTexture[BLOCKTYPE_B_THROUGH]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block012.png",
		&s_pTexture[BLOCKTYPE_L_THROUGH]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ブロックの情報の初期化設定
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		//構造体の初期化処理
		InitStruct(i);

		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//種類の初期化処理
	InitType();
}

//--------------------------------------------------
//ブロックの終了処理
//--------------------------------------------------
void UninitBlock(void)
{
	for (int i = 0; i < MAX_TEX; i++)
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
//ブロックの更新処理
//--------------------------------------------------
void UpdateBlock(void)
{
	if (s_bThrough)
	{//通り抜ける
		//下がるときの当たり判定処理
		DownCollision();
	}
}

//--------------------------------------------------
//ブロックの描画処理
//--------------------------------------------------
void DrawBlock(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TEX; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		//ブロックの描画
		for (int j = 0; j < MAX_BLOCK; j++)
		{
			if (s_aBlock[j].bUse && s_aBlock[j].type == i)
			{//ブロックが使用されている
				//ポリゴンの描画
				pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP,		//プリミティブの種類
					j * 4,						//描画する最初の頂点インデックス
					2);							//プリミティブ(ポリゴン)数
			}
		}
	}
}

//--------------------------------------------------
//ブロックの設定処理
//--------------------------------------------------
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];

		if (pBlock->bUse)
		{//ブロックが使用されている
			continue;
		}

		//ブロックが使用されていない

		pBlock->pos = pos;
		pBlock->fWidth = fWidth;
		pBlock->fHeight = fHeight;

		//種類はもう設定してある

		pBlock->bCollision = true;
		pBlock->bUse = true;		//使用している状態にする

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetMiddlepos(pVtx, pos, fWidth, fHeight);

		float fUTex = fWidth / (SCREEN_WIDTH / MAX_X_BLOCK / 2.0f);
		float fVTex = fHeight / (SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f);

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f, fUTex, 0.0f, fVTex);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;		//ここでfor文を抜ける
	}
}

//--------------------------------------------------
//ブロックの取得処理
//--------------------------------------------------
Block *GetBlock(void)
{
	return &s_aBlock[0];		//ブロックの情報を授ける
}

//--------------------------------------------------
//ブロックの当たり判定処理
//--------------------------------------------------
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bIsLanding = false;		//着地しているかどうか

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];		//ブロックの情報

		if (!pBlock->bUse)
		{//ブロックが使用されていない
			continue;
		}

		//ブロックが使用されている

		float fLeft = pBlock->pos.x - pBlock->fWidth;
		float fRight = pBlock->pos.x + pBlock->fWidth;
		float fTop = pBlock->pos.y - pBlock->fHeight;
		float fBottom = pBlock->pos.y + pBlock->fHeight;

		if ((pPos->x + fWidth > fLeft) && (pPos->x - fWidth < fRight))
		{//Xがブロックの範囲内
			if ((pPosOld->y <= fTop) && (pPos->y >= fTop))
			{//ブロックの下から上
				if (pBlock->bCollision)
				{//当たり判定あり
					pPos->y = fTop;
					bIsLanding = true;
				}
			}
			else if ((pPosOld->y - fHeight >= fBottom) && (pPos->y - fHeight < fBottom))
			{//ブロックの上から下
				if (pBlock->type == BLOCKTYPE_BLUE || pBlock->type == BLOCKTYPE_LIGHT_BLUE)
				{//指定の種類
					pPos->y = fBottom + fHeight;
					pMove->y *= -REFLECT_BOUND;
				}
			}
		}

		if ((pPosOld->y > fTop) && (pPosOld->y - fHeight < fBottom))
		{//前回のYがブロックの範囲内
			if ((pBlock->type == BLOCKTYPE_BLUE) || (pBlock->type == BLOCKTYPE_LIGHT_BLUE))
			{//指定の種類
				if ((pPosOld->x + fWidth <= fLeft) && (pPos->x + fWidth > fLeft))
				{//ブロックの左端
					pPos->x = fLeft - fWidth;
					pMove->x = 0.0f;
				}
				else if ((pPosOld->x - fWidth >= fRight) && (pPos->x - fWidth < fRight))
				{//ブロックの右端
					pPos->x = fRight + fWidth;
					pMove->x = 0.0f;
				}
			}
		}
	}

	return bIsLanding;
}

//--------------------------------------------------
//ブロックの上端の当たり判定処理
//--------------------------------------------------
void CollisionTopBlock(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];		//ブロックの情報

		if (!pBlock->bUse)
		{//ブロックが使用されていない
			continue;
		}

		float fLeft = pBlock->pos.x - pBlock->fWidth;
		float fRight = pBlock->pos.x + pBlock->fWidth;
		float fTop = pBlock->pos.y - pBlock->fHeight;
		float fBottom = pBlock->pos.y + pBlock->fHeight;

		if ((pPos->x + fWidth >= fLeft) && (pPos->x - fWidth <= fRight) &&
			(pPos->y >= fTop) && (pPos->y - fHeight <= fBottom))
		{//X,Yがブロックの範囲内
			if ((pBlock->type == BLOCKTYPE_B_THROUGH) ||
				(pBlock->type == BLOCKTYPE_L_THROUGH))
			{//通り抜けるブロックだったら
				s_bThrough = true;
			}
			else if ((pBlock->type == BLOCKTYPE_BLUE) ||
				(pBlock->type == BLOCKTYPE_LIGHT_BLUE))
			{//通り抜けないブロックだったら
				s_bThrough = false;
				break;
			}
		}
	}

	if (s_bThrough)
	{//通り抜ける
		for (int i = 0; i < MAX_BLOCK; i++)
		{
			Block *pBlock = &s_aBlock[i];		//ブロックの情報

			if (!pBlock->bUse)
			{//ブロックが使用されていない
				continue;
			}

			//ブロックが使用されている

			if ((pBlock->type == BLOCKTYPE_B_THROUGH) ||
				(pBlock->type == BLOCKTYPE_L_THROUGH))
			{//指定の種類
				pBlock->bCollision = false;
			}
		}
	}
}

//--------------------------------------------------
//構造体の初期化処理
//--------------------------------------------------
static void InitStruct(int i)
{
	s_aBlock[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aBlock[i].fWidth = 0.0f;
	s_aBlock[i].fHeight = 0.0f;

	//種類はもう設定してある

	s_aBlock[i].bCollision = false;
	s_aBlock[i].bUse = false;		//使用していない状態にする
}

//--------------------------------------------------
//種類の初期化処理
//--------------------------------------------------
static void InitType(void)
{
	FILE *pBlockFile;		//ファイルポインタを宣言
	char aData[FILE_DATA];
	BLOCKTYPE aBlock[MAX_Y_BLOCK][MAX_X_BLOCK];		//ブロックの種類

	//ファイルを開く
	pBlockFile = fopen(FILE_NAME, "r");

	if (pBlockFile != NULL)
	{//ファイルが開いた場合
		for (int y = 0;  y < MAX_Y_BLOCK; y++)
		{
			//一行読み込み
			fgets(aData, FILE_DATA, pBlockFile);

			for (int x = 0; x < MAX_X_BLOCK; x++)
			{
				//変換
				switch (atoi(&aData[x * 2]))
				{
				case 0:		//使用しない
				default:
					aBlock[y][x] = BLOCKTYPE_NONE;
					break;

				case 1:		//青
					aBlock[y][x] = BLOCKTYPE_BLUE;
					break;

				case 2:		//水色
					aBlock[y][x] = BLOCKTYPE_LIGHT_BLUE;
					break;

				case 3:		//青の通り抜ける
					aBlock[y][x] = BLOCKTYPE_B_THROUGH;
					break;

				case 4:		//水色の通り抜ける
					aBlock[y][x] = BLOCKTYPE_L_THROUGH;
					break;
				}
			}
		}

		//ファイルを閉じる
		fclose(pBlockFile);
	}
	else
	{//ファイルが開かない場合
		assert(false);

		//ファイルを閉じる
		fclose(pBlockFile);
	}

	int nCntUse = 0;

	float fWidth = SCREEN_WIDTH / MAX_X_BLOCK / 2.0f;
	float fHeight = SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f;

	for (int y = 0; y < MAX_Y_BLOCK; y++)
	{
		float fPosY = (SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f) + (y * SCREEN_HEIGHT / MAX_Y_BLOCK);

		for (int x = 0; x < MAX_X_BLOCK; x++)
		{
			switch (aBlock[y][x])
			{
			case BLOCKTYPE_NONE:		//使用しない
			default:
				break;

			case BLOCKTYPE_BLUE:			//青
			case BLOCKTYPE_LIGHT_BLUE:		//水色
			case BLOCKTYPE_B_THROUGH:		//青の通り抜ける
			case BLOCKTYPE_L_THROUGH:		//水色の通り抜ける

				float fPosX = (SCREEN_WIDTH / MAX_X_BLOCK / 2.0f) + (x * SCREEN_WIDTH / MAX_X_BLOCK);

				s_aBlock[nCntUse].type = aBlock[y][x];

				//ブロックの設定
				SetBlock(D3DXVECTOR3(fPosX, fPosY, 0.0f), fWidth, fHeight);
				nCntUse++;

				break;
			}
		}
	}
}

//--------------------------------------------------
//下がるときの当たり判定処理
//--------------------------------------------------
static void DownCollision(void)
{
	bool bCollision = false;

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];		//ブロックの情報

		if (!pBlock->bUse)
		{//ブロックが使用されていない
			continue;
		}

		//ブロックが使用されている

		if (pBlock->type == BLOCKTYPE_B_THROUGH ||
			pBlock->type == BLOCKTYPE_L_THROUGH)
		{//指定の種類
			Player *pPlayer = GetPlayer();		//プレイヤーの情報を取得

			float fPlayerLeft = pPlayer->pos.x - pPlayer->fWidth;
			float fPlayerRight = pPlayer->pos.x + pPlayer->fWidth;
			float fPlayerTop = pPlayer->pos.y - pPlayer->fHeight;
			float fPlayerBottom = pPlayer->pos.y;

			float fLeft = pBlock->pos.x - pBlock->fWidth;
			float fRight = pBlock->pos.x + pBlock->fWidth;
			float fTop = pBlock->pos.y - pBlock->fHeight;
			float fBottom = pBlock->pos.y + pBlock->fHeight;

			if (fPlayerRight >= fLeft && fPlayerLeft <= fRight &&
				fPlayerBottom >= fTop && fPlayerTop <= fBottom)
			{//X,Yがブロックの範囲内
				bCollision = true;
				break;
			}

			bCollision = false;
		}
	}

	if (!bCollision)
	{//通り抜ける
		for (int i = 0; i < MAX_BLOCK; i++)
		{
			Block *pBlock = &s_aBlock[i];		//ブロックの情報

			if (!pBlock->bUse)
			{//ブロックが使用されていない
				continue;
			}

			//ブロックが使用されている

			if (pBlock->type == BLOCKTYPE_B_THROUGH ||
				pBlock->type == BLOCKTYPE_L_THROUGH)
			{//指定の種類
				pBlock->bCollision = true;
			}
		}
	}
}