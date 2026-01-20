#include "globals.h"
#include <deque>

Color yellow = {255, 184, 35, 255};
Color darkGreen = {45, 79, 43, 255};
Color lightGreen = {167, 193, 168, 255};
Color beige = {255, 241, 202, 255};
Color red = {255, 63, 51, 255};
Color blue = {70, 130, 180, 255};
Color purple = {128, 0, 128, 255};
Color orange = {255, 165, 0, 255};
Color cyan = {0, 255, 255, 255};
Color pink = {255, 182, 193, 255};
Color white = {255, 255, 255, 255};
Color black = {0, 0, 0, 255};
Color gray = {128, 128, 128, 255};
Color darkYellow = {204, 147, 28, 255};
Color lightBlue = {173, 216, 230, 255};

GameState currentState = MENU;
GameState previousState = MENU;

int cellSize = 30;
int cellCount = 20;
int offset = 75;

const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 950;

double lastUpdateTime = 0;

Settings gameSettings;

Settings::Settings() {
    snakeColors[0] = darkGreen;
    snakeColors[1] = blue;
    snakeColors[2] = purple;
    snakeColors[3] = red;
    snakeColors[4] = orange;
    snakeColors[5] = cyan;
    
    backgroundColors[0] = yellow;
    backgroundColors[1] = lightBlue;
    backgroundColors[2] = pink;
    backgroundColors[3] = lightGreen;
    backgroundColors[4] = white;
}

Color Settings::GetSnakeColor() const {
    return snakeColors[snakeColorIndex];
}

Color Settings::GetBackgroundColor() const {
    return backgroundColors[backgroundColorIndex];
}

double Settings::GetGameSpeed() const {
    switch(difficulty) {
        case EASY: return 0.4;
        case NORMAL: return 0.25;
        case HARD: return 0.15;
        default: return 0.25;
    }
}

int Settings::GetCellCount() const {
    switch(gridSize) {
        case SMALL: return 15;
        case MEDIUM: return 20;
        case LARGE: return 25;
        default: return 20;
    }
}

const char* Settings::GetDifficultyName() const {
    switch(difficulty) {
        case EASY: return "Easy";
        case NORMAL: return "Normal";
        case HARD: return "Hard";
        default: return "Normal";
    }
}

const char* Settings::GetGridSizeName() const {
    switch(gridSize) {
        case SMALL: return "Small";
        case MEDIUM: return "Medium";
        case LARGE: return "Large";
        default: return "Medium";
    }
}

const char* Settings::GetControlsName() const {
    switch(controls) {
        case ARROW_KEYS: return "Arrow Keys";
        case WASD: return "WASD";
        default: return "Arrow Keys";
    }
}

int GetGameOffsetX() {
    int gameAreaWidth = cellSize * cellCount;
    return (WINDOW_WIDTH - gameAreaWidth) / 2;
}

int GetGameOffsetY() {
    int gameAreaHeight = cellSize * cellCount;
    return (WINDOW_HEIGHT - gameAreaHeight) / 2 + 20;
}

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 element, std::deque<Vector2>& deque) {
    for (size_t i = 0; i < deque.size(); i++) {
        if (deque[i].x == element.x && deque[i].y == element.y) {
            return true;
        }
    }
    return false;
}
