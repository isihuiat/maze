#include <stdio.h>
#include <stdlib.h>
// local
#include "define.h"
#include "memory.h"

// Constants

// Function Prototypes

void AllocCheck(const void *);
void **Malloc2d(size_t, size_t, long, long);
void **Free2d(void **, size_t);

// Functions

/// @brief メモリ確保が成功したかの確認
/// @param p このアドレスがNULLなら強制終了
void AllocCheck(const void *p)
{
    if (p == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        abort();
    }
    return;
}

/// @brief 2次元配列のメモリ確保
/// @param row_size 1次元目の配列要素のサイズ
/// @param field_size 2次元目の配列要素のサイズ
/// @param d1_length 1次元目の配列長
/// @param d2_length 2次元目の配列長
/// @return 確保したメモリ
void **Malloc2d(size_t row_size, size_t field_size, long d1_length, long d2_length)
{
    void **p = (void **)malloc(d1_length * row_size);
    AllocCheck(p);
    for (long i = 0; i < d1_length; i++)
    {
        p[i] = (void *)malloc(d2_length * field_size);
        if (p[i] == NULL)
        {
            fprintf(stderr, "Memory allocation failed for columns\n");
            // 失敗した場合、既に確保したメモリを解放
            for (long j = 0; j < i; j++)
            {
                free(p[j]);
            }
            free(p);
            abort();
        }
    }
    return p;
}

/// @brief 2次元配列のメモリ開放
/// @param p 2次元配列
/// @param d1_length 1次元目の配列長
/// @return NULL
void **Free2d(void **p, size_t d1_length)
{
    for (size_t i = 0; i < d1_length; i++)
    {
        free(p[i]);
    }
    free(p);
    return NULL;
}
