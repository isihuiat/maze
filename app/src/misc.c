#include <stdio.h>
#include <stdlib.h>
/* local */
#include <define.h>
#include <misc.h>

/* Function Prototypes */

int GetRand(const int, const int);

/* Functions */

/// @brief 乱数生成
/// @param min 乱数の最小値
/// @param max 乱数の最大値
/// @return 乱数
int GetRand(const int min, const int max)
{
    return rand() % (max - min + 1) + min;
}
