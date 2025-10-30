#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

// =========================== TIC TAC TOE SECTION ===============================

class Box
{
    char mark;

public:
    Box() { mark = ' '; }
    char getmark() { return mark; }
    void setmark(char m) { mark = m; }
    bool isempty() { return mark == ' '; }
};

class Board
{
    Box **grid;
    int size;

public:
    Board()
    {
        size = 3;
        grid = new Box *[size];
        for (int i = 0; i < size; i++)
            grid[i] = new Box[size];
    }
    ~Board()
    {
        for (int i = 0; i < size; i++)
            delete[] grid[i];
        delete[] grid;
    }

    void display(vector<pair<int, int>> winCell = {})
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                bool winBox = false;
                for (auto &cell : winCell)
                    if (cell.first == i && cell.second == j)
                        winBox = true;
                char m = grid[i][j].getmark();
                if (m == ' ')
                    cout << "   ";
                else if (winBox)
                    cout << " \033[32m" << m << "\033[0m ";
                else
                    cout << " " << m << " ";
                if (j < 2)
                    cout << " | ";
            }
            cout << endl;
            if (i < 2)
                cout << "----#-----#----";
            cout << endl;
        }
    }

    bool place(int pos, char symbol)
    {
        int row = (pos - 1) / 3, col = (pos - 1) % 3;
        if (pos >= 1 && pos <= 9 && grid[row][col].isempty())
        {
            grid[row][col].setmark(symbol);
            return true;
        }
        return false;
    }
    friend vector<pair<int, int>> WinLine(Board &b, char symbol);
};

vector<pair<int, int>> WinLine(Board &b, char symbol)
{
    for (int i = 0; i < 3; i++)
        if (b.grid[i][0].getmark() == symbol && b.grid[i][1].getmark() == symbol && b.grid[i][2].getmark() == symbol)
            return {{i, 0}, {i, 1}, {i, 2}};
    for (int j = 0; j < 3; j++)
        if (b.grid[0][j].getmark() == symbol && b.grid[1][j].getmark() == symbol && b.grid[2][j].getmark() == symbol)
            return {{0, j}, {1, j}, {2, j}};
    if (b.grid[0][0].getmark() == symbol && b.grid[1][1].getmark() == symbol && b.grid[2][2].getmark() == symbol)
        return {{0, 0}, {1, 1}, {2, 2}};
    if (b.grid[0][2].getmark() == symbol && b.grid[1][1].getmark() == symbol && b.grid[2][0].getmark() == symbol)
        return {{0, 2}, {1, 1}, {2, 0}};
    return {};
}

// ============================ MAZE RUNNER SECTION ===============================

class Maze
{
    int rows, cols;
    vector<vector<char>> grid;
    pair<int, int> start, end;
    vector<pair<int, int>> dirs;

    bool isValid(int r, int c) { return r > 0 && c > 0 && r < rows - 1 && c < cols - 1; }

    void dfs(int r, int c)
    {
        random_shuffle(dirs.begin(), dirs.end());
        for (auto &d : dirs)
        {
            int nr = r + d.first;
            int nc = c + d.second;
            if (isValid(nr, nc) && grid[nr][nc] == '#')
            {
                grid[r + d.first / 2][c + d.second / 2] = ' ';
                grid[nr][nc] = ' ';
                dfs(nr, nc);
            }
        }
    }

public:
    Maze(int r = 15, int c = 15) : rows(r), cols(c)
    {
        if (rows % 2 == 0)
            rows++;
        if (cols % 2 == 0)
            cols++;
        grid.resize(rows, vector<char>(cols, '#'));
        dirs = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
    }

    void generate()
    {
        srand(time(0));
        grid[1][1] = ' ';
        dfs(1, 1);
        start = {1, 1};
        end = {rows - 2, cols - 2};
        grid[start.first][start.second] = 'S';
        grid[end.first][end.second] = 'E';
    }

    int shortestPath() const
    {
        vector<vector<int>> dist(rows, vector<int>(cols, -1));
        queue<pair<int, int>> q;
        q.push(start);
        dist[start.first][start.second] = 0;
        vector<pair<int, int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        while (!q.empty())
        {
            pair<int, int> current = q.front();
            q.pop();
            int r = current.first;
            int c = current.second;

            if (make_pair(r, c) == end)
                return dist[r][c];

            for (auto &mv : moves)
            {
                int nr = r + mv.first, nc = c + mv.second;
                if (nr >= 0 && nc >= 0 && nr < rows && nc < cols &&
                    dist[nr][nc] == -1 &&
                    (grid[nr][nc] == ' ' || grid[nr][nc] == 'E'))
                {
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push(make_pair(nr, nc));
                }
            }
        }
        return -1;
    }

    void print(pair<int, int> pos) const
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (make_pair(i, j) == pos)
                    cout << "P";
                else
                    cout << grid[i][j];
            }
            cout << endl;
        }
    }

    bool isEmpty(int r, int c) const { return grid[r][c] == ' ' || grid[r][c] == 'E'; }
    pair<int, int> getStart() const { return start; }
    pair<int, int> getEnd() const { return end; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    char at(int i, int j) const { return grid[i][j]; }
};

class MazeRunner
{
    Maze *maze;
    pair<int, int> pos;
    string name;

public:
    MazeRunner(int r, int c, string n)
    {
        maze = new Maze(r, c);
        maze->generate();
        name = n;
        pos = maze->getStart();
    }
    ~MazeRunner() { delete maze; }

    void play()
    {
        int minMoves = maze->shortestPath();
        cout << "\nMinimum moves to solve: " << minMoves << "\n";
        cout << "\nUse W/A/S/D then Enter to move.\n";

        int moveCount = 0;
        clock_t startTime = clock();
        while (true)
        {
            maze->print(pos);
            cout << "\nYour move: ";
            char ch;
            cin >> ch;
            int r = pos.first, c = pos.second;
            if (ch == 'w' || ch == 'W')
                r--;
            else if (ch == 's' || ch == 'S')
                r++;
            else if (ch == 'a' || ch == 'A')
                c--;
            else if (ch == 'd' || ch == 'D')
                c++;

            if (maze->isEmpty(r, c))
                pos = {r, c};
            moveCount++;

            if (pos == maze->getEnd())
            {
                clock_t endTime = clock();
                double timeTaken = double(endTime - startTime) / CLOCKS_PER_SEC;
                cout << "\n🎉 You reached the exit! 🎉\n";
                cout << "Minimum possible moves: " << minMoves << endl;
                cout << "Your total moves: " << moveCount << endl;
                cout << "Time taken: " << timeTaken << " sec\n";
                break;
            }
            system("clear"); // works on mac/linux; ignored on windows
            system("cls");   // works on windows; ignored on mac/linux
        }
    }
};

// ============================ KNIGHT’S TOUR SECTION ===============================

class KnightsTour
{
    int N;
    pair<int, int> knight;
    vector<pair<int, int>> moves;

public:
    KnightsTour(int n) : N(n)
    {
        knight = {0, 0};
        moves = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
    }

    void showBoard()
    {
        vector<vector<char>> board(N, vector<char>(N, '.'));
        board[knight.first][knight.second] = 'K';

        cout << "\nPossible moves from current position:\n";
        for (auto &m : moves)
        {
            int nr = knight.first + m.first, nc = knight.second + m.second;
            if (nr >= 0 && nc >= 0 && nr < N && nc < N)
                board[nr][nc] = '*';
        }

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                cout << board[i][j] << " ";
            cout << endl;
        }
    }
};

// =============================== MAIN MENU ===============================

int main()
{
    while (true)
    {
        cout << "\n===== GAME HUB =====\n";
        cout << "1. Tic Tac Toe\n";
        cout << "2. Maze Runner\n";
        cout << "3. Knight’s Tour (Static)\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            string p1, p2;
            cout << "Enter Player1 name: ";
            cin >> p1;
            cout << "Enter Player2 name: ";
            cin >> p2;
            Board b;
            int moves = 0;
            char sym = 'X';
            while (moves < 9)
            {
                b.display();
                cout << (sym == 'X' ? p1 : p2) << " (" << sym << ") enter position (1-9): ";
                int pos;
                cin >> pos;
                if (!b.place(pos, sym))
                    cout << "Invalid move!\n";
                else
                {
                    auto win = WinLine(b, sym);
                    if (!win.empty())
                    {
                        b.display(win);
                        cout << (sym == 'X' ? p1 : p2) << " wins!\n";
                        break;
                    }
                    sym = (sym == 'X') ? 'O' : 'X';
                    moves++;
                }
            }
        }
        else if (choice == 2)
        {
            string name;
            cout << "Enter your name: ";
            cin >> name;
            int r, c;
            cout << "Enter maze size (odd numbers, e.g. 15 15): ";
            cin >> r >> c;
            MazeRunner game(r, c, name);
            game.play();
        }
        else if (choice == 3)
        {
            int n;
            cout << "Enter board size (e.g., 8): ";
            cin >> n;
            KnightsTour kt(n);
            kt.showBoard();
        }
        else if (choice == 4)
        {
            cout << "Exiting... Bye!\n";
            break;
        }
        else
            cout << "Invalid choice!\n";
    }
    return 0;
}