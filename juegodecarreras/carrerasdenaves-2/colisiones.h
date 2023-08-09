#ifndef COLISIONES_H
#define COLISIONES_H

#include "nave.h"
#include "pista.h"

bool colisionRectangular(const Nave& nave, const Pista& pista);
bool colisionCircular(int x1, int y1, int radio1, int x2, int y2, int radio2);

#endif // COLISIONES_H

