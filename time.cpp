//--------------------------------------------------
//
// ACG���� ( time.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "game.h"
#include "result.h"
#include "setup.h"
#include "time.h"

//-------------------------
//�}�N����`
//-------------------------
#define MAX_TIME			(6)			//�^�C���̍ő包��
#define MAX_TEX				(10)		//tex�̍ő吔
#define TIME_WIDTH			(30.0f)		//�^�C���̕�
#define TIME_HEIGHT			(25.0f)		//�^�C���̍���
#define WIDTH_INTERVAL		(35.0f)		//���̊Ԋu

//-------------------------
//�X�^�e�B�b�N�ϐ�
//-------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static D3DXVECTOR3					s_pos;					//�^�C���̈ʒu
static int							s_nTime;				//�^�C���̒l

//-------------------------
//�^�C���̏���������
//-------------------------
void InitTime(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number000.png",
		&s_pTexture);

	float fWidth = SCREEN_WIDTH / MAX_X_BLOCK;
	float fHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK) * 0.5f;

	//�ʒu������������
	s_pos = D3DXVECTOR3(fWidth * 30.0f, fHeight * 2.0f, 0.0f);

	s_nTime = 0;		//�l������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�������̐ݒ�
	for (int i = 0; i < MAX_TIME; i++)
	{
		float fInterval = (WIDTH_INTERVAL * 0.3f) * (i / 2);

		//���_���W�̐ݒ菈������
		SetRightpos(pVtx, D3DXVECTOR3(s_pos.x - (WIDTH_INTERVAL * i) - fInterval, s_pos.y, 0.0f), TIME_WIDTH, TIME_HEIGHT);

		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		float fTex = 1.0f / MAX_TEX;

		//�e�N�X�`�����W�̐ݒ�
		Settex(pVtx, 0.0f, fTex, 0.0f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//-------------------------
//�^�C���̏I������
//-------------------------
void UninitTime(void)
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

//-------------------------
//�^�C���̍X�V����
//-------------------------
void UpdateTime(void)
{
	s_nTime++;

	//�J��グ����
	if (s_nTime % 100 >= 60)
	{//�P�b����
		s_nTime += 40;
	}

	if (s_nTime % 10000 >= 6000)
	{//�P������
		s_nTime += 4000;
	}

	if (s_nTime % 1000000 >= 300000)
	{//�R�O������
		//���U���g�̐ݒ菈��
		SetResult(RESULT_LOSE);

		//�Q�[���̐ݒ菈��
		SetGameState(GAMESTATE_END);
	}

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nTime = s_nTime;
	int i = 0;
	float aTex[MAX_TIME];
	int aNumber[MAX_TIME];

	while (nTime != 0)
	{//�P�����ɕ�����
		float fTex = 1.0f / MAX_TEX;

		aNumber[i] = nTime % 10;
		aTex[i] = aNumber[i] * fTex;
		nTime /= 10;

		//�e�N�X�`�����W�̐ݒ�
		Settex(pVtx, 0.0f + aTex[i], fTex + aTex[i], 0.0f, 1.0f);

		i++;
		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i��
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//-------------------------
//�^�C���̕`�揈��
//-------------------------
void DrawTime(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_TIME; i++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}

//-------------------------
//�^�C���̎擾����
//-------------------------
int GetTime(void)
{
	return s_nTime;		//���̃^�C����������
}