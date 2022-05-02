//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
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
	SOUND_LABEL_SE_COUNTDOWN,					// �J�E���g�_�E����
	SOUND_LABEL_SE_�^�C�g���\��,				// �S�[�X�g���S��
	SOUND_LABEL_SE_�q���[���Ɨ���,				// �v���C���[���S��
	SOUND_LABEL_SE_�|�J���Ƃ��񂱂�,			// �v���C���[�̓���������
	SOUND_LABEL_SE_KO,							// K.O��
	SOUND_LABEL_SE_JUMP,						// �W�����v��
	SOUND_LABEL_SE_IN,							// �z�����݉�
	SOUND_LABEL_SE_MAGICAL29,					// �X�^�[��
	SOUND_LABEL_SE_ONE_POINT29,					// �����|�C���g��
	SOUND_LABEL_SE_SYSTEM20,					// �L�����Z����
	SOUND_LABEL_SE_SYSTEM36,					// �|�[�Y��
	SOUND_LABEL_SE_SYSTEM40,					// �Z���N�g��
	SOUND_LABEL_SE_SYSTEM49,					// ���艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
