//--------------------------------------------------
//
// STG制作 ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"
#include "player.h"

//-------------------------
//グローバル変数
//-------------------------
LPDIRECT3DTEXTURE9			g_pTexturePlayer = NULL;		//プレイヤーのテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPlayer = NULL;		//プレイヤーの頂点バッファのポインタ
Player						g_Player;						//プレイヤーの情報

//-------------------------
//プレイヤーの初期化処理
//-------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\リムル 009.png",
		&g_pTexturePlayer);

	g_Player.pos = D3DXVECTOR3(PLAYER_WIDTH * 0.5f, SCREEN_HEIGHT, 0.0f);		//位置を初期化
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//移動量を初期化
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//向きを初期化
	g_Player.nLife = 100;														//体力の初期化
	g_Player.nStock = 2;														//残機の初期化
	g_Player.bDisp = true;														//表示している状態にする

	//対角線の長さを算出する
	g_Player.fLength = sqrtf((PLAYER_WIDTH * 0.5f) * (PLAYER_WIDTH * 0.5f) + PLAYER_HEIGHT * PLAYER_HEIGHT);

	//対角線の角度を算出する
	g_Player.fAngle = atan2f((PLAYER_WIDTH * 0.5f), PLAYER_HEIGHT);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) - PLAYER_WIDTH;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) + PLAYER_WIDTH;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (g_Player.fAngle * -1.0f)) - PLAYER_WIDTH;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (g_Player.fAngle * -1.0f));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) + PLAYER_WIDTH;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + g_Player.fAngle);
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 0.5f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 0.5f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-------------------------
//プレイヤーの終了処理
//-------------------------
void UninitPlayer(void)
{
	if (g_pTexturePlayer != NULL)
	{//プレイヤーのテクスチャの破棄
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{//プレイヤーの頂点バッファの破棄
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//-------------------------
//プレイヤーの更新処理
//-------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//移動処理
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//Aキーが押された
		//移動量を更新 (増加させる)
		g_Player.move.x += sinf(-D3DX_PI * 0.5f) * 0.6f;
		g_Player.move.y += cosf(-D3DX_PI * 0.5f) * 0.6f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//Dキーが押された
		//移動量を更新 (増加させる)
		g_Player.move.x += sinf(D3DX_PI * 0.5f) * 0.6f;
		g_Player.move.y += cosf(D3DX_PI * 0.5f) * 0.6f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
	}

	if (GetKeyboardPress(DIK_LEFT) == true)
	{//←キーが押された
		g_Player.rot.z += D3DX_PI * 0.01f;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{//→キーが押された
		g_Player.rot.z += -D3DX_PI * 0.01f;
	}

	//位置を更新
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	g_Player.move.x += GetJoypadStick(JOYKEY_L_STICK).x;

	//移動量を更新 (減衰させる)
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.075f;
	g_Player.move.y += (0.0f - g_Player.move.y) * 0.075f;

	//画面端処理
	if (g_Player.pos.y >= SCREEN_HEIGHT)
	{//下端
		g_Player.pos.y = SCREEN_HEIGHT;
	}
	else if (g_Player.pos.y <= PLAYER_HEIGHT)
	{//上端
		g_Player.pos.y = PLAYER_HEIGHT;
	}

	if (g_Player.pos.x >= SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f))
	{//右端
		g_Player.pos.x = -(PLAYER_WIDTH * 0.5f);
	}
	else if (g_Player.pos.x <= -(PLAYER_WIDTH * 0.5f))
	{//左端
		g_Player.pos.x = SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f);
	}

	//頂点座標の更新
	pVtx[0].pos.x = g_Player.pos.x + sinf((-D3DX_PI + g_Player.fAngle) + g_Player.rot.z) - (PLAYER_WIDTH * 0.5f);
	pVtx[0].pos.y = g_Player.pos.y + cosf((-D3DX_PI + g_Player.fAngle) + g_Player.rot.z) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf((D3DX_PI - g_Player.fAngle) + g_Player.rot.z) + (PLAYER_WIDTH * 0.5f);
	pVtx[1].pos.y = g_Player.pos.y + cosf((D3DX_PI - g_Player.fAngle) + g_Player.rot.z) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf((-D3DX_PI * 0.5f) + g_Player.rot.z) - (PLAYER_WIDTH * 0.5f);
	pVtx[2].pos.y = g_Player.pos.y + cosf((-D3DX_PI * 0.5f) + g_Player.rot.z);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf((D3DX_PI * 0.5f) + g_Player.rot.z) + (PLAYER_WIDTH * 0.5f);
	pVtx[3].pos.y = g_Player.pos.y + cosf((D3DX_PI * 0.5f) + g_Player.rot.z);
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//-------------------------
//プレイヤーの描画処理
//-------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_Player.bDisp == true)
	{//表示されている
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,							//描画する最初の頂点インデックス
			2);							//プリミティブ(ポリゴン)数
	}
}