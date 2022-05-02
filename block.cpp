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
static bool							s_bThrough;					//�u���b�N���ʂ蔲���邩 [false :�ʂ蔲���Ȃ� true : �ʂ蔲����]
static bool							s_bTexUse;					//�e�N�X�`���̎g�p���邩�ǂ���
static bool							s_bCollisionUse;			//�����蔻��𔻒f���邩�ǂ���

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStructBlock(int i);
static void InitBlockType(void);
static void DownCollisionBlock(void);

//--------------------------------------------------
//�u���b�N�̏���������
//--------------------------------------------------
void InitBlock(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_bThrough = false;
	s_bTexUse = true;
	s_bCollisionUse = false;

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
		//�u���b�N�̉�����Ƃ��̓����蔻��
		DownCollisionBlock();
	}

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
					bCollision = true;
				}
			}
			else if ((pPosOld->y - fHeight >= fBottom) && (pPos->y - fHeight < fBottom))
			{//�u���b�N�̏ォ�牺
				if (pBlock->type == BLOCKTYPE_BLUE || pBlock->type == BLOCKTYPE_LIGHT_BLUE)
				{//�w��̎��
					pPos->y = fBottom + fHeight;
					pMove->y *= -REFLECT_BOUND;
					bCollision = true;
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
					bCollision = true;
				}
				else if ((pPosOld->x - fWidth >= fRight) && (pPos->x - fWidth < fRight))
				{//�u���b�N�̉E�[
					pPos->x = fRight + fWidth;
					pMove->x = 0.0f;
					bCollision = true;
				}
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
//�u���b�N�̏�[�̓����蔻�菈��
//--------------------------------------------------
void CollisionTopBlock(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	int nCntTop = 0;

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &s_aBlock[i];		//�u���b�N�̏��

		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ�
			continue;
		}

		//�u���b�N���g�p����Ă���
		Player *pPlayer = GetPlayer();		//�v���C���[�̏����擾

		if (pPlayer->pos.x + pPlayer->fWidth >= (pBlock->pos.x - pBlock->fWidth) &&
			pPlayer->pos.x - pPlayer->fWidth <= (pBlock->pos.x + pBlock->fWidth) &&
			pPlayer->pos.y >= (pBlock->pos.y - pBlock->fHeight) &&
			pPlayer->pos.y - pPlayer->fHeight <= (pBlock->pos.y + pBlock->fHeight))
		{//X,Y���u���b�N�͈͓̔�
			if (pBlock->type == BLOCKTYPE_B_THROUGH ||
				pBlock->type == BLOCKTYPE_L_THROUGH)
			{//�ʂ蔲����u���b�N��������
				s_bThrough = true;
			}
			else if (pBlock->type == BLOCKTYPE_BLUE ||
				pBlock->type == BLOCKTYPE_LIGHT_BLUE)
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

			if (pBlock->type == BLOCKTYPE_B_THROUGH ||
				pBlock->type == BLOCKTYPE_L_THROUGH)
			{//�w��̎��
				pBlock->bCollision = false;
			}
		}
	}
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

	s_aBlock[i].bCollision = false;
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

				case 1:		//��
					aBlock[i][j] = BLOCKTYPE_BLUE;
					break;

				case 2:		//���F
					aBlock[i][j] = BLOCKTYPE_LIGHT_BLUE;
					break;

				case 3:		//�̒ʂ蔲����
					aBlock[i][j] = BLOCKTYPE_B_THROUGH;
					break;

				case 4:		//���F�̒ʂ蔲����
					aBlock[i][j] = BLOCKTYPE_L_THROUGH;
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
				break;

			case BLOCKTYPE_BLUE:			//��
			case BLOCKTYPE_LIGHT_BLUE:		//���F
			case BLOCKTYPE_B_THROUGH:		//�̒ʂ蔲����
			case BLOCKTYPE_L_THROUGH:		//���F�̒ʂ蔲����

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

//--------------------------------------------------
//�u���b�N�̏�[�̓����蔻�菈��
//--------------------------------------------------
static void DownCollisionBlock(void)
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

			if (pPlayer->pos.x + pPlayer->fWidth >= (pBlock->pos.x - pBlock->fWidth) &&
				pPlayer->pos.x - pPlayer->fWidth <= (pBlock->pos.x + pBlock->fWidth) &&
				pPlayer->pos.y >= (pBlock->pos.y - pBlock->fHeight) &&
				pPlayer->pos.y - pPlayer->fHeight <= (pBlock->pos.y + pBlock->fHeight))
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