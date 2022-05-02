//--------------------------------------------------
//
// ACG���� ( block.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "input.h"
#include "player.h"
#include "setup.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define FILE_DATA				(1024)					//�t�@�C���̃f�[�^��
#define FILE_NAME		"data/test_map.txt"				//�}�b�v�̃p�X
#define MAX_TEX			(BLOCKTYPE_MAX - 1)				//�e�N�X�`���̍ő吔

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(int i);
static void InitType(void);
static void DownCollision(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static Block						s_aBlock[MAX_BLOCK];		//�u���b�N�̏��
static bool							s_bThrough;					//�u���b�N���ʂ蔲���邩 [false :�ʂ蔲���Ȃ� true : �ʂ蔲����]

//--------------------------------------------------
//�u���b�N�̏���������
//--------------------------------------------------
void InitBlock(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_bThrough = false;

	for (int i = 0; i < MAX_TEX; i++)
	{//�������̃N���A
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block009.png",
		&s_pTexture[BLOCKTYPE_BLUE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block011.png",
		&s_pTexture[BLOCKTYPE_LIGHT_BLUE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block010.png",
		&s_pTexture[BLOCKTYPE_B_THROUGH]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block012.png",
		&s_pTexture[BLOCKTYPE_L_THROUGH]);

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
		//�\���̂̏���������
		InitStruct(i);

		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//��ނ̏���������
	InitType();
}

//--------------------------------------------------
//�u���b�N�̏I������
//--------------------------------------------------
void UninitBlock(void)
{
	for (int i = 0; i < MAX_TEX; i++)
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
//�u���b�N�̍X�V����
//--------------------------------------------------
void UpdateBlock(void)
{
	if (s_bThrough)
	{//�ʂ蔲����
		//������Ƃ��̓����蔻�菈��
		DownCollision();
	}
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
		pDevice->SetTexture(0, s_pTexture[i]);

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

		//�u���b�N���g�p����Ă��Ȃ�

		pBlock->pos = pos;
		pBlock->fWidth = fWidth;
		pBlock->fHeight = fHeight;

		//��ނ͂����ݒ肵�Ă���

		pBlock->bCollision = true;
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

		float fLeft = pBlock->pos.x - pBlock->fWidth;
		float fRight = pBlock->pos.x + pBlock->fWidth;
		float fTop = pBlock->pos.y - pBlock->fHeight;
		float fBottom = pBlock->pos.y + pBlock->fHeight;

		if ((pPos->x + fWidth > fLeft) && (pPos->x - fWidth < fRight))
		{//X���u���b�N�͈͓̔�
			if ((pPosOld->y <= fTop) && (pPos->y >= fTop))
			{//�u���b�N�̉������
				if (pBlock->bCollision)
				{//�����蔻�肠��
					pPos->y = fTop;
					bIsLanding = true;
				}
			}
			else if ((pPosOld->y - fHeight >= fBottom) && (pPos->y - fHeight < fBottom))
			{//�u���b�N�̏ォ�牺
				if (pBlock->type == BLOCKTYPE_BLUE || pBlock->type == BLOCKTYPE_LIGHT_BLUE)
				{//�w��̎��
					pPos->y = fBottom + fHeight;
					pMove->y *= -REFLECT_BOUND;
				}
			}
		}

		if ((pPosOld->y > fTop) && (pPosOld->y - fHeight < fBottom))
		{//�O���Y���u���b�N�͈͓̔�
			if ((pBlock->type == BLOCKTYPE_BLUE) || (pBlock->type == BLOCKTYPE_LIGHT_BLUE))
			{//�w��̎��
				if ((pPosOld->x + fWidth <= fLeft) && (pPos->x + fWidth > fLeft))
				{//�u���b�N�̍��[
					pPos->x = fLeft - fWidth;
					pMove->x = 0.0f;
				}
				else if ((pPosOld->x - fWidth >= fRight) && (pPos->x - fWidth < fRight))
				{//�u���b�N�̉E�[
					pPos->x = fRight + fWidth;
					pMove->x = 0.0f;
				}
			}
		}
	}

	return bIsLanding;
}

//--------------------------------------------------
//�u���b�N�̏�[�̓����蔻�菈��
//--------------------------------------------------
void CollisionTopBlock(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];		//�u���b�N�̏��

		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			continue;
		}

		float fLeft = pBlock->pos.x - pBlock->fWidth;
		float fRight = pBlock->pos.x + pBlock->fWidth;
		float fTop = pBlock->pos.y - pBlock->fHeight;
		float fBottom = pBlock->pos.y + pBlock->fHeight;

		if ((pPos->x + fWidth >= fLeft) && (pPos->x - fWidth <= fRight) &&
			(pPos->y >= fTop) && (pPos->y - fHeight <= fBottom))
		{//X,Y���u���b�N�͈͓̔�
			if ((pBlock->type == BLOCKTYPE_B_THROUGH) ||
				(pBlock->type == BLOCKTYPE_L_THROUGH))
			{//�ʂ蔲����u���b�N��������
				s_bThrough = true;
			}
			else if ((pBlock->type == BLOCKTYPE_BLUE) ||
				(pBlock->type == BLOCKTYPE_LIGHT_BLUE))
			{//�ʂ蔲���Ȃ��u���b�N��������
				s_bThrough = false;
				break;
			}
		}
	}

	if (s_bThrough)
	{//�ʂ蔲����
		for (int i = 0; i < MAX_BLOCK; i++)
		{
			Block *pBlock = &s_aBlock[i];		//�u���b�N�̏��

			if (!pBlock->bUse)
			{//�u���b�N���g�p����Ă��Ȃ�
				continue;
			}

			//�u���b�N���g�p����Ă���

			if ((pBlock->type == BLOCKTYPE_B_THROUGH) ||
				(pBlock->type == BLOCKTYPE_L_THROUGH))
			{//�w��̎��
				pBlock->bCollision = false;
			}
		}
	}
}

//--------------------------------------------------
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(int i)
{
	s_aBlock[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aBlock[i].fWidth = 0.0f;
	s_aBlock[i].fHeight = 0.0f;

	//��ނ͂����ݒ肵�Ă���

	s_aBlock[i].bCollision = false;
	s_aBlock[i].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
}

//--------------------------------------------------
//��ނ̏���������
//--------------------------------------------------
static void InitType(void)
{
	FILE *pBlockFile;		//�t�@�C���|�C���^��錾
	char aData[FILE_DATA];
	BLOCKTYPE aBlock[MAX_Y_BLOCK][MAX_X_BLOCK];		//�u���b�N�̎��

	//�t�@�C�����J��
	pBlockFile = fopen(FILE_NAME, "r");

	if (pBlockFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int y = 0;  y < MAX_Y_BLOCK; y++)
		{
			//��s�ǂݍ���
			fgets(aData, FILE_DATA, pBlockFile);

			for (int x = 0; x < MAX_X_BLOCK; x++)
			{
				//�ϊ�
				switch (atoi(&aData[x * 2]))
				{
				case 0:		//�g�p���Ȃ�
				default:
					aBlock[y][x] = BLOCKTYPE_NONE;
					break;

				case 1:		//��
					aBlock[y][x] = BLOCKTYPE_BLUE;
					break;

				case 2:		//���F
					aBlock[y][x] = BLOCKTYPE_LIGHT_BLUE;
					break;

				case 3:		//�̒ʂ蔲����
					aBlock[y][x] = BLOCKTYPE_B_THROUGH;
					break;

				case 4:		//���F�̒ʂ蔲����
					aBlock[y][x] = BLOCKTYPE_L_THROUGH;
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

	for (int y = 0; y < MAX_Y_BLOCK; y++)
	{
		float fPosY = (SCREEN_HEIGHT / MAX_Y_BLOCK / 2.0f) + (y * SCREEN_HEIGHT / MAX_Y_BLOCK);

		for (int x = 0; x < MAX_X_BLOCK; x++)
		{
			switch (aBlock[y][x])
			{
			case BLOCKTYPE_NONE:		//�g�p���Ȃ�
			default:
				break;

			case BLOCKTYPE_BLUE:			//��
			case BLOCKTYPE_LIGHT_BLUE:		//���F
			case BLOCKTYPE_B_THROUGH:		//�̒ʂ蔲����
			case BLOCKTYPE_L_THROUGH:		//���F�̒ʂ蔲����

				float fPosX = (SCREEN_WIDTH / MAX_X_BLOCK / 2.0f) + (x * SCREEN_WIDTH / MAX_X_BLOCK);

				s_aBlock[nCntUse].type = aBlock[y][x];

				//�u���b�N�̐ݒ�
				SetBlock(D3DXVECTOR3(fPosX, fPosY, 0.0f), fWidth, fHeight);
				nCntUse++;

				break;
			}
		}
	}
}

//--------------------------------------------------
//������Ƃ��̓����蔻�菈��
//--------------------------------------------------
static void DownCollision(void)
{
	bool bCollision = false;

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];		//�u���b�N�̏��

		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			continue;
		}

		//�u���b�N���g�p����Ă���

		if (pBlock->type == BLOCKTYPE_B_THROUGH ||
			pBlock->type == BLOCKTYPE_L_THROUGH)
		{//�w��̎��
			Player *pPlayer = GetPlayer();		//�v���C���[�̏����擾

			float fPlayerLeft = pPlayer->pos.x - pPlayer->fWidth;
			float fPlayerRight = pPlayer->pos.x + pPlayer->fWidth;
			float fPlayerTop = pPlayer->pos.y - pPlayer->fHeight;
			float fPlayerBottom = pPlayer->pos.y;

			float fLeft = pBlock->pos.x - pBlock->fWidth;
			float fRight = pBlock->pos.x + pBlock->fWidth;
			float fTop = pBlock->pos.y - pBlock->fHeight;
			float fBottom = pBlock->pos.y + pBlock->fHeight;

			if (fPlayerRight >= fLeft && fPlayerLeft <= fRight &&
				fPlayerBottom >= fTop && fPlayerTop <= fBottom)
			{//X,Y���u���b�N�͈͓̔�
				bCollision = true;
				break;
			}

			bCollision = false;
		}
	}

	if (!bCollision)
	{//�ʂ蔲����
		for (int i = 0; i < MAX_BLOCK; i++)
		{
			Block *pBlock = &s_aBlock[i];		//�u���b�N�̏��

			if (!pBlock->bUse)
			{//�u���b�N���g�p����Ă��Ȃ�
				continue;
			}

			//�u���b�N���g�p����Ă���

			if (pBlock->type == BLOCKTYPE_B_THROUGH ||
				pBlock->type == BLOCKTYPE_L_THROUGH)
			{//�w��̎��
				pBlock->bCollision = true;
			}
		}
	}
}