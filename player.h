//--------------------------------------------------
//
// �A�N�V�����Q�[������ ( player.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _PLAYER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�v���C���[�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//���݂̈ʒu
	D3DXVECTOR3		posOld;				//�O��̈ʒu
	D3DXVECTOR3		move;				//�ړ���
	bool			bjump;				//�W�����v��� [���Ă�  : true ���ĂȂ�  : false]
	int				nCounterAnim;		//�A�j���[�V�����J�E���^�[
	int				nPatternAnim;		//�A�j���[�V�����p�^�[��No.
	int				nDirectionMove;		//���� [0  : �E���� 1  : ������]
}Player;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player GetPlayer(void);


#endif // !_PLAYER_H_