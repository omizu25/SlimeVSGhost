//--------------------------------------------------
//
// ACG���� ( item.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _ITEM_H_		//���̃}�N����`������ĂȂ�������
#define _ITEM_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define ITEM_SIZE		(70)		//�A�C�e���̃T�C�Y

//--------------------------------------------------
//�A�C�e���̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	float			fWidth;			//��
	float			fHeight;		//����
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Item;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos);
Item *GetItem(void);
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif // !_Item_H_