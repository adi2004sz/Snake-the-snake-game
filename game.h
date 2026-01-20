#pragma once
#include "raylib.h"
#include "raymath.h"
#include "globals.h"
#include <deque>

using namespace std;

class Snake {
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw() {
        int offsetX = GetGameOffsetX();
        int offsetY = GetGameOffsetY();
        Color snakeColor = gameSettings.GetSnakeColor();
        
        for (unsigned int i = 0; i < body.size(); i++) {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{(float)(offsetX + x * cellSize), 
                                          (float)(offsetY + y * cellSize),
                                          (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5f, 6, snakeColor);
            
            if (i == 0) {
                Color eyeColor = white;
                float eyeSize = cellSize * 0.15f;
                float eyeOffset = cellSize * 0.25f;
                
                if (direction.x == 1) {
                    DrawCircle((int)(segment.x + cellSize - eyeOffset), (int)(segment.y + eyeOffset), eyeSize, eyeColor);
                    DrawCircle((int)(segment.x + cellSize - eyeOffset), (int)(segment.y + cellSize - eyeOffset), eyeSize, eyeColor);
                } else if (direction.x == -1) {
                    DrawCircle((int)(segment.x + eyeOffset), (int)(segment.y + eyeOffset), eyeSize, eyeColor);
                    DrawCircle((int)(segment.x + eyeOffset), (int)(segment.y + cellSize - eyeOffset), eyeSize, eyeColor);
                } else if (direction.y == -1) {
                    DrawCircle((int)(segment.x + eyeOffset), (int)(segment.y + eyeOffset), eyeSize, eyeColor);
                    DrawCircle((int)(segment.x + cellSize - eyeOffset), (int)(segment.y + eyeOffset), eyeSize, eyeColor);
                } else {
                    DrawCircle((int)(segment.x + eyeOffset), (int)(segment.y + cellSize - eyeOffset), eyeSize, eyeColor);
                    DrawCircle((int)(segment.x + cellSize - eyeOffset), (int)(segment.y + cellSize - eyeOffset), eyeSize, eyeColor);
                }
            }
        }
    }

    void Update() {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment) {
            addSegment = false;
        } else {
            body.pop_back();
        }
    }

    void Reset() {
        int startX = cellCount / 4;
        int startY = cellCount / 2;
        body = {Vector2{(float)startX + 2, (float)startY}, 
                Vector2{(float)startX + 1, (float)startY}, 
                Vector2{(float)startX, (float)startY}};
        direction = {1, 0};
    }
};

class Apple {
public:
    Vector2 position;
    Texture2D texture;
    bool textureLoaded;

    Apple(deque<Vector2> snakeBody) {
        textureLoaded = false;
        position = GenerateRandomPosition(snakeBody);
    }

    void LoadTexture() {
        if (!textureLoaded) {
            Image image = LoadImage("Graphics/apple.png");
            if (image.data != nullptr) {
                ImageResize(&image, cellSize, cellSize);
                texture = LoadTextureFromImage(image);
                UnloadImage(image);
                textureLoaded = true;
            }
        }
    }

    ~Apple() {
        if (textureLoaded) {
            UnloadTexture(texture);
        }
    }

    void Draw() {
        int offsetX = GetGameOffsetX();
        int offsetY = GetGameOffsetY();
        
        if (textureLoaded) {
            DrawTexture(texture, 
                       (int)(offsetX + position.x * cellSize), 
                       (int)(offsetY + position.y * cellSize), 
                       WHITE);
        } else {
            DrawCircle((int)(offsetX + position.x * cellSize + cellSize/2),
                      (int)(offsetY + position.y * cellSize + cellSize/2),
                      cellSize/2 - 2, red);
        }
    }

    Vector2 GenerateRandomCell() {
        float x = (float)GetRandomValue(0, cellCount - 1);
        float y = (float)GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPosition(deque<Vector2> snakeBody) {
        Vector2 pos = GenerateRandomCell();
        while (ElementInDeque(pos, snakeBody)) {
            pos = GenerateRandomCell();
        }
        return pos;
    }
};

class Game {
public:
    Snake snake = Snake();
    Apple apple = Apple(snake.body);
    bool running = true;
    bool pause = false;
    int score = 0;
    int highScore = 0;

    void Draw() {
        apple.Draw();
        snake.Draw();
    }

    void Update() {
        if (running && !pause) {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood() {
        if (Vector2Equals(snake.body[0], apple.position)) {
            apple.position = apple.GenerateRandomPosition(snake.body);
            snake.addSegment = true;
            score++;
            if (score > highScore) {
                highScore = score;
            }
        }
    }

    void CheckCollisionWithEdges() {
        Vector2 head = snake.body[0];
        
        if (gameSettings.wallsEnabled) {
            if (head.x >= cellCount || head.x < 0 || head.y >= cellCount || head.y < 0) {
                GameOver();
            }
        } else {
            if (head.x >= cellCount) {
                snake.body[0].x = 0;
            } else if (head.x < 0) {
                snake.body[0].x = (float)(cellCount - 1);
            }
            if (head.y >= cellCount) {
                snake.body[0].y = 0;
            } else if (head.y < 0) {
                snake.body[0].y = (float)(cellCount - 1);
            }
        }
    }

    void GameOver() {
        running = false;
        currentState = GAME_OVER;
    }

    void CheckCollisionWithTail() {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody)) {
            GameOver();
        }
    }

    void Reset() {
        snake.Reset();
        apple.position = apple.GenerateRandomPosition(snake.body);
        running = true;
        pause = false;
        score = 0;
    }

    void ApplySettings() {
        cellCount = gameSettings.GetCellCount();
        snake.Reset();
        apple.position = apple.GenerateRandomPosition(snake.body);
    }
};
