#Archivo con las instrucciones del make, contiene las instrucciones para mysql

registroBib.out: main.o funciones.o
	gcc main.o funciones.o -o registroBib.out -lnsl -lm -lz -lmysqlclient

main.o: main.c
	gcc main.c -o main.o -c

funciones.o: funciones.c
	gcc funciones.c -o funciones.o -c
