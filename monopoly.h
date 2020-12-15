#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <curses.h>

#define ENTER 10
#define UP 3
#define DOWN 2
#define LEFT 4
#define RIGHT 5
#define BACKSPACE 127
#define TEXT_WIDTH 50
#define INITIAL_CASH 2000
#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"
struct player{
	char *name;
	int player_id;
	int current_position;
	float money;
	int in_jail;
	int time_spent_in_jail;

}typedef player_t;

static int num_of_players;	//variable to store total number of players

static int player_id = 1;	//the first player id number

static player_t players[8];	//array that stores the array of players.

static int DASH = 180;

static int highlight = 1;

static char *save_file_one = "save_file_one.txt";

static char *save_file_two = "save_file_two.txt";

static char *save_file_three = "save_file_three.txt";

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
				 "FENCHURCH ST. STATION"
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
				 "SUPER TAX",
				 "MAYFAIR"};
static int mortgage[40] = {0};
static int ownership[40] = {0};
//if color is brown, color is 1
//if color is sky blue, color is 2
//if color is Pink, color is 3
//if color is orange, color is 4,
//if color is red, color is 5
//if color is yellow, color is 6
//if color is green, color is 7
//if color is Dark blue, color is 8
//if property is airport, color is 9
//if property has no clue, color is 10
static int colors[40] = {0, 1, 0, 1, 0, 9, 2, 0, 2, 2, 0, 3, 0, 3, 3, 9, 4, 0, 4, 4, 0, 5, 0, 5, 5, 9, 6, 6, 0, 6, 0, 7, 7, 0, 7, 9, 0, 8, 0, 8};

static int prices[40] = {0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320,200, 0, 350, 0, 400}; 

static int rent_no_house[40] = {0, 2, 0, 4, 0, 0, 6, 0, 6, 8, 0, 10, 0, 10, 12, 0,14, 0,14, 16, 0, 18, 0, 18, 20, 0, 22, 22, 0, 22, 0, 26, 26, 0, 28, 0, 0, 35, 0, 50};

static int rent_one_house[40] = {0, 10, 0, 20, 0, 25, 30, 0, 30, 40, 0, 50, 0, 50, 60, 25,70, 0,70, 80, 0, 90, 0, 90, 100, 25, 110, 110, 0, 120, 0, 130, 130, 0, 150, 25, 0, 175, 0, 200};

static int rent_two_house[40] = {0, 30, 0, 60, 0, 50, 90, 0, 90, 100, 0, 150, 0, 150, 180, 50,200, 0,200, 220, 0, 250, 0, 250, 300, 50, 330, 330, 0, 360, 0, 390, 390, 0, 450, 50, 0, 500, 0, 600};

static int rent_three_house[40] = {0, 90, 0, 180, 0, 100, 270, 0, 270, 300, 0, 450, 0, 450, 500, 100,550, 0,550, 600, 0, 700, 0, 700, 750, 100, 800, 800, 0, 850, 0, 900, 900, 0,1000, 100, 0,1100, 0, 1400};

static int rent_four_house[40] = {0, 160, 0, 320, 0, 200, 400, 0, 400, 450, 0, 625, 0, 625, 700, 200,750, 0,750, 800, 0, 875, 0, 875, 925, 200, 975, 975, 0, 1025, 0, 1100, 1100, 0, 1200, 200, 0, 1300, 0, 1700};

static int rent_hotel[40] = {0, 250, 0, 450, 0, 0, 550, 0, 550, 600, 0, 750, 0, 750, 900, 0,950, 0,950,1000, 0, 1050, 0, 1050, 1100,0 , 1150, 1150, 0, 1200, 0, 1275, 1275, 0, 1400, 0, 0, 1500, 0, 2000};

static int mortgage_value[40] = {0};

void main_menu(char **menu, int height, int width);
