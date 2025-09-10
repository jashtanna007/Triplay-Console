#include <iostream>
#include <vector>
using namespace std;

class box
{
    char nishaan;

public:
    box() { nishaan = ' '; }
    char get() const { return nishaan; }
    void set(char m) { nishaan = m; }
    bool empty() const { return nishaan == ' '; }
};

class board
{
    vector<vector<box>> grid;

public:
    board() { grid=vector<vector<box>>(3, vector<box>(3)); }
    void buildblock()
    {
        int pos = 1;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (grid[i][j].empty())
                {
                    cout << pos;
                }
                else
                {
                    cout << grid[i][j].get();
                }
                if (j < 2)
                    cout << " | ";
                pos++;
            }
            cout << "\n";
            if (i < 2)
                cout << "--+---+--\n";
        }
    }
    void place(int position, char symbol)
    {
        int row = (position - 1) / 3;
        int col = (position - 1) % 3;
        if (position >= 1 && position <= 9 && grid[row][col].empty())
        {
            grid[row][col].set(symbol);
        }
    }
};

int main()
{
    board bd;
    bd.buildblock();
    return 0;
}
