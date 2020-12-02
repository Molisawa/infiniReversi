#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include <dirent.h>
#include <stdlib.h>
#include <math.h>
#include "graphic_engine.h"

/**
 * Checks if the button is pressed
 * @param menu receives a Menu type structure
 * @param board receives a Board type structure
 * @param screen receives a ScreenFlag type structure
 * @param mouse receives a Vector type structure from raylib
 */
void CheckButtonPressed(Menu *menu, Board *board, ScreenFlag *screen, Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, menu->goBackButton)) {
        goBack(board);
    } else if (CheckCollisionPointRec(mouse, menu->goFowardButton)) {
        goForward(board);
    } else if (CheckCollisionPointRec(mouse, menu->saveGameButton)) {
        *screen = SAVE;
    }
}

/**
 * Gets Directory entries
 * @return
 */
DirectoryEntry getDirectories() {
    DIR *d;
    struct dirent *dir;
    d = opendir("saved/");

    int count = 0;
    char **dirs = malloc(sizeof(char *));

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            int nameLen = TextLength(dir->d_name);
            char *tmp = calloc(nameLen + 1, sizeof(char));
            for (int i = 0; i < nameLen; i++) {
                tmp[i] = dir->d_name[i];

            }
            tmp[nameLen] = '\0';
            if (IsFileExtension(tmp, ".brd")) {
                count++;
                dirs = realloc(dirs, count * sizeof(char *));
                dirs[count - 1] = tmp;
            }
        }
        closedir(d);
    }
    return (DirectoryEntry) {dirs, count};

}

/**
 * Destroys Directories
 * @param directory receives a DirectoryEntry type structure
 */
void DestroyDirectory(DirectoryEntry directory) {
    for (int i = 0; i < directory.NumberOfDirectories; i++) {
        free(directory.directories[i]);
    }
    free(directory.directories);
}

/**
 * Check which piece is being played
 * @param board receives a Board type structure
 * @param screenFeatures receives a ScreenFeautres type structure
 * @param clicked receives integer if piece is clicked
 * @param mouse receives a Vector type structure from raylib
 */
void CheckPiecePlayed(Board *board, ScreenFeatures *screenFeatures, int clicked, Vector2 mouse) {
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            Vector2 vector;
            switch (board->state[i][j].pieceType) {
                case HELPER: {
                    vector = (Vector2) {(i) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                                        (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2};
                    if (CheckCollisionPointCircle(mouse, vector, screenFeatures->squareSize / 2 - 5)) {
                        if (!clicked) {
                            DrawRectangle((i) * screenFeatures->squareSize + 1, (j) * screenFeatures->squareSize + 1,
                                          screenFeatures->squareSize - 2,
                                          screenFeatures->squareSize - 2, DARKGREEN);
                            DrawCircle((i) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                                       (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                                       screenFeatures->squareSize / 2 - 5, Fade(BLACK, 0.4f));
                        } else {
                            Movement m = {.pieceType = BLACK_PLAYER, .x = i, .y = j};
                            makeRealMove(board, m);
                            removeHistoryFoward(board);
                            UpdateDrawingState(board, screenFeatures);
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * Draws the game board grid
 * @param board receives a Board type structure
 * @param screenFeatures receives a ScreenFeatures type structure
 */
void DrawBoardGrid(Board *board, ScreenFeatures *screenFeatures) {
    for (int i = 0; i < board->size + 1; i++) {
        DrawLineV((Vector2) {screenFeatures->squareSize * i, 0},
                  (Vector2) {screenFeatures->squareSize * i, board->size * screenFeatures->squareSize}, BLACK);
    }

    for (int i = 0; i < board->size + 1; i++) {
        DrawLineV((Vector2) {0, screenFeatures->squareSize * i},
                  (Vector2) {board->size * screenFeatures->squareSize, screenFeatures->squareSize * i}, BLACK);
    }
    DrawRectangle(board->size * screenFeatures->squareSize + 1, 0, screenFeatures->screenWidth - 1,
                  screenFeatures->screenHeight, WHITE);
    DrawRectangle(board->size * screenFeatures->squareSize + 1, 0, screenFeatures->screenWidth - 1,
                  screenFeatures->screenHeight, Fade(DARKGREEN, 0.5f));
}

/**
 * Plays the game screen
 * @param board receives a Board type structure
 * @param menu receives a Menu type structure
 * @param screenFeatures receives a ScreenFeatures type structure
 * @param screen receives a ScreenFlag type structure
 * @param mouse receives a Vector type structure from raylib
 * @param clicked receives an integer if mouse is clicked
 */
void PlayScreen(Board *board, Menu menu, ScreenFeatures *screenFeatures, ScreenFlag *screen,
                Vector2 mouse,
                int clicked) {
    ClearBackground(DARKGREEN);
    DrawBoardGrid(board, screenFeatures);

    DrawRectangle(menu.goBackButton.x, menu.goBackButton.y, menu.goBackButton.width, menu.goBackButton.height, WHITE);
    DrawRectangle(menu.goFowardButton.x, menu.goFowardButton.y, menu.goFowardButton.width, menu.goFowardButton.height,
                  WHITE);
    DrawRectangle(menu.saveGameButton.x, menu.saveGameButton.y, menu.saveGameButton.width, menu.saveGameButton.height,
                  WHITE);

    DrawText("Go back", menu.goBackButton.x - MeasureText("Go back", 30) / 2 + menu.goBackButton.width / 2,
             menu.goBackButton.y + menu.goBackButton.height / 2 - 15, 30, canGoBack(board) ? BLACK : GRAY);
    DrawText("Go foward", menu.goFowardButton.x - MeasureText("Go foward", 30) / 2 + menu.goFowardButton.width / 2,
             menu.goFowardButton.y + menu.goFowardButton.height / 2 - 15, 30, canGoFoward(board) ? BLACK : GRAY);
    DrawText("Save game", menu.saveGameButton.x - MeasureText("Save game", 30) / 2 + menu.saveGameButton.width / 2,
             menu.saveGameButton.y + menu.saveGameButton.height / 2 - 15, 30, BLACK);


    if (clicked) {
        CheckButtonPressed(&menu, board, screen, mouse);
    }
    UpdateDrawingState(board, screenFeatures);
    switch (nextTurn(board)) {

        case BLACK_PLAYER:
            SetHelpers(board, BLACK_PLAYER);
            CheckPiecePlayed(board, screenFeatures, clicked, mouse);
            break;
        case WHITE_PLAYER:
            computerMove(board, WHITE_PLAYER);
            break;
        case NONE:
            break;
    }

    DrawText("Your score:", menu.saveGameButton.x, menu.saveGameButton.height + menu.saveGameButton.y + 30, 20, WHITE);
    int val = getScore(board, BLACK_PLAYER);
    DrawText(TextFormat("%d", val), menu.saveGameButton.x, menu.saveGameButton.height + menu.saveGameButton.y + 50, 20,
             WHITE);

    DrawText("CPU score:", menu.saveGameButton.x, menu.saveGameButton.height + menu.saveGameButton.y + 100, 20, WHITE);
    val = getScore(board, WHITE_PLAYER);
    DrawText(TextFormat("%d", val), menu.saveGameButton.x, menu.saveGameButton.height + menu.saveGameButton.y + 120, 20,
             WHITE);


    if (isGameOver(board)) {
        DrawText("Game Over", (screenFeatures->squareSize * board->size) / 2 - MeasureText("Game Over", 80) / 2,
                 screenFeatures->screenHeight / 2 - 40, 80, GRAY);
        char *text;
        Color color;
        switch (getWinner(board)) {
            case WINNER:
                text = "You win!";
                color = GREEN;
                break;
            case LOSER:
                text = "You lose!";
                color = RED;
                break;
            case TIE:
                text = "It's a tie!";
                color = GRAY;
                break;
        }
        DrawText(text, (screenFeatures->squareSize * board->size) / 2 - MeasureText(text, 60) / 2,
                 screenFeatures->screenHeight / 2 - 30 + 80 + 10, 60, color);
    }
    int margin = board->size * screenFeatures->squareSize;
    int freeSpace = screenFeatures->screenWidth - margin;
    Rectangle exit = (Rectangle) {margin + 30, screenFeatures->screenHeight - 150, (freeSpace - 60), 100};
    DrawRectangleRec(exit, WHITE);
    DrawText("Exit", exit.x + exit.width / 2 - MeasureText("Exit", 30) / 2, exit.y + exit.height / 2 - 15, 30, BLACK);
    if (clicked && CheckCollisionPointRec(mouse, exit)) {
        destructBoard(board);
        *screen = MENU;
    }
}

/**
 * Shows the screen with the Saved Files
 * @param board receives a Board type structure
 * @param screenFeatures receives a ScreenFeatures type structure
 * @param filename receives a char with the file name
 * @param frameCounter receives an integer with the number of frames
 * @param mouse receives a Vector type structure from raylib
 * @param screen receives a ScreenFlag type structure
 * @param numOfChars receives an integer with the number of chars
 * @param lastScreen receives a ScreenFlag type structure
 */
void ShowFileSaverScreen(Board *board, ScreenFeatures *screenFeatures, char *filename, int frameCounter, Vector2 mouse,
                         ScreenFlag *screen, int *numOfChars, ScreenFlag *lastScreen) {
    ClearBackground(RAYWHITE);

    int width = fmax(MeasureText(filename, 30), MeasureText("XXXXXXXX", 30)) + 30;

    int saveWidth = MeasureText("Save", 20) + 20;
    int cancelWidth = MeasureText("Cancel", 20) + 20;

    Rectangle saveRect = (Rectangle) {screenFeatures->screenWidth / 2 - (saveWidth + cancelWidth + 40) / 2,
                                      screenFeatures->screenHeight / 2 + 50,
                                      saveWidth, 30};
    Rectangle cancelRect = (Rectangle) {saveRect.x + saveRect.width + 40, saveRect.y, cancelWidth, 30};

    int overSave = CheckCollisionPointRec(mouse, saveRect);
    int overCancel = CheckCollisionPointRec(mouse, cancelRect);


    DrawRectangleRec(saveRect, overSave ? LIGHTGRAY : GRAY);
    DrawText("Save", saveRect.x + 10, saveRect.y + 5, 20, WHITE);
    DrawRectangleRec(cancelRect, overCancel ? LIGHTGRAY : GRAY);
    DrawText("Cancel", cancelRect.x + 10, saveRect.y + 5, 20, WHITE);

    DrawRectangleLines(screenFeatures->screenWidth / 2 - width / 2, screenFeatures->screenHeight / 2 - 20, width, 40,
                       BLACK);
    DrawText(filename, screenFeatures->screenWidth / 2 - width / 2 + 10, screenFeatures->screenHeight / 2 - 15, 30,
             BLACK);
    if (((frameCounter / 20) % 2) == 0)
        DrawText("_", screenFeatures->screenWidth / 2 - width / 2 + MeasureText(filename, 30) + 10,
                 screenFeatures->screenHeight / 2 - 15, 30, BLACK);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overSave) {
        mkdir("saved");
        SaveFileText(TextFormat("saved/%s.brd", filename), saveGame(board));
        memset(filename, 0, 11);
        *numOfChars = 0;
        *screen = *lastScreen;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overCancel) {
        memset(filename, 0, 11);
        *numOfChars = 0;
        *screen = *lastScreen;
    }


}

/**
 * Loads screen with Load Files
 * @param board receives a Board type structure
 * @param screenFeatures receives a ScreenFeatures type structure
 * @param screen receives a ScreenFlag type structure
 * @param slider receives a Slider type structure
 */
void LoadFileScreen(Board *board, ScreenFeatures *screenFeatures, ScreenFlag *screen, Slider *slider) {

    DirectoryEntry directory = getDirectories();
    ClearBackground(RAYWHITE);
    bool bar = (50 * directory.NumberOfDirectories + 10) > (screenFeatures->screenHeight - 80);
    float barSize =
            ((float) (screenFeatures->screenHeight - 80) / (float) (50 * directory.NumberOfDirectories + 10)) *
            (screenFeatures->screenHeight - 70);

    Rectangle cancelRect = (Rectangle) {25, screenFeatures->screenHeight - 60, screenFeatures->screenWidth - 50, 50};

    Rectangle scrollRect = (Rectangle) {screenFeatures->screenWidth - 20,
                                        5 +
                                        fmin(fmax(0, slider->offset + slider->difference),
                                             screenFeatures->screenHeight - 70 - barSize),
                                        15,
                                        barSize};
    if (bar) {
        DrawRectangle(screenFeatures->screenWidth - 20, 5, 15, cancelRect.y - 10, Fade(LIGHTGRAY, 0.6));

        if (slider->collision) {
            slider->offset = GetMousePosition().y;
            if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                slider->collision = false;
            }
        }
        if (CheckCollisionPointRec(GetMousePosition(), scrollRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            slider->offset = GetMousePosition().y;
            slider->difference = scrollRect.y - slider->offset;
            slider->collision = true;
        }
        DrawRectangleRec(scrollRect, slider->collision ? GRAY : Fade(GRAY, 0.65));
    }
    float percent = bar ? (scrollRect.y - 5) / (float) (screenFeatures->screenHeight - 70 - barSize) : 0;
    for (int i = 0; i < directory.NumberOfDirectories; i++) {
        Rectangle rec = (Rectangle) {10, i * 50 + 10 -
                                         (((50 * directory.NumberOfDirectories + 10) - screenFeatures->screenHeight +
                                           60) * percent),
                                     MeasureText(directory.directories[i], 20) + 20, 30};
        int over = CheckCollisionPointRec(GetMousePosition(), rec);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && over) {
            Board boardTemp = loadGame(LoadFileText(TextFormat("saved/%s", directory.directories[i])));
            if (boardTemp.initialized != 1) {
                *screen = GAME;
                DestroyDirectory(directory);
                return;
            }
            *board = boardTemp;
            screenFeatures->squareSize = (float) screenFeatures->screenHeight / (float) board->size;
            *screen = GAME;
        }
        DrawRectangleRec(rec, over ? LIGHTGRAY : RAYWHITE);
        DrawText(directory.directories[i], 20, rec.y + 5, 20, BLACK);
        //DestroyDirectory(directory);
    }

    DrawRectangle(0, screenFeatures->screenHeight - 70, screenFeatures->screenWidth, 70, RAYWHITE);


    int over = CheckCollisionPointRec(GetMousePosition(), cancelRect);
    DrawRectangleRec(cancelRect, over ? LIGHTGRAY : GRAY);
    if (over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) *screen = MENU;
    DrawText("CANCEL", screenFeatures->screenWidth / 2 - MeasureText("CANCEL", 30) / 2 + cancelRect.x / 2,
             cancelRect.y + 10, 30,
             WHITE);

}

/**
 *
 * @param board
 * @param screenFeatures
 */
void UpdateDrawingState(Board *board, ScreenFeatures *screenFeatures) {

    int offset = (screenFeatures->squareSize / 2 - 5) * 0.25;
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            switch (board->state[i][j].pieceType) {
                case VOID:
                    break;
                case BLACK_PIECE:
                    DrawCircle((i) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               screenFeatures->squareSize / 2 - 5, BLACK);
                    DrawCircleGradient((i) * screenFeatures->squareSize + screenFeatures->squareSize / 2 - offset,
                                       (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2 - offset,
                                       (screenFeatures->squareSize / 2 - 5) * 0.5, Fade(WHITE, 0.15), Fade(WHITE, 0));
                    break;
                case WHITE_PIECE:
                    DrawCircle((i) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               screenFeatures->squareSize / 2 - 5, RAYWHITE);
                    DrawCircleGradient((i) * screenFeatures->squareSize + screenFeatures->squareSize / 2 - offset,
                                       (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2 - offset,
                                       (screenFeatures->squareSize / 2 - 5) * 0.5, Fade(BLACK, 0.15), Fade(BLACK, 0));
                    break;
                case HELPER:
                    DrawCircle((i) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               screenFeatures->squareSize / 2 - 5, DARKGRAY);
                    DrawCircle((i) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               (j) * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                               screenFeatures->squareSize / 2 - 7, DARKGREEN);
                    break;
            }
        }
    }
}

/**
 * Gets the menu
 * @param board receives a Board type structure
 * @param screenFeatures receives a ScreenFeatures type structure
 * @return a Menu type structure with the parameters needed
 */
Menu getMenu(Board board, ScreenFeatures *screenFeatures) {
    Rectangle goBackButton = (Rectangle) {board.size * screenFeatures->squareSize + 20, 30,
                                          screenFeatures->screenWidth - board.size * screenFeatures->squareSize - 40,
                                          75};

    Rectangle goFowardButton = (Rectangle) {goBackButton.x, goBackButton.height + goBackButton.y + 10,
                                            screenFeatures->screenWidth - board.size * screenFeatures->squareSize - 40,
                                            75};
    Rectangle saveGameButton = (Rectangle) {goFowardButton.x, goFowardButton.height + goFowardButton.y + 10,
                                            screenFeatures->screenWidth - board.size * screenFeatures->squareSize - 40,
                                            75};
    return (Menu) {goBackButton, goFowardButton, saveGameButton};

}

/**
 * Gets the menu options
 * @param screenFeatures receives a ScreenFeatures type structures
 * @return a MenuOptions type structure
 */
MenuOptions getMenuOptions(ScreenFeatures *screenFeatures) {
    int bussyScreen = 400 + screenFeatures->screenHeight * 0.1;
    int freeScreen = screenFeatures->screenHeight - bussyScreen;
    int number = (screenFeatures->screenWidth - (3 * 250)) / 4;
    Rectangle startGameButton = (Rectangle) {number, bussyScreen + 100, 250, freeScreen - 200};
    Rectangle loadGameButton = (Rectangle) {number + startGameButton.x + startGameButton.width, bussyScreen + 100, 250,
                                            freeScreen - 200};
    Rectangle editorButton = (Rectangle) {number + loadGameButton.x + loadGameButton.width, bussyScreen + 100, 250,
                                          freeScreen - 200};

    return (MenuOptions) {startGameButton, loadGameButton, editorButton};
}

/**
 * Inits the creen slider (scroll)
 * @param slider receives a Slides type structure
 */
void initSlider(Slider *slider) {
    slider->collision = false;
    slider->offset = 0.0f;
    slider->difference = 0.0f;
}

/**
 * Initializes the screen features
 * @param features receives a ScreenFatures type structures
 * @param screenWidth receives an integer with screenWidth
 * @param screenHeight receives an integer with screenHight
 * @param squareSize receives an integer with screenSize
 */
void initScreenFeatures(ScreenFeatures *features, int screenWidth, int screenHeight, float squareSize) {
    features->screenWidth = screenWidth;
    features->screenHeight = screenHeight;
    features->squareSize = squareSize;
}

/**
 * Creates the Menu Screen
 * @param screenFeatures receives a ScreenFatures type structures
 * @param frameCount receives an integer with the frame count
 * @param menuOptions receives a MenuOptions type structure
 * @param screenFlag recives a ScreenFlag type structure
 * @param board receives a Board type structure
 */
void MenuScreen(ScreenFeatures *screenFeatures, int frameCount, MenuOptions menuOptions, ScreenFlag *screenFlag,
                Board *board) {
    ClearBackground(DARKGREEN);
    DrawText("INFINIREVERSI", screenFeatures->screenWidth / 2 - MeasureText("INFINIREVERSI", 30) / 2,
             screenFeatures->screenHeight / 2 - 375,
             30, WHITE);
    int frame = floor(frameCount * 0.383);
    bool putZero = frame < 10;
    Image image = LoadImage(
            TextFormat("resources/frames/frame_%s%d_delay-0.03s.gif", putZero ? "0" : "", frame));
    Texture2D texture = LoadTextureFromImage(image);
    DrawTexture(texture, screenFeatures->screenWidth / 2 - image.width / 2, screenFeatures->screenHeight * 0.1, WHITE);
    DrawRectangleRec(menuOptions.startGameButton, LIGHTGRAY);
    DrawRectangleRec(menuOptions.loadGameButton, LIGHTGRAY);
    DrawRectangleRec(menuOptions.editorButton, LIGHTGRAY);

    DrawText("Start",
             menuOptions.startGameButton.x + menuOptions.startGameButton.width / 2 - MeasureText("Start", 30) / 2,
             menuOptions.startGameButton.y + menuOptions.startGameButton.height / 2 - 15, 30, WHITE);
    DrawText("Load game",
             menuOptions.loadGameButton.x + menuOptions.loadGameButton.width / 2 - MeasureText("Load game", 30) / 2,
             menuOptions.loadGameButton.y + menuOptions.loadGameButton.height / 2 - 15, 30, WHITE);
    DrawText("Game editor",
             menuOptions.editorButton.x + menuOptions.editorButton.width / 2 - MeasureText("Game editor", 30) / 2,
             menuOptions.editorButton.y + menuOptions.editorButton.height / 2 - 15, 30, WHITE);
    DrawText("v1.0", screenFeatures->screenWidth / 2 - MeasureText("v1.0", 30) / 2,
             screenFeatures->screenHeight / 2 + 375,
             15, WHITE);
    DrawText("Created by Molisawa and VaniaPZS",
             screenFeatures->screenWidth - MeasureText("Created by Molisawa and VaniaPZS", 30) / 2,
             screenFeatures->screenHeight / 2 + 375,
             15, WHITE);
    CheckMenuButtonPressed(menuOptions, screenFlag, board);

    EndDrawing();
    UnloadTexture(texture);
    UnloadImage(image);
}

/**
 * Creates the Editor Game screen
 * @param screenFeatures receives a ScreenFeatures type structures
 * @param board receives a Board type structure
 * @param piece receives a Piece type structure
 * @param screen receives a ScreenFlag type structure
 */
void EditorScreen(ScreenFeatures *screenFeatures, Board *board, Piece *piece, ScreenFlag *screen) {
    ClearBackground(DARKGREEN);
    DrawBoardGrid(board, screenFeatures);
    int margin = board->size * screenFeatures->squareSize;
    int freeSpace = screenFeatures->screenWidth - margin;
    float radius = freeSpace / 4;
    Vector2 black = (Vector2) {2 * radius + margin, 50 + radius};
    Vector2 white = (Vector2) {black.x, 50 + black.y + 2 * radius};
    DrawCircleV(black, radius, BLACK);
    DrawCircleV(white, radius, WHITE);
    bool isBlack = piece->pieceType == BLACK_PLAYER;
    DrawCircleV(isBlack ? black : white, radius / 10, RED);

    Vector2 mouse = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    UpdateDrawingState(board, screenFeatures);
    if (mouse.x >= 0 && mouse.x < margin && mouse.y >= 0 && mouse.y < screenFeatures->screenHeight) {
        clicked = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        int x = floorf(mouse.x / screenFeatures->squareSize);
        int y = floorf(mouse.y / screenFeatures->squareSize);
        Vector2 helper = (Vector2) {x * screenFeatures->squareSize + radius,
                                    y * screenFeatures->squareSize + radius};

        Rectangle helperRect = (Rectangle) {helper.x + 1 - radius, helper.y + 1 - radius,
                                            screenFeatures->squareSize - 2,
                                            screenFeatures->squareSize - 2};
        DrawRectangleRec(helperRect, DARKGREEN);

        Vector2 circle = (Vector2) {x * screenFeatures->squareSize + screenFeatures->squareSize / 2,
                                    y * screenFeatures->squareSize + screenFeatures->squareSize / 2};

        switch (piece->pieceType) {
            case BLACK_PLAYER:
                DrawCircleV(circle,
                            screenFeatures->squareSize / 2 - 5, Fade(BLACK, 0.5));

                break;
            case WHITE_PLAYER:
                DrawCircleV(circle,
                            screenFeatures->squareSize / 2 - 5, Fade(WHITE, 0.5));
                break;
            default:
                break;
        }
        if (CheckCollisionPointRec(mouse, helperRect) && clicked)
            board->state[x][y].pieceType = isBlack ? BLACK_PLAYER : WHITE_PLAYER;

        if (CheckCollisionPointRec(mouse, helperRect) && clicked)
            board->initialState[x][y].pieceType = isBlack ? BLACK_PLAYER : WHITE_PLAYER;

        if (CheckCollisionPointRec(mouse, helperRect) && (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) ||
            IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
            board->state[x][y].pieceType = VOID;
    }

    Rectangle exit = (Rectangle) {margin + 30, screenFeatures->screenHeight - 150, (freeSpace - 60), 100};
    DrawRectangleRec(exit, LIGHTGRAY);
    DrawText("Exit", exit.x + exit.width / 2 - MeasureText("Exit", 30) / 2, exit.y + exit.height / 2 - 15, 30, WHITE);

    Rectangle save = (Rectangle) {margin + 30, exit.y - exit.height - 50, (freeSpace - 60), 100};


    DrawRectangleRec(save, LIGHTGRAY);
    DrawText("Save", save.x + save.width / 2 - MeasureText("Save", 30) / 2, save.y + save.height / 2 - 15, 30, WHITE);


    if (clicked && CheckCollisionPointCircle(mouse, black, radius)) piece->pieceType = BLACK_PLAYER;
    if (clicked && CheckCollisionPointCircle(mouse, white, radius)) piece->pieceType = WHITE_PLAYER;

    if (clicked && CheckCollisionPointRec(mouse, save)) *screen = SAVE;

    if (clicked && CheckCollisionPointRec(mouse, exit)) {
        destructBoard(board);
        *screen = MENU;
    }
}

/**
 * Checks if the menu button is pressed
 * @param menuOptions receives a MenuOption type structure
 * @param screen receives a ScreenFlag type structure
 * @param board receives a Board type structure
 */
void CheckMenuButtonPressed(MenuOptions menuOptions, ScreenFlag *screen, Board *board) {
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    Vector2 mouse = GetMousePosition();
    if (clicked && CheckCollisionPointRec(mouse, menuOptions.startGameButton))
        *screen = CONFIG_GAME;
    if (clicked && CheckCollisionPointRec(mouse, menuOptions.loadGameButton))
        *screen = LOAD;
    if (clicked && CheckCollisionPointRec(mouse, menuOptions.editorButton)) {
        *screen = CONFIG_EDITOR;
        board->custom = true;
    }
}

/**
 * Creates the Config Editor Screen
 * @param screenFeatures receives a ScreenFeatures type structure
 * @param board receives a Board type structure
 * @param screen receives a ScreenFlag type structure
 * @param customBoardSize receives an integer with the custom board size
 */
void ConfigEditorScreen(ScreenFeatures *screenFeatures, Board *board, ScreenFlag *screen, int *customBoardSize) {
    ClearBackground(RAYWHITE);
    int size = 6 + 2 * *customBoardSize;
    const char *text = TextFormat("%d", size);
    DrawText(text, screenFeatures->screenWidth / 2 - MeasureText(text, 120) / 2, screenFeatures->screenHeight / 2 - 180,
             120, BLACK);

    DrawText("Chose your board size", screenFeatures->screenWidth / 2 - MeasureText("Chose your board size", 70) / 2,
             screenFeatures->screenHeight / 2 - 300,
             70, GRAY);
    int margin = (screenFeatures->screenWidth - 2 * 130) / 3;
    Rectangle sum = (Rectangle) {margin, screenFeatures->screenHeight / 2 - 15, 130, 130};
    Rectangle subs = (Rectangle) {margin + sum.x + sum.width, sum.y, sum.width, sum.height};

    Vector2 mouse = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool overSum = CheckCollisionPointRec(mouse, sum);
    bool overSubs = CheckCollisionPointRec(mouse, subs);

    DrawRectangleRec(sum, overSum ? LIGHTGRAY : RAYWHITE);
    DrawText("+", sum.x + sum.width / 2 - MeasureText("+", 100) / 2, sum.y + sum.height / 2 - 50, 100, BLACK);
    DrawRectangleRec(subs, overSubs ? LIGHTGRAY : RAYWHITE);
    DrawText("-", subs.x + subs.width / 2 - MeasureText("-", 100) / 2, subs.y + subs.height / 2 - 50, 100, BLACK);

    Rectangle acceptButton = (Rectangle) {screenFeatures->screenWidth / 2 - 100, screenFeatures->screenHeight / 2 + 120,
                                          200, 80};
    Rectangle cancelButton = (Rectangle) {screenFeatures->screenWidth / 2 - 100,
                                          acceptButton.y + acceptButton.height + 50, acceptButton.width,
                                          acceptButton.height};

    DrawRectangleRec(acceptButton, LIGHTGRAY);
    DrawRectangleRec(cancelButton, LIGHTGRAY);

    DrawText("Accept", acceptButton.x + acceptButton.width / 2 - MeasureText("Accept", 40) / 2,
             acceptButton.y + acceptButton.height / 2 - 20, 40, WHITE);
    DrawText("Cancel", cancelButton.x + cancelButton.width / 2 - MeasureText("Cancel", 40) / 2,
             cancelButton.y + cancelButton.height / 2 - 20, 40, WHITE);

    if (overSum && clicked) (*customBoardSize)++;
    if (overSubs && clicked && *customBoardSize > 0) (*customBoardSize)--;

    if (CheckCollisionPointRec(mouse, acceptButton) && clicked) {
        initializeGame(board, size, EASY, true, (Player) {true}, (Player) {true});
        *screen = EDITOR;
        *customBoardSize = 0;
        screenFeatures->squareSize = screenFeatures->screenHeight / size;
    }
    if (CheckCollisionPointRec(mouse, cancelButton) && clicked) {
        *screen = MENU;
        *customBoardSize = 0;
    }

}

/**
 * Creates the Config Game Screen
 * @param screenFeatures receives a ScreenFeatures type structure
 * @param board receives a Board type structure
 * @param screen receives a ScreenFlag type structure
 * @param customBoardSize receives an integer with the custom board size
 * @param difficulty receives a Difficulty type structure
 */
void ConfigGameScreen(ScreenFeatures *screenFeatures, Board *board, ScreenFlag *screen, int *customBoardSize,
                      Difficulty *difficulty) {

    ClearBackground(RAYWHITE);
    int size = 6 + 2 * *customBoardSize;
    const char *text = TextFormat("%d", size);
    DrawText(text, screenFeatures->screenWidth / 3 - MeasureText(text, 100) / 2, screenFeatures->screenHeight / 2 - 180,
             100, BLACK);

    DrawText("Chose your board size", screenFeatures->screenWidth / 3 - MeasureText("Chose your board size", 30) / 2,
             screenFeatures->screenHeight / 2 - 300,
             30, GRAY);
    int margin = (2 * (screenFeatures->screenWidth / 3) - 2 * 130) / 3;
    Rectangle sum = (Rectangle) {margin, screenFeatures->screenHeight / 2 - 30, 130, 130};
    Rectangle subs = (Rectangle) {margin + sum.x + sum.width, sum.y, sum.width, sum.height};

    Vector2 mouse = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool overSum = CheckCollisionPointRec(mouse, sum);
    bool overSubs = CheckCollisionPointRec(mouse, subs);

    DrawRectangleRec(sum, overSum ? LIGHTGRAY : RAYWHITE);
    DrawText("+", sum.x + sum.width / 2 - MeasureText("+", 70) / 2, sum.y + sum.height / 2 - 35, 70, BLACK);
    DrawRectangleRec(subs, overSubs ? LIGHTGRAY : RAYWHITE);
    DrawText("-", subs.x + subs.width / 2 - MeasureText("-", 70) / 2, subs.y + subs.height / 2 - 35, 70, BLACK);

    Rectangle acceptButton = (Rectangle) {screenFeatures->screenWidth / 2 - 100, screenFeatures->screenHeight / 2 + 120,
                                          200, 80};
    Rectangle cancelButton = (Rectangle) {screenFeatures->screenWidth / 2 - 100,
                                          acceptButton.y + acceptButton.height + 50, acceptButton.width,
                                          acceptButton.height};

    DrawRectangleRec(acceptButton, LIGHTGRAY);
    DrawRectangleRec(cancelButton, LIGHTGRAY);


    DrawText("Accept", acceptButton.x + acceptButton.width / 2 - MeasureText("Accept", 40) / 2,
             acceptButton.y + acceptButton.height / 2 - 20, 40, WHITE);
    DrawText("Cancel", cancelButton.x + cancelButton.width / 2 - MeasureText("Cancel", 40) / 2,
             cancelButton.y + cancelButton.height / 2 - 20, 40, WHITE);

    float marginDifficulty = (screenFeatures->screenHeight - 340) / 2;

    Rectangle easyButton = (Rectangle) {
            screenFeatures->screenWidth * 3 / 4 - (screenFeatures->screenWidth / 3 - 80) / 2,
            marginDifficulty, screenFeatures->screenWidth / 3 - 80, 80};

    Rectangle intermediateButton = (Rectangle) {easyButton.x, easyButton.y + easyButton.height + 50, easyButton.width,
                                                easyButton.height};

    Rectangle hardButton = (Rectangle) {intermediateButton.x, intermediateButton.y + intermediateButton.height + 50,
                                        intermediateButton.width,
                                        intermediateButton.height};

    DrawRectangleRec(easyButton, *difficulty == EASY ? GRAY : LIGHTGRAY);
    DrawRectangleRec(intermediateButton, *difficulty == INTERMEDIATE ? GRAY : LIGHTGRAY);
    DrawRectangleRec(hardButton, *difficulty == HARD ? GRAY : LIGHTGRAY);

    DrawText("EASY", easyButton.x + easyButton.width / 2 - MeasureText("EASY", 20) / 2,
             easyButton.y + easyButton.height / 2 - 10, 20, WHITE);
    DrawText("INTERMEDIATE", intermediateButton.x + intermediateButton.width / 2 - MeasureText("INTERMEDIATE", 20) / 2,
             intermediateButton.y + intermediateButton.height / 2 - 10, 20, WHITE);
    DrawText("HARD", hardButton.x + hardButton.width / 2 - MeasureText("HARD", 20) / 2,
             hardButton.y + hardButton.height / 2 - 10, 20, WHITE);

    bool overEasy = CheckCollisionPointRec(mouse, easyButton);
    bool overIntermediate = CheckCollisionPointRec(mouse, intermediateButton);
    bool overHard = CheckCollisionPointRec(mouse, hardButton);

    if (clicked && overEasy) *difficulty = EASY;
    if (clicked && overIntermediate) *difficulty = INTERMEDIATE;
    if (clicked && overHard) *difficulty = HARD;

    if (overSum && clicked) (*customBoardSize)++;
    if (overSubs && clicked && *customBoardSize > 0) (*customBoardSize)--;

    if (CheckCollisionPointRec(mouse, acceptButton) && clicked) {
        initializeGame(board, size, *difficulty, true, (Player) {true}, (Player) {false});
        *screen = GAME;
        *difficulty = EASY;
        *customBoardSize = 0;
        screenFeatures->squareSize = screenFeatures->screenHeight / size;
    }
    if (CheckCollisionPointRec(mouse, cancelButton) && clicked) {
        *screen = MENU;
        *difficulty = EASY;
        *customBoardSize = 0;
    }

}