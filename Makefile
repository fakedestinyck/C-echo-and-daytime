CC =		cc
CFLAGS =	-O0 -pipe -g

PROGS =	echo echod daytime daytimed
OBJS =	echo.o echod.o daytime.o daytimed.o

all: ${PROGS}

echo: echo.c
	${CC} ${CFLAGS} -o echo echo.c
echod: echod.c
	${CC} ${CFLAGS} -o echod echod.c
daytime: daytime.c
	${CC} ${CFLAGS} -o daytime daytime.c
daytimed: daytimed.c
	${CC} ${CFLAGS} -o daytimed daytimed.c

clean:
	rm -rf ${PROGS} *.core *.dSYM 
