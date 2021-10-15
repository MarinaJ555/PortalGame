//Put main method here for debugging purposes
#include "game.hpp"
#include "interface_type.hpp"

int main(int argc, char *argv[])
{
	InterfaceType it = InterfaceType::Print;
	if (argc >= 2) it = InterfaceType::Curses;
	Game g{"game_files/demo_game1.txt", it};
	g.run();
}



