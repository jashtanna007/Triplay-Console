#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

// -------------------------Box Class-----------------------------
class Box {
    char mark;
public:
    Box() { mark = ' '; }
    char getmark() { return mark; }
    void setmark(char m) { mark = m; }
    bool isempty() { return mark == ' '; }
};

// --------------------------Board Class-----------------------------
class Board {
    Box **grid;
    int size;
public:
    Board() {
        size = 3;
        grid = new Box *[size];
        for (int i = 0; i < size; i++)
            grid[i] = new Box[size];
    }
    ~Board() {
        for (int k = 0; k < size; k++) delete[] grid[k];
        delete[] grid;
    }

    void display(vector<pair<int, int>> winCell = {}) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                bool winBox = false;
                for (auto &p : winCell)
                    if (p.first == i && p.second == j) winBox = true;
                char m = grid[i][j].getmark();
                if (m == ' ') cout << "   ";
                else if (winBox) cout << " \033[32m" << m << "\033[0m ";
                else cout << " " << m << " ";
                if (j < 2) cout << " | ";
            }
            cout << endl;
            if (i < 2) cout << "----#-----#----" << endl;
        }
    }

    bool place(int pos, char symbol) {
        int row = (pos - 1) / 3, col = (pos - 1) % 3;
        if (pos >= 1 && pos <= 9 && grid[row][col].isempty()) {
            grid[row][col].setmark(symbol);
            return true;
        }
        return false;
    }

    friend vector<pair<int, int>> WinLine(Board &b, char symbol);
};

vector<pair<int, int>> WinLine(Board &b, char symbol) {
    for (int i = 0; i < 3; i++)
        if (b.grid[i][0].getmark() == symbol &&
            b.grid[i][1].getmark() == symbol &&
            b.grid[i][2].getmark() == symbol)
            return {{i, 0}, {i, 1}, {i, 2}};
    for (int j = 0; j < 3; j++)
        if (b.grid[0][j].getmark() == symbol &&
            b.grid[1][j].getmark() == symbol &&
            b.grid[2][j].getmark() == symbol)
            return {{0, j}, {1, j}, {2, j}};
    if (b.grid[0][0].getmark() == symbol &&
        b.grid[1][1].getmark() == symbol &&
        b.grid[2][2].getmark() == symbol)
        return {{0, 0}, {1, 1}, {2, 2}};
    if (b.grid[0][2].getmark() == symbol &&
        b.grid[1][1].getmark() == symbol &&
        b.grid[2][0].getmark() == symbol)
        return {{0, 2}, {1, 1}, {2, 0}};
    return {};
}

// -------------------------- Maze Runner (Part A) -----------------------------

class Maze {
    int rows, cols;
    vector<vector<char>> grid;
    pair<int,int> start, end;
    vector<pair<int,int>> dirs = {{-2,0},{2,0},{0,-2},{0,2}};

    bool isValid(int r, int c) {
        return r > 0 && c > 0 && r < rows - 1 && c < cols - 1;
    }

    void dfs(int r, int c) {
        random_shuffle(dirs.begin(), dirs.end());
        for (auto &d : dirs) {
            int nr = r + d.first;
            int nc = c + d.second;
            if (isValid(nr, nc) && grid[nr][nc] == '#') {
                grid[nr - d.first / 2][nc - d.second / 2] = ' ';
                grid[nr][nc] = ' ';
                dfs(nr, nc);
            }
        }
    }

public:
    Maze(int r = 15, int c = 15) : rows(r), cols(c) {
        if (rows % 2 == 0) rows++;
        if (cols % 2 == 0) cols++;
        grid.resize(rows, vector<char>(cols, '#'));
    }

    ~Maze() {}

    void generate() {
        srand(time(0));
        grid[1][1] = ' ';
        dfs(1, 1);
        start = {1, 1};
        end = {rows - 2, cols - 2};
        grid[start.first][start.second] = 'S';
        grid[end.first][end.second] = 'E';
    }

    friend ostream &operator<<(ostream &out, const Maze &m) {
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++)
                out << m.grid[i][j];
            out << endl;
        }
        return out;
    }

    char operator()(int i, int j) const { return grid[i][j]; }
};

// -------------------------- Main ------------------------------
int main() {
    int choice;
    cout << "Enter your choice accordingly\n";
    cout << "1. Tic Tac Toe\n2. Maze Runner\n3. Knight Tour\n\n";
    cin >> choice;
    cout << endl;

    if (choice == 1) {
        string username1, username2;
        cout << "Enter Player1 name: ";
        cin >> username1;
        cout << "\nEnter Player2 name: ";
        cin >> username2;
        int series;
        cout << "\nBest of series (1/3/5): ";
        cin >> series;
        cout << endl;

        int minwins = (series / 2) + 1;
        int p1Score = 0, p2Score = 0, round = 0;

        while (p1Score < minwins && p2Score < minwins && round < series) {
            round++;
            cout << "---------------Round " << round << "---------------\n\n";
            Board bd;
            int movesCount = 0;
            char currSymbol = (round % 2 == 0) ? 'O' : 'X';
            string currentPlayer;

            while (movesCount < 9) {
                bd.display();
                currentPlayer = (currSymbol == 'X') ? username1 : username2;
                cout << endl << currentPlayer << "(" << currSymbol << ") Enter position (1-9): ";
                int position;
                cin >> position;
                cout << endl;

                if (!bd.place(position, currSymbol)) {
                    cout << "Not a valid move !!! Enter a correct one.\n";
                } else {
                    vector<pair<int, int>> winCell = WinLine(bd, currSymbol);
                    if (!winCell.empty()) {
                        bd.display(winCell);
                        cout << endl << currentPlayer << " WINS round " << round << "!\n\n";
                        if (currSymbol == 'X') p1Score++;
                        else p2Score++;
                        break;
                    }
                    movesCount++;
                    currSymbol = (currSymbol == 'X') ? 'O' : 'X';
                }
            }
            if (movesCount == 9) {
                bd.display();
                cout << "It's a draw!\n";
            }
        }

        cout << "Final Result: ";
        if (p1Score > p2Score) cout << username1 << " wins the series!\n\n";
        else if (p1Score < p2Score) cout << username2 << " wins the series!\n\n";
        else cout << "Series Draw!\n";
    }

    else if (choice == 2) {
        int r, c;
        cout << "Enter maze size (rows cols): ";
        cin >> r >> c;
        Maze m(r, c);
        m.generate();
        cout << "\nGenerated Maze:\n" << m;
    }

    else {
        cout << "Work in progress\n";
    }

    return 0;
}