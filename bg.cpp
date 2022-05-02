//--------------------------------------------------
//
// ACG���� ( bg.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "bg.h"
#include "block.h"
#include "input.h"
#include "player.h"
#include "setup.h"

//--------------------------------------------------
//�w�i�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	float			fWidth;			//��
	float			fHeight;		//����
}BG;

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_BG		(2)		//�w�i�̍ő吔

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(int i);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static BG							s_aBG[MAX_BG];			//�w�i�̏��

//--------------------------------------------------
//�w�i�̏���������
//--------------------------------------------------
void InitBG(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BG; i++)
	{
		//�\���̂̏���������
		InitStruct(i);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, s_aBG[i].pos, s_aBG[i].fWidth, s_aBG[i].fHeight);

		//rhw�̏���������
		Initrhw(pVtx);

		if (i == 0)
		{
			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 0.615f, 0.215f, 0.341f, 1.0f);
		}
		else
		{
			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`���̏���������
		Inittex(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i��
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�w�i�̏I������
//--------------------------------------------------
void UninitBG(void)
{
	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//�w�i�̍X�V����
//--------------------------------------------------
void UpdateBG(void)
{
	
}

//--------------------------------------------------
//�w�i�̕`�揈��
//--------------------------------------------------
void DrawBG(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < MAX_BG; i++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}

//--------------------------------------------------
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(int i)
{
	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight;

	if (i == 0)
	{
		fHeight = SCREEN_HEIGHT * 0.5f;
	}
	else
	{
		fHeight = SCREEN_HEIGHT / MAX_Y_BLOCK;
	}
	
	s_aBG[i].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);
	s_aBG[i].fWidth = fWidth;
	s_aBG[i].fHeight = fHeight;
}