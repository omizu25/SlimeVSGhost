//--------------------------------------------------
//
// ACG���� ( colon.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _COLON_H_		//���̃}�N����`������ĂȂ�������
#define _COLON_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitColon(void);
void UninitColon(void);
void UpdateColon(void);
void DrawColon(void);
void SetColon(D3DXVECTOR3 pos, float fWidth, float fHeight);

#endif // !_COLON_H_