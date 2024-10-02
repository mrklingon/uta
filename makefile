FHOME=./
#FHOME=/cygnus/cygwin-b20/projects/uta/
CC=gcc
BIN=/unix
LANGS= esper.lng rom.lng tenct.lng tlhingan.lng	uta.lng	vulcan.lng
uta:  uta.c makefile
	$(CC) -g -DFILEHOME=\"$(FHOME)\"  -o uta uta.c
#	strip uta

install: uta
	strip uta



ship: uta.c makefile $(LANGS) readme.uta utab psalm23.txt
	touch uta.tar.gz uta.zip
	rm uta.tar.gz uta.zip
	strip uta.exe
	tar -cvf uta.tar uta.c makefile	readme.uta psalm23.txt $(LANGS)
	zip  uta.zip uta.c makefile readme.uta	psalm23.txt $(LANGS)
	gzip uta.tar
	touch uta.c
	$(CC) -DFILEHOME=\"$(FHOME)\" -mno-cygwin -o uta uta.c


utab: readme.uta  $(LANGS)
	$(CC) -DFILEHOME=\"./\"	-mno-cygwin -o uta uta.c
	strip uta.exe
	touch utab.zip
	rm utab.zip
	zip  utab.zip uta.exe readme.uta  $(LANGS) psalm23.txt
