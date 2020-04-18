#include <iostream>
#include <random>
#include <functional>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#include "gol.h"

using namespace std;
using namespace std::chrono;


class GameOfLifePar : public virtual GameOfLife {

private:
  int nw = 1;
  mutex mtx;

public:
  GameOfLifePar(int rows, int cols, int workers) : GameOfLife(rows, cols) {
    nw = workers;
  }

  void step() {
    vector<thread> workers;
    vector<vector<bool>> tmp(n_rows, vector<bool>(n_cols));
    std::copy(matrix.begin(), matrix.end(), tmp.begin());
    int chunk_size = (n_rows-2) / nw;
    int rest = (n_rows-2) % nw;
    for (int k = 1; k < n_rows-2; k += chunk_size) {
      workers.push_back(thread([&](vector<vector<bool>> M, int idx) {
        int chunk_idx = idx + chunk_size - 1;
        int start_idx = idx;
        int end_idx = (idx != n_rows-1) ? (chunk_idx) : (chunk_idx + rest);
        cout<<"Thread [start_idx="<<start_idx<<", end_idx="<<end_idx<<"]"<<endl;
        for (size_t i = start_idx; i <= end_idx; i++) {
          for (size_t j = 1; j < n_cols-1; j++) {
            int neighbours_alive = 0;
            if (M[i][j])     neighbours_alive++;
            if (M[i][j-1])   neighbours_alive++;
            if (M[i][j+1])   neighbours_alive++;
            if (M[i-1][j])   neighbours_alive++;
            if (M[i+1][j])   neighbours_alive++;
            if (M[i+1][j-1]) neighbours_alive++;
            if (M[i+1][j+1]) neighbours_alive++;
            if (M[i-1][j+1]) neighbours_alive++;
            // an individual in a cell with 2 or 3 alive neighbours stays alive
            if (M[i][j] && (neighbours_alive == 2 || neighbours_alive == 3)) {
              mtx.lock();
              tmp[i][j] = true;
              mtx.unlock();
            }
            // an empty cell with exactly 3 alive neighbours becomes populated by a new individual
            else if (neighbours_alive == 3 && !M[i][j]) {
              mtx.lock();
              tmp[i][j] = true;
              mtx.unlock();
            }
            // an alive cell with less than 2 alive neighbours dies (becomes empty)
            else if (M[i][j] && neighbours_alive < 2) {
              mtx.lock();
              tmp[i][j] = false;
              mtx.unlock();
            }
            // an alive cell with more than 3 alive neighbours dies (become empty)
            else if (M[i][j] && neighbours_alive > 3) {
              mtx.lock();
              tmp[i][j] = false;
              mtx.unlock();
            }
          }
        }

      }, matrix, k));
    }

    for (thread &t : workers) t.join();
    matrix = tmp;
    tmp.clear();
    workers.clear();
  }

};


int main(int argc, char* argv[]) {
  if(argc == 1) cerr<<"Usage: ./gol_par 'rows' 'columns' 'number of iterations' 'nw (number of workers)'"<<endl;

  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  int iters = atoi(argv[3]);
  int nw = atoi(argv[4]);
  if (nw == -1) nw = std::thread::hardware_concurrency();
  cout<<rows<<"x"<<cols<<" matrix, "
      <<(rows * cols)<<" cells, "
      <<iters<<" iterations, "
      <<nw<<" workers."<<endl;

  GameOfLifePar* gol = new GameOfLifePar(rows, cols, nw);
  gol->toString();

  auto begin = high_resolution_clock::now();
  for (size_t i = 0; i < iters; i++) gol->step();
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - begin);

  gol->toString();
  cout<<"Total execution time: "<<duration.count()<<" usec"<<endl;

  return 0;
}
