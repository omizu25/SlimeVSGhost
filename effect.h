//--------------------------------------------------
//
// ACG���� ( effect.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _EFFECT_H_		//���̃}�N����`������ĂȂ�������
#define _EFFECT_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, int nType);

#endif // !_EFFECT_H_