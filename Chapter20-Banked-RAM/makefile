CC=cl65

code:
	$(CC) -O -o BANKCODE.PRG -t cx16 -C cx16-bank.cfg bankedCode.c

runcode:
	../x16emu/x16emu -prg BANKCODE.PRG -run

tiles:
	$(CC) -O -o TILES.PRG -t cx16 tiles.c wait.c

runtiles:
	../x16emu/x16emu -prg TILES.PRG -run