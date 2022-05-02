//--------------------------------------------------
//
// ACG���� ( setup.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "setup.h"

//--------------------------------------------------
//���_���W�̐ݒ菈�� (�^�񒆂ɒ��S)
//--------------------------------------------------
void SetMiddlepos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth,  fHeight, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ菈�� (���ɒ��S)
//--------------------------------------------------
void SetBottompos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3( fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,     0.0f, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3( fWidth,     0.0f, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ菈�� (���ɒ��S)
//--------------------------------------------------
void SetLeftpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(  0.0f, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(  0.0f,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(fWidth,  fHeight, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ菈�� (�E�ɒ��S)
//--------------------------------------------------
void SetRightpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(   0.0f, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth,  fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(   0.0f,  fHeight, 0.0f);
}

//--------------------------------------------------
//���_���W�̐ݒ菈�� (��])
//--------------------------------------------------
void SetRotpos(VERTEX_2D *pVtx, D3DXVECTOR3 pos, float fRot, float fLength, float fAngle)
{
	//���_���W�̍X�V
	pVtx[0].pos.x = pos.x + sinf(fRot + (-D3DX_PI + fAngle)) * fLength;
	pVtx[0].pos.y = pos.y + cosf(fRot + (-D3DX_PI + fAngle)) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(fRot + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.y = pos.y + cosf(fRot + (D3DX_PI - fAngle)) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(fRot + (fAngle * -1.0f)) * fLength;
	pVtx[2].pos.y = pos.y + cosf(fRot + (fAngle * -1.0f)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(fRot + fAngle) * fLength;
	pVtx[3].pos.y = pos.y + cosf(fRot + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;
}

//--------------------------------------------------
//���_�J���[�̐ݒ菈��
//--------------------------------------------------
void Setcol(VERTEX_2D *pVtx, float Red, float Green, float Blue, float Alpha)
{
	pVtx[0].col = D3DXCOLOR(Red, Green, Blue, Alpha);
	pVtx[1].col = D3DXCOLOR(Red, Green, Blue, Alpha);
	pVtx[2].col = D3DXCOLOR(Red, Green, Blue, Alpha);
	pVtx[3].col = D3DXCOLOR(Red, Green, Blue, Alpha);
}

//--------------------------------------------------
//�e�N�X�`���̐ݒ菈��
//--------------------------------------------------
void Settex(VERTEX_2D *pVtx, float ULeft, float URight, float VTop, float VBottom)
{
	pVtx[0].tex = D3DXVECTOR2(ULeft, VTop);
	pVtx[1].tex = D3DXVECTOR2(URight, VTop);
	pVtx[2].tex = D3DXVECTOR2(ULeft, VBottom);
	pVtx[3].tex = D3DXVECTOR2(URight, VBottom);
}

//--------------------------------------------------
//���_���W�̏���������
//--------------------------------------------------
void Initpos(VERTEX_2D *pVtx)
{
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
//rhw�̏���������
//--------------------------------------------------
void Initrhw(VERTEX_2D *pVtx)
{
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//--------------------------------------------------
//���_�J���[�̏���������
//--------------------------------------------------
void Initcol(VERTEX_2D *pVtx)
{
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//--------------------------------------------------
//�e�N�X�`���̏���������
//--------------------------------------------------
void Inittex(VERTEX_2D *pVtx)
{
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//--------------------------------------------------
//�S�Ă̏���������
//--------------------------------------------------
void InitAll(VERTEX_2D *pVtx)
{
	//���_���W�̏���������
	Initpos(pVtx);

	//rhw�̏���������
	Initrhw(pVtx);

	//���_�J���[�̏���������
	Initcol(pVtx);

	//�e�N�X�`���̏���������
	Inittex(pVtx);
}