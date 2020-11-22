

#ifndef INFINIREVERSI_GRAPHIC_ENGINE_H
#define INFINIREVERSI_GRAPHIC_ENGINE_H


#include <raylib.h>
#include "reversi_game_engine.h"


typedef struct {
    Rectangle goBackButton, goFowardButton, saveGameButton;
} Menu;

typedef struct {
    Rectangle startGameButton, loadGameButton, editorButton;
} MenuOptions;

typedef enum {
    MENU, GAME, SAVE, LOAD, EDITOR, CONFIG_EDITOR, CONFIG_GAME
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

void EditorScreen(ScreenFeatures *screenFeatures, Board *board, Piece *piece, ScreenFlag *);

void initScreenFeatures(ScreenFeatures *features, int screenWidth, int screenHeight, float squareSize);

void UpdateDrawingState(Board *board, ScreenFeatures *screenFeatures);

void MenuScreen(ScreenFeatures *, int frameCount, MenuOptions, ScreenFlag *, Board *);

void DrawBoardGrid(Board *board, ScreenFeatures *screenFeatures);

void LoadFileScreen(Board *board, ScreenFeatures *screenFeatures, ScreenFlag *screen, Slider *slider);

Menu getMenu(Board board, ScreenFeatures *screenFeatures);

MenuOptions getMenuOptions(ScreenFeatures *screenFeatures);

void DestroyDirectory(DirectoryEntry directory);

void CheckPiecePlayed(Board *board, ScreenFeatures *screenFeatures, int clicked, Vector2 mouse);

void CheckButtonPressed(Menu *menu, Board *board, ScreenFlag *screen, Vector2 mouse);

void PlayScreen(Board *board, Menu menu, ScreenFeatures *screenFeatures, ScreenFlag *screen,
                Vector2 mouse, int clicked);

void ShowFileSaverScreen(Board *board, ScreenFeatures *screenFeatures, char *filename, int frameCounter, Vector2 mouse,
                         ScreenFlag *screen, int *numOfChars, ScreenFlag *);

void CheckMenuButtonPressed(MenuOptions, ScreenFlag *, Board *);

void ConfigEditorScreen(ScreenFeatures *screenFeatures, Board *board, ScreenFlag *screen, int*);

void ConfigGameScreen(ScreenFeatures *screenFeatures, Board *board, ScreenFlag *screen, int *customBoardSize,
                      Difficulty *difficulty);

#endif //INFINIREVERSI_GRAPHIC_ENGINE_H
