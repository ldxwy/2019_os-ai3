#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

class Game2048 {
public:
    Game2048(int _max_score = 2048): max_score(_max_score) {
        N = 4;
        memset((void*)(matrix[0]), 0, sizeof(matrix));
    }
    ~Game2048() {}
    void LeftOperator();
    void RightOperator();
    void UpOperator();
    void DownOperator();
    void PrintMatrix(int);
    void AddRand();
    int Check();

private:
    int max_score;
    int N;
    int matrix[4][4];
    int arr[4];

    void Move();
    void Merge();
    void MoveAndMerge();
};


void Game2048::Move()
{
    int pos0;
    for(pos0 = 0; pos0 < N; ++pos0) {
        if(!arr[pos0]) break;
    }
    for(int i = pos0 + 1; i < N; ++i) {
        if(arr[i]) arr[pos0++] = arr[i];
    }
    while(pos0 < N) {
        arr[pos0++] = 0;
    }
}

void Game2048::Merge()
{
    int pos;
    for(pos = 0; pos < N && !arr[pos]; ++pos) {}
    for(int i = pos + 1; i < N; ++i) {
        if(arr[i]) {
            if(arr[i] == arr[pos]) {
                arr[pos] <<= 1;
                arr[i] = 0;
            }
            else {
                pos = i;
            }
        }
    }
}

void Game2048::MoveAndMerge()
{
    for(int i = 0; i < 3; ++i) Merge();
    Move();

#ifdef DEBUG
    for(int i = 0; i < N; ++i) printf("%-6d", arr[i]);
#endif
}

void Game2048::LeftOperator()
{
    for(int ver = 0; ver < N; ++ver) {
        memcpy((void*)arr, (void*)matrix[ver], sizeof(arr));
        MoveAndMerge();
        memcpy((void*)matrix[ver], (void*)arr, sizeof(arr));
    }
}

void Game2048::RightOperator()
{
    for(int ver = 0; ver != N; ++ver) {
        for(int hor = 0; hor < N; ++hor) arr[hor] = matrix[ver][N-1-hor];
        MoveAndMerge();
        for( hor = 0; hor < N; ++hor) matrix[ver][hor] = arr[N-1-hor];
    }
}

void Game2048::UpOperator()
{
    for(int hor = 0; hor < N; ++hor) {
        for(int i = 0; i < N; ++i) arr[i] = matrix[i][hor];
        MoveAndMerge();
        for( i = 0; i < N; ++i) matrix[i][hor] = arr[i];
    }
}

void Game2048::DownOperator()
{
    for(int hor = 0; hor < N; ++hor) {
        for(int i = 0; i < N; ++i) arr[i] = matrix[N-1-i][hor];
        MoveAndMerge();
        for( i = 0; i < N; ++i) matrix[i][hor] = arr[N-1-i];
    }
}

void Game2048::AddRand()
{
    int num = 2;
    srand(unsigned(time(NULL)));
    if((rand() % 4) == 3) num = 4;

    int zero = 0;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if(!matrix[i][j]) zero++;
        }
    }

    int pos = rand() % zero + 1;
    int count = 0;
    for( i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if(!matrix[i][j]) {
                count++;
                if(count == pos) {
                    matrix[i][j] = num;
                    break;
                }
            }
        }
    }
}

void Game2048::PrintMatrix(int step)
{
    printf("\nstep: %d\n", step);
    printf("---------------------------\n");
    for(int i = 0; i < N; ++i) {
        printf("| ");
        for(int j = 0; j < N; ++j) {
            if(matrix[i][j]) printf("%-6d", matrix[i][j]);
            else printf("      ");
        }
        printf("|\n");
    }
    printf("---------------------------\n");
}

int Game2048::Check()
{
    int count = 0;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if(matrix[i][j] == max_score) return 1;
            if(matrix[i][j]) count++;
        }
    }
    if(N * N == count) return -1;
    return 0;
}

int main()
{
    while(1) {
        Game2048 game(2048);
        game.AddRand();
        game.AddRand();
        printf("输入方向, w代表上, s代表下, a代表左, d代表右, 然后按回车键\n");
        int step = 0;
        game.PrintMatrix(step++);

        while(1) {
            printf("输入方向： ");
            char temp, c = 'e';
            while( (temp = getchar()) != 10) {
                if(('e' == c) && ('w' == temp || 'a' == temp || 's' == temp || 'd' == temp) ) {
                        c = temp;
                }
            }

            switch (c) {
                case 'a': game.LeftOperator(); break;
                case 'd': game.RightOperator(); break;
                case 'w': game.UpOperator(); break;
                case 's': game.DownOperator(); break;
                default: break;
            }

            if('e' == c) {
                printf("\n输入错误!\n");
                continue;
            }

            game.AddRand();
            game.PrintMatrix(step++);
            int res = game.Check();
            if(1 == res) { 
                printf("\nWin!\n\n\n");
                break;
            }
            if(-1 == res) {
                printf("\n失败!\n\n\n");
                break;
            }
        }
    }

    return 0;
}
