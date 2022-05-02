//--------------------------------------------------
//
// ACG制作 ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "effect.h"
#include "game.h"
#include "gauge.h"
#include "input.h"
#include "item.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "sound.h"

#include <assert.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define PLAYER_WIDTH		(80.0f)			//プレイヤーの幅
#define PLAYER_HEIGHT		(70.0f)			//プレイヤーの高さ
#define MAX_MOVE			(0.3f)			//移動量の最大値
#define MIN_MOVE			(0.1f)			//移動量の最小値
#define EFFECT_MOVE			(2.0f)			//エフェクトの出る移動量
#define MAX_TEX				(3)				//テクスチャの最大数
#define MAX_U_PATTERN		(2)				//Uパターンの最大数
#define MAX_JUMP			(-27.5f)		//ジャンプ量
#define MAX_GRAVITY			(1.5f)			//重力の最大値
#define MAX_INERTIA			(0.05f)			//慣性の最大値
#define MAX_INTERVAL		(7)				//カウンターのインターバル

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(void);
static void UpdateState(VERTEX_2D *pVtx);
static void UpdateMove(VERTEX_2D *pVtx);
static void UpdateAttack(void);
static bool UpdateUpDown(void);
static void UpdateOffScreen(void);
static void UpdateBound(void);
static void UpdateMotion(void);
static void UpdateEffect(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//頂点バッファのポインタ
static Player						s_Player;					//プレイヤーの情報

//--------------------------------------------------
//プレイヤーの初期化処理
//--------------------------------------------------
void InitPlayer(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//メモリのクリア
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player002.png",
		&s_pTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player004.png",
		&s_pTexture[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player006.png",
		&s_pTexture[2]);

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

	//メモリのクリア
	memset(&s_Player, NULL, sizeof(Player));

	//構造体の初期化処理
	InitStruct();

	//頂点座標の設定処理
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//rhwの初期化処理
	Initrhw(pVtx);

	//頂点カラーの初期化処理
	Initcol(pVtx);

	float fTex = (float)s_Player.bDirection / MAX_U_PATTERN;

	//テクスチャ座標の設定処理
	Settex(pVtx, 0.0f + fTex, (1.0f / MAX_U_PATTERN) + fTex, 0.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//ゲージの設定処理
	SetGauge(GAUGEUSE_PLAYER, s_Player.nLife);
}

//--------------------------------------------------
//プレイヤーの終了処理
//--------------------------------------------------
void UninitPlayer(void)
{
	//サウンドの停止
	StopSound();

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
//プレイヤーの更新処理
//--------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//状態処理
	UpdateState(pVtx);

	//移動処理
	UpdateMove(pVtx);

	//モーション処理
	UpdateMotion();

	//頂点座標の設定処理
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//エフェクト処理
	UpdateEffect();
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

	switch (s_Player.state)
	{
	case PLAYERSTATE_NORMAL:		//通常状態

		//ポリゴンとテクスチャのαをまぜる
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		break;

	case PLAYERSTATE_DAMAGE:		//ダメージ状態
	case PLAYERSTATE_STAR:			//無敵状態

		//ポリゴンとテクスチャのαを足す
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

		break;

	default:
		assert(false);
		break;
	}
	
	switch (s_Player.attack)
	{
	case ATTACKSTATE_NONE:			//何もしていない状態
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[0]);

		break;

	case ATTACKSTATE_IN:			//吸い込んでる状態
	case ATTACKSTATE_OUT:			//吐き出す状態
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[1]);

		break;

	case ATTACKSTATE_STORE:			//蓄えている状態
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[2]);

		break;

	default:
		assert(false);
		break;
	}

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//描画する最初の頂点インデックス
		2);							//プリミティブ(ポリゴン)数

	//ポリゴンとテクスチャのαをまぜる
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
}

//--------------------------------------------------
//プレイヤーの取得処理
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_Player;
}

//-------------------------
//プレイヤーのヒット処理
//-------------------------
void HitPlayer(int nDamage)
{
	if (s_Player.state == PLAYERSTATE_NORMAL)
	{
		s_Player.nLife -= nDamage;

		//ゲージの減算処理
		SubGauge(GAUGEUSE_PLAYER, s_Player.nLife);

		if (s_Player.nLife <= 0)
		{//プレイヤーの体力がなくなった
			//リザルトの設定処理
			SetResult(RESULT_LOSE);

			//ゲームの設定処理
			SetGameState(GAMESTATE_END);
		}
		else
		{//まだ生きてる
			s_Player.state = PLAYERSTATE_DAMAGE;

			s_Player.nCounterState = 0;

			VERTEX_2D *pVtx;		//頂点情報へのポインタ

			//頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラーの設定処理
			Setcol(pVtx, 1.0f, 0.25f, 0.0f, 1.0f);

			//頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
		}
	}
}

//--------------------------------------------------
//構造体の初期化処理
//--------------------------------------------------
static void InitStruct(void)
{
	//現在の位置を初期化
	s_Player.pos.x = SCREEN_WIDTH * 0.5f;
	s_Player.pos.y = (SCREEN_HEIGHT / MAX_Y_BLOCK) * MIDDLE_BLOCK;
	s_Player.pos.z = 0.0f;

	s_Player.fWidth = PLAYER_WIDTH * 0.5f;					//幅の初期化
	s_Player.fHeight = PLAYER_HEIGHT;						//高さの初期化
	s_Player.nLife = 100;									//寿命の初期化
	s_Player.bDirection = true;								//右向き

	//他のはmemsetで0にした。
}

//--------------------------------------------------
//状態処理
//--------------------------------------------------
static void UpdateState(VERTEX_2D *pVtx)
{
	switch (s_Player.state)
	{
	case PLAYERSTATE_NORMAL:		//通常状態

		break;

	case PLAYERSTATE_DAMAGE:		//ダメージ状態

		s_Player.nCounterState++;

		if (s_Player.nCounterState >= 15)
		{
			//頂点カラーの初期化処理
			Initcol(pVtx);

			s_Player.state = PLAYERSTATE_STAR;
			s_Player.nCounterState = 0;
		}

		break;

	case PLAYERSTATE_STAR:			//無敵状態

		s_Player.nCounterState++;

		s_Player.fCol = 0.3f + sinf((s_Player.nCounterState * 0.05f) * (D3DX_PI * 2.0f)) * 0.2f ;

		//頂点カラーの設定処理
		Setcol(pVtx, s_Player.fCol, s_Player.fCol, s_Player.fCol, 1.0f);

		if (s_Player.nCounterState >= 100)
		{
			//頂点カラーの初期化処理
			Initcol(pVtx);

			s_Player.state = PLAYERSTATE_NORMAL;
			s_Player.nCounterState = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//移動処理
//--------------------------------------------------
static void UpdateMove(VERTEX_2D *pVtx)
{
	//攻撃処理
	UpdateAttack();

	if (s_Player.attack != ATTACKSTATE_IN && s_Player.attack != ATTACKSTATE_OUT)
	{//吸い込んでない
		//キー入力での移動
		if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT))
		{//Aキーが押された
			switch (s_Player.attack)
			{//移動量を更新 (増加させる)
			case ATTACKSTATE_NONE:			//何もしていない状態
				s_Player.move.x += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;
				s_Player.move.y += cosf(-D3DX_PI * 0.5f) * MAX_MOVE;

				break;

			case ATTACKSTATE_STORE:			//蓄えている状態
				s_Player.move.x += sinf(-D3DX_PI * 0.5f) * MIN_MOVE;
				s_Player.move.y += cosf(-D3DX_PI * 0.5f) * MIN_MOVE;
				
				break;

			case ATTACKSTATE_IN:			//吸い込んでる状態
			case ATTACKSTATE_OUT:			//吐き出す状態

				//上のifでここは通らないからbreakなしにしてassertしまーす。

			default:
				assert(false);
				break;
			}

			s_Player.bDirection = false;		//左向き

		}
		else if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT))
		{//Dキーが押された
			switch (s_Player.attack)
			{//移動量を更新 (増加させる)
			case ATTACKSTATE_NONE:			//何もしていない状態
				s_Player.move.x += sinf(D3DX_PI * 0.5f) * MAX_MOVE;
				s_Player.move.y += cosf(D3DX_PI * 0.5f) * MAX_MOVE;

				break;

			case ATTACKSTATE_STORE:			//蓄えている状態
				s_Player.move.x += sinf(D3DX_PI * 0.5f) * MIN_MOVE;
				s_Player.move.y += cosf(D3DX_PI * 0.5f) * MIN_MOVE;

				break;

			case ATTACKSTATE_IN:			//吸い込んでる状態
			case ATTACKSTATE_OUT:			//吐き出す状態

				//上のifでここは通らないからbreakなしにしてassertしまーす。

			default:
				assert(false);
				break;
			}
			
			s_Player.bDirection = true;		//右向き
		}

		float fTex = (float)s_Player.bDirection / MAX_U_PATTERN;

		//テクスチャ座標の設定処理
		Settex(pVtx, 0.0f + fTex, (1.0f / MAX_U_PATTERN) + fTex, 0.0f, 1.0f);

		//ジャンプ、降りる処理
		bool bDown = UpdateUpDown();

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
			//バウンド処理
			UpdateBound();
		}
		else
		{//空中
			if (s_Player.jump == JUMPSTATE_NONE)
			{//何もしていない
				s_Player.jump = JUMPSTATE_JUMP;
				s_Player.fHeight = PLAYER_HEIGHT;
				s_Player.nCounterMotion = 0;
			}
		}

		if (bDown)
		{//降りる
			//ブロックの上端の当たり判定
			CollisionTopBlock(&s_Player.pos, s_Player.fWidth, s_Player.fHeight);
		}

		//慣性・移動量を更新 (減衰させる)
		s_Player.move.x += (0.0f - s_Player.move.x) * MAX_INERTIA;
	}

	//画面外処理
	UpdateOffScreen();
}

//--------------------------------------------------
//攻撃処理
//--------------------------------------------------
static void UpdateAttack(void)
{
	switch (s_Player.attack)
	{
	case ATTACKSTATE_NONE:			//何もしていない状態
		if (s_Player.jump == JUMPSTATE_NONE)
		{//何もしてない
			if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
			{//ENTERキーが押された
				s_Player.attack = ATTACKSTATE_IN;
				s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}

		break;

	case ATTACKSTATE_IN:			//吸い込んでる状態
		if (GetKeyboardPress(DIK_RETURN) || GetJoypadPress(JOYKEY_B))
		{//ENTERキーが押された
			//アイテムの吸い込み処理
			InhaleItem(s_Player.pos, &s_Player.attack, s_Player.fWidth, s_Player.fHeight, s_Player.bDirection);

			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_PUNCH);

			s_Player.nCounterAttack = 0;
		}
		else if (s_Player.nCounterAttack <= MAX_INTERVAL)
		{//余韻中
			//アイテムの吸い込み処理
			InhaleItem(s_Player.pos, &s_Player.attack, s_Player.fWidth, s_Player.fHeight, s_Player.bDirection);
		}
		else
		{//吸い込めてない
			s_Player.attack = ATTACKSTATE_NONE;
			s_Player.nCounterAttack = 0;
		}

		s_Player.nCounterAttack++;

		break;

	case ATTACKSTATE_STORE:			//蓄えている状態
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
		{//ENTERキーが押された
			//アイテムの設定処理
			SetItem(s_Player.pos - D3DXVECTOR3(0.0f, s_Player.fHeight * 0.5f, 0.0f), ITEMTYPE_STAR, s_Player.bDirection);
			s_Player.attack = ATTACKSTATE_OUT;
		}

		break;

	case ATTACKSTATE_OUT:			//吐き出す状態
		s_Player.nCounterAttack++;

		if (s_Player.nCounterAttack >= MAX_INTERVAL)
		{
			s_Player.attack = ATTACKSTATE_NONE;
			s_Player.nCounterAttack = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//ジャンプ、降りる処理
//--------------------------------------------------
static bool UpdateUpDown(void)
{
	bool bDown = false;

	if (s_Player.attack == ATTACKSTATE_NONE && s_Player.jump == JUMPSTATE_NONE)
	{//何もしていない状態に
		if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN))
		{//Sキーが押された
			bDown = true;
		}
		else if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_W) ||
			GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_UP))
		{//スペースキー、Wキーが押された
			s_Player.move.y += MAX_JUMP;
			s_Player.jump = JUMPSTATE_JUMP;
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.nCounterMotion = 0;

			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_JUMP);
		}
	}

	return bDown;
}

//--------------------------------------------------
//画面外処理
//--------------------------------------------------
static void UpdateOffScreen(void)
{
	//画面端処理
	if (s_Player.pos.y >= SCREEN_HEIGHT)
	{//下端
		s_Player.pos.y = SCREEN_HEIGHT;

		//バウンド処理
		UpdateBound();
	}
	else if (s_Player.pos.y <= PLAYER_HEIGHT)
	{//上端
		s_Player.pos.y = PLAYER_HEIGHT;
		s_Player.move.y = 0.0f;
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
//バウンド処理
//--------------------------------------------------
static void UpdateBound(void)
{
	switch (s_Player.jump)
	{
	case JUMPSTATE_NONE:		//何もしていない
		s_Player.move.y = 0.0f;
		break;

	case JUMPSTATE_JUMP:		//ジャンプ
		s_Player.jump = JUMPSTATE_LAND;

		break;

	case JUMPSTATE_LAND:		//着地

		if (s_Player.fWidth <= PLAYER_WIDTH * 0.5f)
		{
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.fWidth = PLAYER_WIDTH * 0.5f;
			s_Player.jump = JUMPSTATE_NONE;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//モーション処理
//--------------------------------------------------
static void UpdateMotion(void)
{
	if (s_Player.attack == ATTACKSTATE_IN)
	{//吸い込んでる
		s_Player.fHeight = PLAYER_HEIGHT * 1.5f;
	}
	else if (s_Player.attack == ATTACKSTATE_STORE)
	{//吸い込んだ
		s_Player.nCounterMotion++;
		s_Player.fHeight = (PLAYER_HEIGHT * 1.5f) + (sinf((s_Player.nCounterMotion * 0.01f) * (D3DX_PI * 2.0f)) * 5.0f);
	}
	else
	{//吸い込んでいない
		Block *pBlock = GetBlock();		//ブロックの情報を授かる
		float fPosY = 0.0f;
		float fDif = 0.0f;
		float fDifOld = SCREEN_HEIGHT;

		switch (s_Player.jump)
		{
		case JUMPSTATE_NONE:		//何もしていない
			s_Player.nCounterMotion++;
			s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterMotion * 0.01f) * (D3DX_PI * 2.0f)) * 5.0f);
			break;

		case JUMPSTATE_JUMP:		//ジャンプ

			for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
			{
				if (!pBlock->bUse)
				{//ブロックが使用されていない
					continue;
				}

				//ブロックが使用されている

				if (s_Player.pos.y < (pBlock->pos.y - pBlock->fHeight) &&
					(s_Player.pos.x + s_Player.fWidth) >(pBlock->pos.x - pBlock->fWidth) &&
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

			if (s_Player.pos.y <= (fPosY + ((SCREEN_HEIGHT / MAX_Y_BLOCK) * 2.0f)) && s_Player.pos.y >= (fPosY - ((SCREEN_HEIGHT / MAX_Y_BLOCK) * 2.0f)))
			{//下のブロックから１ブロック上の範囲、落下中
				s_Player.nCounterMotion++;
			}

			s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterMotion * 0.04f) * (D3DX_PI * 2.0f)) * 15.0f);
			s_Player.fWidth = PLAYER_WIDTH * 0.5f + (sinf((s_Player.nCounterMotion * 0.02f) * (D3DX_PI * 2.0f)) * 2.0f);

			break;

		case JUMPSTATE_LAND:		//着地
			s_Player.nCounterMotion++;
			s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterMotion * 0.04f) * (D3DX_PI * 2.0f)) * 15.0f);
			s_Player.fWidth = PLAYER_WIDTH * 0.5f + (sinf((s_Player.nCounterMotion * 0.02f) * (D3DX_PI * 2.0f)) * 5.0f);

			break;

		default:
			assert(false);
			break;
		}
	}
}

//--------------------------------------------------
//エフェクト処理
//--------------------------------------------------
static void UpdateEffect(void)
{
	if (s_Player.move.x >= EFFECT_MOVE || s_Player.move.x <= -EFFECT_MOVE)
	{//移動中
		if (s_Player.nCounterMotion % 2 == 0)
		{
			//エフェクトの設定処理
			SetEffect(s_Player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f),
				s_Player.fWidth, s_Player.fHeight * 0.2f, 10, EFFECTTYPE_MOVE);
		}
	}

	if (s_Player.attack == ATTACKSTATE_IN)
	{//吸い込んでる
		SetParticle(s_Player.pos + D3DXVECTOR3(0.0f, -(s_Player.fHeight * 0.325f), 0.0f), EFFECTTYPE_IN, s_Player.bDirection);
	}
}