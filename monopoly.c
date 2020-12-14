#include "monopoly.h"
void dash_line(int number){
	printf("\n");
	for (int i = 0; i < number; i++){
		printf("=");
	}
	printf("\n");
}
int length_of_string(char string[],int name_length,int limit){
	//limit 16
	//printf("%s\n", string);
	for (int k = 0; k < limit; k++){
		if (string[k] == '\0'){
			
			name_length = k;
			break;
		}
		if ((k+1) == limit){
			
			string[k] = '\0';
			name_length = limit;
		}
	}
	return name_length;
}
int number_entered(int min, int max, char *str, char *success, char *prompt){
	//dash_line(DASH);
	char line[17];
	int i;
	
restart:printf("%s",prompt);
	if (fgets(line, 17, stdin)) {
	    if (1 == sscanf(line, "%d", &i)) {
	  
		if ((i < min) || (i > max)){
			printf(RED("%s"),str);
			if (line[15] != '\0'){
				int val = 0;
				if (length_of_string(line, val, 16) == 16){ 
					while (fgetc(stdin)!='\n'){
					
					}
				}
			}					
			goto restart;
		}
		if (line[15] != '\0'){
			int val = 0;
			if (length_of_string(line, val, 16) == 16){ 
				while (fgetc(stdin)!='\n'){
					
				}
			}
		}
		//num_of_players = i;
		printf(GREEN("%s"),success);
		return i;
	    }
	    else{
		printf(RED("%s"),str);
		if (line[15] != '\0'){
			int val = 0;
			if (length_of_string(line, val, 16) == 16){ 
				while (fgetc(stdin)!='\n'){
					
				}
			}
		}
		

		goto restart;
	    }
	}
}
int is_empty_save_slot(char *filename, int print_out){
	FILE *fp = fopen(filename, "r");
	int i = 0;
	if (fp == NULL){
		if (print_out == 1){
			printf("EMPTY SLOT\n");
		}
		return 1;
		
	}


	if (print_out == 1){
		printf("LOAD SAVE FILE\n");
	}
	return 0;
}
void initialize_player(char name[], int player_id, int curr_pos, float money, int jail, int time_in_jail, player_t *player){
	player->name = malloc(16);
	memcpy(player->name, name, 16);	
	//printf("%s\n", player->name);
//	player->name = name;
	player->player_id = player_id;
	player->current_position = curr_pos;
	player->money = money;
	player->in_jail = jail;
	player->time_spent_in_jail=time_in_jail;
}
void print(char *str){
	int string_len = 0;
	int line_count = 0;
	for (int i = 0; str[i] != '\0'; i++){
		string_len++;
	}
	
	line_count = string_len / TEXT_WIDTH;
	if ((string_len % TEXT_WIDTH)!=0){
		line_count++;
	}
	//printf("string_len, line_count %d %d\n", string_len, line_count);
	
	int ptr = 0;
	int temp = 0;
	for (int i = 0; i < line_count; i++){
		printf("\n");		
		temp = ptr;
		while (ptr < temp + TEXT_WIDTH){
			if (string_len == ptr){
				break;
			}
			printf("%c", str[ptr]);
			
			ptr++;
		}
		
	}
}
void start_new_game(){
     //   printf("\n");


	dash_line(DASH);
	char line[17];
	int i;
	num_of_players = number_entered(2,8, "\nThis is an invalid amount of players, please enter a value between 2 - 8\n\n", "\nPlayer number successfully updated\n\n", ">> Enter the number of players that would be participating: ");

	dash_line(DASH);
	int loop = 0;
	//get player info.	
	//players = malloc(sizeof(player_t *) * num_of_players);
	while (loop < num_of_players){
		
		int name_length = 0;
		char name[17];
		char real_name[17];
repeat: 	printf(">> Please enter player %d's name: ",loop + 1);
		if (fgets(name,17,stdin)){		
			if (sscanf(name, "%s\n", real_name) == 1){
				name_length = length_of_string(name, name_length, 16);
				real_name[15] ='\0';
				
				player_id++;	
			}
			else{
				printf("unexpected error\n");
				exit(1);
			}
		}
		if (name_length >= 16){
			while (fgetc(stdin)!='\n'){
			}
		}
		for (int i = 0; i < player_id - 2; i++){
			if (strcmp(real_name, players[i].name) == 0){
				player_id--;
				printf(RED("\nUsername already exist, try again\n\n"));	
				goto repeat;
			}

		}
		printf(GREEN("\nWelcome, %s, you are player %d\n\n"),real_name, player_id);
		initialize_player(real_name, player_id - 1,0, INITIAL_CASH, 0, 0, &players[loop]);
		
		loop++;
	}
	dash_line(DASH);
	printf("\nLet the game begin\n");
}	
int main(){
	
	dash_line(DASH);	
	
	printf("\nWelcome to Monopoly\n");
	dash_line(DASH);
	print("The rules of the game are simple. Firstly, player usernames that inclde spaces are truncated. Usernames can only be 15 characters long. Only 2 - 8 players can play at a time. This game can be saved and be resumed at a later date. There is no autosave, save must be done by selecting the save option while playing");
	dash_line(DASH);
	
	printf("\nThis is the main menu\n");
	printf("1. Start a new game\n");
	printf("2. Continue a prevous game\n");
	int option = number_entered(1, 2, "\nThis is an invalid menu option, Enter a 1 or a 2\n\n","\nMenu successfully entered\n\n","\n>> Enter a menu option: ");
	if (option == 1){
		start_new_game();
	}
	if (option == 2){
		dash_line(DASH);
		printf("SAVE SLOTS:\n");
		printf("1. ");
		int ret_1 = is_empty_save_slot(save_file_one, 1);
		printf("2. ");
		int ret_2 = is_empty_save_slot(save_file_two, 1);
		printf("3. ");
		int ret_3 = is_empty_save_slot(save_file_three, 1);
		if (ret_1 + ret_2 + ret_3 == 3){
			dash_line(DASH);
			print("All save files are empty, would you like to start a new game? Enter 1 for Yes, and 0 for NO");
			printf("\n");
			int opt = number_entered(0, 1, "\nThis is an invalid menu option, Enter a 1 for yes or 0 for no\n\n","\nMenu successfully entered\n\n","\n>> Enter a menu option: ");
			if (opt == 1){
				start_new_game();
			}	

			if (opt == 0){
				printf("\nSAD TO SEE YOU GO!!!SEE YOU NEXT TIME\n\n");
			}
		}
		
	}
	dash_line(DASH);
	initscr();
	cbreak();
	//noecho();
	keypad(stdscr, TRUE);
	printw("Hello World !!");
	
	refresh();
	getch();
	getch();
	endwin();			 
	/*for (int i = 0; i < num_of_players; i++){
		printf("print real name for player, %s\n", players[i].name);
	}*/

	return 0;

}
