#include <iostream>
#include <random>
#include <functional>
#include <vector>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;


/**
* Class implementtion of Game of Life.
*/
class GameOfLife {

protected:
  vector<vector<bool>> matrix;
  int n_rows = 0;
  int n_cols = 0;

  bool randomState() {
    return (bool)(std::rand() % 2);
  }

public:
  GameOfLife (int rows, int cols) {
    std::srand(std::time(nullptr));
    n_rows = rows;
    n_cols = cols;
    matrix.push_back(vector<bool>(n_cols));
    for (size_t i = 1; i < n_rows-1; i++) {
      vector<bool> row(n_cols);
      for (size_t j = 1; j < n_cols-1; j++)
        row[j] = randomState();
      matrix.push_back(row);
    }
    matrix.push_back(vector<bool>(n_cols));
  }

  void toString() {
    string sep = "";
    for (size_t i = 0; i < n_cols*2; i++) sep += '=';
    cout<<sep<<endl;
    for (vector<bool>& row : matrix){
      for (bool item : row) {
        cout<<(item ? '@' : '-')<<' ';
      }
      cout<<endl;
    }
    cout<<sep<<"\n"<<endl;
  }

  virtual void step() = 0;

};
