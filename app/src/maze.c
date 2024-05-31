#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>
// local
#include "define.h"
#include "misc.h"
#include "maze.h"
#include "memory.h"

// Constants
const uint8_t kRow = 11;
const uint8_t kCol = 24;
const char kWall = '#';
const char kBlank = ' ';
const char kGoal = 'G';
const char kStart = 'S';
const char kClear = 'C';

// Function Prototypes
void MapAlloc(Map_t *);
void MapFree(Map_t *);
void ShowMap(Map_t *);
void CreateMap(Map_t *);
void SetObjectInMap(Map_t *, const char);
void GameStart(Map_t *, Flag_t *);
void GetPos(const Map_t *, size_t *, size_t *);
void Move(Map_t *, Flag_t *, char);
void ReadyGame(Map_t *);
void InitMaze(Maze_t *);

// Functions

/// @brief マップのメモリを確保
/// @param map マップ情報
void MapAlloc(Map_t *map)
{
    map->data = (MapData_t)Malloc2d(sizeof(RowData_t),
                                    sizeof(FieldData_t),
                                    map->row_length,
                                    map->col_length);
}

/// @brief マップのメモリを開放
/// @param map マップ情報
void MapFree(Map_t *map)
{
    Free2d((void **)map->data, map->row_length);
    return;
}

/// @brief マップの表示
/// @param map マップ情報
void ShowMap(Map_t *map)
{
    for (RowData_t *row = map->data; row != map->data + map->row_length; row++)
    {
        for (FieldData_t *col = *row; col != *row + map->col_length; col++)
        {
            printf("%c", *col);
        }
        printf("\n");
    }
    return;
}

/// @brief マップの作成
/// @param map マップ情報
void CreateMap(Map_t *map)
{
    const int min = 0, max = 99, border = (int)((30 / 100.0) * (max - min + 1));
    // 外側の壁を追加
    for (size_t i = 0; i < map->row_length; i++)
    {
        map->data[i][0] = kWall;
        map->data[i][map->col_length - 1] = kWall;
    }
    for (size_t i = 0; i < map->col_length; i++)
    {
        map->data[0][i] = kWall;
        map->data[map->row_length - 1][i] = kWall;
    }

    // 内側の壁を追加
    for (size_t i = 1; i < map->row_length - 1; i++)
    {
        for (size_t j = 1; j < map->col_length - 1; j++)
        {
            map->data[i][j] = GetRand(min, max) < border ? kWall : kBlank;
        }
    }

    // Goal設定
    SetObjectInMap(map, kGoal);

    // Start設定
    SetObjectInMap(map, kStart);

    return;
}

/// @brief マップにオブジェクトを配置
/// @param map マップ情報
/// @param obj 配置するオブジェクト
void SetObjectInMap(Map_t *map, const char obj)
{
    // 空白の数を数える
    long blank_num = 0;
    for (RowData_t *row = map->data; row != map->data + map->row_length; row++)
    {
        for (FieldData_t *col = *row; col != *row + map->col_length; col++)
        {
            blank_num = *col == kBlank ? blank_num + 1 : blank_num;
        }
    }
    // オブジェクト設置
    long goal_no = GetRand(0, blank_num - 1);
    long no = 0;
    for (RowData_t *row = map->data; row != map->data + map->row_length; row++)
    {
        for (FieldData_t *col = *row; col != *row + map->col_length; col++)
        {
            if (*col == kBlank)
            {
                no++;
                *col = no == goal_no ? obj : *col;
            }
        }
    }

    return;
}

/// @brief ゲーム開始
/// @param map マップ情報
/// @param flags ゲームステータスフラグ
void GameStart(Map_t *map, Flag_t *flags)
{
    const char DEFAULT_VAR = (char)0xFF;
    char c = DEFAULT_VAR;
    char key[] = {DEFAULT_VAR, DEFAULT_VAR};
    while ((c = getch()) != 'q')
    {
        // キーデコード
        if (c == 0x00)
        {
            key[0] = c;
            key[1] = DEFAULT_VAR;
        }
        else if (key[0] == 0x00 && (c == 0x48 || c == 0x50 || c == 0x4b || c == 0x4d))
        {
            key[1] = c;
        }
        else
        {
            key[0] = DEFAULT_VAR;
            key[1] = DEFAULT_VAR;
        }
        // アクション
        if (key[(sizeof(key) / sizeof(key[0]) - 1)] != DEFAULT_VAR)
        {
            Move(map, flags, key[(sizeof(key) / sizeof(key[0]) - 1)]);
            if (flags->r0.bits.b0)
            {
                return;
            }
        }
    }
}

/// @brief プレイヤーの移動
/// @param map マップ情報
/// @param flags ゲームステータスフラグ
/// @param key 押下されたキーボードのキー情報
void Move(Map_t *map, Flag_t *flags, char key)
{
    size_t row = 0, col = 0;
    GetPos(map, &row, &col);
    int8_t row_mov = 0, col_mov = 0;

    switch (key)
    {
    case 0x48: // 上
        row_mov = -1;
        break;
    case 0x50: // 下
        row_mov = 1;
        break;
    case 0x4d: // 右
        col_mov = 1;
        break;
    case 0x4b: // 左
        col_mov = -1;
        break;
    default:
        return;
    }

    if (row + row_mov >= map->row_length || row + row_mov < 0 || col + col_mov >= map->col_length || col + col_mov < 0)
        return;

    FieldData_t *next = &(map->data[row + row_mov][col + col_mov]);
    FieldData_t *now = &(map->data[row][col]);
    if (*next == kWall)
    {
        return;
    }
    else if (*next == kBlank)
    {
        *next = kStart;
        *now = kBlank;
        flags->r1.byte += 1;
    }
    else if (*next == kGoal)
    {
        *next = kClear;
        *now = kBlank;
        flags->r0.bits.b0 = 1;
        flags->r1.byte += 1;
    }

    // system("cls");
    ShowMap(map);
    fflush(stdout);
    return;
}

void GetPos(const Map_t *map, size_t *row, size_t *col)
{
    for (size_t i = 0; i < map->row_length; i++)
    {
        for (size_t j = 0; j < map->col_length; j++)
        {
            if (map->data[i][j] == kStart)
            {
                *row = i;
                *col = j;
                return;
            }
        }
    }
    return;
}

void ReadyGame(Map_t *map)
{
    MapAlloc(map);
    CreateMap(map);
    ShowMap(map);
}

void InitMaze(Maze_t *maze)
{
    maze->map.row_length = (long)kRow;
    maze->map.col_length = (long)kCol;
    maze->funcs.ready = ReadyGame;
    maze->funcs.start = GameStart;
    maze->funcs.free = MapFree;
}