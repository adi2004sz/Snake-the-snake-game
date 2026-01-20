#pragma once
#include "raylib.h"
#include "globals.h"
#include <functional>

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
    const char* label;
    bool* value;
    Color activeColor;
    Color inactiveColor;
    bool isHovered;
    int fontSize;

    ToggleButton(float x, float y, float width, float height, const char* lbl, bool* val) {
        bounds = {x, y, width, height};
        label = lbl;
        value = val;
        activeColor = {45, 150, 45, 255};
        inactiveColor = gray;
        isHovered = false;
        fontSize = 16;
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, bounds);
        
        if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *value = !(*value);
        }
    }

    void Draw() {
        Color bgColor = *value ? activeColor : inactiveColor;
        if (isHovered) {
            bgColor = ColorBrightness(bgColor, 0.3f);
        }
        
        DrawRectangleRounded(bounds, 0.3f, 6, bgColor);
        DrawRectangleRoundedLines(bounds, 0.3f, 6, darkGreen);
        
        const char* stateText = *value ? "ON" : "OFF";
        int textWidth = MeasureText(stateText, fontSize);
        float textX = bounds.x + (bounds.width - textWidth) / 2;
        float textY = bounds.y + (bounds.height - fontSize) / 2;
        DrawText(stateText, (int)textX, (int)textY, fontSize, WHITE);
    }
};

struct SelectorButton {
    Rectangle bounds;
    const char* label;
    int* currentIndex;
    int optionCount;
    const char** options;
    bool leftHovered;
    bool rightHovered;
    int fontSize;

    SelectorButton(float x, float y, float width, float height, const char* lbl,
                  int* idx, int count, const char** opts) {
        bounds = {x, y, width, height};
        label = lbl;
        currentIndex = idx;
        optionCount = count;
        options = opts;
        leftHovered = false;
        rightHovered = false;
        fontSize = 16;
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        
        Rectangle leftArrow = {bounds.x + 5, bounds.y + bounds.height/2 - 10, 20, 20};
        Rectangle rightArrow = {bounds.x + bounds.width - 25, bounds.y + bounds.height/2 - 10, 20, 20};
        
        leftHovered = CheckCollisionPointRec(mousePos, leftArrow);
        rightHovered = CheckCollisionPointRec(mousePos, rightArrow);
        
        if (leftHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            (*currentIndex)--;
            if (*currentIndex < 0) *currentIndex = optionCount - 1;
        }
        if (rightHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            (*currentIndex)++;
            if (*currentIndex >= optionCount) *currentIndex = 0;
        }
    }

    void Draw() {
        DrawRectangleRounded(bounds, 0.2f, 6, beige);
        DrawRectangleRoundedLines(bounds, 0.2f, 6, darkGreen);
        
        Color leftColor = leftHovered ? darkGreen : gray;
        Color rightColor = rightHovered ? darkGreen : gray;
        
        DrawText("<", (int)(bounds.x + 10), (int)(bounds.y + bounds.height/2 - 8), 18, leftColor);
        DrawText(">", (int)(bounds.x + bounds.width - 22), (int)(bounds.y + bounds.height/2 - 8), 18, rightColor);
        
        const char* currentOption = options[*currentIndex];
        int textWidth = MeasureText(currentOption, fontSize);
        float textX = bounds.x + (bounds.width - textWidth) / 2;
        float textY = bounds.y + (bounds.height - fontSize) / 2;
        DrawText(currentOption, (int)textX, (int)textY, fontSize, darkGreen);
    }
};

struct ColorSelector {
    Rectangle bounds;
    const char* label;
    int* currentIndex;
    Color* colors;
    const char** colorNames;
    int colorCount;
    bool leftHovered;
    bool rightHovered;
    int fontSize;

    ColorSelector(float x, float y, float width, float height, const char* lbl,
                 int* idx, Color* cols, const char** names, int count) {
        bounds = {x, y, width, height};
        label = lbl;
        currentIndex = idx;
        colors = cols;
        colorNames = names;
        colorCount = count;
        leftHovered = false;
        rightHovered = false;
        fontSize = 16;
    }

    void Update() {
        Vector2 mousePos = GetMousePosition();
        
        Rectangle leftArrow = {bounds.x + 5, bounds.y + bounds.height/2 - 10, 20, 20};
        Rectangle rightArrow = {bounds.x + bounds.width - 25, bounds.y + bounds.height/2 - 10, 20, 20};
        
        leftHovered = CheckCollisionPointRec(mousePos, leftArrow);
        rightHovered = CheckCollisionPointRec(mousePos, rightArrow);
        
        if (leftHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            (*currentIndex)--;
            if (*currentIndex < 0) *currentIndex = colorCount - 1;
        }
        if (rightHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            (*currentIndex)++;
            if (*currentIndex >= colorCount) *currentIndex = 0;
        }
    }

    void Draw() {
        DrawRectangleRounded(bounds, 0.2f, 6, beige);
        DrawRectangleRoundedLines(bounds, 0.2f, 6, darkGreen);
        
        Color leftColor = leftHovered ? darkGreen : gray;
        Color rightColor = rightHovered ? darkGreen : gray;
        
        DrawText("<", (int)(bounds.x + 10), (int)(bounds.y + bounds.height/2 - 8), 18, leftColor);
        DrawText(">", (int)(bounds.x + bounds.width - 22), (int)(bounds.y + bounds.height/2 - 8), 18, rightColor);
        
        Rectangle colorPreview = {bounds.x + bounds.width - 60, bounds.y + 5, bounds.height - 10, bounds.height - 10};
        DrawRectangleRounded(colorPreview, 0.2f, 4, colors[*currentIndex]);
        DrawRectangleRoundedLines(colorPreview, 0.2f, 4, darkGreen);
        
        const char* name = colorNames[*currentIndex];
        int textWidth = MeasureText(name, fontSize);
        float textX = bounds.x + (bounds.width - textWidth) / 2 - 15;
        float textY = bounds.y + (bounds.height - fontSize) / 2;
        DrawText(name, (int)textX, (int)textY, fontSize, darkGreen);
    }
};
