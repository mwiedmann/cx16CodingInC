CC=cl65

code:
	$(CC) -O -o CODE.PRG -t cx16 -C cx16-bank.cfg code.c

runcode:
	../x16emu/x16emu -prg CODE.PRG -run

tiles:
	$(CC) -O -o TILES.PRG -t cx16 tiles.c wait.c

runtiles:
	../x16emu/x16emu -prg TILES.PRG -run