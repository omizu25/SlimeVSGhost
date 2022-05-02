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
#include "setup.h"
#include "sound.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_PAUSE			(5)			//ポーズの最大数
#define NUM_PAUSE			(2)			//ポーズの種類
#define PAUSE_WIDTH			(200)		//ポーズの幅の半分
#define PAUSE_HEIGHT		(200)		//ポーズの高さの半分
#define MENU_WIDTH			(215)		//ポーズの選択肢の幅
#define MENU_HEIGHT			(70)		//ポーズの選択肢の高さ

//--------------------------------------------------
//ポーズのモードを定義
//--------------------------------------------------
typedef enum
{
	MENU_GAME = 0,		//ゲームに戻る
	MENU_RETRY,			//ゲームをやり直す
	MENU_TITLE,			//タイトル画面に戻る
	MENU_MAX
}MENU;

//--------------------------------------------------
//ポーズの選択肢の状態(点滅具合)を定義
//--------------------------------------------------
typedef enum
{
	PAUSESTATE_IN = 0,		//見える
	PAUSESTATE_OUT,			//見えない
	PAUSESTATE_MAX
}PAUSESTATE;

//--------------------------------------------------
//ポーズの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXCOLOR		col;		//色
	PAUSESTATE		state;		//状態状態(点滅具合)
}Pause;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx);
static void UpdateMenu(void);
static void UpdateState(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//頂点バッファのポインタ
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		//メニューのテクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMenu = NULL;			//メニューの頂点バッファのポインタ
static Pause						s_aPause[MENU_MAX];				//ポーズメニューの情報
static D3DXVECTOR3					s_pos;							//ポーズの位置
static D3DXCOLOR					s_aCol[NUM_PAUSE];				//ポーズの色
static MENU							s_Menu;							//ポーズメニュー
static float						s_fPausecolor;					//ポーズの色のαの数値

//--------------------------------------------------
//ポーズの初期化処理
//--------------------------------------------------
void InitPause(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//メモリのクリア
	memset(&s_pTextureMenu[0], NULL, sizeof(s_pTextureMenu));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause002.png",
		&s_pTextureMenu[MENU_GAME]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause006.png",
		&s_pTextureMenu[MENU_RETRY]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause001.png",
		&s_pTextureMenu[MENU_TITLE]);

	s_pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		//位置の初期化

	//色の初期化
	s_aCol[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	s_aCol[1] = D3DXCOLOR(0.0f, 0.85f, 1.0f, 0.95f);

	for (int i = 0; i < MENU_MAX; i++)
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
	
	s_Menu = MENU_GAME;				//ポーズメニューの初期化
	s_fPausecolor = 0.025f;			//ポーズの色のαの数値の初期化

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
		sizeof(VERTEX_2D) * 4 * MENU_MAX,
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
			SetMiddlepos(pVtx, s_pos, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		}
		else if (i == 1)
		{
			//頂点座標の設定処理
			SetMiddlepos(pVtx, s_pos, PAUSE_WIDTH, PAUSE_HEIGHT);
		}
		
		//rhwの初期化処理
		Initrhw(pVtx);

		//頂点カラーの設定処理
		Setcol(pVtx, s_aCol[i].r, s_aCol[i].g, s_aCol[i].b, s_aCol[i].a);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MENU_MAX; i++)
	{
		//頂点座標の設定処理
		SetMiddlepos(pVtx, s_aPause[i].pos, MENU_WIDTH, MENU_HEIGHT);

		//rhwの初期化処理
		Initrhw(pVtx);

		//頂点カラーの設定処理
		Setcol(pVtx, s_aPause[i].col.r, s_aPause[i].col.g, s_aPause[i].col.b, s_aPause[i].col.a);

		//テクスチャの初期化処理
		Inittex(pVtx);

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
	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{//メニューのテクスチャの破棄
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
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
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	if (GetFade() == FADE_NONE)
	{//何もしていない状態なら
		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		//メニューの入力時処理
		UpdateInput(pVtx);

		//メニューの選択処理
		UpdateMenu();

		//頂点バッファをアンロックする
		s_pVtxBuffMenu->Unlock();
	}

	//状態の更新処理
	UpdateState();

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_Menu * 4);		//該当の位置まで進める

	//頂点カラーの設定処理
	Setcol(pVtx, s_aPause[s_Menu].col.r, s_aPause[s_Menu].col.g, 
				 s_aPause[s_Menu].col.b, s_aPause[s_Menu].col.a);

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

	for (int i = 0; i < MENU_MAX; i++)
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

//--------------------------------------------------
//メニューの入力時処理
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx)
{
	int nPauseMenu = (int)s_Menu;		//ポーズメニューをintに

	if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S) ||
		GetJoypadTrigger(JOYKEY_UP) || GetJoypadTrigger(JOYKEY_DOWN))
	{//Wキー、Sキーが押されたかどうか
		//色の初期化
		s_aPause[s_Menu].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

		//状態(点滅具合)の初期化
		s_aPause[s_Menu].state = PAUSESTATE_IN;

		pVtx += (s_Menu * 4);		//該当の位置まで進める

		//頂点カラーの設定処理
		Setcol(pVtx, s_aPause[s_Menu].col.r, s_aPause[s_Menu].col.g,
			s_aPause[s_Menu].col.b, s_aPause[s_Menu].col.a);

		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_SYSTEM40);
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{//Wキーが押されたかどうか
		nPauseMenu = ((nPauseMenu - 1) + 3) % 3;

		s_Menu = (MENU)nPauseMenu;

		s_aPause[s_Menu].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{//Sキーが押されたかどうか
		nPauseMenu = ((nPauseMenu + 1) + 3) % 3;

		s_Menu = (MENU)nPauseMenu;

		s_aPause[s_Menu].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

//--------------------------------------------------
//メニューの選択処理
//--------------------------------------------------
static void UpdateMenu(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
	{//決定キー(ENTERキー)が押されたかどうか
		if (s_Menu == MENU_GAME)
		{//ゲームのとき
			//ポーズの有効無効設定
			SetEnablePause(false);
		}
		else if (s_Menu == MENU_RETRY)
		{//リトライのとき
			//フェードの設定
			SetFade(MODE_GAME);
		}
		else if (s_Menu == MENU_TITLE)
		{//タイトルのとき
			//フェードの設定
			SetFade(MODE_TITLE);
		}

		s_fPausecolor = 1.0f;

		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_SYSTEM49);
	}
}

//--------------------------------------------------
//状態の更新処理
//--------------------------------------------------
static void UpdateState(void)
{
	if (s_aPause[s_Menu].state == PAUSESTATE_IN)
	{//フェードイン状態
		s_aPause[s_Menu].col.a -= s_fPausecolor;		//ポリゴンを透明にしていく

		if (s_aPause[s_Menu].col.a <= 0.25f)
		{
			s_aPause[s_Menu].col.a = 0.25f;
			s_aPause[s_Menu].state = PAUSESTATE_OUT;		//フェードアウト状態に
		}
	}
	else if (s_aPause[s_Menu].state == PAUSESTATE_OUT)
	{//フェードアウト状態
		s_aPause[s_Menu].col.a += s_fPausecolor;		//ポリゴンを不透明にしていく

		if (s_aPause[s_Menu].col.a >= 1.0f)
		{
			s_aPause[s_Menu].col.a = 1.0f;
			s_aPause[s_Menu].state = PAUSESTATE_IN;		//フェードイン状態に
		}
	}
}