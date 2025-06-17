#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color yellow = {255 , 184 , 35, 255};
Color darkGreen = {45 , 79 , 43, 255};
Color lightGreen  = {167, 193, 168, 255};
Color beinge = {255, 241 , 202 , 255};
Color red = {255, 63, 51, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

GameState currentState = MENU;

struct Button 
{
    Rectangle button;
    const char * text;
    Color color;
    Color textColor;
    bool isHovered;

    Button(float x, float y, float width, float height, const char* buttonText, Color buttonColor, Color buttonTextColor) {
        button = {x, y, width, height};
        text = buttonText;
        color = buttonColor;
        textColor = buttonTextColor;
        isHovered = false;
    }
    
    void Update() {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, button);
    }
    
    void Draw() {
        Color drawColor = isHovered ? ColorBrightness(color, 0.6) : color;
        DrawRectangleRounded(button, 0.5, 6, drawColor);
        DrawRectangleRoundedLines(button, 0.5, 6, darkGreen);
        
        int textWidth = MeasureText(text, 30);
        DrawText(text, button.x + (button.width - textWidth) / 2, button.y + (button.height - 30) / 2, 30, textColor);
    }
    
    bool IsClicked() {
        return isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }
};

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

                Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize,(float)cellSize,(float)cellSize};
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
            DrawTexture(texture , offset + position.x * cellSize , offset + position.y * cellSize , WHITE);
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
    bool pause = false;
    int score = 0;

    void Draw()
    {
        apple.Draw();
        snake.Draw();
    }

    void Update()
    {
        if(running == true && pause == false)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], apple.position))
        {
            apple.position = apple.GenerateRandomPosition(snake.body);
            snake.addSegment = true;
            score++;
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
        score = 0;
        currentState = GAME_OVER;
    }

    void CheckCollisionWithTail()
    {
        deque <Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(ElementInDeque(snake.body[0],headlessBody))
        {
            GameOver();
        }
    }

    void Reset()
    {
        snake.Reset();
        apple.position = apple.GenerateRandomPosition(snake.body);
        running = true;
        pause = false;
        score = 0;
    }
};

void DrawMenu(Button& startButton, Button& exitButton) {
    ClearBackground(yellow);
    
    
    const char* title = "Snake , the snake game";
    DrawText(title, (2*offset + cellSize * cellCount - MeasureText(title, 60)) / 2, 200, 60, darkGreen);
    
    
    const char* subtitle = "Use arrow keys to move, SPACE to pause";
    DrawText(subtitle, (2*offset + cellSize * cellCount - MeasureText(subtitle, 20)) / 2, 280, 20, darkGreen);
    
    startButton.Update();
    exitButton.Update();
    
    startButton.Draw();
    exitButton.Draw();
}

void DrawGameOver(Button& restartButton, Button& menuButton) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
    
    const char* gameOverText = "Game Over !";
    DrawText(gameOverText, (GetScreenWidth() - MeasureText(gameOverText, 50)) / 2, 250, 50, beinge);
    
    restartButton.Update();
    menuButton.Update();
    
    restartButton.Draw();
    menuButton.Draw();
}



int main() {
    cout << "Game is starting !" << endl;

    int windowWidth = 2*offset + cellSize * cellCount;
    int windowHeight = 2*offset + cellSize * cellCount;
    
    InitWindow(windowWidth, windowHeight, "Snake Window");
    SetTargetFPS(120);
    SetExitKey(KEY_NULL);

    Game game = Game();
    
    Button startButton(windowWidth/2 - 100, 350, 200, 60, "START", beinge, darkGreen);
    Button exitButton(windowWidth/2 - 100, 430, 200, 60, "EXIT", beinge, darkGreen);
    
    Button restartButton(windowWidth/2 - 100, 350, 200, 60, "RESTART", beinge, darkGreen);
    Button menuButton(windowWidth/2 - 100, 430, 200, 60, "MENU", beinge, darkGreen);

    while(WindowShouldClose() == false)
    {
        BeginDrawing();

        switch(currentState) {
            case MENU:
                DrawMenu(startButton, exitButton);
                
                if(startButton.IsClicked()) {
                    currentState = PLAYING;
                    game.Reset();
                }
                if(exitButton.IsClicked()) {
                    CloseWindow();
                    return 0;
                }
                break;
                
            case PLAYING:
                if(eventTriggered(0.3))
                {
                    game.Update();
                }

                if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1 && game.pause == false)
                {
                    game.snake.direction = {0, -1};
                    game.running = true;
                }
                if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1 && game.pause == false)
                {
                    game.snake.direction = {0, 1};
                    game.running = true;
                }
                if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1 && game.pause == false)
                {
                    game.snake.direction = {-1, 0};
                    game.running = true;
                }
                if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1 && game.pause == false)
                {
                    game.snake.direction = {1, 0};
                    game.running = true;
                }
                if(IsKeyPressed(KEY_SPACE))
                {
                    game.pause = !game.pause;
                }
                if(IsKeyPressed(KEY_ESCAPE))
                {
                    currentState = MENU;
                }

                ClearBackground(yellow);

                DrawRectangleLinesEx(Rectangle{(float)offset - 5 , (float)offset - 5, (float)cellSize*cellCount + 10 , (float)cellSize*cellCount + 10} , 5 , darkGreen);

                DrawText("Snake , the snake game", offset - 5 , 20 , 40 , darkGreen);
                DrawText(TextFormat("Score: %i",game.score) , offset-5, offset + cellSize * cellCount + 10, 40 , darkGreen);

                if(game.pause)
                {
                    DrawText("PAUSED - Press SPACE to continue", 150, 375, 30, beinge);
                }
                
                game.Draw();
                break;
                
            case GAME_OVER:
                ClearBackground(yellow);
                DrawRectangleLinesEx(Rectangle{(float)offset - 5 , (float)offset - 5, (float)cellSize*cellCount + 10 , (float)cellSize*cellCount + 10} , 5 , darkGreen);
                DrawText("Snake , the snake game", offset - 5 , 20 , 40 , darkGreen);
                DrawText(TextFormat("Final Score: %i",game.score) , offset-5, offset + cellSize * cellCount + 10, 40 , darkGreen);
                game.Draw();
                
                DrawGameOver(restartButton, menuButton);
                
                if(restartButton.IsClicked()) {
                    currentState = PLAYING;
                    game.Reset();
                }
                if(menuButton.IsClicked()) {
                    currentState = MENU;
                    game.Reset();
                }
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}