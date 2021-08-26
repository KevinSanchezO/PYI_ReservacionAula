#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

MYSQL *conn; 
MYSQL_RES *res; 
MYSQL_ROW row; 
char *server = "localhost"; 
char *user = "root"; 
char *password = "abc1234"; //Kevin
//char *password = "holamundo1";  //Jirgort 
char *database = "DB_Reservacion_Aulas";
FILE *archivo; 


int ConectarBD(){
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){ 
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}

int verificarFormatoNombre(char* nombre){
    if (nombre == " "){
        return TRUE;
    }

    if (strlen(nombre) == 2){
        if (isalpha(nombre[0]) && isdigit(nombre[1])){
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

int verificarFormatoCapacidad(char* capacidad){
    if (capacidad == " " || capacidad == "0"){
        return FALSE;
    }
    for(int i = 0; i<strlen(capacidad); i++){
        if (!isdigit(capacidad[i])){
            return FALSE;
        }
    }
    return TRUE;
}


void insertarAulas(){
    char ruta[300];
    char caracter;
    char nombreAula[90];
    char capacidad[90];
    int count=0;
    int bandera=1;
    int count2=0;
    printf("\nIngrese la ruta del archivo: ");
    scanf(" %s", &ruta);
    archivo=fopen(ruta,"r");
    if (archivo==NULL){
        printf("error");
    }else{
        while((caracter=fgetc(archivo))!=EOF){
            if(caracter==','){
                bandera=2;
            }
            if(caracter=='\n'){
                bandera=1;
                char consult[200];
                char *nombre=nombreAula;

                printf("\n%s", nombreAula);
                printf("\n%s", capacidad);
                if (verificarFormatoNombre(nombreAula) == TRUE){
                    if (verificarFormatoCapacidad(capacidad) == TRUE){
                        snprintf(consult, 200, "INSERT INTO Aulas (nombre, capacidad) VALUES (\'%s\', \'%i\')", nombreAula, atoi(capacidad));
                        if (mysql_query(conn, consult)){
                            printf("\nLos datos ya existen\n");
                        } else {
                            printf("\nSe ingreso el Aula %s, capacidad: %i\n", nombreAula, atoi(capacidad));
                        }    
                    } else {
                        printf("\nEl formato de capacidad no es valido\n");
                    }
                } else {
                    printf("\nEl formato del nombre del aula no es valido\n");
                }
                memset(nombreAula,0,90);
                memset(capacidad,0,90);
                memset(consult,0,200);
                count=0;
                count2=0;
            }
            if (bandera==2 && caracter!=','){
                capacidad[count]=caracter;
                count+=1;
            }
            if(bandera==1 && caracter!='\n'){
                nombreAula[count2]=caracter;
                count2+=1;
            }  
        }
    }
    fclose(archivo);
}

int InsertarProfesores(){
    char consulta[200];
    int cedula;
    char nombre[90];
    char temp;

    printf("\nIngrese el nombre del profesor: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",nombre);

    printf("Ingrese la cedula del profesor: ");
    scanf(" %i", &cedula);

    snprintf(consulta, 200, "INSERT INTO Profesores (cedula, nombre) VALUES (\'%i\', \'%s\')", cedula, nombre);

    if (mysql_query(conn, consulta)){
        printf("\nNo se pudo ingresar los datos\n");
        return (1);
    } else {
        printf("\nSe ingreso el profesor %s, cedula: %i\n", nombre, cedula);
        return (1);
    }
}

int ListarProfesores(){
    if (mysql_query(conn, "SELECT * FROM Profesores")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nCedula\t\tNombre\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\t%s \n", row[0], row[1]);
    }
}

int BorrarTodoProfesores(){
    if (mysql_query(conn, "DELETE FROM Profesores")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	} else {
        printf("\nSe elimino todos los profesores\n");
        return (1);
    }
}

int ListarCursos(){
    if (mysql_query(conn, "SELECT * FROM Cursos")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nCodigo de carrera\tCodigo de curso\t\tNombre del curso\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\t\t\t%s\t\t\t%s \n", row[0], row[1], row[2]);
    }
}

int validarYear(int year){
    if (year >= 1971){
        return TRUE;
    }else{
        return FALSE;
    }
}

int validarPeriodo(int periodo){
    if (periodo == 1 || periodo == 2){
        return TRUE;
    } else {
        return FALSE;
    }
}

int InsertarPeriodo(){
    char consulta[300];
    char codCurso[7];
    int year;
    int periodo;
    int grupo;
    int cedulaProfesor;
    int cantEstudiantes;
    char temp;

    printf("\n");
    ListarCursos();

    printf("\nIngrese el codigo del curso: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",codCurso);

    printf("Ingrese el anio del curso: ");
    scanf(" %i", &year);

    printf("Ingrese el periodo: ");
    scanf(" %i", &periodo);

    printf("Ingrese el grupo: ");
    scanf(" %i", &grupo);

    printf("\n");
    ListarProfesores();
    printf("Ingrese la cedula del profesor: ");
    scanf(" %i", &cedulaProfesor);

    printf("Ingrese la cantidad de estudiantes: ");
    scanf(" %i", &cantEstudiantes);

    if (validarPeriodo(periodo) == TRUE && validarYear(year) == TRUE){
        snprintf(consulta, 300, "INSERT INTO CursosPorPeriodo (codCurso, year, periodo, grupo, cedulaProfesor, cantEstudiantes) VALUES (\'%s\',\'%i\',\'%i\',\'%i\',\'%i\',\'%i\')", codCurso, year, periodo, grupo, cedulaProfesor, cantEstudiantes);
        if (mysql_query(conn, consulta)){
            printf("\nNo se pudo ingresar los datos\n");
            return (1);
        } else {
            printf("\nSe ingreso el Curso por periodo con existo");
            return (1);
        }
    }else {
        printf("\nDatos erroneos...\n");
        return (1);
     }
}

int ListarPeriodos(){
    if (mysql_query(conn, "SELECT * FROM CursosPorPeriodo")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nCod. curso\tAnio\t\tPeriodo\t\tGrupo\t\tCed. profesor\t\tCant. Estudiantes\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s \n", row[0], row[1], row[2], row[3], row[4], row[5]);
    }
}

int BorrarPeriodos(){
    char consulta[300]; 
    char codCurso[7];
    int year;
    int periodo;
    int grupo;
    char temp;

    ListarPeriodos();
    printf("\nIngrese el codigo del curso a eliminar: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",codCurso);

    printf("Ingrese el anio del curso a eliminar: ");
    scanf(" %i", &year);

    printf("Ingrese el periodo a eliminar: ");
    scanf(" %i", &periodo);

    printf("Ingrese el grupo a eliminar: ");
    scanf(" %i", &grupo);

    snprintf(consulta, 300, "DELETE FROM CursosPorPeriodo WHERE codCurso = \'%s\' AND year = \'%i\' AND periodo = \'%i\' AND grupo = \'%i\'", codCurso, year, periodo, grupo);
    if (mysql_query(conn, consulta)) {
        return (1);
	} else {    
        return (1);
    }
}

void terminarConexion(){
    mysql_free_result(res);
	mysql_close(conn);
}