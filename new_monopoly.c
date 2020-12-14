#include "monopoly.h"
/*This is a function that checks to see if the save file is empty or not*/
int is_empty_save_slot(char *filename){
	FILE *fp = fopen(filename, "r");
	//if file does not exist, the save file does not exist
	if (fp == NULL){
		return 1;
	}
	//if the size of the file is 0, save file contains nothing
	fseek(fp, 0l, SEEK_END);
	int valid = ftell(fp);
	if (valid == 0){
		return 1;
	}
	//file exist
	fclose(fp);
	return 0;
}
/*This function deletes windows*/
void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}
/*This function prints number amount of dashes in the Window, it is a decorative print*/
void dash_line(int number){
	
	for (int i = 0; i < number; i++){
		printw("=");
	}

}
/*initializes a player struct*/
void initialize_player(char name[], int player_id, int curr_pos, float money, int jail, int time_in_jail, player_t *player){
	player->name = malloc(16);
	memcpy(player->name, name, 16);	
	player->player_id = player_id;
	player->current_position = curr_pos;
	player->money = money;
	player->in_jail = jail;
	player->time_spent_in_jail=time_in_jail;
}
/*Prints the monopoly board, still in progress*/
void startup_board(){
	int height;
	int width;
	//get the max y coordinate and x coordinate
	getmaxyx(stdscr, height, width);
	
	int starty = (LINES - height)/2;
	int startx = (COLS - width)/2;
	int inner_height = (3 * height)/4;
	int inner_width = (3 * width)/4;
	int inner_starty = (LINES -inner_height)/2;
	int inner_startx = (COLS - inner_width)/2;
	WINDOW *outer;	//create a wide window(exterior window)
	WINDOW *inner;	//create an inner window(interior window)
	outer = newwin(height, width, starty, startx);
	inner = newwin(inner_height, inner_width, inner_starty, inner_startx);
	box(outer, 0, 0);	//create the box outline for the outer window
	box(inner, 0, 0);	//create the box outline for the inner window
	int inc = width / 10;	//increment to next x coordinate
	int next_x = inc;	//next x location
	//print vertical bars on the board
	for (int i = 0; i < 9;i++){
		//This prints each vertical line
		for (int j = 0; j < height; j++){
			//fixes the first vertical line
			if (i == 0){
			
				mvwaddch(outer,j, next_x+(width / 35), ACS_VLINE);
			}
			//fixes the last vertical line
			else if (i == 8){
				mvwaddch(outer,j, next_x-(width /35), ACS_VLINE);
			}
			//every other vertical line
			else{
			
				mvwaddch(outer,j, next_x, ACS_VLINE);
			}
		} 
		//move on to the next x coordinate
		next_x+=inc;
	} 
	//MAYBE REFACTOR??
	inc = height /10;	//increment to next y coordinate
	int next_y = inc;	//next y location
	//print horizontal bars on the board
	for (int i = 0; i < 9;i++){
		//This prints each horizontal line
		for (int j = 0; j < width; j++){
			//fixes the first horizontal line
			if (i == 0){
				mvwaddch(outer,next_y+(height / 35), j, ACS_HLINE);
			}
			//fixes the last horizontal line
			else if (i == 8){
				mvwaddch(outer, next_y-(height /20), j,ACS_HLINE);
			}
			//every other horizontal line
			else{
			
				mvwaddch(outer,next_y, j,ACS_HLINE);
			}
		} 
		//move on to the next y coordinate
		next_y+=inc;
	}
	//print numbers in each box
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
	//display the windows on the screen
	wrefresh(outer);
	wrefresh(inner);
	getch();
	destroy_win(outer);
	destroy_win(inner);
}
/*print out in a regulated character per line manner*/
void print(char *str, int y, int skip_move){
	int string_len = 0;
	int line_count = 0;
	for (int i = 0; str[i] != '\0'; i++){
		//count the number of characters in the sent in string
		string_len++;
	}
	
	line_count = string_len / DASH;
	if ((string_len % TEXT_WIDTH)!=0){
		//calculate the number of lines needed
		line_count++;
	}
	
	//if skip move is 0, then start print on window at current position, else jump
	//to the center of the window
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
/*gets numeric input from user*/
int number_entered(int min, int max, char *str, char *success, char *prompt){
	int entry;
restart:printf("");
	int x, y;
	getmaxyx(stdscr, y, x);
	//print prompt
	dash_line(DASH);
	printw("%s", prompt);
	//get input from the user
	scanw("%d",&entry);
	refresh();
	//if the number is numeric, re-prompt
	if ((entry > max)||(entry < min)){
		//change color to red
		attron(COLOR_PAIR(1));
		//print failure message
		printw("%s", str);
		refresh();
		//back to default color
		attroff(COLOR_PAIR(1));
		goto restart;
	}
	//change color to green
	attron(COLOR_PAIR(2));
	printw("%s", success);
	
	refresh();
	//back to defaul color
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
	//get numeric value from user
	num_of_players = number_entered(2,8, "\nThis is an invalid amount of players, please enter a value between 2 - 8\n\n", "\nValid number entered\n\n", ">> Enter the number of players that would be participating: ");
	
	int loop = 0;
	//get the users name and create structs for each player
	while (loop < num_of_players){
restart:	dash_line(DASH);
		char real_name[17];
		int i = 0;
		printw("Please enter player %d's name: ", loop+1);
		refresh();
		noecho();	//hide user input to abstract backspace error
		while (i < 15){
			real_name[i] = getch();
			//if enter, the user is done entering strings
			if (real_name[i] == ENTER){
				break;
			}
			//if backspace, the user is trying to remove previous character, something that is not achievable with getch(), so i abstracted backspace oper
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
			//if the user entered nothing
			attron(COLOR_PAIR(1));
			printw("\nUsername is invalid, please enter a username with atleast one character\n");
			attroff(COLOR_PAIR(1));
			goto restart;
		}
		//check to see if the user name entered has already been entered
		for (int i = 0; i < player_id - 1; i++){
			if (strcmp(real_name, players[i].name) == 0){
				attron(COLOR_PAIR(1));
				printw("\nUsername has already been taken, please pick another\n");
				attroff(COLOR_PAIR(1));
				goto restart;
			}
		}

			

		//init each player struct
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
/*Prints the interactive menu system*/
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
	//this should always be true because highlight is basically set at the start
	if (highlight == 1){
		//print the first value in the array with the highlight
		wattron(local_win,A_REVERSE);
		mvwprintw(local_win,start_y_win, start_x_win, menu[0]);
		wattroff(local_win,A_REVERSE);
		wrefresh(local_win);
	}
	//print the rest of the array without highlights
	int count = 0;
	for (int i = 1; menu[i] != NULL; i++){
		mvwprintw(local_win,start_y_win + i, start_x_win, menu[i]);
		wrefresh(local_win);
		refresh();
		count++;
	}
	//get the size of the array
	count++;
	keypad(local_win, TRUE);
	char ch;
	refresh();
	noecho();
	//continue until the user selects an option
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
	initscr();	//start up new window
	int x,y;
	getmaxyx(stdscr, y, x);	//get the max y and x
	DASH = x;		//make sure the dash prints to the max x
	dash_line(DASH);
	//if system has no colors 
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
	//if the user pressed enter while highlight was 1, start new game
	if (highlight == 1){
		start_new_game();
	}
	
	if (highlight == 2){
retry:		refresh();
		char empty[3];
		char **slots = malloc(sizeof(char *) * 4);
		//if ENOSPC
		if (slots == NULL){
			endwin();
			printf("No space in memory\n");
		}
		//check to see if slot 1 has any save data
		if (is_empty_save_slot(save_file_one) == 1){
			slots[0] = "EMPTY SLOT";
			empty[0] = 1;
		}
		else{
			slots[0] = "SAVE_SLOT_ONE";
			empty[0] = 0;
		}
		//check to see if slot 2 has any save data
		if (is_empty_save_slot(save_file_two) == 1){
			slots[1] = "EMPTY SLOT";
			empty[1] = 1;
		}
		else{
			slots[1] = "SAVE_SLOT_TWO";
			empty[1] = 0;
		}
		//check to see if slot 3 has any save data
		if (is_empty_save_slot(save_file_three) == 1){
			slots[2] = "EMPTY SLOT";
			empty[2] = 1;
		}
		else{
			slots[2] = "SAVE_SLOT_THREE";
			empty[2] = 0;
		}
		//start up the menu 
		main_menu(slots, 6, 20);
		free(slots);
		clear();
		//if the entered slot is empty
		if (empty[highlight - 1] == 1){
			dash_line(DASH);
			mvprintw(0, 0, ">> The current slot is empty\n");
			printw(">> Would you like to select another slot or start a new game?\n");
			//inform the user that it is empty and ask for the next step
			int retry = number_entered(0, 1, "Number entered is invalid, please enter either 1 or 0\n", "Correct option entered\n", ">> Input 1 to select another slot and 0 to start a new game: ");
			if (retry == 1){
				//back to the load pick, this is because other slots may have data
				goto retry;
			}
			else{
				//if the user decides to start a new game
				start_new_game();
			}		 
		} 
		
		
	}
	free(main);
	clear();
	refresh();
	//draw up board, working on this function atm
	startup_board();

	endwin();
	printf("Application closed successfully\n");
	return 0;
}
