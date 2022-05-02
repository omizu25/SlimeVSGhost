//--------------------------------------------------
//
// アクションゲーム制作 ( main.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"
#include "main.h"
#include "map.h"
#include "player.h"

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define CLASS_NAME		"windowClass"		//ウインドウクラスの名前
#define WINDOW_NAME		"STG制作"			//ウインドウの名前 (キャプションに表示)

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);

//--------------------------------------------------
//スタティック変数
//--------------------------------------------------
static LPDIRECT3D9				s_pD3D = NULL;				//Direct3Dオブジェクトへのポインタ
static LPDIRECT3DDEVICE9		s_pD3DDevice = NULL;		//Direct3Dデバイスへのポインタ
static LPD3DXFONT				s_pFont = NULL;				//フォントへのポインタ
static int						s_nCountFPS = 0;			//FPSカウンタ
static bool						s_bDebug = true;			//デバッグ表示をするか [表示  : true 非表示  : false]
static bool						s_bPause = false;			//ポーズ中かどうか [してる  : true してない  : false]

//--------------------------------------------------
//main関数
//--------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{//ウインドウクラスの構造体
		sizeof(WNDCLASSEX),						//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								//ウインドウのスタイル
		WindowProc,								//ウインドウプロシージャ
		0,										//０にする (通常は使用しない)
		0,										//０にする (通常は使用しない)
		hInstance,								//インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				//クライアント領域の背景色
		NULL,									//メニューバー
		CLASS_NAME,								//ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)			//ファイルのアイコン
	};

	HWND hWnd;												//ウインドウハンドル (識別子)
	MSG msg;												//メッセージを格納する変数
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};		//画面サイズの構造体
	DWORD dwCorrentTime;									//現在時刻
	DWORD dwExecLastTime;									//最後に処理した時刻
	DWORD dwFrameCount;										//フレームカウント
	DWORD dwFPSLastTime;									//最後にFPSを計算した時刻

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを作成
	hWnd = CreateWindowEx(
		0,								//拡張ウインドウスタイル
		CLASS_NAME,						//ウインドウスタイルの名前
		WINDOW_NAME,					//ウインドウの名前
		WS_OVERLAPPEDWINDOW,			//ウインドウスタイル
		CW_USEDEFAULT,					//ウインドウの左上X座標
		CW_USEDEFAULT,					//　　〃　　の左上Y座標
		(rect.right - rect.left),		//　　〃　　の幅
		(rect.bottom - rect.top),		//　　〃　　の高さ
		NULL,							//親ウインドウのハンドル
		NULL,							//メニューハンドルまたは子ウインドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウインドウ作成データ

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCorrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得 (保存)
	dwFrameCount = 0;					//初期化する
	dwFPSLastTime = timeGetTime();		//現在時刻を取得 (保存)

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);			//ウインドウの表示状態を設定
	UpdateWindow(hWnd);					//クライアント領域を更新

	while (1)
	{//メッセージループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{//メッセージの設定
				TranslateMessage(&msg);		//仮想メッセージを文字のメッセージへ変換
				DispatchMessage(&msg);		//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCorrentTime = timeGetTime();		//現在時刻を取得

			if ((dwCorrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSの計算
				s_nCountFPS = (dwFrameCount * 1000) / (dwCorrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCorrentTime;		//FPSを計測した時刻を保存

				dwFrameCount = 0;					//フレームカウントをクリア
			}

			if ((dwCorrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCorrentTime;		//処理開始の時刻[現在時刻]を保存

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;						//フレームカウントを加算
			}
		}
	}
	
	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//--------------------------------------------------
//ウインドウプロシージャ
//--------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:		//ウインドウ破棄のメッセージ
		//WM_QOITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		//[ESC]キーが押された
			//ウインドウを破棄する (WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//規定の処理を返す
}

//--------------------------------------------------
//初期化処理
//--------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;				//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;		//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (s_pD3D == NULL)
	{//開かなかった時用の確認
		return E_FAIL;
	}

	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{//現在のディスプレイモードを取得
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								//パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェア)
	if (FAILED(s_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&s_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(s_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&s_pD3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
			if (FAILED(s_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&s_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//-------------------------
	//各種オブジェクトの初期化処理
	//-------------------------

	//レンダーステートの設定
	s_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	s_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	s_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	s_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		//画像を小さくしても綺麗にする
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		//画像を大きくしても綺麗にする
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		//テクスチャのUの繰り返し方を設定
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		//テクスチャのVの繰り返し方を設定

	//テクスチャステージステートの設定
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		//ポリゴンとテクスチャのαをまぜる
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		//１つ目の色はテクスチャの色
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		//２つ目の色は現在の色

	//デバッグ表示用フォントの生成
	D3DXCreateFont(
		s_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &s_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの初期化処理
	InitJoypad();

	//マップの初期化処理
	InitMap();

	//プレイヤーの初期化処理
	InitPlayer();
	
	return S_OK;
}

//--------------------------------------------------
//終了処理
//--------------------------------------------------
void Uninit(void)
{
	//-------------------------
	//各種オブジェクトの終了処理
	//-------------------------

	//キーボードの終了処理
	UninitKeyboard();

	//ジョイパッドの終了処理
	UninitJoypad();

	//マップの終了処理
	UninitMap();

	//プレイヤーの終了処理
	UninitPlayer();

	if (s_pFont != NULL)
	{//デバッグ表示用フォントの破棄
		s_pFont->Release();
		s_pFont = NULL;
	}

	if (s_pD3DDevice != NULL)
	{//Direct3Dデバイスの破棄
		s_pD3DDevice->Release();
		s_pD3DDevice = NULL;
	}

	if (s_pD3D != NULL)
	{//Direct3Dオブジェクトの破棄
		s_pD3D->Release();
		s_pD3D = NULL;
	}
}

//--------------------------------------------------
//更新処理
//--------------------------------------------------
void Update(void)
{
	//-------------------------
	//各種オブジェクトの更新処理
	//-------------------------

	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッドの更新処理
	UpdateJoypad();

#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_Y))
	{//ポーズキー(Pキー)が押されたかどうか
		s_bPause = !s_bPause;
	}

#endif //  _DEBUG

	if (!s_bPause)
	{//ポーズしてない時
		//マップの更新処理
		UpdateMap();

		//プレイヤーの更新処理
		UpdatePlayer();
	}

#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_F1) || GetJoypadTrigger(JOYKEY_BACK))
	{//F1キーが押された
		s_bDebug = !s_bDebug;
	}

#endif //  _DEBUG

}

//--------------------------------------------------
//描画処理
//--------------------------------------------------
void Draw(void)
{
	//画面クリア(バッグバッファ＆Zバッファのクリア)
	s_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//描画開始
	if (SUCCEEDED(s_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合
		//-------------------------
		//各種オブジェクトの描画処理
		//-------------------------

		//マップの描画処理
		DrawMap();

		//プレイヤーの描画処理
		DrawPlayer();

#ifdef  _DEBUG

		if (s_bDebug)
		{//デバッグ表示
			//デバッグの表示
			DrawDebug();
		}
		
#endif //  _DEBUG

		//描画終了
		s_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	s_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------------------------------
//デバイスの取得
//--------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return s_pD3DDevice;
}

//--------------------------------------------------
//デバッグの表示
//--------------------------------------------------
void DrawDebug(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[80];
	int nLength;

	//文字列の代入
	wsprintf(&aStr[0], "FPS            : %3d\n", s_nCountFPS);
	nLength = (int)strlen(&aStr[0]);		//文字数の取得

	if (s_bPause)
	{//ポーズしてる
		wsprintf(&aStr[nLength], "ポーズ [ Ｐ ]  :【 ON 】\n");
	}
	else
	{//ポーズしてない
		wsprintf(&aStr[nLength], "ポーズ [ Ｐ ]  :【 OFF 】\n");
	}

	//テキストの描画
	s_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}

//--------------------------------------------------
//頂点座標の設定処理 (真ん中に中心)
//--------------------------------------------------
void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);
}


//--------------------------------------------------
//頂点座標の設定処理 (下に中心)
//--------------------------------------------------
void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth, 0.0f, 0.0f);
}

//--------------------------------------------------
//rhwの設定処理
//--------------------------------------------------
void Setrhw(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//--------------------------------------------------
//頂点カラーの設定処理
//--------------------------------------------------
void Setcol(VERTEX_2D *pVtx, float Red, float Green, float Blue, float Alpha)
{
	pVtx[0].col = D3DXCOLOR(Red, Green, Blue, Alpha);
	pVtx[1].col = D3DXCOLOR(Red, Green, Blue, Alpha);
	pVtx[2].col = D3DXCOLOR(Red, Green, Blue, Alpha);
	pVtx[3].col = D3DXCOLOR(Red, Green, Blue, Alpha);
}

//--------------------------------------------------
//頂点カラーの設定処理
//--------------------------------------------------
void Settex(VERTEX_2D *pVtx, float ULeft, float URight, float VTop, float VBottom)
{
	pVtx[0].tex = D3DXVECTOR2(ULeft, VTop);
	pVtx[1].tex = D3DXVECTOR2(URight, VTop);
	pVtx[2].tex = D3DXVECTOR2(ULeft, VBottom);
	pVtx[3].tex = D3DXVECTOR2(URight, VBottom);
}