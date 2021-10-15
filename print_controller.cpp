#include "print_controller.hpp"

#include <iostream>
using namespace std;

PrintController::~PrintController()
{
}

Command PrintController::getInput()
{
    // TODO: Implement.
	// print controller only uses wasd keys for input
	cout << "Enter input: ";
	char move;
	cin >> move;

  if(move == 'w'){ //move forward in direction the user is pointing
    return Command::Forward;
	}
	else if(move == 'a'){ //rotate the player left
    return Command::Left;
	}
	else if(move == 'd'){ //rotate the player right
    return Command::Right;
	}
	else if(move == 's'){ //do nothing
    return Command::Nothing;
	}
	else if(move == 'q'){ //quit the game
    return Command::Quit;
	}

    // To prevent a compiler error in the starter files...
    return Command::Invalid;
}
