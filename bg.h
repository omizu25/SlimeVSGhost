//--------------------------------------------------
//
// ACG���� ( bg.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _BG_H_		//���̃}�N����`������ĂȂ�������
#define _BG_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//-------------------------
//�}�N����`
//-------------------------
#define MOVE_BG		(3)		//�����w�i�̐�
#define NUM_BG		(2)		//�w�i�̎��

//-------------------------
//�Q�[���̏��
//-------------------------
typedef enum
{
	BATTLEBG_NORMAL = 0,		//�ʏ��
	BATTLEBG_BOSS,				//�{�X��
	BATTLEBG_APPEAR,			//�o�����
	BATTLEBG_MAX
}BATTLEBG;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void SetBG(BATTLEBG battle);
BATTLEBG GetBG(void);

#endif // !_BG_H_