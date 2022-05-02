//--------------------------------------------------
//
// ACG���� ( result.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "fade.h"
#include "input.h"
#include "result.h"
#include "setup.h"
#include "sound.h"

#include <assert.h>

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[RESULT_MAX];		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static RESULT						s_Result;					//���U���g�̏��

//--------------------------------------------------
//���U���g�̏���������
//--------------------------------------------------
void InitResult(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�������̃N���A
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result000.png",
		&s_pTexture[RESULT_LOSE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\result003.png",
		&s_pTexture[RESULT_WIN]);

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

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	if (s_Result == RESULT_LOSE)
	{//����
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_MYSTERYMAN);
	}
	else if (s_Result == RESULT_WIN)
	{//����
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_START_BEETS);
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
}

//--------------------------------------------------
//���U���g�̍X�V����
//--------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
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

			//break�Ȃ�

		default:
			assert(false);
			break;
		}
	}
}

//--------------------------------------------------
//���U���g�̕`�揈��
//--------------------------------------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

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