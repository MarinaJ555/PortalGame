#include "curses_controller.hpp"

#include <curses.h>

CursesController::~CursesController()
{
}

Command CursesController::getInput()
{
    // TODO: Implement.
    // Don't forget about the arrow keys.

    // To prevent a compiler error in the starter files...
    return Command::Invalid;
}
