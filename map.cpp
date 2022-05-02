//--------------------------------------------------
//
// アクションゲーム制作 ( map.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"
#include "map.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_X_MAP				(32)			//マップの横の最大数
#define MAX_Y_MAP				(18)			//マップの縦の最大数
#define FILE_DATA				(1024)			//ファイルのデータ数
#define MAP_FILE		"data/test_map.txt"		//マップのテキスト名

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAPTYPE_MAX] = { NULL, NULL };		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;								//頂点バッファのポインタ
static Map							s_aMap[MAX_Y_MAP][MAX_X_MAP];					//マップの情報

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStructMap(int nCntY, int nCntX);
static void InitMapType();

//--------------------------------------------------
//マップの初期化処理
//--------------------------------------------------
void InitMap(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block003.png",
		&s_pTexture[MAPTYPE_PURPLE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block002.png",
		&s_pTexture[MAPTYPE_BROWN]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block004.png",
		&s_pTexture[MAXTYPE_BLUE]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Y_MAP * MAX_X_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//マップのタイプの初期化処理
	InitMapType();

	for (int nCntY = 0; nCntY < MAX_Y_MAP; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_X_MAP; nCntX++)
		{
			//マップの構造体の初期化処理
			InitStructMap(nCntY, nCntX);

			//頂点座標の設定処理
			SetMiddlepos(pVtx, s_aMap[nCntY][nCntX].pos, SCREEN_WIDTH / MAX_X_MAP / 2.0f, SCREEN_HEIGHT / MAX_Y_MAP / 2.0f);

			//rhwの設定処理
			Setrhw(pVtx);

			//頂点カラーの設定処理
			Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定処理
			Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

			pVtx += 4;		//頂点データのポインタを４つ分進める
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//マップの終了処理
//--------------------------------------------------
void UninitMap(void)
{
	for (int nCnt = 0; nCnt < MAPTYPE_MAX; nCnt++)
	{
		if (s_pTexture[nCnt] != NULL)
		{//マップのテクスチャの破棄
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//マップの頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//マップの更新処理
//--------------------------------------------------
void UpdateMap(void)
{
	
}

//--------------------------------------------------
//マップの描画処理
//--------------------------------------------------
void DrawMap(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntType = 0; nCntType < MAPTYPE_MAX; nCntType++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[nCntType]);

		for (int nCntY = 0; nCntY < MAX_Y_MAP; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAX_X_MAP; nCntX++)
			{
				if (s_aMap[nCntY][nCntX].type == nCntType)
				{
					//ポリゴンの描画
					pDevice->DrawPrimitive(
						D3DPT_TRIANGLESTRIP,						//プリミティブの種類
						nCntX * 4 + (nCntY * MAX_X_MAP * 4),		//描画する最初の頂点インデックス
						2);											//プリミティブ(ポリゴン)数
				}
			}
		}
	}
}

//--------------------------------------------------
//マップの取得処理
//--------------------------------------------------
Map *GetMap(void)
{
	return &s_aMap[0][0];
}

//--------------------------------------------------
//マップの構造体の初期化処理
//--------------------------------------------------
static void InitStructMap(int nCntY, int nCntX) 
{
	//位置を初期化
	s_aMap[nCntY][nCntX].pos.x = (SCREEN_WIDTH / MAX_X_MAP / 2.0f) + (nCntX * SCREEN_WIDTH / MAX_X_MAP);
	s_aMap[nCntY][nCntX].pos.y = (SCREEN_HEIGHT / MAX_Y_MAP / 2.0f) + (nCntY * SCREEN_HEIGHT / MAX_Y_MAP);
	s_aMap[nCntY][nCntX].pos.z = 0.0f;

	//移動量を初期化
	s_aMap[nCntY][nCntX].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
//マップのタイプの初期化処理
//--------------------------------------------------
static void InitMapType()
{
	FILE *pMapFile;			//ファイルポインタを宣言
	char adata[FILE_DATA];

	//ファイルを開く
	pMapFile = fopen(MAP_FILE, "r");

	if (pMapFile != NULL)
	{//ファイルが開いた場合
		for (int nCntY = 0; nCntY < MAX_Y_MAP; nCntY++)
		{
			//一行読み込み
			fgets(adata, FILE_DATA, pMapFile);

			for (int nCntX = 0; nCntX < MAX_X_MAP; nCntX++)
			{
				//変換
				switch (atoi(&adata[nCntX * 2]))
				{
				case 0:		//紫
				default:
					s_aMap[nCntY][nCntX].type = MAPTYPE_PURPLE;
					break;

				case 1:		//茶色
					s_aMap[nCntY][nCntX].type = MAPTYPE_BROWN;
					break;
				case 2:		//青
					s_aMap[nCntY][nCntX].type = MAXTYPE_BLUE;
					break;
				}
			}
		}

		//ファイルを閉じる
		fclose(pMapFile);
	}
	else
	{//ファイルが開かない場合
		assert(false);

		//ファイルを閉じる
		fclose(pMapFile);
	}
}