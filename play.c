//
// Created by m1396 on 2021/8/27.
//

#include "play.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define MAZEMAP_STR_INDEX(c, i, j) ((((c) + 3)) * ((i) + 1) + (j) + 1)
#define MAZEMAP_STR_INDEX_P(c, p) MAZEMAP_STR_INDEX(c, (p).p0, (p).p1)

// 高度和宽度最大值
#define MAX_V 1000

Game init_game() {
    Game game = Game_new();
    printf("请输入迷宫的行和列，中间用空格隔开：");
    fflush(stdout);
    maze_size_t row, column;
    DWORD seed;
    
    char input[0xff];
    char *input_pend;
    fgets(input, 0xff, stdin);
    row = strtol(input, &input_pend, 10);
    column = strtol(input_pend, &input_pend, 10);
    seed = strtoul(input_pend, &input_pend, 10);
    while (!((row > 1 && column > 1) && (row < MAX_V && column < MAX_V))) {
        printf("行和列必须同时大于1，且小于%d，请重新输入：", MAX_V);
        fflush(stdout);
        fgets(input, 0xff, stdin);
        row = strtol(input, &input_pend, 10);
        column = strtol(input_pend, &input_pend, 10);
        seed = strtoul(input_pend, &input_pend, 10);
    }
    printf("正在生成迷宫...\n");
    fflush(stdout);
    Game_start_seed(game, row, column, seed);
    return game;
}

const char *how_to_play = {"游戏可能会莫名其妙闪退，不要在意这些细节\n"
                           "玩法：方向键移动，会自动寻找下一个分叉点，会记录当前步数\n"
                           "起点保证在左上角，终点保证在右下角\n"
                           "'R': 人物会回到起点\n"
                           "'ctrl + R': 重新开始，人物会回到起点\n"
                           "'ctrl + S': 求解，显示当前位置到终点的路径，再按一次取消\n"
                           "'ctrl + N': 生成同长同宽的新地图\n"
                           "'ctrl + M': 新地图，重新输入长和宽\n"
                           "'ctrl + X': 结束游戏\n"};

void play() {
    printf("%s", how_to_play);

    char *disp = (char *) malloc(sizeof(char) * (MAX_V + 3) * (MAX_V + 2) + 0xffff);
    char *map_str = (char *) malloc(sizeof(char) * (MAX_V + 3) * (MAX_V + 2));
    Point *moves = (Point *) malloc(sizeof(Point) * MAX_V * MAX_V);
    char tips[0xff] = {0};
    int move_step = 0;
    int on_solve = 0;
    int ch;
    int exception = 0;
    int on_break = 0;

    Game game = init_game();

    while (!on_break) {
        if (exception) {  // 暂时应该不会出异常吧，出了应该也是闪退
            strcpy(tips, "可能出现异常，可能需要 ctrl + 'R' 重启游戏");
        }
        else if (Game_is_win(game)) {
            strcpy(tips, "恭喜你获得胜利！按 ctrl + 'N' 或 ctrl + 'M' 开始新游戏");
        }
        else {
            tips[0] = '\0';
        }
        MazeMap_to_str(game->map, map_str);
        if (game->is_move) {
            map_str[MAZEMAP_STR_INDEX_P(game->map.column, game->player.pos)] = '0';
            if (move_step) {
                for (maze_size_t i = 0; i < move_step; ++i) {
                    map_str[MAZEMAP_STR_INDEX_P(game->map.column, moves[i])] = '.';
                }
                move_step = 0;
            }
        }
        if (on_solve) {
            MazeMap_solve(&game->map, game->player.pos);
            for (maze_size_t i = 1; i < game->map.solve_step; ++i) {
                map_str[MAZEMAP_STR_INDEX_P(game->map.column, game->map.solve_list[i])] = 'V';
            }
        }
        sprintf(disp, "%s种子：%lu 步数：%d 执行次数：%d\n%s\n%s", how_to_play,
                game->map._random->seed, game->player.step, game->player.move_times,
                tips, map_str);
        system("cls");
        printf("%s\n", disp);
        fflush(stdout);

        ch = getch();
        switch (ch) {
            case 'R':
            case 'r':
                game->player.pos = game->map.st;
                break;
            case 14:
                // ctrl + 'N'
                Game_start(game, game->map.row, game->map.column);
                break;
            case 13:
                // ctrl + 'M'
                Game_free(game);
                game = init_game();
                break;
            case 18:
                // ctrl + 'R'
            {
                DWORD seed = game->map._random->seed;
                maze_size_t row = game->map.row;
                maze_size_t column = game->map.column;
                Game_free(game);
                game = Game_new();
                Game_start_seed(game, row, column, seed);
            }
                on_solve = 0;
                exception = 0;
                break;
            case 19:
                // ctrl + 'S'
                on_solve = !on_solve;
                break;
            case 24:
                // ctrl + 'X'
                on_break = 1;
                break;
            case 224:
                // 方向键
                ch = getch();
                if (Game_is_win(game)) continue;
                switch (ch) {
                    case 72:
                        // 上
                        move_step = Game_move(game, move_up, moves);
                        break;
                    case 80:
                        // 下
                        move_step = Game_move(game, move_down, moves);
                        break;
                    case 75:
                        // 左
                        move_step = Game_move(game, move_left, moves);
                        break;
                    case 77:
                        // 右
                        move_step = Game_move(game, move_right, moves);
                        break;
                    default:
                        // ctrl + 小键盘 也是224
//                        exception = 1;
                        break;
                }
                break;
            case 0:
                // 小键盘
                getch();
                break;
            default:
                break;
        }
    }
    free(disp);
    free(map_str);
    free(moves);
}
