//--------------------------------------------------
//
// ACG���� ( pause.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PAUSE_H_		//���̃}�N����`������ĂȂ�������
#define _PAUSE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//-------------------------
//�}�N����`
//-------------------------
#define MAX_PAUSE				(5)			//�|�[�Y�̍ő吔
#define NUM_PAUSE				(2)			//�|�[�Y�̎��
#define PAUSE_WIDTH				(200)		//�|�[�Y�̕��̔���
#define PAUSE_HEIGHT			(200)		//�|�[�Y�̍����̔���
#define PAUSEMENU_WIDTH			(215)		//�|�[�Y�̑I�����̕�
#define PAUSEMENU_HEIGHT		(70)		//�|�[�Y�̑I�����̍���

//-------------------------
//�|�[�Y�̃��[�h���`
//-------------------------
typedef enum
{
	PAUSEMENU_GAME = 0,		//�Q�[���ɖ߂�
	PAUSEMENU_RETRY,		//�Q�[������蒼��
	PAUSEMENU_TITLE,		//�^�C�g����ʂɖ߂�
	PAUSEMENU_MAX
}PAUSEMENU;

//-------------------------
//�|�[�Y�̑I�����̏��(�_�ŋ)���`
//-------------------------
typedef enum
{
	PAUSESTATE_IN = 0,		//������
	PAUSESTATE_OUT,			//�����Ȃ�
	PAUSESTATE_MAX
}PAUSESTATE;

//-------------------------
//�|�[�Y�̍\���̂��`
//-------------------------
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXCOLOR		col;		//�F
	PAUSESTATE		state;		//��ԏ��(�_�ŋ)
}Pause;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif // !_PAUSE_H_