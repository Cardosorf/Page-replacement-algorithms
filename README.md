# Page-replacement-algorithms
Implementation of paging algorithms seen in operating system class

The instructions described at http://wiki.inf.ufpr.br/maziero/doku.php?id=so:algoritmos_de_substituicao_de_paginas were followed but with some adjustments.

# How to Use:

There are 2 ways to run this program.
                
The first one is running the makefile by just typing **make** on your console (make sure you're in the same folder as the program).

`$ make `

The other one you'll need to compile and then execute it.

# How to Compile:
You can compile using the following command:
    
   `$ g++ RafaelCardoso_SO.cpp -o main -O3 `
    
Then you can execute it by typing the following commands:

   `$ ./main 4 vsim-exemplo.txt `
   
   `$ ./main 4 vsim - belady.txt `
   
   `$ ./main 64 vsim - gcc.txt `
   
   `$ ./main 256 vsim - gcc.txt `
   
   `$ ./main 1024 vsim - gcc.txt `
   
   `$ ./main 4096 vsim - gcc.txt `


