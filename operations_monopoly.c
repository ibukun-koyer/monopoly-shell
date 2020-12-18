#include "new_monopoly.c"
void handle_payment(int action, int index, int pay_to_or_pay_from, int price, int who){
//if action is 1 - pay for rent
//if action is 2 - buy property from bank
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
	if (pay_to_or_pay_from == 0){
		printw("-You are about to make a payment of $%d to %s\n", price, sec);
	}
	if (pay_to_or_pay_from == 1){
		printw("-You are about to receive a payment of $%d from %s\n", price, sec);
	}
	
	refresh();
	if (action == 2){
restart:	refresh();	
		char *options[] = {"Buy",
				   "Auction",
				   NULL};
		main_menu(options, 15, 50);
		if (highlight == 1){
			if (players[index].money < price){
				clear();
				print_form("You do not have enough money to pay for this property, please select one of the following options\n");

				refresh();
				char *option[] = {"Auction",
					     "Mortgage property",
					     "Sell houses/hotels",
					     "trade",
					     NULL};
				main_menu(option, 15, 50);
				if (highlight == 1){
					//auction
				}
				if (highlight == 2){
					//mortgage property 
					goto restart;
				}
				if (highlight == 3){
					//sell houses
					goto restart;
				}
				if (highlight == 4){
					//trade
					goto restart;
				}
			}
			else{
				clear();
				printw("You have successfully made the payment\n");
				printw("You now have $%d in your wallet\n", players[index].money);
				printw("Press any key to continue");
				
				refresh();
				getch();
				players[index].money-=price;
			}
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
int monopoly(int pos){
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
}
void property(int dice, int pos, int index, int *pay_to_or_pay_from, int *price, int *who){
	int ME = BANK;
	//pos is assumed to be position - 1;
	//index is also assumed to be inde - 1;
	int id = players[index].player_id;
	if (ownership[pos] == 0){
		
		*pay_to_or_pay_from = 0;
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
void handle_GO(int index, int dice, int *pay_to_or_pay_from, int *price, int *who){
	int pos = players[index].current_position;
	players[index].current_position+=dice;
	if (players[index].current_position > 40){
		players[index].current_position = players[index].current_position - 40;
	}	
	//print_form("the current position is %d\n", players[index].current_position);
	if (pos > players[index].current_position){
		//passed go
		*pay_to_or_pay_from = 1;
		*price 		    = 200;
		*who		    = BANK;
	}
	else{
		*pay_to_or_pay_from = 0;
		*price 		    = 0;
		*who		    = BANK;
	}

}
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
		handle_GO(index, GO, pay_to_or_pay_from, price, who);
		
		print_form("Press any key to proceed");
		
		getch();
		//handle payment
		return;
	}
	if (chance_index == 1){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 200;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 2){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 3){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 4){
		print_form("%s\n", community_chest[chance_index]);
		players[index].GOOJFC++;
		handle_GO(index, 0, pay_to_or_pay_from, price, who);
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;	
	}
	if (chance_index == 5){
		print_form("%s\n", community_chest[chance_index]);
		players[index].current_position = 11;
		players[index].in_jail		 = 1;
		handle_GO(index, 0, pay_to_or_pay_from, price, who);
		//handle payment
		print_form("Press any key to proceed");
		getch();
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
		//handle payment
		return;
	}
	if (chance_index == 7){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 8){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 20;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 9){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 10;
		*who		    = ALL;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 10){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 11){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 12){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 13){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 25;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 14){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 0;
		*who		    = BANK;
		int homes = 0;
		int hotels = 0;
		for (int i = 0; i < 40; i++){
			if (ownership[i] == index + 1){
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
		//handle payment
		return;
	}
	if (chance_index == 15){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 10;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}	
	if (chance_index == 16){
		print_form("%s\n", community_chest[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
}
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
		int GO = 41 - players[index].current_position; 		
		handle_GO(index, GO, pay_to_or_pay_from, price, who);
		
		print_form("Press any key to proceed");
		
		getch();
		//handle payment
		return;
	}
	if (chance_index == 1){
		//go to trafalgar square
		print_form("%s\n", chance[chance_index]);
		if (players[index].current_position <= 25){
			int GO = 25 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//handle payment
		}
		else{
			int GO = 41 - players[index].current_position + 24;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//handle payment
		}
		property(dice,24, index, pay_to_or_pay_from, price, who);
		
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 2){
		//advance to pall mall, if you pass go, collect 200
		print_form("%s\n", chance[chance_index]);
		if (players[index].current_position < 12){
			int GO = 12 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//handle payment
		}
		else{
			int GO = 41 - players[index].current_position + 11;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//handle payment
		}
		property(dice,11, index, pay_to_or_pay_from, price, who);
		
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	//find nearest neigbor, 12 or 28
	if ((chance_index == 3)||(chance_index == 4)){
		//go to nearest utility, moves forward only. pay dice * 10
		print_form("%s\n", chance[chance_index]);
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
			//handle payment
		}
		else{
			int GO = 41 - players[index].current_position + closest - 1;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			
			//handle payment
			
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
		
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 5){
		print_form("%s\n", chance[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 50;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 6){
		print_form("%s\n", chance[chance_index]);
		players[index].GOOJFC++;
		handle_GO(index, 0, pay_to_or_pay_from, price, who);
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;	
	}
	if (chance_index == 7){
		print_form("%s\n", chance[chance_index]);
		if ((players[index].current_position - 3) > 0){
			players[index].current_position-=3;
		}
		else{
			players[index].current_position = 40 + (players[index].current_position - 3);
		}
		handle_GO(index, 0, pay_to_or_pay_from, price, who);
		//handle payment
		property(dice,players[index].current_position - 1, index, pay_to_or_pay_from, price, who);
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;	
	}
	if (chance_index == 8){
		print_form("%s\n", chance[chance_index]);
		players[index].current_position = 11;
		players[index].in_jail		 = 1;
		handle_GO(index, 0, pay_to_or_pay_from, price, who);
		//handle payment
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 9){
		print_form("%s\n", chance[chance_index]);
		*pay_to_or_pay_from = 0;
		*who		    = BANK;
		int homes = 0;
		int hotels = 0;
		for (int i = 0; i < 40; i++){
			if (ownership[i] == index + 1){
				if (houses[i] < 5){
					homes+=houses[i];
				}
				if (houses[i] == 5){
					hotels+=1;
				}
			}
		}
		*price 		     = (homes * 25) + (hotels * 100);
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 10){	
		print_form("%s\n", chance[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 15;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 11){
		//take a trip to kings cross station
		print_form("%s\n", chance[chance_index]);
		if (players[index].current_position < 6){
			int GO = 6 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//handle payment
		}
		else{
			int GO = 41 - players[index].current_position + 5;
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//handle payment
		}
		property(dice,5, index, pay_to_or_pay_from, price, who);
		
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}		
	if (chance_index == 12){
		//take a trip to kings cross station
		print_form("%s\n", chance[chance_index]);
		if (players[index].current_position < 40){
			int GO = 40 - players[index].current_position;	
			handle_GO(index, GO, pay_to_or_pay_from, price, who);
			//handle payment
		}
		if (colors[players[index].current_position - 1] != 0){
			property(dice,39, index, pay_to_or_pay_from, price, who);
		}
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}	
	if (chance_index == 13){	
		print_form("%s\n", chance[chance_index]);
		*pay_to_or_pay_from = 0;
		*price		    = 50;
		*who		    = ALL;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 14){	
		print_form("%s\n", chance[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 150;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
	if (chance_index == 15){	
		print_form("%s\n", chance[chance_index]);
		*pay_to_or_pay_from = 1;
		*price		    = 100;
		*who		    = BANK;
		print_form("Press any key to proceed");
		getch();
		//handle payment
		return;
	}
		
}
/*BE SURE TO USE SRAND(time(NULL)) BEFORE USING THIS FUNCTION*/
void roll_dice(int *dice_one, int *dice_two){
	
	*dice_one = (rand() % 6) + 1;
	*dice_two = (rand() % 6) + 1;
}	
void movement(int index, int amt_moved, int *pay_to_or_pay_from, int *price, int *who){
	//index also must be 0
	
	handle_GO(index, amt_moved, pay_to_or_pay_from, price, who);
	//handle payment
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
			//handle payment
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
			//PROMPT THE USER TO ENTER EITHER 10% OR 200
			//handle payment
			return;
		}
		print_form("YOU ENTERED %s\n", property_names[pos]);
		print_form("Press any key to proceed");
		getch();
		property(amt_moved,pos, index, pay_to_or_pay_from, price, who);
		return;
		
	}

}
	

void rolling_conditions(int index, int a, int b, int *pay_to_or_pay_from, int *price, int *who){
	if ((players[index].in_jail == 1)&&(players[index].current_position == 11)){
		players[index].time_spent_in_jail++;
		print_form("BRODI IN JAIL\n");
		if (a == b){
			print_form("YOU ROLLED DOUBLES, YOU GET OUT OF JAIL\n");
			movement(index, a + b, pay_to_or_pay_from, price, who);
			return;
		}
		if (players[index].GOOJFC > 0){
			if ((a != b)&&(players[index].time_spent_in_jail < 3)){
				//-would you like to use card, pay, wait for next turn
			}
			if ((a != b)&&(players[index].time_spent_in_jail == 3)){
				//-would you like to use card, pay 
			}
			return;
		}
		if (players[index].GOOJFC == 0){
			if ((a != b)&&(players[index].time_spent_in_jail < 3)){
				//-would you like to use pay, wait for next turn
			}
			if ((a != b)&&(players[index].time_spent_in_jail == 3)){
				//-would you like to use pay 
			}
			return;
		}
		return;
	}
	srand(time(NULL));
	int count = 1;
repeat:	movement(index, a + b, pay_to_or_pay_from, price, who);
	if (a == b){
		count++;
		if (count < 3){
			print_form("RE-ROLL BRO\n");
			roll_dice(&a, &b);
			goto repeat;
		}
		if (count == 3){
			print_form("GOTO JAIL MAN\n");
			//goto jail
		}
	}
}				

int main(){
	for (int i = 0; i < 40; i++){
		mortgage_value[i] = prices[i] /2;
		mortgage[i] = 0;
		ownership[i] = 0;
	}
	printf(GREEN("APPLICATION: starting up application\n\n"));
	refresh();
	initscr();	//start up new window
	int x,y;
	getmaxyx(stdscr, y, x);	//get the max y and x
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

	
	print("The rules of the game are simple. Firstly, player usernames that include spaces are truncated. Usernames can only be 15 characters long. Only 2 - 8 players can play at a time. This game can be saved and be resumed at a later date. There is no autosave, save must be done by selecting the save option while playing. PLEASE PLAY IN FULLSCREEN FOR OPTIMAL EXPERIENCE.", y, 0);

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
skip:	if (highlight == 1){
		start_new_game();
	}
	
	if (highlight == 2){
		menu_save(0);
		
		
	}
	free(main);
	clear();
	refresh();
	dash_line(DASH);
	printw("ABOUT THE BOARD YOU ARE ABOUT TO SEE\n");
	printw("1. The first value in the box represents the block number\n");
	printw("2. The second value in the box represents the property color\n");
	printw("3. The third value in the box represents the owner of the property\n");
	printw("4. The last value reresents the price of the property\n");
	printw("5. If the property is morgaged, all this values in the box turn red\n");
	printw("6. The wide center block shows the name for each property\n");
	printw("7. The board will be printed after everyturn, press any button when you see this board to move on to the next players turn\n");

	dash_line(DASH);
	printw("Press any key to continue");
	getch();
	clear();
	refresh();
	//draw up board, working on this function atm
	startup_board();

	//GAME BEGIN
	srand(time(NULL));
	while(1){
		handle_payment(2, 0, 0, 10000, BANK);
		/*int roll_time = 0;
before_roll:	refresh();
		int center_loc_x = (DASH - 8)/2;
		clear();
		curr_player = curr_player % num_of_players;
		int player_id = players[curr_player].player_id;
		mvprintw(0, center_loc_x,"Player %d: \n", player_id);
		refresh();
		char *opt[] = {"1. Roll dice",
			     "2. Trade with a player",
			     "3. Mortgage property",
			     "4. View player information",
			     "5. View other player information",
			     "6. Build house",
			     "7. Build hotel",
			     "8. Declare bankruptcy",
			     NULL};
		main_menu(opt, 15, 50);
		if (highlight == 1){
			clear();
			int a, b, pay_to_or_pay_from, price, who;
			roll_dice(&a, &b);
			movement(curr_player, a + b, &pay_to_or_pay_from, &price, &who);
		}
					
		//functionalities
		clear();
		refresh();
		startup_board();
		curr_player++;*/
		//print the board after rolling
	}
	
	endwin();
	printf(GREEN("APPLICATION: Application closed successfully\n"));
	return 0;
}
