//--------------------------------------------------
//
// ACG���� ( ranking.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _RANKING_H_		//���̃}�N����`������ĂȂ�������
#define _RANKING_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SaveRanking(void);
void SetRanking(int nRankTime);
void InitRankUpdate(void);

#endif // !_RANKING_H_