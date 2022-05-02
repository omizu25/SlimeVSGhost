//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_MWTPG = 0,						// BGM0
	SOUND_LABEL_IN_THE_WAY,						// BGM1
	SOUND_LABEL_GO_TO_SEE_HER,					// BGM2
	SOUND_LABEL_GORILLA_TENDRENESS_FULLVER,		// BGM3
	SOUND_LABEL_MYSTERYMAN,						// BGM4
	SOUND_LABEL_RISE,							// BGM5
	SOUND_LABEL_START_BEETS,					// BGM6
	SOUND_LABEL_SE_GUN05,						// 銃音
	SOUND_LABEL_SE_JUMP,						// ジャンプ音
	SOUND_LABEL_SE_PUNCH,						// 吸い込み音
	SOUND_LABEL_SE_MAGICAL29,					// スター音
	SOUND_LABEL_SE_ONE_POINT29,					// ワンポイント音
	SOUND_LABEL_SE_SYSTEM20,					// キャンセル音
	SOUND_LABEL_SE_SYSTEM36,					// ポーズ音
	SOUND_LABEL_SE_SYSTEM40,					// セレクト音
	SOUND_LABEL_SE_SYSTEM49,					// 決定音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
