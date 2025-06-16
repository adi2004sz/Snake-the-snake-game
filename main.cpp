#include <iostream>
#include <raylib.h>
#include <deque>

using namespace std;

Color yellow = {255 , 184 , 35, 255};
Color darkGreen = {45 , 79 , 43, 255};
Color lightGreen  = {45 , 79, 43, 255};
Color beinge = {255, 241 , 202 , 255};
Color red = {255, 63, 51, 255};

int cellSize = 30;
int cellCount = 25; // 30 * 25 = 750 px

class Snake
{
    public :
        deque <Vector2> body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};

        void Draw()
        {
            for(unsigned int i = 0 ; i < body.size(); i++)
            {
                float x = body[i].x;
                float y = body[i].y;

                Rectangle segment = Rectangle{x * cellSize, y * cellSize,(float)cellSize,(float)cellSize};
                DrawRectangleRounded(segment, 0.5 , 6 , darkGreen);
            }
        }
};






class Apple{

    public:
        Vector2 position;
        Texture2D texture;

        Apple()
        {
            Image image = LoadImage("Graphics/apple.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPosition();
        }

        ~Apple()
        {

            UnloadTexture(texture);
        }

        void Draw()
        {
            DrawTexture(texture , position.x * cellSize , position.y * cellSize , WHITE);
        }

        Vector2 GenerateRandomPosition()
        {
            float x = GetRandomValue(0 , cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x,y};
        }
};

int main() {
    cout << "Game is starting !" << endl;
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake Window");
    SetTargetFPS(120);

    Apple apple = Apple();
    Snake snake = Snake();

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        ClearBackground(yellow);
        apple.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;

}