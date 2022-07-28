#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include<conio.h>


/*************************************���Դ���**************************************/

/*************************************�궨��**************************************/

#define MAZE_MAX_X			10
#define MAZE_MAX_Y			10


/* ��ͼά�� */
enum emMazeMapDimension
{
	MAZE_MAP_DIMENSION_x = 0,
	MAZE_MAP_DIMENSION_y = 1,
	
	MAZE_MAP_DIMENSION_max,
};

/* ��Ҳ��� */
enum emMazePlayerFuncType
{
	MAZE_PLAYER_FUNC_up = 0,
	MAZE_PLAYER_FUNC_down = 1,
	MAZE_PLAYER_FUNC_left = 2,
	MAZE_PLAYER_FUNC_right = 3,
	
	MAZE_PLAYER_FUNC_max
};

/* ������ */
enum emMazePlayerFuncDir
{
	MAZE_PLAYER_FUNC_DIR_normal = 0,
	MAZE_PLAYER_FUNC_DIR_reverse = 1,
	
	MAZE_PLAYER_FUNC_DIR_max
};

/***************************************˽������************************************/

/* �Թ���ͼ */
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

/***************************************�ṹ��************************************/
/* ˽������ */
struct tagMazePlayerPrv
{
	uint32_t Point[MAZE_MAP_DIMENSION_max];
	uint8_t (*Func)(void *);
};

/* ������� */
struct tagMazePlayerCB
{
	struct tagMazePlayerPrv Prv;
};


/*************************************�ڲ�����**************************************/
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

/* �޸ĵ�ǰ��ͼ�ϵ�ֵ */
static void Maze_SetMapData(uint8_t *map,
						uint32_t x,
						uint32_t y,
						uint32_t play_x,
						uint32_t play_y,
						uint8_t data)
{
	/* ���µ�ǰ��ͼ�����λ����Ϣ */
	*(map + x * play_x + play_y) = data;
}


/* ��⵱ǰλ���Ƿ�Ϊǽ */
uint8_t Maze_IsPlayerPointWall(struct tagMazePlayerCB *cb,
								uint8_t *map,
								uint32_t x,
								uint32_t y)
{
	return *(map + x * cb->Prv.Point[MAZE_MAP_DIMENSION_x] + cb->Prv.Point[MAZE_MAP_DIMENSION_y]);
}



/*************************************�ⲿ����**************************************/

/* ��ͼˢ�� */
void Maze_DefreshMap(uint8_t *map,
					uint32_t x,
					uint32_t y)
{
	uint32_t i, j = 0;
	
	/* ������ */
	system("cls");
	
	/* ��ʾ��ͼ */
	for (i = 0; i < x; i++)
	{
		for (j = 0; j < y; j++)
		{
			printf("%d", *(map + i * x + j));
		}
		printf("\n");
	}
}

/* �Թ���ʼ�� */
void Maze_Init(struct tagMazePlayerCB *cb)
{
	uint8_t i;
	
	/* ��ʼ�����λ�� */
	for (i = 0; i < MAZE_MAP_DIMENSION_max; i++)
	{
		cb->Prv.Point[i] = 0;
	}
	
	/* ��ǰ���λ���ϵ�������2 */
	Maze_SetMapData(&MazeMap[0][0],
					MAZE_MAX_X,
					MAZE_MAX_Y,
					cb->Prv.Point[MAZE_MAP_DIMENSION_x],
					cb->Prv.Point[MAZE_MAP_DIMENSION_y],
					2); 
	
	/* ��ʼ����ͼ */
	Maze_DefreshMap(&MazeMap[0][0],
						MAZE_MAX_X,
						MAZE_MAX_Y);
}

/* ��ȡ���� */
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

/* ��ͼ��ˢ�µ�ǰ���λ�� */
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
	
	/* �Ȱ�ԭ��λ����0 */
	Maze_SetMapData(map,
					x,
					y,
					cb->Prv.Point[MAZE_MAP_DIMENSION_x],
					cb->Prv.Point[MAZE_MAP_DIMENSION_y],
					0); 
	
	/* �����ƶ� */
	PlayerFuncTable[MAZE_PLAYER_FUNC_DIR_normal][func](cb);
	
	/* ����Ƿ�Ϊǽ�����򲻸���λ�� */
	if (Maze_IsPlayerPointWall(cb,
								map,
								x,
								y))
	{
		PlayerFuncTable[MAZE_PLAYER_FUNC_DIR_reverse][func](cb);
	}
	
	/* ���ƶ���λ���ϵ�������2 */
	Maze_SetMapData(map,
					x,
					y,
					cb->Prv.Point[MAZE_MAP_DIMENSION_x],
					cb->Prv.Point[MAZE_MAP_DIMENSION_y],
					2); 
}

/* ʤ����� */
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

/*************************************Ӧ��**************************************/
/* ������� */
struct tagMazePlayerCB Player1;


int main()
{
	uint8_t key = 0;
	/* ��ʼ�� */
	Maze_Init(&Player1);
	
	for (;;)
	{
		/* ��Ϸ˵�� */
		printf("\n\n1����ͼ��1Ϊǽ��0Ϊͨ����2Ϊ���λ��\n2��ͨ�������������������������������������л�λ��\n3�������Ͻ��ߵ����½Ǽ�Ϊʤ��\n");
		
		/* �������� */
		key = Maze_GetFunc();


		/* ����λ�� */
		Maze_DefreshPlayer(&Player1,
					&MazeMap[0][0],
					MAZE_MAX_X,
					MAZE_MAX_Y,
					key);
		
					
		/* ��ʾˢ��--������б������ˢ�� */
		Maze_DefreshMap(&MazeMap[0][0],
						MAZE_MAX_X,
						MAZE_MAX_Y);
						
		
		//printf("key = %d\n", key);
		//printf("player now place = [%d, %d]", Player1.Prv.Point[MAZE_MAP_DIMENSION_x], Player1.Prv.Point[MAZE_MAP_DIMENSION_y]);
		
		/* ʤ����� */
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











