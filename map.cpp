//--------------------------------------------------
//
// �A�N�V�����Q�[������ ( map.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"
#include "map.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_X_MAP				(32)			//�}�b�v�̉��̍ő吔
#define MAX_Y_MAP				(18)			//�}�b�v�̏c�̍ő吔
#define FILE_DATA				(1024)			//�t�@�C���̃f�[�^��
#define MAP_FILE		"data/test_map.txt"		//�}�b�v�̃e�L�X�g��

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAPTYPE_MAX] = { NULL, NULL };		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;								//���_�o�b�t�@�̃|�C���^
static Map							s_aMap[MAX_Y_MAP][MAX_X_MAP];					//�}�b�v�̏��

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStructMap(int nCntY, int nCntX);
static void InitMapType();

//--------------------------------------------------
//�}�b�v�̏���������
//--------------------------------------------------
void InitMap(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block003.png",
		&s_pTexture[MAPTYPE_PURPLE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block002.png",
		&s_pTexture[MAPTYPE_BROWN]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\block004.png",
		&s_pTexture[MAXTYPE_BLUE]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Y_MAP * MAX_X_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�}�b�v�̃^�C�v�̏���������
	InitMapType();

	for (int nCntY = 0; nCntY < MAX_Y_MAP; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_X_MAP; nCntX++)
		{
			//�}�b�v�̍\���̂̏���������
			InitStructMap(nCntY, nCntX);

			//���_���W�̐ݒ菈��
			SetMiddlepos(pVtx, s_aMap[nCntY][nCntX].pos, SCREEN_WIDTH / MAX_X_MAP / 2.0f, SCREEN_HEIGHT / MAX_Y_MAP / 2.0f);

			//rhw�̐ݒ菈��
			Setrhw(pVtx);

			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ菈��
			Settex(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

			pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�}�b�v�̏I������
//--------------------------------------------------
void UninitMap(void)
{
	for (int nCnt = 0; nCnt < MAPTYPE_MAX; nCnt++)
	{
		if (s_pTexture[nCnt] != NULL)
		{//�}�b�v�̃e�N�X�`���̔j��
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//�}�b�v�̒��_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//�}�b�v�̍X�V����
//--------------------------------------------------
void UpdateMap(void)
{
	
}

//--------------------------------------------------
//�}�b�v�̕`�揈��
//--------------------------------------------------
void DrawMap(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntType = 0; nCntType < MAPTYPE_MAX; nCntType++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[nCntType]);

		for (int nCntY = 0; nCntY < MAX_Y_MAP; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAX_X_MAP; nCntX++)
			{
				if (s_aMap[nCntY][nCntX].type == nCntType)
				{
					//�|���S���̕`��
					pDevice->DrawPrimitive(
						D3DPT_TRIANGLESTRIP,						//�v���~�e�B�u�̎��
						nCntX * 4 + (nCntY * MAX_X_MAP * 4),		//�`�悷��ŏ��̒��_�C���f�b�N�X
						2);											//�v���~�e�B�u(�|���S��)��
				}
			}
		}
	}
}

//--------------------------------------------------
//�}�b�v�̎擾����
//--------------------------------------------------
Map *GetMap(void)
{
	return &s_aMap[0][0];
}

//--------------------------------------------------
//�}�b�v�̍\���̂̏���������
//--------------------------------------------------
static void InitStructMap(int nCntY, int nCntX) 
{
	//�ʒu��������
	s_aMap[nCntY][nCntX].pos.x = (SCREEN_WIDTH / MAX_X_MAP / 2.0f) + (nCntX * SCREEN_WIDTH / MAX_X_MAP);
	s_aMap[nCntY][nCntX].pos.y = (SCREEN_HEIGHT / MAX_Y_MAP / 2.0f) + (nCntY * SCREEN_HEIGHT / MAX_Y_MAP);
	s_aMap[nCntY][nCntX].pos.z = 0.0f;

	//�ړ��ʂ�������
	s_aMap[nCntY][nCntX].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
//�}�b�v�̃^�C�v�̏���������
//--------------------------------------------------
static void InitMapType()
{
	FILE *pMapFile;			//�t�@�C���|�C���^��錾
	char adata[FILE_DATA];

	//�t�@�C�����J��
	pMapFile = fopen(MAP_FILE, "r");

	if (pMapFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int nCntY = 0; nCntY < MAX_Y_MAP; nCntY++)
		{
			//��s�ǂݍ���
			fgets(adata, FILE_DATA, pMapFile);

			for (int nCntX = 0; nCntX < MAX_X_MAP; nCntX++)
			{
				//�ϊ�
				switch (atoi(&adata[nCntX * 2]))
				{
				case 0:		//��
				default:
					s_aMap[nCntY][nCntX].type = MAPTYPE_PURPLE;
					break;

				case 1:		//���F
					s_aMap[nCntY][nCntX].type = MAPTYPE_BROWN;
					break;
				case 2:		//��
					s_aMap[nCntY][nCntX].type = MAXTYPE_BLUE;
					break;
				}
			}
		}

		//�t�@�C�������
		fclose(pMapFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		assert(false);

		//�t�@�C�������
		fclose(pMapFile);
	}
}