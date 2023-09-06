#include <assert.h>
#include <stdio.h>
#include "connect.h"

const char NONE = '_';
const char RED = 'R';
const char YELLOW = 'Y';

// The number of pieces that have to be in a straight line (horizontal of
//   vertical) in order to win the game.
static const int WINLENGTH = 4;

// The width / number of columns of the Connect Four board.
static const int WIDTH = 7;

// The height / number of rows of the Connect Four board.
static const int HEIGHT = 6;

// get_char(board, c, r) returns the player who's piece is in column c and
//   row r.
// requires: c must be between 0 (inclusive) and WIDTH (exclusive)
//           r must be between 0 (inclusive) and HIGHT (exclusive)
static char get_char(char *board, int c, int r) {
    assert(c >= 0);
    assert(c < WIDTH);
    assert(r >= 0);
    assert(r < HEIGHT);
    return board[r * WIDTH + c];
}

// See connect.h for documentation.
struct connect connect_init() {
    struct connect my_game;
    
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        my_game.board[i] = NONE;
    }

    my_game.current_player = RED;
    return my_game;
}

// See connect.h for documentation.
bool connect_drop_piece(struct connect *cf, int col) {
    assert(col >= 0);
    assert(col < WIDTH);
    int row = HEIGHT - 1;
    while (row >= 0 && cf->board[row * WIDTH + col] != NONE) {
        row--;
    }
    if (row >= 0) {
        cf->board[row * WIDTH + col] = cf->current_player;
        if (cf->current_player == RED) {
            cf->current_player = YELLOW;
        } else {
            cf->current_player = RED;
        }
        return true;
    } else {
        return false;
    }
}

// See connect.h for documentation.
char connect_has_won(const struct connect *cf) {
    const char *board = cf->board;

    // Scanning for Horizontal Wins
    for (int row = 0; row < HEIGHT; ++row) {
        for (int col = 0; col <= WIDTH - WINLENGTH; ++col) {
            char player = board[row * WIDTH + col];
            if (player != NONE) {
                int count = 1;
                for (int i = 1; i < WINLENGTH; ++i) {
                    if (board[row * WIDTH + col + i] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if(count == WINLENGTH) {
                    return player;
                }
            }
        }
    }         

    // Scanning for Vertical Wins
    for(int row = 0; row <= HEIGHT - WINLENGTH; ++row) {
        for (int col = WINLENGTH - 1; col < WIDTH; ++col) {
            char player = board[row * WIDTH + col];
            if (player != NONE) {
                int count = 1;
                for (int i = 1; i < WINLENGTH; ++i) {
                    if (board[(row + i) * WIDTH + col - i] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == WINLENGTH) {
                    return player;
                }
            }
        }
    }

    // Scanning for diagonal wins: left to right
    for (int row = 0; row <= HEIGHT - WINLENGTH; ++row) {
        for (int col = 0; col <= WIDTH - WINLENGTH; ++col) {
            char player = board[row * WIDTH + col];
            if (player != NONE) {
                int count = 1;
                for (int i = 1; i < WINLENGTH; ++i) {
                    if (board[(row + i) * WIDTH + col + i] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == WINLENGTH) {
                    return player;
                }
            }
        }
    }
    
    // Scanning for diagonal wins: right to left
    for (int row = 0; row <= HEIGHT - WINLENGTH; ++row) {
        for (int col = WINLENGTH - 1; col < WIDTH; ++col) {
            char player = board[row * WIDTH + col];
            if (player != NONE) {
                int count = 1;
                for (int i = 1; i < WINLENGTH; ++i) {
                    if (board[(row + i) * WIDTH + col - i] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == WINLENGTH) {
                    return player;
                }
            }
        }
    }
    return NONE;
}

// See connect.h for documentation.
bool connect_has_finished(const struct connect *cf) {
    const char *board = cf->board;

    int empty_slots = 0;
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        if (board[i] == NONE) {
            empty_slots++;
        }
    }
    if (empty_slots == 0) {
        return true;
    }

    char winner = connect_has_won(cf);
    if (winner != NONE) {
        return true;
    }

    return false;
}

// See connect.h for documentation.
void connect_print(struct connect *cf) {
    assert(cf);
    for (int r = 0; r < HEIGHT; ++r) {
        printf("|");
        for (int c = 0; c < WIDTH; ++c) {
            printf(" %c |", get_char(cf->board, c, r));
        }
        printf("\n");
    }
    printf("%c", cf->current_player);
    for (int w = 0; w < 4 * WIDTH - 1; ++w) {
        printf("-");
    }
    printf("%c\n\n", cf->current_player);
}