// Attempt at making a decision tree using a pure MCTS approach.
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

struct node {
    int board[4][4];
    int parent_move;
    int score;
    node* parent;
    vector<node*> children;
    int visits;
    int moves;
    long long total_score;
    //constructor
    node(int a[4][4], int move, int evalScore, node* parent) : 
        parent_move(move), score(evalScore), 
        parent(nullptr), visits(0), moves(0),  total_score(0) {

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                this -> board[i][j] = a[i][j];
        
        this -> parent = parent;
        }

    //default constructor
    node() : 
        parent_move(-1), score(0), 
        parent(nullptr), visits(0), moves(0), total_score(0) {

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

unordered_map<int, int> table_score = {
    {1, 0},
    {2, 4},
    {3, 16},
    {4, 48},
    {5, 128},
    {6, 320},
    {7, 768},
    {8, 1792},
    {9, 4096},
    {10, 9216},
    {11, 20480},
    {12, 45056},
    {13, 98304}
};

void move_up(bool &ok, int a[4][4]);
void move_right(bool &ok, int a[4][4]);
void move_down(bool &ok, int a[4][4]);
void move_left(bool &ok, int a[4][4]);
int score(node* state);
int simulate_random_play(node* node, int& root_move);
// int simulate_random_play_movecount(node* node, int& root_move);  Plays for longer survival, not for better score, yields simmilar results.
//if you want to try it, change to this in MCTS() and uncomment the function;
void generate_2(int board[4][4]);
int MCTS (node* root);
bool random_sort(int a, int b) {
    return rand()%2;
}
void debug(node* state) {
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j)
            if(state->board[i][j])
                cout<<(1<<state->board[i][j])<<" ";
            else cout<<"0 ";
        cout<<endl;
    }
}

int main(int argc, char* argv[]) {

    string file = "Z:/GitHub/2048-environment/decision_tree/tree.txt";
    // if(argc>0) {
    //     file = argv[0];
    // }
    ifstream read_tree(file);
    srand(time(0));

    node* root = new node();
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            int x; read_tree >> x;
            if(x) root -> board[i][j] = static_cast<int>(log2(x));
        }
    }
    cout << MCTS(root);
}

int MCTS (node* root) {
    int zerocount=0;
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            if(root->board[i][j]==0) zerocount++;
    int runs=0;
    if(zerocount > 7) runs = 1000;
    else if (zerocount > 2) runs = 3000;
    else runs = 5000;

    long long scores[4] = {0, 0, 0, 0};
    long long visits[4] = {0, 0, 0, 0};
    for(int iterations = 0; iterations < runs; ++iterations) {
        int first_move = -1;
        int result = simulate_random_play(root, first_move);
        scores[first_move] += result;
        visits[first_move] += 1;
    }
    
    int best_score = -1, move = -1;
    for(int i=0; i<4; i++) {
        if(!visits[i]) continue;
        long long final_score = (scores[i]) / (visits[i]);
        if(final_score>best_score) {
            best_score = final_score;
            move = i;
        }
    }
    return move;

}

// int simulate_random_play_movecount(node* state, int& root_move) {
//     node* new_state = new node(state->board, 0, 0, state); 
//     int moves = 0;
//     int v[4] = {0, 1, 2, 3};
//     sort(v, v+4, random_sort);
//     while(true) {
//         bool game_over = 1;
//        
//        
//         for (int index = 0; index < 4; ++index) {
//             bool legal_move = 0;
//             int move = v[index];
//             switch (move) {
//                 case 0: move_up(legal_move, new_state->board); break; 
//                 case 1: move_right(legal_move, new_state->board); break;
//                 case 2: move_down(legal_move, new_state->board); break;
//                 case 3: move_left(legal_move, new_state->board); break;
//             }
//
//             if(legal_move) {
//                 generate_2(new_state->board);
//                 if(root_move == -1) root_move = move;
//                 moves++;
//                 game_over = 0;
//                 break;
//             }
//         }
//       
//         if(game_over) return moves;
//     }
// }

int simulate_random_play(node* state, int& root_move) {
    node* new_state = new node(state->board, 0, 0, state); 
    
    int v[4] = {2, 1, 3, 0};
    
    // for(int i=0; i<4; i++) cout<<v[i]<<" ";
    while(true) {
        bool game_over = 1;
        sort(v, v+4, random_sort);
        for (int index = 0; index < 4; ++index) {
            bool legal_move = 0;
            int move = v[index];
            switch (move) {
                case 0: move_up(legal_move, new_state->board); break; 
                case 1: move_right(legal_move, new_state->board); break;
                case 2: move_down(legal_move, new_state->board); break;
                case 3: move_left(legal_move, new_state->board); break;
            }

            if(legal_move) {
                generate_2(new_state->board);
                if(root_move == -1) root_move = move;
                game_over = 0;
                break;
            }
        }
        
        if(game_over) return score(new_state);
    }
}

int score (node* state) {
    int evalScore=0;
    for(int i=0; i<4; ++i)
        for(int j=0; j<4; ++j)
            evalScore+=table_score[state->board[i][j]];
    return evalScore;
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
    board[free_places[random]/4][free_places[random]%4] = 1; //said square will become 2
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

