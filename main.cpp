#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <string>

using namespace std;

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

GameState currentState = MENU;
GameState previousState = MENU;

struct Settings {
    Difficulty difficulty = NORMAL;
    bool soundEnabled = true;
    GridSize gridSize = MEDIUM;
    bool wallsEnabled = true;
    int snakeColorIndex = 0;
    int backgroundColorIndex = 0;
    ControlScheme controls = ARROW_KEYS;
    
    Color snakeColors[6] = {darkGreen, blue, purple, red, orange, cyan};
    const char* snakeColorNames[6] = {"Green", "Blue", "Purple", "Red", "Orange", "Cyan"};
    
    Color backgroundColors[5] = {yellow, lightBlue, pink, lightGreen, white};
    const char* backgroundColorNames[5] = {"Yellow", "Light Blue", "Pink", "Light Green", "White"};
    
    Color GetSnakeColor() const {
        return snakeColors[snakeColorIndex];
    }
    
    Color GetBackgroundColor() const {
        return backgroundColors[backgroundColorIndex];
    }
    
    double GetGameSpeed() const {
        switch(difficulty) {
            case EASY: return 0.4;
            case NORMAL: return 0.25;
            case HARD: return 0.15;
            default: return 0.25;
        }
    }
    
    int GetCellCount() const {
        switch(gridSize) {
            case SMALL: return 15;
            case MEDIUM: return 20;
            case LARGE: return 25;
            default: return 20;
        }
    }
    
    const char* GetDifficultyName() const {
        switch(difficulty) {
            case EASY: return "Easy";
            case NORMAL: return "Normal";
            case HARD: return "Hard";
            default: return "Normal";
        }
    }
    
    const char* GetGridSizeName() const {
        switch(gridSize) {
            case SMALL: return "Small";
            case MEDIUM: return "Medium";
            case LARGE: return "Large";
            default: return "Medium";
        }
    }
    
    const char* GetControlsName() const {
        switch(controls) {
            case ARROW_KEYS: return "Arrow Keys";
            case WASD: return "WASD";
            default: return "Arrow Keys";
        }
    }
};

Settings gameSettings;

int cellSize = 30;
int cellCount = 20;
int offset = 75;

const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 950;

double lastUpdateTime = 0;

struct Button {
    Rectangle bounds;
    const char* text;
    Color normalColor;
    Color hoverColor;
    Color textColor;
    Color borderColor;
    bool isHovered;
    bool isSelected;
    int fontSize;

    Button() {
        bounds = {0, 0, 0, 0};
        text = "";
        normalColor = beige;
        hoverColor = ColorBrightness(beige, 0.6f);
        textColor = darkGreen;
        borderColor = darkGreen;
        isHovered = false;
        isSelected = false;
        fontSize = 24;
    }

    Button(float x, float y, float width, float height, const char* buttonText, 
           Color btnColor = beige, Color txtColor = darkGreen, int fSize = 24) {
        bounds = {x, y, width, height};
        text = buttonText;
        normalColor = btnColor;
        hoverColor = ColorBrightness(btnColor, 0.6f);
        textColor = txtColor;
        borderColor = darkGreen;
        isHovered = false;
        isSelected = false;
        fontSize = fSize;
    }

    void SetPosition(float x, float y) {
        bounds.x = x;
        bounds.y = y;
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, bounds);
    }

    void Draw() {
        Color drawColor = isSelected ? ColorBrightness(normalColor, 0.4f) : 
                         (isHovered ? hoverColor : normalColor);
        
        DrawRectangleRounded(bounds, 0.3f, 6, drawColor);
        DrawRectangleRoundedLines(bounds, 0.3f, 6, borderColor);
        
        if (isSelected) {
            DrawRectangleRoundedLines(bounds, 0.3f, 6, red);
        }

        int textWidth = MeasureText(text, fontSize);
        float textX = bounds.x + (bounds.width - textWidth) / 2;
        float textY = bounds.y + (bounds.height - fontSize) / 2;
        DrawText(text, (int)textX, (int)textY, fontSize, textColor);
    }

    bool IsClicked() {
        return isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }
};

struct ToggleButton {
    Rectangle bounds;
    const char* labelText;
    bool* value;
    Color activeColor;
    Color inactiveColor;
    bool isHovered;

    ToggleButton() {
        bounds = {0, 0, 0, 0};
        labelText = "";
        value = nullptr;
        activeColor = darkGreen;
        inactiveColor = gray;
        isHovered = false;
    }

    ToggleButton(float x, float y, float width, float height, const char* label, bool* val) {
        bounds = {x, y, width, height};
        labelText = label;
        value = val;
        activeColor = darkGreen;
        inactiveColor = gray;
        isHovered = false;
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, bounds);
        
        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && value != nullptr) {
            *value = !(*value);
        }
    }

    void Draw() {
        DrawText(labelText, (int)bounds.x, (int)bounds.y - 25, 20, darkGreen);
        
        Color bgColor = (value != nullptr && *value) ? activeColor : inactiveColor;
        if (isHovered) bgColor = ColorBrightness(bgColor, 0.3f);
        
        DrawRectangleRounded(bounds, 0.5f, 6, bgColor);
        DrawRectangleRoundedLines(bounds, 0.5f, 6, darkGreen);
        
        float circleX = (value != nullptr && *value) ? 
                        bounds.x + bounds.width - bounds.height/2 - 5 : 
                        bounds.x + bounds.height/2 + 5;
        float circleY = bounds.y + bounds.height/2;
        DrawCircle((int)circleX, (int)circleY, bounds.height/2 - 8, white);
        
        const char* stateText = (value != nullptr && *value) ? "ON" : "OFF";
        DrawText(stateText, (int)(bounds.x + bounds.width + 15), (int)(bounds.y + 8), 20, darkGreen);
    }
};

struct SelectorButton {
    Rectangle bounds;
    const char* labelText;
    int* currentIndex;
    int maxIndex;
    const char** options;
    bool leftHovered;
    bool rightHovered;

    SelectorButton() {
        bounds = {0, 0, 0, 0};
        labelText = "";
        currentIndex = nullptr;
        maxIndex = 0;
        options = nullptr;
        leftHovered = false;
        rightHovered = false;
    }

    SelectorButton(float x, float y, float width, float height, const char* label, 
                   int* index, int max, const char** opts) {
        bounds = {x, y, width, height};
        labelText = label;
        currentIndex = index;
        maxIndex = max;
        options = opts;
        leftHovered = false;
        rightHovered = false;
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        
        Rectangle leftArrow = {bounds.x, bounds.y, 40, bounds.height};
        Rectangle rightArrow = {bounds.x + bounds.width - 40, bounds.y, 40, bounds.height};
        
        leftHovered = CheckCollisionPointRec(mousePos, leftArrow);
        rightHovered = CheckCollisionPointRec(mousePos, rightArrow);
        
        if (currentIndex != nullptr) {
            if (leftHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                *currentIndex = (*currentIndex - 1 + maxIndex) % maxIndex;
            }
            if (rightHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                *currentIndex = (*currentIndex + 1) % maxIndex;
            }
        }
    }

    void Draw() {
        DrawText(labelText, (int)bounds.x, (int)bounds.y - 25, 20, darkGreen);
        
        DrawRectangleRounded(bounds, 0.3f, 6, beige);
        DrawRectangleRoundedLines(bounds, 0.3f, 6, darkGreen);
        
        Color leftColor = leftHovered ? darkGreen : gray;
        DrawText("<", (int)(bounds.x + 12), (int)(bounds.y + bounds.height/2 - 12), 24, leftColor);
        
        Color rightColor = rightHovered ? darkGreen : gray;
        DrawText(">", (int)(bounds.x + bounds.width - 25), (int)(bounds.y + bounds.height/2 - 12), 24, rightColor);
        
        if (options != nullptr && currentIndex != nullptr) {
            const char* optText = options[*currentIndex];
            int textWidth = MeasureText(optText, 20);
            DrawText(optText, (int)(bounds.x + (bounds.width - textWidth) / 2), 
                    (int)(bounds.y + bounds.height/2 - 10), 20, darkGreen);
        }
    }
};

struct ColorSelector {
    Rectangle bounds;
    const char* labelText;
    int* currentIndex;
    Color* colors;
    const char** colorNames;
    int colorCount;
    bool leftHovered;
    bool rightHovered;

    ColorSelector() {
        bounds = {0, 0, 0, 0};
        labelText = "";
        currentIndex = nullptr;
        colors = nullptr;
        colorNames = nullptr;
        colorCount = 0;
        leftHovered = false;
        rightHovered = false;
    }

    ColorSelector(float x, float y, float width, float height, const char* label,
                  int* index, Color* cols, const char** names, int count) {
        bounds = {x, y, width, height};
        labelText = label;
        currentIndex = index;
        colors = cols;
        colorNames = names;
        colorCount = count;
        leftHovered = false;
        rightHovered = false;
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        
        Rectangle leftArrow = {bounds.x, bounds.y, 40, bounds.height};
        Rectangle rightArrow = {bounds.x + bounds.width - 40, bounds.y, 40, bounds.height};
        
        leftHovered = CheckCollisionPointRec(mousePos, leftArrow);
        rightHovered = CheckCollisionPointRec(mousePos, rightArrow);
        
        if (currentIndex != nullptr) {
            if (leftHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                *currentIndex = (*currentIndex - 1 + colorCount) % colorCount;
            }
            if (rightHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                *currentIndex = (*currentIndex + 1) % colorCount;
            }
        }
    }

    void Draw() {
        DrawText(labelText, (int)bounds.x, (int)bounds.y - 25, 20, darkGreen);
        
        DrawRectangleRounded(bounds, 0.3f, 6, beige);
        DrawRectangleRoundedLines(bounds, 0.3f, 6, darkGreen);
        
        Color leftColor = leftHovered ? darkGreen : gray;
        DrawText("<", (int)(bounds.x + 12), (int)(bounds.y + bounds.height/2 - 12), 24, leftColor);
        
        Color rightColor = rightHovered ? darkGreen : gray;
        DrawText(">", (int)(bounds.x + bounds.width - 25), (int)(bounds.y + bounds.height/2 - 12), 24, rightColor);
        
        if (colors != nullptr && currentIndex != nullptr) {
            Rectangle colorBox = {bounds.x + 50, bounds.y + 8, 30, bounds.height - 16};
            DrawRectangleRounded(colorBox, 0.3f, 4, colors[*currentIndex]);
            DrawRectangleRoundedLines(colorBox, 0.3f, 4, darkGreen);
            
            const char* name = colorNames[*currentIndex];
            int textWidth = MeasureText(name, 18);
            DrawText(name, (int)(bounds.x + 90 + (bounds.width - 140 - textWidth) / 2),
                    (int)(bounds.y + bounds.height/2 - 9), 18, darkGreen);
        }
    }
};

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for (unsigned int i = 0; i < deque.size(); i++) {
        if (Vector2Equals(element, deque[i])) {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int GetGameOffsetX() {
    int gameAreaWidth = cellSize * cellCount;
    return (WINDOW_WIDTH - gameAreaWidth) / 2;
}

int GetGameOffsetY() {
    int gameAreaHeight = cellSize * cellCount;
    return (WINDOW_HEIGHT - gameAreaHeight) / 2 + 20;
}

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

void DrawTitle(const char* title, int y, int fontSize, Color color) {
    int textWidth = MeasureText(title, fontSize);
    DrawText(title, (WINDOW_WIDTH - textWidth) / 2, y, fontSize, color);
}

void DrawMenu(Button& startButton, Button& settingsButton, Button& exitButton) {
    ClearBackground(gameSettings.GetBackgroundColor());
    
    DrawTitle("Snake, The Snake Game", 100, 60, darkGreen);
    
    const char* subtitle = "Use arrow keys to move, SPACE to pause";
    if (gameSettings.controls == WASD) {
        subtitle = "Use WASD to move, SPACE to pause";
    }
    DrawTitle(subtitle, 180, 20, darkGreen);
    
    char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", 0);
    DrawTitle(highScoreText, 220, 24, darkGreen);
    
    startButton.Update();
    settingsButton.Update();
    exitButton.Update();
    
    startButton.Draw();
    settingsButton.Draw();
    exitButton.Draw();
    
    DrawTitle("Press ENTER to Start, ESC to Exit", WINDOW_HEIGHT - 60, 16, gray);
}

void DrawSettingsMenu(Button& backButton, ToggleButton& soundToggle, ToggleButton& wallsToggle,
                      SelectorButton& difficultySelector, SelectorButton& gridSelector,
                      SelectorButton& controlsSelector, ColorSelector& snakeColorSelector,
                      ColorSelector& bgColorSelector) {
    ClearBackground(gameSettings.GetBackgroundColor());
    
    DrawTitle("Settings", 40, 50, darkGreen);
    
    Rectangle panelBg = {(float)(WINDOW_WIDTH/2 - 300), 100, 600, 680};
    DrawRectangleRounded(panelBg, 0.05f, 6, Fade(white, 0.3f));
    DrawRectangleRoundedLines(panelBg, 0.05f, 6, darkGreen);
    
    soundToggle.Update();
    wallsToggle.Update();
    difficultySelector.Update();
    gridSelector.Update();
    controlsSelector.Update();
    snakeColorSelector.Update();
    bgColorSelector.Update();
    backButton.Update();
    
    int leftCol = WINDOW_WIDTH/2 - 260;
    int rightCol = WINDOW_WIDTH/2 + 20;
    
    DrawText("GAMEPLAY", leftCol, 130, 24, darkGreen);
    DrawLine(leftCol, 158, leftCol + 240, 158, darkGreen);
    
    difficultySelector.Draw();
    gridSelector.Draw();
    wallsToggle.Draw();
    
    DrawText("AUDIO & CONTROLS", rightCol, 130, 24, darkGreen);
    DrawLine(rightCol, 158, rightCol + 240, 158, darkGreen);
    
    soundToggle.Draw();
    controlsSelector.Draw();
    
    DrawText("CUSTOMIZATION", WINDOW_WIDTH/2 - 80, 480, 24, darkGreen);
    DrawLine(WINDOW_WIDTH/2 - 120, 508, WINDOW_WIDTH/2 + 120, 508, darkGreen);
    
    snakeColorSelector.Draw();
    bgColorSelector.Draw();
    
    backButton.Draw();
    
    Rectangle previewBg = {(float)(WINDOW_WIDTH/2 - 60), 720, 120, 60};
    DrawRectangleRounded(previewBg, 0.2f, 4, gameSettings.GetBackgroundColor());
    DrawRectangleRoundedLines(previewBg, 0.2f, 4, darkGreen);
    
    for (int i = 0; i < 3; i++) {
        Rectangle seg = {(float)(WINDOW_WIDTH/2 - 40 + i * 25), 735, 20, 20};
        DrawRectangleRounded(seg, 0.4f, 4, gameSettings.GetSnakeColor());
    }
    
    DrawText("Preview", WINDOW_WIDTH/2 - 30, 788, 18, darkGreen);
    
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
    sprintf(scoreText, "Score: %d", score);
    DrawTitle(scoreText, 300, 30, darkGreen);
    
    char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", highScore);
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
    sprintf(scoreText, "Score: %d", score);
    DrawText(scoreText, offsetX, offsetY + gameHeight + 15, 30, darkGreen);
    
    char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", highScore);
    int hsWidth = MeasureText(highScoreText, 24);
    DrawText(highScoreText, offsetX + gameWidth - hsWidth, offsetY + gameHeight + 20, 24, gray);
    
    char diffText[30];
    sprintf(diffText, "Difficulty: %s", gameSettings.GetDifficultyName());
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
    
    ToggleButton soundToggle(WINDOW_WIDTH/2 + 40, 195, 80, 35, "Sound", &gameSettings.soundEnabled);
    ToggleButton wallsToggle(WINDOW_WIDTH/2 - 240, 385, 80, 35, "Walls (Die on collision)", &gameSettings.wallsEnabled);
    
    const char* difficultyOptions[] = {"Easy", "Normal", "Hard"};
    int difficultyIndex = (int)gameSettings.difficulty;
    SelectorButton difficultySelector(WINDOW_WIDTH/2 - 260, 195, 220, 40, "Difficulty",
                                      &difficultyIndex, 3, difficultyOptions);
    
    const char* gridOptions[] = {"Small (15x15)", "Medium (20x20)", "Large (25x25)"};
    int gridIndex = (int)gameSettings.gridSize;
    SelectorButton gridSelector(WINDOW_WIDTH/2 - 260, 290, 220, 40, "Grid Size",
                               &gridIndex, 3, gridOptions);
    
    const char* controlOptions[] = {"Arrow Keys", "WASD"};
    int controlIndex = (int)gameSettings.controls;
    SelectorButton controlsSelector(WINDOW_WIDTH/2 + 40, 290, 220, 40, "Controls",
                                   &controlIndex, 2, controlOptions);
    
    ColorSelector snakeColorSelector(WINDOW_WIDTH/2 - 140, 550, 280, 40, "Snake Color",
                                    &gameSettings.snakeColorIndex, gameSettings.snakeColors,
                                    gameSettings.snakeColorNames, 6);
    
    ColorSelector bgColorSelector(WINDOW_WIDTH/2 - 140, 620, 280, 40, "Background Color",
                                 &gameSettings.backgroundColorIndex, gameSettings.backgroundColors,
                                 gameSettings.backgroundColorNames, 5);

    while (!WindowShouldClose()) {
        BeginDrawing();

        switch (currentState) {
            case MENU: {
                DrawMenu(startButton, settingsButtonMenu, exitButton);
                
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
