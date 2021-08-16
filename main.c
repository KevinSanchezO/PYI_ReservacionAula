#include <stdio.h>
#include <stdlib.h>


void menuOperativas(){
    char op = '0';
    system("cls");
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
        default:
            printf("\nOpcion no valida...\n");
            break;
    }
}


void menuGenerales(){
    char op = '0';
    system("cls");
    printf("\n___________________OPCIONES GENERALES___________________");
    printf("\n1. Consulta por dia");
    printf("\n2. Consulta por aula");
    printf("\n3. Consulta por curso");
    printf("\n0. Volver\n>>");
    scanf(" %c",&op);

    switch (op){
        case '0':
            break;
        default:
            printf("\nOpcion no valida...\n");
            break;
    }
}


void mainMenu(){
	char op = '0';
	do{
        //system("cls");
		printf("\n_______________________MENU PRINCIPAL_______________________");
        printf("\n[1. Opciones operativas | 2. Opciones generales | 0. Salir]\n>>");
		scanf(" %c",&op);                   //siempre dejar un espacio al principio en el scanf que identifica el tipo

		switch (op){
			case '0':
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


int main(){
    mainMenu();
    return 0;
}
