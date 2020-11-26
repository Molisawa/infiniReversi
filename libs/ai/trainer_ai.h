#include "../engine/reversi_game_engine.h"

#ifndef INFINIREVERSI_TRAINER_AI_H
#define INFINIREVERSI_TRAINER_AI_H


void train(int, int);

Movement epsilonMove(Board *, PlayerType);

#endif //INFINIREVERSI_TRAINER_AI_H
