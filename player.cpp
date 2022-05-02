//--------------------------------------------------
//
// �A�N�V�����Q�[������ ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "input.h"
#include "player.h"

#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define PLAYER_WIDTH		(80.0f)			//�v���C���[�̕�
#define PLAYER_HEIGHT		(70.0f)			//�v���C���[�̍���
#define PLAYER_MOVE			(0.3f)			//�v���C���[�̈ړ���
#define MAX_U_PATTERN		(4)				//U�p�^�[���̍ő吔
#define MAX_V_PATTERN		(2)				//V�p�^�[���̍ő吔
#define MIN_MOVE			(0.30f)			//�����Ă�ŏ��l
#define MAX_JUMP			(-27.5f)		//�W�����v��
#define MAX_BOUND			(5.0f)			//�o�E���h�̍ő��
#define MAX_GRAVITY			(1.5f)			//�d�͂̍ő�l
#define MAX_INERTIA			(0.05f)			//�����̍ő�l
#define CNT_INTERVAL		(7)				//�J�E���^�[�̃C���^�[�o��

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;		//���_�o�b�t�@�̃|�C���^
static Player						s_Player;				//�v���C���[�̏��
static bool							s_bTexUse;				//�e�N�X�`���̎g�p���邩�ǂ���

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStructPlayer(void);
static void MovePlayer(VERTEX_2D *pVtx);
static void OffScreenPlayer(void);
static void BoundPlayer(void);
static void MotionPlayer(void);

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
		"Data\\TEXTURE\\player001.png",
		&s_pTexture);

	s_bTexUse = true;

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
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//rhw�̐ݒ菈��
	Setrhw(pVtx);

	//���_�J���[�̐ݒ菈��
	Setcol(pVtx, 1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ菈��
	Settex(pVtx, 0.0f, 0.5f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
//�v���C���[�̏I������
//--------------------------------------------------
void UninitPlayer(void)
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

	//�v���C���[�̃��[�V��������
	MotionPlayer();

	//���_���W�̐ݒ菈��
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

#ifdef  _DEBUG

	if (GetKeyboardTrigger(DIK_F2))
	{//F2�L�[(F2�L�[)�������ꂽ���ǂ���
		s_bTexUse = !s_bTexUse;
	}

#endif //  _DEBUG
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
	if (s_bTexUse)
	{//�g�p����
		pDevice->SetTexture(0, s_pTexture);
	}
	else
	{//�g�p���Ȃ�
		pDevice->SetTexture(0, NULL);
	}

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��
}

//--------------------------------------------------
//�v���C���[�̎擾����
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_Player;
}

//--------------------------------------------------
//�e�N�X�`�����g�p���邩�̎擾����
//--------------------------------------------------
bool GetTexUsePlayer(void)
{
	return s_bTexUse;
}

//--------------------------------------------------
//�v���C���[�̍\���̂̏���������
//--------------------------------------------------
static void InitStructPlayer(void)
{
	//���݂̈ʒu��������
	s_Player.pos.x = PLAYER_WIDTH * 0.5f;
	s_Player.pos.y = (SCREEN_HEIGHT / MAX_Y_BLOCK) * (MAX_Y_BLOCK - 1);
	s_Player.pos.z = 0.0f;

	s_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu��������
	s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ�������
	s_Player.jump = JUMPSTATE_NONE;							//�������Ă��Ȃ���Ԃɂ���
	s_Player.fWidth = PLAYER_WIDTH * 0.5f;					//���̏�����
	s_Player.fHeight = PLAYER_HEIGHT;						//�����̏�����
	s_Player.nCounterState = 0;								//�J�E���^�[�̏�����
}

//--------------------------------------------------
//�v���C���[�̈ړ�����
//--------------------------------------------------
static void MovePlayer(VERTEX_2D *pVtx)
{
	//�L�[���͂ł̈ړ�
	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT))
	{//A�L�[�������ꂽ
		switch (s_Player.jump)
		{//�ړ��ʂ��X�V (����������)
		case JUMPSTATE_NONE:		//�������Ă��Ȃ�

			s_Player.move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_MOVE;
			s_Player.move.y += cosf(-D3DX_PI * 0.5f) * PLAYER_MOVE;

			break;

		case JUMPSTATE_JUMP:		//�W�����v
		case JUMPSTATE_BOUND:		//�o�E���h

			s_Player.move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;
			s_Player.move.y += cosf(-D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;

			break;

		default:
			assert(false);
			break;
		}

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.5f, 1.0f, 0.0f, 1.0f);
	}
	else if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT))
	{//D�L�[�������ꂽ
		switch (s_Player.jump)
		{//�ړ��ʂ��X�V (����������)
		case JUMPSTATE_NONE:		//�������Ă��Ȃ�

			s_Player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_MOVE;
			s_Player.move.y += cosf(D3DX_PI * 0.5f) * PLAYER_MOVE;

			break;

		case JUMPSTATE_JUMP:		//�W�����v
		case JUMPSTATE_BOUND:		//�o�E���h

			s_Player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;
			s_Player.move.y += cosf(D3DX_PI * 0.5f) * PLAYER_MOVE * 0.75f;

			break;

		default:
			assert(false);
			break;
		}

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.0f, 0.5f, 0.0f, 1.0f);
	}

	bool bDown = false;

	//�W�����v����
	if (s_Player.jump == JUMPSTATE_NONE)
	{//�������ĂȂ�
		if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN))
		{
			bDown = true;
		}
		else if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_W) || 
				 GetJoypadTrigger(JOYKEY_B))
		{//�X�y�[�X�L�[�AW�L�[�������ꂽ
			s_Player.move.y += MAX_JUMP;
			s_Player.jump = JUMPSTATE_JUMP;
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.nCounterState = 0;
		}
	}

	//�d��
	s_Player.move.y += MAX_GRAVITY;

	//�X�e�B�b�N�ł̈ړ��ʂ̍X�V
	s_Player.move.x += GetJoypadStick(JOYKEY_L_STICK).x;

	//�O��̈ʒu�̋L��
	s_Player.posOld = s_Player.pos;

	//�ʒu���X�V
	s_Player.pos.x += s_Player.move.x;
	s_Player.pos.y += s_Player.move.y;

	//�u���b�N�̓����蔻�菈��
	if (CollisionBlock(&s_Player.pos, &s_Player.posOld, &s_Player.move, s_Player.fWidth, s_Player.fHeight))
	{//�u���b�N�̏�[�ɂ��鎞
		//�v���C���[�̃o�E���h����
		BoundPlayer();
	}
	else
	{//��
		if (s_Player.jump == JUMPSTATE_NONE)
		{//�������Ă��Ȃ�
			s_Player.jump = JUMPSTATE_JUMP;
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.nCounterState = 0;
		}
	}

	if (bDown)
	{//�~��還
		//�u���b�N�̏�[�̓����蔻��
		CollisionTopBlock(&s_Player.pos, s_Player.fWidth, s_Player.fHeight);
	}

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

		//�v���C���[�̃o�E���h����
		BoundPlayer();
	}
	else if (s_Player.pos.y <= PLAYER_HEIGHT)
	{//��[
		s_Player.pos.y = PLAYER_HEIGHT;
		s_Player.move.y *= -REFLECT_BOUND;
	}

	if (s_Player.pos.x >= SCREEN_WIDTH - (PLAYER_WIDTH * 0.5f))
	{//�E�[
		s_Player.pos.x = SCREEN_WIDTH - (PLAYER_WIDTH * 0.5f);
	}
	else if (s_Player.pos.x <= PLAYER_WIDTH * 0.5f)
	{//���[
		s_Player.pos.x = PLAYER_WIDTH * 0.5f;
	}
}

//--------------------------------------------------
//�v���C���[�̃o�E���h����
//--------------------------------------------------
static void BoundPlayer(void)
{
	switch (s_Player.jump)
	{
	case JUMPSTATE_NONE:		//�������Ă��Ȃ�
		s_Player.move.y = 0.0f;
		break;

	case JUMPSTATE_JUMP:		//�W�����v
		s_Player.jump = JUMPSTATE_BOUND;

		//break����

	case JUMPSTATE_BOUND:		//�o�E���h

		s_Player.move.y *= -REFLECT_BOUND;

		if (s_Player.move.y >= MAX_JUMP * powf(REFLECT_BOUND, MAX_BOUND))
		{//�o�E���h���I�����
			s_Player.jump = JUMPSTATE_NONE;
			s_Player.move.y = 0.0f;
			s_Player.nCounterState = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//�v���C���[�̃��[�V��������
//--------------------------------------------------
static void MotionPlayer(void)
{
	Block *pBlock = GetBlock();		//�u���b�N�̏���������
	float fPosY = 0.0f;
	float fDif = 0.0f;
	float fDifOld = SCREEN_HEIGHT;

	switch (s_Player.jump)
	{
	case JUMPSTATE_NONE:		//�������Ă��Ȃ�
		s_Player.nCounterState++;
		s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterState * 0.01f) * (D3DX_PI * 2.0f)) * 5.0f);
		break;

	case JUMPSTATE_JUMP:		//�W�����v
	case JUMPSTATE_BOUND:		//�o�E���h

		for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
		{
			if (!pBlock->bUse)
			{//�u���b�N���g�p����Ă��Ȃ�
				continue;
			}

			//�u���b�N���g�p����Ă���

			if (s_Player.pos.y < (pBlock->pos.y - pBlock->fHeight) &&
				(s_Player.pos.x + s_Player.fWidth) > (pBlock->pos.x - pBlock->fWidth) &&
				(s_Player.pos.x - s_Player.fWidth) < (pBlock->pos.x + pBlock->fWidth))
			{//�v���C���[�̉����̃u���b�N�̎�
				//�����v�Z
				fDif = (pBlock->pos.y - pBlock->fHeight) - s_Player.pos.y;

				if (fDif < fDifOld)
				{//�������������������
					fDifOld = fDif;
					fPosY = pBlock->pos.y - pBlock->fHeight;
				}
			}
		}

		if (s_Player.pos.y <= fPosY && s_Player.pos.y >= (fPosY - (SCREEN_HEIGHT / MAX_Y_BLOCK)))
		{//���̃u���b�N����P�u���b�N��͈̔͂Ȃ�
			s_Player.nCounterState++;
		}

		s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterState * 0.01f) * (D3DX_PI * 2.0f)) * 3.0f * -s_Player.move.y);

		break;

	default:
		assert(false);
		break;
	}
}