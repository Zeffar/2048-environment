// For_Fun attempt at making a decision tree using a MCTS approach. This needs to be ran only once to generate the decision tree, which it stores as a lookup table
// The memory is dynamically allocated in this approach
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <cmath>         
#include <algorithm>
#include <iostream>
#include <cstdlib> 
#include <time.h> 
using namespace std;
string file;

struct node {
    int board[4][4];
    int parent_move;
    int score;
    node* parent;
    vector<node*> children;
    int visits;
    int win_score;

    //constructor
    node(int a[4][4], int move, int evalScore) : 
        parent_move(move), score(evalScore), 
        parent(nullptr), visits(0), win_score(0) {

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                this -> board[i][j] = a[i][j];
        }

    //default constructor
    node() : 
        parent_move(-1), score(0), 
        parent(nullptr), visits(0), win_score(0) {

            for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                this -> board[i][j] = 0;
        }
    
    //deconstructor
    ~node() {
        for (auto child : children)
            delete child;
    }
 
};
void MCTS(node* root);
void move_up(bool &ok, int a[4][4]);
void move_right(bool &ok, int a[4][4]);
void move_down(bool &ok, int a[4][4]);
void move_left(bool &ok, int a[4][4]);
void simulateRandomPlayout(node* node);
bool next_board(int o_board[4][4], int i_board[4][4], int move);
void generate_2(int board[4][4]);
bool cmp(node a, node b) {
    return (a.score > b.score);
}

void debug(node* state) {
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j)
            cout<<state->board[i][j]<<" ";
        cout<<endl;
    }
}



int main(int argc, char* argv[]) {
    if (argc!=1) {
        cout<<"No input file given, defaulting to lookup.txt\n";
        file="lookup.txt";
    }
    else file = argv[0];
    node* root = new node();
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            for(int ii=i; ii<4; ++ii) {
                for(int jj=j; jj<4; ++jj) {
                    if(ii==i && jj==j) continue;
                    node* current_node = new node();
                    current_node->board[i][j]=2;
                    current_node->board[ii][jj]=2;
                    root->children.push_back(current_node);
                }
            }
        }
    }
    MCTS(root);

}

void MCTS(node *root) {
    
}

void generate_2(int board[4][4]){
    //parse the board, find all places which are empty, choose randomly between them, insert a 2 in said place
    int free_places[16], nr_places=0, counter=0;

    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j){
            if(board[i][j]==0){
              free_places[nr_places] = counter; //keep in a vector the indices of the free spaces, as if marking each square of the matrix ascendigly
              nr_places++;//count free spaces found
            }
            counter++;//count at which square we are
        }
    int random = rand() % nr_places; //will give a random nr in range of the empty spaces
    board[free_places[random]/4][free_places[random]%4] = 2; //said square will become 2
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

