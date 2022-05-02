//--------------------------------------------------
//
// STG���� ( player.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PLAYER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//-------------------------
//�}�N����`
//-------------------------
#define PLAYER_WIDTH		(150)		//�v���C���[�̕�
#define PLAYER_HEIGHT		(120)		//�v���C���[�̍���

//-------------------------
//�v���C���[�̍\���̂��`
//-------------------------
typedef struct
{
	D3DXVECTOR3			pos;			//�ʒu
	D3DXVECTOR3			move;			//�ړ���
	D3DXVECTOR3			rot;			//����
	float				fLength;		//�Ίp���̒���
	float				fAngle;			//�Ίp���̊p�x
	int					nLife;			//�̗�
	int					nStock;			//�c�@
	bool				bDisp;			//�\�����Ă��邩�ǂ���
}Player;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

#endif // !_PLAYER_H_