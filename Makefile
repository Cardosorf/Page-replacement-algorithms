All:
	clear
	g++ RafaelCardoso_SO.cpp -o main -O3
	./main 4 vsim-exemplo.txt
	./main 4 vsim-belady.txt
	./main 64 vsim-gcc.txt
	./main 256 vsim-gcc.txt
	./main 1024 vsim-gcc.txt
	./main 4096 vsim-gcc.txt

