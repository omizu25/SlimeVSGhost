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
//�G�t�F�N�g�̎�ނ��`
//--------------------------------------------------
typedef enum
{
	EFFECTTYPE_IN = 0,			//�z������
	EFFECTTYPE_STAR,			//�X�^�[
	EFFECTTYPE_MOVE,			//�ړ�
	EFFECTTYPE_EXPLOSION,		//����
	EFFECTTYPE_MAX
}EFFECTTYPE;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(EFFECTTYPE type);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, EFFECTTYPE type);
void SetParticle(D3DXVECTOR3 pos, EFFECTTYPE type, bool bDirection);

#endif // !_EFFECT_H_