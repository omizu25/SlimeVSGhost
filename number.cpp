//--------------------------------------------------
//
// ACG制作 ( number.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "setup.h"
#include "number.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_NUMBER		(256)		//数の最大桁数
#define MAX_TEX			(10)		//texの最大数

//--------------------------------------------------
//数の構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	float			fWidth;			//幅
	float			fHeight;		//高さ
	int				nNumber;		//数
	int				nDigit;			//桁数
	bool			bUse;			//使用しているかどうか
}Number;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(Number *pNumber);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static Number						s_aNumber[MAX_NUMBER];		//数の情報

//--------------------------------------------------
//数の初期化処理
//--------------------------------------------------
void InitNumber(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number000.png",
		&s_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_aNumber[i];

		//構造体の初期化処理
		InitStruct(pNumber);

		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//数の終了処理
//--------------------------------------------------
void UninitNumber(void)
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
//数の更新処理
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
//数の描画処理
//--------------------------------------------------
void DrawNumber(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		if (s_aNumber[i].bUse)
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
//数の設定処理
//--------------------------------------------------
void SetNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_aNumber[i];

		if (pNumber->bUse)
		{//数が使用されている
			continue;
		}

		//数が使用されていない

		pNumber->pos = pos;
		pNumber->fWidth = fWidth;
		pNumber->fHeight = fHeight;
		pNumber->nNumber = nNumber;
		pNumber->nDigit = nDigit;
		pNumber->bUse = true;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetRightpos(pVtx, pos, fWidth, fHeight);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		//頂点カラーの設定処理
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//数のテクスチャの処理
//--------------------------------------------------
void TexNumber(int nNumber, int nDigit)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_aNumber[i];

		if (!pNumber->bUse || pNumber->nDigit != nDigit)
		{//数が使用されていない、桁数が違う
			continue;
		}

		//数が使用されている、桁数が同じ

		pNumber->nNumber = nNumber;

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//構造体の初期化処理
//--------------------------------------------------
static void InitStruct(Number *pNumber)
{
	pNumber->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pNumber->fWidth = 0.0f;
	pNumber->fHeight = 0.0f;
	pNumber->nNumber = 0;
	pNumber->nDigit = 0;
	pNumber->bUse = false;		//使用していない状態にする
}