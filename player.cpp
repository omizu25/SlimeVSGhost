//--------------------------------------------------
//
// アクションゲーム制作 ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "input.h"
#include "player.h"

#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define PLAYER_WIDTH		(80.0f)			//プレイヤーの幅
#define PLAYER_HEIGHT		(70.0f)			//プレイヤーの高さ
#define PLAYER_MOVE			(0.3f)			//プレイヤーの移動量
#define MAX_U_PATTERN		(4)				//Uパターンの最大数
#define MAX_V_PATTERN		(2)				//Vパターンの最大数
#define MIN_MOVE			(0.30f)			//動いてる最小値
#define MAX_JUMP			(-27.5f)		//ジャンプ量
#define MAX_BOUND			(5.0f)			//バウンドの最大回数
#define MAX_GRAVITY			(1.5f)			//重力の最大値
#define MAX_INERTIA			(0.05f)			//慣性の最大値
#define CNT_INTERVAL		(7)				//カウンターのインターバル

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//頂点バッファのポインタ
static Player						s_Player;				//プレイヤーの情報
static bool							s_bTexUse;				//テクスチャの使用するかどうか

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStructPlayer(void);
static void MovePlayer(VERTEX_2D *pVtx);
static void OffScreenPlayer(void);
static void BoundPlayer(void);
static void MotionPlayer(void);

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
		"Data\\TEXTURE\\player001.png",
		&s_pTexture);

	s_bTexUse = true;

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
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//rhwの設定処理
	Setrhw(pVtx);

	//頂点カラーの設定処理
	Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定処理
	Settex(pVtx, 0.0f, 0.5f, 0.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//プレイヤーの終了処理
//--------------------------------------------------
void UninitPlayer(void)
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

	//プレイヤーのモーション処理
	MotionPlayer();

	//頂点座標の設定処理
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_F2))
	{//F2キー(F2キー)が押されたかどうか
		s_bTexUse = !s_bTexUse;
	}

#endif //  _DEBUG
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
	if (s_bTexUse)
	{//使用する
		pDevice->SetTexture(0, s_pTexture);
	}
	else
	{//使用しない
		pDevice->SetTexture(0, NULL);
	}

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数
}

//--------------------------------------------------
//プレイヤーの取得処理
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_Player;
}

//--------------------------------------------------
//テクスチャを使用するかの取得処理
//--------------------------------------------------
bool GetTexUsePlayer(void)
{
	return s_bTexUse;
}

//--------------------------------------------------
//プレイヤーの構造体の初期化処理
//--------------------------------------------------
static void InitStructPlayer(void)
{
	//現在の位置を初期化
	s_Player.pos.x = PLAYER_WIDTH * 0.5f;
	s_Player.pos.y = (SCREEN_HEIGHT / MAX_Y_BLOCK) * (MAX_Y_BLOCK - 1);
	s_Player.pos.z = 0.0f;

	s_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置を初期化
	s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量を初期化
	s_Player.jump = JUMPSTATE_NONE;							//何もしていない状態にする
	s_Player.fWidth = PLAYER_WIDTH * 0.5f;					//幅の初期化
	s_Player.fHeight = PLAYER_HEIGHT;						//高さの初期化
	s_Player.nCounterState = 0;								//カウンターの初期化
}

//--------------------------------------------------
//プレイヤーの移動処理
//--------------------------------------------------
static void MovePlayer(VERTEX_2D *pVtx)
{
	//キー入力での移動
	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT))
	{//Aキーが押された
		switch (s_Player.jump)
		{//移動量を更新 (増加させる)
		case JUMPSTATE_NONE:		//何もしていない

			s_Player.move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_MOVE;
			s_Player.move.y += cosf(-D3DX_PI * 0.5f) * PLAYER_MOVE;

			break;

		case JUMPSTATE_JUMP:		//ジャンプ
		case JUMPSTATE_BOUND:		//バウンド

			s_Player.move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;
			s_Player.move.y += cosf(-D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;

			break;

		default:
			assert(false);
			break;
		}

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.5f, 1.0f, 0.0f, 1.0f);
	}
	else if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT))
	{//Dキーが押された
		switch (s_Player.jump)
		{//移動量を更新 (増加させる)
		case JUMPSTATE_NONE:		//何もしていない

			s_Player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_MOVE;
			s_Player.move.y += cosf(D3DX_PI * 0.5f) * PLAYER_MOVE;

			break;

		case JUMPSTATE_JUMP:		//ジャンプ
		case JUMPSTATE_BOUND:		//バウンド

			s_Player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;
			s_Player.move.y += cosf(D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;

			break;

		default:
			assert(false);
			break;
		}

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f, 0.5f, 0.0f, 1.0f);
	}

	bool bDown = false;

	//ジャンプ処理
	if (s_Player.jump == JUMPSTATE_NONE)
	{//何もしてない
		if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN))
		{
			bDown = true;
		}
		else if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_W) || 
				 GetJoypadTrigger(JOYKEY_B))
		{//スペースキー、Wキーが押された
			s_Player.move.y += MAX_JUMP;
			s_Player.jump = JUMPSTATE_JUMP;
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.nCounterState = 0;
		}
	}

	//重力
	s_Player.move.y += MAX_GRAVITY;

	//スティックでの移動量の更新
	s_Player.move.x += GetJoypadStick(JOYKEY_L_STICK).x;

	//前回の位置の記憶
	s_Player.posOld = s_Player.pos;

	//位置を更新
	s_Player.pos.x += s_Player.move.x;
	s_Player.pos.y += s_Player.move.y;

	//ブロックの当たり判定処理
	if (CollisionBlock(&s_Player.pos, &s_Player.posOld, &s_Player.move, s_Player.fWidth, s_Player.fHeight))
	{//ブロックの上端にいる時
		//プレイヤーのバウンド処理
		BoundPlayer();
	}
	else
	{//空中
		if (s_Player.jump == JUMPSTATE_NONE)
		{//何もしていない
			s_Player.jump = JUMPSTATE_JUMP;
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.nCounterState = 0;
		}
	}

	if (bDown)
	{//降りるｄ
		//ブロックの上端の当たり判定
		CollisionTopBlock(&s_Player.pos, s_Player.fWidth, s_Player.fHeight);
	}

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

		//プレイヤーのバウンド処理
		BoundPlayer();
	}
	else if (s_Player.pos.y <= PLAYER_HEIGHT)
	{//上端
		s_Player.pos.y = PLAYER_HEIGHT;
		s_Player.move.y *= -REFLECT_BOUND;
	}

	if (s_Player.pos.x >= SCREEN_WIDTH - (PLAYER_WIDTH * 0.5f))
	{//右端
		s_Player.pos.x = SCREEN_WIDTH - (PLAYER_WIDTH * 0.5f);
	}
	else if (s_Player.pos.x <= PLAYER_WIDTH * 0.5f)
	{//左端
		s_Player.pos.x = PLAYER_WIDTH * 0.5f;
	}
}

//--------------------------------------------------
//プレイヤーのバウンド処理
//--------------------------------------------------
static void BoundPlayer(void)
{
	switch (s_Player.jump)
	{
	case JUMPSTATE_NONE:		//何もしていない
		s_Player.move.y = 0.0f;
		break;

	case JUMPSTATE_JUMP:		//ジャンプ
		s_Player.jump = JUMPSTATE_BOUND;

		//break無し

	case JUMPSTATE_BOUND:		//バウンド

		s_Player.move.y *= -REFLECT_BOUND;

		if (s_Player.move.y >= MAX_JUMP * powf(REFLECT_BOUND, MAX_BOUND))
		{//バウンドが終わった
			s_Player.jump = JUMPSTATE_NONE;
			s_Player.move.y = 0.0f;
			s_Player.nCounterState = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//プレイヤーのモーション処理
//--------------------------------------------------
static void MotionPlayer(void)
{
	Block *pBlock = GetBlock();		//ブロックの情報を授かる
	float fPosY = 0.0f;
	float fDif = 0.0f;
	float fDifOld = SCREEN_HEIGHT;

	switch (s_Player.jump)
	{
	case JUMPSTATE_NONE:		//何もしていない
		s_Player.nCounterState++;
		s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterState * 0.01f) * (D3DX_PI * 2.0f)) * 5.0f);
		break;

	case JUMPSTATE_JUMP:		//ジャンプ
	case JUMPSTATE_BOUND:		//バウンド

		for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
		{
			if (!pBlock->bUse)
			{//ブロックが使用されていない
				continue;
			}

			//ブロックが使用されている

			if (s_Player.pos.y < (pBlock->pos.y - pBlock->fHeight) &&
				(s_Player.pos.x + s_Player.fWidth) > (pBlock->pos.x - pBlock->fWidth) &&
				(s_Player.pos.x - s_Player.fWidth) < (pBlock->pos.x + pBlock->fWidth))
			{//プレイヤーの下側のブロックの時
				//差を計算
				fDif = (pBlock->pos.y - pBlock->fHeight) - s_Player.pos.y;

				if (fDif < fDifOld)
				{//差が小さかったら交換
					fDifOld = fDif;
					fPosY = pBlock->pos.y - pBlock->fHeight;
				}
			}
		}

		if (s_Player.pos.y <= fPosY && s_Player.pos.y >= (fPosY - (SCREEN_HEIGHT / MAX_Y_BLOCK)))
		{//下のブロックから１ブロック上の範囲なら
			s_Player.nCounterState++;
		}

		s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterState * 0.01f) * (D3DX_PI * 2.0f)) * 3.0f * -s_Player.move.y);

		break;

	default:
		assert(false);
		break;
	}
}