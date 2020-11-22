//
// Created by Oscar on 21/11/2020.
//

#ifndef INFINIREVERSI_GRAPHIC_ENGINE_H
#define INFINIREVERSI_GRAPHIC_ENGINE_H


#include <raylib.h>
#include "reversi_game_engine.h"


typedef struct {
    Rectangle goBackButton, goFowardButton, saveGameButton;
} Menu;

typedef enum {
    MENU, GAME, SAVE, LOAD, EDITOR
} ScreenFlag;

typedef struct {
    char **directories;
    int NumberOfDirectories;
} DirectoryEntry;

typedef struct {
    bool collision;
    float difference;
    float offset;
} Slider;

typedef struct {
    int screenWidth;
    int screenHeight;
    float squareSize;

} ScreenFeatures;

void initSlider(Slider *slider);

void EditorScreen(ScreenFeatures *screenFeatures);

void initScreenFeatures(ScreenFeatures *features, int screenWidth, int screenHeight, float squareSize);

void UpdateDrawingState(Board *board, ScreenFeatures *screenFeatures);

void MenuScreen(ScreenFeatures *, int frameCount);

void DrawBoardGrid(Board *board, ScreenFeatures *screenFeatures);

void LoadFileScreen(Board *board, ScreenFeatures *screenFeatures, ScreenFlag *screen, Slider *slider);

Menu getMenu(Board board, ScreenFeatures *screenFeatures);

void DestroyDirectory(DirectoryEntry directory);

void CheckPiecePlayed(Board *board, ScreenFeatures *screenFeatures, int clicked, Vector2 mouse);

void CheckButtonPressed(Menu *menu, Board *board, ScreenFlag *screen, Vector2 mouse);

void PlayScreen(Board *board, Menu menu, ScreenFeatures *screenFeatures, ScreenFlag *screen,
                Vector2 mouse, int clicked);

void ShowFileSaverScreen(Board *board, ScreenFeatures *screenFeatures, char *filename, int frameCounter, Vector2 mouse,
                         ScreenFlag *screen, int *numOfChars);

#endif //INFINIREVERSI_GRAPHIC_ENGINE_H
