//--------------------------------------------------
//
// STG制作 ( block.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "input.h"
#include "player.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define FILE_DATA				(1024)					//ファイルのデータ数
#define BlOCK_FILE		"data/test_map.txt"				//マップのテキスト名
#define MAX_TEX			(BLOCKTYPE_MAX - 1)				//テクスチャの最大数

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static Block						s_aBlock[MAX_BLOCK];		//ブロックの情報
static bool							s_bTexUse;					//テクスチャの使用するかどうか
static bool							s_bCollisionUse;			//当たり判定を判断するかどうか

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStructBlock(int i);
static void InitBlockType(void);

//--------------------------------------------------
//ブロックの初期化処理
//--------------------------------------------------
void InitBlock(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_bTexUse = true;
	s_bCollisionUse = true;

	for (int i = 0; i < MAX_TEX; i++)
	{//メモリのクリア
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block002.png",
		&s_pTexture[BLOCKTYPE_BROWN]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block006.png",
		&s_pTexture[BLOCKTYPE_ORANGE]);

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
		//プレイヤーの構造体の初期化処理
		InitStructBlock(i);

		//頂点座標の設定処理
		SetMiddlepos(pVtx, s_aBlock[i].pos, s_aBlock[i].fWidth, s_aBlock[i].fHeight);

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

	//ブロックの種類の初期化処理
	InitBlockType();
}

//--------------------------------------------------
//ブロックの終了処理
//--------------------------------------------------
void UninitBlock(void)
{
	for (int i = 0; i < MAX_TEX; i++)
	{
		if (s_pTexture[i] != NULL)
		{//プレイヤーのテクスチャの破棄
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
#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_F3))
	{//F3キー(F3キー)が押されたかどうか
		s_bTexUse = !s_bTexUse;
	}

	if (GetKeyboardTrigger(DIK_F4))
	{//F4キー(F4キー)が押されたかどうか
		s_bCollisionUse = !s_bCollisionUse;
	}

#endif //  _DEBUG
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
		if (s_bTexUse)
		{//使用する
			pDevice->SetTexture(0, s_pTexture[i]);
		}
		else
		{//使用しない
			pDevice->SetTexture(0, NULL);
		}

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

		//ブロックが使用されていないq

		pBlock->pos = pos;
		pBlock->fWidth = fWidth;
		pBlock->fHeight = fHeight;

		//種類はもう設定してある

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

		//頂点カラーの設定処理
		Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

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
//テクスチャを使用するかの取得処理
//--------------------------------------------------
bool GetTexUseBlock(void)
{
	return s_bTexUse;
}

//--------------------------------------------------
//当たり判定を判断するかの取得処理
//--------------------------------------------------
bool GetCollisionUse(void)
{
	return s_bCollisionUse;
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

		bool bCollision = false;

		if (pPos->x + fWidth > (pBlock->pos.x - pBlock->fWidth) &&
			pPos->x - fWidth < (pBlock->pos.x + pBlock->fWidth))
		{//Xがブロックの範囲内
			if (pPosOld->y <= (pBlock->pos.y - pBlock->fHeight) &&
				pPos->y >= (pBlock->pos.y - pBlock->fHeight))
			{//ブロックの下から上
				pPos->y = pBlock->pos.y - pBlock->fHeight;
				bIsLanding = true;
				bCollision = true;
			}
			else if (pPosOld->y - fHeight >= (pBlock->pos.y + pBlock->fHeight) &&
				pPos->y - fHeight < (pBlock->pos.y + pBlock->fHeight) &&
				pBlock->type == BLOCKTYPE_BROWN)
			{//ブロックの上から下
				pPos->y = pBlock->pos.y + pBlock->fHeight + fHeight;
				pMove->y *= -REFLECT_BOUND;
				bCollision = true;
			}
		}

		if (pPosOld->y > (pBlock->pos.y - pBlock->fHeight) &&
			pPosOld->y - fHeight < (pBlock->pos.y + pBlock->fHeight) &&
			pBlock->type == BLOCKTYPE_BROWN)
		{//前回のYがブロックの範囲内
			if (pPosOld->x + fWidth <= (pBlock->pos.x - pBlock->fWidth) &&
				pPos->x + fWidth > (pBlock->pos.x - pBlock->fWidth))
			{//ブロックの左端
				pPos->x = pBlock->pos.x - pBlock->fWidth - fWidth;
				pMove->x = 0.0f;
				bCollision = true;
			}
			else if (pPosOld->x - fWidth >= (pBlock->pos.x + pBlock->fWidth) &&
					 pPos->x - fWidth < (pBlock->pos.x + pBlock->fWidth))
			{//ブロックの右端
				pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth;
				pMove->x = 0.0f;
				bCollision = true;
			}
		}

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		if (s_bCollisionUse)
		{
			if (bCollision)
			{//ブロックに当たった
				//頂点カラーの設定処理
				Setcol(pVtx, 0.75f, 0.0f, 0.75f, 1.0f);
			}
			else
			{//ブロックに当たってない
				//頂点カラーの設定処理
				Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		else
		{
			//頂点カラーの設定処理
			Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

		}

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}

	return bIsLanding;
}

//--------------------------------------------------
//ブロックの構造体の初期化処理
//--------------------------------------------------
static void InitStructBlock(int i)
{
	s_aBlock[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aBlock[i].fWidth = 0.0f;
	s_aBlock[i].fHeight = 0.0f;

	//種類はもう設定してある

	s_aBlock[i].bUse = false;		//使用していない状態にする
}

//--------------------------------------------------
//ブロックの種類の初期化処理
//--------------------------------------------------
static void InitBlockType(void)
{
	FILE *pBlockFile;		//ファイルポインタを宣言
	char aData[FILE_DATA];
	BLOCKTYPE aBlock[MAX_Y_BLOCK][MAX_X_BLOCK];		//ブロックの種類

	//ファイルを開く
	pBlockFile = fopen(BlOCK_FILE, "r");

	if (pBlockFile != NULL)
	{//ファイルが開いた場合
		for (int i = 0;  i< MAX_Y_BLOCK; i++)
		{
			//一行読み込み
			fgets(aData, FILE_DATA, pBlockFile);

			for (int j = 0; j < MAX_X_BLOCK; j++)
			{
				//変換
				switch (atoi(&aData[j * 2]))
				{
				case 0:		//使用しない
				default:
					aBlock[i][j] = BLOCKTYPE_NONE;
					break;

				case 1:		//茶色
					aBlock[i][j] = BLOCKTYPE_BROWN;
					break;

				case 2:		//オレンジ
					aBlock[i][j] = BLOCKTYPE_ORANGE;
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

	for (int i = 0; i < MAX_Y_BLOCK; i++)
	{
		float fPosY = (SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f) + (i * SCREEN_HEIGHT / MAX_Y_BLOCK);

		for (int j = 0; j < MAX_X_BLOCK; j++)
		{
			switch (aBlock[i][j])
			{
			case BLOCKTYPE_NONE:		//使用しない
			default:
				continue;
				break;

			case BLOCKTYPE_BROWN:		//茶色
			case BLOCKTYPE_ORANGE:		//オレンジ

				float fPosX = (SCREEN_WIDTH / MAX_X_BLOCK / 2.0f) + (j * SCREEN_WIDTH / MAX_X_BLOCK);

				s_aBlock[nCntUse].type = aBlock[i][j];

				//ブロックの設定
				SetBlock(D3DXVECTOR3(fPosX, fPosY, 0.0f), fWidth, fHeight);
				nCntUse++;

				break;
			}
		}
	}
}