//--------------------------------------------------
//
// ACG���� ( player.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PLAYER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define REFLECT_BOUND		(0.8f)		//�o�E���h�̒��˕Ԃ��

//--------------------------------------------------
//�W�����v�̏�Ԃ��`
//--------------------------------------------------
typedef enum
{
	JUMPSTATE_NONE = 0,		//�������Ă��Ȃ����
	JUMPSTATE_JUMP,			//�W�����v���
	JUMPSTATE_BOUND,		//�o�E���h���
	JUMPSTATE_MAX
}JUMPSTATE;

//--------------------------------------------------
//�v���C���[�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//���݂̈ʒu
	D3DXVECTOR3		posOld;				//�O��̈ʒu
	D3DXVECTOR3		move;				//�ړ���
	JUMPSTATE		jump;				//�W�����v���
	float			fWidth;				//��
	float			fHeight;			//����
	int				nCounterState;		//��ԊǗ��J�E���^�[
}Player;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
bool GetTexUsePlayer(void);

#endif // !_PLAYER_H_