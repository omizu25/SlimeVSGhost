//--------------------------------------------------
//
// ACG���� ( item.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _ITEM_H_		//���̃}�N����`������ĂȂ�������
#define _ITEM_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"
#include "player.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define ITEM_SIZE		(70)		//�A�C�e���̃T�C�Y

//-------------------------
//�G�̎�ނ��`
//-------------------------
typedef enum
{
	ITEMTYPE_BLOCK = 0,		//�u���b�N
	ITEMTYPE_STAR,			//�X�^�[
	ITEMTYPE_MAX
}ITEMTYPE;

//--------------------------------------------------
//�A�C�e���̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	D3DXVECTOR3		move;			//�ړ���
	ITEMTYPE		type;			//���
	float			fWidth;			//��
	float			fHeight;		//����
	float			fRot;			//��]
	float			fLength;		//�Ίp���̒���
	float			fAngle;			//�Ίp���̊p�x
	bool			bDirection;		//���� [false  : ������ true  : �E����]
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Item;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, bool bDirection);
Item *GetItem(void);
bool GetDeath(void);
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fHeight);
void InhaleItem(D3DXVECTOR3 pos, ATTACKSTATE *pAttack, float fWidth, float fHeight, bool bDirection);

#endif // !_Item_H_