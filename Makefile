#*============================---(source-start)---============================*#


#*---(current variables)--------------*#
BASE    = power

#*---(standard variables)-------------*#
COMP    = gcc -std=gnu89 -Wall -g
INC     = 
LINK    = gcc
LIBS    = -L/usr/local/lib -L/lib64 -L/usr/lib64 -lyX11 -lyFONT -lyCOLOR -lyLOG -lX11 -lGL -lpng
COPY    = cp -f
CLEAN   = rm -f
PRINT   = @printf


${BASE}             : ${BASE}.h ${BASE}.c
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "create executable\n"
	${COMP}   -o ${BASE}     ${BASE}.c   ${LIBS}



#*---(housecleaning)------------------*#
clean              :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "cleaning out local object, backup, and temp files\n"
	${CLEAN}  *.o
	${CLEAN}  *~
	${CLEAN}  temp*
	${CLEAN}  ${BASE}

bigclean           :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "clean out local swap files\n"
	${CLEAN}  .*.swp

install            : ${BASE}
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "installing in /usr/local/bin\n"
	${COPY}   ${BASE}  /usr/local/bin
	chmod     0755     /usr/local/bin/${BASE}
	@sha1sum  ${BASE}

remove             :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "remove ${BASE} from production\n"
	${CLEAN}  /usr/bin/${BASE}


#*============================----(source-end)----============================*#
