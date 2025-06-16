#include <iostream>
#include <raylib.h>

using namespace std;

Color yellow = {255 , 184 , 35, 255};
Color darkGreen = {45 , 79 , 43, 255};
Color lightGreen  = {45 , 79, 43, 255};
Color beinge = {255, 241 , 202 , 255};
Color red = {255, 63, 51, 255};

int cellSize = 30;
int cellCount = 25; // 30 * 25 = 750 px

class Apple{

    public:
        Vector2 position = {5,6};
        Texture2D texture;

        Apple()
        {
            Image image = LoadImage("Graphics/apple.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
        }

        ~Apple()
        {

            UnloadTexture(texture);
        }

        void Draw()
        {
            DrawTexture(texture , position.x * cellSize , position.y * cellSize , WHITE);
        }
};

int main() {
    cout << "Game is starting !" << endl;
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake Window");
    SetTargetFPS(120);

    Apple apple = Apple();

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        ClearBackground(yellow);
        apple.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;

}