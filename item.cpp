//--------------------------------------------------
//
// ACG���� ( item.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "enemy.h"
#include "item.h"
#include "player.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_ITEM			(256)			//�A�C�e���̍ő吔
#define INHALE_DIV			(0.05f)			//�z�����݂̕�����
#define INHALE_DISTANCE		(350.0f)		//�z�����ދ���
#define MAX_INERTIA			(0.75f)			//�����̍ő�l
#define STER_MOVE			(10.0f)			//�X�^�[�̈ړ���

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(Item *pItem);
static void UpdateOffScreen(Item *pItem);
static void UpdateCollision(Item *pItem);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[ITEMTYPE_MAX];		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�̃|�C���^
static Item							s_aItem[MAX_ITEM];				//�A�C�e���̏��

//--------------------------------------------------
//�A�C�e���̏���������
//--------------------------------------------------
void InitItem(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ITEMTYPE_MAX; i++)
	{//�������̃N���A
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block005.png",
		&s_pTexture[ITEMTYPE_BLOCK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Star002.png",
		&s_pTexture[ITEMTYPE_STAR]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�A�C�e���̏��̏������ݒ�
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];		//�A�C�e���̏��

		//�\���̂̏���������
		InitStruct(pItem);

		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�A�C�e���̏I������
//--------------------------------------------------
void UninitItem(void)
{
	for (int i = 0; i < ITEMTYPE_MAX; i++)
	{
		if (s_pTexture[i] != NULL)
		{//�e�N�X�`���̔j��
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//�A�C�e���̍X�V����
//--------------------------------------------------
void UpdateItem(void)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];		//�A�C�e���̏��

		if (!pItem->bUse)
		{//�A�C�e�����g�p����Ă��Ȃ�
			continue;
		}

		//�A�C�e�����g�p����Ă���

		//�ʒu���X�V
		pItem->pos += pItem->move;
		
		if (pItem->type == ITEMTYPE_BLOCK)
		{//�u���b�N�̎�
			//�����E�ړ��ʂ��X�V (����������)
			pItem->move.x += (0.0f - pItem->move.x) * MAX_INERTIA;
		}

		//��ʊO����
		UpdateOffScreen(pItem);

		//�����蔻�菈��
		UpdateCollision(pItem);

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, pItem->pos, pItem->fWidth, pItem->fHeight);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//�A�C�e���̕`�揈��
//--------------------------------------------------
void DrawItem(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (s_aItem[i].bUse)
		{//�A�C�e�����g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture[s_aItem[i].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u(�|���S��)��
		}
	}
}

//--------------------------------------------------
//�A�C�e���̐ݒ菈��
//--------------------------------------------------
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, bool bDirection)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];

		if (pItem->bUse)
		{//�A�C�e�����g�p����Ă���
			continue;
		}

		//�A�C�e�����g�p����Ă��Ȃ�

		pItem->pos = pos;
		pItem->type = type;
		pItem->fWidth = ITEM_SIZE * 0.5f;
		pItem->fHeight = ITEM_SIZE * 0.5f;
		pItem->bDirection = bDirection;
		pItem->bUse = true;		//�g�p���Ă����Ԃɂ���

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, pos, pItem->fWidth, pItem->fHeight);

		switch (pItem->type)
		{
		case ITEMTYPE_BLOCK:		//�u���b�N
			pItem->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;

		case ITEMTYPE_STAR:			//�X�^�[

			if (bDirection)
			{//�E����
				pItem->move = D3DXVECTOR3(STER_MOVE, 0.0f, 0.0f);
			}
			else
			{//������
				pItem->move = D3DXVECTOR3(-STER_MOVE, 0.0f, 0.0f);
			}

		default:
			break;
		}

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;		//������for���𔲂���
	}
}

//--------------------------------------------------
//�A�C�e���̎擾����
//--------------------------------------------------
Item *GetItem(void)
{
	return &s_aItem[0];		//�A�C�e���̏���������
}

//--------------------------------------------------
//�A�C�e���̓����蔻�菈��
//--------------------------------------------------
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];		//�A�C�e���̏��

		if (!pItem->bUse || pItem->type == ITEMTYPE_STAR)
		{//�A�C�e�����g�p����Ă��Ȃ��A�w��̎�ނ���Ȃ�
			continue;
		}

		//�A�C�e�����g�p����Ă���A�w��̎��

		float fLeft = pItem->pos.x - pItem->fWidth;
		float fRight = pItem->pos.x + pItem->fWidth;
		float fTop = pItem->pos.y - pItem->fHeight;
		float fBottom = pItem->pos.y + pItem->fHeight;

		if ((pPosOld->y > fTop) && (pPosOld->y - fHeight < fBottom))
		{//�O���Y���A�C�e���͈͓̔�
			if ((pPosOld->x + fWidth <= fLeft) && (pPos->x + fWidth > fLeft))
			{//�A�C�e���̍��[
				pItem->pos.x = pPos->x + fWidth + pItem->fWidth;
			}
			else if ((pPosOld->x - fWidth >= fRight) && (pPos->x - fWidth < fRight))
			{//�A�C�e���̉E�[
				pItem->pos.x = pPos->x - fWidth - pItem->fWidth;
			}
		}
	}
}

//--------------------------------------------------
//�A�C�e���̋z�����ݏ���
//--------------------------------------------------
void InhaleItem(D3DXVECTOR3 pos, ATTACKSTATE *pAttack, float fWidth, float fHeight, bool bDirection)
{
	for (int i = 0; i < MAX_ITEM; i++)
	{
		Item *pItem = &s_aItem[i];		//�A�C�e���̏��

		if (!pItem->bUse || pItem->type == ITEMTYPE_STAR)
		{//�A�C�e�����g�p����Ă��Ȃ��A�w��̎�ނ���Ȃ�
			continue;
		}

		//�A�C�e�����g�p����Ă���A�w��̎��

		if (bDirection != pItem->bDirection)
		{//�������t�Ȏ�
			if (pos.y >= pItem->pos.y && pos.y - fHeight <= pItem->pos.y)
			{//�������v���C���[�͈͓̔�
				if ((!bDirection && pos.x >= pItem->pos.x) ||
					(bDirection && pos.x <= pItem->pos.x))
				{//�������Ńv���C���[��荶�ɂ���A�E�����Ńv���C���[���E�ɂ���
					float Difference = pos.x - pItem->pos.x;

					if ((Difference <= 0.0f && Difference >= -INHALE_DISTANCE) ||
						(Difference >= 0.0f && Difference <= INHALE_DISTANCE))
					{//�����͈͓�
						pItem->move.x += Difference * INHALE_DIV;

						float fLeft = pItem->pos.x - pItem->fWidth;
						float fRight = pItem->pos.x + pItem->fWidth;

						if ((pos.x - fWidth <= fRight) && (pos.x + fWidth >= fLeft))
						{//�A�C�e���ƃv���C���[���d�Ȃ���
							pItem->bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
							pItem->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							*pAttack = ATTACKSTATE_STORE;
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(Item *pItem)
{
	pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pItem->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pItem->type = ITEMTYPE_BLOCK;
	pItem->fWidth = 0.0f;
	pItem->fHeight = 0.0f;
	pItem->bDirection = false;		//������
	pItem->bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
}

//--------------------------------------------------
//��ʊO����
//--------------------------------------------------
static void UpdateOffScreen(Item *pItem)
{
	float ItemSize = ITEM_SIZE * 0.5f;

	//��ʒ[����
	if (pItem->pos.x >= SCREEN_WIDTH + ItemSize || pItem->pos.x <= -ItemSize)
	{//�E�[�����[
		pItem->bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//--------------------------------------------------
//�����蔻�菈��
//--------------------------------------------------
static void UpdateCollision(Item *pItem)
{
	Player *pPlayer = GetPlayer();		//�v���C���[�̎擾

	if (pItem->pos.y <= (pPlayer->pos.y + pItem->fHeight) &&
		pItem->pos.y >= (pPlayer->pos.y - pPlayer->fHeight - pItem->fHeight) &&
		pItem->pos.x <= (pPlayer->pos.x + pPlayer->fWidth + pItem->fWidth) &&
		pItem->pos.x >= (pPlayer->pos.x - pPlayer->fWidth - pItem->fWidth) &&
		pPlayer->state == PLAYERSTATE_NORMAL && pPlayer->attack != ATTACKSTATE_IN &&
		pItem->type == ITEMTYPE_BLOCK && pItem->move.x == 0.0f)
	{//�v���C���[�Ƀu���b�N������������

		//�v���C���[�̃q�b�g����
		HitPlayer(5);

		pItem->bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
	}

	Enemy *pEnemy = GetEnemy();		//�G�̎擾

	for (int i = 0; i < ENEMYTYPE_MAX; i++, pEnemy++)
	{
		if (!pEnemy->bUse)
		{//�G���g�p����Ă��Ȃ�
			continue;
		}

		//�G���g�p����Ă���

		if (pItem->pos.y <= (pEnemy->pos.y + pItem->fHeight) &&
			pItem->pos.y >= (pEnemy->pos.y - pEnemy->fHeight - pItem->fHeight) &&
			pItem->pos.x <= (pEnemy->pos.x + pEnemy->fWidth + pItem->fWidth) &&
			pItem->pos.x >= (pEnemy->pos.x - pEnemy->fWidth - pItem->fWidth) &&
			pEnemy->state == ENEMYSTATE_NORMAL && pItem->type == ITEMTYPE_STAR)
		{//�G�ɃX�^�[������������

			//�G�̃q�b�g����
			HitEnemy(i, 10);

			pItem->bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
		}
	}
}