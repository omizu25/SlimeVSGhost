//--------------------------------------------------
//
// STG制作 ( input.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"

//-------------------------
//マクロ定義
//-------------------------
#define NUM_KEY_MAX		(256)		//キーの最大数

//-------------------------
//グローバル変数
//-------------------------
LPDIRECTINPUT8				g_pInput = NULL;						//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8		g_pDevKeyboard = NULL;					//入力デバイス(キーボード)へのポインタ
BYTE						g_aKeyState[NUM_KEY_MAX];				//キーボードのプレス情報
BYTE						g_aKeyStateTrigger[NUM_KEY_MAX];		//キーボードのトリガー情報
BYTE						g_aKeyStateRelease[NUM_KEY_MAX];		//キーボードのリリース情報
BYTE						g_aKeyStateRepeat[NUM_KEY_MAX];			//キーボードのリピート情報
int							g_aKeyPressTime[NUM_KEY_MAX];			//押し続けてる時間
XINPUT_STATE				g_joyKeyState;							//ジョイパッドのプレス情報
XINPUT_STATE				g_joyKeyStateTrigger;					//ジョイパッドのトリガー情報
D3DXVECTOR3					g_joyKeyStick;							//ジョイパッドのスティック

//-------------------------
//キーボードの初期化処理
//-------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//-------------------------
//ジョイパッドの初期化処理
//-------------------------
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//-------------------------
//キーボードの終了処理
//-------------------------
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//-------------------------
//ジョイパッドの終了処理
//-------------------------
void UninitJoypad(void)
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//-------------------------
//キーボードの更新処理
//-------------------------
void UpdateKeyboard(void)
{
	int nCntKey;
	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = ~g_aKeyState[nCntKey] & aKeyState[nCntKey];		//キーボードのトリガー情報を保存
			g_aKeyStateRelease[nCntKey] = g_aKeyState[nCntKey] & ~aKeyState[nCntKey];		//キーボードのリリース情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];										//キーボードのプレス情報を保存
			
			//g_aKeyStateRepeat[nCntKey] = ~g_aKeyState[nCntKey] & ~aKeyState[nCntKey];		//キーボードのリピート情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();		//キーボードのアクセス権を獲得
	}
}

//-------------------------
//ジョイパッドの更新処理
//-------------------------
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;		//ジョイパッドの入力情報

	//ジョイパッドの状態を取得	
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{//0の部分が操作しているのが何個目のパッドかを表してる
		//ジョイパッドのトリガー情報を保存
		g_joyKeyStateTrigger.Gamepad.wButtons = ~g_joyKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;

		if (g_joyKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			g_joyKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_joyKeyState.Gamepad.sThumbLX = 0;
		}

		if (g_joyKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			g_joyKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			g_joyKeyState.Gamepad.sThumbLY = 0;
		}

		//ジョイパッドのプレス情報を保存
		g_joyKeyState = joykeyState;
	}
}

//-------------------------
//キーボードのプレス情報を取得
//-------------------------
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//-------------------------
//ジョイパッドのプレス情報を取得
//-------------------------
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//-------------------------
//キーボードのトリガー情報を取得
//-------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//-------------------------
//ジョイパッドのトリガー情報を取得
//-------------------------
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joyKeyStateTrigger.Gamepad.wButtons & 0x01 << key) ? true : false;
}

//-------------------------
//キーボードのリリース情報を取得
//-------------------------
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//-------------------------
//キーボードのリピート情報を取得
//-------------------------
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//-------------------------
//ジョイパッドのスティック情報を取得
//-------------------------
D3DXVECTOR3 GetJoypadStick(JOYKEY key)
{
	switch (key)
	{
	case JOYKEY_L_STICK:		//Lスティック
		g_joyKeyStick = D3DXVECTOR3(g_joyKeyState.Gamepad.sThumbLX / 60000.0f, -g_joyKeyState.Gamepad.sThumbLY / 60000.0f, 0.0f);
		break;
	case JOYKEY_R_STICK:		//Rスティック
		g_joyKeyStick = D3DXVECTOR3(g_joyKeyState.Gamepad.sThumbRX / 30000.0f, -g_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return g_joyKeyStick;
}