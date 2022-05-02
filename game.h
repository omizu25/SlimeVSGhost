//--------------------------------------------------
//
// ACG���� ( game.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _GAME_H_		//���̃}�N����`������ĂȂ�������
#define _GAME_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//-------------------------
//�Q�[���̏��
//-------------------------
typedef enum
{
	GAMESTATE_NONE = 0,		//�������Ă��Ȃ����
	GAMESTATE_START,		//�J�n���(�Q�[���J�n��)
	GAMESTATE_NORMAL,		//�ʏ���(�Q�[���i�s��)
	GAMESTATE_END,			//�I�����(�Q�[���I����)
	GAMESTATE_MAX
}GAMESTATE;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
void SetEnablePause(bool bPause);

#endif // !_GAME_H_