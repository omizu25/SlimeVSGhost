//--------------------------------------------------
//
// ACG���� ( ranking.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "colon.h"
#include "fade.h"
#include "input.h"
#include "number.h"
#include "ranking.h"
#include "setup.h"
#include "time.h"

#include <stdio.h>
#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define FILE_NAME			"data/Ranking.txt"		//�����L���O�̃p�X
#define MAX_RANKING					(5)				//�����L���O�̍ő吔
#define MAX_DATA			(MAX_RANKING + 1)		//�f�[�^(��)�̍ő吔
#define NUMBER_WIDTH				(60.0f)			//���̕�
#define NUMBER_HEIGHT				(100.0f)		//���̍���
#define WIDTH_INTERVAL				(40.0f)			//���̊Ԋu
#define HEIGHT_INTERVAL				(20.0f)			//�����̊Ԋu

//--------------------------------------------------
//�����L���O�̏��(�_�ŋ)���`
//--------------------------------------------------
typedef enum
{
	RANKINGSTATE_IN = 0,		//������
	RANKINGSTATE_OUT,			//�����Ȃ�
	RANKINGSTATE_MAX
}RANKINGSTATE;

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�̃|�C���^
static int							s_aRanking[MAX_RANKING];		//�����L���O�̏��
static D3DXVECTOR3					s_pos;							//�ʒu
static RANKINGSTATE					s_state;						//���
static int							s_nRankingUpdate;				//�X�V�����NNo.

//--------------------------------------------------
//�����L���O�̏���������
//--------------------------------------------------
void InitRanking(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, pos, fWidth, fHeight);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 1.0f, 1.0f, 0.5f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//���̏���������
	InitNumber();

	//�R�����̏���������
	InitColon();

	//�ʒu������������
	s_pos = D3DXVECTOR3(800.0f, 100.0f, 0.0f);

	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		int nTime = s_aRanking[i];
		int aNumber[MAX_TIME];
		float fHeightInterval = (NUMBER_HEIGHT * i) + (HEIGHT_INTERVAL * i);

		for (int j = 0; j < MAX_TIME; j++)
		{//�P�����ɕ�����
			aNumber[j] = nTime % 10;
			nTime /= 10;

			float fWidthInterval = (NUMBER_WIDTH * j) + (WIDTH_INTERVAL * (j / 2));

			//���̐ݒ菈��
			SetNumber(s_pos + D3DXVECTOR3(-fWidthInterval, fHeightInterval, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[j], j);
			
			if (j == 2 || j == 4)
			{
				//�R�����̐ݒ菈��
				SetColon(s_pos + D3DXVECTOR3(-fWidthInterval + WIDTH_INTERVAL, fHeightInterval, 0.0f), WIDTH_INTERVAL, NUMBER_HEIGHT * 0.5f);
			}
		}
	}
}

//--------------------------------------------------
//�����L���O�̏I������
//--------------------------------------------------
void UninitRanking(void)
{
	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	//���̏I������
	UninitNumber();

	//�R�����̏I������
	UninitColon();
}

//--------------------------------------------------
//�����L���O�̍X�V����
//--------------------------------------------------
void UpdateRanking(void)
{	
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		//�t�F�[�h�ݒ�
		SetFade(MODE_TITLE);
	}

	//if (s_nRankingUpdate != -1)
	//{//�V�����X�R�A�����鎞
	//	if (s_state == RANKINGSTATE_IN)
	//	{//�t�F�[�h�C�����
	//		s_aRanking[s_nRankingUpdate].col.a -= 0.025f;		//�|���S���𓧖��ɂ��Ă���

	//		if (s_aRanking[s_nRankingUpdate].col.a <= 0.0f)
	//		{
	//			s_aRanking[s_nRankingUpdate].col.a = 0.0f;
	//			s_state = RANKINGSTATE_OUT;		//�t�F�[�h�A�E�g��Ԃ�
	//		}
	//	}
	//	else if (s_state == RANKINGSTATE_OUT)
	//	{//�t�F�[�h�A�E�g���
	//		s_aRanking[s_nRankingUpdate].col.a += 0.025f;		//�|���S����s�����ɂ��Ă���

	//		if (s_aRanking[s_nRankingUpdate].col.a >= 1.0f)
	//		{
	//			s_aRanking[s_nRankingUpdate].col.a = 1.0f;
	//			s_state = RANKINGSTATE_IN;		//�t�F�[�h�C����Ԃ�
	//		}
	//	}

	//	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//	//���_�������b�N���A���_���ւ̃|�C���^���擾
	//	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	//�Y���̈ʒu�܂Ői�߂�
	//	pVtx += (s_nRankingUpdate *  MAX_TIME * 4);

	//	for (int i = 0; i < MAX_RANKING; i++)
	//	{//�W���܂�
	//		//���_�J���[�̐ݒ�
	//		pVtx[0].col = s_aRanking[s_nRankingUpdate].col;
	//		pVtx[1].col = s_aRanking[s_nRankingUpdate].col;
	//		pVtx[2].col = s_aRanking[s_nRankingUpdate].col;
	//		pVtx[3].col = s_aRanking[s_nRankingUpdate].col;

	//		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	//	}
	//	//���_�o�b�t�@���A�����b�N����
	//	s_pVtxBuffRankScore->Unlock();
	//}

	//���̍X�V����
	UpdateNumber();

	//�R�����̍X�V����
	UpdateColon();
}

//--------------------------------------------------
//�����L���O�̕`�揈��
//--------------------------------------------------
void DrawRanking(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//���̕`�揈��
	DrawNumber();

	//�R�����̕`�揈��
	DrawColon();
}

//--------------------------------------------------
//�����L���O�̃��Z�b�g
//--------------------------------------------------
void ResetRanking(void)
{
	FILE *pRankingFile;			//�t�@�C���|�C���^��錾

	s_nRankingUpdate = -1;		//�X�V�����NNo.�̏�����

	//�t�@�C�����J��
	pRankingFile = fopen(FILE_NAME, "r");

	if (pRankingFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int i = 0; i < MAX_RANKING; i++)
		{//�����L���O�̓ǂݍ���
			fscanf(pRankingFile, "%d", &s_aRanking[i]);
		}

		//�t�@�C�������
		fclose(pRankingFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		//�t�F�[�h�̐ݒ�
		SetFade(MODE_TITLE);

		assert(false);
	}
}

//--------------------------------------------------
//�����L���O�̃Z�[�u
//--------------------------------------------------
void SaveRanking(void)
{
	FILE *pRankingFile;			//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pRankingFile = fopen(FILE_NAME, "w");

	if (pRankingFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int i = 0; i < MAX_RANKING; i++)
		{//�����L���O�̏�������
			fprintf(pRankingFile, "%d \n\n", s_aRanking[i]);
		}

		//�t�@�C�������
		fclose(pRankingFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		//�t�F�[�h�̐ݒ�
		SetFade(MODE_TITLE);

		assert(false);
	}
}

//--------------------------------------------------
//�����L���O�̐ݒ菈��
//--------------------------------------------------
void SetRanking(int nRankTime)
{
	int nSwap;					//����ւ��p 
	int aData[MAX_DATA];		//�f�[�^(��)

	for (int i = 0; i < MAX_RANKING; i++)
	{//���
		aData[i] = s_aRanking[i];
	}

	//����̃^�C��
	aData[MAX_RANKING] = nRankTime;

	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		//�l���ŏ��l�ł��I�I�I
		nSwap = aData[i];

		for (int j = i + 1; j < MAX_DATA; j++)
		{
			if (nSwap > aData[j])
			{//���肪��������������
				nSwap = aData[j];
			}
		}

		for (int j = i + 1; j < MAX_DATA; j++)
		{
			if (nSwap == aData[j])
			{//�ŏ��l��T���Č��
				aData[j] = aData[i];
				break;
			}
		}

		//�����ŏ��l���I�I�I
		aData[i] = nSwap;
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//���
		s_aRanking[i] = aData[i];
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		s_nRankingUpdate = -1;

		if (nRankTime == s_aRanking[i])
		{//�w��̃X�R�A��T��
			s_nRankingUpdate = i;
			break;
		}
	}
}