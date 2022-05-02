//--------------------------------------------------
//
// ACG制作 ( tutorial.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "fade.h"
#include "input.h"
#include "number.h"
#include "setup.h"
#include "sound.h"
#include "tutorial.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MIN_MOVE			(15.0f)			//移動量の最小値
#define MAX_MOVE			(30.0f)			//移動量の最大値
#define NEXT_WIDTH			(80.0f)			//つぎへの幅
#define NEXT_HEIGHT			(400.0f)		//つぎへの高さ
#define END_WIDTH			(800.0f)		//終わりの幅
#define END_HEIGHT			(60.0f)			//終わりの高さ
#define NUMBER_WIDTH		(60.0f)			//数の幅
#define NUMBER_HEIGHT		(80.0f)			//数の高さ

//--------------------------------------------------
//メニューを定義
//--------------------------------------------------
typedef enum
{
	MENU_RULE = 0,		//ルール
	MENU_OPERATION,		//操作
	MENU_MAX
}MENU;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void UpdateInput(void);
static void UpdateMove(void);
static void UpdateOffScreen(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			//背景の頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTexture[MENU_MAX];			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureNext[MENU_MAX];		//つぎへのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffNext = NULL;			//つぎへの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureEnd = NULL;			//終わりのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffEnd = NULL;			//終わりの頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureBar = NULL;			//線のテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBar = NULL;			//線の頂点バッファのポインタ
static MENU							s_menu;							//現在のメニュー
static D3DXVECTOR3					s_pos;							//位置
static D3DXVECTOR3					s_move;							//移動量

//--------------------------------------------------
//チュートリアルの初期化処理
//--------------------------------------------------
void InitTutorial(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//メモリのクリア
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	memset(&s_pTextureNext[0], NULL, sizeof(s_pTextureNext));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title005.png",
		&s_pTexture[MENU_RULE]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title012.png",
		&s_pTexture[MENU_OPERATION]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title010.png",
		&s_pTextureNext[MENU_RULE]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title011.png",
		&s_pTextureNext[MENU_OPERATION]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title009.png",
		&s_pTextureEnd);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number002.png",
		&s_pTextureBar);

	s_pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0.0f);
	s_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_menu = MENU_RULE;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffNext,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffEnd,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBar,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	/* ↓背景↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, pos, fWidth, fHeight);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 0.85f, 0.85f, 0.85f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuffBG->Unlock();
		
	/* ↓説明画像↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MENU_MAX; i++)
	{
		//頂点座標の設定処理
		SetMiddlepos(pVtx, D3DXVECTOR3(pos.x + (SCREEN_WIDTH * i), pos.y, 0.0f), fWidth * 0.75f, fHeight * 0.75f);

		//rhwの初期化処理
		Initrhw(pVtx);

		//頂点カラーの初期化処理
		Initcol(pVtx);

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f, 1.0f, 0.001f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/* ↓つぎへ↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffNext->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthNext = NEXT_WIDTH * 0.5f;
	float fHeightNext = NEXT_HEIGHT * 0.5f;
	D3DXVECTOR3 posNext = D3DXVECTOR3(s_pos.x - NEXT_WIDTH, fHeight, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, posNext, fWidthNext, fHeightNext);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの初期化処理
	Initcol(pVtx);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuffNext->Unlock();

	/* ↓終わり↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthEnd = END_WIDTH * 0.5f;
	float fHeightEnd = END_HEIGHT * 0.5f;
	D3DXVECTOR3 posEnd = D3DXVECTOR3(pos.x, SCREEN_HEIGHT - END_HEIGHT * 0.75f, 0.0f);

	//頂点座標の設定処理
	SetMiddlepos(pVtx, posEnd, fWidthEnd, fHeightEnd);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの初期化処理
	Initcol(pVtx);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuffEnd->Unlock();

	/* ↓線↓ */

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);

	float fHeightNumber = NUMBER_HEIGHT * 0.5f;
	D3DXVECTOR3 posNumber = D3DXVECTOR3(fWidth - (NUMBER_WIDTH * 0.5f), fHeightNumber, 0.0f);

	//頂点座標の設定処理
	SetRightpos(pVtx, D3DXVECTOR3(posNumber.x + NUMBER_WIDTH, posNumber.y, 0.0f), NUMBER_WIDTH, fHeightNumber * 0.75f);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の初期化処理
	Inittex(pVtx);

	//頂点バッファをアンロックする
	s_pVtxBuffBar->Unlock();

	/* ↓数↓ */

	//数の初期化処理
	InitNumber();

	for (int i = 0; i < MENU_MAX; i++)
	{
		//数の設定処理
		SetRightNumber(D3DXVECTOR3(posNumber.x + ((NUMBER_WIDTH * i) * 2.0f), posNumber.y, 0.0f), NUMBER_WIDTH, fHeightNumber, i + 1, i, 0);
	}

	//サウンドの再生
	PlaySound(SOUND_LABEL_RISE);
}

//--------------------------------------------------
//チュートリアルの終了処理
//--------------------------------------------------
void UninitTutorial(void)
{
	//サウンドの停止
	StopSound();

	if (s_pVtxBuffBG != NULL)
	{//背景の頂点バッファの破棄
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	for (int i = 0; i < MENU_MAX; i++)
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

	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureNext[i] != NULL)
		{//つぎへのテクスチャの破棄
			s_pTextureNext[i]->Release();
			s_pTextureNext[i] = NULL;
		}
	}

	if (s_pVtxBuffNext != NULL)
	{//つぎへの頂点バッファの破棄
		s_pVtxBuffNext->Release();
		s_pVtxBuffNext = NULL;
	}

	if (s_pTextureEnd != NULL)
	{//終わりのテクスチャの破棄
		s_pTextureEnd->Release();
		s_pTextureEnd = NULL;
	}

	if (s_pVtxBuffEnd != NULL)
	{//終わりの頂点バッファの破棄
		s_pVtxBuffEnd->Release();
		s_pVtxBuffEnd = NULL;
	}

	if (s_pTextureBar != NULL)
	{//線のテクスチャの破棄
		s_pTextureBar->Release();
		s_pTextureBar = NULL;
	}

	if (s_pVtxBuffBar != NULL)
	{//線の頂点バッファの破棄
		s_pVtxBuffBar->Release();
		s_pVtxBuffBar = NULL;
	}

	//数の終了処理
	UninitNumber();
}

//--------------------------------------------------
//チュートリアルの更新処理
//--------------------------------------------------
void UpdateTutorial(void)
{
	//入力処理
	UpdateInput();
	
	//移動処理
	UpdateMove();
}

//--------------------------------------------------
//チュートリアルの描画処理
//--------------------------------------------------
void DrawTutorial(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MENU_MAX; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			i * 4,						//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffNext, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureNext[s_menu]);

	if (s_move.x == 0.0f)
	{//移動量がない時
		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,							//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffEnd, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureEnd);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBar, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureBar);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//数の描画処理
	DrawNumber();
}

//--------------------------------------------------
//入力処理
//--------------------------------------------------
static void UpdateInput(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_B) ||
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_START))
	{//決定キー(ENTERキー)が押されたかどうか
		if (GetFade() == FADE_NONE)
		{//何もしていない
			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_SYSTEM49);
		}

		//フェードの設定
		SetFade(MODE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) ||
		GetJoypadTrigger(JOYKEY_LEFT))
	{//Aキーが押された
		if (s_pos.x != SCREEN_WIDTH)
		{
			if (s_move.x >= MIN_MOVE)
			{//押されたが２回以上
				s_move.x = MAX_MOVE;
			}
			else
			{//１回目
				s_move.x = MIN_MOVE;

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_SYSTEM40);
			}
		}
	}
	else if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT) ||
		GetJoypadTrigger(JOYKEY_RIGHT))
	{//Dキーが押された
		if (s_pos.x != 0.0f)
		{
			if (s_move.x <= -MIN_MOVE)
			{//押されたが２回以上
				s_move.x = -MAX_MOVE;
			}
			else
			{//１回目
				s_move.x = -MIN_MOVE;

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_SYSTEM40);
			}
		}
	}
}

//--------------------------------------------------
//移動処理
//--------------------------------------------------
static void UpdateMove(void)
{
	if (s_move.x != 0.0f)
	{//移動量がある時
		//位置を更新
		s_pos.x += s_move.x;

		//画面外処理
		UpdateOffScreen();

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fWidth = SCREEN_WIDTH * 0.5f;
		float fHeight = SCREEN_HEIGHT * 0.5f;
		D3DXVECTOR3 pos = s_pos - D3DXVECTOR3(fWidth, 0.0f, 0.0f);

		for (int i = 0; i < MENU_MAX; i++)
		{
			//頂点座標の設定処理
			SetMiddlepos(pVtx, D3DXVECTOR3(pos.x + (SCREEN_WIDTH * i), pos.y, 0.0f), fWidth * 0.75f, fHeight * 0.75f);

			pVtx += 4;		//頂点データのポインタを４つ分進める
		}

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		if (s_pos.x >= fWidth)
		{//画面の真ん中より左
			s_menu = MENU_RULE;
		}
		else
		{//画面の真ん中より右
			s_menu = MENU_OPERATION;
		}

		//数のテクスチャの処理
		TexNumber((s_menu + 1), 0);
	}
}

//--------------------------------------------------
//画面外処理
//--------------------------------------------------
static void UpdateOffScreen(void)
{
	if (s_pos.x >= SCREEN_WIDTH)
	{//右端
		s_pos.x = SCREEN_WIDTH;
		s_move.x = 0.0f;
	}
	else if (s_pos.x <= 0.0f)
	{//左端
		s_pos.x = 0.0f;
		s_move.x = 0.0f;
	}

	if (s_pos.x >= SCREEN_WIDTH || s_pos.x <= 0.0f)
	{//右端か左端
		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffNext->Lock(0, 0, (void**)&pVtx, 0);

		float fWidthNext = NEXT_WIDTH * 0.5f;
		float fHeightNext = NEXT_HEIGHT * 0.5f;
		D3DXVECTOR3 posNext = s_pos + D3DXVECTOR3(-NEXT_WIDTH + ((NEXT_WIDTH * 2.0f) * s_menu), 0.0f, 0.0f);

		//頂点座標の設定処理
		SetMiddlepos(pVtx, posNext, fWidthNext, fHeightNext);

		//頂点バッファをアンロックする
		s_pVtxBuffNext->Unlock();
	}
}