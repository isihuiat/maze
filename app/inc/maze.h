#pragma once

// Includes
#include <stdio.h>
#include <stdint.h>

// Constants
extern const uint8_t kRow;
extern const uint8_t kCol;
extern const char kWall;
extern const char kBlank;
extern const char kGoal;
extern const char kStart;

// Structures
typedef uint8_t **MapData_t;
typedef uint8_t *RowData_t;
typedef uint8_t FieldData_t;

typedef struct
{
    uint8_t b0 : 1;
    uint8_t b1 : 1;
    uint8_t b2 : 1;
    uint8_t b3 : 1;
    uint8_t b4 : 1;
    uint8_t b5 : 1;
    uint8_t b6 : 1;
    uint8_t b7 : 1;
} Bits_t;

typedef union
{
    Bits_t bits;
    uint8_t byte;
} Reg_t;

typedef struct
{
    Reg_t r0;
    Reg_t r1;
} Flag_t;

typedef struct
{
    MapData_t data;
    long row_length;
    long col_length;
} Map_t;

/// @brief 迷路用関数群
typedef struct
{
    void (*ready)(Map_t *);
    void (*start)(Map_t *, Flag_t *);
    void (*free)(Map_t *);
} Func_t;

/// @brief 迷路情報
typedef struct
{
    /// @brief ステータスフラグ
    Flag_t flags;
    /// @brief マップ情報
    Map_t map;
    /// @brief ゲーム操作用関数群
    Func_t funcs;
} Maze_t;

// Function Prototypes
extern void InitMaze(Maze_t *);