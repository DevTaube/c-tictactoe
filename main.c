
#include "stdio.h"
#include "stdlib.h"

static int board[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

void print_board() {
    printf("    A   B   C \n");
    printf("\n");
    printf("1   %s | %s | %s \n",
        board[0][0] == 0? " " : board[0][0] == 1? "X" : "O",
        board[1][0] == 0? " " : board[1][0] == 1? "X" : "O",
        board[2][0] == 0? " " : board[2][0] == 1? "X" : "O"
    );
    printf("   ---+---+---\n");
    printf("2   %s | %s | %s \n",
        board[0][1] == 0? " " : board[0][1] == 1? "X" : "O",
        board[1][1] == 0? " " : board[1][1] == 1? "X" : "O",
        board[2][1] == 0? " " : board[2][1] == 1? "X" : "O"
    );
    printf("   ---+---+---\n");
    printf("3   %s | %s | %s \n\n",
        board[0][2] == 0? " " : board[0][2] == 1? "X" : "O",
        board[1][2] == 0? " " : board[1][2] == 1? "X" : "O",
        board[2][2] == 0? " " : board[2][2] == 1? "X" : "O"
    );
}

void user_move() {
    int board_x;
    int board_y;
    int move_done = 0;
    while(!move_done) {
        char input_column;
        int input_done = 0;
        printf("Enter column (A/B/C): ");
        while(!input_done) {
            scanf("%c", &input_column);
            if(input_column == 'A' || input_column == 'B' || input_column == 'C') {
                input_done = 1;
            }
        }
        int input_row;
        printf("Enter row (1/2/3): ");
        input_done = 0;
        while(!input_done) {
            scanf("%d", &input_row);
            if(input_row == 1 || input_row == 2 || input_row == 3) {
                input_done = 1;
            }
        }
        board_x = input_column == 'A'? 0 :
                      input_column == 'B'? 1 :
                      2;
        board_y = input_row - 1;
        if(board[board_x][board_y] == 0) {
            move_done = 1;
        } else {
            printf("Square is already taken up! Enter another field:\n");
        }
    }
    board[board_x][board_y] = 1;
    printf("\n");
}

void choose_move(int* board_x, int* board_y) {
    // first fill in winning rows, then stop winning player rows
    for(int pl = 2; pl != 0; pl--) {
        // rows
        for(int y = 0; y < 3; y++) {
            for(int x = 0; x < 3; x++) {
                if(board[0][y] == (x == 0? 0 : pl) && board[1][y] == (x == 1? 0 : pl) && board[2][y] == (x == 2? 0 : pl)) { *board_x = x; *board_y = y; return; }
            }
        }
        // colums
        for(int x = 0; x < 3; x++) {
            for(int y = 0; y < 3; y++) {
                if(board[x][0] == (y == 0? 0 : pl) && board[x][1] == (y == 1? 0 : pl) && board[x][2] == (y == 2? 0 : pl)) { *board_x = x; *board_y = y; return; }
            }
        }
        // diagonals
        //     top left -> bottom right
        for(int d = 0; d < 3; d++) {
            if(board[0][0] == (d == 0? 0 : pl) && board[1][1] == (d == 1? 0 : pl) && board[2][2] == (d == 2? 0 : pl)) { *board_x = d; *board_y = d; return; }
        }
        //     bottom left -> top right
        for(int d = 0; d < 3; d++) {
            if(board[0][2] == (d == 0? 0 : pl) && board[1][1] == (d == 1? 0 : pl) && board[2][0] == (d == 2? 0 : pl)) { *board_x = d; *board_y = 2 - d; return; }
        }
    }
    // choose some spot the bot is able to pick
    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 3; x++) {
            if(board[x][y] == 0) { *board_x = x; *board_y = y; return; }
        }
    }
}

void ai_move() {
    int board_x = 3;
    int board_y = 3;
    choose_move(&board_x, &board_y);
    if(board_x < 3 && board_y < 3) {
        board[board_x][board_y] = 2;
        printf("---[ CPU chose >%c%d<. ]---\n\n", 'A' + board_x, board_y + 1);
    }
}

int is_tie() {
    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 3; y++) {
            if(board[x][y] == 0) { return 0; }
        }
    }
    return 1;
}
int has_won(int player) {
    // check every row
    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 3; x++) {
            if(board[x][y] != player) { break; }
            // return if last one in the row
            if(x == 2) { return 1; }
        }
    }
    // check every column
    for(int x = 0; x < 3; x++) {
        for(int y = 0; y < 3; y++) {
            if(board[x][y] != player) { break; }
            // return if last one in the column
            if(y == 2) { return 1; }
        }
    }
    // check diagonals
    //     top left -> bottom right
    for(int d = 0; d < 3; d++) {
        if(board[d][d] != player) { break; }
        // return if last in the diagonal
        if(d == 2) { return 1; }
    }
    //     bottom left -> top right
    for(int d = 0; d < 3; d++) {
        if(board[d][3 - d] != player) { break; }
        if(d == 2) { return 1; }
    }
    // no win found
    return 0;
}

int print_wins() {
    if(is_tie()) {
        print_board();
        printf("---[ It's a tie! ]---\n\n");
        return 1;
    }
    if(has_won(1)) {
        print_board();
        printf("---[ Player won! ]---\n\n");
        return 1;
    }
    if(has_won(2)) {
        print_board();
        printf("---[ CPU won! ]---\n\n");
        return 1;
    }
    return 0;
}

int main() {
    for(;;) {
        print_board();
        user_move();
        if(print_wins()) { break; }
        ai_move();
        if(print_wins()) { break; }
    }
    return 0;
}