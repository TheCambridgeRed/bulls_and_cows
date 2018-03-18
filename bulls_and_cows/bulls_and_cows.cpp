// bulls_and_cows
// v3.3 2018-03-13

#include "std_lib_facilities.h"

// GLOBAL CONSTANTS
// -----------------------------------------------------------------------------

const string prompt = "> ";
const string quit = "quit";
const string quit_text = "Gave up!\n";
const string rules_check_text = "Would you like to read the rules?";
const string play_again_text = "Would you like to play again?";

// FUNCTIONS
// -----------------------------------------------------------------------------

void clean(runtime_error e)
{
	cerr << e.what();
	cin.clear();
	cin.ignore(10000, '\n');
	return;
}

int difficulty ()
{	
	const string easy = "easy";
	const string medium = "medium";
	const string hard = "hard";

	const unordered_map<string, int> difficulties = {
		{ easy, 6 },
		{ medium, 8 },
		{ hard, 10 }
	};

	cout << "\nChoose a difficulty: easy (1-6) | medium (1-8) | hard (0-9)\n";

	string diff_label;
	while (true) {
		try {
			cin >> diff_label;
			if (diff_label != easy && diff_label != medium && diff_label != hard)
				error("easy | medium | hard\n");
			else break;
		}
		catch (runtime_error& e) {
			clean(e);
		}
	}
	return difficulties.at(diff_label);
}

bool check(string check_text)
// general [Y/n] test
{
	char checker = 'x';
	cout << check_text << " [Y/n]\n";

	while (checker != 'Y' && checker != 'n') {
		try {
			if (!(cin >> checker)) error("Enter Y/n: ");
			switch (checker) {
			case 'Y':
				return true;
			case 'n':
				return false;
			default:
				error("Enter Y/n: ");
			}
		}
		catch (runtime_error& e) {
			clean(e);
		}
	}
}

string number_gen(string number, int top)
{
	srand(static_cast<unsigned int>(time(NULL)));
	vector<int> local_number(4);
	// find another way to do this
	const vector<char> numbers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	for (size_t i = 0; i < local_number.size(); ++i) {
	    	if (top < 9)
			local_number[i] = (rand() % top + 1);
	    	else if (top > 9)
			local_number[i] = (rand() % top);
	}
		// can remove repeated digits; incorporate into for loop if this is needed
		// if (i >= 1) {
		//	for (size_t j = i - 1; j < local_number.size(); --j) {
		//		while (local_number[i] == local_number[j]) 
		//			local_number[i] = (rand() % top);
		//	}
		// }

	for (size_t i = 0; i < local_number.size(); ++i)
		number[i] = numbers[local_number[i]]; // populates string with char digits
	                                              // identical to local_number
	return number;
}

vector<int> initialise(vector<int> local)
{
	for (size_t i = 0; i < local.size(); ++i)
		local[i] = 0;
	return local;
}

bool is_number(string& s)
{
	return !s.empty() && find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

string get_guess(string local, int guesses)
// accepts a string in the form "0000". Rejects wrong size and not all digits
{
	while (true) {
		try {
			cin >> local;
			if (local == quit) return local;
			else if (local.size() != 4 || !is_number(local)) error("Guess must be 4 digits!\n");
			break;
		}
		catch (runtime_error& e) {
			clean(e);
			cout << guesses << prompt;
		}
	}

	//	for (size_t i = 1; i < local.size(); ++i) {
	//	for (size_t j = i - 1; j < local.size(); --j) {
	//		if (local[i] == local[j]) error("No repeated digits");
	//	}
	//}
	return local;
}

vector<int> compare(string guess, string goal, vector<int> result)
{
	const char seen = 'x';
  	const char used = 'u';
  
  	// work out bulls
  	for (size_t i = 0; i < guess.size(); ++i) {
    		if (guess[i] == goal[i]) {
      		++result[0];
      		guess[i] = used;
      		goal[i] = seen;
		}
	}
  
  	//work out cows
  	for (size_t i = 0; i < guess.size(); ++i) {
    		if (guess[i] != goal[i]) {
      			for (size_t j = 0; j < goal.size(); ++j) {
				if (guess[i] == goal[j]) {
	  			++result[1];
	  			guess[i] = used;
	  			goal[j] = seen;
	  			break;
				}
			}
		}
	}
  	return result;
}

void win(string number, int guesses)
{
	cout << "You win! My number was: "
		<< number << '\n'
		<< "You took " << guesses << " guesses.\n";
}

void rules()
{
	cout << "I will think of 4 digits. You have to guess\n"
		<< "each digit correctly and in the right place\n"
		<< "to win. If you guess a digit correctly in\n"
		<< "the right place, you get a Bull. If you guess\n"
		<< "a digit correctly but in the wrong place, you\n"
		<< "get a Cow. 4 Bulls wins! (Type \"quit\" to\n"
		<< "give up for the round.)\n";
	return;
}

void intro()
{
	bool print_rules = false;

	cout << '\n'
		<< "        ==== B U L L S ====\n"
		<< "        ====== A N D ======\n"
		<< "        ===== C O W S =====\n"
		<< '\n';

	print_rules = check(rules_check_text);
	if (print_rules) {
		rules();
	}
	else
		return;
	return;
}

void gameloop()
{
	bool play_again = true;
	vector<int> bulls_and_cows(2);
	string goal = "    ";
	string guess;

	while (play_again) {
		int diff_number = difficulty();
		int guesses = 0;
		bulls_and_cows = initialise(bulls_and_cows);
		goal = number_gen(goal, diff_number);

		cout << "\nI am thinking of 4 digits...try to guess them!\n";

		while (bulls_and_cows[0] != 4) {
			bulls_and_cows = initialise(bulls_and_cows);
			++guesses;
			
			cout << guesses << prompt;
			guess = get_guess(guess, guesses);
			if (guess == quit) {
				cout << quit_text
					<< "My number was: " << goal << '\n';
				break;
			}

			bulls_and_cows = compare(guess, goal, bulls_and_cows);
			cout << "[" << bulls_and_cows[0] << "] Bulls and [" << bulls_and_cows[1] << "] Cows\n";
			
			if (bulls_and_cows[0] == 4)
				win(goal, guesses);
		}
		play_again = check(play_again_text);
	}
	return;
}

int main()
{
	try {
		intro();
		gameloop();
		return 0;
	}
	catch (exception& e) {
		cerr << "runtime error: " << e.what() << '\n';
		keep_window_open("~1");
		return 1;
	}
	catch (...) {
		cerr << "unknown error\n";
		keep_window_open("~2");
		return 2;
	}
}
