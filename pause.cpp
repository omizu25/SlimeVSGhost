//--------------------------------------------------
//
// ACG制作 ( pause.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "fade.h"
#include "game.h"
#include "input.h"
#include "pause.h"

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;					//頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMenu[PAUSEMENU_MAX];		//メニューのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMenu = NULL;				//メニューの頂点バッファのポインタ
static Pause						s_aPause[PAUSEMENU_MAX];			//ポーズメニューの情報
static D3DXVECTOR3					s_posPause;							//ポーズの位置
static D3DXCOLOR					s_acolPause[NUM_PAUSE];				//ポーズの色
static PAUSEMENU					s_PauseMenu;						//ポーズメニュー
static float						s_fPausecolor;						//ポーズの色のαの数値

//--------------------------------------------------
//ポーズの初期化処理
//--------------------------------------------------
void InitPause(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < PAUSEMENU_MAX; i++)
	{//メモリのクリア
		memset(&s_pTextureMenu[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause002.png",
		&s_pTextureMenu[PAUSEMENU_GAME]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause006.png",
		&s_pTextureMenu[PAUSEMENU_RETRY]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause001.png",
		&s_pTextureMenu[PAUSEMENU_TITLE]);

	s_posPause = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		//位置の初期化

	//色の初期化
	s_acolPause[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	s_acolPause[1] = D3DXCOLOR(0.0f, 0.85f, 1.0f, 0.95f);

	for (int i = 0; i < PAUSEMENU_MAX; i++)
	{
		//位置の初期化
		s_aPause[i].pos.x = SCREEN_WIDTH * 0.5f;
		s_aPause[i].pos.y = 250.0f + (i * 110.0f);
		s_aPause[i].pos.z = 0.0f;

		//色の初期化
		if (i == 0)
		{
			s_aPause[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			s_aPause[i].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		}

		//状態(点滅具合)の初期化
		s_aPause[i].state = PAUSESTATE_IN;
	}
	
	s_PauseMenu = PAUSEMENU_GAME;		//ポーズメニューの初期化
	s_fPausecolor = 0.025f;				//ポーズの色のαの数値の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * PAUSEMENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMenu,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_PAUSE; i++)
	{
		if (i == 0)
		{
			//頂点座標の設定処理
			SetMiddlepos(pVtx, s_posPause, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		}
		else if (i == 1)
		{
			//頂点座標の設定処理
			SetMiddlepos(pVtx, s_posPause, PAUSE_WIDTH, PAUSE_HEIGHT);
		}
		
		//rhwの設定処理
		Setrhw(pVtx);

		//頂点カラーの設定処理
		Setcol(pVtx, s_acolPause[i].r, s_acolPause[i].g, s_acolPause[i].b, s_acolPause[i].a);

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < PAUSEMENU_MAX; i++)
	{
		//頂点座標の設定処理
		SetMiddlepos(pVtx, s_aPause[i].pos, PAUSEMENU_WIDTH, PAUSEMENU_HEIGHT);

		//rhwの設定処理
		Setrhw(pVtx);

		//頂点カラーの設定処理
		Setcol(pVtx, s_aPause[i].col.r, s_aPause[i].col.g, s_aPause[i].col.b, s_aPause[i].col.a);

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffMenu->Unlock();
}

//--------------------------------------------------
//ポーズの終了処理
//--------------------------------------------------
void UninitPause(void)
{
	for (int i = 0; i < PAUSEMENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{//メニューのテクスチャの破棄
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	if (s_pVtxBuffMenu != NULL)
	{//メニューの頂点バッファの破棄
		s_pVtxBuffMenu->Release();
		s_pVtxBuffMenu = NULL;
	}
}

//--------------------------------------------------
//ポーズの更新処理
//--------------------------------------------------
void UpdatePause(void)
{
	int nPauseMenu = (int)s_PauseMenu;		//ポーズメニューをintに

	FADE pFade = GetFade();		//今のフェード

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	if (pFade == FADE_NONE)
	{//何もしていない状態なら
		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S) ||
			GetJoypadTrigger(JOYKEY_UP) || GetJoypadTrigger(JOYKEY_DOWN))
		{//Wキー、Sキーが押されたかどうか
			//色の初期化
			s_aPause[s_PauseMenu].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

			//状態(点滅具合)の初期化
			s_aPause[s_PauseMenu].state = PAUSESTATE_IN;

			pVtx += (s_PauseMenu * 4);		//該当の位置まで進める

			//頂点カラーの設定処理
			Setcol(pVtx, s_aPause[s_PauseMenu].col.r, s_aPause[s_PauseMenu].col.g,
						 s_aPause[s_PauseMenu].col.b, s_aPause[s_PauseMenu].col.a);
		}

		if (GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true)
		{//Wキーが押されたかどうか
			nPauseMenu = ((nPauseMenu - 1) + 3) % 3;

			s_PauseMenu = (PAUSEMENU)nPauseMenu;

			s_aPause[s_PauseMenu].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true)
		{//Sキーが押されたかどうか
			nPauseMenu = ((nPauseMenu + 1) + 3) % 3;

			s_PauseMenu = (PAUSEMENU)nPauseMenu;

			s_aPause[s_PauseMenu].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_B) == true)
		{//決定キー(ENTERキー)が押されたかどうか
			if (s_PauseMenu == PAUSEMENU_GAME)
			{//ゲームのとき
				//ポーズの有効無効設定
				SetEnablePause(false);
			}
			else if (s_PauseMenu == PAUSEMENU_RETRY)
			{//リトライのとき
				//フェードの設定
				SetFade(MODE_GAME);
			}
			else if (s_PauseMenu == PAUSEMENU_TITLE)
			{//タイトルのとき
				//フェードの設定
				SetFade(MODE_TITLE);
			}

			s_fPausecolor = 1.0f;
		}

		//頂点バッファをアンロックする
		s_pVtxBuffMenu->Unlock();
	}

	if (s_aPause[s_PauseMenu].state == PAUSESTATE_IN)
	{//フェードイン状態
		s_aPause[s_PauseMenu].col.a -= s_fPausecolor;		//ポリゴンを透明にしていく

		if (s_aPause[s_PauseMenu].col.a <= 0.25f)
		{
			s_aPause[s_PauseMenu].col.a = 0.25f;
			s_aPause[s_PauseMenu].state = PAUSESTATE_OUT;		//フェードアウト状態に
		}
	}
	else if (s_aPause[s_PauseMenu].state == PAUSESTATE_OUT)
	{//フェードアウト状態
		s_aPause[s_PauseMenu].col.a += s_fPausecolor;		//ポリゴンを不透明にしていく

		if (s_aPause[s_PauseMenu].col.a >= 1.0f)
		{
			s_aPause[s_PauseMenu].col.a = 1.0f;
			s_aPause[s_PauseMenu].state = PAUSESTATE_IN;		//フェードイン状態に
		}
	}

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_PauseMenu * 4);		//該当の位置まで進める

	//頂点カラーの設定処理
	Setcol(pVtx, s_aPause[s_PauseMenu].col.r, s_aPause[s_PauseMenu].col.g, 
				 s_aPause[s_PauseMenu].col.b, s_aPause[s_PauseMenu].col.a);

	//頂点バッファをアンロックする
	s_pVtxBuffMenu->Unlock();
}

//--------------------------------------------------
//ポーズの描画処理
//--------------------------------------------------
void DrawPause(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < NUM_PAUSE; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			i * 4,						//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PAUSEMENU_MAX; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTextureMenu[i]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			i * 4,						//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}