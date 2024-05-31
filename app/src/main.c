#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// local
#include "define.h"
#include "maze.h"

/// @brief main関数
/// @param argc
/// @param argv
/// @return 終了ステータス
int main(int argc, char const *argv[])
{
    // 乱数シード設定
    srand(time(NULL));

    // 迷路初期化
    Maze_t maze = {0};
    InitMaze(&maze);
    maze.funcs.ready(&maze.map);
    maze.funcs.start(&maze.map, &maze.flags);
    maze.funcs.free(&maze.map);

    if (maze.flags.r0.bits.b0)
    {
        printf("Clear!! your score : %d\n", maze.flags.r1.byte);
    }
    else
    {
        printf("Game Over!! your score : %d\n", maze.flags.r1.byte);
    }

    return 0;
}
