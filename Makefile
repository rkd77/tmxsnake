snake.tap: snake.c
	zcc +ts2068ansi -O3 -vn -startup=2 -zorg=33300 -lndos -lim2 -create-app -o snake.bin snake.c
