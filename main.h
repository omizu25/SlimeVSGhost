//--------------------------------------------------
//
// �A�N�V�����Q�[������ ( main.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _MAIN_H_		//���̃}�N����`������ĂȂ�������
#define _MAIN_H_		//�Q�d�C���N���[�h�h�~�̃}�N����`

#include <windows.h>
#include "d3dx9.h"									//�`�揈���ɕK�v
#define  DIRECTINPUT_VERSION		(0x0800)		//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"									//���͏����ɕK�v
#include "xinput.h"									//�W���C�p�b�h�����ɕK�v
#include "xaudio2.h"								//�T�E���h�����ɕK�v

//--------------------------------------------------
//���C�u�����̃����N
//--------------------------------------------------
#pragma comment(lib, "d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		//�V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		//���͏����ɕK�v
#pragma comment(lib, "xinput.lib")		//�W���C�p�b�h�����ɕK�v

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define SCREEN_WIDTH		(1280)												//�E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)												//�E�C���h�E�̍���
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//���_�t�H�[�}�b�g[2D]

//--------------------------------------------------
//���_���[2D]�̍\���̂��`
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;		//���_���W
	float			rhw;		//���W�ϊ��p�W�� (1.0f�ŌŒ�)
	D3DCOLOR		col;		//���_�J���[
	D3DXVECTOR2		tex;		//���_���W
}VERTEX_2D;

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight);
void Setrhw(VERTEX_2D *pVtx);
void Setcol(VERTEX_2D *pVtx, float Red, float Green, float Blue, float Alpha);
void Settex(VERTEX_2D *pVtx, float ULeft, float URight, float VTop, float VBottom);

#endif // !_MAIN_H_