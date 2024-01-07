// Attempt at making a decision tree using a MCTS approach.
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
    int win_score;

    //constructor
    node(int a[4][4], int move, int evalScore, node* parent) : 
        parent_move(move), score(evalScore), 
        parent(nullptr), visits(0), win_score(0) {

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                this -> board[i][j] = a[i][j];
        
        this -> parent = parent;
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

void move_up(bool &ok, int a[4][4]);
void move_right(bool &ok, int a[4][4]);
void move_down(bool &ok, int a[4][4]);
void move_left(bool &ok, int a[4][4]);
int MCTS(node* root);
node* select_node(node* state);
bool is_terminal(node* state);
bool is_leaf (node *state);
int simulate_random_play(node* node);
void expand_node(node* state);
void generate_2(int board[4][4]);
void backpropagate(node* state, int score);
int choose_move(node* state);
void debug(node* state) {
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j)
            if(state->board[i][j])
                cout<<(1<<state->board[i][j])<<" ";
            else cout<<"0 ";
        cout<<endl;
    }
}

int main() {
    ifstream read_tree("Z:/GitHub/2048-environment/tree.txt");
    srand(time(0));

    node* root = new node();
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            int x; read_tree>>x;
            if(x) root->board[i][j]=static_cast<int>(log2(x));
        }
    }
    
    cout<<MCTS(root);
}

int MCTS(node *root) {
    int runs = 1;
    for(int iterations = 0; iterations < runs; ++iterations) {
        node* selected_node = select_node(root);
        if (!is_terminal(selected_node)) {
            expand_node(selected_node);
        }
        int result = simulate_random_play(selected_node);
        backpropagate(selected_node, result);
    }
    return choose_move(root);
}

void backpropagate(node* state, int score) {

}

int choose_move(node* state) {

}

int simulate_random_play(node* state) {
    
}

void expand_node(node* state) {

    for(int move=0; move<4; ++move) {
        node* new_state = new node(state->board, move, 0, state); //score is 0, fix later
        bool legal_move = 0;
        switch (move) {
            case 0: move_up(legal_move, new_state->board); break; 
            case 1: move_right(legal_move, new_state->board); break;
            case 2: move_down(legal_move, new_state->board); break;
            case 3: move_left(legal_move, new_state->board); break;
        }
        if(!legal_move) {
            delete new_state;
        }
        else {
            generate_2(new_state->board);
            state->children.push_back(new_state);
        }
    }
}

node* select_node(node* state) {

    while(true) {

        if (is_leaf(state)) return state;
        int children_number = state->children.size();
        int choice = rand() % children_number;
        state = state ->children[choice]; 
    }
}

bool is_leaf (node *state) {
    return (state->children.size() == 0);
}

bool is_terminal(node* state) { 
    for(int i=0; i<4; i++) {
        node* new_board = new node(state->board, 0, 0, nullptr);
        bool legal_move = 0;
        switch (i) {
            case 0: move_up(legal_move, new_board->board); break; 
            case 1: move_right(legal_move, new_board->board); break;
            case 2: move_down(legal_move, new_board->board); break;
            case 3: move_left(legal_move, new_board->board); break;
        }
        if(legal_move) return false;
    }
    return true;
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

