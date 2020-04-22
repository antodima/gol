#include <iostream>
#include <random>
#include <functional>
#include <vector>
#include <ctime>
#include <chrono>
#include "gol.hpp"

using namespace std;
using namespace std::chrono;


class GameOfLifeSeq : public virtual GameOfLife {

public:
  GameOfLifeSeq(int rows, int cols) : GameOfLife(rows, cols) { }

  void step() {
    vector<vector<bool>> tmp(n_rows, vector<bool>(n_cols));
    std::copy(matrix.begin(), matrix.end(), tmp.begin());
    for (size_t i = 1; i < n_rows-1; i++) {
      for (size_t j = 1; j < n_cols-1; j++) {
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
          tmp[i][j] = true;
        }
        // an empty cell with exactly 3 alive neighbours becomes populated by a new individual
        else if (neighbours_alive == 3 && !matrix[i][j]) {
          tmp[i][j] = true;
        }
        // an alive cell with less than 2 alive neighbours dies (becomes empty)
        else if (matrix[i][j] && neighbours_alive < 2) {
          tmp[i][j] = false;
        }
        // an alive cell with more than 3 alive neighbours dies (become empty)
        else if (matrix[i][j] && neighbours_alive > 3) {
          tmp[i][j] = false;
        }
      }
    }
    matrix = tmp;
    tmp.clear();
  }

};


int main(int argc, char* argv[]) {
  if(argc == 1) cerr<<"Usage: ./gol_seq 'rows' 'columns' 'number of iterations'"<<endl;
  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  int iters = atoi(argv[3]);
  cout<<rows<<"x"<<cols<<" matrix, "
      <<(rows * cols)<<" cells, "
      <<iters<<" iterations."<<endl;

  GameOfLifeSeq* gol = new GameOfLifeSeq(rows, cols);
  gol->toString();

  auto begin = high_resolution_clock::now();
  for (size_t i = 0; i < iters; i++) gol->step();
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - begin);

  gol->toString();
  cout<<"Total execution time: "<<duration.count()<<" usec"<<endl;

  return 0;
}
