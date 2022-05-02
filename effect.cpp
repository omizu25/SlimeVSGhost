//--------------------------------------------------
//
// ACG制作 ( effect.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "effect.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_EFFECT			(4096)		//エフェクトの最大数
#define MAX_TEX				(3)			//テクスチャの最大数
#define MAX_IN				(2)			//吸い込みの最大数
#define MAX_STAR			(7)			//スターの最大数
#define MAX_EXPLOSION		(50)		//爆発の最大数

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
	EFFECTTYPE		type;			//種類
	bool			bUse;			//使用しているかどうか
}Effect;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static Effect						s_aEffect[MAX_EFFECT];		//エフェクトの情報

//--------------------------------------------------
//エフェクトの初期化処理
//--------------------------------------------------
void InitEffect(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//メモリのクリア
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect100.jpg",
		&s_pTexture[EFFECTTYPE_IN]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect002.jpg",
		&s_pTexture[EFFECTTYPE_STAR]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect000.jpg",
		&s_pTexture[EFFECTTYPE_MOVE]);

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
	memset(&s_aEffect[0], NULL, sizeof(s_aEffect));

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

		pEffect->pos += pEffect->move;

		switch (pEffect->type)
		{
		case EFFECTTYPE_IN:			//吸い込み
			pEffect->fHeight -= 2.05f;
			break;

		case EFFECTTYPE_STAR:			//スター
		case EFFECTTYPE_EXPLOSION:		//爆発
			pEffect->col.r = (float)(rand() % 101) / 100.0f;
			pEffect->col.g = (float)(rand() % 101) / 100.0f;
			pEffect->col.b = (float)(rand() % 101) / 100.0f;
			pEffect->col.a = 1.0f;

			break;

		case EFFECTTYPE_MOVE:			//移動

			break;

		default:
			assert(false);
			break;
		}

		pEffect->nLife--;

		if (pEffect->nLife <= 0)
		{//寿命が来た
			pEffect->bUse = false;
		}

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の更新
		SetMiddlepos(pVtx, pEffect->pos, pEffect->fWidth, pEffect->fHeight);

		//頂点カラーの設定
		Setcol(pVtx, pEffect->col.r, pEffect->col.g, pEffect->col.b, pEffect->col.a);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//エフェクトの描画処理
//--------------------------------------------------
void DrawEffect(EFFECTTYPE type)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//αブレンディングを加工合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				//そのまま足す

	//エフェクトの描画
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (s_aEffect[i].bUse && s_aEffect[i].type == type)
		{//エフェクトが使用されている、種類が同じ時

			if (type == EFFECTTYPE_EXPLOSION)
			{//爆発の時
				//テクスチャの設定
				pDevice->SetTexture(0, s_pTexture[EFFECTTYPE_STAR]);
			}
			else
			{
				//テクスチャの設定
				pDevice->SetTexture(0, s_pTexture[type]);
			}

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
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, EFFECTTYPE type)
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
		pEffect->type = type;
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

//--------------------------------------------------
//パーティクルの設定処理
//--------------------------------------------------
void SetParticle(D3DXVECTOR3 pos, EFFECTTYPE type, bool bDirection)
{
	D3DXVECTOR3 Pos;		//位置
	D3DXVECTOR3	move;		//移動量
	D3DXCOLOR col;			//色
	float fDirection;		//方向
	float fWidth;			//幅
	float fHeight;			//高さ
	int nLife;				//寿命(表示時間)

	switch (type)
	{
	case EFFECTTYPE_IN:				//吸い込み

		for (int i = 0; i < MAX_IN; i++)
		{
			if (bDirection)
			{//右向き
				Pos.x = (float)((rand() % 1001) - 500) / 100.0f + pos.x + 250.0f;
			}
			else
			{//左向き
				Pos.x = (float)((rand() % 1001) - 500) / 100.0f + pos.x - 250.0f;
			}

			Pos.y = (float)((rand() % 1601) - 800) / 100.0f + pos.y;
			Pos.z = 0.0f;

			move.x = (float)(rand() % 1501) / 1000.0f + 12.5f;
			move.y = (float)(rand() % 901) / 1000.0f + 2.0f;
			move.z = 0.0f;

			col = D3DXCOLOR(0.1f, 0.3f, 0.6f, 1.0f);

			fDirection = (float)((rand() % (int)(0.32f * 100 + 1)) - (0.32f * 100)) / 100.0f;

			fWidth = (float)(rand() % 51) / 100.0f + 5.0f;
			fHeight = (float)(rand() % 51) / 100.0f + 45.0f;

			nLife = (rand() % 11) + 7;

			if (bDirection)
			{//右向き
				//エフェクトの設定
				SetEffect(Pos, D3DXVECTOR3(sinf(fDirection + -1.59f) * move.x,
					cosf(fDirection + -1.59f) * move.y, 0.0f),
					col, fWidth, fHeight, nLife, type);
			}
			else
			{//左向き
				//エフェクトの設定
				SetEffect(Pos, D3DXVECTOR3(sinf(fDirection + 1.59f) * move.x,
					cosf(fDirection + 1.59f) * move.y, 0.0f),
					col, fWidth, fHeight, nLife, type);
			}
		}

		break;

	case EFFECTTYPE_STAR:			//スター

		for (int i = 0; i < MAX_STAR; i++)
		{
			Pos.x = (float)(rand() % 501) / 100.0f + pos.x;
			Pos.y = (float)((rand() % 1601) - 800) / 30.0f + pos.y;
			Pos.z = 0.0f;

			move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			col.r = (float)(rand() % 101) / 100.0f;
			col.g = (float)(rand() % 101) / 100.0f;
			col.b = (float)(rand() % 101) / 100.0f;
			col.a = 1.0f;

			fDirection = (float)((rand() % (int)(0.32f * 100 + 1)) - (0.32f * 100)) / 100.0f;

			fWidth = (float)(rand() % 51) / 100.0f + 15.0f;
			fHeight = fWidth;

			nLife = (rand() % 11) + 10;

			//エフェクトの設定
			SetEffect(Pos, move, col, fWidth, fHeight, nLife, type);
		}

		break;

	case EFFECTTYPE_EXPLOSION:		//爆発

		for (int i = 0; i < MAX_EXPLOSION; i++)
		{
			Pos.x = (float)((rand() % 2001) - 1000) / 100.0f + pos.x;
			Pos.y = (float)((rand() % 2001) - 1000) / 100.0f + pos.y;
			Pos.z = 0.0f;

			move.x = (float)(rand() % (2001)) / 1000.0f + 0.5f;
			move.y = (float)(rand() % (2001)) / 1000.0f + 0.5f;
			move.z = 0.0f;

			col.r = (float)(rand() % 101) / 100.0f;
			col.g = (float)(rand() % 101) / 100.0f;
			col.b = (float)(rand() % 101) / 100.0f;
			col.a = 1.0f;

			fDirection = (float)((rand() % (int)(D3DX_PI * 2.0f * 100 + 1)) - (D3DX_PI * 100)) / 100.0f;

			fWidth = (float)(rand() % 1001) / 100.0f + 7.5f;
			fHeight = fWidth;

			nLife = (rand() % 31) + 50;

			//エフェクトの設定
			SetEffect(Pos, D3DXVECTOR3(sinf(fDirection) * move.x, cosf(fDirection) * move.y, 0.0f),
				col, fWidth, fHeight, nLife, type);
		}

		break;

	case EFFECTTYPE_MOVE:			//移動

		//移動はパーティクルではないので来てはいけない

	default:
		assert(false);
		break;
	}
}