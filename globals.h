#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>
#include <deque>

extern Color yellow;
extern Color darkGreen;
extern Color lightGreen;
extern Color beige;
extern Color red;
extern Color blue;
extern Color purple;
extern Color orange;
extern Color cyan;
extern Color pink;
extern Color white;
extern Color black;
extern Color gray;
extern Color darkYellow;
extern Color lightBlue;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    SETTINGS,
    PAUSED
};

enum Difficulty {
    EASY,
    NORMAL,
    HARD
};

enum GridSize {
    SMALL,
    MEDIUM,
    LARGE
};

enum ControlScheme {
    ARROW_KEYS,
    WASD
};

extern GameState currentState;
extern GameState previousState;

extern int cellSize;
extern int cellCount;
extern int offset;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern double lastUpdateTime;

struct Settings {
    Difficulty difficulty = NORMAL;
    int soundVolumeIndex = 4;
    GridSize gridSize = MEDIUM;
    bool wallsEnabled = true;
    int snakeColorIndex = 0;
    int backgroundColorIndex = 0;
    ControlScheme controls = ARROW_KEYS;
    
    Color snakeColors[6];
    const char* snakeColorNames[6] = {"Green", "Blue", "Purple", "Red", "Orange", "Cyan"};
    
    Color backgroundColors[5];
    const char* backgroundColorNames[5] = {"Yellow", "Light Blue", "Pink", "Light Green", "White"};
    
    Settings();
    
    Color GetSnakeColor() const;
    Color GetBackgroundColor() const;
    double GetGameSpeed() const;
    int GetCellCount() const;
    const char* GetDifficultyName() const;
    const char* GetGridSizeName() const;
    const char* GetControlsName() const;
};

extern Settings gameSettings;

int GetGameOffsetX();
int GetGameOffsetY();
bool eventTriggered(double interval);
bool ElementInDeque(Vector2 element, std::deque<Vector2>& deque);

int LoadHighScore();
void SaveHighScore(int score);
void DeleteHighScore();

#endif
