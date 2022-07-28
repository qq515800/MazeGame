#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include<conio.h>


/*************************************测试代码**************************************/

/*************************************宏定义**************************************/

#define MAZE_MAX_X			10
#define MAZE_MAX_Y			10


/* 地图维度 */
enum emMazeMapDimension
{
	MAZE_MAP_DIMENSION_x = 0,
	MAZE_MAP_DIMENSION_y = 1,
	
	MAZE_MAP_DIMENSION_max,
};

/* 玩家操作 */
enum emMazePlayerFuncType
{
	MAZE_PLAYER_FUNC_up = 0,
	MAZE_PLAYER_FUNC_down = 1,
	MAZE_PLAYER_FUNC_left = 2,
	MAZE_PLAYER_FUNC_right = 3,
	
	MAZE_PLAYER_FUNC_max
};

/* 正反向 */
enum emMazePlayerFuncDir
{
	MAZE_PLAYER_FUNC_DIR_normal = 0,
	MAZE_PLAYER_FUNC_DIR_reverse = 1,
	
	MAZE_PLAYER_FUNC_DIR_max
};

/***************************************私有数据************************************/

/* 迷宫地图 */
static uint8_t MazeMap[MAZE_MAX_X][MAZE_MAX_Y] =
{
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 1, 1, 0, 1, 0, 1, 1, 0, 1},
	{0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
	{0, 1, 1, 0, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 0, 1, 1, 0},
	{0, 1, 1, 0, 1, 0, 1, 1, 1, 0},
	{0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
	{1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
	{0, 1, 1, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 1, 1, 0, 0, 1, 0},
};

/***************************************结构块************************************/
/* 私有属性 */
struct tagMazePlayerPrv
{
	uint32_t Point[MAZE_MAP_DIMENSION_max];
	uint8_t (*Func)(void *);
};

/* 玩家属性 */
struct tagMazePlayerCB
{
	struct tagMazePlayerPrv Prv;
};


/*************************************内部函数**************************************/
static uint8_t Maze_PlayerFuncUp(struct tagMazePlayerCB *cb)
{
	uint8_t rtn = 1;
	
	if (cb->Prv.Point[MAZE_MAP_DIMENSION_x] == 0)
	{
		rtn = 0;
	}
	else
	{
		cb->Prv.Point[MAZE_MAP_DIMENSION_x] -= 1;
	}
	
	return rtn;
}; 

static uint8_t Maze_PlayerFuncDown(struct tagMazePlayerCB *cb)
{
	uint8_t rtn = 1;
	
	if (cb->Prv.Point[MAZE_MAP_DIMENSION_x] == (MAZE_MAX_X - 1))
	{
		rtn = 0;
	}
	else
	{
		cb->Prv.Point[MAZE_MAP_DIMENSION_x] += 1;
	}
	
	return rtn;
}; 

static uint8_t Maze_PlayerFuncLeft(struct tagMazePlayerCB *cb)
{
	uint8_t rtn = 1;
	
	if (cb->Prv.Point[MAZE_MAP_DIMENSION_y] == 0)
	{
		rtn = 0;
	}
	else
	{
		cb->Prv.Point[MAZE_MAP_DIMENSION_y] -= 1;
	}
	
	return rtn;
}; 

static uint8_t Maze_PlayerFuncRight(struct tagMazePlayerCB *cb)
{
	uint8_t rtn = 1;
	
	if (cb->Prv.Point[MAZE_MAP_DIMENSION_y] == (MAZE_MAX_Y - 1))
	{
		rtn = 0;
	}
	else
	{
		cb->Prv.Point[MAZE_MAP_DIMENSION_y] += 1;
	}
	
	return rtn;
}; 


typedef uint8_t (*MazeFunc)(struct tagMazePlayerCB *);

MazeFunc PlayerFuncTable[MAZE_PLAYER_FUNC_DIR_max][MAZE_PLAYER_FUNC_max] =
{
	[MAZE_PLAYER_FUNC_DIR_normal] = 
	{
		[MAZE_PLAYER_FUNC_up] = Maze_PlayerFuncUp,
		[MAZE_PLAYER_FUNC_down] = Maze_PlayerFuncDown,
		[MAZE_PLAYER_FUNC_left] = Maze_PlayerFuncLeft,
		[MAZE_PLAYER_FUNC_right] = Maze_PlayerFuncRight,
	},
	[MAZE_PLAYER_FUNC_DIR_reverse] = 
	{
		[MAZE_PLAYER_FUNC_up] = Maze_PlayerFuncDown,
		[MAZE_PLAYER_FUNC_down] = Maze_PlayerFuncUp,
		[MAZE_PLAYER_FUNC_left] = Maze_PlayerFuncRight,
		[MAZE_PLAYER_FUNC_right] = Maze_PlayerFuncLeft,
	},
};

/* 修改当前地图上的值 */
static void Maze_SetMapData(uint8_t *map,
						uint32_t x,
						uint32_t y,
						uint32_t play_x,
						uint32_t play_y,
						uint8_t data)
{
	/* 更新当前地图上玩家位置信息 */
	*(map + x * play_x + play_y) = data;
}


/* 检测当前位置是否为墙 */
uint8_t Maze_IsPlayerPointWall(struct tagMazePlayerCB *cb,
								uint8_t *map,
								uint32_t x,
								uint32_t y)
{
	return *(map + x * cb->Prv.Point[MAZE_MAP_DIMENSION_x] + cb->Prv.Point[MAZE_MAP_DIMENSION_y]);
}



/*************************************外部函数**************************************/

/* 地图刷新 */
void Maze_DefreshMap(uint8_t *map,
					uint32_t x,
					uint32_t y)
{
	uint32_t i, j = 0;
	
	/* 先清屏 */
	system("cls");
	
	/* 显示地图 */
	for (i = 0; i < x; i++)
	{
		for (j = 0; j < y; j++)
		{
			printf("%d", *(map + i * x + j));
		}
		printf("\n");
	}
}

/* 迷宫初始化 */
void Maze_Init(struct tagMazePlayerCB *cb)
{
	uint8_t i;
	
	/* 初始化玩家位置 */
	for (i = 0; i < MAZE_MAP_DIMENSION_max; i++)
	{
		cb->Prv.Point[i] = 0;
	}
	
	/* 当前玩家位置上的数据置2 */
	Maze_SetMapData(&MazeMap[0][0],
					MAZE_MAX_X,
					MAZE_MAX_Y,
					cb->Prv.Point[MAZE_MAP_DIMENSION_x],
					cb->Prv.Point[MAZE_MAP_DIMENSION_y],
					2); 
	
	/* 初始化地图 */
	Maze_DefreshMap(&MazeMap[0][0],
						MAZE_MAX_X,
						MAZE_MAX_Y);
}

/* 获取操作 */
enum emMazePlayerFuncType Maze_GetFunc(void)
{
	switch(getch())
	{
		case 72:
		{
			return MAZE_PLAYER_FUNC_up;
		}//printf("UPn"); break;
		case 80:
		{
			return MAZE_PLAYER_FUNC_down;
		}//printf("DOWNn"); break;
		case 75:
		{
			return MAZE_PLAYER_FUNC_left;
		}//printf("LEFTn"); break;
		case 77:
		{
			return MAZE_PLAYER_FUNC_right;
		}//printf("RIGHTn"); break;
		default:
			break;
	}
}

/* 地图上刷新当前玩家位置 */
void Maze_DefreshPlayer(struct tagMazePlayerCB *cb,
						uint8_t *map,
						uint32_t x,
						uint32_t y,
						uint8_t func)
{
	if (func >= MAZE_PLAYER_FUNC_max)
	{
		return;
	}
	
	/* 先把原本位置清0 */
	Maze_SetMapData(map,
					x,
					y,
					cb->Prv.Point[MAZE_MAP_DIMENSION_x],
					cb->Prv.Point[MAZE_MAP_DIMENSION_y],
					0); 
	
	/* 动作移动 */
	PlayerFuncTable[MAZE_PLAYER_FUNC_DIR_normal][func](cb);
	
	/* 检测是否为墙，是则不更新位置 */
	if (Maze_IsPlayerPointWall(cb,
								map,
								x,
								y))
	{
		PlayerFuncTable[MAZE_PLAYER_FUNC_DIR_reverse][func](cb);
	}
	
	/* 把移动后位置上的数据置2 */
	Maze_SetMapData(map,
					x,
					y,
					cb->Prv.Point[MAZE_MAP_DIMENSION_x],
					cb->Prv.Point[MAZE_MAP_DIMENSION_y],
					2); 
}

/* 胜利检测 */
uint8_t Maze_VictoryCheck(struct tagMazePlayerCB *cb,
							uint32_t x,
							uint32_t y)
{
	if (  (cb->Prv.Point[MAZE_MAP_DIMENSION_x] == x)
	   && (cb->Prv.Point[MAZE_MAP_DIMENSION_y] == y)
	   )
	{
	   	return 1;
	}
	return 0;
}

/*************************************应用**************************************/
/* 新增玩家 */
struct tagMazePlayerCB Player1;


int main()
{
	uint8_t key = 0;
	/* 初始化 */
	Maze_Init(&Player1);
	
	for (;;)
	{
		/* 游戏说明 */
		printf("\n\n1、地图中1为墙，0为通道，2为玩家位置\n2、通过按“↑”、“↓”、“←”、“→”键切换位置\n3、从左上角走到左下角即为胜利\n");
		
		/* 动作操作 */
		key = Maze_GetFunc();


		/* 更新位置 */
		Maze_DefreshPlayer(&Player1,
					&MazeMap[0][0],
					MAZE_MAX_X,
					MAZE_MAX_Y,
					key);
		
					
		/* 显示刷新--最好是有变更后再刷新 */
		Maze_DefreshMap(&MazeMap[0][0],
						MAZE_MAX_X,
						MAZE_MAX_Y);
						
		
		//printf("key = %d\n", key);
		//printf("player now place = [%d, %d]", Player1.Prv.Point[MAZE_MAP_DIMENSION_x], Player1.Prv.Point[MAZE_MAP_DIMENSION_y]);
		
		/* 胜利检测 */
		if (Maze_VictoryCheck(&Player1,
								MAZE_MAX_X - 1,
								MAZE_MAX_Y - 1))
		{
			printf("\n\nCongratulations!!\n");
			system("pause");
			break;
		}
	}

	return 0;
}











