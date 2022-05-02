//--------------------------------------------------
//
// ACG���� ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "effect.h"
#include "game.h"
#include "gauge.h"
#include "input.h"
#include "item.h"
#include "player.h"
#include "result.h"
#include "setup.h"
#include "sound.h"

#include <assert.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define PLAYER_WIDTH		(80.0f)			//�v���C���[�̕�
#define PLAYER_HEIGHT		(70.0f)			//�v���C���[�̍���
#define MAX_MOVE			(0.3f)			//�ړ��ʂ̍ő�l
#define MIN_MOVE			(0.1f)			//�ړ��ʂ̍ŏ��l
#define EFFECT_MOVE			(2.0f)			//�G�t�F�N�g�̏o��ړ���
#define MAX_TEX				(3)				//�e�N�X�`���̍ő吔
#define MAX_U_PATTERN		(2)				//U�p�^�[���̍ő吔
#define MAX_JUMP			(-27.5f)		//�W�����v��
#define MAX_GRAVITY			(1.5f)			//�d�͂̍ő�l
#define MAX_INERTIA			(0.05f)			//�����̍ő�l
#define MAX_INTERVAL		(7)				//�J�E���^�[�̃C���^�[�o��

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(void);
static void UpdateState(VERTEX_2D *pVtx);
static void UpdateMove(VERTEX_2D *pVtx);
static void UpdateAttack(void);
static bool UpdateUpDown(void);
static void UpdateOffScreen(void);
static void UpdateBound(void);
static void UpdateMotion(void);
static void UpdateEffect(void);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[MAX_TEX];		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
static Player						s_Player;					//�v���C���[�̏��

//--------------------------------------------------
//�v���C���[�̏���������
//--------------------------------------------------
void InitPlayer(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�������̃N���A
	memset(&s_pTexture[0], NULL, sizeof(s_pTexture));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player002.png",
		&s_pTexture[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player004.png",
		&s_pTexture[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\Player006.png",
		&s_pTexture[2]);

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

	//�������̃N���A
	memset(&s_Player, NULL, sizeof(Player));

	//�\���̂̏���������
	InitStruct();

	//���_���W�̐ݒ菈��
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	float fTex = (float)s_Player.bDirection / MAX_U_PATTERN;

	//�e�N�X�`�����W�̐ݒ菈��
	Settex(pVtx, 0.0f + fTex, (1.0f / MAX_U_PATTERN) + fTex, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//�Q�[�W�̐ݒ菈��
	SetGauge(GAUGEUSE_PLAYER, s_Player.nLife);
}

//--------------------------------------------------
//�v���C���[�̏I������
//--------------------------------------------------
void UninitPlayer(void)
{
	//�T�E���h�̒�~
	StopSound();

	for (int i = 0; i < MAX_TEX; i++)
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
//�v���C���[�̍X�V����
//--------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//��ԏ���
	UpdateState(pVtx);

	//�ړ�����
	UpdateMove(pVtx);

	//���[�V��������
	UpdateMotion();

	//���_���W�̐ݒ菈��
	SetBottompos(pVtx, s_Player.pos, s_Player.fWidth, s_Player.fHeight);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//�G�t�F�N�g����
	UpdateEffect();
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

	switch (s_Player.state)
	{
	case PLAYERSTATE_NORMAL:		//�ʏ���

		//�|���S���ƃe�N�X�`���̃����܂���
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		break;

	case PLAYERSTATE_DAMAGE:		//�_���[�W���
	case PLAYERSTATE_STAR:			//���G���

		//�|���S���ƃe�N�X�`���̃��𑫂�
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

		break;

	default:
		assert(false);
		break;
	}
	
	switch (s_Player.attack)
	{
	case ATTACKSTATE_NONE:			//�������Ă��Ȃ����
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[0]);

		break;

	case ATTACKSTATE_IN:			//�z������ł���
	case ATTACKSTATE_OUT:			//�f���o�����
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[1]);

		break;

	case ATTACKSTATE_STORE:			//�~���Ă�����
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[2]);

		break;

	default:
		assert(false);
		break;
	}

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);							//�v���~�e�B�u(�|���S��)��

	//�|���S���ƃe�N�X�`���̃����܂���
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
}

//--------------------------------------------------
//�v���C���[�̎擾����
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_Player;
}

//-------------------------
//�v���C���[�̃q�b�g����
//-------------------------
void HitPlayer(int nDamage)
{
	if (s_Player.state == PLAYERSTATE_NORMAL)
	{
		s_Player.nLife -= nDamage;

		//�Q�[�W�̌��Z����
		SubGauge(GAUGEUSE_PLAYER, s_Player.nLife);

		if (s_Player.nLife <= 0)
		{//�v���C���[�̗̑͂��Ȃ��Ȃ���
			//���U���g�̐ݒ菈��
			SetResult(RESULT_LOSE);

			//�Q�[���̐ݒ菈��
			SetGameState(GAMESTATE_END);
		}
		else
		{//�܂������Ă�
			s_Player.state = PLAYERSTATE_DAMAGE;

			s_Player.nCounterState = 0;

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

			//���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 1.0f, 0.25f, 0.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();
		}
	}
}

//--------------------------------------------------
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(void)
{
	//���݂̈ʒu��������
	s_Player.pos.x = SCREEN_WIDTH * 0.5f;
	s_Player.pos.y = (SCREEN_HEIGHT / MAX_Y_BLOCK) * MIDDLE_BLOCK;
	s_Player.pos.z = 0.0f;

	s_Player.fWidth = PLAYER_WIDTH * 0.5f;					//���̏�����
	s_Player.fHeight = PLAYER_HEIGHT;						//�����̏�����
	s_Player.nLife = 100;									//�����̏�����
	s_Player.bDirection = true;								//�E����

	//���̂�memset��0�ɂ����B
}

//--------------------------------------------------
//��ԏ���
//--------------------------------------------------
static void UpdateState(VERTEX_2D *pVtx)
{
	switch (s_Player.state)
	{
	case PLAYERSTATE_NORMAL:		//�ʏ���

		break;

	case PLAYERSTATE_DAMAGE:		//�_���[�W���

		s_Player.nCounterState++;

		if (s_Player.nCounterState >= 15)
		{
			//���_�J���[�̏���������
			Initcol(pVtx);

			s_Player.state = PLAYERSTATE_STAR;
			s_Player.nCounterState = 0;
		}

		break;

	case PLAYERSTATE_STAR:			//���G���

		s_Player.nCounterState++;

		s_Player.fCol = 0.3f + sinf((s_Player.nCounterState * 0.05f) * (D3DX_PI * 2.0f)) * 0.2f ;

		//���_�J���[�̐ݒ菈��
		Setcol(pVtx, s_Player.fCol, s_Player.fCol, s_Player.fCol, 1.0f);

		if (s_Player.nCounterState >= 100)
		{
			//���_�J���[�̏���������
			Initcol(pVtx);

			s_Player.state = PLAYERSTATE_NORMAL;
			s_Player.nCounterState = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//�ړ�����
//--------------------------------------------------
static void UpdateMove(VERTEX_2D *pVtx)
{
	//�U������
	UpdateAttack();

	if (s_Player.attack != ATTACKSTATE_IN && s_Player.attack != ATTACKSTATE_OUT)
	{//�z������łȂ�
		//�L�[���͂ł̈ړ�
		if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT))
		{//A�L�[�������ꂽ
			switch (s_Player.attack)
			{//�ړ��ʂ��X�V (����������)
			case ATTACKSTATE_NONE:			//�������Ă��Ȃ����
				s_Player.move.x += sinf(-D3DX_PI * 0.5f) * MAX_MOVE;
				s_Player.move.y += cosf(-D3DX_PI * 0.5f) * MAX_MOVE;

				break;

			case ATTACKSTATE_STORE:			//�~���Ă�����
				s_Player.move.x += sinf(-D3DX_PI * 0.5f) * MIN_MOVE;
				s_Player.move.y += cosf(-D3DX_PI * 0.5f) * MIN_MOVE;
				
				break;

			case ATTACKSTATE_IN:			//�z������ł���
			case ATTACKSTATE_OUT:			//�f���o�����

				//���if�ł����͒ʂ�Ȃ�����break�Ȃ��ɂ���assert���܁[���B

			default:
				assert(false);
				break;
			}

			s_Player.bDirection = false;		//������

		}
		else if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT))
		{//D�L�[�������ꂽ
			switch (s_Player.attack)
			{//�ړ��ʂ��X�V (����������)
			case ATTACKSTATE_NONE:			//�������Ă��Ȃ����
				s_Player.move.x += sinf(D3DX_PI * 0.5f) * MAX_MOVE;
				s_Player.move.y += cosf(D3DX_PI * 0.5f) * MAX_MOVE;

				break;

			case ATTACKSTATE_STORE:			//�~���Ă�����
				s_Player.move.x += sinf(D3DX_PI * 0.5f) * MIN_MOVE;
				s_Player.move.y += cosf(D3DX_PI * 0.5f) * MIN_MOVE;

				break;

			case ATTACKSTATE_IN:			//�z������ł���
			case ATTACKSTATE_OUT:			//�f���o�����

				//���if�ł����͒ʂ�Ȃ�����break�Ȃ��ɂ���assert���܁[���B

			default:
				assert(false);
				break;
			}
			
			s_Player.bDirection = true;		//�E����
		}

		float fTex = (float)s_Player.bDirection / MAX_U_PATTERN;

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, 0.0f + fTex, (1.0f / MAX_U_PATTERN) + fTex, 0.0f, 1.0f);

		//�W�����v�A�~��鏈��
		bool bDown = UpdateUpDown();

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
			//�o�E���h����
			UpdateBound();
		}
		else
		{//��
			if (s_Player.jump == JUMPSTATE_NONE)
			{//�������Ă��Ȃ�
				s_Player.jump = JUMPSTATE_JUMP;
				s_Player.fHeight = PLAYER_HEIGHT;
				s_Player.nCounterMotion = 0;
			}
		}

		if (bDown)
		{//�~���
			//�u���b�N�̏�[�̓����蔻��
			CollisionTopBlock(&s_Player.pos, s_Player.fWidth, s_Player.fHeight);
		}

		//�����E�ړ��ʂ��X�V (����������)
		s_Player.move.x += (0.0f - s_Player.move.x) * MAX_INERTIA;
	}

	//��ʊO����
	UpdateOffScreen();
}

//--------------------------------------------------
//�U������
//--------------------------------------------------
static void UpdateAttack(void)
{
	switch (s_Player.attack)
	{
	case ATTACKSTATE_NONE:			//�������Ă��Ȃ����
		if (s_Player.jump == JUMPSTATE_NONE)
		{//�������ĂȂ�
			if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
			{//ENTER�L�[�������ꂽ
				s_Player.attack = ATTACKSTATE_IN;
				s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}

		break;

	case ATTACKSTATE_IN:			//�z������ł���
		if (GetKeyboardPress(DIK_RETURN) || GetJoypadPress(JOYKEY_B))
		{//ENTER�L�[�������ꂽ
			//�A�C�e���̋z�����ݏ���
			InhaleItem(s_Player.pos, &s_Player.attack, s_Player.fWidth, s_Player.fHeight, s_Player.bDirection);

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_PUNCH);

			s_Player.nCounterAttack = 0;
		}
		else if (s_Player.nCounterAttack <= MAX_INTERVAL)
		{//�]�C��
			//�A�C�e���̋z�����ݏ���
			InhaleItem(s_Player.pos, &s_Player.attack, s_Player.fWidth, s_Player.fHeight, s_Player.bDirection);
		}
		else
		{//�z�����߂ĂȂ�
			s_Player.attack = ATTACKSTATE_NONE;
			s_Player.nCounterAttack = 0;
		}

		s_Player.nCounterAttack++;

		break;

	case ATTACKSTATE_STORE:			//�~���Ă�����
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_B))
		{//ENTER�L�[�������ꂽ
			//�A�C�e���̐ݒ菈��
			SetItem(s_Player.pos - D3DXVECTOR3(0.0f, s_Player.fHeight * 0.5f, 0.0f), ITEMTYPE_STAR, s_Player.bDirection);
			s_Player.attack = ATTACKSTATE_OUT;
		}

		break;

	case ATTACKSTATE_OUT:			//�f���o�����
		s_Player.nCounterAttack++;

		if (s_Player.nCounterAttack >= MAX_INTERVAL)
		{
			s_Player.attack = ATTACKSTATE_NONE;
			s_Player.nCounterAttack = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//�W�����v�A�~��鏈��
//--------------------------------------------------
static bool UpdateUpDown(void)
{
	bool bDown = false;

	if (s_Player.attack == ATTACKSTATE_NONE && s_Player.jump == JUMPSTATE_NONE)
	{//�������Ă��Ȃ���Ԃ�
		if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN))
		{//S�L�[�������ꂽ
			bDown = true;
		}
		else if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_W) ||
			GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_UP))
		{//�X�y�[�X�L�[�AW�L�[�������ꂽ
			s_Player.move.y += MAX_JUMP;
			s_Player.jump = JUMPSTATE_JUMP;
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.nCounterMotion = 0;

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_JUMP);
		}
	}

	return bDown;
}

//--------------------------------------------------
//��ʊO����
//--------------------------------------------------
static void UpdateOffScreen(void)
{
	//��ʒ[����
	if (s_Player.pos.y >= SCREEN_HEIGHT)
	{//���[
		s_Player.pos.y = SCREEN_HEIGHT;

		//�o�E���h����
		UpdateBound();
	}
	else if (s_Player.pos.y <= PLAYER_HEIGHT)
	{//��[
		s_Player.pos.y = PLAYER_HEIGHT;
		s_Player.move.y = 0.0f;
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
//�o�E���h����
//--------------------------------------------------
static void UpdateBound(void)
{
	switch (s_Player.jump)
	{
	case JUMPSTATE_NONE:		//�������Ă��Ȃ�
		s_Player.move.y = 0.0f;
		break;

	case JUMPSTATE_JUMP:		//�W�����v
		s_Player.jump = JUMPSTATE_LAND;

		break;

	case JUMPSTATE_LAND:		//���n

		if (s_Player.fWidth <= PLAYER_WIDTH * 0.5f)
		{
			s_Player.fHeight = PLAYER_HEIGHT;
			s_Player.fWidth = PLAYER_WIDTH * 0.5f;
			s_Player.jump = JUMPSTATE_NONE;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//���[�V��������
//--------------------------------------------------
static void UpdateMotion(void)
{
	if (s_Player.attack == ATTACKSTATE_IN)
	{//�z������ł�
		s_Player.fHeight = PLAYER_HEIGHT * 1.5f;
	}
	else if (s_Player.attack == ATTACKSTATE_STORE)
	{//�z������
		s_Player.nCounterMotion++;
		s_Player.fHeight = (PLAYER_HEIGHT * 1.5f) + (sinf((s_Player.nCounterMotion * 0.01f) * (D3DX_PI * 2.0f)) * 5.0f);
	}
	else
	{//�z������ł��Ȃ�
		Block *pBlock = GetBlock();		//�u���b�N�̏���������
		float fPosY = 0.0f;
		float fDif = 0.0f;
		float fDifOld = SCREEN_HEIGHT;

		switch (s_Player.jump)
		{
		case JUMPSTATE_NONE:		//�������Ă��Ȃ�
			s_Player.nCounterMotion++;
			s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterMotion * 0.01f) * (D3DX_PI * 2.0f)) * 5.0f);
			break;

		case JUMPSTATE_JUMP:		//�W�����v

			for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
			{
				if (!pBlock->bUse)
				{//�u���b�N���g�p����Ă��Ȃ�
					continue;
				}

				//�u���b�N���g�p����Ă���

				if (s_Player.pos.y < (pBlock->pos.y - pBlock->fHeight) &&
					(s_Player.pos.x + s_Player.fWidth) >(pBlock->pos.x - pBlock->fWidth) &&
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

			if (s_Player.pos.y <= (fPosY + ((SCREEN_HEIGHT / MAX_Y_BLOCK) * 2.0f)) && s_Player.pos.y >= (fPosY - ((SCREEN_HEIGHT / MAX_Y_BLOCK) * 2.0f)))
			{//���̃u���b�N����P�u���b�N��͈̔́A������
				s_Player.nCounterMotion++;
			}

			s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterMotion * 0.04f) * (D3DX_PI * 2.0f)) * 15.0f);
			s_Player.fWidth = PLAYER_WIDTH * 0.5f + (sinf((s_Player.nCounterMotion * 0.02f) * (D3DX_PI * 2.0f)) * 2.0f);

			break;

		case JUMPSTATE_LAND:		//���n
			s_Player.nCounterMotion++;
			s_Player.fHeight = PLAYER_HEIGHT + (sinf((s_Player.nCounterMotion * 0.04f) * (D3DX_PI * 2.0f)) * 15.0f);
			s_Player.fWidth = PLAYER_WIDTH * 0.5f + (sinf((s_Player.nCounterMotion * 0.02f) * (D3DX_PI * 2.0f)) * 5.0f);

			break;

		default:
			assert(false);
			break;
		}
	}
}

//--------------------------------------------------
//�G�t�F�N�g����
//--------------------------------------------------
static void UpdateEffect(void)
{
	if (s_Player.move.x >= EFFECT_MOVE || s_Player.move.x <= -EFFECT_MOVE)
	{//�ړ���
		if (s_Player.nCounterMotion % 2 == 0)
		{
			//�G�t�F�N�g�̐ݒ菈��
			SetEffect(s_Player.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f),
				s_Player.fWidth, s_Player.fHeight * 0.2f, 10, EFFECTTYPE_MOVE);
		}
	}

	if (s_Player.attack == ATTACKSTATE_IN)
	{//�z������ł�
		SetParticle(s_Player.pos + D3DXVECTOR3(0.0f, -(s_Player.fHeight * 0.325f), 0.0f), EFFECTTYPE_IN, s_Player.bDirection);
	}
}