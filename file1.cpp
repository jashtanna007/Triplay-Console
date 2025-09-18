#include <iostream>
#include <vector>
using namespace std;

// -------------------------Box Class-----------------------------

class Box
{
    char mark;

public:
    Box()
    {
        mark = ' ';
    }
    char getmark()
    {
        return mark;
    }
    void setmark(char m)
    {
        mark = m;
    }
    bool isempty()
    {
        if (mark == ' ')
            return true;
        return false;
    }
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
        for (int i = 0; i < size; i++) // create a 3 by 3 board
        {
            grid[i] = new Box[size];
        }
    }

    ~Board()
    {
        for (int k = 0; k < size; k++)
        {
            delete[] grid[k];
        }
        delete[] grid;
    }

    void display(vector<pair<int, int>> winCell = {})
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                bool winBox = false;
                for (auto &p : winCell)
                {
                    if (p.first == i && p.second == j)
                    {
                        winBox = true;
                        break;
                    }
                }

                char m = grid[i][j].getmark();
                if (m == ' ')
                {
                    cout << "   ";
                }
                else if (winBox)
                {
                    cout << " \033[32m" << m << "\033[0m ";
                }
                else
                {
                    cout << " " << m << " ";
                }
                if (j < 2)
                {
                    cout << " | ";
                }
            }
            cout << endl;
            if (i < 2)
            {
                cout << "----#-----#----";
            }
            cout << endl;
        }
    }

    bool place(int pos, char symbol)
    {
        int row = (pos - 1) / 3;
        int col = (pos - 1) % 3;
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
    {
        if (b.grid[i][0].getmark() == symbol &&
            b.grid[i][1].getmark() == symbol &&
            b.grid[i][2].getmark() == symbol)
            return {{i, 0}, {i, 1}, {i, 2}};
    }

    for (int j = 0; j < 3; j++)
    {
        if (b.grid[0][j].getmark() == symbol &&
            b.grid[1][j].getmark() == symbol &&
            b.grid[2][j].getmark() == symbol)
            return {{0, j}, {1, j}, {2, j}};
    }

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

// --------------------------int main------------------------------

int main()
{

    int choice;
    cout << "Enter your choice accordingly\n";
    cout << "1.tic tac toe\n2.Maze runner\n3.knight tour\n\n";
    cin >> choice;
    cout << endl;
    if (choice == 1)
    {

        string username1;
        string username2;
        cout << "ENter Player1 name: ";
        cin >> username1;
        cout << "\nENter Player2 name: ";
        cin >> username2;
        int series;
        cout << "\nbest of series(1/3/5): ";
        cin >> series;
        cout << endl;

        int minwins = (series / 2) + 1;
        int p1Score = 0, p2Score = 0;
        int round = 0;
        while (p1Score < minwins && p2Score < minwins && round < series)
        {
            round++;
            cout << "---------------Round " << round << "---------------\n\n";
            Board bd;
            int movesCount = 0;
            char currSymbol = 'X';
            if (round % 2 == 0)
            {
                currSymbol = 'O';
            }
            else
            {
                currSymbol = 'X';
            }

            string currentPlayer;

            while (movesCount < 9)
            {

                bd.display();
                if (currSymbol == 'X')
                {
                    currentPlayer = username1;
                }
                else
                {
                    currentPlayer = username2;
                }
                cout << endl
                     << currentPlayer << "(" << currSymbol << ")" << "Enter position (1-9): ";
                int position;
                cin >> position;
                cout << endl
                     << endl;
                if (!bd.place(position, currSymbol))
                {
                    cout << "Not a valid move !!! Enter a correct \n";
                }
                else
                {
                    vector<pair<int, int>> winCell = WinLine(bd, currSymbol);
                    if (!winCell.empty())
                    {
                        bd.display(winCell);
                        cout << endl
                             << currentPlayer << " WINS round " << round << "!\n\n";
                        if (currSymbol == 'X')
                            p1Score++;
                        else
                            p2Score++;
                        break;
                    }

                    if (currSymbol == 'X')
                        currSymbol = 'O';
                    else
                        currSymbol = 'X';

                    movesCount++;
                }
            }
            if (movesCount == 9)
            {
                bd.display();
                cout << "It's a draw!\n";
            }
        }
        cout << "final result : ";
        if (p1Score > p2Score)
        {
            cout << username1 << " wins the series!\n\n";
        }
        else if (p1Score < p2Score)
        {
            cout << username2 << " wins the series!\n\n";
        }
        else
        {
            cout << "Series Draw!\n";
        }
    }
    else
    {
        cout << "work in progress\n";
    }

    return 0;
}