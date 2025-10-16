//This file is just outline for making Maze Game
//The main file is Triplay-Console/file1.cpp

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
using namespace std;

class Maze;
class MazeCell;

class BaseMaze {
public:
    virtual void display() {
        cout << "Base Maze Display\n";
    }
};

class MazeCell {
    char symbol;

public:
    MazeCell(char c = '#') : symbol(c) {}  

    void setSymbol(char c) {
        symbol = c;
    }

    char getSymbol() const {
        return symbol;
    }

    bool operator==(const MazeCell& other) const {
        return symbol == other.symbol;
    }

    friend void showCellSymbol(const MazeCell& cell);
    friend class Maze;
};

void showCellSymbol(const MazeCell& cell) {
    cout << "Cell contains: " << cell.symbol << endl;
}

class Maze : public BaseMaze {
    int n, m;
    MazeCell** grid;
    vector<pair<int, int>> directions = {{-2,0},{2,0},{0,-2},{0,2}};

public:
    Maze(int rows, int cols) : n(rows), m(cols) {
        grid = new MazeCell*[n];
        for (int i = 0; i < n; ++i) {
            grid[i] = new MazeCell[m];
        }
    }

    ~Maze() {
        for (int i = 0; i < n; ++i)
            delete[] grid[i];
        delete[] grid;
    }

    void setCell(int i, int j, char c) {
        if (i >= 0 && i < n && j >= 0 && j < m)
            grid[i][j].setSymbol(c);
    }

    void setCell(pair<int, int> pos, char c) {
        setCell(pos.first, pos.second, c);
    }

    void generate(int r, int c) {
        grid[r][c].setSymbol(' ');
        static random_device rd;
        static mt19937 g(rd());
        shuffle(directions.begin(), directions.end(), g);

        for (auto& dir : directions) {
            int nr = r + dir.first;
            int nc = c + dir.second;
            if (nr > 0 && nr < n - 1 && nc > 0 && nc < m - 1 &&
                grid[nr][nc].getSymbol() == '#') {
                grid[r + dir.first / 2][c + dir.second / 2].setSymbol(' ');
                generate(nr, nc);
            }
        }
    }

    void placeStartEnd() {
        grid[1][1].setSymbol('S');
        grid[n - 2][m - 2].setSymbol('E');
    }

    void display() override {
        cout << "\nGenerated Maze:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << grid[i][j].getSymbol();
            }
            cout << endl;
        }
    }

    MazeCell getCell(int i, int j) {
        return grid[i][j];
    }
};

int main() {
    srand(time(0));
    int n, m;
    cout << "Enter maze dimensions (odd numbers recommended, e.g. 15 25): ";
    cin >> n >> m;

    if (n < 5 || m < 5) {
        cout << "Maze too small! Try bigger dimensions.\n";
        return 0;
    }

    Maze maze(n, m);

    maze.generate(1, 1);
    maze.placeStartEnd();
    maze.display();  

    maze.setCell({0, 0}, 'X');
    maze.setCell(n - 1, m - 1, 'X');

    MazeCell a = maze.getCell(1, 1);
    MazeCell b = maze.getCell(n - 2, m - 2);

    if (a == b) {
        cout << "Start and End symbols are same.\n";
    } else {
        cout << "Start and End symbols are different.\n";
    }

    showCellSymbol(a);

    return 0;
}
