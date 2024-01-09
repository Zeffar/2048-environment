2048 environment and MCTS simulation for RL 2023 - UniBuc
```
File contents for MCTS and GUI:
└── decision_tree
    ├── MCTS.exe
    ├── decision_tree.cpp
    ├── tree.txt
    └── Py_Master
        ├── yada yada
        ├── puzzle.py
        └── yada yada
```
How to use the MCTS inside GUI:
Technically, just run `puzzle.py` and it should work. In practice, I have no idea how to fix the problems with absolute paths so you have to go into `decision_tree.cpp`, 
change the absolute path of `tree.txt` to match your dir architecture, build it (I used `g++ decision_tree.cpp -o MCTS.exe`), go into `Py_Master -> puzzle.py` and change the absolute paths of `tree.txt` and `MCTS.exe` to match your own.

Then run puzzle.py. The key `N` gives you the next board. 

if you wish to use the MCTS without the GUI, you can simply edit tree.txt with the board you want it to evaluate and call the executable `MCTS.exe` in a bash. If the absolute paths are correct, 
the script will read the board and output the resulting move in the terminal.






 How to use this environment: 
 1. Put the current state (4x4 matrix) into `input.txt`, followed by the move you wish to make
```
    0 - up
    1 - right
    2 - down
    3 - left
```
    
 2. For Linux, cd into the 2048-environment directory and run the following commands:
```
g++ move.cpp -o main  ->  compiles the program, run if you change the source-code in move.cpp.
./main                ->  runs the program.
```
 3. In `output.txt` you will find:
```
 1 - legal move
-1 - illegal move

If the move was a legal one, it will also contain the new 4x4 matrix.
```
  
  4. In `score.txt` you will find the attributed score of the move.
   This score means to give an approximate value as to how good the move was, but is tremendously biased towards having the biggest value in the bottom-right corner. 
