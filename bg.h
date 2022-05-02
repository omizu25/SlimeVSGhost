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
//�w�i�̍\���̂��`
//-------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	float			fWidth;			//��
	float			fHeight;		//����
}BG;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif // !_BG_H_