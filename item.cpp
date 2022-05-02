//--------------------------------------------------
//
// ACG���� ( item.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "item.h"
#include "player.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_ITEM		(256)		//�A�C�e���̍ő吔

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(int i);
static void UpdateOffScreen(Item *pItem);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static Item							s_aItem[MAX_ITEM];		//�A�C�e���̏��

//--------------------------------------------------
//�A�C�e���̏���������
//--------------------------------------------------
void InitItem(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block005.png",
		&s_pTexture);

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
		//�\���̂̏���������
		InitStruct(i);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, s_aItem[i].pos, s_aItem[i].fWidth, s_aItem[i].fHeight);

		//rhw�̐ݒ菈��
		Setrhw(pVtx);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

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
	if (s_pTexture != NULL)
	{//�e�N�X�`���̔j��
		s_pTexture->Release();
		s_pTexture = NULL;
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

		//��ʊO����
		UpdateOffScreen(pItem);

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

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (s_aItem[i].bUse)
		{//�A�C�e�����g�p����Ă���
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
void SetItem(D3DXVECTOR3 pos)
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
		pItem->fWidth = ITEM_SIZE * 0.5f;
		pItem->fHeight = ITEM_SIZE * 0.5f;
		pItem->bUse = true;		//�g�p���Ă����Ԃɂ���

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, pos, pItem->fWidth, pItem->fHeight);

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

		if (!pItem->bUse)
		{//�A�C�e�����g�p����Ă��Ȃ�
			continue;
		}

		//�A�C�e�����g�p����Ă���

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
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(int i)
{
	s_aItem[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aItem[i].fWidth = 0.0f;
	s_aItem[i].fHeight = 0.0f;
	s_aItem[i].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
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