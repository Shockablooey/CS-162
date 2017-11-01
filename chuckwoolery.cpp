/********************************************************************* 
 ** Program Filename: chuckwoolery.cpp
 ** Author: Chase Stubblefield
 ** Date: 19 February 2017 
 ** Description: Wheel of Fortune simulator
 ** Input: Menu choices, number of rounds/players, and letter guesses
 ** Output: the game simulation
*********************************************************************/

#include <iostream>
#include <ctime> // used for seeding the random number generator
#include <string>
#include <cstdlib>
#include <unistd.h> // used for sleep()

using namespace std;


/*--- FUNCTION DECLARATIONS ------------------------------------------------------*/
void bankrupt(int &, int &, int &, int &);
void blank_board(string, string &);
void board_display(string &);
void buy_vowel(string , string &, int &, int &, int &, int &);
void check_money_for_vowel(int &, int &, int &, int &, bool &, string, string &);
void display_game_scores(int &, int &, int &); 
void money_for_letters(int &, int &, int &, int &, int &, int &);
void player_loop(int &, int &, int &, int &, string, string &, bool &, int &);
void round_loop (int &, int &, int &, int &, int &); 
void round_over(int &, int &, int &, int &, int &, int &, int &, int &);
void spin_wheel(bool &, int &, int &, int &, int &, string, string &);
void startup_display(int &, int &);
void turn_loop(int &, int &, int &, int &, string, string &, int &, int &, int &, int &);
void turn_menu(int &, int &, int &, int &);
int guess_board(string, string &, bool &, int &, int &, int &, int &, int &);
int solve(bool &, string, bool &, int &);
int get_123(string);
char is_consonant();
char is_vowel();
string get_lowercase_string();
/*--------------------------------------------------------------------------------*/ 

int main(){
	int numRounds;
	int numPlayers;
	int totalMoneyP1 = 0;
	int totalMoneyP2 = 0;
	int totalMoneyP3 = 0;
	
	startup_display(numRounds, numPlayers);
	round_loop(totalMoneyP1, totalMoneyP2, totalMoneyP3, numRounds, numPlayers);
	return 0;
} // end of main function

/*--------------------------------------------------------------------------------*/ 
// Additions:
// time delay for spinning the wheel
// extra aesthetics throughout
// when secret message is inputted, hides what was inputted
// money is multiplied by 100 to imitate the real game
/*--------------------------------------------------------------------------------*/ 


/********************************************************************* 
 ** Function: startup_display
 ** Description: beginning display that calls an error checking function to get two ints
 ** Parameters: nothing. it's a void function
 ** Pre-Conditions: none
 ** Post-Conditions: numRounds and numPlayers will have been inputted
 ** Return: none
 *********************************************************************/
void startup_display(int &numRounds,int &numPlayers){
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
	cout << "|                             Welcome to my Wheel of Fortune Game Simulator!                         " << endl;
	cout << "| " << endl;			       
	cout << "|			      How many rounds? (1-3): ";
	numRounds = get_123("");
	cout << "| " << endl;
	cout << "|			      How many players? (1-3): ";
	numPlayers = get_123("");
	cout << "| " << endl;	
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
}
/********************************************************************* 
 ** Function: round_loop
 ** Description: a loop that repeats for the number of rounds specified by the user
 ** Parameters: the game money for each player, number of players, and number of rounds
 ** Pre-Conditions: numRounds and numPlayers have been set prev.
 ** Post-Conditions: game will be over when this function ends
 ** Return: none
 *********************************************************************/
void round_loop(int &totalMoneyP1, int &totalMoneyP2, int &totalMoneyP3, int &numRounds, int &numPlayers){
	int moneyP1 = 0;
	int moneyP2 = 0;
	int moneyP3 = 0;	
	int round = 1;
	string secret_message;
	string board;

	do{
		moneyP1 = 0;
		moneyP2	= 0;
		moneyP3 = 0;	
		cout << "Enter the secret message: ";
		if (round == 1)
			cin.ignore(); // to make the cin work on round 1 since getline and cin don't work well together
		secret_message = get_lowercase_string(); // get a good input for secret_message
		blank_board(secret_message, board); // turn board to dashes
		board_display(board); // display the board

		turn_loop(numPlayers, moneyP1, moneyP2, moneyP3, secret_message, board, totalMoneyP1, totalMoneyP2, totalMoneyP3, round); // start the player's turns 
		
		round++;
	}while(round < numRounds + 1);
	cout << " ****************************************************************************************************" << endl; 
	cout << "|" << endl;
	cout << "|			      *** The game is over! ***" << endl;
	cout << "|" << endl;
	cout << " ****************************************************************************************************" << endl; 
}

/********************************************************************* 
 ** Function: turn_loop
 ** Description: loop that switches player turns when a player makes a mistake
 ** Parameters: number of players/rounds, money values, the message & the current board
 ** Pre-Conditions: a round is still going
 ** Post-Conditions: current round is over
 ** Return: none
 *********************************************************************/
void turn_loop(int &numPlayers, int &moneyP1, int &moneyP2, int &moneyP3, string secret_message, string &board, int &totalMoneyP1, int&totalMoneyP2, int&totalMoneyP3, int &round){

	bool unsolved = true;
	int player = 1;
	int whoWon; // declare the whoWon int that we will pass to player_loop() then to solve() to decide end of round money

	do{ 
		player_loop(player, moneyP1, moneyP2, moneyP3, secret_message, board, unsolved, whoWon);	
		player++;
		if (player > numPlayers){
			player = 1;
		}
	}while(unsolved == true);
	cout << "\n\n    *** The round has ended *** \n\n" << endl;
	round_over(moneyP1, moneyP2, moneyP3, totalMoneyP1, totalMoneyP2, totalMoneyP3, round, whoWon);
	display_game_scores(totalMoneyP1, totalMoneyP2, totalMoneyP3);
}	
/********************************************************************* 
 ** Function: player_loop
 ** Description: allows a player to keep making actions as long as they are successful
 ** Parameters: money values, whose turn it is, the message & board, the bool unsolved to end the round when board is solved, and int whoWon
 ** Pre-Conditions: it is still the players turn
 ** Post-Conditions: the players turn is over/the round is over
 ** Return: none
 *********************************************************************/
void player_loop(int &player, int &moneyP1, int &moneyP2, int &moneyP3, string secret_message, string &board, bool &unsolved, int &whoWon){
	bool success = true;
	int choice;
	
	while(success == true){

		turn_menu(player, moneyP1, moneyP2, moneyP3);
		choice = get_123("Your choice: ");
		
		if (choice == 1){
			spin_wheel(success, player, moneyP1, moneyP2, moneyP3, secret_message, board);
		}
		else if (choice == 2){
			check_money_for_vowel(moneyP1, moneyP2, moneyP3, player, success, secret_message, board);
		}
		else if (choice == 3){
			board_display(board);
			whoWon = solve(unsolved, secret_message, success, player);
		}
	}
}	
/********************************************************************* 
 ** Function: turn_menu 
 ** Description: displays the menu for each action of a player
 ** Parameters: the money values and which player it is 
 ** Pre-Conditions: a player's turn is going
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
void turn_menu(int &player, int &moneyP1, int &moneyP2, int &moneyP3){ 
	
	cout << " ----------------------------------------------------------------------------------------------------" << endl;	
	cout << "                      It is player " << player << "'s turn." << endl;
	cout << "        	      Player " << player << ", you have ";
	if (player == 1){
		cout << moneyP1;
	}
	else if (player == 2){
		cout << moneyP2;
	}
	else if (player == 3){
		cout << moneyP3;
	}
	cout << " dollars.";
	cout << " " << endl;
	cout << "		      What would you like to do?" << endl; 
	cout << " 		      (1) Spin the wheel (2) Buy a vowel (3) Guess the word/phrase " << endl;
	cout << "|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n";
}
/********************************************************************* 
 ** Function: blank_board
 ** Description: takes the secret_message and creates a string, board, that is the message converted to underscores
 ** Parameters: string secret_message and string board
 ** Pre-Conditions: secret message has been set
 ** Post-Conditions: board is set
 ** Return: none
 *********************************************************************/
void blank_board(string secret_message, string &board){
	board = secret_message;
	for (int i = 0; i < secret_message.length(); i++){
		if ((board).at(i) != ' '){
			(board).at(i) = '-';
		}			
	}
	cout << "|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n";
	cout << "|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n" << endl;
}
/********************************************************************* 
 ** Function: guess_board 
 ** Description: allows a player to guess a consonant after a wheel spin
 ** Parameters: the message strings, bool success to possibly end a turn, the spin result, whose turn it is, and the money values
 ** Pre-Conditions: a player has chosen to guess a consonant
 ** Post-Conditions: the players guess is added to "board" if it is in the secret message
 ** Return: unused
 *********************************************************************/
int guess_board(string secret_message, string &board, bool &success, int &moneyP1, int &moneyP2, int &moneyP3, int &spin, int &player){
	char x;
	int howManyCorrect = 0;

	x = is_consonant();
	for (int i = 0; i < secret_message.size(); i++){	
		if (x == secret_message.at(i) || x == secret_message.at(i) + 32 || x == secret_message.at(i) - 32){
			(board).at(i) = x;
			howManyCorrect++;
		}
	}
	cout << "You found: " << howManyCorrect << " letter(s)!" << endl;
	if (howManyCorrect == 0){
		success = false;
	}
	else if (howManyCorrect > 0){
		money_for_letters(moneyP1, moneyP2, moneyP3, player, howManyCorrect, spin);
	}
}
/********************************************************************* 
 ** Function: board_display 
 ** Description: simple board output
 ** Parameters: the board
 ** Pre-Conditions: none
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
void board_display(string &board){
	cout << " ----------------------------------------------------------------------------------------------------" << endl;	
	cout << "" << endl;
	cout << "| The board:          " << board << endl;
	cout << "" << endl;
	
}
/********************************************************************* 
 ** Function: spin_wheel
 ** Description: gives the player random spin 0-21
 ** Parameters: success bool, whose turn, money values, and the message strings
 ** Pre-Conditions: player has chosen to spin wheel
 ** Post-Conditions: the player will guess and recieve money if correct
 ** Return: none
 *********************************************************************/
void spin_wheel(bool &success, int &player, int &moneyP1, int &moneyP2, int &moneyP3, string secret_message, string &board){
	int spin; // our spin variable
	srand (time(NULL)); // seeds the random number generator
	
	cout << "** The wheel is spinning! **" << endl;
	cout << "." << endl;
	sleep(1);
	cout << ".." << endl;
	sleep(1);
	cout << "..." << endl;
	
	spin = rand()%22;
	if (spin == 0){
		cout << "Looks like the odds are not in your favor.  B B B B B B BANKRUPT!!!!!" << endl;
		bankrupt(moneyP1, moneyP2, moneyP3, player);
		success = false;
		board_display(board);
	} 
	else if (spin == 21){
		cout << "The wheel has landed on 'Lose Turn'" << endl;
		success = false;
		board_display(board);
	}
	else {
		cout << "The wheel has landed on: " << spin*100 << "!" << endl; 	
		int howManyCorrect = guess_board(secret_message, board, success, moneyP1, moneyP2, moneyP3, spin, player);
		board_display(board);
	}
	
}

/********************************************************************* 
 ** Function: buy_vowel
 ** Description: buys a vowel and checks the vowel
 ** Parameters: message strings, whose turn, and the money values
 ** Pre-Conditions: player has chosen to buy a vowel and he/she has enough money
 ** Post-Conditions: board is altered if player is correct
 ** Return: none
 *********************************************************************/
void buy_vowel(string secret_message, string &board, int &player, int &moneyP1, int &moneyP2, int &moneyP3){

	if (player == 1) // charges the correct player $10
		moneyP1 = moneyP1 - 1000;
	else if (player == 2)
		moneyP2 = moneyP2 - 1000;
	else if (player == 3)
		moneyP3 = moneyP3 - 1000;
	
	char x; // our input variable
	int howManyCorrect = 0;
	
	x = is_vowel();
	for (int i = 0; i < secret_message.size(); i++){	
		if (x == secret_message.at(i)){
			(board).at(i) = x;
			howManyCorrect++;
		}
	}
	cout << "You found: " << howManyCorrect << " vowel(s)!" << endl;
	board_display(board);
}
/********************************************************************* 
 ** Function: check_money_for_vowel
 ** Description: checks to see if a player has enough money first before letting them input a vowel
 ** Parameters: round money scores, whose turn it is, if the player's turn should still be going, the message, and the current board
 ** Pre-Conditions: player has chosen to buy a vowel
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
void check_money_for_vowel(int &moneyP1, int &moneyP2, int &moneyP3, int &player, bool &success, string secret_message, string &board){
	if (player == 1){
		if (moneyP1 < 1000){
			success = false;
			player = 3;
		}
	}
	else if (player == 2){
		if (moneyP2 < 1000){
			success = false;
			player = 1;
		}
	}
	else if (player == 3){
		if (moneyP3 < 1000){
			success = false;
			player = 2;
		}
	}
	if (success == true){
		buy_vowel(secret_message, board, player, moneyP1, moneyP2, moneyP3);
	}
}
/********************************************************************* 
 ** Function: bankrupt 
 ** Description: makes players money 0
 ** Parameters: money values, and whose turn it is
 ** Pre-Conditions: player spins a 0
 ** Post-Conditions: money is 0
 ** Return: none
 *********************************************************************/
void bankrupt(int &moneyP1, int &moneyP2, int &moneyP3, int &player){
	if      (player == 1)
		moneyP1 = 0;

	else if (player == 2)
		moneyP2 = 0;

	else if (player ==3)
		moneyP3 = 0;
}
/********************************************************************* 
 ** Function: money_for_letters 
 ** Description: finds out how much money a player should recieve
 ** Parameters: money values, how many occurences of the guess there were, and the spin
 ** Pre-Conditions: player guesses a letter
 ** Post-Conditions: money is given to a player
 ** Return: none
 *********************************************************************/
void money_for_letters(int &moneyP1, int &moneyP2, int &moneyP3, int &player, int &howManyCorrect, int &spin){
	if (player == 1){
		moneyP1 = moneyP1 + howManyCorrect * spin * 100;
	}
	else if (player == 2){
		moneyP2 = moneyP2 + howManyCorrect * spin * 100;
	}	
	else if (player == 3){
		moneyP3 = moneyP3 + howManyCorrect * spin * 100;
	}
}	
/********************************************************************* 
 ** Function: solve 
 ** Description: used to solve the puzzle
 ** Parameters: bool to end round, secret_message, and whose turn it is
 ** Pre-Conditions: player has chosen to solve the puzzle
 ** Post-Conditions: players turn ends or round ends
 ** Return: whoWon int that is used to determine end of round money values
 *********************************************************************/
int solve(bool &unsolved, string secret_message, bool &success, int &player){
	string input;
	int whoWon;
	
	cout << "Guess the word/phrase!" << endl;
	cin.ignore(); // to make the cin work
	input = get_lowercase_string();
	if (secret_message == input){
		cout << "\n******* You've solved the puzzle! *******\n" << endl;
		whoWon = player; // sets whoWon to whatever the integer of the current player is
		success = false; 
		unsolved = false;
	}
	else {
		cout << "Your answer (or my program) is invalid." << endl;
		success = false;
	}
	return whoWon;
}
/********************************************************************* 
 ** Function: get_123 
 ** Description: checks input for 1 2 or 3
 ** Parameters: a string prompt
 ** Pre-Conditions: an input is requested
 ** Post-Conditions: a good input is given
 ** Return: the good input
 *********************************************************************/
int get_123(string promptInput){
	bool good = false; // good stays false until we get the right input
	string input;
	int goodInput;
	
	while(good == false){
		cout << promptInput; // the custom user prompt for input given when the function is called
		cin >> input;

		if (input.length() != 1){ // if no input or more than one character, return false
			cout << "Input is not 1, 2, or 3." << endl;
		}
		else if (input.at(0) == '1' || input.at(0) == '2' || input.at(0) == '3'){
			good = true; // if we find no errors in the for loop cycle, return true
		}
		else{
			cout << "Input is not 1, 2, or 3." << endl;
		}	
	}
	goodInput = atoi(input.c_str()); // converts 1, 2, or 3 (stored as a string) to an int
	return goodInput;
}
/********************************************************************* 
 ** Function: round_over 
 ** Description: changes money values at end of round
 ** Parameters: money values, whoWon, what round it is
 ** Pre-Conditions: a round ended
 ** Post-Conditions: money values are changed
 ** Return: none
 *********************************************************************/
void round_over(int &moneyP1, int &moneyP2, int &moneyP3, int &totalMoneyP1, int &totalMoneyP2, int &totalMoneyP3, int &round, int &whoWon){
	if (whoWon == 1){
		totalMoneyP1 = totalMoneyP1 + moneyP1; // the player who won adds his round score to the total score
	}		
	else if (whoWon == 2){
		totalMoneyP2 = totalMoneyP2 + moneyP2;
	}		
	else if (whoWon == 3){
		totalMoneyP3 = totalMoneyP3 + moneyP3;
	}		
}
/********************************************************************* 
 ** Function: display_game_scores 
 ** Description: simple game money display
 ** Parameters: money values
 ** Pre-Conditions: none
 ** Post-Conditions: none
 ** Return: none
 *********************************************************************/
void display_game_scores(int &totalMoneyP1, int &totalMoneyP2, int &totalMoneyP3){ 
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
	cout << "| " << endl;			       
	cout << "|			      Current Game Scores" << endl;
	cout << "|			      P1 $: " << totalMoneyP1 << endl;
	cout << "|			      P2 $: " << totalMoneyP2 << endl;
	cout << "|			      P3 $: " << totalMoneyP3 << endl;
	cout << "| " << endl;	
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
}
/********************************************************************* 
 ** Function: get_lowercase_string 
 ** Description: check to see if input is all lowercase
 ** Parameters: none
 ** Pre-Conditions: a string is requested
 ** Post-Conditions: a good string is given
 ** Return: good input
 *********************************************************************/
string get_lowercase_string(){
	string input;
	bool good = true;
	string goodInput;

	do{
		good = true;
		getline(cin,input);
		for (int i = 0; i < input.length(); i++){ // cycle through each character of string input
			if ( input.at(i) != ' ' && (input.at(i) < 'a' || input.at(i) > 'z') ) 
				good = false; // if not lowercase or space, set good to false and do loop again 
		}
		if (good == false)
			cout << "Please enter a string of lowercase characters and spaces." << endl;	
	} while (good == false);
	
	goodInput = input; // if we break out the while loop, input was good
	return goodInput;
	
}
/********************************************************************* 
 ** Function: shameful_is_vowel_function 
 ** Description: the function has brought great dishonor to me, my family, and my cow
 ** Parameters: this function is a failure regardless
 ** Pre-Conditions: "wow, I sure hope my function will work!"
 ** Post-Conditions: "dangit + many other expletives"
 ** Return: disappointment
 *********************************************************************/
/*char is_vowel(){
	string input;
	bool good = true;
	char vowel;

	cin.ignore();
	do{
		good = true;
		cout << good << endl;
		getline(cin, input);
		cout << good << endl;	
		if (input.length() != 1 || input.at(0) != 'a' || input.at(0) != 'e' || input.at(0) != 'i' || input.at(0) != 'o' || input.at(0) != 'u'){ 
			good = false; // if not lowercase or space, set good to false and do loop again 
			cout << "Please enter a vowel." << endl;
		}	
	} while (good == false);
	
	// if we break out the while loop, input was good

		
	vowel = input.at(0);
	return vowel;	
}
*/
/********************************************************************* 
 ** Function: is_vowel 
 ** Description: takes in an input and checks to see if it is char a, e, i, o, or u
 ** Parameters: none
 ** Pre-Conditions: player must have enough money
 ** Post-Conditions: char x is set to char guess from this function
 ** Return: char guess
 *********************************************************************/
char is_vowel(){
	bool fail; 
	char guess;
		
	cout << "Choose a vowel! ";
	cin >> guess;
	
	do {
		fail = false; // set fail to false and make it true if we do not have an a, e, i, o, or u
		if (!(guess == 'a' || guess == 'e' || guess == 'i' || guess == 'o' || guess == 'u')){
			fail = true; 
		}
		if (fail == true){
			cout << "Please enter a vowel: "; // reprompt user
			cin >> guess; 
		}
	} while(fail == true); // keep doing until fail = false

	return guess;
}
/********************************************************************* 
 ** Function: is_consonant 
 ** Description: takes in an input and checks to see if it is not chat a, e, i, o, or u
 ** Parameters: none
 ** Pre-Conditions: player must have spun a 1-20
 ** Post-Conditions: char x is set to char guess from this function
 ** Return: char guess
 *********************************************************************/
char is_consonant(){
	bool fail; 
	char guess;
		
	cout << "Choose a consonant! ";
	cin >> guess;
	
	do {
		fail = false; // set fail to false and make it true if we do not have an a, e, i, o, or u
		if ((guess == 'a' || guess == 'e' || guess == 'i' || guess == 'o' || guess == 'u'|| guess < 'a' || guess > 'z')){
			fail = true; 
		}
		if (fail == true){
			cout << "Please enter a consonant: "; // reprompt user
			cin >> guess; 
		}
	} while(fail == true); // keep doing until fail = false

	return guess;
}


