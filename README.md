2048 environment for RL 2023 - UniBuc
```
File contents:
├── move.cpp
├── input.txt
├── output.txt
└── score.txt
```

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
