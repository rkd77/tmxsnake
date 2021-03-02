snake.tap: snake.c snake.bas t2068.tap Makefile
	zcc +ts2068 -zorg=33300 --generic-console -O3 -Cl-v  -lndos -o snake.bin snake.c
	rm -f snake.tap snake2.tap
	tapmaker snake.bin snake2.tap
	zmakebas -n 'SNAKE2' -a 10 snake.bas
	cat t2068.tap out.tap snake2.tap > snake.tap
