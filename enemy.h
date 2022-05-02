//--------------------------------------------------
//
// ACG���� ( enemy.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _ENEMY_H_		//���̃}�N����`������ĂȂ�������
#define _ENEMY_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define ENEMY_WIDTH			(100.0f)		//�G�̕��̔���
#define ENEMY_HEIGHT		(100.0f)		//�G�̍����̔���

//-------------------------
//�G�̎�ނ��`
//-------------------------
typedef enum
{
	ENEMYTYPE_YELLOW = 0,		//���F
	ENEMYTYPE_PURPLE,			//���F
	ENEMYTYPE_MAX
}ENEMYTYPE;

//-------------------------
//�G�̏�Ԃ��`
//-------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//�ʏ���
	ENEMYSTATE_DAMAGE,			//�_���[�W���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//-------------------------
//�G�̃|�b�v�ꏊ���`
//-------------------------
typedef enum
{
	ENEMYPOP_TOP = 0,		//��̒i
	ENEMYPOP_MIDDLE,		//�^�񒆂̒i
	ENEMYPOP_BOTTOM,		//���̒i
	ENEMYPOP_MAX
}ENEMYPOP;

//-------------------------
//�G�̍\���̂��`
//-------------------------
typedef struct
{
	D3DXVECTOR3		pos;				//�ʒu
	D3DXVECTOR3		posOld;				//�O��̈ʒu
	D3DXVECTOR3		move;				//�ړ���
	ENEMYTYPE		type;				//���
	ENEMYSTATE		state;				//���
	ENEMYPOP		pop;				//�|�b�v�ꏊ
	int				nCounterState;		//��ԊǗ��J�E���^�[
	int				nCounterAnim;		//�A�j���[�V�����J�E���^�[
	int				nPatternAnim;		//�A�j���[�V�����p�^�[��No.
	bool			bDirection;			//���� [false  : ������ true  : �E����]
	int				nLife;				//�̗�
	bool			bUse;				//�g�p���Ă��邩�ǂ���
}Enemy;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);

#endif // !_ENEMY_H_