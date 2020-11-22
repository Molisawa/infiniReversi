#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include <dirent.h>
#include <stdlib.h>
#include <math.h>
#include "graphic_engine.h"

void CheckButtonPressed(Menu *menu, Board *board, ScreenFlag *screen, Vector2 mouse) {
    if (CheckCollisionPointRec(mouse, menu->goBackButton)) {
        goBack(board);
        SetHelpers(board);
    } else if (CheckCollisionPointRec(mouse, menu->goFowardButton)) {
        goForward(board);
    } else if (CheckCollisionPointRec(mouse, menu->saveGameButton)) {
        *screen = SAVE;
    }
}

DirectoryEntry getDirectories() {
    DIR *d;
    struct dirent *dir;
    d = opendir("saved/");

    int count = 0;
    char **dirs = malloc(sizeof(char *));

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char *tmp = calloc(dir->d_namlen + 1, sizeof(char));
            for (int i = 0; i < dir->d_namlen; i++) {
                tmp[i] = dir->d_name[i];

            }
            tmp[dir->d_namlen] = '\0';
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

void DestroyDirectory(DirectoryEntry directory) {
    for (int i = 0; i < directory.NumberOfDirectories; i++) {
        free(directory.directories[i]);
    }
    free(directory.directories);
}

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
                            Movement m = {.pieceType = BLACK_PIECE, .x = i, .y = j};
                            makeRealMove(board, m);
                            removeHistoryFoward(board);
                            computerMove(board);
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

void DrawBoardGrid(Board *board, ScreenFeatures *screenFeatures) {
    for (int i = 0; i < board->size + 1; i++) {
        DrawLineV((Vector2) {screenFeatures->squareSize * i, 0},
                  (Vector2) {screenFeatures->squareSize * i, board->size * screenFeatures->squareSize}, BLACK);
    }

    for (int i = 0; i < board->size + 1; i++) {
        DrawLineV((Vector2) {0, screenFeatures->squareSize * i},
                  (Vector2) {board->size * screenFeatures->squareSize, screenFeatures->squareSize * i}, BLACK);
    }
}

void PlayScreen(Board *board, Menu menu, ScreenFeatures *screenFeatures, ScreenFlag *screen,
                Vector2 mouse,
                int clicked) {
    ClearBackground(DARKGREEN);
    DrawBoardGrid(board, screenFeatures);

    DrawRectangle(board->size * screenFeatures->squareSize + 1, 0, screenFeatures->screenWidth - 1,
                  screenFeatures->screenHeight, WHITE);
    DrawRectangle(board->size * screenFeatures->squareSize + 1, 0, screenFeatures->screenWidth - 1,
                  screenFeatures->screenHeight, Fade(DARKGREEN, 0.5f));
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
    SetHelpers(board);
    UpdateDrawingState(board, screenFeatures);

    if (clicked)
        CheckButtonPressed(&menu, board, screen, mouse);
    CheckPiecePlayed(board, screenFeatures, clicked, mouse);

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
}

void ShowFileSaverScreen(Board *board, ScreenFeatures *screenFeatures, char *filename, int frameCounter, Vector2 mouse,
                         ScreenFlag *screen, int *numOfChars) {
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
        *screen = GAME;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overCancel) {
        memset(filename, 0, 11);
        *numOfChars = 0;
        *screen = GAME;
    }


}

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
    if (over && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) *screen = GAME;
    DrawText("CANCEL", screenFeatures->screenWidth / 2 - MeasureText("CANCEL", 30) / 2 + cancelRect.x / 2,
             cancelRect.y + 10, 30,
             WHITE);

}

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
    Rectangle loadGameButton = (Rectangle) {saveGameButton.x, saveGameButton.height + saveGameButton.y + 10,
                                            screenFeatures->screenWidth - board.size * screenFeatures->squareSize - 40,
                                            75};

    return (Menu) {goBackButton, goFowardButton, saveGameButton};

}

void initSlider(Slider *slider) {
    slider->collision = false;
    slider->offset = 0.0f;
    slider->difference = 0.0f;
}

void initScreenFeatures(ScreenFeatures *features, int screenWidth, int screenHeight, float squareSize) {
    features->screenWidth = screenWidth;
    features->screenHeight = screenHeight;
    features->squareSize = squareSize;
}

void MenuScreen(ScreenFeatures *screenFeatures, int frameCount) {
    ClearBackground(DARKGREEN);
    int frame = floor(frameCount * 0.383);
    bool putZero = frame < 10;
    Image image = LoadImage(
            TextFormat("resources/frames/frame_%s_delay-0.03s.gif", TextFormat("%s%d", putZero ? "0" : "", frame)));
    Texture2D texture = LoadTextureFromImage(image);
    DrawTexture(texture, screenFeatures->screenWidth / 2 - image.width / 2, screenFeatures->screenHeight * 0.1, WHITE);
    EndDrawing();
    UnloadTexture(texture);
    UnloadImage(image);
}

void EditorScreen(ScreenFeatures *screenFeatures) {
    ClearBackground(DARKGREEN);
}

#pragma clang diagnostic pop