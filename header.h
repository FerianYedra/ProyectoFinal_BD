/**
 * @file header.h
 * @brief Este es el archivo de cabecera del programa, incluye las bibliotecas estandar de C
 *        asi como la biblioteca de mysql para compilar las funciones especiales. Tambien incluye
 *        los prototipos de las funciones a llamar y un struct con las variables de los usuarios 
 *        como se crearon en la talba (registros).
 * @author Jaen Izamar, Jorge Rojas, Fernando Yedra
 * @date 29/11/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>


int modo_admin(char user[], char buffer[], MYSQL_RES *res, MYSQL mysql);
int pedir_libro(char user[], char buffer[], MYSQL_RES *res, MYSQL mysql);
int regresar_libro(char user[], char buffer[], MYSQL_RES *res, MYSQL mysql);

struct usuario{
                char nom[20];
                char apPater[20];
                char apMater[20];
                char nCuenta[10];
                char carr[100];
                int sem;
                int tipoU;
                char corr[40];
                int anioN;
                int mesN;
                int diaN;
                char cont[20];
};
