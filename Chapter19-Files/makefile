CC=cl65

tile:
	$(CC) -O -o TILE.PRG -t cx16 tile.c

runtile:
	../x16emu/x16emu -prg TILE.PRG -run

score:
	$(CC) -O -o SCORE.PRG -t cx16 score.c

runscore:
	../x16emu/x16emu -prg SCORE.PRG -run

newhigh:
	cp HIGH_ORG.BIN HIGH.BIN