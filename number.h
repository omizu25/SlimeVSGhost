//--------------------------------------------------
//
// ACG���� ( number.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _NUMBER_H_		//���̃}�N����`������ĂȂ�������
#define _NUMBER_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(void);
void SetRightNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber, int nDigit, int nRank);
void SetMiddleNumber(D3DXVECTOR3 pos, float fWidth, float fHeight, int nNumber);
void TexNumber(int nNumber, int nDigit);
void RankNumber(int nRank);
void PosNumber(D3DXVECTOR3 pos, int nNumber, int nDigit, int nRank);
void UseNumber(int nDigit);

#endif // !_NUMBER_H_