#include "header.h"

int main(int argc, char *argv){
	//Conectar la base de datos
	char buffer[1024];
	char *server = "localhost";
	char *user = "im20fyl";
	char *passwd = "218392";
	char *db = "im20fyl";

	MYSQL mysql;

	mysql_init(&mysql);

	if(!mysql_real_connect(&mysql, server, user, passwd, db, 0, NULL, 0)){
		fprintf(stderr, "No se pudo conectar con la base de datos: Error: %s\n",mysql_error(&mysql));
		return(0);
	}

	if( mysql_select_db(&mysql,db) ){
		fprintf(stderr,"Error al elegir la base de datos: Error: %s\n",mysql_error(&mysql));
		exit(1);
	}

	//---------------- Declaración de Variables ------------------------------------
	char  ncuenta[10], pass[10], query[1024];
	int loop = 0, datos = 0;
	int selec;
	MYSQL_RES *res;
	MYSQL_ROW fil;

	system("clear");
	printf("\033[0;33m");
	printf("------------------- Bienvenido, ingresa tus datos ---------------\n");
	printf("\033[0m");
	while (loop == 0){
		printf("Número de cuenta: ");
		scanf(" %s", ncuenta);
		printf("Contraseña: ");
		scanf(" %s", pass);


		//--------------------------- Verificar el usuario --------------------
		sprintf(buffer, "SELECT n_cuenta FROM registros WHERE n_cuenta  = '%s'", ncuenta);
		//printf("El query es \"%s\"\n", buffer);
		if(mysql_query(&mysql, buffer)){
			fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
			abort();
		}

		if(!(res = mysql_store_result(&mysql))){
			fprintf(stderr, "Error: No se guardo el query, %s\n", mysql_error(&mysql));
			abort();
		}
		while(fil = mysql_fetch_row(res)){
			strcpy(query, fil[0]);
			printf("registro guardado %s\n", fil[0]);
		}
		mysql_free_result(res);
		if(strcmp(ncuenta, query) == 0){
			system("clear");
			printf("\033[0;32m");
			printf("Usuario verificado\n");
			printf("\033[0m");
			datos = 1;
		}else{
			printf("\033[0;31m");
			printf("Usuario no verificado\n");
			printf("\033[0m");
		}
		
		//--------------------------- Verificar contraseña --------------------
		
		sprintf(buffer, "SELECT contrasena FROM registros WHERE n_cuenta  = '%s'", ncuenta);
		//printf("El query es \"%s\"\n", buffer);
		if(mysql_query(&mysql, buffer)){
			fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
			abort();
		}

		if(!(res = mysql_store_result(&mysql))){
			fprintf(stderr, "Error: No se guardo el query, %s\n", mysql_error(&mysql));
			abort();
		}
		while(fil = mysql_fetch_row(res)){
			strcpy(query, fil[0]);
			//printf("registro guardado %s\n", fil[0]);
		}
		mysql_free_result(res);
		if(strcmp(pass, query) == 0){
			printf("\033[0;32m");
			printf("Contraseña verificada\n");
			printf("\033[0m");
			datos = datos + 1;
		}else{
			printf("\033[0;31m");
			printf("Contraseña equivocada, intente denuevo:\n");
			printf("\033[0m");
		}
		//--------------------------- Verificar Administrador ------------------
		if(datos == 2){
		sprintf(buffer, "SELECT tipo_u FROM registros WHERE n_cuenta = '%s';", ncuenta);
		//printf("El query es \"%s\"\n", buffer);
		if(mysql_query(&mysql, buffer)){
			fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
			abort();
		}

		if(!(res = mysql_store_result(&mysql))){
			fprintf(stderr, "Error: No se guardo el query %s\n", mysql_error(&mysql));
			abort();
		}
		
		while(fil = mysql_fetch_row(res)){
			strcpy(query, fil[0]);
			//printf("registro guardado %s\n", fil[0]);
		}
		mysql_free_result(res);
		if(strcmp("1", query) == 0){
			printf("Administrador  verificado\n");
			modo_admin(ncuenta, buffer, res, mysql);
			break;
		}
		}

		//---------------------------- Menu --------------------------------
		if(datos == 2){
			while (loop == 0){
			printf("\033[0;33m");
			printf("*********+ Menu de la bilbioteca ***********\n");
			printf("\033[0m");
			printf("Selecciones una opción:\n");
			printf("1 - Solicitar un libro\n");
			printf("2 - Devolver un libro\n");
			printf("3 - Salir\n");
			printf("Opción: ");
			scanf(" %i", &selec);
			switch (selec){
				case 1:
					pedir_libro(ncuenta, buffer, res, mysql);
					break;
				case 2:
					regresar_libro(ncuenta, buffer, res, mysql);
					break;
				case 3:
					printf("Salinedo.....\n");
					loop = 1;
					break;
				default:
					printf("Selección invalida\n");
					break;
			}
			}
		}

	}
	mysql_free_result(res);
	//-------------------------Cerrar sesión-----------------------------
	mysql_close(&mysql);
	return 0;
}
