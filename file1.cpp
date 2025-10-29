#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <conio.h> // for _getch() on Windows
using namespace std;

// =========================== TIC TAC TOE SECTION ===============================

// -------------------------Box Class-----------------------------
class Box
{
    char mark;

public:
    Box() { mark = ' '; }
    char getmark() { return mark; }
    void setmark(char m) { mark = m; }
    bool isempty() { return mark == ' '; }
};

// --------------------------Board Class-----------------------------
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
                for (size_t k = 0; k < winCell.size(); k++)
                {
                    if (winCell[k].first == i && winCell[k].second == j)
                    {
                        winBox = true;
                        break;
                    }
                }
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

// -------------------------- Maze Class --------------------------
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
        for (size_t i = 0; i < dirs.size(); i++)
        {
            int nr = r + dirs[i].first;
            int nc = c + dirs[i].second;
            if (isValid(nr, nc) && grid[nr][nc] == '#')
            {
                grid[r + dirs[i].first / 2][c + dirs[i].second / 2] = ' ';
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
    ~Maze() {}

    void generate()
    {
        srand(time(0));
        grid[1][1] = ' ';
        dfs(1, 1);
        start = make_pair(1, 1);
        end = make_pair(rows - 2, cols - 2);
        grid[start.first][start.second] = 'S';
        grid[end.first][end.second] = 'E';
    }

    // BFS shortest path computation (fixed for C++14)
    int shortestPath() const
    {
        vector<vector<int>> dist(rows, vector<int>(cols, -1));
        queue<pair<int, int>> q;
        q.push(start);
        dist[start.first][start.second] = 0;
        vector<pair<int, int>> moves;
        moves.push_back(make_pair(1, 0));
        moves.push_back(make_pair(-1, 0));
        moves.push_back(make_pair(0, 1));
        moves.push_back(make_pair(0, -1));

        while (!q.empty())
        {
            pair<int, int> curr = q.front();
            q.pop();
            int r = curr.first;
            int c = curr.second;

            if (make_pair(r, c) == end)
                return dist[r][c];

            for (size_t i = 0; i < moves.size(); i++)
            {
                int nr = r + moves[i].first;
                int nc = c + moves[i].second;
                if (nr >= 0 && nc >= 0 && nr < rows && nc < cols && dist[nr][nc] == -1 && (grid[nr][nc] == ' ' || grid[nr][nc] == 'E'))
                {
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push(make_pair(nr, nc));
                }
            }
        }
        return -1;
    }

    friend ostream &operator<<(ostream &out, const Maze &m)
    {
        for (int i = 0; i < m.rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
                out << m.grid[i][j];
            out << endl;
        }
        return out;
    }

    char operator()(int i, int j) const { return grid[i][j]; }
    pair<int, int> getStart() const { return start; }
    pair<int, int> getEnd() const { return end; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    bool isEmpty(int r, int c) const { return grid[r][c] == ' ' || grid[r][c] == 'E'; }
};

// -------------------------- Player Base Class --------------------------
class Player
{
protected:
    pair<int, int> pos;
    string name;

public:
    Player(string n = "Player") : name(n) {}
    virtual ~Player() {}
    void setStart(pair<int, int> s) { pos = s; }
    pair<int, int> getPos() const { return pos; }
    virtual pair<int, int> makeMove(const Maze &maze) = 0;
};

// -------------------------- Human Player --------------------------
class HumanPlayer : public Player
{
public:
    HumanPlayer(string n = "Player") : Player(n) {}
    pair<int, int> makeMove(const Maze &maze) override
    {
        char ch = _getch();
        int r = pos.first, c = pos.second;
        if (ch == 'w' || ch == 'W')
            r--;
        else if (ch == 's' || ch == 'S')
            r++;
        else if (ch == 'a' || ch == 'A')
            c--;
        else if (ch == 'd' || ch == 'D')
            c++;
        if (maze.isEmpty(r, c))
            pos = make_pair(r, c);
        return pos;
    }
};

// -------------------------- MazeRunner Game Controller --------------------------
class MazeRunner
{
    Maze *maze;
    Player *player;

public:
    MazeRunner(int r, int c, string name)
    {
        maze = new Maze(r, c);
        maze->generate();
        player = new HumanPlayer(name);
        player->setStart(maze->getStart());
    }
    ~MazeRunner()
    {
        delete maze;
        delete player;
    }

    void play()
    {
        int minMoves = maze->shortestPath();
        cout << "\nGenerated Maze (Minimum moves to solve: " << minMoves << ")\n\n";
        cout << *maze;
        cout << "\nPress any key to start moving...\n";
        _getch();

        int moveCount = 0;           // count total moves made by player
        clock_t startTime = clock(); // start timer

        while (true)
        {
            system("cls");
            for (int i = 0; i < maze->getRows(); i++)
            {
                for (int j = 0; j < maze->getCols(); j++)
                {
                    if (player->getPos() == make_pair(i, j))
                        cout << "P";
                    else
                        cout << (*maze)(i, j);
                }
                cout << endl;
            }

            cout << "\nUse W/A/S/D to move.\n";
            player->makeMove(*maze);
            moveCount++;

            if (player->getPos() == maze->getEnd())
            {
                system("cls");
                clock_t endTime = clock();
                double timeTaken = double(endTime - startTime) / CLOCKS_PER_SEC;

                cout << "\n🎉 Congratulations! You reached the exit! 🎉\n";
                cout << "--------------------------------------------\n";
                cout << "Minimum possible moves: " << minMoves << endl;
                cout << "Your total moves:       " << moveCount << endl;
                cout << "Time taken:             " << timeTaken << " seconds\n";

                // scoring formula ---
                // Higher base score, softer penalties for time and moves
                double efficiency = (double)minMoves / moveCount;
                if (efficiency > 1.0)
                    efficiency = 1.0;

                // Base points + efficiency bonus - light time penalty
                int score = (int)(70 + efficiency * 30 - (timeTaken * 0.7));

                // Cap between 0 and 100
                if (score > 100)
                    score = 100;
                if (score < 0)
                    score = 0;

                cout << "--------------------------------------------\n";
                cout << " Final Score: " << score << " / 100\n";
                cout << "--------------------------------------------\n";
                break;
            }
        }
    }
};

// =============================== MAIN MENU ===============================
int main()
{
    int choice;
    cout << "Enter your choice accordingly\n";
    cout << "1. Tic Tac Toe\n2. Maze Runner\n3. Knight Tour (WIP)\n\n";
    cin >> choice;
    cout << endl;

    if (choice == 1)
    {
        string username1, username2;
        cout << "Enter Player1 name: ";
        cin >> username1;
        cout << "\nEnter Player2 name: ";
        cin >> username2;
        int series;
        cout << "\nBest of series (1/3/5): ";
        cin >> series;
        cout << endl;

        int minwins = (series / 2) + 1, p1Score = 0, p2Score = 0, round = 0;
        while (p1Score < minwins && p2Score < minwins && round < series)
        {
            round++;
            cout << "--------------- Round " << round << " ---------------\n\n";
            Board bd;
            int movesCount = 0;
            char currSymbol = (round % 2 == 0) ? 'O' : 'X';
            string currentPlayer;

            while (movesCount < 9)
            {
                bd.display();
                currentPlayer = (currSymbol == 'X') ? username1 : username2;
                cout << endl
                     << currentPlayer << " (" << currSymbol << ") Enter position (1-9): ";
                int position;
                cin >> position;
                cout << endl;
                if (!bd.place(position, currSymbol))
                    cout << "Not a valid move! Try again.\n";
                else
                {
                    vector<pair<int, int>> winCell = WinLine(bd, currSymbol);
                    if (!winCell.empty())
                    {
                        bd.display(winCell);
                        cout << endl
                             << currentPlayer << " wins round " << round << "!\n\n";
                        if (currSymbol == 'X')
                            p1Score++;
                        else
                            p2Score++;
                        break;
                    }
                    currSymbol = (currSymbol == 'X') ? 'O' : 'X';
                    movesCount++;
                }
            }
            if (movesCount == 9)
            {
                bd.display();
                cout << "It's a draw!\n";
            }
        }
        cout << "Final Result: ";
        if (p1Score > p2Score)
            cout << username1 << " wins the series!\n\n";
        else if (p1Score < p2Score)
            cout << username2 << " wins the series!\n\n";
        else
            cout << "Series Draw!\n";
    }
    else if (choice == 2)
    {
        string name;
        cout << "Enter your name: ";
        cin >> name;
        int r, c;
        cout << "Enter maze dimensions (odd numbers recommended, e.g. 15 15): ";
        cin >> r >> c;
        MazeRunner game(r, c, name);
        game.play();
    }
    else
        cout << "Work in progress.\n";
    return 0;
}