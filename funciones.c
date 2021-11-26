#include "header.h"

int pedir_libro(char user[], char buffer[], MYSQL_RES *res, MYSQL mysql){
	int loop = 0;
	int selec, isbn, ejem, disp, check = 0;
	char nom[20], query[1024];
	MYSQL_ROW fil;

	system("clear");
	while(loop == 0){
		printf("\033[0;33m");
		printf("+++++++++++++++ Menu de solicitud +++++++++++++\n");
		printf("\033[0m");
		printf("1 - Busqueda de ISBN por nombre\n");
		printf("2 - Solicitar con ISBN\n");
		printf("3 - Regresar\n");
		printf("Opción: ");
		scanf(" %i", &selec);
		switch (selec){
			case 1:
				printf("Ingresar nombre: ");
				scanf(" %s", nom);
				
				sprintf(buffer, "SELECT nom_libro, ISBN  FROM catalogo WHERE nom_libro LIKE '%s%%'", nom);
                		//printf("El query es \"%s\"\n", buffer);
                		if(mysql_query(&mysql, buffer)){
                        		fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        		abort();
                		}

                		if(!(res = mysql_store_result(&mysql))){
                        		fprintf(stderr, "Error: No se guardo el query, %s\n", mysql_error(&mysql));
                        		abort();
                		}
				printf("\033[0;36m");
				printf("Resultado de busqueda:\n");
				printf("|NOMBRE \t| ISBN|\n");
				while(fil = mysql_fetch_row(res)){
					printf("|%s \t| %s|\n", fil[0], fil[1]);
				}
				printf("Fin de busqueda\n\n");
				printf("\033[0m");
			       break;
			case 2:
			       check = 0;
			       printf("Ingresa el ISBN: ");
			       scanf(" %i", &isbn);
			       
			        sprintf(buffer, "SELECT nom_libro, ISBN, ejemplares FROM catalogo WHERE ISBN = '%i'", isbn);
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
					printf("El libro %s tiene %s ejemplares\n", fil[0], fil[2]);
					strcpy(query, fil[2]);
					check += 1;
				}
				if(check == 0){
					printf("ISBN no encontrado\n");
					break;
				}
				printf("Cuantos ejemplares deseas pedir: ");
				scanf(" %i", &ejem);
				sscanf(query, "%i", &disp);
				if(ejem <= disp && ejem >= 1){
					sprintf(buffer, "CALL registro_bib('%s', '%i', '%i')", user, ejem, isbn);
                			//printf("El query es \"%s\"\n", buffer);
                			if(mysql_query(&mysql, buffer)){
                        			fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        			abort();
                			}
                			if(!(res = mysql_store_result(&mysql))){
                        			fprintf(stderr, "Error: No se guardo el query, %s\n", mysql_error(&mysql));
                        			abort();
                			}
					printf("\033[0;32m");
					printf("Solicitud realizada con exito\n");
					printf("\033[0m");
				}else{
					printf("No hay suficientes ejemplares\n");
				}
			       break;
			case 3:
			       loop = 1;
			       printf("Regresando...\n");
			       break;
			default:
			       printf("Selección invalida\n");
			       break;
		}
	}
return 0;
}

//-------------------------------------------------------------------------------------------------

int regresar_libro(char user[], char buffer[], MYSQL_RES *res, MYSQL mysql){
	int loop = 0, check = 0;
	int isbn, ejem, disp, selec;
	char nom[20], query[1024];
	MYSQL_ROW fil;

	system("clear");
	while(loop == 0){
		printf("\033[0;33m");
		printf("+++++++++++++++ Menu de retorno de libros +++++++++++++\n");
		printf("\033[0m");
		printf("1 - Busqueda de ISBN por nombre\n");
		printf("2 - Regresar libros con ISBN\n");
		printf("3 - Regresar\n");
		printf("Opción: ");
		scanf(" %i", &selec);
		switch (selec){
			case 1:
				printf("Ingresar nombre: ");
				scanf(" %s", nom);
				
				sprintf(buffer, "SELECT nom_libro, ISBN  FROM catalogo WHERE nom_libro LIKE '%s%%'", nom);
                		//printf("El query es \"%s\"\n", buffer);
                		if(mysql_query(&mysql, buffer)){
                        		fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        		abort();
                		}

                		if(!(res = mysql_store_result(&mysql))){
                        		fprintf(stderr, "Error: No se guardo el query, %s\n", mysql_error(&mysql));
                        		abort();
                		}
				printf("\033[0;36m");
				printf("Resultado de busqueda:\n");
				printf("|NOMBRE \t| ISBN|\n");
				while(fil = mysql_fetch_row(res)){
					printf("|%s \t| %s|\n", fil[0], fil[1]);
				}
				printf("Fin de busqueda\n\n");
				printf("\033[0m");
			       break;
			case 2:
			       check = 0;
			       printf("Ingresar ISBN del libro a regresar: ");
			       scanf(" %i", &isbn);

			        sprintf(buffer, "SELECT n_cuenta, fecha_dev, solicitudes FROM prestamos WHERE ISBN = '%i' AND n_cuenta = '%s'", isbn, user);
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
					printf("Cuenta: %s, solicito %s ejemplares de %i para el %s\n", fil[0], fil[2], isbn, fil[1]);
					strcpy(query, fil[2]);
					check += 1;
				}
				if(check == 0){
					printf("Prestamo no encontrado\n");
					break;
				}
				printf("Cuantos ejemplares deseas devolver: ");
				scanf(" %i", &ejem);
				sscanf(query, "%i", &disp);
				if(ejem <= disp && ejem >= 1){
					sprintf(buffer, "CALL devolucion_bib('%s', '%i', '%i')", user, ejem, isbn);
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
						printf("%s\n",fil[0]);
					}
					printf("\033[0;32m");
					printf("Devolución realizada con exito\n");
					printf("\033[0m");
				}else{
					printf("Número de devoluciones incorrecta\n");
				}
				break;
			case 3:
			       loop = 1;
			       printf("Reresando...\n");
			       break;
			default:
			       printf("Selección invalida\n");
			       break;
		}
	}
return 0;
}

//-------------------------------------------------------------------------------------------------

int modo_admin(char user[], char buffer[], MYSQL_RES *res, MYSQL mysql){
	struct usuario nuevo;
	int loop = 0;
	char ncuenta[10], busq[100];
	int selec, selec2, op;
	MYSQL_ROW fil;

	system("clear");
	while(loop == 0){
		printf("\033[0;33m");
		printf("====================== Menu de administrador =====================\n");
		printf("\033[0m");
		printf("1 - Solicitar un libro\n");
		printf("2 - Devolver un libro\n");
		printf("3 - Alta de nuevo usuario\n");
		printf("4 - Actualizar usuario\n");
		printf("5 - Busqueda de usuario\n");
		printf("6 - Eliminar un usuario\n");
		printf("7 - Salir\n");
		printf("Opción: ");
		scanf(" %i", &selec);
		switch(selec){
			case 1:
				pedir_libro(user, buffer, res, mysql);
				break;
			case 2:
				regresar_libro(user, buffer, res, mysql);
				break;
			case 3:
				system("clear");
				printf("\033[0;33m");
				printf("--------- Alta de usuario --------\n");
				printf("\033[0m");
				printf("Del nuevo usuario, ingresa los siguientes datos:\n");
				printf("Nombre: ");
				scanf(" %s", nuevo.nom);	
				printf("Apellido paterno: ");
				scanf(" %s", nuevo.apPater);
				printf("Apellido materno: ");
				scanf(" %s", nuevo.apMater);
				printf("Número de cuenta (NNNNNN-N): ");
				scanf(" %s", nuevo.nCuenta);
				printf("Carrea: ");
				scanf(" %s", nuevo.carr);
				printf("Semestre cruzando: ");
				scanf(" %i", &nuevo.sem);
				printf("Tipo de usuario (Admin = 1, Sol = 0): ");
				scanf(" %i", &nuevo.tipoU);
				printf("Correo institucional: ");
				scanf(" %s", nuevo.corr);
				printf("Año de nacimiento: ");
				scanf(" %i", &nuevo.anioN);
				printf("Mes de nacimiento (número): ");
				scanf(" %i", &nuevo.mesN);
				printf("Día de naicmiento: ");
				scanf(" %i", &nuevo.diaN);
				printf("Contraseña: ");
				scanf(" %s", nuevo.cont);
				
			        sprintf(buffer, "INSERT INTO registros (nombre, ap_paterno, ap_materno, n_cuenta, carrera, semestre, tipo_u, correo, fecha_nac, contrasena) VALUES ('%s','%s','%s','%s','%s','%i','%i','%s','%i-%i-%i','%s')", nuevo.nom, nuevo.apPater, nuevo.apMater, nuevo.nCuenta, nuevo.carr, nuevo.sem, nuevo.tipoU, nuevo.corr, nuevo.anioN, nuevo.mesN, nuevo.diaN, nuevo.cont);
                		//printf("El query es \"%s\"\n", buffer);
                		if(mysql_query(&mysql, buffer)){
                        		fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        		abort();
                		}
				printf("\033[0;32m");
				printf("Usuario registrado exitosamente\n");
				printf("\033[0m");
				break;
			case 4:
				system("clear");
				printf("\033[0;33m");
				printf("---------- Actualizar usuario ---------\n");
				printf("\033[0m");
				printf("Ingresa el número de cuenta del usuario: ");
				scanf(" %s", ncuenta);
				printf("¿Qué deseas modificar?\n");
				printf("1 - Nombre\n");
				printf("2 - Apellido paterno\n");
				printf("3 - Apellido materno\n");
				printf("4 - Carrera\n");
				printf("5 - Semestre\n");
				printf("6 - Tipo de usuario\n");
				printf("7 - Correo electrónico\n");
				printf("8 - Fecha de nacimiento\n");
				printf("9 - Contraseña\n");
				printf("10 - Regresar\n");
				printf("Opción: ");
				scanf(" %i", &selec2);
				switch(selec2){
					case 1:
						printf("Ingresa el nuevo nombre: ");
						scanf(" %s", nuevo.nom);
						sprintf(buffer, "UPDATE registros SET nombre = '%s' WHERE n_cuenta = '%s'", nuevo.nom, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 2:
						printf("Ingresa el nuevo apellido: ");
						scanf(" %s", nuevo.apPater);
						sprintf(buffer, "UPDATE registros SET ap_paterno = '%s' WHERE n_cuenta = '%s'", nuevo.apPater, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 3:
						printf("Ingresa el nuevo apellido: ");
						scanf(" %s", nuevo.apMater);
						sprintf(buffer, "UPDATE registros SET ap_materno = '%s' WHERE n_cuenta = '%s'", nuevo.apMater, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 4:
						printf("Ingresa la nueva carrera: ");
						scanf(" %s", nuevo.carr);
						sprintf(buffer, "UPDATE registros SET carrera = '%s' WHERE n_cuenta = '%s'", nuevo.carr, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 5:
						printf("Ingresa el nuevo semestre: ");
						scanf(" %i", &nuevo.sem);
						sprintf(buffer, "UPDATE registros SET semestre = %i WHERE n_cuenta = '%s'", nuevo.sem, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 6:
						printf("Ingresa el nuevo tipo de usuario (Admin = 1, Sol = 0: ");
						scanf(" %i", &nuevo.tipoU);
						sprintf(buffer, "UPDATE registros SET tipo_u = '%i' WHERE n_cuenta = '%s'", nuevo.tipoU, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 7:
						printf("Ingresa el nuevo correo: ");
						scanf(" %s", nuevo.corr);
						sprintf(buffer, "UPDATE registros SET correo = '%s' WHERE n_cuenta = '%s'", nuevo.corr, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 8:
						printf("Ingresa el nuevo año de nacimiento: ");
						scanf(" %i", &nuevo.anioN);
						printf("Ingresa el nuevo mes (número) de nacimiento: ");
						scanf(" %i", &nuevo.mesN);
						printf("Ingresa el nuevo día de nacimiento: ");
						scanf(" %i", &nuevo.diaN);
						sprintf(buffer, "UPDATE registros SET fecha_nac = '%i-%i-%i' WHERE n_cuenta = '%s'", nuevo.anioN, nuevo.mesN, nuevo.diaN, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 9:
						printf("Ingresa la nueva contraseña: ");
						scanf(" %s", nuevo.cont);
						sprintf(buffer, "UPDATE registros SET contrasena = '%s' WHERE n_cuenta = '%s'", nuevo.cont, ncuenta);
                				//printf("El query es \"%s\"\n", buffer);
                				if(mysql_query(&mysql, buffer)){
                        				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        				abort();
						}
						printf("\033[0;32m");
						printf("Actualización exitosa\n");
						printf("\033[0m");
						break;
					case 10:
						break;
					default:
						printf("Selección invalida\n");
						break;
                			}
				break;
			case 5:
				system("clear");
				printf("\033[0;33m");
				printf("----------- Busqueda de usuario -----------\n");
				printf("\033[0m");
				printf("Tipos de búsqueda:\n");
				printf("1 - Por nombre\n");
				printf("2 - Por número de cuenta\n");
				printf("3 - Por carrera\n");
				printf("4 - Por Nombre del libro\n");
				printf("5 - Regresar\n");
				printf("Selecciona una opción: ");
				scanf(" %i", &op);
				printf("Ingresa el parametro: ");
				scanf(" %s", busq);
			        
				if(op == 5){
					break;
				}
				sprintf(buffer, "CALL busqueda_bib('%i', '%s')", op, busq);
                		//printf("El query es \"%s\"\n", buffer);
                		if(mysql_query(&mysql, buffer)){
                        		fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                        		abort();
                		}
                		if(!(res = mysql_store_result(&mysql))){
                        		fprintf(stderr, "Error: No se guardo el query, %s\n", mysql_error(&mysql));
                        		abort();
                		}
				printf("\033[0;36m");
				while(fil = mysql_fetch_row(res)){
					printf("|Cuenta \t|Nombre\t\t|Ap Paterno\t|Nombre Libro\t|Ejemplares pedidos\t|Fecha Solicitud\t|Fecha Devolución|\n");
					printf("|%s\t|%s\t|%s\t\t|%s\t|%s\t\t\t|%s\t\t|%s|\n", fil[0], fil[1], fil[2], fil[3], fil[4], fil[5], fil[6]);
				}
				printf("\033[0;32m");
				printf("Busqueda realizada exitosamente\n");
				printf("\033[0m");

				mysql_free_result(res);
				break;
			case 6:
				system("clear");
				printf("\033[0;33m");
				printf("---------- Borrado de usuario -------------\n");
				printf("\033[0;31m");
				printf("Precaución, estas por borrar un usuario, ingresa 'cancelar' para regresar\n");
				printf("\033[0m");
				printf("Ingresa el número de cuenta a borrar: ");
				scanf(" %s", ncuenta);
				if(strcmp(ncuenta, "cancelar") == 0){
					break;
				}else{
					sprintf(buffer, "DELETE FROM registros  WHERE n_cuenta = '%s'", ncuenta);
                			//printf("El query es \"%s\"\n", buffer);
                			if(mysql_query(&mysql, buffer)){
                       				fprintf(stderr, "Error: No se ejecuto el query, %s\n", mysql_error(&mysql));
                       				abort();
					}
                			if(!(res = mysql_store_result(&mysql))){
                        			fprintf(stderr, "Error: No se guardo el query, %s\n", mysql_error(&mysql));
                        			abort();
                			}
					printf("\033[0;32m");
					printf("Usuario eliminado exitosamente\n");
					printf("\033[0m");
				}

			case 7:
				printf("Saliendo...\n");
				loop = 1;
				break;
		}
	}
	return 0;
}

