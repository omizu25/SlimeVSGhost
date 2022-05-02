//--------------------------------------------------
//
// ACG���� ( countdown.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "colon.h"
#include "game.h"
#include "number.h"
#include "result.h"
#include "setup.h"
#include "countdown.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define NUMBER_WIDTH		(300.0f)		//���̕�
#define NUMBER_HEIGHT		(500.0f)		//���̍���
#define START_WIDTH			(700.0f)		//�X�^�[�g�̕�
#define START_HEIGHT		(300.0f)		//�X�^�[�g�̍���
#define ONE_SECOND			(60)			//�P�b

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static D3DXVECTOR3					s_pos;					//�ʒu
static int							s_nCountdown;			//�J�E���g�_�E���̒l
static int							s_nSecond;				//�P�b

//--------------------------------------------------
//�J�E���g�_�E���̏���������
//--------------------------------------------------
void InitCountdown(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\start000.png",
		&s_pTexture);

	//�ʒu������������
	s_pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	s_nCountdown = 3;
	s_nSecond = 0;

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

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, s_pos, START_WIDTH * 0.5f, START_HEIGHT * 0.5f);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`���̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//���̐ݒ菈��
	SetMiddleNumber(s_pos, NUMBER_WIDTH * 0.5f, NUMBER_HEIGHT * 0.5f, s_nCountdown);
}

//--------------------------------------------------
//�J�E���g�_�E���̏I������
//--------------------------------------------------
void UninitCountdown(void)
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

//--------------------------------------------------
//�J�E���g�_�E���̍X�V����
//--------------------------------------------------
void UpdateCountdown(void)
{

}

//--------------------------------------------------
//�J�E���g�_�E���̕`�揈��
//--------------------------------------------------
void DrawCountdown(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	if (s_nCountdown == 0)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}

//--------------------------------------------------
//�J�E���g�_�E���̉��Z����
//--------------------------------------------------
void AddCountdown(int nValue)
{
	s_nSecond += nValue;

	if (s_nSecond % ONE_SECOND == 0)
	{
		s_nCountdown--;
	}

	if (s_nCountdown > 0)
	{
		//���̃e�N�X�`���̏���
		TexNumber(s_nCountdown, -1);
	}
	else
	{
		//���̎g�p����
		UseNumber(-1);

		//�Q�[���̐ݒ菈��
		SetGameState(GAMESTATE_NORMAL, 0);
	}
}

//--------------------------------------------------
//�J�E���g�_�E���̐ݒ�
//--------------------------------------------------
void SetCountdown(int nCountdown)
{
	s_nCountdown = nCountdown;
}