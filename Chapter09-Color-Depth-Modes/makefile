CC=cl65

make:
	$(CC) -O -o EXAMPLE.PRG -t cx16 main.c

run:
	../x16emu/x16emu -prg EXAMPLE.PRG -run

256:
	$(CC) -O -o EXAMPLE2.PRG -t cx16 main-256.c

run256:
	../x16emu/x16emu -prg EXAMPLE2.PRG -run