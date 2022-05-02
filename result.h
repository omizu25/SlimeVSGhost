//--------------------------------------------------
//
// STG���� ( result.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _RESULT_H_		//���̃}�N����`������ĂȂ�������
#define _RESULT_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//-------------------------
//�}�N����`
//-------------------------
#define MAX_ROUTE		(5)		//���[�g�̍ő吔

//-------------------------
//���[�g�̌������`
//-------------------------
typedef enum
{
	ROUTE_NONE = -1,	//�܂����[�g�����܂��ĂȂ�
	ROUTE_TRUE = 0,		//�g�D���[�G���h(�ʏ�)
	ROUTE_HAPPY,		//�n�b�s�[�G���h(�{�X�����|��)
	ROUTE_ALL_KILL,		//�I�[���L���G���h(�S���|��)
	ROUTE_NO_KILL,		//NO�L���G���h(�{�X����|���Ȃ�)
	ROUTE_ESCAPE,		//�{�X�ɓ�������(���Ԑ؂�)
	ROUTE_BAD,			//�o�b�h�G���h(��)
	ROUTE_HAPPYBAD,		//�n�b�s�[�G���h�ł̎�
	ROUTE_MAX
}ROUTE;

//-------------------------
//�v���g�^�C�v�錾
//-------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetResult(ROUTE Route);
ROUTE GetResult(void);

#endif // !_RESULT_H_