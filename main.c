#include <stdio.h>
#include "coneccionBD.h"
#include "prototipos.h"


int main(){
    ConectarBD();
    mainMenu();
    return 0;
}

void mainMenu(){
	char op = '0';
	do{
        //system("cls");
		printf("\n_______________________MENU PRINCIPAL_______________________");
        printf("\n[1. Opciones operativas | 2. Opciones generales | 0. Salir]\n>>");
		scanf(" %c",&op);  //siempre dejar un espacio al principio en el scanf que identifica el tipo

		switch (op){
			case '0':
                terminarConexion();
				break;
			case '1':
                menuOperativas();
				break;
            case '2':
                menuGenerales();
                break;
			default:
                printf("\nOpcion no valida...\n");
                break;
		}
	}while(op != '0');
}

void menuOperativas(){
    char op = '0';
    //system("clear");
    printf("\n__________________OPCIONES OPERATIVAS__________________");
    printf("\n1. Informacion Aulas");
    printf("\n2. Informacion de profesores");
    printf("\n3. Informacion de cursos");
    printf("\n4. Cursos por periodo");
    printf("\n5. Reservacion de aula");
    printf("\n6. Cancelar reservacion");
    printf("\n7. Estadisticas");
    printf("\n0. Volver\n>>");
    scanf(" %c",&op);

    switch (op){
        case '0':
            break;
        case '1':
            infoAulas();
            break;
        case '2':
            infoProfesores();
            break;
        case '3':
            infoCursos();
            break;
        case '4':
            cursosX_Periodo();
            break;
        case '5':
            incluirReservacion();
            break;
        case '6':
            pedirCodReservacion();
            break;
        case '7':
            estadisticas();
            break;
        default:
            printf("\nOpcion no valida...\n");
            break;
    }
}
void infoAulas(){
    char op = '0';
    printf("\nDigite una operacion \n[1. Incluir | 0.Salir]\n>>");
    scanf(" %c",&op);

    switch (op) {
        case '0':
            break;
        case '1':
            insertarAulas();
            break;
        default:
            printf("\nOpcion no valida...\n");
            break;
    }
}
void infoProfesores(){
    char op = '0';
    printf("\nDigite una operacion \n[1. Incluir | 2. Listar | 3.Borrar todo]\n>>");
    scanf(" %c",&op);

    switch (op) {
        case '1':
            InsertarProfesores();
            break;
        case '2':
            ListarProfesores();
            break;
        case '3':
            BorrarTodoProfesores();
            break;
        default:
            printf("\nOpcion no valida...\n");
            break;
    }
}
void infoCursos(){
    printf("\n_______________LISTA DE CURSOS_______________\n");
    ListarCursos();
}

void cursosX_Periodo(){
    char op = '0';
    printf("\nDigite una operacion \n[1. Incluir | 2. Listar | 3.Borrar]\n>>");
    scanf(" %c",&op);

    switch (op) {
        case '1':
            InsertarPeriodo();
            break;
        case '2':
            ListarPeriodos();
            break;
        case '3':
            BorrarPeriodos();
            break;
        default:
            printf("\nOpcion no valida...\n");
            break; 
    }
}
void estadisticas(){//+add
    printf("\nA. Top 3 Aulas mas reservadas\n");
    topAulasReservadas();
    printf("\nB. Top 3 Profesores con mas reservas\n");
    topProfesoresReservas();
    printf("\nC. Cantidad de reservaciones anio-mes\n");
    cantReservacionesYear();
}

void menuGenerales(){
    char op = '0';
    //system("clear");
    printf("\n___________________OPCIONES GENERALES___________________");
    printf("\n1. Consulta por dia");
    printf("\n2. Consulta por aula");
    printf("\n3. Consulta por curso");
    printf("\n0. Volver\n>>");
    scanf(" %c",&op);

    switch (op){
        case '1':
            consultaPorDia();
            break;
        case '2':
            consultaPorAula();
            break;
        case '3':
            consultaPorCurso();
            break;
        default:
            printf("\nOpcion no valida...\n");
            break;
    }
}

//mysql -h localhost -u root -p

//gcc -o main $(mysql_config --cflags) main.c $(mysql_config --libs)
