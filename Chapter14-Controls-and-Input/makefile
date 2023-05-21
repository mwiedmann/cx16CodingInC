CC=cl65

joy:
	$(CC) -O -o JOY.PRG -t cx16 joy.c wait.c

runjoy:
	../x16emu/x16emu -prg JOY.PRG -run

mouse:
	$(CC) -O -o MOUSE.PRG -t cx16 mouse.c wait.c

runmouse:
	../x16emu/x16emu -prg MOUSE.PRG -run
