#include <fstream>
#include <vector>
#include <cmath>              //////////////modified all functions to not use files anymore, but nodes
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
bool next_board(int o_board[4][4], int i_board[4][4], int move);

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

void make_tree(string b, int depth, int width){ //reads a state from a file and gives the next best move

    node tree[2][width]; //2 rows for list of current states of the board and for list of board with the 2s generated on them
    int x; 
    short parent_nr = 0; //to count how many parents of the big 4 are valid

    ifstream brd(b);
    int board[4][4] = {{0,0,0,0},
                       {0,0,0,0},
                       {0,0,0,0},
                       {0,0,0,0}};


    for (int i = 0; i < 4; ++i)  //read board from file 
        for (int j = 0; j < 4; ++j) {
            brd>>x;
            if (x)
                board[i][j]=int(log2(x));
        }

    x = next_board(tree[0][parent_nr].board,board,0); //will write in 'tree[0][parent_nr].board', the board for the very first node in the first row of the tree, the modified 'board' after move '0' and x will verify if the fct outputed true or false
    if (x) {
        tree[0][parent_nr].parent = 1;
        parent_nr++; //goes ot the next node of the first row of the tree
    }
    x = next_board(tree[0][parent_nr].board,board,1);
    if (x) {
        tree[0][parent_nr].parent = 2;
        parent_nr++;
    }
    x = next_board(tree[0][parent_nr].board,board,2);
    if (x) {
        tree[0][parent_nr].parent = 3;
        parent_nr++;
    }
    x = next_board(tree[0][parent_nr].board,board,3);
    if (x) {
        tree[0][parent_nr].parent = 4;
        parent_nr++;
    }

    cout<<parent_nr;
    //parent_nr will keep the number of valid parents, ex 3 means 3 valid parents

    //  for (int i = 0; i < 4; ++i) {  //write in parents
    //     for (int j = 0; j < 4; ++j) {
    //             p1.board[i][j] = board1[i][j];
    //             p2.board[i][j] = board2[i][j];
    //             p3.board[i][j] = board3[i][j];
    //             p4.board[i][j] = board4[i][j];
    //     }
    // }
    

    // for (int r=0; r<depth; r++){ //every row

    // }

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

bool next_board(int o_board[4][4], int i_board[4][4], int move){  //given an input board, an output board and a move, writes in the output board how the next board will look like, returning 1 if valid and 0 if not
    
    int a[4][4] = {{0,0,0,0},
                   {0,0,0,0},
                   {0,0,0,0},
                   {0,0,0,0}};

    for (int i = 0; i < 4; ++i) //copy input board in a
        for (int j = 0; j < 4; ++j) 
            a[i][j]=i_board[i][j];

    bool legal_move=0;

    switch(move)
    {
        case 0: move_up(legal_move, a); break; //legal move will stay 0 if the move is invalid and 1 if valid, and a will change accordingly
        case 1: move_right(legal_move, a); break;
        case 2: move_down(legal_move, a); break;
        case 3: move_left(legal_move, a); break;
    }

    if(legal_move) //write modified board in output board if move valid
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                o_board[i][j] = a[i][j];

    return legal_move;
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
