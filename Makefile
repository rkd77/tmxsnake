snake.tap: snake.c snake.bas t2068.tap Makefile
	zcc +ts2068 -clib=ansi -O3 -vn -startup=2 -zorg=33300 -lndos -lmalloc -o snake.bin snake.c
	rm -f snake.tap snake2.tap
	tapmaker snake.bin snake2.tap
	zmakebas -n 'SNAKE2' -a 10 snake.bas
	cat t2068.tap out.tap snake2.tap > snake.tap

#snake.tap: snake.c
#	zcc +ts2068 -clib=ansi -O3 -vn -startup=2 -zorg=33300 -lndos -lim2 -lmalloc -create-app -o snake.bin snake.c
