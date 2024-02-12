#include <stdio.h>
#include <stdbool.h>

#define BOARD_MAX_SIDE 9
#define BOARD_MIN_SIDE 3
#define MIN_TOKENS 3

#define RED_SLOT_SYMBOL ('R')
#define YELLOW_SLOT_SYMBOL ('Y')
#define EMPTY_SLOT_SYMBOL (' ')


//----------------------- Message Identifiers --------------------------//
#define MSG_GET_BOARD_ROWS   0
#define MSG_GET_BOARD_COLS   1
#define MSG_GET_NUMBER_TOKENS 2

//----------------------- Situation Identifier -------------------------//
#define NO_ONE 3
#define TIE 0

#define REDO -2
#define UNDO -1
//--------------------------- Board Edges ------------------------------//

#define BOARD_VERT_SEP  '|'
#define BOARD_LEFT_ANG '\\'
#define BOARD_RIGHT_ANG '/'
#define BOARD_BOTTOM  '-'
#define BOARD_BOTTOM_SEP '-'

//----------------------------------------------------------------------//


void print_welcome_message();
void print_read_game_params_message(int param);
void print_chose_color_message();
void print_chose_move_message(int player);
void print_enter_column_message();
void print_full_column_message();
void print_unavailable_undo_redo_message();

int scan_symbol_from_user(char *symbol);
int perform_game(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], int number_tokens);
void apply_regular_move(char board[][BOARD_MAX_SIDE], char players_symbols[], bool is_second_player_move,int row, int column, bool *ptr_undo_or_redo,
                        int columns_elements_number[]);
void print_board(char board[][BOARD_MAX_SIDE], int board_side[]);
void print_winner(int player_id);
bool is_in_range(int element, int min, int max);
void reset_board(char board[][BOARD_MAX_SIDE]);
int scan_number_from_user(int *number, int board_side[], int min_range, int max_range, int func_number);
int scan_firsts_params(char *ptr_first_player_symbol, int board_side[], int *ptr_tokens);
int scan_move(int *ptr_action, int columns_elements_number[], int board_side[], bool is_last_undo_or_redo,
               int current_action, int actions[]);
int scan_move_again(int *ptr_action, bool *ptr_is_valid_column, bool *ptr_is_column_full, int board_side[], int columns_elements_number[],
                     int *res, bool *is_valid_action, int current_action, bool is_last_undo_or_redo, int actions[]);
bool is_action_valid(int *ptr_action, int current_action, bool is_last_undo_or_redo, int actions[]);
void apply_move(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], bool *ptr_is_second_player_move, int action,
                int columns_elements_number[], bool *ptr_undo_or_redo, int actions[], int *ptr_current_action);
void apply_changing_move(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], bool is_second_player_move, int action,
                         int columns_elements_number[], bool *ptr_undo_or_redo, int actions[], int *ptr_current_action);
int scan_move_and_apply_it(char board[][BOARD_MAX_SIDE], int board_side[], bool *ptr_is_second_player_move, int *ptr_action,
                            int columns_elements_number[], bool is_last_undo_or_redo, char players_symbols[],
                            bool *ptr_undo_or_redo, int actions[], int *ptr_current_action);
int check_winning(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], int number_tokens, bool is_second_player_move, int action,
                  int columns_elements_number[]);
void update_actions(bool ptr_undo_or_redo, int *ptr_current_action, int actions[]);
void print_request_to_scan(bool is_valid_column,bool is_column_full, bool *ptr_is_valid_action, int *ptr_action);
bool check_sequence_in_column(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                              int columns_elements_number[]);
bool check_sequence_in_row(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                           int columns_elements_number[]);
bool check_sequence_in_diagonal_up(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                                   int columns_elements_number[]);
bool check_sequence_in_diagonal_down(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                                     int columns_elements_number[]);
bool is_board_full(char board[][BOARD_MAX_SIDE], int board_side[]);
int max_moves(int board_side[], int x_start_point, int y_start_point, bool is_diagonal_up);
void print_board_and_winner(char board[][BOARD_MAX_SIDE], int board_side[], int winner);

//--------------------------- Main Program -----------------------------//

/*
 * main function to start the game
 * # Lentgh: 9
 */
int main()
{
    int number_tokens, *ptr_tokens = &number_tokens, board_side[2];

    char players_symbols[2] = {EMPTY_SLOT_SYMBOL}, board[BOARD_MAX_SIDE][BOARD_MAX_SIDE] = {{EMPTY_SLOT_SYMBOL}};
    reset_board(board);

    print_welcome_message();

    if(scan_firsts_params(&players_symbols[0], board_side, ptr_tokens) == 1)
        return 1;
    players_symbols[1] = players_symbols[0] == RED_SLOT_SYMBOL ? YELLOW_SLOT_SYMBOL : RED_SLOT_SYMBOL;

    if(perform_game(board, board_side, players_symbols, number_tokens) == 1) return 1;
    return 0;
}

//---------------------- Printing Functions ----------------------------//

// message number 0 = "Welcome to 4-in-a-row game! \n"
void print_welcome_message()
{
    printf("Welcome to 4-in-a-row game! \n");
}


// message number 1 = "Please enter number of rows:"
// message number 2 = "Please enter number of columns:"
// message number 3 = "Please enter number of tokens:"
void print_read_game_params_message(int param)
{
    char const* const possible_params[] = {"rows", "columns", "tokens"};

    printf("Please enter number of %s: ", possible_params[param]);
}

// message number 4 = "Please choose starting color (Y)ellow or (R)ed: "
void print_chose_color_message()
{
    printf("Please choose starting color (Y)ellow or (R)ed: ");
}

// message number 5 = "Your move, player <player>. "
void print_chose_move_message(int player)
{
    printf("Your move, player %d. ", player);
}

// message number 6
void print_enter_column_message()
{
    printf("Please enter column: ");
}

void print_full_column_message()
{
    printf("Column full. ");
}

void print_unavailable_undo_redo_message()
{
    printf("No moves to undo/redo. ");
}

/*------------------------------Help Functions------------------------*/

/*
 * resets the 2D array board
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 *
 * # Lentgh: 3
 */
void reset_board(char board[][BOARD_MAX_SIDE]){
    for(int i = 0 ; i < BOARD_MAX_SIDE; ++i)
        for(int j = 0 ; j < BOARD_MAX_SIDE; ++j)
            board[i][j] = EMPTY_SLOT_SYMBOL;
}

/*
 * Outputs game board.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 *
 * # Lentgh: 5
 */
void print_board(char board[][BOARD_MAX_SIDE], int board_side[])
{
    for (int row = 0; row < board_side[0]; ++row)
    {
        printf("\n%c", BOARD_VERT_SEP);
        for (int col = 0; col < board_side[1]; ++col)
            printf("%c%c", board[row][col], BOARD_VERT_SEP);
    }
    printf("\n%c", BOARD_LEFT_ANG);
    for (int ii = 0; ii < board_side[1] - 1; ++ii)
        printf("%c%c", BOARD_BOTTOM, BOARD_BOTTOM_SEP);
    printf("%c%c\n", BOARD_BOTTOM, BOARD_RIGHT_ANG);
}

/*
 * Outputs which player won.
 *
 * player_id - player's number identifier
 *
 * # Lentgh: 3
 */
void print_winner(int player_id)
{
    if (player_id > TIE)
        printf("Player %d won! \n", player_id);
    else
        printf("That's a tie. \n");
}

/*
 * Prints specific request (to get input) by specific boolian terms happening
 *
 * is_valid_column - specify whether the column is between 1 and 9.
 * is_column_full - specify whether the specific wanted column is full
 * ptr_is_valid_action - specify whether the specific wanted action (redo / undo) can happen
 * ptr_action - pointer to the action (column input from the user)
 *
 * # Lentgh: 7
 */
void print_request_to_scan(bool is_valid_column,bool is_column_full, bool *ptr_is_valid_action, int *ptr_action){
    if(is_valid_column && is_column_full)
        print_full_column_message();
    else
        if((*ptr_action == UNDO || *ptr_action == REDO) && !*ptr_is_valid_action){
            print_unavailable_undo_redo_message();
            print_enter_column_message();
            return;
        }
    print_enter_column_message();
}


/*
 * Performs the whole game. By getting input, processing it and repeating the process until the end.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * players_symbols - contains players slot symbols {player1, player2}
 * number_tokens - number of tokens required to win the game
 *
 * # Lentgh: 12
 */
int perform_game(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], int number_tokens){
    bool is_last_undo_or_redo = false, is_second_player_move = false, *ptr_undo_or_redo = &is_last_undo_or_redo,
    *ptr_is_second_player_move = &is_second_player_move;

    int actions[BOARD_MAX_SIDE * BOARD_MAX_SIDE] = {0}, current_action = 0, columns_elements_number[BOARD_MAX_SIDE] = {0},
    action, *ptr_action = &action, *ptr_current_action = &current_action, winner = NO_ONE;


    while(winner == NO_ONE){
        if(scan_move_and_apply_it(board, board_side, ptr_is_second_player_move, ptr_action, columns_elements_number, is_last_undo_or_redo, 
                                  players_symbols, ptr_undo_or_redo, actions, ptr_current_action) == 1) return 1;
        if(!is_last_undo_or_redo)
            winner = check_winning(board, board_side, players_symbols, number_tokens, !is_second_player_move, action,
                                   columns_elements_number);
    }
    print_board_and_winner(board, board_side, winner);
    return 0;
}
void print_board_and_winner(char board[][BOARD_MAX_SIDE], int board_side[], int winner){
    print_board(board, board_side);
    print_winner(winner);
}
/*
 * Checks if the last action made conducted a win.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * players_symbols - contains players slot symbols {player1, player2}
 * number_tokens - number of tokens required to win the game
 * is_second_player_move - indicates to which player the move belongs
 * action - action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * # Lentgh: 11
 */
int check_winning(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], int number_tokens, bool is_second_player_move, int action,
                  int columns_elements_number[]){
    char symbol_to_check = players_symbols[is_second_player_move];
    bool column_win, diagonal_up_win, diagonal_down_win, row_win;
    column_win = check_sequence_in_column(board, board_side, symbol_to_check, number_tokens, action, columns_elements_number);
    diagonal_up_win = check_sequence_in_diagonal_up(board, board_side, symbol_to_check, number_tokens, action, columns_elements_number);
    diagonal_down_win = check_sequence_in_diagonal_down(board, board_side, symbol_to_check, number_tokens, action, columns_elements_number);
    row_win = check_sequence_in_row(board, board_side, symbol_to_check, number_tokens, action, columns_elements_number);
    if(column_win || diagonal_up_win || diagonal_down_win || row_win)
        return is_second_player_move + 1;
    if(is_board_full(board, board_side))
        return TIE;
    return NO_ONE;
}

/*
 * Checks if the last action made the board full, indicates the end of the game.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 *
 * # Lentgh: 5
 */
bool is_board_full(char board[][BOARD_MAX_SIDE], int board_side[]){
    for (int row = 0; row < board_side[0]; ++row)
        for (int col = 0; col < board_side[1]; ++col)
            if(board[row][col] == EMPTY_SLOT_SYMBOL)
                return false;
    return true;
}

/*
 * Checks if the there is a winning sequence in a specific column.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * symbol_to_check - symbol of the last player that played
 * number_tokens - number of tokens required to win the game
 * action - action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * # Lentgh: 10
 */
bool check_sequence_in_column(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                              int columns_elements_number[]){
    int max_in_sequence = 0;
    if(columns_elements_number[action - 1] < number_tokens)
        return false;
    for(int i = board_side[0] - 1; i >= 0; --i){
        if(board[i][action - 1] == symbol_to_check)
            max_in_sequence++;
        else
            max_in_sequence = 0;
        if(max_in_sequence == number_tokens)
            return true;
        }
    return false;
}

/*
 * Checks if the there is a winning sequence in a specific row.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * symbol_to_check - symbol of the last player that played
 * number_tokens - number of tokens required to win the game
 * action - action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * # Lentgh: 8
 */
bool check_sequence_in_row(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                              int columns_elements_number[]){
    int max_in_sequence = 0, row = columns_elements_number[action - 1] - 1;
    for(int j = 0; j < board_side[1]; ++j) {
        if (board[board_side[0] - row - 1][j] == symbol_to_check)
            max_in_sequence++;
        else
            max_in_sequence = 0;
        if (max_in_sequence == number_tokens)
            return true;
    }
    return false;
}

/*
 * Checks if the there is a winning sequence in the up diagonal.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * symbol_to_check - symbol of the last player that played
 * number_tokens - number of tokens required to win the game
 * action - action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * # Lentgh: 10
 */
bool check_sequence_in_diagonal_up(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                           int columns_elements_number[]){
    int max_in_sequence = 0, row = columns_elements_number[action - 1] - 1, column = action - 1, min_row_col = row > column ? column : row;
    int x_start_point = row - min_row_col, y_start_point = column - min_row_col;
    int maximum_moves = max_moves(board_side, x_start_point, y_start_point, true);
    for (int i = 0; i < maximum_moves; ++i) {
        if (board[board_side[0] - x_start_point - i - 1][y_start_point + i] == symbol_to_check)
            max_in_sequence++;
        else
            max_in_sequence = 0;
        if (max_in_sequence == number_tokens)
            return true;
    }
    return false;
}

/*
 * Checks if the there is a winning sequence in the down diagonal.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * symbol_to_check - symbol of the last player that played
 * number_tokens - number of tokens required to win the game
 * action - action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * # Lentgh: 11
 */
bool check_sequence_in_diagonal_down(char board[][BOARD_MAX_SIDE], int board_side[], char symbol_to_check, int number_tokens, int action,
                                   int columns_elements_number[]){
    int max_in_sequence = 0, row = columns_elements_number[action - 1] - 1, column = action - 1;
    int min_row_col = row > board_side[1] - column - 1 ? board_side[1] - column - 1 : row;
    int x_start_point = row - min_row_col, y_start_point = column + min_row_col;
    int maximum_moves = max_moves(board_side, x_start_point, y_start_point, false);
    for (int i = 0; i < maximum_moves; ++i) {
        if (board[board_side[0] - x_start_point - i - 1][y_start_point - i] == symbol_to_check)
            max_in_sequence++;
        else
            max_in_sequence = 0;
        if (max_in_sequence == number_tokens)
            return true;
    }
    return false;
}

/*
 * Returns how many posibble elements are they in a diagonal.
 *
 * board_side - contains board dimensions {rows, cols}
 * x_start_point - index of the element's row
 * y_start_point - index of the element's column
 * is_diagonal_up - specify in what directions we want to check the diagonal's elements number
 * # Lentgh: 6
 */
int max_moves(int board_side[], int x_start_point, int y_start_point, bool is_diagonal_up){
    int difference_x = board_side[0] - x_start_point, difference_y;
    if(is_diagonal_up)
        difference_y = board_side[1] - y_start_point;
    else
        difference_y = y_start_point + 1;
    return difference_x > difference_y ? difference_y : difference_x;
}

/*
 * Get move input from the user and applies it to the interface.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * ptr_is_second_player_move - indicates to which player the move belongs
 * ptr_action - pointer to the action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * is_last_undo_or_redo - specify whether the last action was a redo or undo
 * players_symbols - contains players slot symbols {player1, player2}
 * actions - array that holds the history of the actions made.
 * ptr_current_action - pointer to the current action in the actions array
 *
 * # Lentgh: 7
 */
int scan_move_and_apply_it(char board[][BOARD_MAX_SIDE], int board_side[], bool *ptr_is_second_player_move, int *ptr_action,
                            int columns_elements_number[], bool is_last_undo_or_redo, char players_symbols[],
                            bool *ptr_undo_or_redo, int actions[], int *ptr_current_action){
    print_board(board, board_side);
    print_chose_move_message(*ptr_is_second_player_move + 1);
    print_enter_column_message();
    if(scan_move(ptr_action, columns_elements_number, board_side, is_last_undo_or_redo, *ptr_current_action, actions) == 1) return 1;
    apply_move(board, board_side, players_symbols, ptr_is_second_player_move, *ptr_action, columns_elements_number, ptr_undo_or_redo, actions,
        ptr_current_action);
    return 0;
}

/*
 * Gets move input from the user and processing it (checking validation).
 *
 * board_side - contains board dimensions {rows, cols}
 * ptr_is_second_player_move - indicates to which player the move belongs
 * ptr_action - pointer to the action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * is_last_undo_or_redo - specify whether the last action was a redo or undo
 * actions - array that holds the history of the actions made.
 * ptr_current_action - pointer to the current action in the actions array
 *
 * # Lentgh: 7
 */
int scan_move(int *ptr_action, int columns_elements_number[], int board_side[], bool is_last_undo_or_redo,
               int current_action, int actions[]){
    int res, *ptr_res = &res;
    bool is_column_full = true, is_valid_column, is_valid_action;
    bool *ptr_is_valid_column = &is_valid_column, *ptr_is_column_full = &is_column_full, *ptr_is_valid_action = &is_valid_action;

    res = scanf("%d", ptr_action);
    if(!res || res == EOF)
        return 1;
    is_valid_column = is_in_range(*ptr_action, 1, board_side[1]);
    is_column_full = is_valid_column && columns_elements_number[*ptr_action - 1] == board_side[1];
    is_valid_action = (*ptr_action == UNDO || *ptr_action == REDO) && is_action_valid(ptr_action, current_action, is_last_undo_or_redo, actions);
    while(!(is_valid_column && !is_column_full) && !((*ptr_action != UNDO || *ptr_action != REDO) && is_valid_action)){
        if(scan_move_again(ptr_action, ptr_is_valid_column, ptr_is_column_full, board_side, columns_elements_number, ptr_res,
                        ptr_is_valid_action, current_action, is_last_undo_or_redo, actions) == 1)
            return 1;
    }
    return 0;
}

/*
 * Returns if it is posiible to perform an UNDO or REDO action
 *
 * ptr_action - pointer to the action (column input from the user)
 * ptr_current_action - pointer to the current action in the actions array
 * is_last_undo_or_redo - specify whether the last action was a redo or undo
 * actions - array that holds the history of the actions made.
 *
 * # Lentgh: 3
 */
bool is_action_valid(int *ptr_action, int current_action, bool is_last_undo_or_redo, int actions[]){
    if(*ptr_action == UNDO){
        return current_action > 0;
    }
    return is_last_undo_or_redo && actions[current_action] != 0;
}

/*
 * Get move input from the user and processes terms to check it the input is valid .
 *
 * board_side - contains board dimensions {rows, cols}
 * ptr_is_valid_action - specify whether the specific wanted action (redo / undo) can happen
 * ptr_is_column_full - specify whether the specific wanted column is full
 * ptr_action - pointer to the action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * is_last_undo_or_redo - specify whether the last action was a redo or undo
 * players_symbols - contains players slot symbols {player1, player2}
 * actions - array that holds the history of the actions made.
 * current_action - the current action in the actions array
 * res - indicates whether a char was successfully scaned from the user.
 * ptr_is_valid_action - specify whether the specific wanted action (redo / undo) can happen
 *
 * # Lentgh: 8
 */
int scan_move_again(int *ptr_action, bool *ptr_is_valid_column, bool *ptr_is_column_full, int board_side[], int columns_elements_number[],
                     int *res, bool *ptr_is_valid_action, int current_action, bool is_last_undo_or_redo, int actions[])
{
    print_request_to_scan(*ptr_is_valid_column,*ptr_is_column_full, ptr_is_valid_action, ptr_action);
    *res = scanf("%d", ptr_action);
    if(!*res || *res == EOF)
        return 1;
    *ptr_is_valid_column = is_in_range(*ptr_action, 1, board_side[1]);
    *ptr_is_column_full = *ptr_is_valid_column && columns_elements_number[*ptr_action - 1] == board_side[1];
    *ptr_is_valid_action = (*ptr_action == UNDO || *ptr_action == REDO) && is_action_valid(ptr_action, current_action, is_last_undo_or_redo, actions);
    return 0;
}
/*
 * Applying the move scaned.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * players_symbols - contains players slot symbols {player1, player2}
 * ptr_is_second_player_move - indicates to which player the move belongs
 * action - the action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * ptr_undo_or_redo - specify whether the last action was a redo or undo
 * actions - array that holds the history of the actions made.
 * ptr_current_action - pointer to the current action in the actions array
 *
 * # Lentgh: 8
 */
void apply_move(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], bool *ptr_is_second_player_move, int action,
                int columns_elements_number[], bool *ptr_undo_or_redo, int actions[], int *ptr_current_action){
    int row_number;
    if(is_in_range(action, 1, BOARD_MAX_SIDE)) {
        update_actions(*ptr_undo_or_redo, ptr_current_action, actions);
        row_number = board_side[0] - columns_elements_number[action - 1];
        apply_regular_move(board, players_symbols, *ptr_is_second_player_move, row_number, action, ptr_undo_or_redo, columns_elements_number);
        actions[*ptr_current_action - 1] = action;

    }
    else
        apply_changing_move(board, board_side, players_symbols, *ptr_is_second_player_move, action, columns_elements_number, ptr_undo_or_redo, actions
                            ,ptr_current_action);
    *ptr_is_second_player_move = !*ptr_is_second_player_move;
}

/*
 *
 */
void update_actions(bool ptr_undo_or_redo, int *ptr_current_action, int actions[]){
    if(ptr_undo_or_redo) {
        for (int i = *ptr_current_action + 1; i < BOARD_MAX_SIDE * BOARD_MAX_SIDE; ++i)
            actions[i] = 0;
    }
    (*ptr_current_action)++;
}

/*
 * Applying the regular move (Not REDO or UNDO).
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * players_symbols - contains players slot symbols {player1, player2}
 * is_second_player_move - indicates to which player the move belongs
 * row - row's number input from the user
 * column - column's number input from the user
 * columns_elements_number - contains the number of elements currently at each column
 * ptr_undo_or_redo - specify whether the last action was a redo or undo
 *
 * # Lentgh: 3
 */
void apply_regular_move(char board[][BOARD_MAX_SIDE], char players_symbols[], bool is_second_player_move,int row, int column, bool *ptr_undo_or_redo,
                        int columns_elements_number[]){
    board[row - 1][column - 1] = players_symbols[is_second_player_move];
    columns_elements_number[column - 1]++;
    *ptr_undo_or_redo = false;

}

/*
 * Applying the REDO or UNDO action.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 * players_symbols - contains players slot symbols {player1, player2}
 * is_second_player_move - indicates to which player the move belongs
 * action - the action (column input from the user)
 * columns_elements_number - contains the number of elements currently at each column
 * ptr_undo_or_redo - specify whether the last action was a redo or undo
 * actions - array that holds the history of the actions made.
 * ptr_current_action - pointer to the current action in the actions array
 *
 * # Lentgh: 11
 */
void apply_changing_move(char board[][BOARD_MAX_SIDE], int board_side[], char players_symbols[], bool is_second_player_move, int action,
                         int columns_elements_number[], bool *ptr_undo_or_redo, int actions[], int *ptr_current_action){
    int row, column;
    *ptr_undo_or_redo = true;
    if(action == UNDO){
        column = actions[*ptr_current_action - 1];
        row = board_side[0] - --columns_elements_number[column - 1];
        board[row - 1][column - 1] = EMPTY_SLOT_SYMBOL;
        (*ptr_current_action)--;
    }
    else{
        column = actions[*ptr_current_action];
        row = board_side[0] - ++columns_elements_number[column - 1];
        board[row][column - 1] = players_symbols[is_second_player_move];
        (*ptr_current_action)++;
    }
}
/*
 * Gets a number {element} and a range {min, max}, Returns whether the number is in the range
 * Length : 1
 */
bool is_in_range(int element, int min, int max){
    return element >= min && element <= max;
}

/*
 * Scans the first paramethers needed to start the game from the user.
 * ptr_first_player_symbol - pointer to the variable holds the first player symbol
 * board_side - contains board dimensions {rows, cols}
 * ptr_tokens - pointer to the variable holds the number of tokens required to win the game
 *
 * Length: 10
 */
int scan_firsts_params(char *ptr_first_player_symbol, int board_side[], int *ptr_tokens){
    if(scan_symbol_from_user(ptr_first_player_symbol) == 1)
        return 1;
    if(scan_number_from_user(&board_side[0], board_side, BOARD_MIN_SIDE, BOARD_MAX_SIDE, MSG_GET_BOARD_ROWS) == 1)
        return 1;
    if(scan_number_from_user(&board_side[1], board_side, BOARD_MIN_SIDE, BOARD_MAX_SIDE, MSG_GET_BOARD_COLS) == 1)
        return 1;
    int max_tokens_range = board_side[0] > board_side[1] ? board_side[1] : board_side[0];
    if(scan_number_from_user(ptr_tokens, board_side, MIN_TOKENS, max_tokens_range, MSG_GET_NUMBER_TOKENS) == 1)
        return 1;
    return 0;
}

/*
 * Scans the symbol needed for the first player
 *
 * symbol - pointer to the variable holds the symbol
 *
 * Length: 8
 */
int scan_symbol_from_user(char *symbol){
    int res;
    do{
        print_chose_color_message();
        res = scanf(" %c", symbol);
        if(!res || res == EOF)
            return 1;        
    } while(*symbol != YELLOW_SLOT_SYMBOL && *symbol != RED_SLOT_SYMBOL);
    return 0;
}

/*
 * Scans the number needed for starting the game (Dimensions)
 *
 * number - pointer to the variable holds the number
 * board_side - contains board dimensions {rows, cols}
 * min_range - specify the minimum range required for the number to be in
 * max_range - specify the maximum range required for the number to be in
 * func_number - indicates what number we are scanning (this required for the printing funtion)
 * Len:10
 */
int scan_number_from_user(int *number, int board_side[], int min_range, int max_range, int func_number){
    int res;
    bool is_col_valid = false;
    do{
        print_read_game_params_message(func_number);
        res = scanf("%d", number);
        if(!res || res == EOF)
            return 1;
        is_col_valid = func_number == 1 ? board_side[0] <= board_side[1] : true;
    } while(!is_in_range(*number, min_range, max_range) || !is_col_valid);
    return 0;
}
