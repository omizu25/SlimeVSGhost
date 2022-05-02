//--------------------------------------------------
//
// ACG���� ( result.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _RESULT_H_		//���̃}�N����`������ĂȂ�������
#define _RESULT_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//--------------------------------------------------
//���U���g���`
//--------------------------------------------------
typedef enum
{
	RESULT_NONE = -1,		//�܂����܂��ĂȂ�
	RESULT_WIN,				//����
	RESULT_LOSE,			//����
	RESULT_MAX
}RESULT;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(RESULT Result);

#endif // !_RESULT_H_