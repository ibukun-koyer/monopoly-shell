#include "new_monopoly.c"
/*This is a function that checks to see if two names are the same*/
int namecmp(char src[], char dest[]){
	int i = 0;
	while (i < 6){
		if ((src[i] == '\0')&&(dest[i] == '\0')){
			return 0;
		}
		if (src[i] != dest[i]){
			return 1;
		}
		i++;
	}
	return 0;
}
/*This is a function that prompts the user to choose whatever they would like to and when they are done/exit, the properties picked are sent to back*/
int current_players_assets(int index, char *array[], int picked[], int *ret,float *money, int *card){
	clear();
	refresh();
	print_form("This are your assets, %s\n", players[index].name);
	
	//properties w/o houses
		//GOOJFC
		//money
	*card = 0;
	*money = 0;
	
	int k = 0;
	int m = 0;
	int n = 0;
	while (k < 40){
		if ((ownership[k] == players[index].player_id)&&(houses[k] == 0)){
			array[m] = property_names[k];
			m++;
		}
		k++;
	}
	n = m; 		//now n contains the number of properties
	
	if (n > 0){
		memset(picked, 0, sizeof(int) * n);
	}
	if (players[index].GOOJFC > 0){
		array[m] = "GET OUT OF JAIL CARD";
		m++;
	}
	array[m] = "MONEY";
	m++;
	array[m] = "DONE";
	m++;
	array[m] = "EXIT";
	m++;
	array[m] = NULL;
redo:	main_menu(array, 20, 35);
	//if the highlight points to a property
	if ((n > 0) && (highlight <= n)){
		if (picked[highlight - 1] == 0){
			picked[highlight - 1] = 1;
			clear();
			refresh();
			print_form("You have added %s to the cart\n", array[highlight - 1]);
			print_form("Press any key to continue\n");
			getch();
			refresh();
			clear();
			refresh();
			goto redo;			
		}
		else{
			picked[highlight - 1] = 0;
			clear();
			refresh();
			print_form("You have removed %s from the cart\n", array[highlight - 1]);
			print_form("Press any key to continue\n");
			getch();
			refresh();
			clear();
			refresh();
			goto redo;
		}
	}
	//if the highlight points to GOOJFC
	else if ((n + 1 == highlight)&&(players[index].GOOJFC > 0)){
		clear();
		refresh();
		*card = number_entered(0, players[index].GOOJFC, "Number entered is invalid\n", "Correct option entered\n", "Please enter the amount of Get out of jail free cards you want: ");
		clear();
		refresh();
		print_form("You have added %d Get out of jail cards to the cart\n", *card);
		print_form("Press any key to continue\n");
		getch();
		refresh();
		clear();
		refresh();
		goto redo;
	}
	//if the user does not have a get out of jail free card, then we start from n, else we start from n+1. used p because n must remain constant until return after n as been set
	int p = n;

	if (players[index].GOOJFC > 0){
		p = n+1;
	}
	//if highlight points to money
	else if (p + 1 == highlight){
		clear();
		refresh();
		*money = number_entered(0, players[index].money, "Number entered is invalid\n", "Correct option entered\n", "Please enter the amount of money you want: ");
		clear();
		refresh();
		print_form("You have added $%f cash to the cart\n", *money);
		print_form("Press any key to continue\n");
		getch();
		refresh();
		clear();
		refresh();
		goto redo;
	}
	//if highligh points to done
	else if (p + 2 == highlight){
		clear();
		*ret = n;
		return 1;		//done
	}
	//if highlight points to exit
	else if (p + 3 == highlight){
		clear();
		return 0;		//exit
	}	
	return 0;
			
	
	
}
/*This is a function for that implements the trade functionality*/
	/*Needs the current_players_assets function*/
void trade_with_player(int index){
	clear();
	int y, x;
	getmaxyx(stdscr, y, x);
	mvprintw(0, (x - 30)/2, "Welcome to the Trading section\n");
	refresh();
	print_form(">> Who would you like to trade with?\n");
	char *opt[8];
	int a = 0;
	int i = 0;
	//This gets all the players name excluding the current player
	while (a < num_of_players){
		if (a != index){
			opt[i] = players[a].name;
			i++;
		}
		a++;
	}
	opt[i] = "EXIT";
	i++;
	opt[i] = NULL;
	if (i != 0){	
		main_menu(opt, 35, 30);
	}
	/*Very unlikely*/
	if (i == 0){
		printw("There are no other players\n");
		getch();
		return;
	}
	//if exit
	if (highlight - 1 == i - 1){
		return;
	}
	int j = 0;
	//for the asker
	char *options_give[40];
	int picked_give[40];
	int size_picked_give;
	float money_give;
	int card_give = 0;
	//for the offered
	char *options_take[40];
	int picked_take[40];
	int size_picked_take;
	float money_take;
	int card_take = 0;

	int who = 0;
	int temp = highlight;
	while (j < num_of_players){
		//if we have found the player we want to trade with
		if (namecmp(players[j].name,opt[temp - 1])==0){
			//player to be traded with found
			int ret = current_players_assets(index, options_give, picked_give, &size_picked_give,&money_give, &card_give);
			if (ret == 0){
				return;	//exit
			}
			ret = current_players_assets(j, options_take, picked_take, &size_picked_take,&money_take, &card_take);
			if (ret == 0){
				return;	//exit
			}
			who = j;	//be sure to get the players id
			
		
		}
		j++;
	}
	clear();
	refresh();

	getch();
	//print out the deal that the player has offered
	attron(A_UNDERLINE);
	print_form("The below shows the trade being initiated\n");
	attroff(A_UNDERLINE);
	print_form("Please press any key to continue\n");
	//this basically prints a vertical line in the middle to dimacate the two player deals
	for (int i =2; i < y; i++){
		mvaddch(i, x/2, ACS_VLINE);
	}
	//on left side
	mvprintw(2, 0, "What %s is offering %s", players[index].name, players[who].name);
	//on the right side
	mvprintw(2, (x+2)/2,"What %s wants from %s", players[index].name, players[who].name);
 	
	int begin = 4;
	int begin_2 = 4;
	//print the properties offered by each player
	for (int i = 0; i < 40; i++){
		//picked give and picked take are the size of the options_give and options_take respectively. Note even though they may be 40 spaces, only picked_give/picked_take values are used
		if ((picked_give[i] == 1)&&(i < size_picked_give)){		
			mvprintw(begin, 0, "Property --> %s",options_give[i]);
			begin++;
		}
		if ((picked_take[i] == 1)&&(i < size_picked_take)){		
			mvprintw(begin_2, (x+2)/2, "Property --> %s",options_take[i]);
			begin_2++;
		}
	}
	//print the money and the GOOJFC offered and expecting
	mvprintw(begin, 0, "Money --> $%f", money_give);
	mvprintw(begin_2, (x+2)/2,"Money --> $%f", money_take);	
	mvprintw(begin+1, 0, "Get out of jail free card --> %d", card_give);
	mvprintw(begin_2 + 1, (x+2)/2,"Get out of jail free card --> %d", card_take);
	getch();
	clear();
	refresh();
	print_form("Would you like to accept this offer, %s?\n", players[who].name);	 
	//accept or decline offer
	char * options[] = {"ACCEPT",
			"DECLINE",
			NULL};
	main_menu(options, 10, 25);
	if (highlight == 1){
		//handle the acceptance
		for (int i = 0; i < 40; i++){
			if ((picked_give[i] == 1)&&(i < size_picked_give)){		
				for (int j = 0; j < 40; j++){
					if (strcmp(property_names[j], options_give[i])==0){
						ownership[j] = players[who].player_id;
					}
				}				

			}
			if ((picked_take[i] == 1)&&(i < size_picked_take)){				
				for (int j = 0; j < 40; j++){
					if (strcmp(property_names[j], options_take[i])==0){
						ownership[j] = players[index].player_id;
					}
				}				

			}
		}
		players[index].GOOJFC+=card_take;
		players[who].GOOJFC+=card_give;
		handle_payment(3, index, 1, money_take, players[who].player_id,-1);	
		handle_payment(3, who, 1, money_give, players[index].player_id,-1);
	}
	else if (highlight == 2){
		//decline
		return;
	}	
	return;
}
/*Declare bankruptcy*/
void declare_bankruptcy(int index){
	//remove all houses the player owned and also unmortgage and sell the properties to other players
	int temp = players[index].player_id;

	for (int i = 0; i < 40; i++){
		if (ownership[i] == temp){
			mortgage[i] = 0;
			houses[i]   = 0;
		}
		
	}
	//current player declared bankruptcy, remove him from the available players array
	for (int i = 0; i < num_of_players - 1;i++){
		if (i >= index){
			players[i] = players[i + 1];
		}
	}
	num_of_players-=1;
	clear();
	refresh();
	print_form("player %d declared bankruptcy, so we are auctioning off their property WITHOUT the houses they may have had.\nPress any key to continue\n", temp);
	getch();
	if (num_of_players == 1){
		return;
	}
	//auction off their property
	for (int i = 0; i < 40; i++){
		if (ownership[i] == temp){
			auction(0, i);
		}
	}
	
}
/*Mortgaged properties implementation*/
void mortgage_property(int index, int mortgage_var){
	//1 means mortgage and 0 means un-mortgage
	clear();
	refresh();
	print_form("Welcome to the Mortgaging section\n");
	refresh();	
	char *props[40];
	int j = 0;
	//print out only properties that are unmortgaged
	for (int i = 0; i < 40; i++){
		if (ownership[i] == players[index].player_id){
			if ((mortgage[i] != mortgage_var)&&(houses[i] == 0)){
				props[j] = property_names[i];
				j++;
			}
			
		}
	}
	props[j] = "EXIT";
	j++;
	props[j] = NULL;
	main_menu(props, 35, 30);
	
	if (highlight - 1 == j - 1){
		return;
	}
	
	for (int i = 0; i < 40; i++){
		if (strcmp(props[highlight - 1], property_names[i]) == 0){
			//if attempting to mortgage
			if (mortgage_var == 1){
				clear();
				refresh();
				handle_payment(3, index, 1, mortgage_value[i], BANK, -1);
				mortgage[i] = 1;
				print_form("Property has been mortgaged\n");
				print_form("Press any key to return");
				getch();
				refresh();
				return;
				
			}
			//if attempting to un-mortgage
			else if (mortgage_var == 0){
				int id = players[index].player_id;
				clear();
				refresh();
				handle_payment(3, index, 0, mortgage_value[i] + (10 * mortgage_value[i])/100, BANK, -1);
				if (id == players[index].player_id){
					mortgage[i] = 0;
					
					print_form("Property has been mortgaged\n");
					print_form("Press any key to return");
					getch();
					refresh();
				}
				return;
				
			}
		}
	}
	
}
/*Sell houses implementation*/
void sell_houses(int index){
	//index also starts from 0
	clear();
	refresh();
	//few notes to the player
	print_form("You have $%f in your wallet\n", players[index].money);
	print_form("This are the prices for bulding houses/hotels\n");
	print_form("1. BROWN AND SKY_BLUES COST $50\n");
	print_form("2. PINKS AND ORANGES COST $100\n");
	print_form("3. REDS AND YELLOWS COST $150\n");
	print_form("4. GREENS AND DARK_BLUES COST $200\n");
	print_form("Press any key to acknowledge\n");
	getch();
	clear();
	refresh();
	print_form(">> These are the properties that have monopolies\n");
	refresh();	
	char *props[40];
	int j = 0;
	//print out only properties that have monopolies
	for (int i = 0; i < 40; i++){
		if (ownership[i] == players[index].player_id){
			int ret = monopoly(i);
			if (ret == 1){
				props[j] = property_names[i];
				j++;
			}
		}
	}
	props[j] = "EXIT";
	j++;
	props[j] = NULL;
	main_menu(props, 35, 30);
	
	if (highlight - 1 == j - 1){
		return;
	}
	int id;
	for (int i = 0; i < 40; i++){
		if ((strcmp(props[highlight - 1], property_names[i]) == 0)&&(houses[i] > 0)){
			//this block of code here simply gets the price to be collected from the bank using their colors
			id = players[index].player_id;	
			if ((colors[i] == 1)||(colors[i] == 2)){
				handle_payment(3, index, 1, 25, BANK, -1);
			}
			if ((colors[i] == 3)||(colors[i] == 4)){
				handle_payment(3, index, 1, 50, BANK, -1);
			}				
			if ((colors[i] == 5)||(colors[i] == 6)){
				handle_payment(3, index, 1, 75, BANK, -1);
			}
			if ((colors[i] == 7)||(colors[i] == 8)){
				handle_payment(3, index, 1, 100, BANK, -1);
			}
			if (id == players[index].player_id){
				houses[i]--;
			}
			
		}
		//if all houses have been sold
		else if ((strcmp(props[highlight - 1], property_names[i]) == 0)&&(houses[i] == 0)){
			clear();
			refresh();
			print_form("Cannot sell anymore houses. You do not own any houses on this property.\n");
			print_form("Press any key to continue\n");
			getch();	
			return;
		}	
		
	}
	
}
/*Buy homes implementation*/
void buy_houses(int index){
	//THIS FUNCTION IMPLEMENTATION IS EXTREMELY SIMILAR TO SELL HOUSES. WITH A FEW DIFFERENCE
		//stops building at 5
		//2x more expensive than selling
	//index also starts from 0
	clear();
	refresh();
	
	print_form("You have $%f in your wallet\n", players[index].money);
	print_form("PLEASE NOTE YOU HAVE TO HAVE ENOUGH MONEY TO PUCHASE A PROPERTY OR YOU WILL HAVE TO FIND A WAY TO GET SOME CASH\n");
	print_form("This are the prices for bulding houses/hotels\n");
	print_form("1. BROWN AND SKY_BLUES COST $50\n");
	print_form("2. PINKS AND ORANGES COST $100\n");
	print_form("3. REDS AND YELLOWS COST $150\n");
	print_form("4. GREENS AND DARK_BLUES COST $200\n");
	print_form("Press any key to acknowledge\n");
	getch();
	clear();
	refresh();
	print_form(">> These are the properties that have monopolies\n");
	refresh();	
	char *props[40];
	int j = 0;
	for (int i = 0; i < 40; i++){
		if (ownership[i] == players[index].player_id){
			int ret = monopoly(i);
			if (ret == 1){
				props[j] = property_names[i];
				j++;
			}
		}
	}
	props[j] = "EXIT";
	j++;
	props[j] = NULL;
	if (j != 0){	
		main_menu(props, 35, 30);
	}
	if (j == 0){
		printw(">> You have no monopolies so you cannot build. Press any button to continue\n");
		getch();
		return;
	}
	if (highlight - 1 == j - 1){
		return;
	}
	int id;
	for (int i = 0; i < 40; i++){
		if ((strcmp(props[highlight - 1], property_names[i]) == 0)&&(houses[i] < 5)){
			id = players[index].player_id;	
			if ((colors[i] == 1)||(colors[i] == 2)){
				handle_payment(1, index, 0, 50, BANK, -1);
			}
			if ((colors[i] == 3)||(colors[i] == 4)){
				handle_payment(1, index, 0, 100, BANK, -1);
			}				
			if ((colors[i] == 5)||(colors[i] == 6)){
				handle_payment(1, index, 0, 150, BANK, -1);
			}
			if ((colors[i] == 7)||(colors[i] == 8)){
				handle_payment(1, index, 0, 200, BANK, -1);
			}
			if (id == players[index].player_id){
				houses[i]++;
			}
			
		}
		else if ((strcmp(props[highlight - 1], property_names[i]) == 0)&&(houses[i] >= 5)){
			clear();
			refresh();
			print_form("Cannot build anymore houses. You already buit the maximum amount of houses that can be built on this property.\n");
			print_form("Press any key to continue\n");
			getch();	
			return;
		}	
		
	}
	
}
/*This function implements the auction functionality*/
void auction(int index, int property_index){
	clear();
	int y, x;
	getmaxyx(stdscr, y, x);
	int bid = 1;
	int fold_list[8] = {0};
	int winner = 0;
	int loop = index;
	//continues until someone wins the auction
	for (int i = index; TRUE; i++){
		clear();
		refresh();
		mvprintw(0, (x - 30)/2, "Welcome to the auctioning room\n");
		print_form("This is player %d\n", players[loop].player_id);
		print_form("The current property on bid is %s\n", property_names[property_index]);
		char *opt[] = {"Bid",
			     "Fold",
			     NULL};
		main_menu(opt, 7, 20);
		//if bid, be sure the user does not enter a less bid than the prev
		if (highlight == 1){
			clear();
			print_form("You own $%f\n",players[loop].money);
			print_form("Previous bid was $%d\n", bid); 
			bid = number_entered(bid + 1, players[loop].money, "Invalid bid, please enter a valid amount\n", "Correct option entered\n", "Please enter your bid: ");
		}
		//if fold, set the fold_list at index
		if (highlight == 2){
			fold_list[loop] = 1;
		}
		
		int track = 0;
		//if all players but one player remains, the remaining player wins auction
		for (int j = 0; j < num_of_players; j++){
			if (fold_list[j] == 1){
				track++;
			}
			if (fold_list[j] == 0){
				winner = j;
			}

		}
		if (track == num_of_players - 1){
			break;
		}
		loop++;
		loop = loop % num_of_players;
	}
	clear();
	print_form("Player %d is the winner of the auction\n", players[winner].player_id);
	print_form("Press any key to continue\n");
	getch();
	handle_payment(2, winner, 0, bid, BANK, property_index);
}
/*This function prints out properties based on color.*/
void print_props(int size, char color[], int id, char *str){
	int count = 0;
	//size is the number of properties with the color
	//color is an array that stores all the indexes for a particular color
	for (int i = 0; i < size; i++){
		
		int temp = color[i];
		//if the current player owns the current property
		if (ownership[temp] == id){
			if (mortgage[temp] == 1){
				attron(COLOR_PAIR(1));
			}
			print_form("%20s(%d)   ", property_names[temp], houses[temp]);
			if (mortgage[temp] == 1){
				attroff(COLOR_PAIR(1));
			}
			count++;
		}
	}
	if (count != 0){
		int y, x;
		getyx(stdscr, y, x);
		move(y, 100);
		print_form("-->%s", str);
		int ret = monopoly(color[0]);
		if ((ret == 1)||(ret == 5)||(ret == 7)){
			attron(COLOR_PAIR(2));
			print_form("%s", "-->Monopoly");
			attroff(COLOR_PAIR(2));
		}
		print_form("\n\n");
		
	}
	//print_form("\n");
	refresh();
}
/*This function implements the print player info functionality*/
	/*This function uses the print_props function*/
void print_player_info(int index){
	clear();
	int y, x;
	getmaxyx(stdscr, y, x);
	mvprintw(0, (x - 8)/2, "player %d\n", players[index].player_id);
	printw("-This players name is %s.\n", players[index].name);
	printw("-This player is currently on block %d, A.K.A %s.\n", players[index].current_position, property_names[players[index].current_position - 1]);
	printw("-This player has $%f.\n", players[index].money);
	if (players[index].in_jail == 1){
		printw("-This player is currently in jail.\n");
		printw("-This player has spent %d turns in jail.\n", players[index].time_spent_in_jail);
	}
	else{
		printw("-This player is NOT in jail.\n");
	}
	printw("-This player has %d get out of jail free cards.\n", players[index].GOOJFC);
	int previous = 0;
	refresh();
	printw("\n\n-The following lines shows the properties owned by this player, the number of houses each property has is showed inside the parenthesis. 5 houses means the property has an hotel.\n\n");
	char brown[2] = {1, 3};
	char airport[4] = {5, 15, 25, 35};
	char sky_blue[3] = {6, 8, 9};
	char pink[3] = {11, 13, 14};
	char utility[2] = {12, 28};
	char orange[3] = {16, 18, 19};
	char red[3] = {21, 23, 24};
	char yellow[3] = {26, 27, 29};
	char green[3] = {31, 32, 34};
	char blue[2] = {37, 39}; 
	print_props(2, brown, players[index].player_id, "BROWN");
	print_props(4, airport, players[index].player_id, "STATION");
	print_props(3, sky_blue, players[index].player_id, "SKY BLUE");
	print_props(3, pink, players[index].player_id, "PINK");
	print_props(2, utility, players[index].player_id, "UTILITIES");
	print_props(3, orange, players[index].player_id, "ORANGE");
	print_props(3, red, players[index].player_id, "RED");
	print_props(3, yellow,players[index].player_id, "YELLOW");
	print_props(3, green, players[index].player_id, "GREEN");
	print_props(2, blue,players[index].player_id, "BLUE");
	print_form("\nPress any key to continue");
	getch();
	
}
/*This is one of the most important function. it handles everything that has to do with players money*/
void handle_payment(int action, int index, int pay_to_or_pay_from, int price, int who, int property_index){
//if action is 1 - pay for rent
//if action is 2 - buy property from bank

//if pay_to_or_pay_from is 0, pay
//if pay_to_or_pay_from is 1, collect
//if pay_to_or_pay_from is 2, buy a property
	if(price == 0){	
		return;
	}
	clear();
	int y, x;
	getmaxyx(stdscr, y, x);
	mvprintw(0, (x - 30)/2, "Welcome to the payment method\n");
	refresh();
	char *sec;	
	if (who == BANK){
		sec = "BANK";
	}
	else if (who == ALL){
		sec = "ALL PLAYERS";
	}
	else{
		sec = players[who - 1].name;
	}
	if ((who != 0)&&(who != 9)){
		print_form("-This is player %d\n", players[index].player_id);
	}
	if (pay_to_or_pay_from == 0){
		printw("-You are about to make a payment of $%d to %s\n", price, sec);
	}
	if (pay_to_or_pay_from == 1){
		printw("-You are about to receive a payment of $%d from %s\n", price, sec);
	}
	if (pay_to_or_pay_from == 2){
		if (property_index < 0){
			printw("You are about to buy a property called %s from the BANK for $%d\n", property_names[players[index].current_position-1], price);
		}
		else{
			printw("You are about to buy a property called %s from the BANK for $%d\n", property_names[property_index], price);
		}
	}
	
	refresh();
	//if buyinh
	if (action == 2){
restart:	refresh();
		//options available
		if (property_index < 0){	
			char *options[] = {"Buy",
					   "Auction",
					   NULL};
			main_menu(options, 15, 50);
		}
		else{	
			char *options[] = {"Buy",
					   NULL};
			main_menu(options, 15, 50);
		}
		if (highlight == 1){
			if (players[index].money < price){
				clear();
				print_form("You do not have enough money to pay for this property, please select one of the following options\n");

				refresh();
				//if unable to buy
				char *option[] = {"Auction",
					     "Mortgage property",
					     "Sell houses/hotels",
					     "trade",
					     NULL};
				main_menu(option, 15, 50);
				if (highlight == 1){
					clear();
					refresh();
					auction(index, players[index].current_position - 1);
					return;
				}
				if (highlight == 2){
					clear();
					refresh();
					mortgage_property(index, 1);
					goto restart;
				}
				if (highlight == 3){
					clear();
					refresh();
					sell_houses(index);
					goto restart;
				}
				if (highlight == 4){
					clear();
					refresh();
					trade_with_player(index);
					goto restart;
				}
			}
			else{
				players[index].money-=price;
				clear();
				printw("You have successfully made the payment\n");
				printw("You now have $%f in your wallet\n", players[index].money);
				printw("Press any key to continue");
				if (property_index < 0){
					ownership[players[index].current_position - 1] = players[index].player_id;
				}
				else{
					ownership[property_index] = players[index].player_id;
				}
				refresh();
				getch();

			}
		}
		else if (highlight == 2){
			auction(index, players[index].current_position - 1);
			return;
		}
	}
	//if pay rent
	if (action == 1){
redo:		refresh();
		int amt = 0;
		if (who == ALL){
			amt = price * (num_of_players - 1);
		}
		else{
			amt = price;
		}
		//options available
		char *options[] = {"Pay rent",
				   "Mortgage",
				   "Sell houses/hotel",
				   "Trade",
				   "Declare bankruptcy",
				   NULL};
		main_menu(options, 15, 50);
		if (highlight == 1){
			clear();
			if (players[index].money >= amt){
				players[index].money-=amt;
				if ((who != ALL)&&(who != BANK)){
					players[who - 1].money+=amt;
					print_form("-you have successfully paid %s\n", players[who - 1].name);
					print_form("Press any key to continue\n");
					getch();
				}
				if (who == BANK){
					print_form("-you have successfully paid the bank\n");
					print_form("Press any key to continue\n");
					getch();
				} 
				if (who == ALL){
					for (int i = 0; i < num_of_players; i++){
						if (i != index){
							players[i].money+=price;
						}
					}
					print_form("-you have successfully paid all players\n");
					print_form("Press any key to continue\n");
					getch();
				} 
		
			}

			else{ 
				print_form("You do not have enough money to pay the rent, please select an option\n");
				goto redo;
			}	
		}
		if (highlight == 2){
			clear();
			refresh();
			mortgage_property(index, 1);
			goto redo;
		}
		if (highlight == 3){
			clear();
			refresh();
			sell_houses(index);
			goto redo;
		}
		if (highlight == 4){
			clear();
			refresh();
			trade_with_player(index);
			goto redo;
		}
		if (highlight == 5){
			clear();
			refresh();
			declare_bankruptcy(index);
			return;
		}
	}
	//if collect, recursive calls made here
	if (pay_to_or_pay_from == 1){
		//collecting
		if (who == ALL){
			for (int i = 0; i < num_of_players; i++){
				if (i != index){
					clear();
					print_form("Every player has to pay player %d $%d, press any key to continue\n", players[index].player_id, price);
					getch();
					handle_payment(1, i, 0, price, players[index].player_id, -1);
				}
			}
		}
		else if (who == BANK){
			players[index].money+=price;
			print_form("BANK has paid player %d $%d, press any key to continue\n",players[index].player_id, price);			
			getch();
		}
		else{
			print_form("Player %d has to pay player %d $%d, press any key to continue\n", who, players[index].player_id, price);
			getch();
			handle_payment(1, who - 1, 0, price, players[index].player_id, -1);
		}
	}

//print in pay and print price
//show menu

	//if the price is 0, ignore transaction
	//if rent, 
		//-pay person
			//-mortgage if not enough money
			//-sell houses option if not enough
			//-trade with player
			//-declare bankruptcy if unable to pay
	//if buy
		//-auction property
			//-mortgage houses
			//-sell houses		
			//-trade		
		//-pay for the property
			//-mortgage houses
			//-sell houses		
			//-trade		
	//if collect
		//deposit money into players account


}
/*checks to see if a property has a monopoly*/
int monopoly(int pos){
//if color is brown,color is 1
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
	if (colors[pos] == 1){
		if (ownership[1] == ownership[3]){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 2){
		if ((ownership[6] == ownership[8])&&(ownership[6] == ownership[9])){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 3){
		if ((ownership[11] == ownership[13])&&(ownership[11] == ownership[14])){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 4){
		if ((ownership[16] == ownership[18])&&(ownership[16] == ownership[19])){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 5){
		if ((ownership[21] == ownership[23])&&(ownership[21] == ownership[24])){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 6){
		if ((ownership[26] == ownership[27])&&(ownership[26] == ownership[29])){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 7){
		if ((ownership[31] == ownership[32])&&(ownership[31] == ownership[34])){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 8){
		if (ownership[37] == ownership[39]){
			return 1;
		}
		else{
			return 0;
		} 
	}
	else if (colors[pos] == 9){
		int val = 1;
		if (ownership[pos] == ownership[5]){
			val++;
		}
		if (ownership[pos] == ownership[15]){
			val++;
		}
		if (ownership[pos] == ownership[25]){
			val++;
		}
		if (ownership[pos] == ownership[35]){
			val++;
		}
		return val;
	}
	else if (colors[pos] == 10){
		int val = 5;
		if (ownership[pos] == ownership[12]){
			val++;
		}
		if (ownership[pos] == ownership[28]){
			val++;
		}
		return val;
	}
	return 0;
}
/*This checks to see the expected price and who the pay is supposed to go to based on some factors*/
void property(int dice, int pos, int index, int *pay_to_or_pay_from, int *price, int *who){
	int ME = 11;	//means player owns the current property
	//pos is assumed to be position - 1;
	//index is also assumed to be inde - 1;
	//2 means buy, pay_to_or_pay_from
	int id = players[index].player_id;
	if (ownership[pos] == 0){
		
		*pay_to_or_pay_from = 2;
		*price 		    = prices[pos];
		*who		    = BANK;
		return;
	}
	else if (ownership[pos] == id){
		*pay_to_or_pay_from = 0;
		*price 		    = 0;
		*who		    = ME;
		return;
	}
	else if (mortgage[pos] == 1){
		*pay_to_or_pay_from = 0;
		*price 		    = 0;
		*who		    = BANK;
		return;
	}
	else if (monopoly(pos) == 0){
		*pay_to_or_pay_from = 0;
		*price 		    = rent_no_house[pos];
		*who		    = ownership[pos];
		return;
	}
	else if (monopoly(pos) == 1){
		if (houses[pos] == 1){
			*pay_to_or_pay_from = 0;
			*price 		    = rent_one_house[pos];
			*who		    = ownership[pos];
		}
		else if (houses[pos] == 2){
			*pay_to_or_pay_from = 0;
			*price 		    = rent_two_house[pos];
			*who		    = ownership[pos];
		}
		else if (houses[pos] == 3){
			*pay_to_or_pay_from = 0;
			*price 		    = rent_three_house[pos];
			*who		    = ownership[pos];
		}
		else if (houses[pos] == 4){
			*pay_to_or_pay_from = 0;
			*price 		    = rent_four_house[pos];
			*who		    = ownership[pos];
		}
		else if (houses[pos] == 5){
			*pay_to_or_pay_from = 0;
			*price 		    = rent_hotel[pos];
			*who		    = ownership[pos];
		}
	
		return;
	}
	else if (monopoly(pos) == 2){
		*pay_to_or_pay_from = 0;
		*price 		    = rent_one_house[pos];
		*who		    = ownership[pos];
		return;
	}
	else if (monopoly(pos) == 3){
		*pay_to_or_pay_from = 0;
		*price 		    = rent_two_house[pos];
		*who		    = ownership[pos];
		return;
	}
	else if (monopoly(pos) == 4){
		*pay_to_or_pay_from = 0;
		*price 		    = rent_three_house[pos];
		*who		    = ownership[pos];
		return;
	}
	else if (monopoly(pos) == 5){
		*pay_to_or_pay_from = 0;
		*price 		    = rent_four_house[pos];
		*who		    = ownership[pos];
		return;
	}
	else if (monopoly(pos) == 6){
		*pay_to_or_pay_from = 0;
		*price 		    = 4 * dice;
		*who		    = ownership[pos];
		return;
	}
	else if (monopoly(pos) == 7){
		*pay_to_or_pay_from = 0;
		*price 		    = 10 * dice;
		*who		    = ownership[pos];
		return;
	}
		
	
	
}
/*This handles more than GO. it handles most of the movement of each player*/
void handle_GO(int index, int dice, int *pay_to_or_pay_from, int *price, int *who){
	int pos = players[index].current_position;
	players[index].current_position+=dice;
	if (players[index].current_position > 40){
		players[index].current_position = players[index].current_position - 40;
	}	
	//print_form("the current position is %d\n", players[index].current_position);
	if (pos > players[index].current_position){
		print_form("You have passed GO, Press any key to continue\n");
		getch();
		//passed go
		*pay_to_or_pay_from = 1;
		*price 		    = 200;
		*who		    = BANK;
	}
	//if player did not pass go
	else{
		*pay_to_or_pay_from = 0;
		*price 		    = 0;
		*who		    = BANK;
	}

}
/*This finds the closest property a, b, c, or d from pos*/
int is_closest(int pos, int a, int b, int c, int d){
	//pos is starting from 1
	if (pos < a){
		return a;
	}
	if (pos < b){
		return b;
	}
	if ((pos < c)&&(c > 0)){
		return c;
	}
	if ((pos < d)&&(d > 0)){
		return d;
	}
	return a;
}
/*Handles community chest*/
void handle_community_chest(int index, int dice, int *pay_to_or_pay_from, int *price, int *who){
	//pay_to_or_pay_from is 0 to pay to someone and 1 to collect
	//who is 0 to represent bank, player_id to rep player, 10 to rep all player
	//index also starts from 0
	//GO takes actual pos, not index
	srand(time(0));	
	clear();
	int chance_index = rand() % 17;
	
	//print_form("The chance_index is %d\n", chance_index);
	if (chance_index == 0){
		//advance to go
		print_form("%s\n", community_chest[chance_index]);
		int GO = 41 - players[index].current_position; 	
		print_form("Press any key to proceed");
		
		getch();	
		handle_GO(index, GO, pay_to_or_pay_from, price, who);
		

		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 1){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 200;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 2){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 3){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 4){
		print_form("%s\n", community_chest[chance_index]);
		players[index].GOOJFC++;
		print_form("Press any key to proceed");
		getch();
		handle_GO(index, 0, pay_to_or_pay_from, price, who);

		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;	
	}
	if (chance_index == 5){
		print_form("%s\n", community_chest[chance_index]);
		players[index].current_position = 11;
		players[index].in_jail		 = 1;
		print_form("Press any key to proceed");
		getch();
		handle_GO(index, 0, pay_to_or_pay_from, price, who);
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		//handle payment
		return;
	}
	if (chance_index == 6){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 50;
		*who		    = ALL;
		print_form("Press any key to proceed");
		getch();
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 7){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 8){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 20;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 9){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 10;
		*who		    = ALL;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 10){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 11){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 12){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 13){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 25;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 14){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*who		    = BANK;
		int homes = 0;
		int hotels = 0;
		for (int i = 0; i < 40; i++){
			if (ownership[i] == players[index].player_id){
				if (houses[i] < 5){
					homes+=houses[i];
				}
				if (houses[i] == 5){
					hotels+=1;
				}
			}
		}
		*price 		     = (homes * 40) + (hotels * 115);
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 15){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 10;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}	
	if (chance_index == 16){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
}
/*Handles chance*/
void handle_chance(int index, int dice, int *pay_to_or_pay_from, int *price, int *who){
	//pay_to_or_pay_from is 0 to pay to someone and 1 to collect
	//who is 0 to represent bank, player_id to rep player, 10 to rep all player
	//index also starts from 0
	//GO takes actual pos, not index
	srand(time(0));	
	clear();
	int chance_index = rand() % 16;
	
	//print_form("The chance_index is %d\n", chance_index);
	if (chance_index == 0){
		//advance to go
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		
		getch();
		int GO = 41 - players[index].current_position; 		
		handle_GO(index, GO, pay_to_or_pay_from, price, who);
		

		//3 means use pay_to_pay_from(1)
		//2 means buy property
		//1 means pay rent
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 1){
		//go to trafalgar square
		print_form("%s\n", chance[chance_index]);
		
		print_form("Press any key to proceed");
		getch();
		if (players[index].current_position <= 25){
			int GO = 25 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//3 means use pay_to_pay_from(1)
			//2 means buy property
			//1 means pay rent
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			int GO = 41 - players[index].current_position + 24;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//3 means use pay_to_pay_from(1)
			//2 means buy property
			//1 means pay rent
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		property(dice,24, index, pay_to_or_pay_from, price, who);

		if (*pay_to_or_pay_from == 2){
			handle_payment(2, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		return;
	}
	if (chance_index == 2){
		//advance to pall mall, if you pass go, collect 200
		print_form("%s\n", chance[chance_index]);
		
		print_form("Press any key to proceed");
		getch();
		if (players[index].current_position < 12){
			int GO = 12 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			int GO = 41 - players[index].current_position + 11;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		property(dice,11, index, pay_to_or_pay_from, price, who);

		if (*pay_to_or_pay_from == 2){
			handle_payment(2, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		return;
	}
	//find nearest neigbor, 12 or 28
	if ((chance_index == 3)||(chance_index == 4)){
		//go to nearest utility, moves forward only. pay dice * 10
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		int closest;
		if (chance_index == 3){
			closest = is_closest(players[index].current_position, 13, 29, -1, -1);
		}
		if (chance_index == 4){
			closest = is_closest(players[index].current_position, 6, 16, 26, 36);
		}
		 
		if (players[index].current_position < closest){
			int GO = closest - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			int GO = 41 - players[index].current_position + closest - 1;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
			
		}
		
		property(dice,closest - 1, index, pay_to_or_pay_from, price, who);
		if (who != BANK){
			if (chance_index == 3){
				*price = 10 * dice;
			}
			if (chance_index == 4){
				*price = 2 * (*price);
			}
		}
		

		if (*pay_to_or_pay_from == 2){
			handle_payment(2, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		return;
	}
	if (chance_index == 5){
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		*pay_to_or_pay_from = 1;
		*price		    = 50;
		*who		    = BANK;

		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 6){
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		players[index].GOOJFC++;
		handle_GO(index, 0, pay_to_or_pay_from, price, who);

		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;	
	}
	if (chance_index == 7){
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		if ((players[index].current_position - 3) > 0){
			players[index].current_position-=3;
		}
		else{
			players[index].current_position = 40 + (players[index].current_position - 3);
		}
		//handle_GO(index, 0, pay_to_or_pay_from, price, who);
		
		//handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		//property(dice,players[index].current_position - 1, index, pay_to_or_pay_from, price, who);
		movement(index, 0, pay_to_or_pay_from, price,who);
		//handle payment
		return;	
	}
	if (chance_index == 8){
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		players[index].current_position = 11;
		players[index].in_jail		 = 1;
		handle_GO(index, 0, pay_to_or_pay_from, price, who);
		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);

		
		return;
	}
	if (chance_index == 9){
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		*pay_to_or_pay_from = 0;
		*who		    = BANK;
		int homes = 0;
		int hotels = 0;
		for (int i = 0; i < 40; i++){
			if (ownership[i] == players[index].player_id){
				if (houses[i] < 5){
					homes+=houses[i];
				}
				if (houses[i] == 5){
					hotels+=1;
				}
			}
		}
		*price 		     = (homes * 25) + (hotels * 100);

		handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 10){	
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		*pay_to_or_pay_from = 0;
		*price		    = 15;
		*who		    = BANK;

		handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 11){
		//take a trip to kings cross station
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		if (players[index].current_position < 6){
			int GO = 6 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			int GO = 41 - players[index].current_position + 5;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		property(dice,5, index, pay_to_or_pay_from, price, who);
		

		if (*pay_to_or_pay_from == 2){
			handle_payment(2, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		return;
	}		
	if (chance_index == 12){
		//take a trip to kings cross station
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		if (players[index].current_position < 40){
			int GO = 40 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		if (colors[players[index].current_position - 1] != 0){
			property(dice,39, index, pay_to_or_pay_from, price, who);
			if (*pay_to_or_pay_from == 2){
				handle_payment(2, index, *pay_to_or_pay_from, *price, *who, -1);
			}
			else{
				handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
			}
		}


		return;
	}	
	if (chance_index == 13){	
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = ALL;

		handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 14){	
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		*pay_to_or_pay_from = 1;
		*price		    = 150;
		*who		    = BANK;

		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
	if (chance_index == 15){	
		print_form("%s\n", chance[chance_index]);
		print_form("Press any key to proceed");
		getch();
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;

		handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
		return;
	}
		
}
/*This generates a random number for the first and second dice*/
/*BE SURE TO USE SRAND(time(NULL)) BEFORE USING THIS FUNCTION*/
void roll_dice(int *dice_one, int *dice_two){
	
	*dice_one = (rand() % 6) + 1;
	*dice_two = (rand() % 6) + 1;
	clear();
	refresh();
	print_form("Your first dice is %d, and your second is %d\n", *dice_one, *dice_two);
	print_form("You have a total of %d\n", *dice_one+*dice_two);
	print_form("Please press any key to continue");
	getch();
}	
/*this handles the actual movement, and the behaviour of each box*/
void movement(int index, int amt_moved, int *pay_to_or_pay_from, int *price, int *who){
	//index also must be 0
	
	handle_GO(index, amt_moved, pay_to_or_pay_from, price, who);
	handle_payment(3, index, *pay_to_or_pay_from, *price, *who, -1);
	print_form("You are at location %d\n", players[index].current_position);
	int id = players[index].player_id;
	int pos = players[index].current_position - 1;
	if (ownership[pos] != id){
		if (strcmp(property_names[pos], "CHANCE") == 0){
			print_form("YOU ENTERED CHANCE\n");
			print_form("Press any key to proceed");
			getch();
			handle_chance(index, amt_moved, pay_to_or_pay_from, price,who);
			return;

		} 
		if (strcmp(property_names[pos], "COMMUNITY CHEST") == 0){
			print_form("YOU ENTERED COMMUNUITY CHEST\n");
			print_form("Press any key to proceed");
			getch();
			handle_community_chest(index, amt_moved, pay_to_or_pay_from, price,who);
			return;
		}
		if (strcmp(property_names[pos], "LUXURY TAX") == 0){
			print_form("YOU ENTERED LUXURY TAX\n");
			*pay_to_or_pay_from = 0;
			*price		    = 75;
			*who		    = BANK;
			print_form("Press any key to proceed");
			getch();
			handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
			return;
		}
		if (strcmp(property_names[pos], "GO TO JAIL") == 0){
			print_form("GO TO JAIL\n");
			players[index].current_position = 11;
			players[index].in_jail		 = 1;
			print_form("Press any key to proceed");
			getch();
			return;
		}
		if (strcmp(property_names[pos], "INCOME TAX") == 0){
			print_form("YOU ENTERED INCOME TAX\n");
			print_form("Press any key to proceed");
			getch();
			char *opt[] = {"Pay 10%",
				     "Pay $200",
				     NULL};
			main_menu(opt, 15, 50);
			if (highlight == 1){
				float total_asset = players[index].money;
				for (int i = 0; i < 40; i++){
					if (ownership[i] == players[index].player_id){
						if (mortgage[i] == 1){							total_asset+=mortgage_value[i];
						}
						else if(mortgage[i] == 0){
							total_asset+=prices[i];
						}
						if ((colors[i] == 1)||(colors[i] == 2)){
							total_asset = total_asset + (houses[i] * 50);
						}
						if ((colors[i] == 3)||(colors[i] == 4)){
							total_asset = total_asset + (houses[i] * 100);
						}				
						if ((colors[i] == 5)||(colors[i] == 6)){
							total_asset = total_asset + (houses[i] * 150);
						}
						if ((colors[i] == 7)||(colors[i] == 8)){
							total_asset = total_asset + (houses[i] * 200);
						}
					}
				}
				*pay_to_or_pay_from = 0;
				*price = (10 * total_asset)/100;
				*who = BANK;
				
			}
			if (highlight == 2){
				*pay_to_or_pay_from = 0;
				*price 		    = 200;
				*who		    = BANK;
			}
			handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
			return;
		}
		print_form("YOU ENTERED %s\n", property_names[pos]);
		print_form("Press any key to proceed");
		getch();
		property(amt_moved,pos, index, pay_to_or_pay_from, price, who);
		if (*pay_to_or_pay_from == 2){
			handle_payment(2, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		else{
			handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
		}
		if (*who == 11){
			print_form("The property belongs to you. Press any key to continue.\n");
			getch();
		}
		return;
		
	}

}
	
/*This basically checks for two major things before calling movement*/
	/*Checks to see if the player is in jail, hence behaviour should be a diff*/
	/*checks to see if the player rolls three times and puts them in jail*/
void rolling_conditions(int index, int *a, int *b, int *pay_to_or_pay_from, int *price, int *who){
	clear();
	refresh();
	if ((players[index].in_jail == 1)&&(players[index].current_position == 11)){
		players[index].time_spent_in_jail++;
		//if player rolled doubles
		if (*a == *b){
			print_form("YOU ROLLED DOUBLES, YOU GET OUT OF JAIL\n");
			movement(index, *a + *b, pay_to_or_pay_from, price, who);
			return;
		}
		//if the user has a get out of jail card
		if (players[index].GOOJFC > 0){
			if ((*a != *b)&&(players[index].time_spent_in_jail < 3)){
				//-would you like to use card, pay, wait for next turn
				
				char *opt[] = {"USE CARD",
					       "PAY",
					       "WAIT IN JAIL",
					       NULL};
				main_menu(opt, 20, 35);
				if (highlight == 1){
					clear();
					refresh();
					print_form("You have used one of your get our of jail cards.\nPress any key to continue");
					getch();
					players[index].in_jail = 0;
					players[index].time_spent_in_jail = 0;
					players[index].GOOJFC--;
					movement(index, *a + *b, pay_to_or_pay_from, price, who);

					return;
				}
				if (highlight == 2){
					clear();
					refresh();
					print_form("You are choosing to pay a $50 fine to get out of jail.\nPress any key to continue.");
					getch();
					*pay_to_or_pay_from = 0;
					*price 		    = 50;
					*who		    = BANK;
					players[index].in_jail = 0;
					players[index].time_spent_in_jail = 0;
					handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
					movement(index, *a + *b, pay_to_or_pay_from, price, who);
					return;
				}
				if (highlight == 3){
					clear();
					refresh();
					print_form("You are still in jail.\nPress any key to continue.");
					getch();
					return;				
				}
			}
			if ((*a != *b)&&(players[index].time_spent_in_jail == 3)){
				//-would you like to use card, pay 
				char *opt[] = {"USE CARD",
					       "PAY",
					       NULL};
				main_menu(opt, 20, 35);
				if (highlight == 1){
					clear();
					refresh();
					print_form("You have used one of your get our of jail cards.\nPress any key to continue");
					getch();
					players[index].GOOJFC--;
					players[index].in_jail = 0;
					players[index].time_spent_in_jail = 0;
					movement(index, *a + *b, pay_to_or_pay_from, price, who);
					return;
				}
				if (highlight == 2){
					clear();
					refresh();
					print_form("You are choosing to pay a $50 fine to get out of jail.\nPress any key to continue.");
					getch();
					*pay_to_or_pay_from = 0;
					*price 		    = 50;
					*who		    = BANK;
					players[index].in_jail = 0;
					players[index].time_spent_in_jail = 0;
					handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
					movement(index, *a + *b, pay_to_or_pay_from, price, who);
					return;
				}
			}
			return;
		}
		//if player has no get out of jail cards
		if (players[index].GOOJFC == 0){
			if ((*a != *b)&&(players[index].time_spent_in_jail < 3)){
				//-would you like to use pay, wait for next turn
				char *opt[] = {"PAY",
					       "WAIT IN JAIL",
					       NULL};
				main_menu(opt, 20, 35);
				if (highlight == 1){
					clear();
					refresh();
					print_form("You are choosing to pay a $50 fine to get out of jail.\nPress any key to continue.");
					getch();
					*pay_to_or_pay_from = 0;
					*price 		    = 50;
					*who		    = BANK;
					players[index].in_jail = 0;
					players[index].time_spent_in_jail = 0;
					handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
					return;
				}
				if (highlight == 2){
					clear();
					refresh();
					print_form("You are still in jail.\nPress any key to continue.");
					getch();
					return;				
				}
			}
			if ((*a != *b)&&(players[index].time_spent_in_jail == 3)){
				//-would you like to use pay 
				print_form("You couldn't roll doubles three times in a roll,so you have to pay a $50 fine to get out of jail.\nPress any key to continue.");
				getch();
				*pay_to_or_pay_from = 0;
				*price 		    = 50;
				*who		    = BANK;
				players[index].in_jail = 0;
				players[index].time_spent_in_jail = 0;
				handle_payment(1, index, *pay_to_or_pay_from, *price, *who, -1);
				movement(index, *a + *b, pay_to_or_pay_from, price, who);
				return;
			}
			return;
		}
		return;
	}
	srand(time(NULL));
	int count = 1;
repeat:	movement(index, *a + *b, pay_to_or_pay_from, price, who);
	if (*a == *b){
		count++;
		if (count < 3){
			clear();
			refresh();
			roll_dice(a, b);
			goto repeat;
		}
		//if rolled three times
		if (count == 3){
			clear();
			refresh();
			print_form("GO TO JAIL\n");
			players[index].current_position = 11;
			players[index].in_jail		 = 1;
			print_form("Press any key to proceed");
			getch();
		}
	}
}				

int main(int argc, char **argv){
	int number_gotten = 0;
	//if the user enters more than 2 arguements
	if (argc > 2){
		printf("Too many arguements, please enter a maximum of two arguements.\n");
		return 1;
	}
	//if the user enters two, hence the second must be the speed of animation
	if (argc == 2){
		int k = 0;	
		//check to see if the string contains non-numeric chars, also calc the size
		while (argv[1][k] != '\0'){
			if ((argv[1][k] < 48)||(argv[1][k] > 57)){
				printf("Second arguement is the speed, hence must be numeric.\n");
				return 1;
			}
			k++;
		}
		//convert from ascii to int
		for (int i = 0; i < k; i++){
			int power = 1;
			for (int j = 0; j < k - i - 1; j++){
				power = power * 10;
			}
			number_gotten = number_gotten + ((argv[1][i] - 48) *  power);
		} 
		SPEED = number_gotten;
	}
	//calculate the mortgage and value 
	for (int i = 0; i < 40; i++){
		mortgage_value[i] = prices[i] /2;
		mortgage[i] = 0;
		ownership[i] = 0;
	}
	//application started
	printf(GREEN("APPLICATION: starting up application\n\n"));
	refresh();
	initscr();	//start up new window
	int x,y;
	getmaxyx(stdscr, y, x);	//get the max y and x
	//if the size of the screen is not 132 x 41 screen resolution
	if ((y != 41) || (x != 132)){
		endwin();
		printf(RED("APPLICATION: please open the application using the default screen, precisely 132 x 41\n"));
		return 0;
	}
	DASH = x;		//make sure the dash prints to the max x
	dash_line(DASH);
	//if system has no colors 

	if (has_colors() == FALSE){
		endwin();		
		printf(RED("\nAPPLICATION: This system has no colors\n"));
		return 1;
	}
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);

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

	
	print("The rules of the game are simple. Firstly, Usernames can only be 5 characters long, while the save names must be at most 15 characters long. Only 2 - 8 players can play at a time. This game can be saved and be resumed at a later date. There is no autosave, save must be done manually by hitting ctrl+s. PLEASE PLAY IN FULLSCREEN FOR OPTIMAL EXPERIENCE.", y, 0);

	refresh();
	mvprintw(0,0,"Press any button to continue\n");	
	refresh();
	getch();
	clear();
	refresh();
	char **mains = malloc(sizeof(char *) * 3);
	if (mains == NULL){
		endwin();
		printf("No space in memory\n");
	}
	mains[0] = "Start new game";
	mains[1] = "Load saved game";
	mains[2] = NULL;
	
	main_menu(mains, 6, 30);
	clear();
	//mains[3] = NULL;
	
	//start reading the file names in
	if (is_empty_save_slot(save_file_one) && is_empty_save_slot(save_file_two) && is_empty_save_slot(save_file_three)){
		goto skip;
	}
	FILE *fp = fopen(save_file_save_name, "rb");
	int next_name = 0;
again:
	if (next_name == 0){	
		fread(save_one, sizeof(char), 16, fp);
	}
	else if (next_name == 1){
		fread(save_two, sizeof(char), 16, fp);
	}
	else if (next_name == 2){
		fread(save_three, sizeof(char), 16, fp);
	}	
	next_name++;
	if (next_name < 3){
		goto again;
	}
	fclose(fp);
	//done reading file names
	//if the user pressed enter while highlight was 1, start new game
	int here = 0; 
	//here is 0 - start new game
	//here is 1 - menu load
skip:	if (highlight == 1){

		start_new_game();
	}
	
	else if (highlight == 2){
		here = 1;
		menu_save(0);
		
		
	}
	free(mains);
	clear();
	refresh();
	dash_line(DASH);
	printw("ABOUT THE BOARD YOU ARE ABOUT TO SEE\n");
	printw("1. The first value in the box represents the block number.\n");
	printw("2. The second value in the box represents the property color.\n");
	printw("3. The third value in the box represents the owner of the property.\n");
	printw("4. The last value reresents the price of the property.\n");
	printw("5. If the property is morgaged, all this values in the box turn red.\n");
	printw("6. The wide center block shows the name for each property.\n");
	printw("7. The board will be printed after everyturn, press any button when you see this board to move on to the next players turn.\n");
	printw("8. The current player is marked using the green text\n");
	printw("9. If the property is mortgaged and another player is currently on the property, the text turns yellow to represent red + green.\n");
	printw("10. Inside the smaller box, you would see how the to each property, each property has a parenthesis right infront of it to represent the number of houses the property has.\n");
	printw("11. If a property has 5 houses(5 in the parenthesis), then the property has an hotel.\n");
	printw("12. Every player starts with $1500 dollars.\n");
	printw("13. The current position of all players can be seen right in front of the property name. For example, 1. GO>1>2, means player 1 and 2 are on location 1, A.K.A GO.\n");
	printw("14. Saving and exiting can only be done when the board is printed, when a player is still playing, you are unable to save or exit.\n");
	printw("15. Save can be done by pressing 'CTRL + S', and exiting can be done by pressing 'esc'.\n");
	printw("16. To see these rules again, please read the monopoly_emulator_rules.txt.\n");
	printw("17. As you would notice, the speed of the player movement animation(represented by making the current location green texted) may be too slow or too fast for some users. Therefore, to speed up the animation, be sure to attach a small number while executing this program. For example, running using './a.out 10' makes the animation extremely fast and running using './a.out 5000' makes the animation very slow. So it is recommended to pick a value between 100 - 3000, but again, it depends on YOU.\n");
	printw("18. Please to read the rules for monopoly, open up the rules.txt file. It contains the official monopoly rules as provided by hasbro.\n");


	dash_line(DASH);
	printw("Press any key to continue");
	getch();
	clear();
	refresh();
	//draw up board, working on this function atm
	if (here == 0){
		startup_board(curr_player, players[curr_player].current_position, 0);
	}
	else{
		int val = 0;
		if (curr_player == 0){	
			val = 0;
		}
		else{
			val = curr_player - 1;
		}
		startup_board(val, players[val].current_position, 0);
	}
	//GAME BEGINs
	srand(time(NULL));
	while(1){
		
		int roll_time = 0;
before_roll:	refresh();
		int center_loc_x = (DASH - 8)/2;
		clear();
		curr_player = curr_player % num_of_players;
		int player_id = players[curr_player].player_id;
		mvprintw(0, center_loc_x,"Player %d, %s: \n", player_id, players[curr_player].name);
		refresh();
		char *opt[] = {"1. Roll dice",
			     "2. Trade with a player",
			     "3. Mortgage property",
			     "4. Unmortgaged property",
			     "5. View player information",
			     "6. Sell houses/hotels",
			     "7. Build house/hotels",
			     "8. Declare bankruptcy",
			     NULL};
		main_menu(opt, 15, 50);
		int a, b;
		int prev = players[curr_player].current_position;
		if (highlight == 1){

			
			roll_dice(&a, &b);

			int s,v,w;
			clear();
			refresh();
			rolling_conditions(curr_player, &a, &b, &s, &v, &w);

		}
		else if (highlight == 8){
			declare_bankruptcy(curr_player);
			if (num_of_players != 1){
				curr_player--;
			}

		}
		else if (highlight == 3){
			mortgage_property(curr_player, 1);
			goto before_roll;
		}
		else if (highlight == 4){
			mortgage_property(curr_player, 0);
			goto before_roll;
		}
		else if (highlight == 6){
			clear();
			sell_houses(curr_player);
			goto before_roll;
		}
		else if (highlight == 7){
			clear();
			buy_houses(curr_player);
			goto before_roll;
		}
		else if (highlight == 5){
			print_player_info(curr_player);
			goto before_roll;
		}
		else if(highlight == 2){
			trade_with_player(curr_player);
			goto before_roll;
		}		
		//functionalities
		clear();
		refresh();
		if (num_of_players == 1){
			clear();
			refresh();
			attron(COLOR_PAIR(2));
			print_form("Congratulation player %d, you have won the game\n", players[curr_player].player_id);
			print_form("Press any button to continue\n");
			attroff(COLOR_PAIR(2));
			getch();
			refresh();
			break;
		}
		int temp = curr_player;
		curr_player++;
		startup_board(temp, prev, a + b);
		

		//print the board after rolling
	}
	
	endwin();
	//this should always be printed if the program was exited using esc, if the program was exited using ctrl+c, you wouldn't see this message
	printf(GREEN("APPLICATION: Application closed successfully\n"));
	return 0;
}
