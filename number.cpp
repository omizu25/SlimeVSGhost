//--------------------------------------------------
//
// ACG���� ( number.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "setup.h"
#include "number.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_NUMBER		(256)		//���̍ő包��
#define MAX_TEX			(10)		//tex�̍ő吔

//--------------------------------------------------
//���̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	float			fWidth;			//��
	float			fHeight;		//����
	int				nNumber;		//��
	int				nDigit;			//����
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Number;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(Number *pNumber);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static Number						s_aNumber[MAX_NUMBER];		//���̏��

//--------------------------------------------------
//���̏���������
//--------------------------------------------------
void InitNumber(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number000.png",
		&s_pTexture);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_aNumber[i];

		//�\���̂̏���������
		InitStruct(pNumber);

		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//���̏I������
//--------------------------------------------------
void UninitNumber(void)
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
//���̍X�V����
//--------------------------------------------------
void UpdateNumber(void)
{
	
}

//--------------------------------------------------
//���̕`�揈��
//--------------------------------------------------
void DrawNumber(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_NUMBER; i++)
	{
		if (s_aNumber[i].bUse)
		{//�����g�p����Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u(�|���S��)��
		}
	}
}

//--------------------------------------------------
//���̐ݒ菈��
//--------------------------------------------------
void SetNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_aNumber[i];

		if (pNumber->bUse)
		{//�����g�p����Ă���
			continue;
		}

		//�����g�p����Ă��Ȃ�

		pNumber->pos = pos;
		pNumber->fWidth = fWidth;
		pNumber->fHeight = fHeight;
		pNumber->nNumber = nNumber;
		pNumber->nDigit = nDigit;
		pNumber->bUse = true;

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetRightpos(pVtx, pos, fWidth, fHeight);

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//���̃e�N�X�`���̏���
//--------------------------------------------------
void TexNumber(int nNumber, int nDigit)
{
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Number *pNumber = &s_aNumber[i];

		if (!pNumber->bUse || pNumber->nDigit != nDigit)
		{//�����g�p����Ă��Ȃ��A�������Ⴄ
			continue;
		}

		//�����g�p����Ă���A����������

		pNumber->nNumber = nNumber;

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		float fTex = 1.0f / MAX_TEX;
		float fNumberTex = fTex * nNumber;

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.0f + fNumberTex, fTex + fNumberTex, 0.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(Number *pNumber)
{
	pNumber->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pNumber->fWidth = 0.0f;
	pNumber->fHeight = 0.0f;
	pNumber->nNumber = 0;
	pNumber->nDigit = 0;
	pNumber->bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
}