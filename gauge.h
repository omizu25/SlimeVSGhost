//--------------------------------------------------
//
// ACG���� ( gauge.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _GAUGE_H_		//���̃}�N����`������ĂȂ�������
#define _GAUGE_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�Q�[�W�̎g�p�҂��`
//--------------------------------------------------
typedef enum
{
	GAUGEUSE_PLAYER = 0,		//�v���C���[
	GAUGEUSE_BOY,				//�j�̎q (����)
	GAUGEUSE_GIRL,				//���̎q (����)
	GAUGEUSE_MAX
}GAUGEUSE;

//--------------------------------------------------
//�Q�[�W�̎�ނ��`
//--------------------------------------------------
typedef enum
{
	GAUGETYPE_MAXIMUM = 0,		//�ő�l
	GAUGETYPE_DECREASE,			//������
	GAUGETYPE_REMAINING,		//�c��
	GAUGETYPE_MAX
}GAUGETYPE;

//--------------------------------------------------
//�Q�[�W�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	GAUGEUSE		use;			//�g�p��
	GAUGETYPE		type;			//���
	float			fWidth;			//��
	float			fHeight;		//����
	int				nCounter;		//�J�E���^�[
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Gauge;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);
void SetGauge(GAUGEUSE use, int nLife);
void SubGauge(GAUGEUSE use, int nLife);

#endif // !_GAUGE_H_