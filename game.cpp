//--------------------------------------------------
//
// ACG���� ( game.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "bg.h"
#include "block.h"
#include "enemy.h"
#include "fade.h"
#include "game.h"
#include "input.h"
#include "pause.h"
#include "player.h"

#include <assert.h>

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static GAMESTATE		s_gameState = GAMESTATE_NONE;		//�Q�[���̏��
static int				s_nCounterState;					//��ԊǗ��J�E���^�[
static bool				s_bPause = false;					//�|�[�Y�����ǂ��� [���Ă�  : true ���ĂȂ�  : false]

//--------------------------------------------------
//�Q�[���̏���������
//--------------------------------------------------
void InitGame(void)
{
	//�|�[�Y�̏���������
	InitPause();

	//�w�i�̏���������
	InitBG();

	//�u���b�N�̏���������
	InitBlock();

	//�G�̏���������
	InitEnemy();

	float fYellowWidth = ENEMY_WIDTH * 0.5f;
	float fPurpleWidth = SCREEN_WIDTH - (ENEMY_WIDTH * 0.5f);
	float fYellowHeught = (SCREEN_HEIGHT / MAX_Y_BLOCK) * TOP_BLOCK;
	float fPurpleHeught = (SCREEN_HEIGHT / MAX_Y_BLOCK) * BOTTOM_BLOCK;

	//�G�̐ݒ菈��
	SetEnemy(D3DXVECTOR3(fYellowWidth, fYellowHeught, 0.0f), ENEMYTYPE_YELLOW);
	SetEnemy(D3DXVECTOR3(fPurpleWidth, fPurpleHeught, 0.0f), ENEMYTYPE_PURPLE);

	//�v���C���[�̏���������
	InitPlayer();

	s_gameState = GAMESTATE_NORMAL;		//�ʏ��Ԃɐݒ�

	s_nCounterState = 0;				//�J�E���^�[�̏�����

	s_bPause = false;					//�|�[�Y����
}

//--------------------------------------------------
//�Q�[���̏I������
//--------------------------------------------------
void UninitGame(void)
{
	//�|�[�Y�̏I������
	UninitPause();

	//�w�i�̏I������
	UninitBG();

	//�u���b�N�̏I������
	UninitBlock();

	//�G�̏I������
	UninitEnemy();

	//�v���C���[�̏I������
	UninitPlayer();
}

//--------------------------------------------------
//�Q�[���̍X�V����
//--------------------------------------------------
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{//�|�[�Y�L�[(P�L�[)�������ꂽ���ǂ���
		s_bPause = !s_bPause;

		//�|�[�Y�̏�����
		InitPause();
	}

	if (!s_bPause)
	{//�|�[�Y���ĂȂ�
		switch (s_gameState)
		{
		case GAMESTATE_NORMAL:		//�ʏ���

			//�w�i�̍X�V����
			UpdateBG();

			//�u���b�N�̍X�V����
			UpdateBlock();

			//�G�̍X�V����
			UpdateEnemy();

			//�v���C���[�̍X�V����
			UpdatePlayer();

			break;

		case GAMESTATE_END:			//�I�����
			s_nCounterState++;

			if (s_nCounterState >= 45)
			{
				s_gameState = GAMESTATE_NONE;		//�������Ă��Ȃ���Ԃɐݒ�

				//�t�F�[�h�̐ݒ�
				SetFade(MODE_RESULT);
			}

			break;

		default:
			assert(false);
			break;
		}
	}
	else
	{//�|�[�Y��
		//�|�[�Y�̍X�V����
		UpdatePause();
	}
}

//--------------------------------------------------
//�Q�[���̕`�揈��
//--------------------------------------------------
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBG();

	//�u���b�N�̕`�揈��
	DrawBlock();

	//�G�̕`�揈��
	DrawEnemy();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	if (s_bPause)
	{//�|�[�Y��
		//�|�[�Y�̕`�揈��
		//DrawPause();
	}
}

//--------------------------------------------------
//�Q�[���̐ݒ菈��
//--------------------------------------------------
void SetGameState(GAMESTATE state)
{
	s_gameState = state;

	s_nCounterState = 0;
}

//--------------------------------------------------
//�|�[�Y�̗L�������ݒ�
//--------------------------------------------------
void SetEnablePause(bool bPause)
{
	s_bPause = bPause;
}