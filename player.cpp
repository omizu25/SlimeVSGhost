//--------------------------------------------------
//
// アクションゲーム制作 ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"
#include "player.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define PLAYER_WIDTH		(40.0f)			//プレイヤーの幅
#define PLAYER_HEIGHT		(100.0f)		//プレイヤーの高さ
#define PLAYER_MOVE			(0.3f)			//プレイヤーの移動量
#define MAX_U_PATTERN		(4)				//Uパターンの最大数
#define MAX_V_PATTERN		(2)				//Vパターンの最大数
#define MIN_MOVE			(0.30f)			//動いてる最小値
#define MAX_JUMP			(-30.0f)		//ジャンプ量
#define MAX_GRAVITY			(1.5f)			//重力の最大値
#define MAX_INERTIA			(0.05f)			//慣性の最大値
#define CNT_INTERVAL		(7)				//カウンターのインターバル

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//頂点バッファのポインタ
static Player						s_Player;				//プレイヤーの情報

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStructPlayer(void);
static void MovePlayer(VERTEX_2D *pVtx);
static void TexAnimPlayer(VERTEX_2D *pVtx);
static void OffScreenPlayer(void);

//--------------------------------------------------
//プレイヤーの初期化処理
//--------------------------------------------------
void InitPlayer(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player000.png",
		&s_pTexture);

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

	//プレイヤーの構造体の初期化処理
	InitStructPlayer();

	//頂点座標の設定処理
	SetBottompos(pVtx, s_Player.pos, PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT);

	//rhwの設定処理
	Setrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定処理
	Settex(pVtx, 0.0f, 1.0f / MAX_U_PATTERN, 0.0f, 1.0f / MAX_V_PATTERN);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//プレイヤーの終了処理
//--------------------------------------------------
void UninitPlayer(void)
{
	if (s_pTexture != NULL)
	{//プレイヤーのテクスチャの破棄
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{//プレイヤーの頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//プレイヤーの更新処理
//--------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーの移動処理
	MovePlayer(pVtx);

	//プレイヤーの画面外処理
	OffScreenPlayer();

	//頂点座標の設定処理
	SetBottompos(pVtx, s_Player.pos, PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//プレイヤーの描画処理
//--------------------------------------------------
void DrawPlayer(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数
}

//--------------------------------------------------
//プレイヤーの取得処理
//--------------------------------------------------
Player GetPlayer(void)
{
	return s_Player;
}

//--------------------------------------------------
//プレイヤーの構造体の初期化処理
//--------------------------------------------------
static void InitStructPlayer(void)
{
	//現在の位置を初期化
	s_Player.pos.x = PLAYER_WIDTH * 0.5f;
	s_Player.pos.y = SCREEN_HEIGHT;
	s_Player.pos.z = 0.0f;

	s_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置を初期化
	s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量を初期化
	s_Player.bjump = false;									//ジャンプしてない
	s_Player.nCounterAnim = 0;								//カウンターの初期化
	s_Player.nPatternAnim = 0;								//パターンの初期化
	s_Player.nDirectionMove = 0;							//向きの初期化
}

//--------------------------------------------------
//プレイヤーの移動処理
//--------------------------------------------------
static void MovePlayer(VERTEX_2D *pVtx)
{
	//キー入力での移動
	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT))
	{//Aキーが押された
		if (s_Player.bjump)
		{//ジャンプしてる
			s_Player.move.x += -PLAYER_MOVE * 0.75f;
		}
		else
		{//ジャンプしてない
			s_Player.move.x += -PLAYER_MOVE;
		}

		s_Player.nDirectionMove = 1;		//左向き
	}
	else if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT))
	{//Dキーが押された
		if (s_Player.bjump)
		{//ジャンプしてる
			s_Player.move.x += PLAYER_MOVE * 0.75f;
		}
		else
		{//ジャンプしてない
			s_Player.move.x += PLAYER_MOVE;
		}

		s_Player.nDirectionMove = 0;		//右向き
	}

	//ジャンプ処理
	if (s_Player.bjump == false &&
		GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_B))
	{//何もしてない、スペースキーが押された
		s_Player.move.y += MAX_JUMP;
		s_Player.bjump = true;
	}

	//重力
	s_Player.move.y += MAX_GRAVITY;

	//位置を更新
	s_Player.pos.x += s_Player.move.x;
	s_Player.pos.y += s_Player.move.y;

	//スティックでの移動量の更新
	s_Player.move.x += GetJoypadStick(JOYKEY_L_STICK).x;

	//プレイヤーのテクスチャアニメーション処理
	TexAnimPlayer(pVtx);

	//慣性・移動量を更新 (減衰させる)
	s_Player.move.x += (0.0f - s_Player.move.x) * MAX_INERTIA;
}

//--------------------------------------------------
//プレイヤーの画面外処理
//--------------------------------------------------
static void OffScreenPlayer(void)
{
	//画面端処理
	if (s_Player.pos.y >= SCREEN_HEIGHT)
	{//下端
		s_Player.pos.y = SCREEN_HEIGHT;
		s_Player.move.y = 0.0f;
		s_Player.bjump = false;
	}
	else if (s_Player.pos.y <= PLAYER_HEIGHT)
	{//上端
		s_Player.pos.y = PLAYER_HEIGHT;
		s_Player.move.y = 0.0f;
	}

	if (s_Player.pos.x >= SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f))
	{//右端
		s_Player.pos.x = -(PLAYER_WIDTH * 0.5f);
	}
	else if (s_Player.pos.x <= -(PLAYER_WIDTH * 0.5f))
	{//左端
		s_Player.pos.x = SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f);
	}
}

//--------------------------------------------------
//プレイヤーのテクスチャアニメーション処理
//--------------------------------------------------
static void TexAnimPlayer(VERTEX_2D *pVtx)
{
	if (s_Player.bjump)
	{//ジャンプしている
		s_Player.nCounterAnim = 0;		//カウンターの初期化

		//パターンNo.を更新する (指定のテクスチャ)
		if (s_Player.nPatternAnim % 2 == 0)
		{
			s_Player.nPatternAnim++;
		}

		float fPattren = (float)s_Player.nPatternAnim / MAX_U_PATTERN;
		float fDirection = (float)s_Player.nDirectionMove / MAX_V_PATTERN;

		//テクスチャ座標の設定処理
		Settex(pVtx, fPattren, fPattren + (1.0f / MAX_U_PATTERN), fDirection, fDirection + (1.0f / MAX_V_PATTERN));
	}
	else
	{//ジャンプしてない
		if (s_Player.move.x >= MIN_MOVE || s_Player.move.x <= -MIN_MOVE)
		{//移動中
			s_Player.nCounterAnim++;		//カウンターを加算

			if ((s_Player.nCounterAnim % CNT_INTERVAL) == 0)
			{//一定時間経過した
				//パターンNo.を更新する
				s_Player.nPatternAnim = (s_Player.nPatternAnim + 1) % MAX_U_PATTERN;

				float fPattren = (float)s_Player.nPatternAnim / MAX_U_PATTERN;
				float fDirection = (float)s_Player.nDirectionMove / MAX_V_PATTERN;

				//テクスチャ座標の設定処理
				Settex(pVtx, fPattren, fPattren + (1.0f / MAX_U_PATTERN), fDirection, fDirection + (1.0f / MAX_V_PATTERN));
			}
		}
		else if (s_Player.nPatternAnim == 0)
		{//ほぼ止まってる、指定のテクスチャ
			s_Player.nCounterAnim = 0;		//カウンターの初期化
		}
		else
		{//ほぼ止まってる、指定のテクスチャじゃない
			s_Player.nCounterAnim++;		//カウンターを加算

			if ((s_Player.nCounterAnim % CNT_INTERVAL) == 0)
			{//一定時間経過した
				//パターンNo.を更新する
				if (s_Player.nPatternAnim % 2 == 1)
				{
					s_Player.nPatternAnim++;
				}

				float fPattren = (float)s_Player.nPatternAnim / MAX_U_PATTERN;
				float fDirection = (float)s_Player.nDirectionMove / MAX_V_PATTERN;

				//テクスチャ座標の設定処理
				Settex(pVtx, fPattren, fPattren + (1.0f / MAX_U_PATTERN), fDirection, fDirection + (1.0f / MAX_V_PATTERN));
			}
		}
	}
}