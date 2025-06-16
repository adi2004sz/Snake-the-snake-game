#include <iostream>
#include <raylib.h>

using namespace std;

Color yellow = {255 , 184 , 35, 255};
Color darkGreen = {45 , 79 , 43, 255};
Color lightGreen  = {45 , 79, 43, 255};

int main() {
    cout << "Game is starting !" << endl;
    InitWindow(750,750,"Snake Window");
    SetTargetFPS(120);

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        ClearBackground(yellow);

        EndDrawing();
    }

    CloseWindow();
    return 0;

}