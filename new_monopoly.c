#include "monopoly.h"
int is_empty_save_slot(char *filename){
	FILE *fp = fopen(filename, "r");
	if (fp == NULL){
		return 1;
	}

	return 0;
}
void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}
void dash_line(int number){
	//printw("\n");
	for (int i = 0; i < number; i++){
		printw("=");
	}
	//printw("\n");
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
void startup_board(){
	int height;
	int width;
	getmaxyx(stdscr, height, width);
	int starty = (LINES - height)/2;
	int startx = (COLS - width)/2;
	int inner_height = (3 * height)/4;
	int inner_width = (3 * width)/4;
	int inner_starty = (LINES -inner_height)/2;
	int inner_startx = (COLS - inner_width)/2;
	WINDOW *outer;
	WINDOW *inner;
	outer = newwin(height, width, starty, startx);
	inner = newwin(inner_height, inner_width, inner_starty, inner_startx);
	box(outer, 0, 0);
	box(inner, 0, 0);
	int inc = width / 10;
	int next_x = inc;
	for (int i = 0; i < 9;i++){
		for (int j = 0; j < height; j++){
			if (i == 0){
				mvwaddch(outer,j, next_x+(width / 35), ACS_VLINE);
			}
			else if (i == 8){
				mvwaddch(outer,j, next_x-(width /35), ACS_VLINE);
			}
			else{
			
				mvwaddch(outer,j, next_x, ACS_VLINE);
			}
		} 
		next_x+=inc;
	} 
	inc = height /10;
	int next_y = inc;
	for (int i = 0; i < 9;i++){
		for (int j = 0; j < width; j++){
			if (i == 0){
				mvwaddch(outer,next_y+(height / 35), j, ACS_HLINE);
			}
			else if (i == 8){
				mvwaddch(outer, next_y-(height /20), j,ACS_HLINE);
			}
			else{
			
				mvwaddch(outer,next_y, j,ACS_HLINE);
			}
		} 
		next_y+=inc;
	}
	int start_index_x = width - (width/15);
	int start_index_y = height - (height/10);
	int inc_val = 10;
	for (int i = 0; i < 4;i++){	
		for (int j = 0; j < 10; j++){
			if (j != 9){	
				if (i == 1){
					start_index_y -= (height/inc_val);
				}
			
			
				if (i == 2){
					start_index_x += (width/inc_val);
				}
				if (i == 3){
					start_index_y += (height/inc_val);
				}	
			}
			int val = (i * 10)+(j + 1);
			mvwprintw(outer,start_index_y, start_index_x, "%d", val);
			if (j != 9){
				if (i == 0){
					start_index_x -= (width/inc_val);
				}
			}


		} 

	}

	wrefresh(outer);
	wrefresh(inner);
	getch();
	destroy_win(outer);
	destroy_win(inner);
}
void print(char *str, int y, int skip_move){
	int string_len = 0;
	int line_count = 0;
	for (int i = 0; str[i] != '\0'; i++){
		string_len++;
	}
	
	line_count = string_len / DASH;
	if ((string_len % TEXT_WIDTH)!=0){
		line_count++;
	}
	
	
	if (skip_move == 0){
		int additional_lines = y - (line_count + 1 + 1);
		move(additional_lines/2, 0);
	}
	dash_line(DASH);
	int ptr = 0;
	int temp = 0;
	for (int i = 0; i < line_count; i++){
		if (i != 0){
	
			printw("\n");		
		}
		temp = ptr;
		while (ptr < temp + DASH){
			if (string_len == ptr){
				break;
			}
			printw("%c", str[ptr]);
			
			ptr++;
		}
		
	}
	printw("\n");
	dash_line(DASH);
	
}

int number_entered(int min, int max, char *str, char *success, char *prompt){
	int entry;
restart:printf("");
	int x, y;
	getmaxyx(stdscr, y, x);
	
	dash_line(DASH);
	printw("%s", prompt);
	
	scanw("%d",&entry);
	refresh();
	if ((entry > max)||(entry < min)){
		attron(COLOR_PAIR(1));
		printw("%s", str);
		refresh();
		attroff(COLOR_PAIR(1));
		goto restart;
	}
	attron(COLOR_PAIR(2));
	printw("%s", success);
	
	refresh();

	attroff(COLOR_PAIR(2));
	//dash_line(DASH);
	printw(">> Press any Key to continue\n");
	getch();
	clear();
	return entry;
}
void start_new_game(){
	//dash_line(DASH);
	char line[17];
	int i;
	num_of_players = number_entered(2,8, "\nThis is an invalid amount of players, please enter a value between 2 - 8\n\n", "\nValid number entered\n\n", ">> Enter the number of players that would be participating: ");
	
	int loop = 0;

	while (loop < num_of_players){
restart:	dash_line(DASH);
		char real_name[17];
		int i = 0;
		printw("Please enter player %d's name: ", loop+1);
		refresh();
		noecho();
		while (i < 15){
			real_name[i] = getch();
			if (real_name[i] == ENTER){
				break;
			}
			if (real_name[i] == BACKSPACE){
				real_name[i] = '\0';
				if (i >= 1){
					real_name[i-1] = '\0';
					i--;
				}
				i--;
				
				
			}
		
			i++;
		}
		real_name[i] = '\0';
		if (i == 0){
			attron(COLOR_PAIR(1));
			printw("\nUsername is invalid, please enter a username with atleast one character\n");
			attroff(COLOR_PAIR(1));
			goto restart;
		}
		for (int i = 0; i < player_id - 1; i++){
			if (strcmp(real_name, players[i].name) == 0){
				attron(COLOR_PAIR(1));
				printw("\nUsername has already been taken, please pick another\n");
				attroff(COLOR_PAIR(1));
				goto restart;
			}
		}

			

		
		initialize_player(real_name, player_id ,0, INITIAL_CASH, 0, 0, &players[loop]);
		
		attron(COLOR_PAIR(2));
		printw("\nWelcome, %s, you are player %d. Press any key to continue\n", players[loop].name, players[loop].player_id);
		attroff(COLOR_PAIR(2));
		getch();
		
		refresh();
		player_id++;
		loop++;
	}
	dash_line(DASH);

	

}

void main_menu(char **menu, int height, int width){
	highlight = 1;
	printw("Select an option\n");

	int start_x_win = 2;
	int start_y_win = 2;
	int y = (LINES - height) / 2;
	int x = (COLS -width) / 2;
	WINDOW *local_win;
	local_win = newwin(height, width, y, x);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 	*/
	if (highlight == 1){
		wattron(local_win,A_REVERSE);
		mvwprintw(local_win,start_y_win, start_x_win, menu[0]);
		wattroff(local_win,A_REVERSE);
		wrefresh(local_win);
	}
	int count = 0;
	for (int i = 1; menu[i] != NULL; i++){
		mvwprintw(local_win,start_y_win + i, start_x_win, menu[i]);
		wrefresh(local_win);
		refresh();
		count++;
	}
	count++;
	keypad(local_win, TRUE);
	char ch;
	refresh();
	noecho();
	while ((ch = wgetch(local_win)) != ENTER){
		
		if ((highlight == 1)&&(ch == UP)){
			highlight = count;
		}
		else if ((highlight == count)&&(ch == DOWN)){
			highlight = 1;
		}
		else if (ch == DOWN){
			highlight++;
		}
		else if (ch == UP){
			highlight--;

		}	
		refresh();
	
		if ((ch == UP)||(ch == DOWN)){
			for (int i = 1; i < count+1; i++){
				if (highlight == i){
					wattron(local_win,A_REVERSE);
					mvwprintw(local_win,start_y_win+(i - 1), start_x_win, menu[i-1]);
					wattroff(local_win,A_REVERSE);
					wrefresh(local_win);
					refresh();
				}
				else{
					
					mvwprintw(local_win,start_y_win + (i - 1), start_x_win, menu[i-1]);
					wrefresh(local_win);
					refresh();
				}
			}
		}
		
			
			
	}
	destroy_win(local_win);
	echo();
} 

int main(){
	
	printf("starting up application\n\n");
	refresh();
	initscr();
	int x,y;
	getmaxyx(stdscr, y, x);
	DASH = x;
	dash_line(DASH);
	if (has_colors() == FALSE){
		endwin();
		printf("\nThis system has no colors\n");
		return 1;
	}
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	attron(A_BOLD);
	//attron(COLOR_PAIR(1));
	printw("Welcome to Monopoly!!!\n");
	refresh();
	attroff(A_BOLD);	
	//attroff(COLOR_PAIR(1));
	dash_line(DASH);
	printw("press any button to continue\n");
	getch();
	clear();

	
	print("The rules of the game are simple. Firstly, player usernames that include spaces are truncated. Usernames can only be 15 characters long. Only 2 - 8 players can play at a time. This game can be saved and be resumed at a later date. There is no autosave, save must be done by selecting the save option while playing", y, 0);

	refresh();
	mvprintw(0,0,"Press any button to continue\n");	
	refresh();
	getch();
	clear();
	refresh();
	char **main = malloc(sizeof(char *) * 3);
	if (main == NULL){
		endwin();
		printf("No space in memory\n");
	}
	main[0] = "Start new game";
	main[1] = "Load saved game";
	main[2] = NULL;
	
	main_menu(main, 6, 30);
	clear();
	main[3] = NULL;
	if (highlight == 1){
		start_new_game();
	}
	if (highlight == 2){
retry:		refresh();
		char empty[3];
		char **slots = malloc(sizeof(char *) * 4);
		if (slots == NULL){
			endwin();
			printf("No space in memory\n");
		}
		if (is_empty_save_slot(save_file_one) == 1){
			slots[0] = "EMPTY SLOT";
			empty[0] = 1;
		}
		else{
			slots[0] = "SAVE_SLOT_ONE";
			empty[0] = 0;
		}
		if (is_empty_save_slot(save_file_two) == 1){
			slots[1] = "EMPTY SLOT";
			empty[1] = 1;
		}
		else{
			slots[1] = "SAVE_SLOT_TWO";
			empty[1] = 0;
		}
		if (is_empty_save_slot(save_file_three) == 1){
			slots[2] = "EMPTY SLOT";
			empty[2] = 1;
		}
		else{
			slots[2] = "SAVE_SLOT_THREE";
			empty[2] = 0;
		}
		
		main_menu(slots, 6, 20);
		free(slots);
		clear();
		if (empty[highlight - 1] == 1){
			dash_line(DASH);
			mvprintw(0, 0, ">> The current slot is empty\n");
			printw(">> Would you like to select another slot or start a new game?\n");
			int retry = number_entered(0, 1, "Number entered is invalid, please enter either 1 or 0\n", "Correct option entered\n", ">> Input 1 to select another slot and 0 to start a new game: ");
			if (retry == 1){
				goto retry;
			}
			else{
				start_new_game();
			}		 
		} 
		
		
	}
	free(main);
	clear();
	refresh();
	startup_board();

	endwin();
	printf("Application closed successfully\n");
	return 0;
}
