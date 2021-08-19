#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>

/* 
KEVIN = abc1234
JIRGORT = holamundo1
*/

MYSQL *conn; 
MYSQL_RES *res; 
MYSQL_ROW row; 
char *server = "localhost"; 
char *user = "root"; 
char *password = "abc1234"; 
char *database = "DB_Reservacion_Aulas"; 


int ConectarBD(){
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){ 
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}

int InsertarAulas(char* nombre, int capacidad){
    if (mysql_query(conn, "INSERT INTO Aulas values ('A1',20)")) {
        printf("Unable to insert data into Employee table\n");
        mysql_close(conn);
        return (1);
    }
}

void terminarConexion(){
    mysql_free_result(res);
	mysql_close(conn);
}
 