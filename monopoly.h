#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <curses.h>
#include <errno.h>
#include <time.h>


#define CTRL_S 19
#define EXIT 27
#define ENTER 10
#define UP 3
#define DOWN 2
#define LEFT 4
#define RIGHT 5
#define BACKSPACE 127
#define TEXT_WIDTH 50
#define INITIAL_CASH 1500
#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"
#define BANK 0
#define ALL 9

#define print_w
#ifdef print_f
#define print_form printf
#endif
#ifdef print_w
#define print_form printw
#endif

struct player{
	char name[6];
	int player_id;
	int current_position;
	float money;
	int in_jail;
	int time_spent_in_jail;
	int GOOJFC;

}typedef player_t;

static int SPEED = 1000;

static int curr_player = 0;

static int num_of_players;	//variable to store total number of players

static int player_id = 1;	//the first player id number

static player_t players[8];	//array that stores the array of players.

static int DASH = 180;

static int highlight = 1;

static char save_one[16] = {};

static char save_two[16] = {};

static char save_three[16] = {};

static char *save_file_one = "save_file_one.txt";

static char *save_file_two = "save_file_two.txt";

static char *save_file_three = "save_file_three.txt";

static char *save_file_save_name = "save_names.txt";

static char *property_names[] = {"GO",
				 "OLD KENT ROAD",
				 "COMMUNITY CHEST",
				 "WHITECHAPEL ROAD",
				 "INCOME TAX",
				 "KINGS CROSS STATION",
				 "THE ANGEL, ISLINGTON",
				 "CHANCE",
				 "EUSTON",
				 "PENTONVILLE",
				 "JUST VISITING JAIL",
				 "PALL MALL",
				 "ELECTRIC COMPANY",
				 "WHITEHALL",
				 "NORTHMURL'D AVENUE", 
				 "MARYLBONE STATION",
				 "BOW STREET",
				 "COMMUNITY CHEST",
				 "MARLBOROUGH STREET",
				 "VINE STREET",
				 "FREE PARKING",
				 "STRAND", 
				 "CHANCE",
				 "FLEET STREET",
				 "TRAFALGAR SQUARE",
				 "FENCHURCH ST. STATION",
				 "LEICESTER SQUARE",
				 "CONVENTRY STREET",
				 "WATER WORKS",
				 "PICCADILLY",
				 "GO TO JAIL",
				 "REGENT STREET",
				 "OXFORD STREET",
				 "COMMUNITY CHEST",
			         "BOND STREET",
				 "LIVERPOOL ST. STATION",
				 "CHANCE",
				 "PARK LANE",
				 "LUXURY TAX",
				 "MAYFAIR",
				  NULL};

char *community_chest[] = {"Advance to \"Go\". (Collect $200)",
			   "Bank error in your favor. Collect $200.",
			   "Doctor's fees. {fee} Pay $50.",
			   "From sale of stock you get $50.",
			   "Get Out of Jail Free.",
			   "Go to Jail.",
			   "Grand Opera Night. Collect $50 from every player for opening night seats.",
			   "Holiday {Xmas} Fund matures. Receive {Collect} $100.",
			   "Income tax refund. Collect $20.",
			   "It is your birthday. Collect $10 from every player.",
			   "Life insurance matures – Collect $100",
			   "Hospital Fees. Pay $50.",
			   "School fees. Pay $50.",
			   "Receive $25 consultancy fee.",
			   "You are assessed for street repairs: Pay $40 per house and $115 per hotel you own.",
			   "You have won second prize in a beauty contest. Collect $10.",	
			   "You inherit $100.",
			   NULL};

char *chance[] = {"Advance to \"Go\". (Collect $200)",
		  "Advance to Trafalgar square.",
		  "Advance to Pall Mall. If you pass Go, collect $200.",
		  "Advance token to nearest Utility. If unowned, you may buy it from the Bank. If owned, throw dice and pay owner a total 10 (ten) times the amount thrown.",
		  "Advance token to the nearest station and pay owner twice the rental to which he/she {he} is otherwise entitled. If Railroad is unowned, you may buy it from the Bank.",
		  "Bank pays you dividend of $50.",
		  "Get out of Jail Free.","Go Back Three {3} Spaces.",
		  "Go to Jail. Go directly to Jail.",
		  "Make general repairs on all your property: For each house pay $25, For each hotel {pay} $100.",
		  "Pay poor tax of $15",
		  "Take a ride to King’s Cross Station. If you pass Go, collect $200.",
		  "Take a walk on the Mayfair. Advance token to Mayfair.",
		  "You have been elected Chairman of the Board. Pay each player $50.",
		  "Your building {and} loan matures. Receive {Collect} $150.",
		  "You have won a crossword competition. Collect $100.",
		  NULL};

static int mortgage[40] = {0};
static int ownership[40] = {0};
static int houses[40] = {0};
//if color is brown, color is 1
//if color is sky blue, color is 2
//if color is Pink, color is 3
//if color is orange, color is 4,
//if color is red, color is 5
//if color is yellow, color is 6
//if color is green, color is 7
//if color is Dark blue, color is 8
//if property is airport, color is 9
//if property is water_works/ electric company, color is 10
//if property has no clue, color is 0
static int colors[40] = {0, 1, 0, 1, 0, 9, 2, 0, 2, 2, 0, 3, 10, 3, 3, 9, 4, 0, 4, 4, 0, 5, 0, 5, 5, 9, 6, 6, 10, 6, 0, 7, 7, 0, 7, 9, 0, 8, 0, 8};
static int prices[40] = {0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320,200, 0, 350, 0, 400}; 
static int rent_no_house[40] = {0, 2, 0, 4, 0, 0, 6, 0, 6, 8, 0, 10, 0, 10, 12, 0,14, 0,14, 16, 0, 18, 0, 18, 20, 0, 22, 22, 0, 22, 0, 26, 26, 0, 28, 0, 0, 35, 0, 50};
static int rent_one_house[40] = {0, 10, 0, 20, 0, 25, 30, 0, 30, 40, 0, 50, 0, 50, 60, 25,70, 0,70, 80, 0, 90, 0, 90, 100, 25, 110, 110, 0, 120, 0, 130, 130, 0, 150, 25, 0, 175, 0, 200};
static int rent_two_house[40] = {0, 30, 0, 60, 0, 50, 90, 0, 90, 100, 0, 150, 0, 150, 180, 50,200, 0,200, 220, 0, 250, 0, 250, 300, 50, 330, 330, 0, 360, 0, 390, 390, 0, 450, 50, 0, 500, 0, 600};
static int rent_three_house[40] = {0, 90, 0, 180, 0, 100, 270, 0, 270, 300, 0, 450, 0, 450, 500, 100,550, 0,550, 600, 0, 700, 0, 700, 750, 100, 800, 800, 0, 850, 0, 900, 900, 0,1000, 100, 0,1100, 0, 1400};
static int rent_four_house[40] = {0, 160, 0, 320, 0, 200, 400, 0, 400, 450, 0, 625, 0, 625, 700, 200,750, 0,750, 800, 0, 875, 0, 875, 925, 200, 975, 975, 0, 1025, 0, 1100, 1100, 0, 1200, 200, 0, 1300, 0, 1700};
static int rent_hotel[40] = {0, 250, 0, 450, 0, 0, 550, 0, 550, 600, 0, 750, 0, 750, 900, 0,950, 0,950,1000, 0, 1050, 0, 1050, 1100,0 , 1150, 1150, 0, 1200, 0, 1275, 1275, 0, 1400, 0, 0, 1500, 0, 2000};
static int mortgage_value[40] = {0};

/*function declarations*/

/*This function copies the entered save_name and sends them to the actual save names*/
void save_name(char real_name[], int highlight);

/*This function gets a string from the user*/
int string_retrieval_user(char real_name[], int size);

/*This function converts the color number into a descriptive string*/
char* get_color(int number);

/*This function prompts the user to pick a slot and then either saves or loads based on the load_save variable*/
void menu_save(int load_save);

/*This saves to the file*/
int save_file(char *file_name);

/*This loads from the file*/
int load_file(char *filename);

/*This basically checks to see if saving is being called from esc or just a save*/
int saving(int on_exit);

/*Prints a decriptive string of the values in the mortgaged array*/
char *is_mortgaged(int number);

/*Returns a name string of a property*/
char *owner(int number);

/*This is a function that checks to see if the save file is empty or not*/
int is_empty_save_slot(char *filename);

/*This function deletes windows*/
void destroy_win(WINDOW *local_win);

/*This function prints number amount of dashes in the Window, it is a decorative print*/
void dash_line(int number);

/*initializes a player struct*/
void initialize_player(char name[], int player_id, int curr_pos, float money, int 
jail, int time_in_jail, int GOOJFC, player_t *player);

/*Prints the monopoly board, still in progress*/
void startup_board(int play, int begin, int dice);

/*print out in a regulated character per line manner*/
void print(char *str, int y, int skip_move);

/*gets numeric input from user*/
int number_entered(int min, int max, char *str, char *success, char *prompt);

/*This starts a new game*/
void start_new_game();

/*Prints the interactive menu system*/
void main_menu(char **menu, int height, int width);

/*This is a function that checks to see if two names are the same*/
int namecmp(char src[], char dest[]);

/*This is a function that prompts the user to choose whatever they would like to and when they are done/exit, the properties picked are sent to back*/
int current_players_assets(int index, char *array[], int picked[], int *ret,float *money, int *card);

/*This is a function for that implements the trade functionality*/
	/*Needs the current_players_assets function*/
void trade_with_player(int index);

/*Declare bankruptcy*/
void declare_bankruptcy(int index);

/*Mortgaged properties implementation*/
void mortgage_property(int index, int mortgage_var);

/*Sell houses implementation*/
void sell_houses(int index);

/*Buy homes implementation*/
void buy_houses(int index);

/*This function implements the auction functionality*/
void auction(int index, int property_index);

/*This function prints out properties based on color.*/
void print_props(int size, char color[], int id, char *str);

/*This function implements the print player info functionality*/
	/*This function uses the print_props function*/
void print_player_info(int index);

/*This is one of the most important function. it handles everything that has to do with players money*/
void handle_payment(int action, int index, int pay_to_or_pay_from, int price, int who, int property_index);

/*checks to see if a property has a monopoly*/
int monopoly(int pos);

/*This checks to see the expected price and who the pay is supposed to go to based on some factors*/
void property(int dice, int pos, int index, int *pay_to_or_pay_from, int *price, int *who);

/*This handles more than GO. it handles most of the movement of each player*/
void handle_GO(int index, int dice, int *pay_to_or_pay_from, int *price, int *who);

/*This finds the closest property a, b, c, or d from pos*/
int is_closest(int pos, int a, int b, int c, int d);

/*Handles community chest*/
void handle_community_chest(int index, int dice, int *pay_to_or_pay_from, int *price, int *who);

/*Handles chance*/
void handle_chance(int index, int dice, int *pay_to_or_pay_from, int *price, int *who);

/*This generates a random number for the first and second dice*/
/*BE SURE TO USE SRAND(time(NULL)) BEFORE USING THIS FUNCTION*/
void roll_dice(int *dice_one, int *dice_two);

/*this handles the actual movement, and the behaviour of each box*/
void movement(int index, int amt_moved, int *pay_to_or_pay_from, int *price, int *who);

/*This basically checks for two major things before calling movement*/
	/*Checks to see if the player is in jail, hence behaviour should be a diff*/
	/*checks to see if the player rolls three times and puts them in jail*/
void rolling_conditions(int index, int *a, int *b, int *pay_to_or_pay_from, int *price, int *who);
