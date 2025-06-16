#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color yellow = {255 , 184 , 35, 255};
Color darkGreen = {45 , 79 , 43, 255};
Color lightGreen  = {45 , 79, 43, 255};
Color beinge = {255, 241 , 202 , 255};
Color red = {255, 63, 51, 255};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element , deque <Vector2> deque)
{
    for(unsigned int i = 0 ; i < deque.size(); i++)
    {
        if(Vector2Equals(element,deque[i]))
        {
            return true;
        }
    }
    return false;
}


bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}


class Snake
{
    public :
        deque <Vector2> body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        Vector2 direction = {1,0};
        bool addSegment = false;

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

        void Update()
        {
            body.push_front(Vector2Add(body[0],direction));
            if(addSegment == true)
            {
                addSegment = false;
            }
            else
            {
                body.pop_back();
            }
        }

        void Reset()
        {
            body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
            direction = {1,0};            
        }
};


class Apple{

    public:
        Vector2 position;
        Texture2D texture;

        Apple(deque <Vector2> snakeBody)
        {
            Image image = LoadImage("Graphics/apple.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPosition(snakeBody);
        }

        ~Apple()
        {

            UnloadTexture(texture);
        }

        void Draw()
        {
            DrawTexture(texture , position.x * cellSize , position.y * cellSize , WHITE);
        }

        Vector2 GenerateRandomCell()
        {
            float x = GetRandomValue(0 , cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x,y};
        }

        Vector2 GenerateRandomPosition(deque <Vector2> snakeBody)
        {
            Vector2 position = GenerateRandomCell();
            while (ElementInDeque(position,snakeBody))
            {
                 position = GenerateRandomCell();
            }
            return position;
        }
};


class Game
{
    public:
    Snake snake = Snake();
    Apple apple = Apple(snake.body);
    bool running = true;

    void Draw()
    {
        apple.Draw();
        snake.Draw();
    }

    void Update()
    {
        if(running)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
        }
    }

    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], apple.position))
        {
            apple.position = apple.GenerateRandomPosition(snake.body);
            snake.addSegment = true;
        }
    }

    void CheckCollisionWithEdges()
    {
        if(snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        apple.position = apple.GenerateRandomPosition(snake.body);
        running = false;
    }
};

int main() {
    cout << "Game is starting !" << endl;
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake Window");
    SetTargetFPS(120);

    Game game = Game();

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        if(eventTriggered(0.3))
        {
            game.Update();
        }


        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        

        ClearBackground(yellow);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;

}