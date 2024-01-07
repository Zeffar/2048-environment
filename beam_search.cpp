#include <fstream>
#include <vector>
#include <cmath>              //////////////modified all functions to not use files anymore, but nodes
#include <algorithm>
#include <iostream>
#include <cstdlib> 
#include <time.h> 
using namespace std;
ofstream g("output.txt");

struct node {
    int board[4][4];
    int root_move;
    int score;
};

void move_up(bool &ok, int a[4][4]);
void move_right(bool &ok, int a[4][4]);
void move_down(bool &ok, int a[4][4]);
void move_left(bool &ok, int a[4][4]);
int make_tree(string b, int depth, int width);
int evaluate(int board[4][4]);
bool next_board(int o_board[4][4], int i_board[4][4], int move);
void generate_2(int board[4][4]);
int make_generations(node out[], node in[], int width);
int make_children(node out[], node in[], int width);

bool child_cmp(node a, node b) {
    return (a.score > b.score);
}

void debug(int board[4][4])
{
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j)
            cout<<board[i][j]<<" ";
        cout<<endl;
    }
}
int main(){
    // srand(time(0)); //for the random function
    int tree_depth=5, tree_width=64;
    //here I should create the tree using two vectors with min max beam search and then choose the children using the eval function
    int result = make_tree("C:/2048_env/2048-tree/tree.txt",tree_depth,tree_width);

    cout<<to_string(result); 

    return 0;
}


//reads a state from a file and gives the next best move
int make_tree(string b, int depth, int width) { 

    //2 rows for list of current states of the board and for list of board with the 2s generated on them
    node children[width*5];
    node new_children[width*5];

    int x; 
    int child_nr = 0; //to count how many children of the big 4 are valid

    ifstream brd(b);
    int board[4][4];
    int orig[4][4];

    for (int i = 0; i < 4; ++i)  //read board from file 
        for (int j = 0; j < 4; ++j) {
            brd>>x;
            if (x)
                orig[i][j]=static_cast<int>(log2(x));//since all values are powers of 2, save the log
            else orig[i][j]=0;
        }

    
    
    int rounds = 100;
    int freq[4]={0, 0, 0, 0};
    //child_nr will keep the number of valid root_moves, ex 3 means 3 valid root_moves
    int number_of_generations = 0;
    int best_move=0;
    while(rounds--){
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                board[i][j]=orig[i][j];
        child_nr=0;
        

        for(int i=0; i<4; ++i) {
            x = next_board(children[child_nr].board,board,i); //will write in 'tree[0][child_nr].board', the board for the very first node in the first row of the tree, the modified 'board' after move '0' and x will verify if the fct outputed true or false
            if (x){

                children[child_nr].root_move = i; //goes ot the next node of the first row of the tree
                children[child_nr].score = evaluate(children[child_nr].board);
                if(children[child_nr].score < 0) continue;
                child_nr++;
            } 
        }
        sort(children, children + child_nr, child_cmp);
        best_move = children[0].root_move;
        // cerr<<best_move<<" ";

        for (int r=0; r<depth; ++r) { 
            // generate all possible moves
            child_nr = make_children(new_children, children, min(child_nr, width));
            if(!child_nr) break;
            // sort all moves decreasingly
            sort(new_children, new_children + child_nr, child_cmp);
            best_move=children[0].root_move;

            child_nr = make_children(children, new_children, min(child_nr, width));
            if(!child_nr) break;
            // sort all moves decreasingly
            sort(children, children + child_nr, child_cmp);
            best_move=children[0].root_move;

        }
        freq[best_move]++;

    }
    int maxi=-1;
    int rez=1;
    for (int i=0; i<4; i++) {
        cerr<<i<<' '<<freq[i]<<' ';
        maxi=max(maxi, freq[i]);
    }
        
    for (int i=0; i<4; i++) {
        
        if(freq[i]==maxi){
            rez=i;
            break;
        }
    }

    brd.close();
    return rez;
    //return best found move
    
}
int make_children(node out[], node in[], int width) {

    int index_in=0, index_out=0;
    for( ; index_in < width; ++index_in) {
        if(in[index_in].score<1) return index_out;
        for(int i=0; i<4; ++i) {
            int ok=0;
            ok = next_board(out[index_out].board, in[index_in].board, i); 
            if (ok) {
                out[index_out].score = evaluate(out[index_out].board);
                if(out[index_out].score < 0) continue;
                out[index_out].root_move = in[index_in].root_move;
                index_out++;
            }   
        }
    }
    return index_out;
}

//generates a random 2 in the empty spaces of the board
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

//evaluates a state/board
int evaluate(int board[4][4]){

    long score=0;
    bool ok=0;
    int aux[4][4];
    for (int i=0; i<4; i++){
        bool canMove=next_board(aux, board, i);
        if (canMove) ok=1;
    }
    if(!ok) return -10;

	int w_matrix[4][4]= {    //snake-like pattern for the values, aims to have the highest number in the down left corner
	         {-4, -3, -2, -1},
             {1, 2, 3, 4},
             {8, 7, 6, 5},
             {9, 10, 11, 15}
    };

    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            score += board[i][j] * w_matrix[i][j];  

    // reward: ajacent conscutive numbers (snake shaped), many blank spaces, possible merges, punish: not enough blanks
    return score;

}

//writes in the output board how the next board based on the input board will look like, returning 1 if valid and 0 if not
bool next_board(int o_board[4][4], int i_board[4][4], int move){  
    
    for (int i = 0; i < 4; ++i) //copy input board in a
        for (int j = 0; j < 4; ++j) 
            o_board[i][j]=i_board[i][j];

    bool legal_move=0;

    switch(move) {
        case 0: move_up(legal_move, o_board); break; //legal move will stay 0 if the move is invalid and 1 if valid, and a will change accordingly
        case 1: move_right(legal_move, o_board); break;
        case 2: move_down(legal_move, o_board); break;
        case 3: move_left(legal_move, o_board); break;
    }
    if(!legal_move) return 0;
    generate_2(o_board);
    return 1;
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
