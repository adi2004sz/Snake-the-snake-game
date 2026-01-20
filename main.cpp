#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <string>

#include "globals.h"
#include "ui.h"
#include "game.h"
#include "screens.h"

using namespace std;

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake - The Snake Game");
    SetTargetFPS(120);
    SetExitKey(KEY_NULL);

    Game game = Game();
    game.apple.LoadTexture();
    
    cellCount = gameSettings.GetCellCount();
    game.ApplySettings();

    float menuBtnX = WINDOW_WIDTH/2 - 120;
    Button startButton(menuBtnX, 300, 240, 55, "START GAME", beige, darkGreen, 28);
    Button settingsButtonMenu(menuBtnX, 370, 240, 55, "SETTINGS", beige, darkGreen, 28);
    Button exitButton(menuBtnX, 440, 240, 55, "EXIT", beige, darkGreen, 28);

    Button restartButton(WINDOW_WIDTH/2 - 100, 400, 200, 50, "RESTART", beige, darkGreen, 24);
    Button menuButtonGO(WINDOW_WIDTH/2 - 100, 465, 200, 50, "MAIN MENU", beige, darkGreen, 24);

    Button resumeButton(WINDOW_WIDTH/2 - 100, 290, 200, 50, "RESUME", beige, darkGreen, 24);
    Button restartPauseButton(WINDOW_WIDTH/2 - 100, 355, 200, 50, "RESTART", beige, darkGreen, 24);
    Button settingsPauseButton(WINDOW_WIDTH/2 - 100, 420, 200, 50, "SETTINGS", beige, darkGreen, 24);
    Button menuPauseButton(WINDOW_WIDTH/2 - 100, 485, 200, 50, "MAIN MENU", beige, darkGreen, 24);

    Button backButton(WINDOW_WIDTH/2 - 100, 860, 200, 45, "BACK", beige, darkGreen, 24);
    
    ToggleButton soundToggle(WINDOW_WIDTH/2 + 40, 190, 80, 35, "Sound", &gameSettings.soundEnabled);
    ToggleButton wallsToggle(WINDOW_WIDTH/2 - 240, 340, 80, 35, "Walls (Die on collision)", &gameSettings.wallsEnabled);
    
    const char* difficultyOptions[] = {"Easy", "Normal", "Hard"};
    int difficultyIndex = (int)gameSettings.difficulty;
    SelectorButton difficultySelector(WINDOW_WIDTH/2 - 260, 190, 220, 40, "Difficulty",
                                      &difficultyIndex, 3, difficultyOptions);
    
    const char* gridOptions[] = {"Small (15x15)", "Medium (20x20)", "Large (25x25)"};
    int gridIndex = (int)gameSettings.gridSize;
    SelectorButton gridSelector(WINDOW_WIDTH/2 - 260, 265, 220, 40, "Grid Size",
                               &gridIndex, 3, gridOptions);
    
    const char* controlOptions[] = {"Arrow Keys", "WASD"};
    int controlIndex = (int)gameSettings.controls;
    SelectorButton controlsSelector(WINDOW_WIDTH/2 + 40, 265, 220, 40, "Controls",
                                   &controlIndex, 2, controlOptions);
    
    ColorSelector snakeColorSelector(WINDOW_WIDTH/2 - 140, 540, 280, 40, "Snake Color",
                                    &gameSettings.snakeColorIndex, gameSettings.snakeColors,
                                    gameSettings.snakeColorNames, 6);
    
    ColorSelector bgColorSelector(WINDOW_WIDTH/2 - 140, 615, 280, 40, "Background Color",
                                 &gameSettings.backgroundColorIndex, gameSettings.backgroundColors,
                                 gameSettings.backgroundColorNames, 5);

    while (!WindowShouldClose()) {
        BeginDrawing();

        switch (currentState) {
            case MENU: {
                DrawMenu(startButton, settingsButtonMenu, exitButton, game.highScore);
                
                if (startButton.IsClicked() || IsKeyPressed(KEY_ENTER)) {
                    currentState = PLAYING;
                    cellCount = gameSettings.GetCellCount();
                    game.ApplySettings();
                    game.Reset();
                }
                if (settingsButtonMenu.IsClicked()) {
                    previousState = MENU;
                    currentState = SETTINGS;
                }
                if (exitButton.IsClicked() || IsKeyPressed(KEY_ESCAPE)) {
                    CloseWindow();
                    return 0;
                }
                break;
            }

            case PLAYING: {
                if (eventTriggered(gameSettings.GetGameSpeed())) {
                    game.Update();
                }

                bool canMove = !game.pause;
                
                if (gameSettings.controls == ARROW_KEYS) {
                    if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1 && canMove) {
                        game.snake.direction = {0, -1};
                        game.running = true;
                    }
                    if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1 && canMove) {
                        game.snake.direction = {0, 1};
                        game.running = true;
                    }
                    if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1 && canMove) {
                        game.snake.direction = {-1, 0};
                        game.running = true;
                    }
                    if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1 && canMove) {
                        game.snake.direction = {1, 0};
                        game.running = true;
                    }
                } else {
                    if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1 && canMove) {
                        game.snake.direction = {0, -1};
                        game.running = true;
                    }
                    if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1 && canMove) {
                        game.snake.direction = {0, 1};
                        game.running = true;
                    }
                    if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1 && canMove) {
                        game.snake.direction = {-1, 0};
                        game.running = true;
                    }
                    if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1 && canMove) {
                        game.snake.direction = {1, 0};
                        game.running = true;
                    }
                }

                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) {
                    game.pause = !game.pause;
                    if (game.pause) {
                        currentState = PAUSED;
                    }
                }

                if (IsKeyPressed(KEY_R)) {
                    game.Reset();
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    game.pause = true;
                    currentState = PAUSED;
                }

                ClearBackground(gameSettings.GetBackgroundColor());
                DrawGameUI(game.score, game.highScore, game.pause);
                game.Draw();
                break;
            }

            case PAUSED: {
                ClearBackground(gameSettings.GetBackgroundColor());
                DrawGameUI(game.score, game.highScore, true);
                game.Draw();
                
                DrawPauseOverlay(resumeButton, restartPauseButton, settingsPauseButton, menuPauseButton);
                
                if (resumeButton.IsClicked() || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) {
                    game.pause = false;
                    currentState = PLAYING;
                }
                if (restartPauseButton.IsClicked() || IsKeyPressed(KEY_R)) {
                    game.Reset();
                    currentState = PLAYING;
                }
                if (settingsPauseButton.IsClicked()) {
                    previousState = PAUSED;
                    currentState = SETTINGS;
                }
                if (menuPauseButton.IsClicked() || IsKeyPressed(KEY_ESCAPE)) {
                    currentState = MENU;
                    game.Reset();
                }
                break;
            }

            case SETTINGS: {
                DrawSettingsMenu(backButton, soundToggle, wallsToggle, difficultySelector,
                               gridSelector, controlsSelector, snakeColorSelector, bgColorSelector);
                
                gameSettings.difficulty = (Difficulty)difficultyIndex;
                gameSettings.gridSize = (GridSize)gridIndex;
                gameSettings.controls = (ControlScheme)controlIndex;
                
                if (backButton.IsClicked() || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
                    if (cellCount != gameSettings.GetCellCount()) {
                        cellCount = gameSettings.GetCellCount();
                        game.ApplySettings();
                    }
                    currentState = previousState;
                    if (previousState == PAUSED) {
                        game.pause = true;
                    }
                }
                break;
            }

            case GAME_OVER: {
                ClearBackground(gameSettings.GetBackgroundColor());
                DrawGameUI(game.score, game.highScore, false);
                game.Draw();
                
                DrawGameOver(restartButton, menuButtonGO, game.score, game.highScore);
                
                if (restartButton.IsClicked() || IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)) {
                    game.Reset();
                    currentState = PLAYING;
                }
                if (menuButtonGO.IsClicked() || IsKeyPressed(KEY_ESCAPE)) {
                    currentState = MENU;
                    game.Reset();
                }
                break;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
