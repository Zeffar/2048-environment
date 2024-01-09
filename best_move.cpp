#include <fstream>
#include <vector>
#include <cmath>                                      ////old version that uses files!!!!!!!!!!!
#include <algorithm>
#include <iostream>
using namespace std;
ofstream g("output.txt");
void move_up(bool &ok, int a[4][4]);
void move_right(bool &ok, int a[4][4]);
void move_down(bool &ok, int a[4][4]);
void move_left(bool &ok, int a[4][4]);
void make_tree(string b, int depth, int width);
int evaluate(int board[4][4]);
void next_board(string s);

int main()
{
    int tree_depth=20, tree_width=50;
    //here I should create the tree using two vectors with min max beam search and then choose the children using the eval function
    make_tree("tree.txt",tree_depth,tree_width); //reads a board(state) from tree.txt and will return last row with thier root move 
    return 0;
}

struct node {
    int board[4][4];
    int parent;
};

void make_tree(string b, int depth, int width){

    int tree[2][width], x; //change to node later
    ifstream brd(b);
    int board[4][4] = {{0,0,0,0},
                       {0,0,0,0},
                       {0,0,0,0},
                       {0,0,0,0}};

    for (int i = 0; i < 4; ++i) {  //read board from file
        for (int j = 0; j < 4; ++j) {
            brd>>x;
            if (x){
                board[i][j]=int(log2(x));
            }
        }
    }

    for (int p = 0; p < 1; p++) { //loop to test if there is a new parent for each direction
        ofstream next_node("input.txt");
        for (int i = 0; i < 4; ++i) {  //write the board with each move from 0-3 in input.txt to get the files for the parent boards
           for (int j = 0; j < 4; ++j) {

               if (board[i][j]){
                   next_node<<(1<<board[i][j])<<" "; //shift back the log2
               }
               else next_node<<"0 ";
           }
           next_node<<'\n';
        }
        next_node<<p;//add the move
        next_node<<'\n';
        next_node.close();
        next_board("input.txt");
    }
    

    for (int r=0; r<depth; r++){ //every row

    }

    brd.close();

    //read from b, them modify input.txt repeatedly and use next_board to get the children of each node one by one
}

int evaluate(int board[4][4]){

    long score=0;

	int w_matrix[4][4]=    //snake-like pattern for the values, aims to have the highest number in the down left corner
    {
	         {0, 1, 2, 3},
             {7, 6, 5, 4},
             {8, 9, 10, 11},
             {15, 14, 13, 12}
    };

    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j){

            score += static_cast<int>(log2(board[i][j])) * w_matrix[i][j];  

        }

    // reward: ajacent conscutive numbers (snake shaped), many blank spaces, possible merges, punish: not enough blanks
    return score;

}

void next_board(string s){  //given an input file with a state and a move, writes in output.txt how the next board will look like, if valid
    int a[4][4] = {{0,0,0,0},
                   {0,0,0,0},
                   {0,0,0,0},
                   {0,0,0,0}};
    ifstream f(s);
    int x{};
    for (int i = 0; i < 4; ++i) {  //read board from file
        for (int j = 0; j < 4; ++j) {
            f>>x;
            if(x) a[i][j]=int(log2(x));
        }
    }
    int move;
    f>>move; //read move
    cout<<move;
    bool legal_move=0;
    switch(move)
    {
        case 0: move_up(legal_move, a); break;
        case 1: move_right(legal_move, a); break;
        case 2: move_down(legal_move, a); break;
        case 3: move_left(legal_move, a); break;
    }

    if (legal_move) g<<"1\n"; //here write in output file 1 if the move is legal and -1 if it isn't
    else {g<<"-1\n";}//return -1;}

    for (int i = 0; i < 4; ++i) { //write modified board in output file
        for (int j = 0; j < 4; ++j) {
            if(a[i][j])
                g<<(1<<a[i][j])<<" ";
            else g<<"0 ";
        }
        g<<'\n';
    }
    f.close();
    //return 0;
}
//move functions
void move_left(bool &ok, int a[4][4]){
    for(int i=0; i<4; i++){
        int n=0;
        int prev=0;
        for (int j=0; j<4; j++)
        {
            if (n==a[i][j] && n!=0){ ok=1;
                a[i][prev] = n+1;
                a[i][j] = 0;
                n = 0;
                continue;
            }
            if (a[i][j]!=0){
                n = a[i][j];
                prev = j;
            }
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<3; k++){
                if(a[i][k]==0 && a[i][k+1]!=0){ ok=1;
                    a[i][k]=a[i][k]^a[i][k+1];
                    a[i][k+1]=a[i][k]^a[i][k+1];
                    a[i][k]=a[i][k]^a[i][k+1];
                }
            }
        }
    }
}

void move_right(bool &ok, int a[4][4]){
    for(int i=0; i<4; i++){
        int n=0;
        int prev=0;
        for (int j=3; j>=0; j--)
        {
            if (n==a[i][j] && n!=0){ ok=1;
                a[i][prev] = 1+n;
                a[i][j] = 0;
                n = 0;
                continue;
            }
            if (a[i][j]!=0){
                n = a[i][j];
                prev = j;
            }
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=3; k>0; k--){
                if(a[i][k]==0 && a[i][k-1]!=0){ ok=1;
                    a[i][k]=a[i][k]^a[i][k-1];
                    a[i][k-1]=a[i][k]^a[i][k-1];
                    a[i][k]=a[i][k]^a[i][k-1];
                }
            }
        }
    }

}

void move_up(bool &ok, int a[4][4]){
    for(int i=0; i<4; i++){
        int n=0;
        int prev=0;
        for (int j=0; j<4; j++)
        {
            if (n==a[j][i] && n!=0){ ok=1;
                a[prev][i] = 1+n;
                a[j][i] = 0;
                n = 0;
                continue;
            }
            if (a[j][i]!=0){
                n = a[j][i];
                prev = j;
            }
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<3; k++){
                if(a[k][i]==0 && a[k+1][i]!=0){ ok=1;
                    a[k][i]=a[k][i]^a[k+1][i];
                    a[k+1][i]=a[k][i]^a[k+1][i];
                    a[k][i]=a[k][i]^a[k+1][i];
                }
            }
        }
    }

}

void move_down(bool &ok, int a[4][4]){
    for(int i=0; i<4; i++){
        int n=0;
        int prev=0;
        for (int j=3; j>=0; j--)
        {
            if (n==a[j][i] && n!=0){ ok=1;
                a[prev][i] = 1+n;
                a[j][i] = 0;
                n = 0;
                continue;
            }
            if (a[j][i]!=0){
                n = a[j][i];
                prev = j;
            }
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=3; k>0; k--){
                if(a[k][i]==0 && a[k-1][i]!=0){ ok=1;
                    a[k][i]=a[k][i]^a[k-1][i];
                    a[k-1][i]=a[k][i]^a[k-1][i];
                    a[k][i]=a[k][i]^a[k-1][i];
                }
            }
        }
    }

}
