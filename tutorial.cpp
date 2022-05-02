//--------------------------------------------------
//
// ACG���� ( tutorial.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "fade.h"
#include "input.h"
#include "number.h"
#include "setup.h"
#include "sound.h"
#include "tutorial.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MIN_MOVE			(15.0f)			//�ړ��ʂ̍ŏ��l
#define MAX_MOVE			(30.0f)			//�ړ��ʂ̍ő�l
#define NEXT_WIDTH			(80.0f)			//���ւ̕�
#define NEXT_HEIGHT			(400.0f)		//���ւ̍���
#define END_WIDTH			(800.0f)		//�I���̕�
#define END_HEIGHT			(60.0f)			//�I���̍���
#define NUMBER_WIDTH		(60.0f)			//���̕�
#define NUMBER_HEIGHT		(80.0f)			//���̍���

//--------------------------------------------------
//���j���[���`
//--------------------------------------------------
typedef enum
{
	MENU_RULE = 0,		//���[��
	MENU_OPERATION,		//����
	MENU_MAX
}MENU;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void UpdateInput(void);
static void UpdateMove(void);
static void UpdateOffScreen(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBG = NULL;			//�w�i�̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTexture[MENU_MAX];			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureNext[MENU_MAX];		//���ւ̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffNext = NULL;			//���ւ̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureEnd = NULL;			//�I���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffEnd = NULL;			//�I���̒��_�o�b�t�@�̃|�C���^
static LPDIRECT3DTEXTURE9			s_pTextureBar = NULL;			//���̃e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuffBar = NULL;			//���̒��_�o�b�t�@�̃|�C���^
static MENU							s_menu;							//���݂̃��j���[
static D3DXVECTOR3					s_pos;							//�ʒu
static D3DXVECTOR3					s_move;							//�ړ���

//--------------------------------------------------
//�`���[�g���A���̏���������
//--------------------------------------------------
void InitTutorial(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�������̃N���A
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	memset(&s_pTextureNext[0], NULL, sizeof(s_pTextureNext));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title005.png",
		&s_pTexture[MENU_RULE]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title012.png",
		&s_pTexture[MENU_OPERATION]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title010.png",
		&s_pTextureNext[MENU_RULE]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title011.png",
		&s_pTextureNext[MENU_OPERATION]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\title009.png",
		&s_pTextureEnd);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\number002.png",
		&s_pTextureBar);

	s_pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0.0f);
	s_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_menu = MENU_RULE;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBG,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffNext,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffEnd,
		NULL);

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBar,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	/* ���w�i�� */

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
	Setcol(pVtx, 0.85f, 0.85f, 0.85f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBG->Unlock();
		
	/* �������摜�� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MENU_MAX; i++)
	{
		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, D3DXVECTOR3(pos.x + (SCREEN_WIDTH * i), pos.y, 0.0f), fWidth * 0.75f, fHeight * 0.75f);

		//rhw�̏���������
		Initrhw(pVtx);

		//���_�J���[�̏���������
		Initcol(pVtx);

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.0f, 1.0f, 0.001f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	/* �����ց� */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffNext->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthNext = NEXT_WIDTH * 0.5f;
	float fHeightNext = NEXT_HEIGHT * 0.5f;
	D3DXVECTOR3 posNext = D3DXVECTOR3(s_pos.x - NEXT_WIDTH, fHeight, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, posNext, fWidthNext, fHeightNext);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffNext->Unlock();

	/* ���I��聫 */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	float fWidthEnd = END_WIDTH * 0.5f;
	float fHeightEnd = END_HEIGHT * 0.5f;
	D3DXVECTOR3 posEnd = D3DXVECTOR3(pos.x, SCREEN_HEIGHT - END_HEIGHT * 0.75f, 0.0f);

	//���_���W�̐ݒ菈��
	SetMiddlepos(pVtx, posEnd, fWidthEnd, fHeightEnd);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffEnd->Unlock();

	/* ������ */

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);

	float fHeightNumber = NUMBER_HEIGHT * 0.5f;
	D3DXVECTOR3 posNumber = D3DXVECTOR3(fWidth - (NUMBER_WIDTH * 0.5f), fHeightNumber, 0.0f);

	//���_���W�̐ݒ菈��
	SetRightpos(pVtx, D3DXVECTOR3(posNumber.x + NUMBER_WIDTH, posNumber.y, 0.0f), NUMBER_WIDTH, fHeightNumber * 0.75f);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 0.0f, 0.0f, 0.0f, 1.0f);

	//�e�N�X�`�����W�̏���������
	Inittex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBar->Unlock();

	/* ������ */

	//���̏���������
	InitNumber();

	for (int i = 0; i < MENU_MAX; i++)
	{
		//���̐ݒ菈��
		SetRightNumber(D3DXVECTOR3(posNumber.x + ((NUMBER_WIDTH * i) * 2.0f), posNumber.y, 0.0f), NUMBER_WIDTH, fHeightNumber, i + 1, i, 0);
	}

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_RISE);
}

//--------------------------------------------------
//�`���[�g���A���̏I������
//--------------------------------------------------
void UninitTutorial(void)
{
	//�T�E���h�̒�~
	StopSound();

	if (s_pVtxBuffBG != NULL)
	{//�w�i�̒��_�o�b�t�@�̔j��
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	for (int i = 0; i < MENU_MAX; i++)
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
		if (s_pTextureNext[i] != NULL)
		{//���ւ̃e�N�X�`���̔j��
			s_pTextureNext[i]->Release();
			s_pTextureNext[i] = NULL;
		}
	}

	if (s_pVtxBuffNext != NULL)
	{//���ւ̒��_�o�b�t�@�̔j��
		s_pVtxBuffNext->Release();
		s_pVtxBuffNext = NULL;
	}

	if (s_pTextureEnd != NULL)
	{//�I���̃e�N�X�`���̔j��
		s_pTextureEnd->Release();
		s_pTextureEnd = NULL;
	}

	if (s_pVtxBuffEnd != NULL)
	{//�I���̒��_�o�b�t�@�̔j��
		s_pVtxBuffEnd->Release();
		s_pVtxBuffEnd = NULL;
	}

	if (s_pTextureBar != NULL)
	{//���̃e�N�X�`���̔j��
		s_pTextureBar->Release();
		s_pTextureBar = NULL;
	}

	if (s_pVtxBuffBar != NULL)
	{//���̒��_�o�b�t�@�̔j��
		s_pVtxBuffBar->Release();
		s_pVtxBuffBar = NULL;
	}

	//���̏I������
	UninitNumber();
}

//--------------------------------------------------
//�`���[�g���A���̍X�V����
//--------------------------------------------------
void UpdateTutorial(void)
{
	//���͏���
	UpdateInput();
	
	//�ړ�����
	UpdateMove();
}

//--------------------------------------------------
//�`���[�g���A���̕`�揈��
//--------------------------------------------------
void DrawTutorial(void)
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

	for (int i = 0; i < MENU_MAX; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[i]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffNext, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureNext[s_menu]);

	if (s_move.x == 0.0f)
	{//�ړ��ʂ��Ȃ���
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffEnd, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureEnd);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBar, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureBar);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//���̕`�揈��
	DrawNumber();
}

//--------------------------------------------------
//���͏���
//--------------------------------------------------
static void UpdateInput(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_B) ||
		GetJoypadTrigger(JOYKEY_B) || GetJoypadTrigger(JOYKEY_START))
	{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
		if (GetFade() == FADE_NONE)
		{//�������Ă��Ȃ�
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_SYSTEM49);
		}

		//�t�F�[�h�̐ݒ�
		SetFade(MODE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) ||
		GetJoypadTrigger(JOYKEY_LEFT))
	{//A�L�[�������ꂽ
		if (s_pos.x != SCREEN_WIDTH)
		{
			if (s_move.x >= MIN_MOVE)
			{//�����ꂽ���Q��ȏ�
				s_move.x = MAX_MOVE;
			}
			else
			{//�P���
				s_move.x = MIN_MOVE;

				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SYSTEM40);
			}
		}
	}
	else if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT) ||
		GetJoypadTrigger(JOYKEY_RIGHT))
	{//D�L�[�������ꂽ
		if (s_pos.x != 0.0f)
		{
			if (s_move.x <= -MIN_MOVE)
			{//�����ꂽ���Q��ȏ�
				s_move.x = -MAX_MOVE;
			}
			else
			{//�P���
				s_move.x = -MIN_MOVE;

				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SYSTEM40);
			}
		}
	}
}

//--------------------------------------------------
//�ړ�����
//--------------------------------------------------
static void UpdateMove(void)
{
	if (s_move.x != 0.0f)
	{//�ړ��ʂ����鎞
		//�ʒu���X�V
		s_pos.x += s_move.x;

		//��ʊO����
		UpdateOffScreen();

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fWidth = SCREEN_WIDTH * 0.5f;
		float fHeight = SCREEN_HEIGHT * 0.5f;
		D3DXVECTOR3 pos = s_pos - D3DXVECTOR3(fWidth, 0.0f, 0.0f);

		for (int i = 0; i < MENU_MAX; i++)
		{
			//���_���W�̐ݒ菈��
			SetMiddlepos(pVtx, D3DXVECTOR3(pos.x + (SCREEN_WIDTH * i), pos.y, 0.0f), fWidth * 0.75f, fHeight * 0.75f);

			pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
		}

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		if (s_pos.x >= fWidth)
		{//��ʂ̐^�񒆂�荶
			s_menu = MENU_RULE;
		}
		else
		{//��ʂ̐^�񒆂��E
			s_menu = MENU_OPERATION;
		}

		//���̃e�N�X�`���̏���
		TexNumber((s_menu + 1), 0);
	}
}

//--------------------------------------------------
//��ʊO����
//--------------------------------------------------
static void UpdateOffScreen(void)
{
	if (s_pos.x >= SCREEN_WIDTH)
	{//�E�[
		s_pos.x = SCREEN_WIDTH;
		s_move.x = 0.0f;
	}
	else if (s_pos.x <= 0.0f)
	{//���[
		s_pos.x = 0.0f;
		s_move.x = 0.0f;
	}

	if (s_pos.x >= SCREEN_WIDTH || s_pos.x <= 0.0f)
	{//�E�[�����[
		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffNext->Lock(0, 0, (void**)&pVtx, 0);

		float fWidthNext = NEXT_WIDTH * 0.5f;
		float fHeightNext = NEXT_HEIGHT * 0.5f;
		D3DXVECTOR3 posNext = s_pos + D3DXVECTOR3(-NEXT_WIDTH + ((NEXT_WIDTH * 2.0f) * s_menu), 0.0f, 0.0f);

		//���_���W�̐ݒ菈��
		SetMiddlepos(pVtx, posNext, fWidthNext, fHeightNext);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffNext->Unlock();
	}
}