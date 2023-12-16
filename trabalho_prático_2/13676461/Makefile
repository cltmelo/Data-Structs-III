all:
	gcc -o main *.c
run:
	./main
	

# Estas linhas não podem estar neste Makefile quando for entregue!
valgrind:
	valgrind --leak-check=full ./main
	
clear:
	rm -f main
