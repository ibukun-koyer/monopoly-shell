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
void initialize_player(char name[], int player_id, int curr_pos, float money, int jail, int time_in_jail, player_t *player){
	player->name = malloc(16);
	memcpy(player->name, name, 16);	
	printf("%s\n", player->name);
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
int main(){
        printf("\n");
	dash_line(DASH);	
	
	printf("\nWelcome to Monopoly\n");
	print("The rules of the game are simple. Firstly, player usernames that inclde spaces are truncated. Usernames can only be 15 characters long. Only 2 - 8 players can play at a time. This game can be saved and be resumed at a later date.");
	dash_line(DASH);
	char line[17];
	int i;
	
restart:printf(">> Enter the number of players that would be participating: ");
	if (fgets(line, 17, stdin)) {
	    if (1 == sscanf(line, "%d", &i)) {
	  
		if ((i < 2) || (i > 8)){
			printf(RED("\nThis is an invalid amount of players, please enter a value between 2 - 8\n\n"));
			if (line[15] != '\0'){
				while (fgetc(stdin)!='\n'){
					
				}
			}					
			goto restart;
		}
		if (line[15] != '\0'){
			while (fgetc(stdin)!='\n'){
					
			}
		}
		num_of_players = i;
		printf(GREEN("\nPlayer number successfully updated\n\n"));
	    }
	    else{
		printf(RED("\nThis is an invalid amount of players, please enter a value between 2 - 8\n\n"));
		if (line[15] != '\0'){
			while (fgetc(stdin)!='\n'){
					
			}
		}
		

		goto restart;
	    }
	}
	dash_line(DASH);
	int loop = 0;
	//get player info.	
	//players = malloc(sizeof(player_t *) * num_of_players);
	while (loop < num_of_players){
		
		int name_length = 0;
		char name[17];
		char real_name[17];
		printf(">> Please enter player %d's name: ",loop + 1);
		if (fgets(name,17,stdin)){		
			if (sscanf(name, "%s\n", real_name) == 1){
				name_length = length_of_string(name, name_length, 16);
				real_name[15] ='\0';
				printf(GREEN("\nWelcome, %s, you are player %d\n\n"),real_name, player_id);
				player_id++;	
			}
			else{
				printf("unexpected error\n");
				return 1;
			}
		}
		if (name_length >= 16){
			while (fgetc(stdin)!='\n'){
			}
		}

		initialize_player(real_name, player_id - 1,0, INITIAL_CASH, 0, 0, &players[loop]);
		
		loop++;
	}
	dash_line(DASH);
	printf("\nLet the game begin\n");
	for (int i = 0; i < num_of_players; i++){
		printf("print real name for player, %s\n", players[i].name);
	}













	return 0;

}
