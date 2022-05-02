//--------------------------------------------------
//
// ACG���� ( block.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _BLOCK_H_		//���̃}�N����`������ĂȂ�������
#define _BLOCK_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_X_BLOCK					(32)				//�u���b�N�̉��̍ő吔
#define MAX_Y_BLOCK					(18)				//�u���b�N�̏c�̍ő吔
#define MAX_BLOCK		(MAX_Y_BLOCK * MAX_X_BLOCK)		//�u���b�N�̍ő吔
#define TOP_BLOCK					(7)					//��̒i�̃u���b�N
#define MIDDLE_BLOCK				(12)				//�^�񒆂̒i�̃u���b�N
#define BOTTOM_BLOCK				(17)				//���̒i�̃u���b�N

//--------------------------------------------------
//�u���b�N�̎�ނ��`
//--------------------------------------------------
typedef enum
{
	BLOCKTYPE_BLUE = 0,			//��
	BLOCKTYPE_LIGHT_BLUE,		//���F
	BLOCKTYPE_B_THROUGH,		//�̒ʂ蔲����
	BLOCKTYPE_L_THROUGH,		//���F�̒ʂ蔲����
	BLOCKTYPE_NONE,
	BLOCKTYPE_MAX
}BLOCKTYPE;

//--------------------------------------------------
//�u���b�N�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	float			fWidth;			//��
	float			fHeight;		//����
	BLOCKTYPE		type;			//���
	bool			bCollision;		//�����邩�ǂ���
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Block;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);
Block *GetBlock(void);
bool GetTexUseBlock(void);
bool GetCollisionUse(void);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void CollisionTopBlock(D3DXVECTOR3 *pPos, float fWidth, float fHeight);

#endif // !_Block_H_