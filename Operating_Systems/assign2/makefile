# Do not edit the contents of this file.
# Your code must compile using ths command.
a2: *.c
	gcc -Werror -Wall -g -std=c99 -o Dispatcher *.c

run: 
	./Dispatcher 800 < test_inputs/test1.txt

valgrind:
	valgrind --leak-check=full --leak-resolution=med --track-origins=yes --show-leak-kinds=all ./Dispatcher 800 < test_inputs/test1.txt
