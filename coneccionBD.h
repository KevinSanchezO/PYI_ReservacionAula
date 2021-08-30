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
char *password = "holamundo1"; //Kevin
//char *password = "holamundo1";  //Jirgort 
char *database = "DB_Reservacion_Aulas";
FILE *archivo; 
struct ReservacionAulas{
    
};

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
    mysql_free_result(res);
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
////////////////////////////////////////////////////////////*********
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





//*********************************************************************************************
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
//////////////////////////////////////////////////
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
//************************************************************************************************
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






/////////////////////*************************************************************************************
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








///////////////////////////////////////////////////////







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

///////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////
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
        printf("\nFALLOOO! ");
    }
    printf("\nIngrese la hora de inicio: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&horaInicio);
    if(isHora(horaInicio)){
        printf("\nfacilito! ");
    }
    printf("\nIngrese la hora de fin: ");
    scanf("%c",&temp); // temp statement to clear buffer
	scanf("%[^\n]",&horaFin);
    if(isHora(horaFin)){
        printf("\nfacilito! ");
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
int pedirCodReservacion(){
    int codReservacion;
    printf("\nIngrese el codigo de reservacion: ");
   // temp statement to clear buffer
	scanf("%i",&codReservacion);
    cancelarReservacion(codReservacion);
}
void terminarConexion(){
    mysql_free_result(res);
	mysql_close(conn);
}