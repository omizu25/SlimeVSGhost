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
#define MAX_MOVE					(10.0f)			//�ړ��ʂ̍ő�l
#define RANKING_WIDTH				(600.0f)		//�����N�̕�
#define RANKING_HEIGHT				(80.0f)			//�����N�̍���
#define END_WIDTH					(800.0f)		//�I���̕�
#define END_HEIGHT					(60.0f)			//�I���̍���
#define RANK_WIDTH					(80.0f)			//�����N�̕�
#define RANK_HEIGHT					(80.0f)			//�����N�̍���
#define NUMBER_WIDTH				(60.0f)			//���̕�
#define NUMBER_HEIGHT				(80.0f)			//���̍���
#define NEW_WIDTH					(160.0f)		//�V�����z�̕�
#define NEW_HEIGHT					(80.0f)			//�V�����z�̍���
#define WIDTH_INTERVAL				(40.0f)			//���̊Ԋu
#define HEIGHT_INTERVAL				(20.0f)			//�����̊Ԋu

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateMove(void);
static bool UpdateWherePos(void);
static void UpdatePos(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;		//�w�i�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureEnd = NULL;		//�I���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffEnd = NULL;		//�I���̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureRank = NULL;		//�ʂ̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffRank = NULL;		//�ʂ̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureNew = NULL;		//�V�����z�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffNew = NULL;		//�V�����z�̒��_�o�b�t�@�̃|�C���^
static int							s_aScore[MAX_RANKING];		//�����L���O�̃X�R�A
static D3DXVECTOR3					s_pos;						//�ʒu
static float						s_fWidth[MAX_RANKING];		//�V�����z�̈ʒu
static D3DXVECTOR3					s_move[MAX_RANKING];		//�ړ���
static int							s_nRankUpdate;				//�X�V�����NNo.

//--------------------------------------------------
//�����L���O�̏���������
//--------------------------------------------------
void InitRanking(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\ranking001.png",
		&s_pTexture);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title009.png",
		&s_pTextureEnd);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\ranking000.png",
		&s_pTextureRank);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\ranking002.png",
		&s_pTextureNew);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffEnd,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRank,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffNew,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	/* ���w�i�� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, pos, fWidth, fHeight);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 0.85f, 0.85f, 0.85f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBG->Unlock();

	/* ��ranking�� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, D3DXVECTOR3(fWidth, RANKING_HEIGHT * 0.75f, 0.0f), RANKING_WIDTH * 0.5f, RANKING_HEIGHT * 0.5f);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	/* ���I��聫 */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthEnd = END_WIDTH * 0.5f;
	float fHeightEnd = END_HEIGHT * 0.5f;
	D3DXVECTOR3 posEnd = D3DXVECTOR3(pos.x, SCREEN_HEIGHT - END_HEIGHT * 0.75f, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, posEnd, fWidthEnd, fHeightEnd);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffEnd->Unlock();

	/* �����֘A�� */

	//���̏���������
	InitNumber();

	//�R�����̏���������
	InitColon();

	//s_nRankUpdate = 1;

	//�ʒu������������
	s_pos = D3DXVECTOR3(2000.0f, 175.0f, 0.0f);

	for (int i = 0; i < MAX_RANKING; i++)
	{
		s_move[i] = D3DXVECTOR3(-MAX_MOVE, 0.0f, 0.0f);
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		int nTime = s_aScore[i];
		int aNumber[MAX_TIME];
		float fHeightInterval = (NUMBER_HEIGHT * i) + (HEIGHT_INTERVAL * i);
		float fWidthInterval;
		s_fWidth[i] = s_pos.x - (WIDTH_INTERVAL * i);

		for (int j = 0; j < MAX_TIME; j++)
		{//�P�����ɕ�����
			aNumber[j] = nTime % 10;
			nTime /= 10;

			fWidthInterval = (NUMBER_WIDTH * j) + (WIDTH_INTERVAL * (j / 2)) + (WIDTH_INTERVAL * i);

			//���̐ݒ菈��
			SetNumber(s_pos + D3DXVECTOR3(-fWidthInterval, fHeightInterval, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[j], j, i);

			if ((j % 2 == 0) && (j != 0))
			{//�Q�̔{��
				//�R�����̐ݒ菈��
				SetColon(s_pos + D3DXVECTOR3(-fWidthInterval + WIDTH_INTERVAL, fHeightInterval, 0.0f), WIDTH_INTERVAL, NUMBER_HEIGHT * 0.5f, i);
			}
		}

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		D3DXVECTOR3 posRank = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, s_pos.y, 0.0f);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, posRank + D3DXVECTOR3(0.0f, fHeightInterval, 0.0f), RANK_WIDTH * 0.5f, RANK_HEIGHT * 0.5f);

		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̏���������
		Inittex(pVtx);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffRank->Unlock();

		//���̐ݒ菈��
		SetNumber(posRank + D3DXVECTOR3(-RANK_WIDTH, fHeightInterval, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, i + 1, 0, -2);
	}

	/* ��New�� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffNew->Lock(0, 0, (void**)&pVtx, 0);

	float fHeightNew;

	if (s_nRankUpdate == -1)
	{//New���Ȃ�
		//�ʒu������������
		fHeightNew = 0.0f;
	}
	else
	{//New������
		//�ʒu������������
		fHeightNew = (NUMBER_HEIGHT * s_nRankUpdate) + (HEIGHT_INTERVAL * s_nRankUpdate);
	}

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, s_pos + D3DXVECTOR3(NEW_WIDTH, fHeightNew, 0.0f), NEW_WIDTH * 0.5f, NEW_HEIGHT * 0.5f);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffNew->Unlock();
}

//--------------------------------------------------
//�����L���O�̏I������
//--------------------------------------------------
void UninitRanking(void)
{
	if (s_pVtxBuffBG != NULL)
	{//�w�i�̒��_�o�b�t�@�̔j��
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

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

	if (s_pTextureEnd != NULL)
	{//�I���̃e�N�X�`���̔j��
		s_pTextureEnd->Release();
		s_pTextureEnd = NULL;
	}

	if (s_pVtxBuffEnd != NULL)
	{//�I���̒��_�o�b�t�@�̔j��
		s_pVtxBuffEnd->Release();
		s_pVtxBuffEnd = NULL;
	}

	if (s_pTextureRank != NULL)
	{//�ʂ̃e�N�X�`���̔j��
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	if (s_pVtxBuffRank != NULL)
	{//�ʂ̒��_�o�b�t�@�̔j��
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	if (s_pTextureNew != NULL)
	{//�V�����z�̃e�N�X�`���̔j��
		s_pTextureNew->Release();
		s_pTextureNew = NULL;
	}

	if (s_pVtxBuffNew != NULL)
	{//�V�����z�̒��_�o�b�t�@�̔j��
		s_pVtxBuffNew->Release();
		s_pVtxBuffNew = NULL;
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
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_B) ||
		GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���

		//pos���ǂ��ɂ���̂�����
		if (UpdateWherePos())
		{//pos������̈ʒu�ɂ���
			//�t�F�[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
		else
		{//pos������̈ʒu�ɂ��Ȃ�
			//���_����
			UpdatePos();
		}
	}

	//�ړ�����
	UpdateMove();

	//���_����
	UpdatePos();

	//���̃����N����
	RankNumber(s_nRankUpdate);

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
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffEnd, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureEnd);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureRank);

	for (int i = 0; i < MAX_RANKING; i++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffNew, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureNew);

	if (s_nRankUpdate != -1)
	{//New������
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}

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

	s_nRankUpdate = -1;		//�X�V�����NNo.�̏�����

	//�t�@�C�����J��
	pRankingFile = fopen(FILE_NAME, "r");

	if (pRankingFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int i = 0; i < MAX_RANKING; i++)
		{//�����L���O�̓ǂݍ���
			fscanf(pRankingFile, "%d", &s_aScore[i]);
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
			fprintf(pRankingFile, "%d \n\n", s_aScore[i]);
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
		aData[i] = s_aScore[i];
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
		s_aScore[i] = aData[i];
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		s_nRankUpdate = -1;

		if (nRankTime == s_aScore[i])
		{//�w��̃X�R�A��T��
			s_nRankUpdate = i;
			break;
		}
	}
}

//--------------------------------------------------
//�����L���O�̍X�V�̏���������
//--------------------------------------------------
void InitRankUpdate(void)
{
	s_nRankUpdate = -1;
}

//--------------------------------------------------
//�ړ�����
//--------------------------------------------------
static void UpdateMove(void)
{
	//�ʒu���X�V
	s_pos.x += s_move[s_nRankUpdate].x;

	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		//�ʒu�̍X�V
		s_fWidth[i] += s_move[i].x;

		if (s_fWidth[i] <= 900.0f)
		{//����̒l���z����
			s_fWidth[i] = 900.0f;
			s_move[i].x = 0.0f;
		}
	}
}

//--------------------------------------------------
//pos���ǂ��ɂ���̂�����
//--------------------------------------------------
static bool UpdateWherePos(void)
{
	bool bWherePos = true;

	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		if (s_fWidth[i] > 900.0f)
		{//����̒l���z���ĂȂ�
			bWherePos = false;
		}

		s_move[i].x = -(s_fWidth[i] - 900.0f);
	}

	if (!bWherePos)
	{
		//�ʒu���X�V
		s_pos.x = 900.0f;
	}

	return bWherePos;
}

//--------------------------------------------------
//���_����
//--------------------------------------------------
static void UpdatePos(void)
{
	for (int i = 0; i < MAX_RANKING; i++)
	{//�T�ʂ܂�
		int nTime = s_aScore[i];
		int aNumber[MAX_TIME];
		float fHeightInterval = (NUMBER_HEIGHT * i) + (HEIGHT_INTERVAL * i);
		float fWidthInterval;

		for (int j = 0; j < MAX_TIME; j++)
		{//�P�����ɕ�����
			aNumber[j] = nTime % 10;
			nTime /= 10;

			fWidthInterval = (NUMBER_WIDTH * j) + (WIDTH_INTERVAL * (j / 2));

			//���̒��_����
			PosNumber(D3DXVECTOR3(s_fWidth[i] - fWidthInterval, s_pos.y + fHeightInterval, 0.0f), aNumber[j], j, i);

			if ((j % 2 == 0) && (j != 0))
			{//�Q�̔{��
				//�R�����̈ړ�����
				MoveColon(D3DXVECTOR3(s_move[i].x * 0.5f, s_move[i].y, 0.0f), i);
			}
		}
	}

	/* ��New�� */

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffNew->Lock(0, 0, (void**)&pVtx, 0);

	float fHeightNew = (NUMBER_HEIGHT * s_nRankUpdate) + (HEIGHT_INTERVAL * s_nRankUpdate);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, s_pos + D3DXVECTOR3(NEW_WIDTH - (WIDTH_INTERVAL * s_nRankUpdate), fHeightNew, 0.0f), NEW_WIDTH * 0.5f, NEW_HEIGHT * 0.5f);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffNew->Unlock();
}