#include <iostream>
#include <vector>
using namespace std;

class box {
    char nishaan;
public:
    box(){
        nishaan=' '; 
}
    char get() const { return nishaan; }
    void set(char m) { nishaan = m; }
};

class board {
    vector<vector<box>> grid;
public:
    board() {
        grid=vector<vector<box>>(3, vector<box>(3));
    }
    void render_with_numbers() {
        for (int i=0;i<3;i++) {
            for (int j=0;j<3;j++) {
                cout<<"x";
                if (j<2) cout<<" | ";
            }
            cout<<"\n";
            if(i<2) cout<<"--+---+--\n";
        }
    }
};

int main() {
    board bd;
    bd.render_with_numbers();
    return 0;
}
