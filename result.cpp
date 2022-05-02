//--------------------------------------------------
//
// ACG���� ( result.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "colon.h"
#include "fade.h"
#include "input.h"
#include "number.h"
#include "result.h"
#include "setup.h"
#include "sound.h"
#include "time.h"

#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define RESULT_WIDTH		(600.0f)		//�����Ə����̕�
#define RESULT_HEIGHT		(300.0f)		//�����Ə����̍���
#define TIME_WIDTH			(400.0f)		//�^�C���̕�
#define TIME_HEIGHT			(160.0f)		//�^�C���̍���
#define NUMBER_WIDTH		(100.0f)		//���̕�
#define NUMBER_HEIGHT		(250.0f)		//���̍���
#define WIDTH_INTERVAL		(40.0f)			//���̊Ԋu
#define HEIGHT_INTERVAL		(20.0f)			//�����̊Ԋu

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTextureBG[RESULT_MAX];		//�w�i�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			//�w�i�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexture[RESULT_MAX];			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureTime = NULL;			//�^�C���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffTime = NULL;			//�^�C���̒��_�o�b�t�@�̃|�C���^
static RESULT						s_Result;						//���U���g�̏��
static int							s_nTime;						//�^�C��

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateMode(void);

//--------------------------------------------------
//���U���g�̏���������
//--------------------------------------------------
void InitResult(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�������̃N���A
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	//�������̃N���A
	memset(&s_pTextureBG[0], NULL, sizeof(s_pTextureBG));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result006.jpg",
		&s_pTextureBG[RESULT_LOSE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result007.png",
		&s_pTextureBG[RESULT_WIN]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result005.png",
		&s_pTexture[RESULT_LOSE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result004.png",
		&s_pTexture[RESULT_WIN]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result009.png",
		&s_pTextureTime);

	s_nTime = 0;

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

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTime,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	/* ���w�i�� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthBG = SCREEN_WIDTH * 0.5f;
	float fHeightBG = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 posBG = D3DXVECTOR3(fWidthBG, fHeightBG, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, posBG, fWidthBG, fHeightBG);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBG->Unlock();

	/* �������Ə����� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (s_Result == RESULT_LOSE)
	{//����
		float fWidth = RESULT_WIDTH * 0.5f;
		float fHeight = RESULT_HEIGHT * 0.5f;
		D3DXVECTOR3 pos = D3DXVECTOR3(RESULT_WIDTH * 0.6f, RESULT_HEIGHT * 0.35f, 0.0f);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, pos, fWidth, fHeight);
	}
	else
	{
		float fWidth = RESULT_WIDTH * 0.5f;
		float fHeight = RESULT_HEIGHT * 0.5f;
		D3DXVECTOR3 pos = D3DXVECTOR3(fHeightBG, RESULT_HEIGHT * 0.35f, 0.0f);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, pos, fWidth, fHeight);
	}

	//rhw�̏���������
	Initrhw(pVtx);

	if (s_Result == RESULT_WIN)
	{//����
		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		//���_�J���[�̏���������
		Initcol(pVtx);
	}

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	/* ������̃^�C���� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthTime = TIME_WIDTH * 0.5f;
	float fHeightTime = TIME_HEIGHT * 0.5f;
	D3DXVECTOR3 posTime = D3DXVECTOR3(TIME_WIDTH * 0.75f, SCREEN_HEIGHT * 0.4f, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, posTime, fWidthTime, fHeightTime);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 0.8f, 0.0f, 0.8f, 1.0f);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffTime->Unlock();

	/* �����֘A�� */

	//���̏���������
	InitNumber();

	//�R�����̏���������
	InitColon();

	//�ʒu������������
	D3DXVECTOR3 posNumber = D3DXVECTOR3(1000.0f, SCREEN_HEIGHT * 0.7f, 0.0f);

	int nTime = GetTime();
	int aNumber[MAX_TIME];

	for (int i = 0; i < MAX_TIME; i++)
	{//�P�����ɕ�����
		aNumber[i] = nTime % 10;
		nTime /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * (i / 2));

		//���̐ݒ菈��
		SetRightNumber(D3DXVECTOR3(posNumber.x - fInterval, posNumber.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, -1);

		if ((i % 2 == 0) && (i != 0))
		{//�Q�̔{��
			//�R�����̐ݒ菈��
			SetColon(D3DXVECTOR3(posNumber.x - fInterval + WIDTH_INTERVAL, posNumber.y, 0.0f), WIDTH_INTERVAL, NUMBER_HEIGHT * 0.5f, -1);
		}
	}

	switch (s_Result)
	{
	case RESULT_LOSE:		//����
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_MYSTERYMAN);
		break;

	case RESULT_WIN:		//����
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_START_BEETS);
		break;

	case RESULT_NONE:		//�܂����܂��ĂȂ�
		//break�Ȃ�
	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//���U���g�̏I������
//--------------------------------------------------
void UninitResult(void)
{
	//�T�E���h�̒�~
	StopSound();

	for (int i = 0; i < RESULT_MAX; i++)
	{
		if (s_pTextureBG[i] != NULL)
		{//�w�i�̃e�N�X�`���̔j��
			s_pTextureBG[i]->Release();
			s_pTextureBG[i] = NULL;
		}
	}

	if (s_pVtxBuffBG != NULL)
	{//�w�i�̒��_�o�b�t�@�̔j��
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	for (int i = 0; i < RESULT_MAX; i++)
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

	if (s_pTextureTime != NULL)
	{//�^�C���̃e�N�X�`���̔j��
		s_pTextureTime->Release();
		s_pTextureTime = NULL;
	}

	if (s_pVtxBuffTime != NULL)
	{//�^�C���̒��_�o�b�t�@�̔j��
		s_pVtxBuffTime->Release();
		s_pVtxBuffTime = NULL;
	}

	//���̏I������
	UninitNumber();

	//�R�����̏I������
	UninitColon();
}

//--------------------------------------------------
//���U���g�̍X�V����
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_START))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		//���[�h����
		UpdateMode();
	}

	s_nTime++;

	if (s_nTime >= 900)
	{
		if (GetFade() == FADE_NONE)
		{//�܂��t�F�[�h���ĂȂ�
			//���[�h����
			UpdateMode();
		}
	}

	//���̍X�V����
	UpdateNumber();

	//�R�����̍X�V����
	UpdateColon();
}

//--------------------------------------------------
//���U���g�̕`�揈��
//--------------------------------------------------
void DrawResult(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureBG[s_Result]);

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
	pDevice->SetTexture(0, s_pTexture[s_Result]);
	
	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	if (s_Result == RESULT_WIN)
	{//����
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffTime, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureTime);

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
}

//--------------------------------------------------
//���U���g�̐ݒ�
//--------------------------------------------------
void SetResult(RESULT Result)
{
	if (s_Result == RESULT_NONE || Result == RESULT_NONE)
	{//�܂����U���g�����܂��ĂȂ��A���U���g�̏�����
		s_Result = Result;		//���U���g����
	}
}

//--------------------------------------------------
//���[�h����
//--------------------------------------------------
static void UpdateMode(void)
{
	switch (s_Result)
	{
	case RESULT_WIN:		//����
		//�t�F�[�h�ݒ�
		SetFade(MODE_RANKING);
		break;

	case RESULT_LOSE:		//����
		//�t�F�[�h�ݒ�
		SetFade(MODE_TITLE);
		break;

	case RESULT_NONE:		//�������Ă��Ȃ����

		//break�Ȃ��@

	default:
		assert(false);
		break;
	}
}