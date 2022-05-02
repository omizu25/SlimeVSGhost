//--------------------------------------------------
//
// ACG制作 ( result.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "fade.h"
#include "input.h"
#include "result.h"
#include "setup.h"
#include "sound.h"

#include <assert.h>

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[RESULT_MAX];		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static RESULT						s_Result;					//リザルトの情報

//--------------------------------------------------
//リザルトの初期化処理
//--------------------------------------------------
void InitResult(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//メモリのクリア
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result000.png",
		&s_pTexture[RESULT_LOSE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result003.png",
		&s_pTexture[RESULT_WIN]);

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
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, pos, fWidth, fHeight);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの初期化処理
	Initcol(pVtx);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	if (s_Result == RESULT_LOSE)
	{//負け
		//サウンドの再生
		PlaySound(SOUND_LABEL_MYSTERYMAN);
	}
	else if (s_Result == RESULT_WIN)
	{//勝ち
		//サウンドの再生
		PlaySound(SOUND_LABEL_START_BEETS);
	}
	
}

//--------------------------------------------------
//リザルトの終了処理
//--------------------------------------------------
void UninitResult(void)
{
	//サウンドの停止
	StopSound();

	for (int i = 0; i < RESULT_MAX; i++)
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
//リザルトの更新処理
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
	{//決定キー(ENTERキー)が押されたかどうか
		switch (s_Result)
		{
		case RESULT_WIN:		//勝ち
			//フェード設定
			SetFade(MODE_RANKING);
			break;

		case RESULT_LOSE:		//負け
			//フェード設定
			SetFade(MODE_TITLE);
			break;

		case RESULT_NONE:		//何もしていない状態

			//breakなし

		default:
			assert(false);
			break;
		}
	}
}

//--------------------------------------------------
//リザルトの描画処理
//--------------------------------------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture[s_Result]);
	
	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数
}

//--------------------------------------------------
//リザルトの設定
//--------------------------------------------------
void SetResult(RESULT Result)
{
	if (s_Result == RESULT_NONE || Result == RESULT_NONE)
	{//まだリザルトが決まってない、リザルトの初期化
		s_Result = Result;		//リザルトを代入
	}
}