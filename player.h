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
//�U���̏�Ԃ��`
//--------------------------------------------------
typedef enum
{
	ATTACKSTATE_NONE = 0,		//�������Ă��Ȃ����
	ATTACKSTATE_IN,				//�z�����ݏ��
	ATTACKSTATE_STORE,			//�~���Ă�����
	ATTACKSTATE_OUT,			//�f���o�����
	ATTACKSTATE_MAX
}ATTACKSTATE;

//-------------------------
//�v���C���[�̏�Ԃ��`
//-------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//�ʏ���
	PLAYERSTATE_DAMAGE,			//�_���[�W���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//--------------------------------------------------
//�v���C���[�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//���݂̈ʒu
	D3DXVECTOR3		posOld;				//�O��̈ʒu
	D3DXVECTOR3		move;				//�ړ���
	JUMPSTATE		jump;				//�W�����v���
	ATTACKSTATE		attack;				//�U�����
	PLAYERSTATE		state;				//���
	float			fWidth;				//��
	float			fHeight;			//����
	int				nCounterState;		//��ԊǗ��J�E���^�[
	int				nCounterAttack;		//�U���J�E���^�[
	int				nLife;				//����
	bool			bDirection;			//���� [false  : ������ true  : �E����]
}Player;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);
Player *GetPlayer(void);
bool GetTexUsePlayer(void);

#endif // !_PLAYER_H_