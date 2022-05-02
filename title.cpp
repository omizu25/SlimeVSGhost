//--------------------------------------------------
//
// ACG���� ( title.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "enemy.h"
#include "fade.h"
#include "gauge.h"
#include "input.h"
#include "item.h"
#include "player.h"
#include "ranking.h"
#include "result.h"
#include "setup.h"
#include "sound.h"
#include "title.h"

#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_TITLE			(3)				//�^�C�g���̍ő吔
#define VS_WIDTH			(150.0f)		//VS�̕�
#define VS_HEIGHT			(200.0f)		//VS�̍���
#define TITLE_WIDTH			(500.0f)		//�^�C�g���̕�
#define TITLE_HEIGHT		(350.0f)		//�^�C�g���̍���
#define MENU_WIDTH			(550.0f)		//���j���[�̕�
#define MENU_HEIGHT			(175.0f)		//���j���[�̍���
#define WIDTH_INTERVAL		(125.0f)		//���̊Ԋu
#define HEIGHT_INTERVAL		(125.0f)		//�����̊Ԋu

//--------------------------------------------------
//���j���[���`
//--------------------------------------------------
typedef enum
{
	MENU_GAME = 0,		//�Q�[��
	MENU_TUTORIAL,		//�`���[�g���A��
	MENU_RANKING,		//�����L���O
	MENU_MAX
}MENU;

//--------------------------------------------------
//���j���[�̏��(�_�ŋ)���`
//--------------------------------------------------
typedef enum
{
	MENUSTATE_IN = 0,		//������
	MENUSTATE_OUT,			//�����Ȃ�
	MENUSTATE_MAX
}MENUSTATE;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx);
static void UpdateMenu(void);
static void UpdateState(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			//�w�i�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TITLE];			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureMenu[MENU_MAX];		//���j���[�̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffMenu = NULL;			//���j���[�̒��_�o�b�t�@�̃|�C���^
static MENU							s_Menu;							//���j���[
static MENUSTATE					s_state;						//���j���[�̏��
static D3DXCOLOR					s_col;							//���j���[�̐F
static float						s_fChange;						//���l�̕ω���

//--------------------------------------------------
//�^�C�g���̏���������
//--------------------------------------------------
void InitTitle(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�������̃N���A
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));
	
	//�������̃N���A
	memset(&s_pTextureMenu[0], NULL, sizeof(s_pTextureMenu));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title013.png",
		&s_pTexture[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title014.png",
		&s_pTexture[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title015.png",
		&s_pTexture[2]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title002.png",
		&s_pTextureMenu[MENU_GAME]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title003.png",
		&s_pTextureMenu[MENU_TUTORIAL]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title004.png",
		&s_pTextureMenu[MENU_RANKING]);

	s_Menu = MENU_GAME;
	s_state = MENUSTATE_IN;
	s_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	s_fChange = 0.025f;

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
		sizeof(VERTEX_2D) * 4 * MAX_TITLE,
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
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, pos, fWidth, fHeight);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 0.615f, 0.215f, 0.341f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBG->Unlock();

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_TITLE; i++)
	{
		float fTitleWidth = 0.0f;
		float fTitleHeight = 0.0f;
		D3DXVECTOR3 TitlePos = D3DXVECTOR3(0.0f, fHeight - 175.0f, 0.0f);

		switch (i)
		{
		case 0:
			TitlePos.x = 280.0f;
			fTitleWidth = TITLE_WIDTH * 0.5f;
			fTitleHeight = TITLE_HEIGHT * 0.5f;
			break;

		case 1:
			TitlePos.x = 630.0f;
			fTitleWidth = VS_WIDTH * 0.5f;
			fTitleHeight = VS_HEIGHT * 0.5f;
			break;

		case 2:
			TitlePos.x = 1005.0f;
			fTitleWidth = TITLE_WIDTH * 0.5f;
			fTitleHeight = TITLE_HEIGHT * 0.5f;
			break;

		default:
			assert(false);
			break;
		}

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, TitlePos, fTitleWidth, fTitleHeight);

		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̏���������
		Initcol(pVtx);

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.005f, 0.998f, 0.005f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MENU_MAX; i++)
	{
		float fMenuWidth = MENU_WIDTH * 0.5f;
		float fMenuHeight = MENU_HEIGHT * 0.5f;
		D3DXVECTOR3 MenuPos = D3DXVECTOR3(fWidth, fHeight + 25.0f, 0.0f);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, MenuPos + D3DXVECTOR3(0.0f, (HEIGHT_INTERVAL * i), 0.0f), fMenuWidth, fMenuHeight);

		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx,0.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̏���������
		Inittex(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMenu->Unlock();

	//�Q�[�W�̏���������
	InitGauge();

	//�A�C�e���̏���������
	InitItem();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();

	//�G�̐ݒ菈��
	SetEnemy(D3DXVECTOR3(900.0f, 525.0f, 0.0f), ENEMYTYPE_BOY);

	//�G�̐ݒ菈��
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + (ENEMY_WIDTH * 0.5f), 680.0f, 0.0f), ENEMYTYPE_GIRL);

	//�G�̏�Ԑݒ菈��
	SetEnemyState(ENEMYSTATE_PV);

	//�v���C���[�̐ݒ菈��
	SetPlayer(PLAYERSTATE_PV);

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_IN_THE_WAY);
}

//--------------------------------------------------
//�^�C�g���̏I������
//--------------------------------------------------
void UninitTitle(void)
{
	//�T�E���h�̒�~
	StopSound();

	if (s_pVtxBuffBG != NULL)
	{//�w�i�̒��_�o�b�t�@�̔j��
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	for (int i = 0; i < MAX_TITLE; i++)
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

	//�Q�[�W�̏I������
	UninitGauge();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�̏I������
	UninitEnemy();
}

//--------------------------------------------------
//�^�C�g���̍X�V����
//--------------------------------------------------
void UpdateTitle(void)
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
	Setcol(pVtx, s_col.r, s_col.g, s_col.b, s_col.a);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMenu->Unlock();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�G�̍X�V����
	UpdateEnemy();

	//�A�C�e���̍X�V����
	UpdateItem();
}

//--------------------------------------------------
//�^�C�g���̕`�揈��
//--------------------------------------------------
void DrawTitle(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TITLE; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[i]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}

	//�G�̕`�揈��
	DrawEnemy();

	//�A�C�e���̕`�揈��
	DrawItem(ITEMTYPE_BLOCK);

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

	//�v���C���[�̕`�揈��
	DrawPlayer();
}

//--------------------------------------------------
//���j���[�̓��͎�����
//--------------------------------------------------
static void UpdateInput(VERTEX_2D *pVtx)
{
	int nMenu = (int)s_Menu;		//���j���[��int��

	if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S) ||
		GetJoypadTrigger(JOYKEY_UP) || GetJoypadTrigger(JOYKEY_DOWN))
	{//W�L�[�AS�L�[�������ꂽ���ǂ���
		//�F�̏�����
		s_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		//���(�_�ŋ)�̏�����
		s_state = MENUSTATE_IN;

		pVtx += (s_Menu * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, s_col.r, s_col.g, s_col.b, s_col.a);

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_SYSTEM40);
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP))
	{//W�L�[�������ꂽ���ǂ���
		nMenu = ((nMenu - 1) + 3) % 3;

		s_Menu = (MENU)nMenu;

		s_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN))
	{//S�L�[�������ꂽ���ǂ���
		nMenu = ((nMenu + 1) + 3) % 3;

		s_Menu = (MENU)nMenu;

		s_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	}
}

//--------------------------------------------------
//���j���[�̑I������
//--------------------------------------------------
static void UpdateMenu(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || 
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_START))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		if (s_Menu == MENU_GAME)
		{//�Q�[���̂Ƃ�
			//�t�F�[�h�̐ݒ�
			SetFade(MODE_GAME);
		}
		else if (s_Menu == MENU_TUTORIAL)
		{//�`���[�g���A���̂Ƃ�
			//�t�F�[�h�̐ݒ�
			SetFade(MODE_TUTORIAL);
		}
		else if (s_Menu == MENU_RANKING)
		{//�����L���O�̂Ƃ�
			//�����L���O�̍X�V�̏���������
			InitRankUpdate();

			//�t�F�[�h�̐ݒ�
			SetFade(MODE_RANKING);
		}

		s_fChange = 1.0f;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_SYSTEM49);
	}
}

//--------------------------------------------------
//��Ԃ̍X�V����
//--------------------------------------------------
static void UpdateState(void)
{
	if (s_state == MENUSTATE_IN)
	{//�t�F�[�h�C�����
		s_col.a -= s_fChange;		//�|���S���𓧖��ɂ��Ă���

		if (s_col.a <= 0.25f)
		{
			s_col.a = 0.25f;
			s_state = MENUSTATE_OUT;		//�A�E�g��Ԃ�
		}
	}
	else if (s_state == MENUSTATE_OUT)
	{//�t�F�[�h�A�E�g���
		s_col.a += s_fChange;		//�|���S����s�����ɂ��Ă���

		if (s_col.a >= 1.0f)
		{
			s_col.a = 1.0f;
			s_state = MENUSTATE_IN;			//�C����Ԃ�
		}
	}
}