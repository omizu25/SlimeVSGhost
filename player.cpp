//--------------------------------------------------
//
// �A�N�V�����Q�[������ ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"
#include "player.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define PLAYER_WIDTH		(40.0f)			//�v���C���[�̕�
#define PLAYER_HEIGHT		(100.0f)		//�v���C���[�̍���
#define PLAYER_MOVE			(0.3f)			//�v���C���[�̈ړ���
#define MAX_U_PATTERN		(4)				//U�p�^�[���̍ő吔
#define MAX_V_PATTERN		(2)				//V�p�^�[���̍ő吔
#define MIN_MOVE			(0.30f)			//�����Ă�ŏ��l
#define MAX_JUMP			(-30.0f)		//�W�����v��
#define MAX_GRAVITY			(1.5f)			//�d�͂̍ő�l
#define MAX_INERTIA			(0.05f)			//�����̍ő�l
#define CNT_INTERVAL		(7)				//�J�E���^�[�̃C���^�[�o��

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static Player						s_Player;				//�v���C���[�̏��

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStructPlayer(void);
static void MovePlayer(VERTEX_2D *pVtx);
static void TexAnimPlayer(VERTEX_2D *pVtx);
static void OffScreenPlayer(void);

//--------------------------------------------------
//�v���C���[�̏���������
//--------------------------------------------------
void InitPlayer(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player000.png",
		&s_pTexture);

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

	//�v���C���[�̍\���̂̏���������
	InitStructPlayer();

	//���_���W�̐ݒ菈��
	SetBottompos(pVtx, s_Player.pos, PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT);

	//rhw�̐ݒ菈��
	Setrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ菈��
	Settex(pVtx, 0.0f, 1.0f / MAX_U_PATTERN, 0.0f, 1.0f / MAX_V_PATTERN);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�v���C���[�̏I������
//--------------------------------------------------
void UninitPlayer(void)
{
	if (s_pTexture != NULL)
	{//�v���C���[�̃e�N�X�`���̔j��
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{//�v���C���[�̒��_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//�v���C���[�̍X�V����
//--------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[�̈ړ�����
	MovePlayer(pVtx);

	//�v���C���[�̉�ʊO����
	OffScreenPlayer();

	//���_���W�̐ݒ菈��
	SetBottompos(pVtx, s_Player.pos, PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�v���C���[�̕`�揈��
//--------------------------------------------------
void DrawPlayer(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
//�v���C���[�̎擾����
//--------------------------------------------------
Player GetPlayer(void)
{
	return s_Player;
}

//--------------------------------------------------
//�v���C���[�̍\���̂̏���������
//--------------------------------------------------
static void InitStructPlayer(void)
{
	//���݂̈ʒu��������
	s_Player.pos.x = PLAYER_WIDTH * 0.5f;
	s_Player.pos.y = SCREEN_HEIGHT;
	s_Player.pos.z = 0.0f;

	s_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu��������
	s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ�������
	s_Player.bjump = false;									//�W�����v���ĂȂ�
	s_Player.nCounterAnim = 0;								//�J�E���^�[�̏�����
	s_Player.nPatternAnim = 0;								//�p�^�[���̏�����
	s_Player.nDirectionMove = 0;							//�����̏�����
}

//--------------------------------------------------
//�v���C���[�̈ړ�����
//--------------------------------------------------
static void MovePlayer(VERTEX_2D *pVtx)
{
	//�L�[���͂ł̈ړ�
	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT))
	{//A�L�[�������ꂽ
		if (s_Player.bjump)
		{//�W�����v���Ă�
			s_Player.move.x += -PLAYER_MOVE * 0.75f;
		}
		else
		{//�W�����v���ĂȂ�
			s_Player.move.x += -PLAYER_MOVE;
		}

		s_Player.nDirectionMove = 1;		//������
	}
	else if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT))
	{//D�L�[�������ꂽ
		if (s_Player.bjump)
		{//�W�����v���Ă�
			s_Player.move.x += PLAYER_MOVE * 0.75f;
		}
		else
		{//�W�����v���ĂȂ�
			s_Player.move.x += PLAYER_MOVE;
		}

		s_Player.nDirectionMove = 0;		//�E����
	}

	//�W�����v����
	if (s_Player.bjump == false &&
		GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(JOYKEY_B))
	{//�������ĂȂ��A�X�y�[�X�L�[�������ꂽ
		s_Player.move.y += MAX_JUMP;
		s_Player.bjump = true;
	}

	//�d��
	s_Player.move.y += MAX_GRAVITY;

	//�ʒu���X�V
	s_Player.pos.x += s_Player.move.x;
	s_Player.pos.y += s_Player.move.y;

	//�X�e�B�b�N�ł̈ړ��ʂ̍X�V
	s_Player.move.x += GetJoypadStick(JOYKEY_L_STICK).x;

	//�v���C���[�̃e�N�X�`���A�j���[�V��������
	TexAnimPlayer(pVtx);

	//�����E�ړ��ʂ��X�V (����������)
	s_Player.move.x += (0.0f - s_Player.move.x) * MAX_INERTIA;
}

//--------------------------------------------------
//�v���C���[�̉�ʊO����
//--------------------------------------------------
static void OffScreenPlayer(void)
{
	//��ʒ[����
	if (s_Player.pos.y >= SCREEN_HEIGHT)
	{//���[
		s_Player.pos.y = SCREEN_HEIGHT;
		s_Player.move.y = 0.0f;
		s_Player.bjump = false;
	}
	else if (s_Player.pos.y <= PLAYER_HEIGHT)
	{//��[
		s_Player.pos.y = PLAYER_HEIGHT;
		s_Player.move.y = 0.0f;
	}

	if (s_Player.pos.x >= SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f))
	{//�E�[
		s_Player.pos.x = -(PLAYER_WIDTH * 0.5f);
	}
	else if (s_Player.pos.x <= -(PLAYER_WIDTH * 0.5f))
	{//���[
		s_Player.pos.x = SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f);
	}
}

//--------------------------------------------------
//�v���C���[�̃e�N�X�`���A�j���[�V��������
//--------------------------------------------------
static void TexAnimPlayer(VERTEX_2D *pVtx)
{
	if (s_Player.bjump)
	{//�W�����v���Ă���
		s_Player.nCounterAnim = 0;		//�J�E���^�[�̏�����

		//�p�^�[��No.���X�V���� (�w��̃e�N�X�`��)
		if (s_Player.nPatternAnim % 2 == 0)
		{
			s_Player.nPatternAnim++;
		}

		float fPattren = (float)s_Player.nPatternAnim / MAX_U_PATTERN;
		float fDirection = (float)s_Player.nDirectionMove / MAX_V_PATTERN;

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, fPattren, fPattren + (1.0f / MAX_U_PATTERN), fDirection, fDirection + (1.0f / MAX_V_PATTERN));
	}
	else
	{//�W�����v���ĂȂ�
		if (s_Player.move.x >= MIN_MOVE || s_Player.move.x <= -MIN_MOVE)
		{//�ړ���
			s_Player.nCounterAnim++;		//�J�E���^�[�����Z

			if ((s_Player.nCounterAnim % CNT_INTERVAL) == 0)
			{//��莞�Ԍo�߂���
				//�p�^�[��No.���X�V����
				s_Player.nPatternAnim = (s_Player.nPatternAnim + 1) % MAX_U_PATTERN;

				float fPattren = (float)s_Player.nPatternAnim / MAX_U_PATTERN;
				float fDirection = (float)s_Player.nDirectionMove / MAX_V_PATTERN;

				//�e�N�X�`�����W�̐ݒ菈��
				Settex(pVtx, fPattren, fPattren + (1.0f / MAX_U_PATTERN), fDirection, fDirection + (1.0f / MAX_V_PATTERN));
			}
		}
		else if (s_Player.nPatternAnim == 0)
		{//�قڎ~�܂��Ă�A�w��̃e�N�X�`��
			s_Player.nCounterAnim = 0;		//�J�E���^�[�̏�����
		}
		else
		{//�قڎ~�܂��Ă�A�w��̃e�N�X�`������Ȃ�
			s_Player.nCounterAnim++;		//�J�E���^�[�����Z

			if ((s_Player.nCounterAnim % CNT_INTERVAL) == 0)
			{//��莞�Ԍo�߂���
				//�p�^�[��No.���X�V����
				if (s_Player.nPatternAnim % 2 == 1)
				{
					s_Player.nPatternAnim++;
				}

				float fPattren = (float)s_Player.nPatternAnim / MAX_U_PATTERN;
				float fDirection = (float)s_Player.nDirectionMove / MAX_V_PATTERN;

				//�e�N�X�`�����W�̐ݒ菈��
				Settex(pVtx, fPattren, fPattren + (1.0f / MAX_U_PATTERN), fDirection, fDirection + (1.0f / MAX_V_PATTERN));
			}
		}
	}
}