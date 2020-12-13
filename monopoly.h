#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DASH 60
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
