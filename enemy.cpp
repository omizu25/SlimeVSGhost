//--------------------------------------------------
//
// ACG制作 ( enemy.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "enemy.h"
#include "game.h"
#include "gauge.h"
#include "item.h"
#include "player.h"
#include "ranking.h"
#include "result.h"
#include "setup.h"
#include "time.h"

#include <assert.h>
#include <time.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define MAX_U_PATTERN		(3)			//Uパターンの最大数
#define MAX_V_PATTERN		(2)			//Vパターンの最大数
#define TEX_INTERVAL		(7)			//カウンターのインターバル
#define STOP_TIME			(60)		//止まる時間

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
static void InitStruct(Enemy *pEnemy);
static void UpdateStop(Enemy *pEnemy);
static void UpdateCollision(Enemy *pEnemy);
static void UpdateOffScreen(Enemy *pEnemy);
static void UpdatePop(Enemy *pEnemy);
static void UpdateState(VERTEX_2D *pVtx, Enemy *pEnemy);
static void UpdateTexAnim(VERTEX_2D *pVtx, Enemy *pEnemy);
static void PopItem(Enemy *pEnemy);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[ENEMYTYPE_MAX];		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//頂点バッファのポインタ
static Enemy						s_aEnemy[ENEMYTYPE_MAX];		//敵の情報

//--------------------------------------------------
//敵の初期化処理
//--------------------------------------------------
void InitEnemy(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{//メモリのクリア
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\obake003.png",
		&s_pTexture[ENEMYTYPE_BOY]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\obake004.png",
		&s_pTexture[ENEMYTYPE_GIRL]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ENEMYTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//敵の情報の初期化設定
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];

		//構造体の初期化処理
		InitStruct(pEnemy);

		//全ての初期化処理
		InitAll(pVtx);

		pVtx += 4;		//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//世界の種子の初期化
	srand((unsigned)time(NULL));
}

//--------------------------------------------------
//敵の終了処理
//--------------------------------------------------
void UninitEnemy(void)
{
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
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
//敵の更新処理
//--------------------------------------------------
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];

		if (!pEnemy->bUse)
		{//敵が使用されていない
			continue;
		}

		//敵が使用されている

		//前回の位置の記憶
		pEnemy->posOld = pEnemy->pos;

		//位置の更新
		pEnemy->pos += pEnemy->move;

		//ストップ処理
		UpdateStop(pEnemy);

		//アイテムの当たり判定処理
		CollisionItem(&pEnemy->pos, &pEnemy->posOld, pEnemy->fWidth, pEnemy->fHeight);

		//画面外処理
		UpdateOffScreen(pEnemy);

		//当たり判定処理
		UpdateCollision(pEnemy);

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//状態処理
		UpdateState(pVtx, pEnemy);

		//頂点座標の設定処理
		SetBottompos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);

		//テクスチャアニメーション処理
		UpdateTexAnim(pVtx, pEnemy);

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//敵の描画処理
//--------------------------------------------------
void DrawEnemy(void)
{
	//デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		if (s_aEnemy[i].bUse)
		{//敵が使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexture[s_aEnemy[i].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				i * 4,						//描画する最初の頂点インデックス
				2);							//プリミティブ(ポリゴン)数
		}
	}
}

//--------------------------------------------------
//敵の設定処理
//--------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];

		if (pEnemy->bUse)
		{//敵が使用されている
			continue;
		}

		//敵が使用されていない

		pEnemy->pos = pos;
		pEnemy->type = type;
		pEnemy->posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->fWidth = ENEMY_WIDTH * 0.5f;
		pEnemy->fHeight = ENEMY_HEIGHT;
		pEnemy->nCounterState = 0;
		pEnemy->nCounterAnim = 0;
		pEnemy->nPatternAnim = 0;
		pEnemy->nLife = 100;
		pEnemy->bUse = true;		//使用している状態にする

		switch (type)
		{
		case ENEMYTYPE_BOY:			//男の子

			pEnemy->move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
			pEnemy->bDirection = true;		//右向き
			pEnemy->pop = ENEMYPOP_TOP;

			//ゲージの設定処理
			SetGauge(GAUGEUSE_BOY, pEnemy->nLife);

			break;

		case ENEMYTYPE_GIRL:		//女の子

			pEnemy->move = D3DXVECTOR3(-3.0f, 0.0f, 0.0f);
			pEnemy->bDirection = false;		//左向き
			pEnemy->pop = ENEMYPOP_BOTTOM;

			//ゲージの設定処理
			SetGauge(GAUGEUSE_GIRL, pEnemy->nLife);

			break;

		default:
			assert(false);
			break;
		}

		VERTEX_2D *pVtx;		//頂点情報へのポインタ

		//頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//該当の位置まで進める

		//頂点座標の設定処理
		SetBottompos(pVtx, pos, pEnemy->fWidth, pEnemy->fHeight);

		float fPattren = (float)pEnemy->nPatternAnim / MAX_U_PATTERN;
		float fDirection = (float)pEnemy->bDirection / MAX_V_PATTERN;

		//テクスチャ座標の設定処理
		Settex(pVtx, fPattren + 0.01f, (fPattren + (1.0f / MAX_U_PATTERN)) - 0.01f, fDirection + 0.01f, fDirection + (1.0f / MAX_V_PATTERN));

		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		//アイテムのポップ処理
		PopItem(pEnemy);

		break;		//ここでfor文を抜ける
	}
}

//--------------------------------------------------
//敵の取得処理
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return &s_aEnemy[0];		//敵の情報を授ける
}

//--------------------------------------------------
//敵のヒット処理
//--------------------------------------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	Enemy *pEnemy = &s_aEnemy[nCntEnemy];

	if (pEnemy->state == ENEMYSTATE_NORMAL)
	{
		pEnemy->nLife -= nDamage;

		switch (pEnemy->type)
		{
		case ENEMYTYPE_BOY:			//男の子

			//ゲージの減算処理
			SubGauge(GAUGEUSE_BOY, pEnemy->nLife);

			break;

		case ENEMYTYPE_GIRL:		//女の子

			//ゲージの減算処理
			SubGauge(GAUGEUSE_GIRL, pEnemy->nLife);

			break;

		default:
			assert(false);
			break;
		}

		if (pEnemy->nLife <= 0)
		{//敵の体力がなくなった
			pEnemy->bUse = false;		//使用していない状態にする

			int nUse = 0;

			for (int i = 0; i < ENEMYTYPE_MAX; i++)
			{
				if (s_aEnemy[i].bUse)
				{//敵が使用されている
					continue;
				}

				//敵が使用されていない
				nUse++;
			}

			if (nUse >= ENEMYTYPE_MAX)
			{//全員使用されていない
				//リザルトの設定処理
				SetResult(RESULT_WIN);

				//ゲームの設定処理
				SetGameState(GAMESTATE_END);

				//ランキングの設定処理
				SetRanking(GetTime());
			}
		}
		else
		{//まだ生きてる
			pEnemy->state = ENEMYSTATE_DAMAGE;

			pEnemy->nCounterState = 0;

			VERTEX_2D *pVtx;		//頂点情報へのポインタ

			//頂点情報をロックし、頂点情報へのポインタを取得
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntEnemy * 4);		//該当の位置まで進める

			//頂点カラーの設定処理
			Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

			//頂点バッファをアンロックする
			s_pVtxBuff->Unlock();
		}
	}
}

//--------------------------------------------------
//構造体の初期化処理
//--------------------------------------------------
static void InitStruct(Enemy *pEnemy)
{
	pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->type = ENEMYTYPE_BOY;
	pEnemy->state = ENEMYSTATE_NORMAL;
	pEnemy->pop = ENEMYPOP_TOP;
	pEnemy->fWidth = 0.0f;
	pEnemy->fHeight = 0.0f;
	pEnemy->nCounterState = 0;
	pEnemy->nCounterStop = 0;
	pEnemy->nCounterAnim = 0;
	pEnemy->nPatternAnim = 0;
	pEnemy->bDirection = false;
	pEnemy->nLife = 0;
	pEnemy->bUse = false;		//使用していない状態にする
}

//--------------------------------------------------
//ストップ処理
//--------------------------------------------------
static void UpdateStop(Enemy *pEnemy)
{
	if (pEnemy->nCounterStop <= STOP_TIME)
	{//まだ止まってない
		float fEnemyWidth = ENEMY_WIDTH * 0.5f;
		if (pEnemy->bDirection)
		{//右向き
			if (pEnemy->pos.x >= fEnemyWidth)
			{
				pEnemy->pos.x = fEnemyWidth;
				pEnemy->nCounterStop++;
			}
		}
		else
		{//左向き
			if (pEnemy->pos.x <= SCREEN_WIDTH - fEnemyWidth)
			{
				pEnemy->pos.x = SCREEN_WIDTH - fEnemyWidth;
				pEnemy->nCounterStop++;
			}
		}
	}
}

//--------------------------------------------------
//画面外処理
//--------------------------------------------------
static void UpdateOffScreen(Enemy *pEnemy)
{
	//画面端処理
	if (pEnemy->pos.y >= SCREEN_HEIGHT)
	{//下端
		pEnemy->pos.y = SCREEN_HEIGHT;
		pEnemy->move.y = 0.0f;
	}
	else if (pEnemy->pos.y <= ENEMY_HEIGHT)
	{//上端
		pEnemy->pos.y = ENEMY_HEIGHT;
		pEnemy->move.y = 0.0f;
	}

	if ((pEnemy->pos.x >= SCREEN_WIDTH + ENEMY_WIDTH) || (pEnemy->pos.x <= -ENEMY_WIDTH))
	{//右端か左端
		pEnemy->bDirection = !pEnemy->bDirection;
		pEnemy->move *= -1.0f;
		pEnemy->nCounterStop = 0;
		
		//ポップ場所の処理
		UpdatePop(pEnemy);
	}
}

//--------------------------------------------------
//ポップ場所の処理
//--------------------------------------------------
static void UpdatePop(Enemy *pEnemy)
{
	do
	{//ポップ場所をランダム

		pEnemy->pop = (ENEMYPOP)(rand() % (int)ENEMYPOP_MAX);

	} while (s_aEnemy[0].pop == s_aEnemy[1].pop);

	float fBlock = SCREEN_HEIGHT / MAX_Y_BLOCK;

	switch (pEnemy->pop)
	{
	case ENEMYPOP_TOP:			//上の段
		pEnemy->pos.y = fBlock * TOP_BLOCK;
		break;

	case ENEMYPOP_MIDDLE:		//真ん中の段
		pEnemy->pos.y = fBlock * MIDDLE_BLOCK;
		break;

	case ENEMYPOP_BOTTOM:		//下の段
		pEnemy->pos.y = fBlock * BOTTOM_BLOCK;
		break;

	default:
		assert(false);
		break;
	}

	//アイテムのポップ処理
	PopItem(pEnemy);
}

//--------------------------------------------------
//当たり判定処理
//--------------------------------------------------
static void UpdateCollision(Enemy *pEnemy)
{
	Player *pPlayer = GetPlayer();		//プレイヤーの取得

	if (pEnemy->pos.y <= (pPlayer->pos.y + pEnemy->fHeight) &&
		pEnemy->pos.y >= (pPlayer->pos.y - pPlayer->fHeight) &&
		pEnemy->pos.x <= (pPlayer->pos.x + pPlayer->fWidth + pEnemy->fWidth) &&
		pEnemy->pos.x >= (pPlayer->pos.x - pPlayer->fWidth - pEnemy->fWidth))
	{//プレイヤーに敵が当たった時

		//プレイヤーのヒット処理
		HitPlayer(20);
	}
}

//--------------------------------------------------
//状態処理
//--------------------------------------------------
static void UpdateState(VERTEX_2D *pVtx, Enemy *pEnemy)
{
	switch (pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:		//通常状態

		break;

	case ENEMYSTATE_DAMAGE:		//ダメージ状態

		pEnemy->nCounterState++;

		if (pEnemy->nCounterState >= 15)
		{
			//頂点カラーの初期化処理
			Initcol(pVtx);

			pEnemy->state = ENEMYSTATE_NORMAL;

			pEnemy->nCounterState = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//テクスチャアニメーション処理
//--------------------------------------------------
static void UpdateTexAnim(VERTEX_2D *pVtx, Enemy *pEnemy)
{
	pEnemy->nCounterAnim++;		//カウンターを加算

	if ((pEnemy->nCounterAnim % TEX_INTERVAL) == 0)
	{//一定時間経過した
		//パターンNo.を更新する
		pEnemy->nPatternAnim = (pEnemy->nPatternAnim + 1) % MAX_U_PATTERN;

		float fPattren = (float)pEnemy->nPatternAnim / MAX_U_PATTERN;
		float fDirection = (float)pEnemy->bDirection / MAX_V_PATTERN;

		//テクスチャ座標の設定処理
		Settex(pVtx, fPattren + 0.01f, (fPattren + (1.0f / MAX_U_PATTERN)) - 0.01f, fDirection + 0.01f, fDirection + (1.0f / MAX_V_PATTERN));
	}
}

//--------------------------------------------------
//アイテムのポップ処理
//--------------------------------------------------
static void PopItem(Enemy *pEnemy)
{
	float fItemSize = ITEM_SIZE * 0.5f;

	//アイテムの設定処理
	if (pEnemy->bDirection)
	{//右向き
		SetItem(pEnemy->pos + D3DXVECTOR3(pEnemy->fWidth + fItemSize, -fItemSize, 0.0f), ITEMTYPE_BLOCK, pEnemy->bDirection);
	}
	else
	{//左向き
		SetItem(pEnemy->pos + D3DXVECTOR3(-pEnemy->fWidth - fItemSize, -fItemSize, 0.0f), ITEMTYPE_BLOCK, pEnemy->bDirection);
	}
}