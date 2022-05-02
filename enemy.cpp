//--------------------------------------------------
//
// ACG���� ( enemy.cpp )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#include "block.h"
#include "enemy.h"
#include "game.h"
#include "gauge.h"
#include "item.h"
#include "player.h"
#include "ranking.h"
#include "result.h"
#include "setup.h"
#include "time.h"

#include <assert.h>
#include <time.h>

//--------------------------------------------------
//�}�N����`
//--------------------------------------------------
#define MAX_U_PATTERN		(3)			//U�p�^�[���̍ő吔
#define MAX_V_PATTERN		(2)			//V�p�^�[���̍ő吔
#define TEX_INTERVAL		(7)			//�J�E���^�[�̃C���^�[�o��
#define STOP_TIME			(60)		//�~�܂鎞��

//--------------------------------------------------
//�v���g�^�C�v�錾
//--------------------------------------------------
static void InitStruct(Enemy *pEnemy);
static void UpdateStop(Enemy *pEnemy);
static void UpdateCollision(Enemy *pEnemy);
static void UpdateOffScreen(Enemy *pEnemy);
static void UpdatePop(Enemy *pEnemy);
static void UpdateState(VERTEX_2D *pVtx, Enemy *pEnemy);
static void UpdateTexAnim(VERTEX_2D *pVtx, Enemy *pEnemy);
static void PopItem(Enemy *pEnemy);

//--------------------------------------------------
//�X�^�e�B�b�N�ϐ�
//--------------------------------------------------
static LPDIRECT3DTEXTURE9			s_pTexture[ENEMYTYPE_MAX];		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9		s_pVtxBuff = NULL;				//���_�o�b�t�@�̃|�C���^
static Enemy						s_aEnemy[ENEMYTYPE_MAX];		//�G�̏��

//--------------------------------------------------
//�G�̏���������
//--------------------------------------------------
void InitEnemy(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{//�������̃N���A
		memset(&s_pTexture[i], NULL, sizeof(LPDIRECT3DTEXTURE9));
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\obake003.png",
		&s_pTexture[ENEMYTYPE_BOY]);

	D3DXCreateTextureFromFile(
		pDevice,
		"Data\\TEXTURE\\obake004.png",
		&s_pTexture[ENEMYTYPE_GIRL]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ENEMYTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�G�̏��̏������ݒ�
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];

		//�\���̂̏���������
		InitStruct(pEnemy);

		//�S�Ă̏���������
		InitAll(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//���E�̎�q�̏�����
	srand((unsigned)time(NULL));
}

//--------------------------------------------------
//�G�̏I������
//--------------------------------------------------
void UninitEnemy(void)
{
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		if (s_pTexture[i] != NULL)
		{//�e�N�X�`���̔j��
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	if (s_pVtxBuff != NULL)
	{//���_�o�b�t�@�̔j��
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
//�G�̍X�V����
//--------------------------------------------------
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];

		if (!pEnemy->bUse)
		{//�G���g�p����Ă��Ȃ�
			continue;
		}

		//�G���g�p����Ă���

		//�O��̈ʒu�̋L��
		pEnemy->posOld = pEnemy->pos;

		//�ʒu�̍X�V
		pEnemy->pos += pEnemy->move;

		//�X�g�b�v����
		UpdateStop(pEnemy);

		//�A�C�e���̓����蔻�菈��
		CollisionItem(&pEnemy->pos, &pEnemy->posOld, pEnemy->fWidth, pEnemy->fHeight);

		//��ʊO����
		UpdateOffScreen(pEnemy);

		//�����蔻�菈��
		UpdateCollision(pEnemy);

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//��ԏ���
		UpdateState(pVtx, pEnemy);

		//���_���W�̐ݒ菈��
		SetBottompos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);

		//�e�N�X�`���A�j���[�V��������
		UpdateTexAnim(pVtx, pEnemy);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
//�G�̕`�揈��
//--------------------------------------------------
void DrawEnemy(void)
{
	//�f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		if (s_aEnemy[i].bUse)
		{//�G���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture[s_aEnemy[i].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				i * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);							//�v���~�e�B�u(�|���S��)��
		}
	}
}

//--------------------------------------------------
//�G�̐ݒ菈��
//--------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	for (int i = 0; i < ENEMYTYPE_MAX; i++)
	{
		Enemy *pEnemy = &s_aEnemy[i];

		if (pEnemy->bUse)
		{//�G���g�p����Ă���
			continue;
		}

		//�G���g�p����Ă��Ȃ�

		pEnemy->pos = pos;
		pEnemy->type = type;
		pEnemy->posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->fWidth = ENEMY_WIDTH * 0.5f;
		pEnemy->fHeight = ENEMY_HEIGHT;
		pEnemy->nCounterState = 0;
		pEnemy->nCounterAnim = 0;
		pEnemy->nPatternAnim = 0;
		pEnemy->nLife = 100;
		pEnemy->bUse = true;		//�g�p���Ă����Ԃɂ���

		switch (type)
		{
		case ENEMYTYPE_BOY:			//�j�̎q

			pEnemy->move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
			pEnemy->bDirection = true;		//�E����
			pEnemy->pop = ENEMYPOP_TOP;

			//�Q�[�W�̐ݒ菈��
			SetGauge(GAUGEUSE_BOY, pEnemy->nLife);

			break;

		case ENEMYTYPE_GIRL:		//���̎q

			pEnemy->move = D3DXVECTOR3(-3.0f, 0.0f, 0.0f);
			pEnemy->bDirection = false;		//������
			pEnemy->pop = ENEMYPOP_BOTTOM;

			//�Q�[�W�̐ݒ菈��
			SetGauge(GAUGEUSE_GIRL, pEnemy->nLife);

			break;

		default:
			assert(false);
			break;
		}

		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_�������b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 4);		//�Y���̈ʒu�܂Ői�߂�

		//���_���W�̐ݒ菈��
		SetBottompos(pVtx, pos, pEnemy->fWidth, pEnemy->fHeight);

		float fPattren = (float)pEnemy->nPatternAnim / MAX_U_PATTERN;
		float fDirection = (float)pEnemy->bDirection / MAX_V_PATTERN;

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, fPattren + 0.01f, (fPattren + (1.0f / MAX_U_PATTERN)) - 0.01f, fDirection + 0.01f, fDirection + (1.0f / MAX_V_PATTERN));

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		//�A�C�e���̃|�b�v����
		PopItem(pEnemy);

		break;		//������for���𔲂���
	}
}

//--------------------------------------------------
//�G�̎擾����
//--------------------------------------------------
Enemy *GetEnemy(void)
{
	return &s_aEnemy[0];		//�G�̏���������
}

//--------------------------------------------------
//�G�̃q�b�g����
//--------------------------------------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	Enemy *pEnemy = &s_aEnemy[nCntEnemy];

	if (pEnemy->state == ENEMYSTATE_NORMAL)
	{
		pEnemy->nLife -= nDamage;

		switch (pEnemy->type)
		{
		case ENEMYTYPE_BOY:			//�j�̎q

			//�Q�[�W�̌��Z����
			SubGauge(GAUGEUSE_BOY, pEnemy->nLife);

			break;

		case ENEMYTYPE_GIRL:		//���̎q

			//�Q�[�W�̌��Z����
			SubGauge(GAUGEUSE_GIRL, pEnemy->nLife);

			break;

		default:
			assert(false);
			break;
		}

		if (pEnemy->nLife <= 0)
		{//�G�̗̑͂��Ȃ��Ȃ���
			pEnemy->bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���

			int nUse = 0;

			for (int i = 0; i < ENEMYTYPE_MAX; i++)
			{
				if (s_aEnemy[i].bUse)
				{//�G���g�p����Ă���
					continue;
				}

				//�G���g�p����Ă��Ȃ�
				nUse++;
			}

			if (nUse >= ENEMYTYPE_MAX)
			{//�S���g�p����Ă��Ȃ�
				//���U���g�̐ݒ菈��
				SetResult(RESULT_WIN);

				//�Q�[���̐ݒ菈��
				SetGameState(GAMESTATE_END);

				//�����L���O�̐ݒ菈��
				SetRanking(GetTime());
			}
		}
		else
		{//�܂������Ă�
			pEnemy->state = ENEMYSTATE_DAMAGE;

			pEnemy->nCounterState = 0;

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

			//���_�������b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntEnemy * 4);		//�Y���̈ʒu�܂Ői�߂�

			//���_�J���[�̐ݒ菈��
			Setcol(pVtx, 1.0f, 0.0f, 0.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N����
			s_pVtxBuff->Unlock();
		}
	}
}

//--------------------------------------------------
//�\���̂̏���������
//--------------------------------------------------
static void InitStruct(Enemy *pEnemy)
{
	pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->type = ENEMYTYPE_BOY;
	pEnemy->state = ENEMYSTATE_NORMAL;
	pEnemy->pop = ENEMYPOP_TOP;
	pEnemy->fWidth = 0.0f;
	pEnemy->fHeight = 0.0f;
	pEnemy->nCounterState = 0;
	pEnemy->nCounterStop = 0;
	pEnemy->nCounterAnim = 0;
	pEnemy->nPatternAnim = 0;
	pEnemy->bDirection = false;
	pEnemy->nLife = 0;
	pEnemy->bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
}

//--------------------------------------------------
//�X�g�b�v����
//--------------------------------------------------
static void UpdateStop(Enemy *pEnemy)
{
	if (pEnemy->nCounterStop <= STOP_TIME)
	{//�܂��~�܂��ĂȂ�
		float fEnemyWidth = ENEMY_WIDTH * 0.5f;
		if (pEnemy->bDirection)
		{//�E����
			if (pEnemy->pos.x >= fEnemyWidth)
			{
				pEnemy->pos.x = fEnemyWidth;
				pEnemy->nCounterStop++;
			}
		}
		else
		{//������
			if (pEnemy->pos.x <= SCREEN_WIDTH - fEnemyWidth)
			{
				pEnemy->pos.x = SCREEN_WIDTH - fEnemyWidth;
				pEnemy->nCounterStop++;
			}
		}
	}
}

//--------------------------------------------------
//��ʊO����
//--------------------------------------------------
static void UpdateOffScreen(Enemy *pEnemy)
{
	//��ʒ[����
	if (pEnemy->pos.y >= SCREEN_HEIGHT)
	{//���[
		pEnemy->pos.y = SCREEN_HEIGHT;
		pEnemy->move.y = 0.0f;
	}
	else if (pEnemy->pos.y <= ENEMY_HEIGHT)
	{//��[
		pEnemy->pos.y = ENEMY_HEIGHT;
		pEnemy->move.y = 0.0f;
	}

	if ((pEnemy->pos.x >= SCREEN_WIDTH + ENEMY_WIDTH) || (pEnemy->pos.x <= -ENEMY_WIDTH))
	{//�E�[�����[
		pEnemy->bDirection = !pEnemy->bDirection;
		pEnemy->move *= -1.0f;
		pEnemy->nCounterStop = 0;
		
		//�|�b�v�ꏊ�̏���
		UpdatePop(pEnemy);
	}
}

//--------------------------------------------------
//�|�b�v�ꏊ�̏���
//--------------------------------------------------
static void UpdatePop(Enemy *pEnemy)
{
	do
	{//�|�b�v�ꏊ�������_��

		pEnemy->pop = (ENEMYPOP)(rand() % (int)ENEMYPOP_MAX);

	} while (s_aEnemy[0].pop == s_aEnemy[1].pop);

	float fBlock = SCREEN_HEIGHT / MAX_Y_BLOCK;

	switch (pEnemy->pop)
	{
	case ENEMYPOP_TOP:			//��̒i
		pEnemy->pos.y = fBlock * TOP_BLOCK;
		break;

	case ENEMYPOP_MIDDLE:		//�^�񒆂̒i
		pEnemy->pos.y = fBlock * MIDDLE_BLOCK;
		break;

	case ENEMYPOP_BOTTOM:		//���̒i
		pEnemy->pos.y = fBlock * BOTTOM_BLOCK;
		break;

	default:
		assert(false);
		break;
	}

	//�A�C�e���̃|�b�v����
	PopItem(pEnemy);
}

//--------------------------------------------------
//�����蔻�菈��
//--------------------------------------------------
static void UpdateCollision(Enemy *pEnemy)
{
	Player *pPlayer = GetPlayer();		//�v���C���[�̎擾

	if (pEnemy->pos.y <= (pPlayer->pos.y + pEnemy->fHeight) &&
		pEnemy->pos.y >= (pPlayer->pos.y - pPlayer->fHeight) &&
		pEnemy->pos.x <= (pPlayer->pos.x + pPlayer->fWidth + pEnemy->fWidth) &&
		pEnemy->pos.x >= (pPlayer->pos.x - pPlayer->fWidth - pEnemy->fWidth))
	{//�v���C���[�ɓG������������

		//�v���C���[�̃q�b�g����
		HitPlayer(20);
	}
}

//--------------------------------------------------
//��ԏ���
//--------------------------------------------------
static void UpdateState(VERTEX_2D *pVtx, Enemy *pEnemy)
{
	switch (pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:		//�ʏ���

		break;

	case ENEMYSTATE_DAMAGE:		//�_���[�W���

		pEnemy->nCounterState++;

		if (pEnemy->nCounterState >= 15)
		{
			//���_�J���[�̏���������
			Initcol(pVtx);

			pEnemy->state = ENEMYSTATE_NORMAL;

			pEnemy->nCounterState = 0;
		}

		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
//�e�N�X�`���A�j���[�V��������
//--------------------------------------------------
static void UpdateTexAnim(VERTEX_2D *pVtx, Enemy *pEnemy)
{
	pEnemy->nCounterAnim++;		//�J�E���^�[�����Z

	if ((pEnemy->nCounterAnim % TEX_INTERVAL) == 0)
	{//��莞�Ԍo�߂���
		//�p�^�[��No.���X�V����
		pEnemy->nPatternAnim = (pEnemy->nPatternAnim + 1) % MAX_U_PATTERN;

		float fPattren = (float)pEnemy->nPatternAnim / MAX_U_PATTERN;
		float fDirection = (float)pEnemy->bDirection / MAX_V_PATTERN;

		//�e�N�X�`�����W�̐ݒ菈��
		Settex(pVtx, fPattren + 0.01f, (fPattren + (1.0f / MAX_U_PATTERN)) - 0.01f, fDirection + 0.01f, fDirection + (1.0f / MAX_V_PATTERN));
	}
}

//--------------------------------------------------
//�A�C�e���̃|�b�v����
//--------------------------------------------------
static void PopItem(Enemy *pEnemy)
{
	float fItemSize = ITEM_SIZE * 0.5f;

	//�A�C�e���̐ݒ菈��
	if (pEnemy->bDirection)
	{//�E����
		SetItem(pEnemy->pos + D3DXVECTOR3(pEnemy->fWidth + fItemSize, -fItemSize, 0.0f), ITEMTYPE_BLOCK, pEnemy->bDirection);
	}
	else
	{//������
		SetItem(pEnemy->pos + D3DXVECTOR3(-pEnemy->fWidth - fItemSize, -fItemSize, 0.0f), ITEMTYPE_BLOCK, pEnemy->bDirection);
	}
}