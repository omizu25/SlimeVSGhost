//--------------------------------------------------
//
// STG���� ( block.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "input.h"
#include "player.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define FILE_DATA				(1024)					//�t�@�C���̃f�[�^��
#define BlOCK_FILE		"data/test_map.txt"				//�}�b�v�̃e�L�X�g��
#define MAX_TEX			(BLOCKTYPE_MAX - 1)				//�e�N�X�`���̍ő吔

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static Block						s_aBlock[MAX_BLOCK];		//�u���b�N�̏��
static bool							s_bTexUse;					//�e�N�X�`���̎g�p���邩�ǂ���
static bool							s_bCollisionUse;			//�����蔻��𔻒f���邩�ǂ���

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStructBlock(int i);
static void InitBlockType(void);

//--------------------------------------------------
//�u���b�N�̏���������
//--------------------------------------------------
void InitBlock(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_bTexUse = true;
	s_bCollisionUse = true;

	for (int i = 0; i < MAX_TEX; i++)
	{//�������̃N���A
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block002.png",
		&s_pTexture[BLOCKTYPE_BROWN]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block006.png",
		&s_pTexture[BLOCKTYPE_ORANGE]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�u���b�N�̏��̏������ݒ�
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		//�v���C���[�̍\���̂̏���������
		InitStructBlock(i);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, s_aBlock[i].pos, s_aBlock[i].fWidth, s_aBlock[i].fHeight);

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

	//�u���b�N�̎�ނ̏���������
	InitBlockType();
}

//--------------------------------------------------
//�u���b�N�̏I������
//--------------------------------------------------
void UninitBlock(void)
{
	for (int i = 0; i < MAX_TEX; i++)
	{
		if (s_pTexture[i] != NULL)
		{//�v���C���[�̃e�N�X�`���̔j��
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
//�u���b�N�̍X�V����
//--------------------------------------------------
void UpdateBlock(void)
{
#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_F3))
	{//F3�L�[(F3�L�[)�������ꂽ���ǂ���
		s_bTexUse = !s_bTexUse;
	}

	if (GetKeyboardTrigger(DIK_F4))
	{//F4�L�[(F4�L�[)�������ꂽ���ǂ���
		s_bCollisionUse = !s_bCollisionUse;
	}

#endif //  _DEBUG
}

//--------------------------------------------------
//�u���b�N�̕`�揈��
//--------------------------------------------------
void DrawBlock(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TEX; i++)
	{
		//�e�N�X�`���̐ݒ�
		if (s_bTexUse)
		{//�g�p����
			pDevice->SetTexture(0, s_pTexture[i]);
		}
		else
		{//�g�p���Ȃ�
			pDevice->SetTexture(0, NULL);
		}

		//�u���b�N�̕`��
		for (int j = 0; j < MAX_BLOCK; j++)
		{
			if (s_aBlock[j].bUse && s_aBlock[j].type == i)
			{//�u���b�N���g�p����Ă���
				//�|���S���̕`��
				pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
					j * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
					2);							//�v���~�e�B�u(�|���S��)��
			}
		}
	}
}

//--------------------------------------------------
//�u���b�N�̐ݒ菈��
//--------------------------------------------------
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];

		if (pBlock->bUse)
		{//�u���b�N���g�p����Ă���
			continue;
		}

		//�u���b�N���g�p����Ă��Ȃ�q

		pBlock->pos = pos;
		pBlock->fWidth = fWidth;
		pBlock->fHeight = fHeight;

		//��ނ͂����ݒ肵�Ă���

		pBlock->bUse = true;		//�g�p���Ă����Ԃɂ���

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, pos, fWidth, fHeight);

		float fUTex = fWidth / (SCREEN_WIDTH / MAX_X_BLOCK / 2.0f);
		float fVTex = fHeight / (SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f);

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.0f, fUTex, 0.0f, fVTex);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;		//������for���𔲂���
	}
}

//--------------------------------------------------
//�u���b�N�̎擾����
//--------------------------------------------------
Block *GetBlock(void)
{
	return &s_aBlock[0];		//�u���b�N�̏���������
}

//--------------------------------------------------
//�e�N�X�`�����g�p���邩�̎擾����
//--------------------------------------------------
bool GetTexUseBlock(void)
{
	return s_bTexUse;
}

//--------------------------------------------------
//�����蔻��𔻒f���邩�̎擾����
//--------------------------------------------------
bool GetCollisionUse(void)
{
	return s_bCollisionUse;
}

//--------------------------------------------------
//�u���b�N�̓����蔻�菈��
//--------------------------------------------------
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bIsLanding = false;		//���n���Ă��邩�ǂ���

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];		//�u���b�N�̏��

		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			continue;
		}

		//�u���b�N���g�p����Ă���

		bool bCollision = false;

		if (pPos->x + fWidth > (pBlock->pos.x - pBlock->fWidth) &&
			pPos->x - fWidth < (pBlock->pos.x + pBlock->fWidth))
		{//X���u���b�N�͈͓̔�
			if (pPosOld->y <= (pBlock->pos.y - pBlock->fHeight) &&
				pPos->y >= (pBlock->pos.y - pBlock->fHeight))
			{//�u���b�N�̉������
				pPos->y = pBlock->pos.y - pBlock->fHeight;
				bIsLanding = true;
				bCollision = true;
			}
			else if (pPosOld->y - fHeight >= (pBlock->pos.y + pBlock->fHeight) &&
				pPos->y - fHeight < (pBlock->pos.y + pBlock->fHeight) &&
				pBlock->type == BLOCKTYPE_BROWN)
			{//�u���b�N�̏ォ�牺
				pPos->y = pBlock->pos.y + pBlock->fHeight + fHeight;
				pMove->y *= -REFLECT_BOUND;
				bCollision = true;
			}
		}

		if (pPosOld->y > (pBlock->pos.y - pBlock->fHeight) &&
			pPosOld->y - fHeight < (pBlock->pos.y + pBlock->fHeight) &&
			pBlock->type == BLOCKTYPE_BROWN)
		{//�O���Y���u���b�N�͈͓̔�
			if (pPosOld->x + fWidth <= (pBlock->pos.x - pBlock->fWidth) &&
				pPos->x + fWidth > (pBlock->pos.x - pBlock->fWidth))
			{//�u���b�N�̍��[
				pPos->x = pBlock->pos.x - pBlock->fWidth - fWidth;
				pMove->x = 0.0f;
				bCollision = true;
			}
			else if (pPosOld->x - fWidth >= (pBlock->pos.x + pBlock->fWidth) &&
					 pPos->x - fWidth < (pBlock->pos.x + pBlock->fWidth))
			{//�u���b�N�̉E�[
				pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth;
				pMove->x = 0.0f;
				bCollision = true;
			}
		}

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		if (s_bCollisionUse)
		{
			if (bCollision)
			{//�u���b�N�ɓ�������
				//���_�J���[�̐ݒ菈��
				Setcol(pVtx, 0.75f, 0.0f, 0.75f, 1.0f);
			}
			else
			{//�u���b�N�ɓ������ĂȂ�
				//���_�J���[�̐ݒ菈��
				Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		else
		{
			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

		}

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}

	return bIsLanding;
}

//--------------------------------------------------
//�u���b�N�̍\���̂̏���������
//--------------------------------------------------
static void InitStructBlock(int i)
{
	s_aBlock[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aBlock[i].fWidth = 0.0f;
	s_aBlock[i].fHeight = 0.0f;

	//��ނ͂����ݒ肵�Ă���

	s_aBlock[i].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
}

//--------------------------------------------------
//�u���b�N�̎�ނ̏���������
//--------------------------------------------------
static void InitBlockType(void)
{
	FILE *pBlockFile;		//�t�@�C���|�C���^��錾
	char aData[FILE_DATA];
	BLOCKTYPE aBlock[MAX_Y_BLOCK][MAX_X_BLOCK];		//�u���b�N�̎��

	//�t�@�C�����J��
	pBlockFile = fopen(BlOCK_FILE, "r");

	if (pBlockFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int i = 0;  i< MAX_Y_BLOCK; i++)
		{
			//��s�ǂݍ���
			fgets(aData, FILE_DATA, pBlockFile);

			for (int j = 0; j < MAX_X_BLOCK; j++)
			{
				//�ϊ�
				switch (atoi(&aData[j * 2]))
				{
				case 0:		//�g�p���Ȃ�
				default:
					aBlock[i][j] = BLOCKTYPE_NONE;
					break;

				case 1:		//���F
					aBlock[i][j] = BLOCKTYPE_BROWN;
					break;

				case 2:		//�I�����W
					aBlock[i][j] = BLOCKTYPE_ORANGE;
					break;
				}
			}
		}

		//�t�@�C�������
		fclose(pBlockFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		assert(false);

		//�t�@�C�������
		fclose(pBlockFile);
	}

	int nCntUse = 0;

	float fWidth = SCREEN_WIDTH / MAX_X_BLOCK / 2.0f;
	float fHeight = SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f;

	for (int i = 0; i < MAX_Y_BLOCK; i++)
	{
		float fPosY = (SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f) + (i * SCREEN_HEIGHT / MAX_Y_BLOCK);

		for (int j = 0; j < MAX_X_BLOCK; j++)
		{
			switch (aBlock[i][j])
			{
			case BLOCKTYPE_NONE:		//�g�p���Ȃ�
			default:
				continue;
				break;

			case BLOCKTYPE_BROWN:		//���F
			case BLOCKTYPE_ORANGE:		//�I�����W

				float fPosX = (SCREEN_WIDTH / MAX_X_BLOCK / 2.0f) + (j * SCREEN_WIDTH / MAX_X_BLOCK);

				s_aBlock[nCntUse].type = aBlock[i][j];

				//�u���b�N�̐ݒ�
				SetBlock(D3DXVECTOR3(fPosX, fPosY, 0.0f), fWidth, fHeight);
				nCntUse++;

				break;
			}
		}
	}
}