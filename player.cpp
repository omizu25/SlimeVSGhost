//--------------------------------------------------
//
// STG���� ( player.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "input.h"
#include "player.h"

//-------------------------
//�O���[�o���ϐ�
//-------------------------
LPDIRECT3DTEXTURE9			g_pTexturePlayer = NULL;		//�v���C���[�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPlayer = NULL;		//�v���C���[�̒��_�o�b�t�@�̃|�C���^
Player						g_Player;						//�v���C���[�̏��

//-------------------------
//�v���C���[�̏���������
//-------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\������ 009.png",
		&g_pTexturePlayer);

	g_Player.pos = D3DXVECTOR3(PLAYER_WIDTH * 0.5f, SCREEN_HEIGHT, 0.0f);		//�ʒu��������
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//�ړ��ʂ�������
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//������������
	g_Player.nLife = 100;														//�̗͂̏�����
	g_Player.nStock = 2;														//�c�@�̏�����
	g_Player.bDisp = true;														//�\�����Ă����Ԃɂ���

	//�Ίp���̒������Z�o����
	g_Player.fLength = sqrtf((PLAYER_WIDTH * 0.5f) * (PLAYER_WIDTH * 0.5f) + PLAYER_HEIGHT * PLAYER_HEIGHT);

	//�Ίp���̊p�x���Z�o����
	g_Player.fAngle = atan2f((PLAYER_WIDTH * 0.5f), PLAYER_HEIGHT);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) - PLAYER_WIDTH;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) + PLAYER_WIDTH;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (g_Player.fAngle * -1.0f)) - PLAYER_WIDTH;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (g_Player.fAngle * -1.0f));
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + g_Player.fAngle) + PLAYER_WIDTH;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + g_Player.fAngle);
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 0.5f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 0.5f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-------------------------
//�v���C���[�̏I������
//-------------------------
void UninitPlayer(void)
{
	if (g_pTexturePlayer != NULL)
	{//�v���C���[�̃e�N�X�`���̔j��
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{//�v���C���[�̒��_�o�b�t�@�̔j��
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//-------------------------
//�v���C���[�̍X�V����
//-------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//�ړ�����
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//A�L�[�������ꂽ
		//�ړ��ʂ��X�V (����������)
		g_Player.move.x += sinf(-D3DX_PI * 0.5f) * 0.6f;
		g_Player.move.y += cosf(-D3DX_PI * 0.5f) * 0.6f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//D�L�[�������ꂽ
		//�ړ��ʂ��X�V (����������)
		g_Player.move.x += sinf(D3DX_PI * 0.5f) * 0.6f;
		g_Player.move.y += cosf(D3DX_PI * 0.5f) * 0.6f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
	}

	if (GetKeyboardPress(DIK_LEFT) == true)
	{//���L�[�������ꂽ
		g_Player.rot.z += D3DX_PI * 0.01f;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{//���L�[�������ꂽ
		g_Player.rot.z += -D3DX_PI * 0.01f;
	}

	//�ʒu���X�V
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;

	g_Player.move.x += GetJoypadStick(JOYKEY_L_STICK).x;

	//�ړ��ʂ��X�V (����������)
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.075f;
	g_Player.move.y += (0.0f - g_Player.move.y) * 0.075f;

	//��ʒ[����
	if (g_Player.pos.y >= SCREEN_HEIGHT)
	{//���[
		g_Player.pos.y = SCREEN_HEIGHT;
	}
	else if (g_Player.pos.y <= PLAYER_HEIGHT)
	{//��[
		g_Player.pos.y = PLAYER_HEIGHT;
	}

	if (g_Player.pos.x >= SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f))
	{//�E�[
		g_Player.pos.x = -(PLAYER_WIDTH * 0.5f);
	}
	else if (g_Player.pos.x <= -(PLAYER_WIDTH * 0.5f))
	{//���[
		g_Player.pos.x = SCREEN_WIDTH + (PLAYER_WIDTH * 0.5f);
	}

	//���_���W�̍X�V
	pVtx[0].pos.x = g_Player.pos.x + sinf((-D3DX_PI + g_Player.fAngle) + g_Player.rot.z) - (PLAYER_WIDTH * 0.5f);
	pVtx[0].pos.y = g_Player.pos.y + cosf((-D3DX_PI + g_Player.fAngle) + g_Player.rot.z) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf((D3DX_PI - g_Player.fAngle) + g_Player.rot.z) + (PLAYER_WIDTH * 0.5f);
	pVtx[1].pos.y = g_Player.pos.y + cosf((D3DX_PI - g_Player.fAngle) + g_Player.rot.z) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf((-D3DX_PI * 0.5f) + g_Player.rot.z) - (PLAYER_WIDTH * 0.5f);
	pVtx[2].pos.y = g_Player.pos.y + cosf((-D3DX_PI * 0.5f) + g_Player.rot.z);
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf((D3DX_PI * 0.5f) + g_Player.rot.z) + (PLAYER_WIDTH * 0.5f);
	pVtx[3].pos.y = g_Player.pos.y + cosf((D3DX_PI * 0.5f) + g_Player.rot.z);
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//-------------------------
//�v���C���[�̕`�揈��
//-------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Player.bDisp == true)
	{//�\������Ă���
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,							//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u(�|���S��)��
	}
}