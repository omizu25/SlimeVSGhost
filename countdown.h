//--------------------------------------------------
//
// ACG���� ( countdown.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _COUNTDOWN_H_		//���̃}�N����`������ĂȂ�������
#define _COUNTDOWN_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitCountdown(void);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
void AddCountdown(int nValue);
void SetCountdown(int Countdown);
bool GetCountdown(void);

#endif // !_COUNTDOWN_H_