//--------------------------------------------------
//
// ACG���� ( gauge.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "setup.h"
#include "gauge.h"

#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_GAUGE			(256)			//�Q�[�W�̍ő吔
#define GAUGE_WIDTH			(3.0f)			//�Q�[�W�̕�

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static Gauge						s_Gauge[MAX_GAUGE];		//�Q�[�W�̏��

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
static void InitStruct(Gauge *pGauge);
static void SetPos(VERTEX_2D *pVtx, Gauge *pGauge);
static void SetCol(VERTEX_2D *pVtx, Gauge *pGauge);
static void UpdateDecrease(void);

//--------------------------------------------------
//�Q�[�W�̏���������
//--------------------------------------------------
void InitGauge(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_GAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_Gauge[i];

		//�\���̂̏���������
		InitStruct(pGauge);

		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i��
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�Q�[�W�̏I������
//--------------------------------------------------
void UninitGauge(void)
{
	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//�Q�[�W�̍X�V����
//--------------------------------------------------
void UpdateGauge(void)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_Gauge[i];

		if (!pGauge->bUse)
		{//�Q�[�W���g�p����Ă��Ȃ�
			continue;
		}

		//�Q�[�W���g�p����Ă���

		//�����ʂ̏���
		UpdateDecrease();

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetPos(pVtx, pGauge);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//�Q�[�W�̕`�揈��
//--------------------------------------------------
void DrawGauge(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < MAX_GAUGE; i++)
	{
		if (s_Gauge[i].bUse)
		{//�Q�[�W���g�p����Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u(�|���S��)��
		}
	}
}

//--------------------------------------------------
//�Q�[�W�̐ݒ菈��
//--------------------------------------------------
void SetGauge(GAUGEUSE use, int nLife)
{
	for (int i = 0; i < GAUGETYPE_MAX; i++)
	{
		for (int j = 0; j < MAX_GAUGE; j++)
		{
			Gauge *pGauge = &s_Gauge[j];

			if (pGauge->bUse)
			{//�Q�[�W���g�p����Ă���
				continue;
			}

			//�Q�[�W���g�p����Ă��Ȃ�

			pGauge->use = use;
			pGauge->type = (GAUGETYPE)i;
			pGauge->bUse = true;
			pGauge->nCounter = 0;
			pGauge->fWidth = GAUGE_WIDTH * nLife;

			float fWidth = (SCREEN_WIDTH / MAX_X_BLOCK);
			float fHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK) * 0.5f;

			pGauge->fHeight = fHeight;

			switch (use)
			{
			case GAUGEUSE_PLAYER:		//�v���C���[

				pGauge->pos = D3DXVECTOR3(fWidth * 2.5f, fHeight * 2.0f, 0.0f);

				break;

			case GAUGEUSE_BOY:			//�j�̎q (����)
			case GAUGEUSE_GIRL:			//���̎q (����)

				pGauge->pos = D3DXVECTOR3(fWidth * 21.5f, fHeight * 2.0f, 0.0f);

				break;

			default:
				assert(false);
				break;
			}

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

			//���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (j * 4);		//�Y���̈ʒu�܂Ői�߂�

			//���_���W�̐ݒ菈��
			SetPos(pVtx, pGauge);
			
			//�F�̐ݒ菈��
			SetCol(pVtx, pGauge);

			//���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();

			break;		//������for���𔲂���
		}
	}
}

//--------------------------------------------------
//�Q�[�W�̌��Z����
//--------------------------------------------------
void SubGauge(GAUGEUSE use, int nLife)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pGauge = &s_Gauge[i];

		if (!pGauge->bUse || pGauge->type != GAUGETYPE_REMAINING || pGauge->use != use)
		{//�Q�[�W���g�p����Ă��Ȃ��A�w��̎�ނ���Ȃ��A�g�p�҂���Ȃ�
			continue;
		}

		//�Q�[�W���g�p����Ă���

		pGauge->fWidth = GAUGE_WIDTH * nLife;

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetPos(pVtx, pGauge);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;		//������for���𔲂���
	}
}

//--------------------------------------------------
//���_���W�̐ݒ菈�� (�E�ɒ��S)
//--------------------------------------------------
static void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(   0.0f, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(   0.0f,  fHeight, 0.0f);
}

//--------------------------------------------------
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(Gauge *pGauge)
{
	pGauge->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pGauge->use = GAUGEUSE_PLAYER;
	pGauge->type = GAUGETYPE_REMAINING;
	pGauge->fWidth = 0.0f;
	pGauge->fHeight = 0.0f;
	pGauge->nCounter = 0;
	pGauge->bUse = false;
}

//--------------------------------------------------
//���_���W�̐ݒ菈��
//--------------------------------------------------
static void SetPos(VERTEX_2D *pVtx, Gauge *pGauge)
{
	switch (pGauge->use)
	{
	case GAUGEUSE_PLAYER:		//�v���C���[
	case GAUGEUSE_GIRL:			//���̎q (����)

		//���_���W�̐ݒ菈��
		SetLeftpos(pVtx, pGauge->pos, pGauge->fWidth, pGauge->fHeight);

		break;

	case GAUGEUSE_BOY:			//�j�̎q (����)

		//���_���W�̐ݒ菈��
		SetRightpos(pVtx, pGauge->pos, pGauge->fWidth, pGauge->fHeight);

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//�F�̐ݒ菈��
//--------------------------------------------------
static void SetCol(VERTEX_2D *pVtx, Gauge *pGauge)
{
	switch (pGauge->type)
	{
	case GAUGETYPE_MAXIMUM:			//�ő�l

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 0.3f, 0.3f, 0.3f, 1.0f);

		break;

	case GAUGETYPE_DECREASE:		//������

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

		break;

	case GAUGETYPE_REMAINING:		//�c��

		switch (pGauge->use)
		{
		case GAUGEUSE_PLAYER:		//�v���C���[

			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 1.0f, 0.0f, 1.0f, 1.0f);

			break;

		case GAUGEUSE_BOY:			//�j�̎q (����)
		case GAUGEUSE_GIRL:			//���̎q (����)

			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

			break;

		default:
			assert(false);
			break;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//�����ʂ̏���
//--------------------------------------------------
static void UpdateDecrease(void)
{
	for (int i = 0; i < MAX_GAUGE; i++)
	{
		Gauge *pRemaining = &s_Gauge[i];

		if (!pRemaining->bUse || pRemaining->type != GAUGETYPE_REMAINING)
		{//�Q�[�W���g�p����Ă��Ȃ��A�w��̎�ނ���Ȃ�
			continue;
		}

		//�Q�[�W���g�p����Ă���

		for (int j = 0; j < MAX_GAUGE; j++)
		{
			Gauge *pDecrease = &s_Gauge[j];

			if (!pDecrease->bUse || pDecrease->type != GAUGETYPE_DECREASE || pDecrease->use != pRemaining->use)
			{//�Q�[�W���g�p����Ă��Ȃ��A�w��̎�ނ���Ȃ��A�g�p�҂��Ⴄ
				continue;
			}

			//�Q�[�W���g�p����Ă���

			if (pDecrease->fWidth == pRemaining->fWidth)
			{//��������
  				pDecrease->nCounter = 0;
				break;
			}
			
			//�����Ⴄ

			pDecrease->nCounter++;

			if (pDecrease->nCounter >= 180)
			{//�J�E���^�[����������
				float Difference = pDecrease->fWidth - pRemaining->fWidth;
				
				pDecrease->fWidth -= Difference * 0.0075f;

				if (Difference <= 0.5f)
				{
					pDecrease->nCounter = 0;
					pDecrease->fWidth = pRemaining->fWidth;
				}
			}
		}
	}
}