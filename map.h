//--------------------------------------------------
//
// �A�N�V�����Q�[������ ( map.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _MAP_H_		//���̃}�N����`������ĂȂ�������
#define _MAP_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�}�b�v�̎�ނ��`
//--------------------------------------------------
typedef enum
{
	MAPTYPE_PURPLE = 0,		//��
	MAPTYPE_BROWN,			//���F
	MAXTYPE_BLUE,			//��
	MAPTYPE_MAX
}MAPTYPE;

//--------------------------------------------------
//�}�b�v�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		move;		//�ړ���
	MAPTYPE			type;		//���
}Map;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
Map *GetMap(void);

#endif // !_Map_H_