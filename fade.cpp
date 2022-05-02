//--------------------------------------------------
//
// ACG���� ( fade.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "fade.h"
#include "input.h"
#include "setup.h"

#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define FADE_CHANGE		(0.035f)		//�t�F�[�h�̕ω���

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static FADE							s_fade;					//���̃t�F�[�h
static MODE							s_modeNext;				//���̃��[�h
static D3DXCOLOR					s_colorFade;			//�|���S��(�t�F�[�h)�̐F
static int							s_nCntOut;				//�A�E�g�̃J�E���^�[

//--------------------------------------------------
//�t�F�[�h�̏���������
//--------------------------------------------------
void InitFade(MODE modeNext)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�����|���S��(�s����)�ɂ��Ă���
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	s_fade = FADE_NONE;			//�t�F�[�h�C����Ԃ�

	s_modeNext = modeNext;		//���̉��(���[�h)��ݒ�

	s_nCntOut = 0;

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

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, D3DXVECTOR3(fWidth, fHeight, 0.0f), fWidth, fHeight);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, s_colorFade.r, s_colorFade.g, s_colorFade.b, s_colorFade.a);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//���[�h�̐ݒ�
	SetMode(s_modeNext);
}

//--------------------------------------------------
//�t�F�[�h�̏I������
//--------------------------------------------------
void UninitFade(void)
{
	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//�t�F�[�h�̍X�V����
//--------------------------------------------------
void UpdateFade(void)
{
	if (s_fade != FADE_NONE)
	{//�t�F�[�h��
		switch (s_fade)
		{
		case FADE_OUT:		//�t�F�[�h�A�E�g���

			s_colorFade.a += FADE_CHANGE;		//�|���S����s�����ɂ��Ă���

			if (s_colorFade.a >= 1.0f)
			{//�s�����ɂȂ���
				s_colorFade.a = 1.0f;
				s_fade = FADE_IN;		//�t�F�[�h�C����Ԃ�

				//���[�h�̐ݒ�
				SetMode(s_modeNext);
			}

			break;

		case FADE_SKIP:		//�t�F�[�h�X�L�b�v

			s_colorFade.a -= FADE_CHANGE;		//�|���S���𓧖��ɂ��Ă���

			//break�Ȃ�

		case FADE_IN:		//�t�F�[�h�C�����

			s_colorFade.a -= FADE_CHANGE;		//�|���S���𓧖��ɂ��Ă���

			if (s_colorFade.a <= 0.0f)
			{//�����ɂȂ���
				s_colorFade.a = 0.0f;
				s_fade = FADE_NONE;		//�������Ă��Ȃ���Ԃ�
			}

			break;

		default:
			assert(false);
			break;
		}

		switch (s_fade)
		{
		case FADE_OUT:		//�t�F�[�h�A�E�g���

			if (s_nCntOut < 1)
			{//�P��艺�Ȃ�break
				s_nCntOut++;
				break;
			}

			//�P�ȏ�Ȃ�break�Ȃ�

		case FADE_IN:		//�t�F�[�h�C�����

			if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
			{//����L�[(ENTER�L�[)�������ꂽ���ǂ���

				if (s_fade == FADE_OUT)
				{
					//�����|���S��(�s����)�ɂ��Ă���
					s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

					//���[�h�̐ݒ�
					SetMode(s_modeNext);
				}

				s_fade = FADE_SKIP;		//�t�F�[�h�X�L�b�v
			}

			break;

		case FADE_NONE:		//�������Ă��Ȃ����
		case FADE_SKIP:		//�t�F�[�h�X�L�b�v

			break;

		default:
			assert(false);
			break;
		}

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, s_colorFade.r, s_colorFade.g, s_colorFade.b, s_colorFade.a);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//�t�F�[�h�̕`�揈��
//--------------------------------------------------
void DrawFade(void)
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
}

//--------------------------------------------------
//�t�F�[�h�̐ݒ菈��
//--------------------------------------------------
void SetFade(MODE modeNext)
{
	if (s_fade == FADE_NONE)
	{//�������Ă��Ȃ���ԂȂ�
		s_fade = FADE_OUT;			//�t�F�[�h�A�E�g��Ԃ�

		s_modeNext = modeNext;		//���̉��(���[�h)��ݒ�

		//�����|���S��(����)�ɂ��Ă���
		s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		s_nCntOut = 0;
	}
}

//--------------------------------------------------
//�t�F�[�h�̎擾����
//--------------------------------------------------
FADE GetFade(void)
{
	return s_fade;
}