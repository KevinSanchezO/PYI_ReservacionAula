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
struct ReservacionAulas{
    
};
/*
*Metodo que conecta la base de datos con el sistema de reservacionde aula
*E:Variables globales: Server,user,password,database
*S:Conexion con la base de datos
*R:Preasignar las varibles globales de entrada
*/
int ConectarBD(){
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){ 
		fprintf(stderr, "%s\n", mysql_error(conn));
       
		exit(1);
	}
}
/*
*Funcion para verificar que sea correcto el nombre de las Aulas
*E: Un nombre de aula tipo char
*S: Retorna verdadero si el nombre tiene la estructura correcta de lo contrario retorna falso
*R:El nombre no puede ser vacio, y la estructura es de una letra y un numero
*/
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
/*
*Funcion para verificar que sea correcta la capacidad del aula
*E:La capacidad del aula
*S:Verdadero si la capacidad es un numero entero de lo contrario retorna falso
*R:La cantidad no puede ser vacia ni cero
*/
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
/*
*Metodo para pedir la ruta de un archivo e insertar los nombres de aulas y la cantidad en la base de datos 
*E:Una ruta del archivo .txt
*S:Mesaje exito en el caso que los datos son correctos de lo contrario un mensaje de error
*R:El archivo .txt debe tener la estructura definida por el cliente
*/

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
                char *nombre = nombreAula;

                if (verificarFormatoNombre(nombreAula) == TRUE){
                    if (verificarFormatoCapacidad(capacidad) == TRUE){
                        snprintf(consult, 200, "INSERT INTO Aulas (nombreAula, capacidad) VALUES (\'%s\', \'%i\')", nombreAula, atoi(capacidad));
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
/*
*Metodo usado para insertar profesores a la base de datos
*E:El nombre del profesor y la cedula del mismo
*S:Mensaje de exito si los datos ingresados son validos de lo contrario un mensaje de error
*R:La cedula tiene que ser de tipo entero y el nombre tipo char
*/

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

    snprintf(consulta, 200, "INSERT INTO Profesores (cedula, nombreProfesor) VALUES (\'%i\', \'%s\')", cedula, nombre);

    if (mysql_query(conn, consulta)){
        printf("\nNo se pudo ingresar los datos\n");
        return (1);
    } else {
        printf("\nSe ingreso el profesor %s, cedula: %i\n", nombre, cedula);
        return (1);
    }
} 
/*
*Metodo que muestra la lista de profesores guardada en la base de datos
*E:No tiene
*S:Lista de profesores con nombre y cedula
*R: No tiene
*/
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
/*
*Funcion que borra a todos los profesores guardados en la base de datos
*E:No tiene
*S:Mensaje que indica que los profesores fueron borrados de la base de datos
*R:No tiene
*/
int BorrarTodoProfesores(){
    if (mysql_query(conn, "DELETE FROM Profesores")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	} else {
        printf("\nSe elimino todos los profesores\n");
        return (1);
    }
}
/*
*Funcion que se encarga de listar todos los cursos guardados en la base de datos
*E:No tiene
*S:El codigo de carrera, el codigo del curso y el nombre del curso
*R:No tiene
*/
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
    mysql_free_result(res);
}
/*
*Funcion encargada de validar que el anio sea correcto
*E: un entero
*S: Verdadero si el anio es mayor a 1971 de lo contrario retorna falso
*R:El numero debe ser mayor a 1971
*/
int validarYear(int year){
    if (year >= 1971){
        return TRUE;
    }else{
        return FALSE;
    }
}
/*
*Funcion que se encarga de validar el formato correcto del periodo 
*E:un entero
*S:Verdadero si el periodo es igual a 1 o 2, de lo contrario retorna falso
*R:El numero debe ser un entero que sea 1 o 2
*/
int validarPeriodo(int periodo){
    if (periodo == 1 || periodo == 2){
        return TRUE;
    } else {
        return FALSE;
    }
}
/*
*Funcion encargada de insertar un curso por periodo a la base de datos
*E:Un codigo de curso tipo char, un anio tipo entero, un periodo tipo entero,un grupo tipo entero,
** cedula de profesor tipo entero,cantidad de estudiantes tipo entero
*S:Un mensaje de exito si los datos eran correctos de lo contraririo un mensaje de error
*R:La unicidad la dara el codigo del curso, anio, periodo y grupo para la base de datos
*/
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
/*
*Funcion encargada de listar los cursos por periodo
*E:No tiene
*S:Muestra el codigo del curso,anio,periodo,grupo,cedulad del profesor y cantidad de estudiantes
*R:No tiene
*/
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
/*
*Funcion que se encarga de borrar el periodo seleccionado
*E:Codigo del curso tipo char,periodo tipo entero,grupo tipo entero
*S:No hay
*R:Respetar el formato de entrada
*/
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
/*
*Funcion encargada de listar todas la aulas
*E:No tiene
*S:Una lista de todas las aulas guardadas en la base de datos por nombre y cantidad
*R:No tiene
*/

int listarAulas(){
    if (mysql_query(conn, "SELECT * FROM Aulas")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nNombre\t\tCantidad\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\t%s \n", row[0], row[1]);
    }
}

/*
*Funcion que validad si la capacidad del aula es mayor a la cantidad de estudiantes
*E:Un codigo de curso tipo char, nombre de aula tipo char
*S:Verdaero si la caontidad de estudiantes es mayor que la capacidad del aula de lo contrario retorna falso
*R: Respetar formato de entrada
*/

int validarCapacidad(char *codCurso,char *nombreAula){
    char consulta[300];
    char consulta1[300];
    int cantEstudiantes;
    int capacidadAula;
    snprintf(consulta, 300, "SELECT CantEstudiantes FROM CursosPorPeriodo where codCurso='%s'",codCurso);
    if (mysql_query(conn, consulta)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL) {
        cantEstudiantes=atoi(row[0]);
		

    }
    snprintf(consulta1, 300, "SELECT capacidad FROM Aulas where nombreAula='%s'",nombreAula);
    if (mysql_query(conn, consulta1)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL) {
        capacidadAula=atoi(row[0]);
		
    }
    if(capacidadAula<cantEstudiantes){
        return FALSE;
    }else{
        return TRUE;
    }



}
/*
*Funcion que valida que se este respetando las demas horas de reservacion para que no exista choques de horario
*E:La hora de inicio tipo char,la hora final tipo char, la hora de inicio de la reservacion actual, la hora de fin de la reservacion actual
*S:Restorna verdadero si la hora-fin introducida no choca con las horas-fin de las reservaciones guardadas en la base de datos
*R:La hora-fin no puede estar entre otra hora-fin previamente guardada en la base de datos
*/
int isHora2(char* horaInicial,char* horaFinal,char *horaBDInicial,char* horaBDFinal){
    char *end;
    char *end2;
    char *end3;
    char *end4;
    int base=10;
    int horasInicial,minutosInicial,horasBDInicial,horasBDFin,minutosBDInicial,horasFinal,minutosFin,minutosBDFin;
    horasInicial=strtol(horaInicial,&end,base);
    horasBDInicial=strtol(horaBDInicial,&end2,base);
    horasFinal=strtol(horaFinal,&end3,base);
    horasBDFin=strtol(horaBDFinal,&end4,base);
    printf("%d\n",horasInicial);
    printf("%d\n",horasBDInicial);
    printf("%d\n",horasFinal);
    printf("%d\n",horasBDFin);
    if (horasInicial<horasBDInicial){
        if(horasFinal<horasBDInicial){
            return TRUE;
        }else{
            return FALSE;
        }
            
    }else{
        if(horasInicial>horasBDInicial){
            if(horasInicial>horasBDFin){
                return TRUE;
            }else{
                return FALSE;
            }
        }else{
            if(horasInicial==horasBDInicial){
                if(end[0]==':' && end2[0]==':' && end3[0]==':' && end4[0]==':'){
                    minutosInicial=strtol(end+1,&end,base);//hora

                    minutosBDInicial=strtol(end2+1,&end2,base);//horaB

                    minutosFin=strtol(end3+1,&end3,base);//horaL
                    minutosBDFin=strtol(end4+1,&end4,base);//horaB2
                    printf("%d\n",minutosInicial);
                    printf("%d\n",minutosBDInicial);
                    printf("%d\n",minutosFin);
                    printf("%d\n",minutosBDFin);
                    if(minutosInicial>minutosBDInicial){
                        if(horasInicial==horasBDFin){
                            if(minutosInicial>minutosBDFin){
                                return TRUE;
                            }else{
                                return FALSE;
                            }
                        }
                    }else{
                        if(minutosFin>minutosBDInicial){
                            return FALSE;
                        }else{
                            return TRUE;
                        }
                    }
                }
       
       
            
            
               
            }
        }
    }
   
    
}

/*
*Funcion encargada de validar si la fecha-hora-inicio-fin es la correcta para ser ingresada a la base de datos
*E:Una fecha(dd/mm/yyyy) tipo char, una hora de inico tipo char, una hora final tipo char, un aula tipo char,
* un codigo de curso tipo char, un periodo tipo entero, un anio tipo anio, un grupo tipo entero
*S:DEvuelve verdadero si la fecha-hora-inicio-fin es valida y no choca con el horario de las demas reservaciones
*R:La fecha-hora-fin no puede estar entre otra fecha-hora-fin previamente guardada en la base de datos
*/
int validarFecha(char *fecha,char *horaInicio, char *horaFin,char *aula,char *codCurso,int periodo,int anio,int grupo){
    char consulta[300];
    char consulta1[300];
    char consulta2[300];
    int cont=0;
    char date[30];
    char xd[30];

    char horaI[30];
    char horaS[30];

    char horaF[30];
    char horaL[30];

    char *fec;
    char *fec1;
   
    snprintf(consulta, 300, "SELECT fecha,horaInicio,horaFin FROM ReservacionAulas where nombreAula='%s' AND periodo='%i' AND anio='%i'",aula,periodo,anio);
    if (mysql_query(conn, consulta)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
   
	while ((row = mysql_fetch_row(res)) != NULL) {
        fec=row[0];
        strcpy(date,fec);
        strcpy(xd,fecha);
        if(strcmp(xd,date)==0){
            fec=row[1];
            fec1=row[2];
            strcpy(horaF,fec1);
            strcpy(horaL,horaFin);

            strcpy(horaI,fec);
            strcpy(horaS,horaInicio);
            if(isHora2(horaS,horaL,horaI,horaF)){

            }else{
              cont+=1;
            }
            
        }else{
             
        }
		
    }
    if(cont==0){
        return TRUE;
    }else{
        return FALSE;
    }
    

}


/*
*Funcion encargada de listar las reservaciones guradadas en la base de datos
*E:No tiene
*S:Muestra el codigo de reservacion,fecha,hora de inicio, hora final, codigo del curso,periodo, anio, grupo, nombre del aula
*R:No tiene
*/
int listarReservaciones(){
    if (mysql_query(conn, "SELECT * FROM ReservacionAulas")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nCod. reservacion\tfecha\t\thora. inicio\t\thora. fin\t\tCod. curso\t\tperiodo\t\tanio\t\tgrupo\t\tnombre. aula\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\t\t\t%s\t\t%s\t\t%s\t\t%s\t\t\t%s\t\t%s\t\t%s\t\t%s\n", row[0], row[1], row[2], row[3], row[4], row[5],row[6],row[7],row[8]);
    }
}

/*
*Funcion encargada de mostrar el codigo de reservacion cuando se guarda una nueva reservacion
*E:una fecha tipo char, hora de inicio tipo char, una hora fin tipo char, un nombre de aula tipo char, un anio tipo entero,un periodo tipo entero,
**un codigo de curso tipo char, un grupo tipo entero
*S:Muestra el codigo de reservacion de la reservacion exitosa
*R:Respetar el formato de entrada
*/

int MostrarIdReservacion(char *fecha,char *horainicio,char *horaFin,char *aula,int anio,int periodo,char *codCurso,int grupo){
    char consulta[300];
    snprintf(consulta, 300, "SELECT codReservacion FROM ReservacionAulas where fecha='%s' AND horaInicio='%s' AND horaFin='%s' AND codCurso='%s' AND grupo='%i' AND nombreAula='%s' AND periodo='%i' AND anio='%i'",fecha,horainicio,horaFin,codCurso,grupo,aula,periodo,anio);
    if (mysql_query(conn, consulta)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nCod. reservacion\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\n", row[0]);
    }
}
/*
*Funcion qe validad si el formato de fecha es correcto
*E:Una fecha tipo char(dd/mm/yyyy)
*S:Verdadero si el formato de fecha es el correcto de lo contrario Falso
*R:El dia debe ser mayor a uno y menor a 31, el mes tiene que ser mayor a 0 o menor que 13, el anio debe ser mayor a 1900 y menor a 2100
*/
bool isDate(char* fecha){
    char *end;
    int base=10;
    int anio,mes,dia;
    dia=strtol(fecha,&end,base);
    printf("Dia: %d\n",dia);
    if (dia<1 || dia>31){
            return FALSE;
    }
    if(end[0]=='/'){
       
        mes=strtol(end+1,&end,base);
        printf("mes: %d\n",mes);
        if (mes<1 || mes>12){
            return FALSE;
        }
        
    }else{

    }
    if(end[0]=='/'){
        anio=strtol(end+1,&end,base);
        printf("anio: %d\n",anio);
        if (anio>=1900 || anio<=2100){
                    
            return TRUE;
        }
        
    }else{
          printf("\nDatos erroneos...\n");

    }
     printf("\nDatos erroneos...\n");
     return FALSE;
}

/*
*La funcion valida que la hora sea en formato 24 horas
*E: Una fecha tipo char(hh:mm)
*S: Verdero si la hora es en formato 24 horas con 59 min como maximo
*R: Las horas minimo es 00 y 24 como maximo, los minutos deben ser desde 00 hasta 59 como maximo
*/
bool isHora(char* hora){
    char *end;
    int base=10;
    int horas,minutos;
    horas=strtol(hora,&end,base);
    printf("hora: %d\n",horas);
    if (horas<0 || horas>24){
            
            
            return FALSE;
        }
    if(end[0]==':'){
       
       
       
        minutos=strtol(end+1,&end,base);
        printf("minutos: %d\n",minutos);
        if (minutos<0 || minutos>59){
            return TRUE;
        }
        
    }else{
        return FALSE;
    }
    
}

/*
*Funcion encargada de reservar las aulas y gardar en la base de datos la informacion
*E: Por medio de entrada en consola Una fecha, una hora de inicio, una hora de fin,
** el nombre de una aula, el anio, el periodo, el codigo del curso y el grupos
*S: Un mensaje de exito si los datos fueron correctos y se insertaron en la base de datos correctamente
*R: Respetar la entrada de cada uno de los formatos
*/
int incluirReservacion(){
    char consulta[300];
    char codCurso[7];
    int year;
    char fecha[50];
    char horaInicio[50];
    char horaFin[50];
    char aula[20];
    int grupo;
    int periodo;
    int anio;
    int dia;
    int cedulaProfesor;
    int cantEstudiantes;
    char temp;
    //listarReservaciones();
    printf("\n");
   
    
    printf("\nIngrese la fecha: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&fecha);

    if(!isDate(fecha)){
        printf("\nError de formato fecha\n");
        return (1);
    }
    printf("\nIngrese la hora de inicio: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&horaInicio);
    if(!isHora(horaInicio)){
        printf("\nError de formato hora\n");
        return (1);
    }
    printf("\nIngrese la hora de fin: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&horaFin);
    if(!isHora(horaFin)){
        printf("\nError de formato hora\n");
        return (1);
    }
    listarAulas();
    printf("\nIngrese el nombre del aula: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&aula);

    printf("\nIngrese el anio: ");
   // temp statement to clear buffer
	scanf("%i",&anio);

    printf("\nIngrese el periodo: ");
   // temp statement to clear buffer
	scanf("%i",&periodo);
    ListarCursos();
    printf("\nIngrese el codigo de curso: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&codCurso);
    printf("\nIngrese el grupo: ");
   // temp statement to clear buffer
	scanf("%i",&grupo);
    if (validarPeriodo(periodo) == TRUE && validarYear(anio) == TRUE){
        if(validarFecha(fecha,horaInicio,horaFin,aula,codCurso,periodo,anio,grupo)){
                //system("clear");
                mysql_free_result(res);
                if(validarCapacidad(codCurso,aula)==TRUE){
                    snprintf(consulta, 300, "INSERT INTO  ReservacionAulas(codReservacion, fecha, horaInicio, horaFin, codCurso, periodo,anio,grupo,nombreAula) VALUES (\'%i\',\'%s\',\'%s\',\'%s\',\'%s\',\'%i\',\'%i\',\'%i\',\'%s\')", 0, fecha, horaInicio, horaFin, codCurso, periodo,anio,grupo,aula);
                    if (mysql_query(conn, consulta)){
                         printf("\nNo se pudo ingresar los datos\n");
                        return (1);
                    } else {
                        MostrarIdReservacion(fecha,horaInicio,horaFin,aula,anio,periodo,codCurso,grupo);
                        printf("\nSe reservo el aula con existo!");
            
                        return (1);
                    }
                }else{
                     //system("clear");
                    printf("\nLa cantidad de estudiantes exede la capacidad del aula...\n");
                    return (1);
                }
        }else{
             //system("clear");
             mysql_free_result(res);
             printf("\nEl aula no esta disponible para la fecha y hora...\n");
                    return (1);
        }
        
       
    }else {
        printf("\nDatos erroneos...\n");
        return (1);
     }
}
/*
*Funcion encargada de la cancelacion y lberacion de las aulas
*E:Un codigo de reservacion tipo entero
*S: Un mensaje de exito si se elimino correctamente la reservacion de lo contrario un mensaje de error
*R: el codigo de reservacion debe ser un entero
*/
int cancelarReservacion(int codReservacion){
    
    char consulta2[300];
    snprintf(consulta2, 300, "SELECT codReservacion FROM ReservacionAulas where codReservacion='%i'",codReservacion);
    if (mysql_query(conn, consulta2)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        printf("\nDatos erroneos...\n");
        return (1);
	}
    
    res = mysql_use_result(conn);
   
	while ((row = mysql_fetch_row(res)) != NULL) {
        
        if(atoi(row[0])==codReservacion){
            mysql_free_result(res);
            char consulta[300];
            snprintf(consulta, 300, "DELETE FROM ReservacionAulas where codReservacion='%i'",codReservacion);
            if (mysql_query(conn, consulta)) {
		        fprintf(stderr, "%s\n", mysql_error(conn));
                printf("\nDatos erroneos...\n");
                return (1);
	        }else{
                printf("\nLa reservacion se cancelo exitosamente\n");
                return (1);
            }
            
            
            
        }else{
           
        }
		
    }
    printf("\nEl codigo ingresado no pertenece a ninguna aula reservada\n");

    
    
}
/*
*Se encarga de almacenar el codigo de reservacion y llamar a la funcion que que ara la cancelacion 
*E: Un codigo de reservacion tipo entero
*S: llamada a otra funcion para realizar la cancelacion
*R:la entrada debe ser un entero
*/
int pedirCodReservacion(){
    int codReservacion;
    printf("\nIngrese el codigo de reservacion: ");
   // temp statement to clear buffer
	scanf("%i",&codReservacion);
    cancelarReservacion(codReservacion);
}
/*
*Funcion que muestra las 3 aulas mas reservadas de mayor a menor
*E:No tiene
*S:El top 3 de las aulas mas reservadas
*R:No tiene
*/
void topAulasReservadas(){//+add
    if (mysql_query(conn, "SELECT nombreAula, COUNT(nombreAula) AS MOST_FREQUENT FROM ReservacionAulas GROUP BY nombreAula ORDER BY COUNT(nombreAula) DESC")){
		fprintf(stderr, "%s\n", mysql_error(conn));
        return;
	} else {
        res = mysql_use_result(conn);
	    for (int x = 1; x<4; x++) {
            row = mysql_fetch_row(res);
            if (row == NULL){
                mysql_free_result(res);
                return;
            } else {
                printf("\n\t%i. %s\n", x, row[0]);
            }
        }
        mysql_free_result(res);
    }
}

/*
*Funcion que muestra el top 3 de los profesores con mas reservas
*E:No tiene
*S: El top 3 de los profesores con mas reservaciones de aulas
*R:No tiene
*/
void topProfesoresReservas(){
    if (mysql_query(conn, "SELECT P.nombreProfesor, P.cedula, COUNT(CP.cedulaProfesor) AS MOST_FREQUENT FROM ReservacionAulas R INNER JOIN CursosPorPeriodo CP ON R.codCurso = CP.codCurso INNER JOIN Profesores P ON CP.cedulaProfesor = P.cedula GROUP BY nombreProfesor, cedula ORDER BY COUNT(CP.cedulaProfesor) DESC")){
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    } else {
        res = mysql_use_result(conn);
	    for (int x = 1; x<4; x++) {
            row = mysql_fetch_row(res);
            if (row == NULL){
                mysql_free_result(res);
                return;
            } else {
                printf("\n\t%i. nombre: %s\tcedula: %s\n", x, row[0], row[1]);
            }
        }
        mysql_free_result(res);
    }
}


/*
*Funcion que muestra la cantidad de reservaciones por fecha
*E: N/A
*S: La cantidad de reservaciones por fecha
*R: N/A
*/
void cantReservacionesYear(){
    if (mysql_query(conn, "SELECT DATE_FORMAT(STR_TO_DATE(fecha,'%d/%m/%Y'),'%m/%Y'), Count(codReservacion) AS RANKING FROM ReservacionAulas GROUP BY DATE_FORMAT(STR_TO_DATE(fecha,'%d/%m/%Y'),'%m/%Y') ORDER BY COUNT(codReservacion) DESC")){
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    } else {
        res = mysql_use_result(conn);
        while ((row = mysql_fetch_row(res))!=NULL) {
            printf("\n\tFecha: %s, cantidad de reservaciones: %s\n", row[0], row[1]);
        }
        mysql_free_result(res);
    }
}


///MENU DE GENERALIDADES**************
/*
*Funcion que se encarga de mostrar datos de la reserva segun la fecha
*E:Una fecha tipo char(dd/mm/yyyy)
*S:Muestra en consola el nombre del aula, el codigo de reservacion, el anio, el periodo, el codigo del curso, el grupo, la hora de incio y la hora final 
*R:Los datos deben mostrarse a partir de la fecha introducida
*/

int consultaPorDia(){
     char fecha[50];
     char temp;
     char consulta[300];
    printf("\nIngrese la fecha: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&fecha);

    if(!isDate(fecha)){
        printf("\nIngrese una fecha valida ");
    }
    snprintf(consulta, 300, "SELECT nombreAula,codReservacion,anio,periodo,codCurso,grupo,horaInicio,horaFin FROM ReservacionAulas where fecha='%s' ",fecha);
    if (mysql_query(conn, consulta)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nNombre de Aula\t\tCodigo de reservacion\t\tAnio\t\tperiodo\t\tCodigo del curso\t\tGrupo\t\tHora de inicio\t\thora fin\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\t\t\t%i\t\t\t\t%i\t\t%i\t\t%s\t\t\t\t%i\t\t%s\t\t\t%s\n", row[0], atoi(row[1]), atoi(row[2]), atoi(row[3]), row[4], atoi(row[5]),row[6],row[7]);
    }
    mysql_free_result(res);
    return(1);
}
/*
*Funcion que consultar por aula y mostrar datos respectivos a las reservaciones de dicha aula 
*E: El nombre de una aula
*S: Muestra en consola la fecha, la hora incio, la hora final, el codigo de reservacion, el anio, el periodo, el codigo del curso, el grupo
*R: Mostrar de fomarma ascendente
*/

int consultaPorAula(){
    char aula[50];
    char temp;
    char consulta[300];
    printf("\nIngrese el nombre del aula: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&aula);

    snprintf(consulta, 300, "SELECT fecha,horaInicio,horaFin,codReservacion,anio,periodo,codCurso,grupo FROM ReservacionAulas where nombreAula='%s' ORDER BY STR_TO_DATE(fecha,'%D/%M/%Y') ASC, STR_TO_DATE(horaInicio,'%h:%i') ASC ",aula);
    if (mysql_query(conn, consulta)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nFecha\t\tHora de inicio\t\tHora final\t\tCodigo de reservacion\t\tAnio\t\tPeriodo\t\tCodigo de curso\t\tGrupo\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s\t\t\t%s\t\t\t\t%s\t\t%i\t\t%i\t\t\t\t%i\t\t%s\t\t\t%i\n", row[0], row[1], row[2], atoi(row[3]), atoi(row[4]), atoi(row[5]),row[6],atoi(row[7]));
        
    }
    mysql_free_result(res);
    printf("\nNo se encontro ninguna aula reservada con ese nombre");
    return(1);
}
/*
*Funcion encargada de consultar el anio el periodo, el codigo de curso y grupo para mostrar informacion relacionada a las reservaciones
*E: Un anio tipo entero, un periodo tipo entero, un codigo de curso tipo char, un grupo tipo entero
*S:Muestr en consola el codigo de reservacion, la fecha, la hora inicial, la hora final y el nombre del aula segun la entrada
*R: La informacion debe mostrarse de forma ascendente segun fecha-hora inicial-aula
*/
int consultaPorCurso(){
    char codCurso[50];
    int anio,grupo,periodo;

    char temp;
    char consulta[300];

    printf("\nIngrese el anio: ");
   // temp statement to clear buffer
	scanf("%i",&anio);

    printf("\nIngrese el periodo: ");
   // temp statement to clear buffer
	scanf("%i",&periodo);
    ListarCursos();
    printf("\nIngrese el codigo de curso: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&codCurso);

    printf("\nIngrese el grupo: ");
   // temp statement to clear buffer
	scanf("%i",&grupo);

    if(validarPeriodo(periodo)==FALSE || validarYear(anio)==FALSE){
        printf("\nDatos erroneos...\n");
        return (1);
    }
    snprintf(consulta, 300, "SELECT codReservacion,fecha,horaInicio,horaFin,nombreAula FROM ReservacionAulas where anio='%i' AND periodo='%i' AND codCurso='%s' AND grupo='%i' ORDER BY STR_TO_DATE(fecha,'%D/%M/%Y') ASC, STR_TO_DATE(horaInicio,'%h:%i') ASC, nombreAula ASC ",anio,periodo,codCurso,grupo);
    if (mysql_query(conn, consulta)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        return (1);
	}
    
    res = mysql_use_result(conn);
    printf("\nCodigo de reservacion\t\tFecha\t\tHora de inicio\t\tHora final\t\tNombre del Aula\n");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%i\t\t\t%s\t\t\t\t%s\t\t%s\t\t%s\n", atoi(row[0]), row[1], row[2], row[3], row[4]);
        
    }
    mysql_free_result(res);
    return(1);
}
/*
*Funcion encargada de terminar la conexion con la base de datos mysql
*E:No tiene
*S:Termina la conxion
*R:No tiene
*/
void terminarConexion(){
    mysql_free_result(res);
	mysql_close(conn);
}