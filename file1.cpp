#include <iostream>
using namespace std;

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

class Board
{
     Box** grid;
    int size;

public:

 Board(){
        size=3;
        grid = new Box*[size];
        for(int i = 0; i < size; i++) {
            grid[i] = new Box[size]; 
        }
    }

    ~Board() { 
        for(int grp23 = 0; grp23 < size; grp23++) {
            delete[] grid[grp23];
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
};

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
        // cout << "\nthe x is printing in the first row\n";
        // bd.place(1, 'X');
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
                if(currSymbol == 'X' ){
                    currSymbol = 'O';
                }
                else{
                    currSymbol = 'X';
                }
                bd.place(position, currSymbol);
                movesCount++;
            }
        }

        bd.display();
    }
    else
    {
        cout << "work in progress\n";
    }

    return 0;
}