#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#define RED 12
#define GREEN 10
#define BLUE 9

struct game{
    char player1[20];
    char player2[20];
    char* boards;
    char* unfinished;
    int player1_score;
    int player2_score;
};

struct node{
    struct game data;
    struct node* link;
};

int set_turn (char board[10]){
    int x = 0, o = 0;
    for ( int i = 1; i <= 9 ; i++){
        if ( board[i] == 'X'){
            x++;
        }
        else if ( board[i] == 'O'){
            o++;
        }
    }
    if ( x == o){
        return 1;
    }
    else if ( o > x){
        return 2;
    }
}

int is_unfinished (char board[10]){
    if ( board[0] == 'y'){
        return 1;
    }
    else if ( board[0] == 'd'){
        return 2;
    }
    return 0;
}

void add_game (struct node *head,struct game dta){
    struct node *ptr, *tmp;
    ptr = head;
    tmp = (struct node *) malloc(sizeof(struct node));
    tmp ->data = dta;
    tmp ->link = NULL;
    if ( head ->link == NULL){
        head ->link = tmp;
        return;
    }

    while (ptr ->link != NULL){
        ptr = ptr ->link;
    }
    ptr ->link = tmp;
}
void edit_game (struct node *head, struct game chosen_game){
    struct node *ptr;
    ptr = head;
    while (1){
        if (strcmp(ptr ->data.player1, chosen_game.player1) == 0 && strcmp(ptr ->data.player2, chosen_game.player2) == 0){
            ptr ->data = chosen_game;
            return;
        }
        if ( ptr ->link == NULL){
            return;
        }
        ptr = ptr ->link;
    }
}

void search_game_to_print (struct node *head, char* name){
    struct node *ptr;
    ptr = head;
    int flag = 0;
    while (1){
        if (strcmp(ptr ->data.player1, name) == 0 || strcmp(ptr ->data.player2, name) == 0){
            print_game(ptr ->data);
            flag = 1;
        }
        if ( ptr ->link == NULL){
            break;
        }
        ptr = ptr ->link;
    }
    if ( flag == 0){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),RED);
        printf("\n'%s' has played no games\n", name);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    }
}

int get_list_size (struct node *head){
    struct node *ptr;
    int cnt = 0;
    ptr = head;
    while (1){
        if ( ptr ->link == NULL){
            break;
        }
        cnt++;
        ptr = ptr ->link;
    }
    return cnt;
}

struct game get_game (struct node *head, char* name, char* name2){
    struct node *ptr;
    ptr = head ->link;
    while (1){
        if ((strcmp(ptr ->data.player1, name) == 0 && strcmp(ptr->data.player2, name2) == 0) || (strcmp(ptr ->data.player2, name) == 0 && strcmp(ptr->data.player1, name2) == 0)){
            return ptr ->data;
        }
        if ( ptr ->link == NULL){
            break;
        }
        ptr = ptr ->link;
    }
}

int search_game (struct node *head, char* name, char* name2){
    struct node *ptr;
    ptr = head;
    while (1){
        if ((strcmp(ptr ->data.player1, name) == 0 && strcmp(ptr->data.player2, name2) == 0) || (strcmp(ptr ->data.player2, name) == 0 && strcmp(ptr->data.player1, name2) == 0)){
            return 1;
        }
        if ( ptr ->link == NULL){
            return 0;
        }

        ptr = ptr ->link;
    }
}

void export_list (struct node *head){
    FILE *fp;
    struct node *ptr;
    ptr = head ->link;
    fp = fopen("save.txt", "w");
    if ( fp == NULL){
        return;
    }

    int cnt = get_list_size(head);
    fprintf(fp, "%d\n", cnt);
    while (1){
        fprintf(fp, "%s %s %d %d %s\n", ptr->data.player1, ptr->data.player2, ptr->data.player1_score, ptr->data.player2_score, ptr->data.boards);
        if ( ptr->link == NULL){
            fclose(fp);
            return;
        }
        ptr = ptr ->link;
    }
}

void import_list (struct node *head){
    FILE *fp;
    struct node *temp;
    fp = fopen("save.txt", "r");
    int cnt;
    fscanf(fp, "%d\n", &cnt);
    for ( int i = 0 ; i < cnt ; i++){
        temp = malloc ( sizeof (struct node));
        temp ->data.boards = calloc (500, sizeof(char));
        temp ->data.unfinished = calloc (10, sizeof(char));
        strcpy(temp ->data.unfinished, "0123456789");
        fscanf(fp, "%s %s %d %d %s", temp ->data.player1, temp ->data.player2, &temp->data.player1_score, &temp->data.player2_score, temp->data.boards);
        add_game(head, temp->data);
        free(temp);
    }
    fclose(fp);
}

int can_move (char board[10]){
    for ( int i = 1 ; i < 10 ; i++){
        if ( board[i] != 'X' && board[i] != 'O'){
            return 1;
        }
    }
    return 0;
}
int checkwin(char board[10]){
    if (board[1] == board[2] && board[2] == board[3])
        return 1;

    else if (board[4] == board[5] && board[5] == board[6])
        return 1;

    else if (board[7] == board[8] && board[8] == board[9])
        return 1;

    else if (board[1] == board[4] && board[4] == board[7])
        return 1;

    else if (board[2] == board[5] && board[5] == board[8])
        return 1;

    else if (board[3] == board[6] && board[6] == board[9])
        return 1;

    else if (board[1] == board[5] && board[5] == board[9])
        return 1;

    else if (board[3] == board[5] && board[5] == board[7])
        return 1;

    else if (board[1] != '1' && board[2] != '2' && board[3] != '3' &&
        board[4] != '4' && board[5] != '5' && board[6] != '6' && board[7]
        != '7' && board[8] != '8' && board[9] != '9')

        return 0;
    else
        return  - 1;
}

void print_board (char board[10]){
    printf("\n        _____ _____ _____");
    printf("\n       |     |     |     |\n");
    printf("       |  ");
    if ( board[1] == 'X'){
        color_print("X  ", RED);
    }
    else if ( board[1] == 'O'){
        color_print("O  ", GREEN);
    }
    else{
        printf("%c  ", board[1]);
    }
    printf("|  ");

    if ( board[2] == 'X'){
        color_print("X  ", RED);
    }
    else if ( board[2] == 'O'){
        color_print("O  ", GREEN);
    }
    else{
        printf("%c  ", board[2]);
    }
    printf("|  ");

    if ( board[3] == 'X'){
        color_print("X  ", RED);
        printf("|\n");
    }
    else if ( board[3] == 'O'){
        color_print("O  ", GREEN);
        printf("|\n");
    }
    else{
        printf("%c  |\n", board[3]);
    }

    printf("       |_____|_____|_____|\n");
    printf("       |     |     |     |\n");

    printf("       |  ");
    if ( board[4] == 'X'){
        color_print("X  ", RED);
    }
    else if ( board[4] == 'O'){
        color_print("O  ", GREEN);
    }
    else{
        printf("%c  ", board[4]);
    }
    printf("|  ");

    if ( board[5] == 'X'){
        color_print("X  ", RED);
    }
    else if ( board[5] == 'O'){
        color_print("O  ", GREEN);
    }
    else{
        printf("%c  ", board[5]);
    }
    printf("|  ");

    if ( board[6] == 'X'){
        color_print("X  ", RED);
        printf("|\n");
    }
    else if ( board[6] == 'O'){
        color_print("O  ", GREEN);
        printf("|\n");
    }
    else{
        printf("%c  |\n", board[6]);
    }

    printf("       |_____|_____|_____|\n");
    printf("       |     |     |     |\n");

    printf("       |  ");
    if ( board[7] == 'X'){
        color_print("X  ", RED);
    }
    else if ( board[7] == 'O'){
        color_print("O  ", GREEN);
    }
    else{
        printf("%c  ", board[7]);
    }
    printf("|  ");

    if ( board[8] == 'X'){
        color_print("X  ", RED);
    }
    else if ( board[8] == 'O'){
        color_print("O  ", GREEN);
    }
    else{
        printf("%c  ", board[8]);
    }
    printf("|  ");

    if ( board[9] == 'X'){
        color_print("X  ", RED);
        printf("|\n");
    }
    else if ( board[9] == 'O'){
        color_print("O  ", GREEN);
        printf("|\n");
    }
    else{
        printf("%c  |\n", board[9]);
    }

    printf("       |_____|_____|_____|\n\n");
}

void color_print (char *text, int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
    printf("%s", text);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);


}

void print_game (struct game* chosen_game){

    printf("\n\n\t%s %d - %d %s\n\n", chosen_game->player1, chosen_game->player1_score, chosen_game->player2_score, chosen_game->player2);

    for ( int i = 0 ; i < strlen(chosen_game ->boards) ; i += 10){
        char board[10];
        for ( int j = i, k = 0 ; j < 10 + i ; j++, k++){
            board[k] = chosen_game ->boards[j];
        }

        if ( board[0] == 'y'){
            printf(" %s is 'O'\n %s is 'X'", chosen_game ->player1, chosen_game ->player2);
        }
        else{
            printf(" %s is 'O'\n %s is 'X'", chosen_game ->player2, chosen_game ->player1);
        }
        print_board(board);
    }
}

int check_win_minimax (char board[10], char bot_symbol){
     if (board[1] == board[2] && board[2] == board[3]){
        if ( board[1] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else if (board[4] == board[5] && board[5] == board[6]){
        if ( board[4] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else if (board[7] == board[8] && board[8] == board[9]){
        if ( board[7] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else if (board[1] == board[4] && board[4] == board[7]){
        if ( board[1] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else if (board[2] == board[5] && board[5] == board[8]){
        if ( board[2] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else if (board[3] == board[6] && board[6] == board[9]){
        if ( board[3] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else if (board[1] == board[5] && board[5] == board[9]){
        if ( board[1] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else if (board[3] == board[5] && board[5] == board[7]){
        if ( board[3] == bot_symbol){
            return 1;
        }
        else{
            return -1;
        }
     }
    else
        return 0;
}

int minimax(char board[10], int player, char bot_symbol, char player_symbol) {
    int winner = check_win_minimax(board, bot_symbol);
    if(winner != 0){
      return winner*player;
    }


    int move = -1;
    int score = -2;
    int i;
    for(i = 1; i <= 9; i++) {
        if(board[i] != 'O' && board[i] != 'X') {
            char tmp = board[i];
            if ( player == 1){
                board[i] = bot_symbol;
            }
            else{
                board[i] = player_symbol;
            }
            int thisScore = - minimax(board, player*-1, bot_symbol, player_symbol);
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }
            board[i] = tmp;
        }
    }
    if(move == -1) return 0;
    return score;
}

int ai_move(char board[10], char bot_symbol, char player_symbol) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 1; i <= 9; i++) {
        if(board[i] != 'X' && board[i] != 'O') {
            char tmp = board[i];
            board[i] = bot_symbol;
            int tempScore = -minimax(board, -1, bot_symbol, player_symbol);
            board[i] = tmp;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    return move;
}

void play_game (struct game* chosen_game, int turn_setter, char board[10], int* unfinished){

    int i, choice, turn = 1, input;
    if ( is_unfinished(board)){
        turn_setter = is_unfinished(board);
        turn = set_turn(board);
    }
    char* player1 = malloc (sizeof(char *));
    char* player2 = malloc (sizeof(char *));
    if ( turn_setter == 1){
        strcpy(player1, chosen_game ->player1);
        strcpy(player2, chosen_game ->player2);
        board[0] = 'y';
    }
    else{
        strcpy(player2, chosen_game ->player1);
        strcpy(player1, chosen_game ->player2);
        board[0] = 'd';
    }
    char symbol;
    do
    {
        system("cls");
        printf(" %s is O\n %s is X\n\n", player1, player2);
        printf("        %s %d - %d %s", chosen_game ->player1, chosen_game ->player1_score, chosen_game ->player2_score, chosen_game ->player2);
        print_board(board);
        turn = (turn % 2);
        printf("\n* Enter 0 to end the game unfinished *\n");

        if ( turn == 1){
            printf(" %s's turn: ", player1);
        }
        else{
            printf(" %s's turn: ", player2);
        }
        scanf("%d", &input);
        if ( input == 0){
            *unfinished = 1;
            break;
        }

        if ( turn == 1){
            symbol = 'O';
        }
            else{
                symbol = 'X';
            }

        if ( board[input] != 'O' && board[input] != 'X'){
            board[input] = symbol;
        }
        else
        {
            color_print(" Chosen space is already occupied\n", RED);
            turn -= 1;
            getch();
        }
        i = checkwin(board);

        turn += 1;


    }while (i ==  - 1);

    if ( *unfinished == 1){
        strcpy(chosen_game->unfinished, board);
        return;
    }
    system("cls");
    print_board(board);

    if (i == 1){
        if ( turn == 2){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),GREEN);
            printf("\a %s is the Winner!", player1);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            if ( strcmp(player1, chosen_game->player1)){
                chosen_game ->player2_score += 1;
            }
            else{
                chosen_game ->player1_score += 1;
            }
        }
        else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),GREEN);
            printf("\a %s is the Winner!", player2);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            if ( strcmp(player2, chosen_game->player1)){
                chosen_game ->player2_score += 1;
            }
            else{
                chosen_game ->player1_score += 1;
            }
        }
    }
    else{
        printf("\a Game is a draw");
    }
    strcat(chosen_game ->boards, board);
}

void play_game_bot (struct game* chosen_game, int turn_setter, char board[10], int* unfinished){

    int i, choice, turn = 1, input, bot_num;
    if ( is_unfinished(board)){
        turn_setter = is_unfinished(board);
        turn = set_turn(board);
    }
    char* player1 = malloc (sizeof(char *));
    char* player2 = malloc (sizeof(char *));
    if ( turn_setter == 1){
        strcpy(player1, chosen_game ->player1);
        strcpy(player2, chosen_game ->player2);
        board[0] = 'y';
        bot_num = 0;
    }
    else{
        strcpy(player2, chosen_game ->player1);
        strcpy(player1, chosen_game ->player2);
        board[0] = 'd';
        bot_num = 1;
    }
    char symbol;
    do
    {
        system("cls");
        printf(" %s is O\n %s is X\n\n", player1, player2);
        printf("        %s %d - %d %s", chosen_game ->player1, chosen_game ->player1_score, chosen_game ->player2_score, chosen_game ->player2);
        print_board(board);
        turn = (turn % 2);
        printf("\n* Enter 0 to end the game unfinished *\n");

        if ( turn != bot_num){
            printf(" Your turn: ");
            scanf("%d", &input);
        }
        else{
            printf(" %s's turn: ", player2);
            if ( bot_num == 1){
                input = ai_move(board, 'O', 'X');
            }
            else{
                input = ai_move(board, 'X', 'O');
            }
        }
        if ( input == 0){
            *unfinished = 1;
            break;
        }

        if ( turn == 1){
            symbol = 'O';
        }
            else{
                symbol = 'X';
            }

        if ( board[input] != 'O' && board[input] != 'X'){
            board[input] = symbol;
        }
        else
        {
            color_print(" Chosen space is already occupied\n", RED);
            turn -= 1;
            getch();
        }
        i = checkwin(board);

        turn += 1;


    }while (i ==  - 1);

    if ( *unfinished == 1){
        strcpy(chosen_game->unfinished, board);
        return;
    }
    system("cls");
    print_board(board);

    if (i == 1){
        if ( turn == 2){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),GREEN);
            printf("\a %s is the Winner!", player1);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            if ( strcmp(player1, chosen_game->player1)){
                chosen_game ->player2_score += 1;
            }
            else{
                chosen_game ->player1_score += 1;
            }
        }
        else{
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),GREEN);
            printf("\a %s is the Winner!", player2);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            if ( strcmp(player2, chosen_game->player1)){
                chosen_game ->player2_score += 1;
            }
            else{
                chosen_game ->player1_score += 1;
            }
        }
    }
    else{
        printf("\a Game is a draw");
    }
    strcat(chosen_game ->boards, board);
}

void menu (struct node* head){
    int flag = 0;
    while (1){
        system("cls");
        if ( flag == 1){
            color_print("Invalid input\n", RED);
        }
        printf("1. Start New Game\n2. Load a Previous Game\n3. Points\n4. Exit\n>> ");
        int input;
        scanf("%d", &input);
        if ( input == 1){
            system("cls");
            printf("1. Single Player\n2. Multiplayer\n>> ");
            int input1;
            scanf("%d", &input1);
            if ( input1 == 1 ){
                char name_single[20];
                char name_bot[20];
                strcpy(name_bot, "COMPUTER");
                system("cls");
                printf("- Single Player -\n Enter your username: ");
                scanf("%s", name_single);
                struct game *single_player = malloc ( sizeof(struct game));

                if ( search_game(head, name_single, name_bot)){
                    *single_player = get_game(head, name_single, name_bot);
                }
                else{
                    strcpy(single_player->player1, name_single);
                    strcpy(single_player->player2, name_bot);
                    single_player->player1_score = 0;
                    single_player->player2_score = 0;
                    single_player->boards = calloc (500, sizeof(char));
                    single_player ->unfinished = calloc (10, sizeof(char));
                    add_game(head, *single_player);
                }
                int trn = single_player->player1_score + single_player->player2_score;
                while (1){
                    int unfinished = 0;
                    char* board = calloc (10, sizeof(char));
                    strcpy(board, "0123456789");
                    play_game_bot(single_player, (trn % 2) + 1, board, &unfinished);
                    if ( unfinished == 1){
                        edit_game(head, *single_player);
                        export_list(head);
                        break;
                    }
                    printf("\n\n Enter 1 to play again or enter 0 to go back to menu: ");
                    free(board);
                    int input3;
                    scanf("%d", &input3);
                    if ( input3 == 0){
                        edit_game(head, *single_player);
                        export_list(head);
                        break;
                    }
                    trn++;
                }
            }
            else if ( input1 == 2){
                char name_player_1[20];
                char name_player_2[20];
                system("cls");
                printf("- MultiPlayer -\n Enter Player 1 username: ");
                scanf("%s", name_player_1);
                printf(" Enter Player 2 username: ");
                scanf("%s", name_player_2);
                struct game *multi_player = malloc ( sizeof(struct game));

                if ( search_game(head, name_player_1, name_player_2)){
                    *multi_player = get_game(head, name_player_1, name_player_2);
                }
                else{
                    strcpy(multi_player->player1, name_player_1);
                    strcpy(multi_player->player2, name_player_2);
                    multi_player->player1_score = 0;
                    multi_player->player2_score = 0;
                    multi_player->boards = calloc (500, sizeof(char));
                    multi_player->unfinished = calloc(10, sizeof(char));
                    strcpy(multi_player ->unfinished, "0123456789");
                    add_game(head, *multi_player);
                }
                int trn = multi_player->player1_score + multi_player->player2_score;
                while (1){
                    int unfinished;
                    unfinished = 0;
                    char* board = calloc (10, sizeof(char));
                    strcpy(board, "0123456789");
                    play_game(multi_player, (trn % 2) + 1, board, &unfinished);
                    if ( unfinished == 1){
                        edit_game(head, *multi_player);
                        export_list(head);
                        break;
                    }
                    printf("\n\n Enter 1 to play again or enter 0 to go back to menu: ");
                    free(board);
                    int input3;
                    scanf("%d", &input3);
                    if ( input3 == 0){
                        edit_game(head, *multi_player);
                        export_list(head);
                        break;
                    }
                    trn++;
                }
            }
        }

        else if ( input == 2){
            system("cls");
            printf("- Load unfinished game -\n Enter Player 1 username: ");
            char name1[20];
            char name2[20];
            scanf("%s", name1);
            printf(" Enter Player 2 username: ");
            scanf("%s", name2);
            printf("\n");
            struct game *target = malloc (sizeof(struct game));
            *target = get_game(head, name1, name2);
            if ( is_unfinished(target-> unfinished)){
                if ( strcmp(name1, "COMPUTER") == 0 || strcmp(name2, "COMPUTER") == 0){
                    int n = 0;
                    play_game_bot(target, 1 , target->unfinished, &n);
                    edit_game(head, *target);
                    export_list(head);
                }
                else{
                    int n = 0;
                    play_game(target, 1, target->unfinished, &n);
                    edit_game(head, *target);
                    export_list(head);
                }
            }
            else{
                color_print("\nThere is no unfinished game, press enter to go back\n", RED);
                getch();
            }

        }
        else if ( input == 3){
            char name[20];
            system("cls");
            printf("- Points -\n Enter players username: ");
            scanf("%s", name);
            search_game_to_print(head, name);
            printf("\n Press Enter to go back");
            getch();
        }
        else if ( input == 4){
            export_list(head);
            exit(1);
        }
        else {
            flag = 1;
            system("cls");
        }
    }
}

int main(){
    struct node *head = malloc(sizeof(struct node ));
    strcpy(head ->data.player1, "Test");
    strcpy(head ->data.player2, "Test2");
    head ->link = NULL;
    import_list(head);
    menu(head);
    return 0;
}
