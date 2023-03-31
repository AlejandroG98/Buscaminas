/**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas les funciones que tenéis 
 * que implementar en ensamblador.
 * Desde este código se hacen las llamadas a les subrutinas de ensamblador. 
 * ESTE CÓDIGO NO SE PUEDE MODIFICAR Y NO HAY QUE ENTREGARLO.
 * */
 
#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO


/**
 * Constantes
 */
#define DIMMATRIX  10    //Dimensión de la matriz
#define SIZEMATRIX DIMMATRIX*DIMMATRIX  //=100

/**
 * Definición de variables globales
 */
extern int developer;	//Variable declarada en ensamblador que indica el nombre del programador.

// Matriz 10x10 con las minas (Hay 20 minas marcadas).
char mines[DIMMATRIX][DIMMATRIX] = { {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ','*',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ','*',' ','*',' ',' ',' ',' '},
                                     {' ','*','*','*','*','*','*','*',' ',' '},
                                     {' ',' ',' ','*',' ','*',' ',' ',' ',' '},
                                     {' ',' ','*','*','*','*','*','*',' ',' '},
                                     {' ',' ',' ','*',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ',' ',' ',' ',' ',' ',' ','*'} };

// Matriz 10x10 donde se indican las minas marcadas inicialmente (Hay 1 mina marcada).
char marksIni[DIMMATRIX][DIMMATRIX] = { {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' ','M'} };
// Matriz 10x10 donde se marcaran les minas.
char marks[DIMMATRIX][DIMMATRIX];
                                     
short numMines = 19;//Minas que quedan por marcar.

int  rowScreen;	    //Fila para posicionar el cursor en pantalla.
int  colScreen;     //Columna para posicionar el cursor en pantalla.
char charac;        //Carácter leído de teclado y para escribir en pantalla.
char state    = '1';//Estado del juego.
                    // '0': Salimos, hemos pulsado la tecla 'ESC' para salir del juego.
					// '1': Continuamos jugando.
					// '2': Gana la partida, se han marcado todas las minas.
					
int  indexMat = 54;	//Índice para acceder a las matrices mines y marks desde ensamblador.


/**
 * Definición de las subrutinas de ensamblador que se llaman desde C
 */
void countMinesP1();
void showMinesP1();
void showCharBoardP1();
void moveCursorP1();
void mineMarkerP1();
void checkEndP1();
void playP1();

/**
 * Definición de les funciones de C
 */
void clearscreen_C();
void gotoxyP1_C();
void printchP1_C();
void getchP1_C();

void printMenuP1_C();
void printBoardP1_C();

void countMinesP1_C();
void showMinesP1_C();
void showCharBoardP1_C();
void moveCursorP1_C();
void mineMarkerP1_C();
void checkEndP1_C();

void printMessageP1_C();
void playP1_C();


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:	
 * Ninguna.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void clearScreen_C(){
	
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en una posición de la pantalla.
 * 
 * Variables globales utilizadas:	
 * (rowScreen) : Fila de la pantalla donde se situa el cursor.
 * (colScreen) : Columna de la pantalla donde se situa el cursor.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'gotoxyP1'  
 * para poder llamar a esta función guardando el estado de los registros 
 * del procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 */
void gotoxyP1_C(){
	
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter en pantalla en la posición del cursor.
 * 
 * Variables globales utilizadas:	
 * (charac) : Carácter a mostrar.
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'printchP1' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 */
void printchP1_C(){

   printf("%c",charac);
   
}


/**
 * Lea un carácter desde el teclado sin mostrarlo en la pantalla
 * y almacenarlo en la variable (charac). 
 * 
 * Variables globales utilizadas:	
 * (charac) : Carácter leído desde el teclado.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'getchP1' para
 * llamar a esta función guardando el estado de los registros del procesador.
 * Esto se hace porque las funciones de C no mantienen el estado de los 
 * registros.
 */
void getchP1_C(){

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándard (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

    /* ~ICANON para tratar la entrada de teclado carácter a carácter no como línea entera acabada en /n
    ~ECHO para que no se muestre el carácter leído.*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente. */
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   charac = (char) getchar();              
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
   
}



/**
 * Mostrar en pantalla el menú del juego y pide una opción.
 * Sólo acepta una de les opciones correctas del menú ('0'-'9')
 * 
 * Variables globales utilizadas:	
 * (rowScreen) : Fila de la pantalla donde se situa el cursor.
 * (colScreen) : Columna de la pantalla donde se situa el cursor.
 * (charac)    : Carácter lleído desde el teclado.
 * (developer) :((char *)&developer): Variable definida en el código ensamblador.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void printMenuP1_C(){
	clearScreen_C();
    rowScreen = 1;
    colScreen = 1;
    gotoxyP1_C();
    printf("                                     \n");
    printf("            Developed by:            \n");
	printf("         ( %s )   \n",(char *)&developer);
    printf(" ___________________________________ \n");
    printf("|                                   |\n");
    printf("|          MENU MINESWEEPER         |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|          1. countMines            |\n");
    printf("|          2. showMines             |\n");
    printf("|          3. showCharBoard         |\n");
    printf("|          4. moveCursor            |\n");
    printf("|          5. mineMarker            |\n");
    printf("|          6.                       |\n");
    printf("|          7. checkEnd              |\n");
    printf("|          8. Play Game             |\n");
    printf("|          9. Play Game C           |\n");
    printf("|          0. Exit                  |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|             OPTION:               |\n");
    printf("|___________________________________|\n"); 
    
    charac=' ';
    while (charac < '0' || charac > '9') {
      rowScreen = 21;
      colScreen = 23;
      gotoxyP1_C(); 
	  getchP1_C(); 
	  printchP1_C();
	}
	
}


/**
 * Mostrar el tablero de juego en pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:	
 * (rowScreen) : Fila de la pantalla donde se situa el cursor.
 * (colScreen) : Columna de la pantalla donde se situa el cursor.
 *  
 * Esta función se llama des de C y desde ensamblador,
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void printBoardP1_C(){

   rowScreen = 1;
   colScreen = 1;
   gotoxyP1_C();
   printf(" _____________________________________________ \n");	//01
   printf("|                       |                     |\n");	//02
   printf("|      MINESWEEPER      |  Mines to  Mark: __ |\n");	//03
   printf("|_______________________|_____________________|\n");	//04
   printf("|     0   1   2   3   4   5   6   7   8   9   |\n");	//05
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//06
   printf("| 0 |   |   |   |   |   |   |   |   |   |   | |\n");	//07
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//08
   printf("| 1 |   |   |   |   |   |   |   |   |   |   | |\n");	//09
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//10
   printf("| 2 |   |   |   |   |   |   |   |   |   |   | |\n");	//11
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//12
   printf("| 3 |   |   |   |   |   |   |   |   |   |   | |\n");	//13
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//14
   printf("| 4 |   |   |   |   |   |   |   |   |   |   | |\n");	//15
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//16
   printf("| 5 |   |   |   |   |   |   |   |   |   |   | |\n");	//17
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//18
   printf("| 6 |   |   |   |   |   |   |   |   |   |   | |\n");	//19
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//20
   printf("| 7 |   |   |   |   |   |   |   |   |   |   | |\n");	//21
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//22
   printf("| 8 |   |   |   |   |   |   |   |   |   |   | |\n");	//23
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//24
   printf("| 9 |   |   |   |   |   |   |   |   |   |   | |\n");	//25
   printf("|   +---+---+---+---+---+---+---+---+---+---+ |\n");	//26
   printf("|_____________________________________________|\n");	//27
   printf("|                                             |\n");	//28
   printf("|                                             |\n");	//29
   printf("|_____________________________________________|\n");	//30
   printf("|                                             |\n");	//31
   printf("|   (m)Mark Mine                  (ESC)Exit   |\n"); //32
   printf("|   (i)Up    (j)Left    (k)Down    (l)Right   |\n"); //33
   printf("|_____________________________________________|\n");	//34
   
}

/**
 * Recorrer la matriz (mines) para contar el número de minas que hay.
 * 
 * Variables globales utilizadas:	
 * (mines)    : Matriz donde ponemos las minas.
 * (numMines) : Minas que quedan por marcar.
 *  
 * Esta función no es llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'countMinesP1'.
 */
void countMinesP1_C(){
   int i,j;
   
   numMines = 0;
   for (i=0;i<DIMMATRIX;i++){
	  for (j=0;j<DIMMATRIX;j++){
         if(mines[i][j]=='*') numMines++;
      }
   }
   
}


/**
 * Convierte el valor del número de minas que quedan por marcar (numMines)
 * (entre 0 y 99) a dos caracteres ASCII. 
 * Se tiene que dividir el valor (numMines) entre 10, el cociente 
 * representará las decenas y el residuo las unidades, y después se
 * tienen que convertir a ASCII sumando 48, carácter '0'.
 * Mostrar los dígitos (carácter ASCII) de les decenas en la fila 3, 
 * columna 44 de la pantalla y las unidades en la fila 3, columna 45.
 * Para posicionar el cursor se llama a la función gotoxyP1_C y para 
 * mostrar los caracteres a la función printchP1_C.
 * 
 * Variables globales utilizadas:
 * (rowScreen) : Fila de la pantalla donde se situa el cursor.
 * (colScreen) : Columna de la pantalla donde se situa el cursor.
 * (numMines)  : Minas que quedan para marcar.
 * (charac)    : Carácter a escribir en pantalla.
 * 
 * 
 * Esta función no es llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'showMinesP1'.  
 */
 void showMinesP1_C() {
	
	short tens  = numMines/10;//Decenes
	short units = numMines%10;//Unitats
	
	rowScreen = 3;
	colScreen = 44;
	gotoxyP1_C();   
	charac = tens + '0';
	printchP1_C();
	
	colScreen = 45;
	gotoxyP1_C();   
	charac = units + '0';
	printchP1_C();
	
}


/**
 * Mostrar el carácter de la posición indicada por (indexMat) de la
 * matriz (marks) en la posición del cursor en pantalla dentro del 
 * tablero en función del índice de la matriz (indexMat). Dejar el 
 * cursor en la posición donde hemos mostrado el carácter.
 * Para calcular la posición del cursor en pantalla utilizar 
 * esta fórmula:
 * rowScreen=((indexMat/10)*2)+7
 * colScreen=((indexMat%10)*4)+7
 * Para posicionar el cursor llamar a la función gotoxyP1_C y 
 * para mostrar el carácter a la función printchP1_C.
 * 
 * Variables globales utilizadas:	
 * (marks)     : Matriz con las minas marcadas.
 * (indexMat)  : Índice para acceder a las matrices mines y marks desde ensamblador.
 * (rowScreen) : Fila de la pantalla donde se situa el cursor.
 * (colScreen) : Columna de la pantalla donde se situa el cursor.
 * (charac)    : Carácter a escribir en pantalla.
 * 
 * Esta función no es llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'showCharBoardP1',  
 */
void showCharBoardP1_C() {
   
   int row, col;
   
   row = (indexMat/10);
   col = (indexMat%10);
   
   rowScreen=(row*2)+7;
   colScreen=(col*4)+7;
   
   gotoxyP1_C();
   
   charac = marks[row][col];
   printchP1_C();
   
   gotoxyP1_C();
   
}


/**
 * Actualizar la posición del cursor en el tablero, que tenemos indicada
 * con la variable (indexMat), en función de la tecla pulsada (charac). 
 * Si se sale fuera del tablero no actualizar la posición del cursor.
 * (i:arriba, j:izquierda, k:a bajo, l:derecha)
 * Arriba y abajo: ( indexMat = indexMat +/- 10 ) 
 * Derecha ye izquierda( indexMat = indexMat +/- 1 ) 
 * No se tiene que posicionar el cursor en pantalla.
 *  
 * Variables globales utilizadas:
 * (charac)   : Carácter leído de teclado.
 * (indexMat) : Índice para acceder a las matrices mines y marks desde ensamblador.
 *  
 * Esta función no es llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'moveCursorP1'.
 * 
 */
void moveCursorP1_C(){
	
   int row = indexMat/10;
   int col = indexMat%10; 
   
   switch(charac){
      case 'i': //amunt
         if (row>0) indexMat=indexMat-10;
      break;
      case 'j': //esquerra
         if (col>0) indexMat--;
      break;
      case 'k': //avall
         if (row<DIMMATRIX-1) indexMat=indexMat+10;
      break;
      case 'l': //dreta
		 if (col<DIMMATRIX-1) indexMat++;
      break;     
	}
	
}


/**
 * Marcar/desmarcar una mina en la matriz (marks) en la posición actual 
 * del cursor, indicada por la variable (indexMat).
 * Si en aquella posición de la matriz (marks) hay un espacio en blanco
 * y no hemos marcado todas las minas, marcamos una mina poniendo una 
 * 'M' en la matriz (marks) y decrementamos el número de minas que quedan
 * por marcar (numMines), si en aquella posición de la matriz (marks) 
 * hay una 'M', pondremos un espacio (' ') e incrementaremos el número 
 * de minas que quedan por marcar (numMines).
 * Si hay otro valor no cambiaremos nada.
 * No se tiene que mostrar la matriz, sólo actualizar la matriz (marks) 
 * y la variable (numMines).
 * 
 * Variables globales utilizadas:
 * (marks)    : Matriz con las minas marcadas y las minas de las abiertas.
 * (indexMat) : Índice para acceder a las matrices mines y marks desde ensamblador.
 * (numMines) : Minas que quedan por marcar.
 * 
 * Esta función no es llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'mineMarkerP1'.
 */
void mineMarkerP1_C() {
	
	//Para acceder a la matriz en ensamblador no es necesario hacer este cálculo.
	int row = indexMat/10;
    int col = indexMat%10;
    
	if (marks[row][col] == ' ' && numMines > 0) {
		marks[row][col] = 'M';      //Marcar
		numMines--;
	} else {
		if (marks[row][col] == 'M') {
			marks[row][col] = ' ';  //Desmarcar
			numMines++;
		}
	}
		
} 

  
/**
 * Verificar si hemos marcado todas las minas. 
 * Si (numMines==0) cambiar el estado (state='2') "Gana la partida".
 * 
 * Variables globals utilitzades:	
 * (numMines) : Minas que quedan por marcar.
 * (state)    : Estado del juego.
 * 
 * Esta función no es llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'checkEndP1'.
 */
void checkEndP1_C() {
	
	if (numMines == 0) {
		state = '2';
	}
		
} 


/**
 * Mostrar un mensaje debajo del tablero según el valor de la variable 
 * (state).
 * state: '0': Hemos pulsado la tecla 'ESC' para salir del juego.
 * 		  '1': Continuamos jugando.
 * 		  '2': Gana la partida, se han marcado todas las minas.
 * Se espera que se pulse una tecla para continuar.
 *  
 * Variables globales utilizadas:	
 * (rowScreen) : Fila de la pantalla donde se situa el cursor.
 * (colScreen) : Columna de la pantalla donde se situa el cursor.
 * (state)     : Estado del juego.
 * 
 * Esta función se llama des de C y desde ensamblador,
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void printMessageP1_C() {

   rowScreen = 29;
   colScreen = 14;
   gotoxyP1_C();  
   
   switch(state){
      case '0':
         printf("<<<< EXIT: ESC >>>>");
        break;
      case '2':
         printf("++++ YOU WIN ! ++++");
      break;
   }
   
   getchP1_C();
   
}
 

/**
 * Juego del Buscaminas
 * Función principal del juego
 * Permite jugar al juego del buscaminas llamando a todas las funcionalidades.
 *
 * Pseudo código:
 * Inicializar estado del juego, (state='1')
 * Mostrar el tablero de juego llamando la función PrintBoardP1_C).
 * Copiar la matriz (marksIni) a (marks) con las marcas iniciales.
 * Mostrar en el tablero los movimientos marcados inicialmente en (marksIni).
 * Decrementar (numMines) según las minas marcadas inicialmente.
 * Inicializar (indexMat=54) para indicar la posición inicial del cursor.
 * 
 * Mientras (state='1') hacer:
 *   Mostrar en el tablero las minas que quedan por marcar 
 *     llamando a la función showMinesP1_C.
 *   Mostrar el carácter de la posición indicada por (indexMat) de la
 *     matriz (marks) en la posición del cursor en pantalla dentro 
 *     del tablero en función del índice de la matriz (indexMat) y 
 *     dejar el cursor en la posición donde hemos mostrado el carácter  
 *     llamando a la función showCharBoarP1_C.
 *   Leer una tecla y guardarla en la variable (charac) llamando
 *     a la funció getchP1_C.
 *   Según la tecla leída llamaremos a la función correspondiente.
 *     - ['i','j','k' o 'l']       (llamar a la función moveCursorP1_C).
 *     - 'x'                       (llamar a la función mineMarkerP1_C).
 *     - '<ESC>'  (codi ASCII 27) poner (state = '0') para salir.   
 *   Verificar si hemos marcado todas las minas llamando a la función checkEndP1_C).
 * Fin mientras.
 * Salir: 
 *   Mostrar en el tablero las minas que quedan por marcar 
 *     llamando a la función showMinesP1_C.
 *   Mostrar el carácter de la posición indicada por (indexMat) de la
 *     matriz (mMarks) en la posición del cursor en pantalla dentro 
 *     del tablero en función del índice de la matriz (indexMat) y 
 *     dejar el cursor en la posición donde hemos mostrado el carácter  
 *     llamando a la funció showCharBoarP1_C.
 *   Mostrar el mensaje de salida que corresponda llamando a la función
 *   printMessageP1_C.
 * Se acaba el juego.
 * 
 * Variables globales utilizadas:	
 * (state)    : Estado del juego.
 * (marksIni) : Matriz con las minas marcadas inicialmente.
 * (marks)    : Matriz con las minas marcadas.
 * (mines)    : Matriz donde ponemos las minas.
 * (indexMat) : Índice para acceder a las matrices mines y marks desde ensamblador.
 * (numMines) : Minas que quedan por marcar.
 * (charac)   : Carácter leído desde el teclado y a mostrar.
 * 
 * Esta función no es llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'playP1' 
 */
void playP1_C(){
	
   state = '1';               //Estado para empezar a jugar.
   
   printBoardP1_C();
   
   for(int i=0; i<DIMMATRIX; i++){      //Copiar marcas iniciales
	 for(int j=0; j<DIMMATRIX; j++){
	   marks[i][j]=marksIni[i][j];
	 }
   }
   countMinesP1_C(); 	      //Contar minas que quedan por marcar.
   indexMat = 99;
   showCharBoardP1_C();       //Mina marcada inicialmente en marks.
   numMines--;

   indexMat = 54;	     
   
   while (state == '1') {     //Bucle principal.  
	 showMinesP1_C();
	 showCharBoardP1_C();
     
     getchP1_C();             //Leer una tecla.
	
     if (charac>='i' && charac<='l') {  //Mover cursor
       moveCursorP1_C();
     }
     if (charac=='m') {       //Marcar mina.
       mineMarkerP1_C();
     }
     if (charac==27) {        //Salir del programa.
       state = '0';
     }
     
     checkEndP1_C();
   }
   showMinesP1_C();
   showCharBoardP1_C();          
   printMessageP1_C();	 //Mostrar mensaje para indicar cómo acaba.
   
}


/**
 * Programa Principal
 * 
 * ATENCIÓN: En cada opción se llama a una subrutina de ensamblador.
 * Debajo hay comentada la función en C equivalente que os damos hecha 
 * por si queréis ver como funciona.
 *  */
int main(void){   

   for(int i=0; i<DIMMATRIX; i++){      //Copiar marcas iniciales.
	 for(int j=0; j<DIMMATRIX; j++){
	   marks[i][j]=marksIni[i][j];
	 }
   }
   
   int op=-1;
   while (op!='0') {
	  clearScreen_C();
      printMenuP1_C();		  //Mostrar menú y leer opción.
      op = charac;
      switch(op){
        case '1':  //Contar las minas de la matriz mines.
          printf(" %c",op);
          clearScreen_C();
          printBoardP1_C();
          //=======================================================
          countMinesP1();
          //countMinesP1_C();
          //=======================================================
          showMinesP1_C();
          rowScreen = 29;
          colScreen = 20;
          gotoxyP1_C();
          printf(" Press any key ");
          getchP1_C();
        break;
        case '2':  //Mostrar el número de minas por marcar.
          printf(" %c",op);
          clearScreen_C();  
          printBoardP1_C();   
          countMinesP1_C();
          //=======================================================
          showMinesP1();
          //showMinesP1_C();
          //=======================================================
          rowScreen = 29;
          colScreen = 20;
          gotoxyP1_C();
          printf(" Press any key ");
          getchP1_C();
        break;
        case '3': //Mostrar una posición de la matriz marks y posiciona el cursor.
          printf(" %c",op);
          clearScreen_C();    
          printBoardP1_C();
          indexMat = 99;
          //=======================================================
          showCharBoardP1();
          //showCharBoardP1_C();  
          //=======================================================
          rowScreen = 29;
          colScreen = 20;
          gotoxyP1_C();
          printf("Press any key ");
          getchP1_C();
        break;
        case '4':  //Mover el cursor.
          clearScreen_C(); 
          printBoardP1_C();
          rowScreen = 29;
          colScreen = 3;
          gotoxyP1_C();
          printf("Move cursor: i:Up, j:Left, k:Down, l:Right");
          indexMat = 22;    
          showCharBoardP1_C();
          getchP1_C();	
	      if (charac >= 'i' && charac <= 'l') { 
		    //===================================================
		    moveCursorP1();
		    //moveCursorP1_C();
		   //===================================================
		    rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
		    printf("              Press any key               ");
		    showCharBoardP1_C();
          } else {
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("             Incorrect option             ");
		  }
          getchP1_C();
        break;
        case '5': //Marcar Mina.
          clearScreen_C();  
          printBoardP1_C();
          showMinesP1_C();
          rowScreen = 29;
          colScreen = 3;
          gotoxyP1_C();
          printf("        Mark a Mine: m:mark/unmark        ");
          indexMat=33;      
          showCharBoardP1_C();
          getchP1_C();
   		  if (charac=='m') {
			//===================================================
            mineMarkerP1();
			//mineMarkerP1_C();
			//===================================================
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
		    printf("              Press any key               ");
		    showMinesP1_C();
		    showCharBoardP1_C();
		    getchP1_C();
		  } else {
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("             Incorrect option             ");
			getchP1_C();
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("                                          ");
          }
        break;
        case '6': //Contar cuantas minas hay alrededor de una posición.
          rowScreen = 24;
          colScreen = 3;
          gotoxyP1_C();
		  printf("        Incorrect option         ");
		  getchP1_C();
		  rowScreen = 24;
          colScreen = 3;
          gotoxyP1_C();
		  printf("                                 ");
		  
		break;
        case '7': //Verificar si hemos marcado todas las minas.
          clearScreen_C(); 
          printBoardP1_C();
          showMinesP1_C();
 		  //===================================================
          checkEndP1();
		  //checkEndP1_C();
		  //===================================================
		  if (state=='1') {
			rowScreen = 29;
            colScreen = 3;
            gotoxyP1_C();
			printf("              Not Finished!!              ");
			getchP1_C();
		  } else {
			printMessageP1_C(state);
		  }
        break;
        case '8': //Juego completo en ensamblador.
          clearScreen_C();
          //=======================================================
          playP1();
          //=======================================================
        break;
        case '9': //Juego completo en C.
          clearScreen_C();
          //=======================================================
          playP1_C();
          //=======================================================
        break;
        case '0':
		break;	 
      }
   }
   printf("\n\n");
   
   return 0;
}
