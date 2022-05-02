//--------------------------------------------------
//
// ACG���� ( effect.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "effect.h"
#include "setup.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_EFFECT		(4096)		//�G�t�F�N�g�̍ő吔

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
	int				nType;			//���
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Effect;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static Effect						s_aEffect[MAX_EFFECT];		//�G�t�F�N�g�̏��

//--------------------------------------------------
//�G�t�F�N�g�̏���������
//--------------------------------------------------
void InitEffect(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\effect101.jpg",
		&s_pTexture);

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
	memset(&s_aEffect, NULL, sizeof(Effect));

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

		pEffect->nLife--;

		if (pEffect->nLife <= 0)
		{
			pEffect->bUse = false;
		}
	}
}

//--------------------------------------------------
//�G�t�F�N�g�̕`�揈��
//--------------------------------------------------
void DrawEffect(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	//���u�����f�B���O�����H�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				//���̂܂ܑ���

	//�G�t�F�N�g�̕`��
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (s_aEffect[i].bUse)
		{//�G�t�F�N�g���g�p����Ă���
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
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int nLife, int nType)
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
		pEffect->nType = nType;
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