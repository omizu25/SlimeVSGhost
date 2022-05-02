//--------------------------------------------------
//
// ACG���� ( pause.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "fade.h"
#include "game.h"
#include "input.h"
#include "pause.h"
#include "setup.h"
#include "sound.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_PAUSE			(5)			//�|�[�Y�̍ő吔
#define NUM_PAUSE			(2)			//�|�[�Y�̎��
#define PAUSE_WIDTH			(200)		//�|�[�Y�̕��̔���
#define PAUSE_HEIGHT		(200)		//�|�[�Y�̍����̔���
#define MENU_WIDTH			(215)		//�|�[�Y�̑I�����̕�
#define MENU_HEIGHT			(70)		//�|�[�Y�̑I�����̍���

//--------------------------------------------------
//�|�[�Y�̃��[�h���`
//--------------------------------------------------
typedef enum
{
	MENU_GAME = 0,		//�Q�[���ɖ߂�
	MENU_RETRY,			//�Q�[������蒼��
	MENU_TITLE,			//�^�C�g����ʂɖ߂�
	MENU_MAX
}MENU;

//--------------------------------------------------
//�|�[�Y�̑I�����̏��(�_�ŋ)���`
//--------------------------------------------------
typedef enum
{
	PAUSESTATE_IN = 0,		//������
	PAUSESTATE_OUT,			//�����Ȃ�
	PAUSESTATE_MAX
}PAUSESTATE;

//--------------------------------------------------
//�|�[�Y�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXCOLOR		col;		//�F
	PAUSESTATE		state;		//��ԏ��(�_�ŋ)
}Pause;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx);
static void UpdateMenu(void);
static void UpdateState(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		//���j���[�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMenu = NULL;			//���j���[�̒��_�o�b�t�@�̃|�C���^
static Pause						s_aPause[MENU_MAX];				//�|�[�Y���j���[�̏��
static D3DXVECTOR3					s_pos;							//�|�[�Y�̈ʒu
static D3DXCOLOR					s_aCol[NUM_PAUSE];				//�|�[�Y�̐F
static MENU							s_Menu;							//�|�[�Y���j���[
static float						s_fPausecolor;					//�|�[�Y�̐F�̃��̐��l

//--------------------------------------------------
//�|�[�Y�̏���������
//--------------------------------------------------
void InitPause(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�������̃N���A
	memset(&s_pTextureMenu[0], NULL, sizeof(s_pTextureMenu));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause002.png",
		&s_pTextureMenu[MENU_GAME]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause006.png",
		&s_pTextureMenu[MENU_RETRY]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\pause001.png",
		&s_pTextureMenu[MENU_TITLE]);

	s_pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		//�ʒu�̏�����

	//�F�̏�����
	s_aCol[0] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	s_aCol[1] = D3DXCOLOR(0.0f, 0.85f, 1.0f, 0.95f);

	for (int i = 0; i < MENU_MAX; i++)
	{
		//�ʒu�̏�����
		s_aPause[i].pos.x = SCREEN_WIDTH * 0.5f;
		s_aPause[i].pos.y = 250.0f + (i * 110.0f);
		s_aPause[i].pos.z = 0.0f;

		//�F�̏�����
		if (i == 0)
		{
			s_aPause[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			s_aPause[i].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		}

		//���(�_�ŋ)�̏�����
		s_aPause[i].state = PAUSESTATE_IN;
	}
	
	s_Menu = MENU_GAME;				//�|�[�Y���j���[�̏�����
	s_fPausecolor = 0.025f;			//�|�[�Y�̐F�̃��̐��l�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMenu,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_PAUSE; i++)
	{
		if (i == 0)
		{
			//���_���W�̐ݒ菈��
			SetMiddlepos(pVtx, s_pos, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		}
		else if (i == 1)
		{
			//���_���W�̐ݒ菈��
			SetMiddlepos(pVtx, s_pos, PAUSE_WIDTH, PAUSE_HEIGHT);
		}
		
		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, s_aCol[i].r, s_aCol[i].g, s_aCol[i].b, s_aCol[i].a);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MENU_MAX; i++)
	{
		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, s_aPause[i].pos, MENU_WIDTH, MENU_HEIGHT);

		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, s_aPause[i].col.r, s_aPause[i].col.g, s_aPause[i].col.b, s_aPause[i].col.a);

		//�e�N�X�`���̏���������
		Inittex(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMenu->Unlock();
}

//--------------------------------------------------
//�|�[�Y�̏I������
//--------------------------------------------------
void UninitPause(void)
{
	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	for (int i = 0; i < MENU_MAX; i++)
	{
		if (s_pTextureMenu[i] != NULL)
		{//���j���[�̃e�N�X�`���̔j��
			s_pTextureMenu[i]->Release();
			s_pTextureMenu[i] = NULL;
		}
	}

	if (s_pVtxBuffMenu != NULL)
	{//���j���[�̒��_�o�b�t�@�̔j��
		s_pVtxBuffMenu->Release();
		s_pVtxBuffMenu = NULL;
	}
}

//--------------------------------------------------
//�|�[�Y�̍X�V����
//--------------------------------------------------
void UpdatePause(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	if (GetFade() == FADE_NONE)
	{//�������Ă��Ȃ���ԂȂ�
		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

		//���j���[�̓��͎�����
		UpdateInput(pVtx);

		//���j���[�̑I������
		UpdateMenu();

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffMenu->Unlock();
	}

	//��Ԃ̍X�V����
	UpdateState();

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (s_Menu * 4);		//�Y���̈ʒu�܂Ői�߂�

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, s_aPause[s_Menu].col.r, s_aPause[s_Menu].col.g, 
				 s_aPause[s_Menu].col.b, s_aPause[s_Menu].col.a);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMenu->Unlock();
}

//--------------------------------------------------
//�|�[�Y�̕`�揈��
//--------------------------------------------------
void DrawPause(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < NUM_PAUSE; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MENU_MAX; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureMenu[i]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}

//--------------------------------------------------
//���j���[�̓��͎�����
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx)
{
	int nPauseMenu = (int)s_Menu;		//�|�[�Y���j���[��int��

	if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S) ||
		GetJoypadTrigger(JOYKEY_UP) || GetJoypadTrigger(JOYKEY_DOWN))
	{//W�L�[�AS�L�[�������ꂽ���ǂ���
		//�F�̏�����
		s_aPause[s_Menu].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

		//���(�_�ŋ)�̏�����
		s_aPause[s_Menu].state = PAUSESTATE_IN;

		pVtx += (s_Menu * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, s_aPause[s_Menu].col.r, s_aPause[s_Menu].col.g,
			s_aPause[s_Menu].col.b, s_aPause[s_Menu].col.a);

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_SYSTEM40);
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{//W�L�[�������ꂽ���ǂ���
		nPauseMenu = ((nPauseMenu - 1) + 3) % 3;

		s_Menu = (MENU)nPauseMenu;

		s_aPause[s_Menu].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{//S�L�[�������ꂽ���ǂ���
		nPauseMenu = ((nPauseMenu + 1) + 3) % 3;

		s_Menu = (MENU)nPauseMenu;

		s_aPause[s_Menu].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

//--------------------------------------------------
//���j���[�̑I������
//--------------------------------------------------
static void UpdateMenu(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		if (s_Menu == MENU_GAME)
		{//�Q�[���̂Ƃ�
			//�|�[�Y�̗L�������ݒ�
			SetEnablePause(false);
		}
		else if (s_Menu == MENU_RETRY)
		{//���g���C�̂Ƃ�
			//�t�F�[�h�̐ݒ�
			SetFade(MODE_GAME);
		}
		else if (s_Menu == MENU_TITLE)
		{//�^�C�g���̂Ƃ�
			//�t�F�[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}

		s_fPausecolor = 1.0f;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_SYSTEM49);
	}
}

//--------------------------------------------------
//��Ԃ̍X�V����
//--------------------------------------------------
static void UpdateState(void)
{
	if (s_aPause[s_Menu].state == PAUSESTATE_IN)
	{//�t�F�[�h�C�����
		s_aPause[s_Menu].col.a -= s_fPausecolor;		//�|���S���𓧖��ɂ��Ă���

		if (s_aPause[s_Menu].col.a <= 0.25f)
		{
			s_aPause[s_Menu].col.a = 0.25f;
			s_aPause[s_Menu].state = PAUSESTATE_OUT;		//�t�F�[�h�A�E�g��Ԃ�
		}
	}
	else if (s_aPause[s_Menu].state == PAUSESTATE_OUT)
	{//�t�F�[�h�A�E�g���
		s_aPause[s_Menu].col.a += s_fPausecolor;		//�|���S����s�����ɂ��Ă���

		if (s_aPause[s_Menu].col.a >= 1.0f)
		{
			s_aPause[s_Menu].col.a = 1.0f;
			s_aPause[s_Menu].state = PAUSESTATE_IN;		//�t�F�[�h�C����Ԃ�
		}
	}
}