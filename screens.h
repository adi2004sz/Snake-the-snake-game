#pragma once
#include "raylib.h"
#include "globals.h"
#include "ui.h"
#include <cstdio>

void DrawTitle(const char* title, int y, int fontSize, Color color) {
    int textWidth = MeasureText(title, fontSize);
    DrawText(title, (WINDOW_WIDTH - textWidth) / 2, y, fontSize, color);
}

void DrawMenu(Button& startButton, Button& settingsButton, Button& exitButton, int highScore) {
    ClearBackground(gameSettings.GetBackgroundColor());
    
    DrawTitle("Snake, The Snake Game", 100, 80, darkGreen);
    
    const char* subtitle = "Use arrow keys to move, SPACE to pause";
    if (gameSettings.controls == WASD) {
        subtitle = "Use WASD to move, SPACE to pause";
    }
    DrawTitle(subtitle, 180, 24, darkGreen);
    
    char highScoreText[50];
    snprintf(highScoreText, sizeof(highScoreText), "High Score: %d", highScore);
    DrawTitle(highScoreText, 220, 28, darkGreen);
    
    startButton.Update();
    settingsButton.Update();
    exitButton.Update();
    
    startButton.Draw();
    settingsButton.Draw();
    exitButton.Draw();
    
    DrawTitle("Press ENTER to Start, ESC to Exit", WINDOW_HEIGHT - 60, 16, gray);
}

void DrawSettingsMenu(Button& backButton, Button& deleteHighScoreButton, SelectorButton& soundVolumeSelector, ToggleButton& wallsToggle,
                      SelectorButton& difficultySelector, SelectorButton& gridSelector,
                      SelectorButton& controlsSelector, ColorSelector& snakeColorSelector,
                      ColorSelector& bgColorSelector) {
    ClearBackground(gameSettings.GetBackgroundColor());
    
    DrawTitle("Settings", 40, 60, darkGreen);
    
    Rectangle panelBg = {(float)(WINDOW_WIDTH/2 - 300), 100, 600, 680};
    DrawRectangleRounded(panelBg, 0.05f, 6, Fade(white, 0.3f));
    DrawRectangleRoundedLines(panelBg, 0.05f, 6, darkGreen);
    
    soundVolumeSelector.Update();
    wallsToggle.Update();
    difficultySelector.Update();
    gridSelector.Update();
    controlsSelector.Update();
    snakeColorSelector.Update();
    bgColorSelector.Update();
    backButton.Update();
    deleteHighScoreButton.Update();
    
    int leftCol = WINDOW_WIDTH/2 - 260;
    int rightCol = WINDOW_WIDTH/2 + 20;
    
    DrawText("GAMEPLAY", leftCol, 130, 28, darkGreen);
    DrawLine(leftCol, 158, leftCol + 240, 158, darkGreen);
    
    DrawText("Difficulty", leftCol, 170, 20, darkGreen);
    difficultySelector.Draw();
    
    DrawText("Grid Size", leftCol, 245, 20, darkGreen);
    gridSelector.Draw();
    
    DrawText("Walls", leftCol, 320, 20, darkGreen);
    wallsToggle.Draw();
    
    DrawText("AUDIO & CONTROLS", rightCol, 130, 28, darkGreen);
    DrawLine(rightCol, 158, rightCol + 240, 158, darkGreen);
    
    DrawText("Sound", rightCol, 170, 20, darkGreen);
    soundVolumeSelector.Draw();
    
    DrawText("Controls", rightCol, 245, 20, darkGreen);
    controlsSelector.Draw();
    
    int customTextWidth = MeasureText("CUSTOMIZATION", 28);
    int customTextX = WINDOW_WIDTH/2 - customTextWidth/2;
    DrawText("CUSTOMIZATION", customTextX, 480, 28, darkGreen);
    DrawLine(WINDOW_WIDTH/2 - 140, 508, WINDOW_WIDTH/2 + 140, 508, darkGreen);
    
    DrawText("Snake Color", WINDOW_WIDTH/2 - 140, 520, 20, darkGreen);
    snakeColorSelector.Draw();
    
    DrawText("Background Color", WINDOW_WIDTH/2 - 140, 595, 20, darkGreen);
    bgColorSelector.Draw();
    
    DrawText("PREVIEW", WINDOW_WIDTH/2 - 50, 660, 20, darkGreen);
    
    Rectangle previewBg = {(float)(WINDOW_WIDTH/2 - 60), 685, 120, 60};
    DrawRectangleRounded(previewBg, 0.2f, 4, gameSettings.GetBackgroundColor());
    DrawRectangleRoundedLines(previewBg, 0.2f, 4, darkGreen);
    
    for (int i = 0; i < 3; i++) {
        Rectangle seg = {(float)(WINDOW_WIDTH/2 - 40 + i * 25), 700, 20, 20};
        DrawRectangleRounded(seg, 0.4f, 4, gameSettings.GetSnakeColor());
    }
    
    deleteHighScoreButton.Draw();
    backButton.Draw();
    
    DrawTitle("Press ESC or BACKSPACE to go back", WINDOW_HEIGHT - 40, 16, gray);
}

void DrawPauseOverlay(Button& resumeButton, Button& restartButton, Button& settingsButton, Button& menuButton) {
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.7f));
    
    Rectangle panel = {(float)(WINDOW_WIDTH/2 - 150), 200, 300, 400};
    DrawRectangleRounded(panel, 0.1f, 6, Fade(beige, 0.95f));
    DrawRectangleRoundedLines(panel, 0.1f, 6, darkGreen);
    
    DrawTitle("PAUSED", 230, 40, darkGreen);
    
    resumeButton.Update();
    restartButton.Update();
    settingsButton.Update();
    menuButton.Update();
    
    resumeButton.Draw();
    restartButton.Draw();
    settingsButton.Draw();
    menuButton.Draw();
    
    DrawTitle("Press SPACE to Resume", 560, 16, gray);
}

void DrawGameOver(Button& restartButton, Button& menuButton, int score, int highScore) {
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.7f));
    
    Rectangle panel = {(float)(WINDOW_WIDTH/2 - 180), 200, 360, 350};
    DrawRectangleRounded(panel, 0.1f, 6, Fade(beige, 0.95f));
    DrawRectangleRoundedLines(panel, 0.1f, 6, darkGreen);
    
    DrawTitle("GAME OVER", 230, 45, red);
    
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    DrawTitle(scoreText, 300, 30, darkGreen);
    
    char highScoreText[50];
    snprintf(highScoreText, sizeof(highScoreText), "High Score: %d", highScore);
    DrawTitle(highScoreText, 340, 24, gray);
    
    restartButton.Update();
    menuButton.Update();
    
    restartButton.Draw();
    menuButton.Draw();
    
    DrawTitle("Press R to Restart, ESC for Menu", 520, 16, gray);
}

void DrawGameUI(int score, int highScore, bool isPaused) {
    int offsetX = GetGameOffsetX();
    int offsetY = GetGameOffsetY();
    int gameWidth = cellSize * cellCount;
    int gameHeight = cellSize * cellCount;
    
    DrawRectangleLinesEx(Rectangle{(float)(offsetX - 5), (float)(offsetY - 5),
                                   (float)(gameWidth + 10), (float)(gameHeight + 10)}, 
                         5, darkGreen);
    
    const char* title = "Snake, The Snake Game";
    int titleWidth = MeasureText(title, 35);
    DrawText(title, (WINDOW_WIDTH - titleWidth) / 2, 20, 35, darkGreen);
    
    char scoreText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    DrawText(scoreText, offsetX, offsetY + gameHeight + 15, 30, darkGreen);
    
    char highScoreText[50];
    snprintf(highScoreText, sizeof(highScoreText), "High Score: %d", highScore);
    int hsWidth = MeasureText(highScoreText, 24);
    DrawText(highScoreText, offsetX + gameWidth - hsWidth, offsetY + gameHeight + 20, 24, gray);
    
    char diffText[30];
    snprintf(diffText, sizeof(diffText), "Difficulty: %s", gameSettings.GetDifficultyName());
    DrawText(diffText, 20, 60, 16, darkGreen);
    
    const char* wallText = gameSettings.wallsEnabled ? "Walls: ON" : "Walls: OFF";
    int wallWidth = MeasureText(wallText, 16);
    DrawText(wallText, WINDOW_WIDTH - wallWidth - 20, 60, 16, darkGreen);
    
    const char* controlHint = gameSettings.controls == ARROW_KEYS ? 
                              "Arrow Keys to move | SPACE: Pause | ESC: Menu" :
                              "WASD to move | SPACE: Pause | ESC: Menu";
    int hintWidth = MeasureText(controlHint, 14);
    DrawText(controlHint, (WINDOW_WIDTH - hintWidth) / 2, WINDOW_HEIGHT - 30, 14, gray);
}
