#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <omp.h>
#include "gol.h"

using namespace std;


class GameOfLifeOmp : public virtual GameOfLife {

private:
  int nw = 1;

  bool check_cell(int i, int j) {
    int neighbours_alive = 0;
    if (matrix[i][j])     neighbours_alive++;
    if (matrix[i][j-1])   neighbours_alive++;
    if (matrix[i][j+1])   neighbours_alive++;
    if (matrix[i-1][j])   neighbours_alive++;
    if (matrix[i+1][j])   neighbours_alive++;
    if (matrix[i+1][j-1]) neighbours_alive++;
    if (matrix[i+1][j+1]) neighbours_alive++;
    if (matrix[i-1][j+1]) neighbours_alive++;
    // an individual in a cell with 2 or 3 alive neighbours stays alive
    if (matrix[i][j] && (neighbours_alive == 2 || neighbours_alive == 3)) {
      return(true);
    }
    // an empty cell with exactly 3 alive neighbours becomes populated by a new individual
    else if (neighbours_alive == 3 && !matrix[i][j]) {
      return(true);
    }
    // an alive cell with less than 2 alive neighbours dies (becomes empty)
    else if (matrix[i][j] && neighbours_alive < 2) {
      return(false);
    }
    // an alive cell with more than 3 alive neighbours dies (become empty)
    else if (matrix[i][j] && neighbours_alive > 3) {
      return(false);
    }
    return(matrix[i][j]);
  }

public:
  GameOfLifeOmp(int rows, int cols, int workers) : GameOfLife(rows, cols) {
    nw = workers;
  }

  void step() {
    vector<vector<bool>> tmp(n_rows, vector<bool>(n_cols));
    std::copy(matrix.begin(), matrix.end(), tmp.begin());
    double start = omp_get_wtime();
    size_t i, j = 0;//collapse(2) shared(tmp, matrix)
    #pragma omp parallel for num_threads(nw) collapse(2) private(i, j) shared(tmp, matrix)
    for (i = 1; i < n_rows-1; i++) {
      for (j = 1; j < n_cols-1; j++) {
        tmp[i][j] = check_cell(i, j);
      }
    }
    double duration = omp_get_wtime() - start;
    cout<<"Step execution time: "<<duration*1000000<<" usec"<<endl;
    matrix = tmp;
    tmp.clear();
  }

};


int main(int argc, char* argv[]) {
  if(argc == 1) cerr<<"Usage: ./gol_omp 'rows' 'columns' 'number of iterations' 'nw (number of workers)'"<<endl;

  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  int iters = atoi(argv[3]);
  int nw = atoi(argv[4]);
  if (nw == -1) nw = std::thread::hardware_concurrency();
  cout<<rows<<"x"<<cols<<" matrix, "
      <<(rows * cols)<<" cells, "
      <<iters<<" iterations, "
      <<nw<<" workers."<<endl;

  GameOfLifeOmp* gol = new GameOfLifeOmp(rows, cols, nw);
  gol->toString();
  double begin = omp_get_wtime();
  for (size_t i = 0; i < iters; i++) {
    cout<<"Step "<<i+1<<endl;
    gol->step();
    gol->toString();
  }
  double duration = omp_get_wtime() - begin;
  cout<<"Total execution time: "<<duration*1000000<<" usec"<<endl;

  return 0;
}
