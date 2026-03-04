#ifdef _WIN32
#include <windows.h>
#endif

#include "game.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    Game g;
    g.play();
    return 0;
}