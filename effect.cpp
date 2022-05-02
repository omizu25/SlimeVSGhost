//--------------------------------------------------
//
// ACG制作 ( effect.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "effect.h"
#include "setup.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_EFFECT		(4096)		//エフェクトの最大数

//--------------------------------------------------
//エフェクトの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	D3DXVECTOR3		move;			//移動量
	D3DXCOLOR		col;			//色
	float			fWidth;			//幅
	float			fHeight;		//高さ
	int				nLife;			//寿命(表示時間)
	int				nType;			//種類
	bool			bUse;			//使用しているかどうか
}Effect;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static Effect						s_aEffect[MAX_EFFECT];		//エフェクトの情報

//--------------------------------------------------
//エフェクトの初期化処理
//--------------------------------------------------
void InitEffect(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect101.jpg",
		&s_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//メモリのクリア
	memset(&s_aEffect, NULL, sizeof(Effect));

	//もろもろの設定
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//エフェクトの終了処理
//--------------------------------------------------
void UninitEffect(void)
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
//エフェクトの更新処理
//--------------------------------------------------
void UpdateEffect(void)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (!pEffect->bUse)
		{//エフェクトが使用されていない
			continue;
		}

		//エフェクトが使用されている

		pEffect->nLife--;

		if (pEffect->nLife <= 0)
		{
			pEffect->bUse = false;
		}
	}
}

//--------------------------------------------------
//エフェクトの描画処理
//--------------------------------------------------
void DrawEffect(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	//αブレンディングを加工合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				//そのまま足す

	//エフェクトの描画
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (s_aEffect[i].bUse)
		{//エフェクトが使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				i * 4,						//描画する最初の頂点インデックス
				2);							//プリミティブ(ポリゴン)数
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		//半分足す
}

//--------------------------------------------------
//エフェクトの設定処理
//--------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, int nType)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (pEffect->bUse)
		{//エフェクトが使用されている
			continue;
		}

		//エフェクトが使用されていない

		pEffect->pos = pos;
		pEffect->move = move;
		pEffect->col = col;
		pEffect->fWidth = fWidth;
		pEffect->fHeight = fHeight;
		pEffect->nLife = nLife;
		pEffect->nType = nType;
		pEffect->bUse = true;		//使用している状態にする

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の更新
		SetMiddlepos(pVtx, pos, fWidth, fHeight);

		//頂点カラーの設定
		Setcol(pVtx, col.r, col.g, col.b, col.a);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;		//ここでfor文を抜ける
	}
}