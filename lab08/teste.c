#include <stdio.h>
#include <stdlib.h>

#define XMAXSIZE 10
#define YMAXSIZE 10

char mapa[XMAXSIZE][YMAXSIZE] = 
{
{'X','X','X','X','X','X','X','X','X','X'},
{'X','_','_','_','X','X','_','X','_','X'},
{'X','_','X','_','X','X','_','_','_','X'},
{'X','_','X','X','_','_','_','X','_','X'},
{'X','_','X','_','_','X','_','_','_','X'},
{'X','_','_','_','X','X','_','X','_','X'},
{'X','_','X','_','_','X','_','X','_','X'},
{'X','_','_','X','_','_','_','X','_','X'},
{'X','_','_','_','_','X','_','_','_','X'},
{'X','X','X','X','X','X','X','X','X','X'}
};

int visitados[XMAXSIZE][YMAXSIZE];

int xRob, yRob, xLoc, yLoc;

int daParaPassar(int x, int y);

int posicaoXRobinson();

int posicaoYRobinson();

int posicaoXLocal();

int posicaoYLocal();

void inicializaVisitados();

int foiVisitado(int x, int y);

void visitaCelula(int x, int y);

int daParaPassar(int x, int y) {
 if ((x < 0 || x >= XMAXSIZE) || (y < 0 || y >= YMAXSIZE))
  return 0;
 else
  return mapa[x][y] != 'X';
}

int posicaoXRobinson() {
  return xRob;
}

int posicaoYRobinson() {
  return yRob;
}

int posicaoXLocal() {
  return xLoc;
}

int posicaoYLocal() {
  return yLoc;
}

void inicializaVisitados() {
  int i, j;
  for (i = 0; i < XMAXSIZE; i++) {
    for (j = 0; j < YMAXSIZE; j++) {
      visitados[i][j] = 0;
    }
  }
}

int foiVisitado(int x, int y) {
  return visitados[x][y] == 1;
}

void visitaCelula(int x, int y) {
  visitados[x][y] = 1;
}

int dfs(int xr, int yr, int nivel, int coord[][2]){
    visitaCelula(xr, yr);
    if (xr == xLoc && yr == yLoc){
        coord[nivel][0] = xr;
        coord[nivel][1] = yr;
        return 0;
    }
    else{
        coord[nivel][0] = xr;
        coord[nivel][1] = yr;
        if (daParaPassar(xr+1, yr) == 1 && foiVisitado(xr+1, yr+1) == 0)
            if (dfs(xr+1, yr, nivel+1, coord) == 0)
                return 0;
        if (daParaPassar(xr+1, yr-1) == 1 && foiVisitado(xr+1, yr-1) == 0)
            if (dfs(xr+1, yr-1, nivel+1, coord) == 0)
                return 0;
        if (daParaPassar(xr, yr-1) == 1 && foiVisitado(xr, yr-1) == 0)
            if (dfs(xr, yr-1, nivel+1, coord) == 0)
                return 0;
        if (daParaPassar(xr-1, yr-1) == 1 && foiVisitado(xr-1, yr-1) == 0)
            if (dfs(xr-1, yr-1, nivel+1, coord) == 0)
                return 0;
        if (daParaPassar(xr-1, yr) == 1 && foiVisitado(xr-1, yr) == 0)
            if (dfs(xr-1, yr, nivel+1, coord) == 0)
                return 0;
        if (daParaPassar(xr-1, yr+1) == 1 && foiVisitado(xr-1, yr+1) == 0)
            if (dfs(xr-1, yr+1, nivel+1, coord) == 0)
                return 0;
        if (daParaPassar(xr, yr+1) == 1 && foiVisitado(xr, yr+1) == 0)
            if (dfs(xr, yr+1, nivel+1, coord) == 0)
                return 0;
        if (daParaPassar(xr+1, yr+1) == 1 && foiVisitado(xr+1, yr+1) == 0)
            if (dfs(xr+1, yr+1, nivel+1, coord) == 0)
                return 0;
        return 1;
    }
}


void ajudaORobinson(){
    inicializaVisitados();
    int coord[64][2];
    dfs(xRob, yRob, 0, coord);
    int i = 0;
    while (coord[i][0] != xLoc || coord[i][1] != yLoc)
        i++;
    while (coord[i][0] > 0 && coord[i][1] > 0 && coord[i][0] < 10 && coord[i][1] < 10){
        printf("%d %d\n", coord[i][0], coord[i][1]);
        i--;
    }
}

int main() {

    xRob = 3;
    yRob = 1;
    xLoc = 8;
    yLoc = 3;

    ajudaORobinson();

    return 0;
}