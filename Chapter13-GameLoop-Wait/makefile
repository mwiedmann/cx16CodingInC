CC=cl65

make:
	$(CC) -O -o MOVE.PRG -t cx16 movechar.c wait.c

run:
	../x16emu/x16emu -prg MOVE.PRG -run

hb:
	$(CC) -O -o HB.PRG -t cx16 heartbeat.c wait.c

runhb:
	../x16emu/x16emu -prg HB.PRG -run