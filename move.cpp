#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
using namespace std;
ifstream f("input.txt");
ofstream g("output.txt");
ofstream h("score.txt");
void move_up(bool &ok);
void move_right(bool &ok);
void move_down(bool &ok);
void move_left(bool &ok);
int evaluate();

int a[4][4];
int main()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int x; f>>x;
            if(x) a[i][j]=int(log2(x));
        }
    }
    int move;
    f>>move;
    bool legal_move=0;
    switch(move)
    {
        case 0: move_up(legal_move); break;
        case 1: move_right(legal_move); break;
        case 2: move_down(legal_move); break;
        case 3: move_left(legal_move); break;
    }

    if (legal_move) g<<"1\n";
    else {g<<"-1\n"; return -1;}

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if(a[i][j])
                g<<(1<<a[i][j])<<" ";
            else g<<"0 ";
        }
        g<<'\n';
    }
    h<<evaluate();
    return 0;

}





int evaluate (){ int score=0;
    vector<int> tiles; tiles.reserve(16);

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++) {
            int val=int(a[i][j]);
            if(val) tiles.push_back(val);
        }
    sort(tiles.begin(), tiles.end(), greater<int>());

    int cnt{};
    for(int i=3; i; i--)
        for(int j=3; j; j--)
        {
            if(!tiles[cnt]) return score;
            if(tiles[cnt++]==int(a[i][j])) score=score+int(a[i][j])*(i*4+j);
        }
    return score;
}

//move functions
void move_left(bool &ok){
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

void move_right(bool &ok){
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

void move_up(bool &ok){
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

void move_down(bool &ok){
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