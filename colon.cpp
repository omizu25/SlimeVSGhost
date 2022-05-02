//--------------------------------------------------
//
// ACG���� ( colon.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "game.h"
#include "number.h"
#include "result.h"
#include "setup.h"
#include "colon.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_COLON		(64)		//�R�����̍ő吔

//--------------------------------------------------
//�R�����̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//�ʒu
	float			fWidth;			//��
	float			fHeight;		//����
	int				nRank;			//����
	bool			bUse;			//�g�p���Ă��邩�ǂ���
}Colon;

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static Colon						s_aColon[MAX_COLON];		//�R�����̏��

//--------------------------------------------------
//�R�����̏���������
//--------------------------------------------------
void InitColon(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\time003.png",
		&s_pTexture);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_COLON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�������̃N���A
	memset(&s_aColon[0], NULL, sizeof(s_aColon));

	for (int i = 0; i < MAX_COLON; i++)
	{
		//���_���̏���������
		Initpos(pVtx);

		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̐ݒ�
		Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̏���������
		Inittex(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�R�����̏I������
//--------------------------------------------------
void UninitColon(void)
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
//�R�����̍X�V����
//--------------------------------------------------
void UpdateColon(void)
{
	
}

//--------------------------------------------------
//�R�����̕`�揈��
//--------------------------------------------------
void DrawColon(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	for (int i = 0; i < MAX_COLON; i++)
	{
		if (s_aColon[i].bUse)
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
//�R�����̐ݒ菈��
//--------------------------------------------------
void SetColon(D3DXVECTOR3 pos, float fWidth, float fHeight, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (pColon->bUse)
		{//�R�������g�p����Ă���
			continue;
		}

		//�R�������g�p����Ă��Ȃ�

		pColon->pos = pos;
		pColon->fWidth = fWidth;
		pColon->fHeight = fHeight;
		pColon->nRank = nRank;
		pColon->bUse = true;

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetRightpos(pVtx, pos, fWidth, fHeight);

		if (nRank == -1)
		{//�w��̃����N
			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 0.0f, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);
		}

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//--------------------------------------------------
//�R�����̃����N����
//--------------------------------------------------
void RankColon(D3DXCOLOR col, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (!pColon->bUse || pColon->nRank != nRank)
		{//�R�������g�p����Ă��Ȃ��A���ʂ��Ⴄ
			continue;
		}

		//�R�������g�p����Ă���A���ʂ�����

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, col.r, col.g, col.b, col.a);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//�R�����̈ړ�����
//--------------------------------------------------
void MoveColon(D3DXVECTOR3 move, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (!pColon->bUse || pColon->nRank != nRank)
		{//�R�������g�p����Ă��Ȃ��A���ʂ��Ⴄ
			continue;
		}

		//�R�������g�p����Ă���A���ʂ�����

		//�ʒu���X�V
		pColon->pos.x += move.x;

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetRightpos(pVtx, pColon->pos, pColon->fWidth, pColon->fHeight);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//�R�����̒��_����
//--------------------------------------------------
void PosColon(D3DXVECTOR3 pos, int nRank)
{
	for (int i = 0; i < MAX_COLON; i++)
	{
		Colon *pColon = &s_aColon[i];

		if (!pColon->bUse || pColon->nRank != nRank)
		{//�R�������g�p����Ă��Ȃ��A���ʂ��Ⴄ
			continue;
		}

		//�R�������g�p����Ă���A���ʂ�����

		//�ʒu���X�V
		pColon->pos.x += pos.x;

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetRightpos(pVtx, pos, pColon->fWidth, pColon->fHeight);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}