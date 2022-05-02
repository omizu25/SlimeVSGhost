//--------------------------------------------------
//
// ACG���� ( time.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "colon.h"
#include "game.h"
#include "number.h"
#include "result.h"
#include "time.h"

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define NUMBER_WIDTH		(30.0f)		//���̕�
#define NUMBER_HEIGHT		(50.0f)		//���̍���
#define WIDTH_INTERVAL		(20.0f)		//���̊Ԋu

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static D3DXVECTOR3		s_pos;			//�ʒu
static int				s_nTime;		//�^�C���̒l

//--------------------------------------------------
//�^�C���̏���������
//--------------------------------------------------
void InitTime(void)
{
	float fWidth = SCREEN_WIDTH / MAX_X_BLOCK;
	float fHeight = (SCREEN_HEIGHT / MAX_Y_BLOCK) * 0.5f;

	//�ʒu������������
	s_pos = D3DXVECTOR3(fWidth * 30.0f, fHeight * 2.0f, 0.0f);

	s_nTime = 0;

	int nTime = s_nTime;
	int aNumber[MAX_TIME];

	for (int i = 0; i < MAX_TIME; i++)
	{//�P�����ɕ�����
		aNumber[i] = nTime % 10;
		nTime /= 10;

		float fInterval = (NUMBER_WIDTH * i) + (WIDTH_INTERVAL * (i / 2));

		//���̐ݒ菈��
		SetRightNumber(D3DXVECTOR3(s_pos.x - fInterval, s_pos.y, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT * 0.5f, aNumber[i], i, 0);

		if ((i % 2 == 0) && (i != 0))
		{//�Q�̔{��
			//�R�����̐ݒ菈��
			SetColon(D3DXVECTOR3(s_pos.x - fInterval + WIDTH_INTERVAL, s_pos.y, 0.0f), WIDTH_INTERVAL, NUMBER_HEIGHT * 0.5f, 0);
		}
	}
}

//--------------------------------------------------
//�^�C���̉��Z����
//--------------------------------------------------
void AddTime(int nValue)
{
	s_nTime += nValue;

	if (s_nTime % 10000 == 6000)
	{//�P������
		s_nTime += 4000;
	}

	if (s_nTime % 1000000 >= 300000)
	{//�R�O���ȏ�
		//���U���g�̐ݒ菈��
		SetResult(RESULT_LOSE);

		//�Q�[���̐ݒ菈��
		SetGameState(GAMESTATE_END, 45);
	}

	int nTime = s_nTime;
	int i = 0;
	int aNumber[MAX_TIME];

	while (nTime != 0)
	{//�P�����ɕ�����
		aNumber[i] = nTime % 10;
		nTime /= 10;

		//���̃e�N�X�`���̏���
		TexNumber(aNumber[i], i);

		i++;
	}
}

//--------------------------------------------------
//�^�C���̎擾
//--------------------------------------------------
int GetTime(void)
{
	return s_nTime;
}