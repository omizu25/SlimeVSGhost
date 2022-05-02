//--------------------------------------------------
//
// ACG���� ( effect.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "effect.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_EFFECT			(4096)		//�G�t�F�N�g�̍ő吔
#define MAX_TEX				(3)			//�e�N�X�`���̍ő吔
#define MAX_IN				(2)			//�z�����݂̍ő吔
#define MAX_STAR			(7)			//�X�^�[�̍ő吔
#define MAX_EXPLOSION		(50)		//�����̍ő吔

//--------------------------------------------------
//�G�t�F�N�g�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	D3DXVECTOR3		move;			//�ړ���
	D3DXCOLOR		col;			//�F
	float			fWidth;			//��
	float			fHeight;		//����
	int				nLife;			//����(�\������)
	EFFECTTYPE		type;			//���
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Effect;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static Effect						s_aEffect[MAX_EFFECT];		//�G�t�F�N�g�̏��

//--------------------------------------------------
//�G�t�F�N�g�̏���������
//--------------------------------------------------
void InitEffect(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�������̃N���A
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect100.jpg",
		&s_pTexture[EFFECTTYPE_IN]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect002.jpg",
		&s_pTexture[EFFECTTYPE_STAR]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect000.jpg",
		&s_pTexture[EFFECTTYPE_MOVE]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�������̃N���A
	memset(&s_aEffect[0], NULL, sizeof(s_aEffect));

	//�������̐ݒ�
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�G�t�F�N�g�̏I������
//--------------------------------------------------
void UninitEffect(void)
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
//�G�t�F�N�g�̍X�V����
//--------------------------------------------------
void UpdateEffect(void)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (!pEffect->bUse)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�
			continue;
		}

		//�G�t�F�N�g���g�p����Ă���

		pEffect->pos += pEffect->move;

		switch (pEffect->type)
		{
		case EFFECTTYPE_IN:			//�z������
			pEffect->fHeight -= 2.05f;
			break;

		case EFFECTTYPE_STAR:			//�X�^�[
		case EFFECTTYPE_EXPLOSION:		//����
			pEffect->col.r = (float)(rand() % 101) / 100.0f;
			pEffect->col.g = (float)(rand() % 101) / 100.0f;
			pEffect->col.b = (float)(rand() % 101) / 100.0f;
			pEffect->col.a = 1.0f;

			break;

		case EFFECTTYPE_MOVE:			//�ړ�

			break;

		default:
			assert(false);
			break;
		}

		pEffect->nLife--;

		if (pEffect->nLife <= 0)
		{//����������
			pEffect->bUse = false;
		}

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̍X�V
		SetMiddlepos(pVtx, pEffect->pos, pEffect->fWidth, pEffect->fHeight);

		//���_�J���[�̐ݒ�
		Setcol(pVtx, pEffect->col.r, pEffect->col.g, pEffect->col.b, pEffect->col.a);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//�G�t�F�N�g�̕`�揈��
//--------------------------------------------------
void DrawEffect(EFFECTTYPE type)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���u�����f�B���O�����H�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				//���̂܂ܑ���

	//�G�t�F�N�g�̕`��
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (s_aEffect[i].bUse && s_aEffect[i].type == type)
		{//�G�t�F�N�g���g�p����Ă���A��ނ�������

			if (type == EFFECTTYPE_EXPLOSION)
			{//�����̎�
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, s_pTexture[EFFECTTYPE_STAR]);
			}
			else
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, s_pTexture[type]);
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u(�|���S��)��
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		//��������
}

//--------------------------------------------------
//�G�t�F�N�g�̐ݒ菈��
//--------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, EFFECTTYPE type)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Effect *pEffect = &s_aEffect[i];

		if (pEffect->bUse)
		{//�G�t�F�N�g���g�p����Ă���
			continue;
		}

		//�G�t�F�N�g���g�p����Ă��Ȃ�

		pEffect->pos = pos;
		pEffect->move = move;
		pEffect->col = col;
		pEffect->fWidth = fWidth;
		pEffect->fHeight = fHeight;
		pEffect->nLife = nLife;
		pEffect->type = type;
		pEffect->bUse = true;		//�g�p���Ă����Ԃɂ���

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̍X�V
		SetMiddlepos(pVtx, pos, fWidth, fHeight);

		//���_�J���[�̐ݒ�
		Setcol(pVtx, col.r, col.g, col.b, col.a);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;		//������for���𔲂���
	}
}

//--------------------------------------------------
//�p�[�e�B�N���̐ݒ菈��
//--------------------------------------------------
void SetParticle(D3DXVECTOR3 pos, EFFECTTYPE type, bool bDirection)
{
	D3DXVECTOR3 Pos;		//�ʒu
	D3DXVECTOR3	move;		//�ړ���
	D3DXCOLOR col;			//�F
	float fDirection;		//����
	float fWidth;			//��
	float fHeight;			//����
	int nLife;				//����(�\������)

	switch (type)
	{
	case EFFECTTYPE_IN:				//�z������

		for (int i = 0; i < MAX_IN; i++)
		{
			if (bDirection)
			{//�E����
				Pos.x = (float)((rand() % 1001) - 500) / 100.0f + pos.x + 250.0f;
			}
			else
			{//������
				Pos.x = (float)((rand() % 1001) - 500) / 100.0f + pos.x - 250.0f;
			}

			Pos.y = (float)((rand() % 1601) - 800) / 100.0f + pos.y;
			Pos.z = 0.0f;

			move.x = (float)(rand() % 1501) / 1000.0f + 12.5f;
			move.y = (float)(rand() % 901) / 1000.0f + 2.0f;
			move.z = 0.0f;

			col = D3DXCOLOR(0.1f, 0.3f, 0.6f, 1.0f);

			fDirection = (float)((rand() % (int)(0.32f * 100 + 1)) - (0.32f * 100)) / 100.0f;

			fWidth = (float)(rand() % 51) / 100.0f + 5.0f;
			fHeight = (float)(rand() % 51) / 100.0f + 45.0f;

			nLife = (rand() % 11) + 7;

			if (bDirection)
			{//�E����
				//�G�t�F�N�g�̐ݒ�
				SetEffect(Pos, D3DXVECTOR3(sinf(fDirection + -1.59f) * move.x,
					cosf(fDirection + -1.59f) * move.y, 0.0f),
					col, fWidth, fHeight, nLife, type);
			}
			else
			{//������
				//�G�t�F�N�g�̐ݒ�
				SetEffect(Pos, D3DXVECTOR3(sinf(fDirection + 1.59f) * move.x,
					cosf(fDirection + 1.59f) * move.y, 0.0f),
					col, fWidth, fHeight, nLife, type);
			}
		}

		break;

	case EFFECTTYPE_STAR:			//�X�^�[

		for (int i = 0; i < MAX_STAR; i++)
		{
			Pos.x = (float)(rand() % 501) / 100.0f + pos.x;
			Pos.y = (float)((rand() % 1601) - 800) / 30.0f + pos.y;
			Pos.z = 0.0f;

			move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			col.r = (float)(rand() % 101) / 100.0f;
			col.g = (float)(rand() % 101) / 100.0f;
			col.b = (float)(rand() % 101) / 100.0f;
			col.a = 1.0f;

			fDirection = (float)((rand() % (int)(0.32f * 100 + 1)) - (0.32f * 100)) / 100.0f;

			fWidth = (float)(rand() % 51) / 100.0f + 15.0f;
			fHeight = fWidth;

			nLife = (rand() % 11) + 10;

			//�G�t�F�N�g�̐ݒ�
			SetEffect(Pos, move, col, fWidth, fHeight, nLife, type);
		}

		break;

	case EFFECTTYPE_EXPLOSION:		//����

		for (int i = 0; i < MAX_EXPLOSION; i++)
		{
			Pos.x = (float)((rand() % 2001) - 1000) / 100.0f + pos.x;
			Pos.y = (float)((rand() % 2001) - 1000) / 100.0f + pos.y;
			Pos.z = 0.0f;

			move.x = (float)(rand() % (2001)) / 1000.0f + 0.5f;
			move.y = (float)(rand() % (2001)) / 1000.0f + 0.5f;
			move.z = 0.0f;

			col.r = (float)(rand() % 101) / 100.0f;
			col.g = (float)(rand() % 101) / 100.0f;
			col.b = (float)(rand() % 101) / 100.0f;
			col.a = 1.0f;

			fDirection = (float)((rand() % (int)(D3DX_PI * 2.0f * 100 + 1)) - (D3DX_PI * 100)) / 100.0f;

			fWidth = (float)(rand() % 1001) / 100.0f + 7.5f;
			fHeight = fWidth;

			nLife = (rand() % 31) + 50;

			//�G�t�F�N�g�̐ݒ�
			SetEffect(Pos, D3DXVECTOR3(sinf(fDirection) * move.x, cosf(fDirection) * move.y, 0.0f),
				col, fWidth, fHeight, nLife, type);
		}

		break;

	case EFFECTTYPE_MOVE:			//�ړ�

		//�ړ��̓p�[�e�B�N���ł͂Ȃ��̂ŗ��Ă͂����Ȃ�

	default:
		assert(false);
		break;
	}
}