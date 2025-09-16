#include <iostream>
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

    void display()
    {
        int index = 1;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (grid[i][j].isempty())
                {
                    cout << " ";
                }
                else
                {
                    cout << grid[i][j].getmark();
                }
                if (j < 2)
                {
                    cout << " | ";
                }
                index++;
            }
            cout << endl;
            if (i < 2)
            {
                cout << "--#---#--";
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

    friend bool checkWinner(Board &b, char symbol);
};

bool checkWinner(Board &b, char symbol)
{
    for (int i = 0; i < 3; i++)
    {
        if (b.grid[i][0].getmark() == symbol &&
            b.grid[i][1].getmark() == symbol &&
            b.grid[i][2].getmark() == symbol)
            return true;
    }

    for (int j = 0; j < 3; j++)
    {
        if (b.grid[0][j].getmark() == symbol &&
            b.grid[1][j].getmark() == symbol &&
            b.grid[2][j].getmark() == symbol)
            return true;
    }

    if (b.grid[0][0].getmark() == symbol &&
        b.grid[1][1].getmark() == symbol &&
        b.grid[2][2].getmark() == symbol)
        return true;

    if (b.grid[0][2].getmark() == symbol &&
        b.grid[1][1].getmark() == symbol &&
        b.grid[2][0].getmark() == symbol)
        return true;

    return false;
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
        Board bd;
        int movesCount = 0;
        char currSymbol = 'X';
        string username1;
        string username2;
        cout << "ENter Player1 name \n";
        cin >> username1;
        cout << "ENter Player2 name \n";
        cin >> username2;

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
                 << currentPlayer << "(" << currSymbol << ")" << "Enter position (1-9)" << endl;
            int position;
            cin >> position;
            if (!bd.place(position, currSymbol))
            {
                cout << "Not a valid move !!! Enter a correct \n";
            }
            else
            {
                if (checkWinner(bd, currSymbol))
                {
                    bd.display();
                    cout << currentPlayer << " WINS!\n";
                    return 0;
                }

                if (currSymbol == 'X')
                    currSymbol = 'O';
                else
                    currSymbol = 'X';

                movesCount++;
            }
        }

        bd.display();
        cout << "It's a draw!\n";
    }
    else
    {
        cout << "work in progress\n";
    }

    return 0;
}