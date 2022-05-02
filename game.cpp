//--------------------------------------------------
//
// ACG���� ( game.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "bg.h"
#include "block.h"
#include "colon.h"
#include "effect.h"
#include "enemy.h"
#include "fade.h"
#include "game.h"
#include "gauge.h"
#include "input.h"
#include "item.h"
#include "number.h"
#include "pause.h"
#include "player.h"
#include "result.h"
#include "time.h"

#include <assert.h>

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static GAMESTATE		s_gameState = GAMESTATE_NONE;		//�Q�[���̏��
static int				s_nCounterState;					//��ԊǗ��J�E���^�[
static bool				s_bPause = false;					//�|�[�Y�����ǂ��� [���Ă�  : true ���ĂȂ�  : false]

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void SetPop(void);

//--------------------------------------------------
//�Q�[���̏���������
//--------------------------------------------------
void InitGame(void)
{
	//�|�[�Y�̏���������
	InitPause();

	//�w�i�̏���������
	InitBG();

	//�Q�[�W�̏���������
	InitGauge();

	//���̏���������
	InitNumber();

	//�R�����̏���������
	InitColon();

	//�^�C���̏���������
	InitTime();

	//�u���b�N�̏���������
	InitBlock();

	//�A�C�e���̏���������
	InitItem();

	//�G�̏���������
	InitEnemy();

	//�|�b�v����
	SetPop();

	//�G�t�F�N�g�̏���������
	//InitEffect();

	//�v���C���[�̏���������
	InitPlayer();

	s_gameState = GAMESTATE_NORMAL;		//�ʏ��Ԃɐݒ�

	s_nCounterState = 0;				//�J�E���^�[�̏�����

	s_bPause = false;					//�|�[�Y����

	//���U���g�̐ݒ菈��
	SetResult(RESULT_NONE);
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

	//�A�C�e���̏I������
	UninitItem();

	//�G�̏I������
	UninitEnemy();

	//�G�t�F�N�g�̏I������
	//UninitEffect();

	//�v���C���[�̏I������
	UninitPlayer();

	//�Q�[�W�̏I������
	UninitGauge();

	//���̏I������
	UninitNumber();

	//�R�����̏I������
	UninitColon();
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
		case GAMESTATE_NONE:		//�������Ă��Ȃ����

			break;

		case GAMESTATE_NORMAL:		//�ʏ���

			//�^�C���̉��Z����
			AddTime(1);

			//�w�i�̍X�V����
			UpdateBG();

			//�u���b�N�̍X�V����
			UpdateBlock();

			//�A�C�e���̍X�V����
			UpdateItem();

			//�G�̍X�V����
			UpdateEnemy();

			//�G�t�F�N�g�̍X�V����
			//UpdateEffect();

			//�v���C���[�̍X�V����
			UpdatePlayer();

			//�Q�[�W�̍X�V����
			UpdateGauge();

			//���̍X�V����
			UpdateNumber();

			//�R�����̍X�V����
			UpdateColon();

			break;

		case GAMESTATE_END:			//�I�����

			//�Q�[�W�̍X�V����
			UpdateGauge();

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

	//�A�C�e���̕`�揈��
	DrawItem();

	//�G�̕`�揈��
	DrawEnemy();

	//�G�t�F�N�g�`�揈��
	//DrawEffect();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�Q�[�W�̕`�揈��
	DrawGauge();

	//���̕`�揈��
	DrawNumber();

	//�R�����̕`�揈��
	DrawColon();

	if (s_bPause)
	{//�|�[�Y��
#ifdef DEBUG

		//�|�[�Y�̕`�揈��
		DrawPause();

#endif // DEBUG
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

//--------------------------------------------------
//�|�b�v����
//--------------------------------------------------
static void SetPop(void)
{
	float fEnemyWidth = ENEMY_WIDTH * 0.5f;
	float fBlockHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK);

	D3DXVECTOR3 posBoy  = D3DXVECTOR3(fEnemyWidth, fBlockHeight * TOP_BLOCK, 0.0f);
	D3DXVECTOR3 posGirl = D3DXVECTOR3(SCREEN_WIDTH - fEnemyWidth, fBlockHeight * BOTTOM_BLOCK, 0.0f);

	//�G�̐ݒ菈��
	SetEnemy(posBoy, ENEMYTYPE_BOY);
	SetEnemy(posGirl, ENEMYTYPE_GIRL);
}