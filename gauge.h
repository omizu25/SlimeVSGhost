//--------------------------------------------------
//
// ACG制作 ( gauge.h )
// Author  : katsuki mizuki
//
//--------------------------------------------------
#ifndef _GAUGE_H_		//このマクロ定義がされてなかったら
#define _GAUGE_H_		//２重インクルード防止のマクロ定義

#include "main.h"

//--------------------------------------------------
//ゲージの使用者を定義
//--------------------------------------------------
typedef enum
{
	GAUGEUSE_PLAYER = 0,		//プレイヤー
	GAUGEUSE_BOY,				//男の子 (リリ)
	GAUGEUSE_GIRL,				//女の子 (ルル)
	GAUGEUSE_MAX
}GAUGEUSE;

//--------------------------------------------------
//ゲージの種類を定義
//--------------------------------------------------
typedef enum
{
	GAUGETYPE_MAXIMUM = 0,		//最大値
	GAUGETYPE_DECREASE,			//減少量
	GAUGETYPE_REMAINING,		//残り
	GAUGETYPE_MAX
}GAUGETYPE;

//--------------------------------------------------
//ゲージの構造体を定義
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3		pos;			//位置
	GAUGEUSE		use;			//使用者
	GAUGETYPE		type;			//種類
	float			fWidth;			//幅
	float			fHeight;		//高さ
	int				nCounter;		//カウンター
	bool			bUse;			//使用しているかどうか
}Gauge;

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);
void SetGauge(GAUGEUSE use, int nLife);
void SubGauge(GAUGEUSE use, int nLife);

#endif // !_GAUGE_H_