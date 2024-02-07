#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INF 100000000
#define MAX 110


/* ============================  GRUPO 19  ============================ */
/*                     MARCONI VEJAR, Axel Gonzalo.                     */
/* ==================================================================== */

/* ==================================================================== */
/*                            TIPO VENDEDOR                             */
/* ==================================================================== */


typedef struct {
    int doc;
    char nom[51];
    char telefono[16];
    float montoVendido;
    int cantidadVendida;
    char canalDeVenta[21];
} vendedor;


/* ==================================================================== */
/*                        FUNCIONES AUXILIARES                          */
/* ==================================================================== */

typedef struct {     // Estructura auxiliar para almacenar costos.
    float costoLSOalta;
    float costoLSObaja;
    float costoLSOevocar;
    float costoLSOevocarFracaso;
    float costoLSOBBalta;
    float costoLSOBBbaja;
    float costoLSOBBevocar;
    float costoLSOBBevocarFracaso;
    float costoABBalta;
    float costoABBbaja;
    float costoABBevocar;
    float costoABBevocarFracaso;
    float costoRALalta;
    float costoRALbaja;
    float costoRALevocar;
    float costoRALevocarFracaso;
    float costoRSalta;
    float costoRSbaja;
    float costoRSevocar;
    float costoRSevocarFracaso;
} costos;

int contVend;
int contAux;

void show (vendedor y) {
    printf("\tDocumento: %d\n",y.doc);
    printf("\tNombre: %s\n",y.nom);
    printf("\tTelefono: %s\n",y.telefono);
    printf("\tMonto vendido: $%0.2f\n",y.montoVendido);
    printf("\tCantidad vendida: %d\n",y.cantidadVendida);
    printf("\tCanal de venta: %s\n",y.canalDeVenta);
}

void arreglarString (char str[]) {
    int tam = strlen(str);
    str[tam-1]='\0';
}

int hashing(int dni, int M) {
    char x[8];
    int longitud, i;
    int contador = 0;
    sprintf (x,"%d",dni);
    longitud = strlen(x);
    for (i = 0 ; i < longitud ; i++)
        contador += ((int)x[i]) * (i+1);
    return (contador%M);
}

/* ==================================================================== */
/*                 LISTA SECUENCIAL ORDENADA CON +INF                   */
/* ==================================================================== */


/* -------------------      DEFINICION     -------------------*/

vendedor LSO[MAX+1]; //1 espacio extra para el +INF
int cantLSO = 0;
int cantLSOAltas = 0;
int cantLSOBajas = 0;
int cantLSOEvoc = 0;
int cantLSOEvocF = 0;
float sumaCostosLSOaltas = 0;
float sumaCostosLSObajas = 0;
float sumaCostosLSOevoc = 0;
float sumaCostosLSOevocF = 0;

/* -------------------      LOCALIZAR      -------------------*/

int localizarBS (int x, int *pos, costos *c_max, costos *c_med){
    int i=0;
    while(x > LSO[i].doc){  // Mientras esté dentro de la estructura.
        i++;
        sumaCostosLSOevoc++;
    }
    *pos = i;
    sumaCostosLSOevoc++;    // Consulta por éxito.
    if (x == LSO[i].doc)
        return 1;    // Localizado exitosamente.
    else
        return 2;   // No localizado.
}

/* -------------------         ALTA        -------------------*/

int altaBS(int x, vendedor y, costos *c_max, costos *c_med){
    strupr(y.nom);
    strupr(y.canalDeVenta);
    int i,pos,exito;
    if (strcmp(y.nom,"") == 0)
        return 2; // Nupla con campos vacíos.
    if (cantLSO>=MAX)
        return -1;  // No hay espacio.
    exito = localizarBS(x,&pos,c_max,c_med);
    if (exito == 1)
        return 0;   // Documento ya registrado.
    sumaCostosLSOaltas = 0;
    for (i = cantLSO ; i >= pos ; i--){
        LSO[i+1] = LSO[i];
        sumaCostosLSOaltas += 1.5;
    }
    LSO[pos] = y;
    cantLSO++;
    return 1;   // Alta exitosa.
}

/* -------------------         BAJA        -------------------*/

int bajaBS(int x, vendedor y, costos *c_max, costos *c_med){
    strupr(y.nom);
    strupr(y.canalDeVenta);
    int i, pos, exito;
    exito = localizarBS(x,&pos,c_max,c_med);
    if (exito == 1){    // Se encontró el X.
        if (strcmp(LSO[pos].nom,y.nom) == 0 &&
                LSO[pos].doc == y.doc &&
                strcmp(LSO[pos].telefono , y.telefono) == 0 &&
                LSO[pos].montoVendido == y.montoVendido &&
                LSO[pos].cantidadVendida == y.cantidadVendida &&
                strcmp(LSO[pos].canalDeVenta , y.canalDeVenta) == 0){
            sumaCostosLSObajas = 0;
            for (i = pos ; i < cantLSO ; i++){
                LSO[i] = LSO[i+1];
                sumaCostosLSObajas += 1.5;
            }
            cantLSO--;
            return 1;   // Eliminación exitosa.
        }
        return -1;   // Eliminación cancelada (No es la misma nupla).
    } else
        return 0;  // No se encontró el elemento.
}

/* -------------------        EVOCAR       -------------------*/

int evocacionBS(int x, vendedor *y, costos *c_max, costos *c_med){
    int pos,exito;
    sumaCostosLSOevoc = 0;
    exito = localizarBS(x,&pos,c_max,c_med);
    if (exito==1){
        *y = LSO[pos];
        cantLSOEvoc++;
        return 1;     // Evocación exitosa.
    }
    else {
        *y = LSO[pos];
        cantLSOEvocF++;
        return 0;  // Fracaso.
    }
}

/* -------------------       MOSTRAR      -------------------*/

void mostrarLSO (void){
    int i;
    int cont = 0;
    printf("\n");
    printf("\n");
    for (i = 0 ; i < cantLSO ; i++)
    {
        fflush(stdin);
        printf("\n\tVendedor %d\n",i+1);
        printf("\t--------------\n");
        show(LSO[i]);
        printf("\t--------------\n\n");
        cont++;
        if (cont == 5){
            cont = 0;
            printf("Presione Enter para seguir mostrando.\n");
            getchar();
        }
    }
    printf("\t--------------\n");
    printf("\tFIN DE LA LISTA\n");
    printf("\t--------------\n\n");
}


/* ==================================================================== */
/*                LISTA SECUENCIAL ORDENADA - BISECCIÓN                 */
/* ==================================================================== */


vendedor LSOBB[MAX];
int cantLSOBB = 0;
int cantLSOBBAltas = 0;
int cantLSOBBEvoc = 0;
int cantLSOBBEvocF = 0;
float sumaCostosLSOBBaltas = 0;
float sumaCostosLSOBBbajas = 0;
float sumaCostosLSOBBevoc = 0;
float sumaCostosLSOBBevocF = 0;
int cantLSOBBBajas = 0;

/* -------------------      LOCALIZAR      -------------------*/

int localizarBB(int x, int *pos, costos *c_max, costos *c_med){   // BISECCIÓN.
    // Limite inferior inclusivo.
    // Limite superior inclusivo.
    // Testigo y segmento mas grande a la izquierda.
    if (cantLSOBB==0){
        *pos = 0;
        return 2;
    }
    int marcas[cantLSOBB];
    int i;
    for (i=0 ; i<cantLSOBB ; i++)
        marcas[i] = 0;
    int li,ls,t;
    li = 0;
    ls = cantLSOBB-1;
    t = floor((li+ls)/2.0);
    while (li < ls){
        if (x > LSOBB[t].doc)
            li = t+1;
        else
            ls = t;
        if (marcas[t] == 0){
            sumaCostosLSOBBevoc++;
            marcas[t] = 1;
        }
        t = floor((li+ls)/2.0);
    }
    if (marcas[t] == 0){
        sumaCostosLSOBBevoc++;  // Consulta por éxito.
        marcas[t] = 1;
    }
    if (x == LSOBB[li].doc){ //Xb == x
        *pos = li;
        return 1;
    }
    if (x < LSOBB[li].doc)
        *pos = li;  // x menor -> pos actual.
    else
        *pos = ls+1;// x mayor -> pos siguiente.
    return 2;
}

/* -------------------         ALTA        -------------------*/

int altaBB(int x, vendedor y, costos *c_max, costos *c_med){
    strupr(y.nom);
    strupr(y.canalDeVenta);
    int i,pos,exito;
    if (strcmp(y.nom,"") == 0)
        return 2; // Nupla con campos vacíos.
    if (cantLSOBB>=MAX)
        return -1;  // No hay espacio.
    exito = localizarBB(x,&pos,c_max,c_med);
    if (exito == 1)
        return 0;   // Documento ya registrado.
    sumaCostosLSOBBaltas = 0;
    for (i = cantLSOBB-1 ; i >= pos ; i--){
        LSOBB[i+1] = LSOBB[i];
        sumaCostosLSOBBaltas += 1.5;
    }
    LSOBB[pos] = y;
    cantLSOBB++;
    return 1;
}

/* -------------------         BAJA        -------------------*/

int bajaBB(int x, vendedor y, costos *c_max, costos *c_med){
    int i, pos, exito;
    exito = localizarBB(x,&pos,c_max,c_med);
    strupr(y.nom);
    strupr(y.canalDeVenta);
    if (exito == 1){    // Se encontró el X.
        if (strcmp(LSOBB[pos].nom , y.nom) == 0 &&
                LSOBB[pos].doc == y.doc &&
                strcmp(LSOBB[pos].telefono , y.telefono) == 0 &&
                LSOBB[pos].montoVendido == y.montoVendido &&
                LSOBB[pos].cantidadVendida == y.cantidadVendida &&
                strcmp(LSOBB[pos].canalDeVenta , y.canalDeVenta) == 0){
            sumaCostosLSOBBbajas = 0;
            for (i = pos ; i < cantLSOBB-1 ; i++){
                LSOBB[i] = LSOBB[i+1];
                sumaCostosLSOBBbajas += 1.5;
            }
            cantLSOBB--;
            return 1;   // Eliminación exitosa.
        }
        return -1;   // Eliminación cancelada (No es la misma nupla).
    } else
        return 0;  // No se encontró el elemento.
}

/* -------------------        EVOCAR       -------------------*/

int evocacionBB(int x, vendedor *y, costos *c_max, costos *c_med){
    int pos,exito;
    sumaCostosLSOBBevoc = 0;
    exito = localizarBB(x,&pos,c_max,c_med);
    if (exito==1){
        *y = LSOBB[pos];
        cantLSOBBEvoc++;
        return 1;     // Evocación exitosa.
    }
    else {
        *y = LSOBB[pos];
        cantLSOBBEvocF++;
        return 0;  // Fracaso.
    }
}

/* -------------------       MOSTRAR      -------------------*/

void mostrarLSOBB (void){
    int i;
    int cont = 0;
    printf("\n");
    printf("\n");
    for (i = 0 ; i < cantLSOBB ; i++)
    {
        fflush(stdin);
        printf("\n\tVendedor %d\n",i+1);
        printf("\t--------------\n");
        show(LSOBB[i]);
        printf("\t--------------\n\n");
        cont++;
        if (cont == 5){
            cont = 0;
            printf("\n\tPresione Enter para seguir mostrando.\n\t");
            getchar();
        }
    }
    printf("\t--------------\n");
    printf("\tFIN DE LA LISTA\n");
    printf("\t--------------\n\n");
}


/* ==================================================================== */
/*                      ARBOL BINARIO DE BÚSQUEDA                       */
/* ==================================================================== */


/* -------------------      DEFINICION     -------------------*/

typedef struct nodo {
   vendedor vend;
   struct nodo *hi;
   struct nodo *hd;
} Nodet;

Nodet *arbol = NULL;
int cantABB = 0;
int cantABBAltas = 0;
int cantABBBajas = 0;
int cantABBEvoc = 0;
int cantABBEvocF = 0;
float sumaCostosABBaltas = 0;
float sumaCostosABBbajas = 0;
float sumaCostosABBevoc = 0;
float sumaCostosABBevocF = 0;


/* -------------------      LOCALIZAR      -------------------*/

Nodet* localizarABB (int x, int *mayorMenor, int *exito, Nodet** ant, costos *c_max, costos *c_med) {
    // mayorMenor guarda información sobre a qué subárbol se bajó
    // desde el anterior nodo (ant) a la pos en la que termina el proceso.
    // (Me sirve en el caso del fracaso).
    if (arbol == NULL){
        *exito = -1;
        return arbol;  // Arbol vacío.
    }
    Nodet *p = arbol;
    *ant = NULL;
    while (p != NULL && p->vend.doc != x){
        sumaCostosABBevoc++;
        *ant = p;
        if (p->vend.doc < x){
            *mayorMenor = 1;
            p = p->hd;
        }
        else {
            *mayorMenor = -1;
            p = p->hi;
        }
        sumaCostosABBevoc++;
    }
    sumaCostosABBevoc++; // Costo de consultar solo la raíz (No ejecuta el while)
    if (p == NULL)
        *exito = 0; // No Localizado.
    else
        *exito = 1; // localizado.
    return p;
}

/* -------------------         ALTA        -------------------*/

int altaABB (int x, vendedor y, costos *c_max, costos *c_med) {
    int exito ,mayorMenor;
    Nodet *pos = NULL;
    Nodet *ant = NULL;
    strupr(y.nom);
    strupr(y.canalDeVenta);
    pos = localizarABB(x,&mayorMenor,&exito,&ant,c_max,c_med);
    if (strcmp(y.nom,"") == 0)
        return 2; // La nupla tiene campos vacíos.
    if (exito==1)
        return 0; // Nupla repetida.
    Nodet *nuevo = (Nodet*)malloc(sizeof(Nodet));
    if (nuevo == NULL)
        return -1;  // No hay espacio.
    sumaCostosABBaltas = 0;
    nuevo->vend = y; // Prepara el nuevo nodo.
    nuevo->hi = NULL;
    sumaCostosABBaltas += 1;
    nuevo->hd = NULL;
    sumaCostosABBaltas += 1;
    if (exito == -1){ // Arbol vacío.
        arbol = nuevo;
        sumaCostosABBaltas += 1;
        cantABB++;
        return 1;
    }
    // Arbol no vacío.
    if (mayorMenor>0)
        ant->hd = nuevo;
    else
        ant->hi = nuevo;
    cantABB++;
    sumaCostosABBaltas += 1;
    return 1;
}

/* -------------------         BAJA        -------------------*/

int bajaABB (int x, vendedor y, costos *c_max, costos *c_med) {
    int exito, mayorMenor;
    Nodet* pos = NULL;
    Nodet *ant = NULL;
    strupr(y.nom);
    strupr(y.canalDeVenta);
    pos = localizarABB(x,&mayorMenor,&exito,&ant,c_max,c_med);
    if (exito != 1)
        return 0;   // No localizado.
    if (strcmp(pos->vend.nom , y.nom) == 0 &&
                pos->vend.doc == y.doc &&
                strcmp(pos->vend.telefono , y.telefono) == 0 &&
                pos->vend.montoVendido == y.montoVendido &&
                pos->vend.cantidadVendida == y.cantidadVendida &&
                strcmp(pos->vend.canalDeVenta , y.canalDeVenta) == 0){   // Es la misma nupla.
        Nodet *mayorDeMenores;
        int comparacion;
        sumaCostosABBbajas = 0;
        if (ant == NULL)
            comparacion = 0;    // Es la raíz.
        else if (ant->vend.doc > pos->vend.doc)
                comparacion = -1;   // Es hijo izq del padre.
            else
                comparacion = 1;    // Es hijo der del padre.
        if (pos->hi == NULL && pos->hd == NULL){ // Nodo hoja.
            if (comparacion != 0){  // Si no es la raíz...
                if (comparacion<0)
                    ant->hi = NULL;
                else
                    ant->hd = NULL;
            }
            else
                arbol = NULL;
            sumaCostosABBbajas += 1;
            free(pos);
            cantABB--;
            return 1;   // Exito.
        }
        if (pos->hi != NULL && pos->hd == NULL){ // Nodo con hijo izquierdo.
            if (comparacion != 0){  // Si no es la raíz.
                if (comparacion<0)
                    ant->hi = pos->hi;
                else
                    ant->hd = pos->hi;
            }
            else // Es la raíz.
                arbol = pos->hi;
            sumaCostosABBbajas += 1;
            free(pos);
            cantABB--;
            return 1;
        }
        if (pos->hi == NULL && pos->hd != NULL){ // Nodo con hijo derecho.
            if (comparacion != 0){  // Si no es la raíz.
                if (comparacion<0)
                    ant->hi = pos->hd;
                else
                    ant->hd = pos->hd;
            }
            else // Es la raíz.
                arbol = pos->hd;
            sumaCostosABBbajas += 1;
            free(pos);
            cantABB--;
            return 1;
        }
        if (pos->hi != NULL && pos->hd != NULL){ // Nodo con dos hijos - (Política de reemplazo: Mayor de los menores).
            Nodet *antMayMenores = pos;
            mayorDeMenores = pos->hi; // Menor...
            while (mayorDeMenores->hd != NULL){
                antMayMenores = mayorDeMenores; // Guardo el anterior.
                mayorDeMenores = mayorDeMenores->hd; // Mayor de los menores.
            }
            pos->vend = mayorDeMenores->vend;   // Reemplazo la nupla (Ahora hay 2 nuplas iguales).
            sumaCostosABBbajas += 1.5;  // Costo por la copia de datos.
            if (mayorDeMenores->hi == NULL){ // Si el reemplazante no tiene hijo.
                if (pos->hi->vend.doc != mayorDeMenores->vend.doc) // Si el reemplazo no es hijo izq del nodo a eliminar...
                     antMayMenores->hd = NULL;
                else //El reemplazo es hijo izq del nodo a eliminar (pos).
                     pos->hi = NULL;
            }
            else {  // El reemplazante tiene hijo.
                if (pos->hi->vend.doc != mayorDeMenores->vend.doc) //Si el reemplazo no es hijo izq del nodo a eliminar...
                    antMayMenores->hd = mayorDeMenores->hi;
                else
                    pos->hi = mayorDeMenores->hi;
            }
            sumaCostosABBbajas += 1;
            free(mayorDeMenores);
            cantABB--;
            return 1;
        }
    }
    return -1;
}

/* -------------------        EVOCAR       -------------------*/

int evocacionABB(int x, vendedor *y, costos *c_max, costos *c_med) {
    int exito,mayorMenor;
    Nodet *ant = NULL;
    sumaCostosABBevoc = 0;
    Nodet *pos = localizarABB(x,&mayorMenor,&exito,&ant,c_max,c_med);
    if (exito == 1){    // Localizado.
        *y = pos->vend;
        cantABBEvoc++;
        return 1;
    }
    else {    // No localizado.
        cantABBEvocF++;
        return 0;
    }
}

/* -------------------       MOSTRAR       -------------------*/

void mostrarABB (Nodet *tree) {  // Barrido pre-orden.
    if (tree==NULL)
        return;
    fflush(stdin);
    printf("\n\tVendedor %d\n",contVend+1);
    printf("\t--------------\n");
    show(tree->vend);
    printf("\t--------------\n");
    contVend++;
    if (tree->hi == NULL)
        printf("\tHijo izquierdo: NO TIENE.\n");
    else
        printf("\tHijo izquierdo: %d\n",tree->hi->vend.doc);
    if (tree->hd == NULL)
        printf("\tHijo derecho: NO TIENE.\n");
    else
        printf("\tHijo derecho: %d\n",tree->hd->vend.doc);
    printf("\t--------------\n\n");
    contAux++;
    if (contAux == 5){
        contAux = 0;
        printf("\n\tPresione Enter para seguir mostrando.\n\t");
        getchar();
    }   // <-- Raíz
    mostrarABB(tree->hi);   // Hijo izquierdo.
    mostrarABB(tree->hd);   // Hijo derecho.
}


/* ==================================================================== */
/*                     REBALSE ABIERTO CUADRÁTICO                       */
/* ==================================================================== */


/* -------------------      DEFINICION     -------------------*/

// p = 0.77 <-> N = 110 y M = 142 ---> M tiene que ser primo. M = 149.

int M_RAL = 149;
vendedor RAL[149];   // Estructura.
int cantRAL = 0;
int cantRALAltas = 0;
int cantRALBajas = 0;
int cantRALEvoc = 0;
int cantRALEvocF = 0;
float sumaCostosRALaltas = 0;
float sumaCostosRALbajas = 0;
float sumaCostosRALevoc = 0;
float sumaCostosRALevocF = 0;

void inicializarRAL() {
    int i;
    for (i=0 ; i<M_RAL ; i++)
        RAL[i].doc = -1;    // Carga las M celdas con marca de vírgen (-1).
}

/* -------------------      LOCALIZAR      -------------------*/

int localizarRAL(int x, int *pos, costos *c_max, costos *c_med) {// -1 -> Celda virgen , -2 -> Celda libre.
    int i,flag;
    int condFlag = 0;
    int contador = 0;
    i = hashing(x,M_RAL);
    while ((contador < M_RAL) && (RAL[i].doc != -1) && (RAL[i].doc != x)){  // Mientras no mire M baldes, no esté en una celda virgen y no sea el x que busco.
        sumaCostosRALevoc++;
        if (condFlag != 1){
            if (RAL[i].doc == -2){
                flag = i;
                condFlag = 1;   // Si ya guardó la primera libre, la condición del if no se cumple más.
            }
        }
        i = (i+1)%M_RAL;  // Avance.
        contador++;
    }
    sumaCostosRALevoc++;    // Consulta por éxito.
    if (RAL[i].doc == x){   // Xb == X.
        *pos = i;
        return 1;   // Localizado.
    }
    if (contador < M_RAL){  // Si NO recorrió M baldes...
        if (condFlag == 1)   // Si pasó por una celda libre...
            *pos = flag;    // pos = primera celda libre.
        else
            *pos = i;   // pos = celda virgen en la que fracasó.
    }
    else {  // Si recorrió M baldes...  (No paró en una celda virgen)
        if (condFlag == 1)  // Si pasó por una libre...
            *pos = flag;    // pos = primera celda libre.
        else
            return -1;  // No localizado. (No hay espacio para la lista que empieza en h(x)).
    }
    return 0;   // No localizado. (Hay espacio para la lista que empieza en h(x)).
}

/* -------------------         ALTA        -------------------*/

int altaRAL(int x, vendedor y, costos *c_max, costos *c_med) {
    int pos,ex;
    if (cantRAL >= M_RAL)
        return -1;   // No hay espacio.
    if (strcmp(y.nom,"") == 0)
        return 2;   // La nupla tiene campos vacíos.
    strupr(y.nom);
    strupr(y.canalDeVenta);
    ex = localizarRAL(x,&pos,c_max,c_med);
    if (ex == 1)
        return 0;   // Documento repetido.
    if (ex == -1)
        return -2;  // No hay espacio para la lista que empieza en h(x).
    // Alta.
    RAL[pos] = y;
    sumaCostosRALaltas = 0;
    cantRAL++;
    return 1; // Alta exitosa.
}

/* -------------------         BAJA        -------------------*/

int bajaRAL(int x, vendedor y, costos *c_max, costos *c_med) {
    int pos,ex;
    strupr(y.nom);
    strupr(y.canalDeVenta);
    ex = localizarRAL(x,&pos,c_max,c_med);
    if (ex != 1)
        return 0;   // No se encontró el elemento.
    if (strcmp(RAL[pos].nom , y.nom) == 0 &&
                RAL[pos].doc == y.doc &&
                strcmp(RAL[pos].telefono , y.telefono) == 0 &&
                RAL[pos].montoVendido == y.montoVendido &&
                RAL[pos].cantidadVendida == y.cantidadVendida &&
                strcmp(RAL[pos].canalDeVenta , y.canalDeVenta) == 0){   // Es la misma nupla.
        RAL[pos].doc = -2; // Celda libre.
        cantRAL--;
        sumaCostosRALbajas = 0;
        return 1;   // Eliminación exitosa.
    }
    return -1;
}

/* -------------------        EVOCAR       -------------------*/

int evocacionRAL(int x, vendedor *y, costos *c_max, costos *c_med) {
    int pos,ex;
    sumaCostosRALevoc = 0;
    ex = localizarRAL(x,&pos,c_max,c_med);
    if (ex == 1){
        *y = RAL[pos];
        cantRALEvoc++;
        return 1; // Encontrado.
    }
    else {
        cantRALEvocF++;
        return 0; // No encontrado.
    }
}

/* -------------------       MOSTRAR       -------------------*/

void mostrarRAL() {
    int i;
    int cont = 0;
    for (i=0 ; i<M_RAL ; i++){
        fflush(stdin);
        printf("\n\tCELDA %d\n",i);
        if (RAL[i].doc == -1){
            printf("\t--------------\n");
            printf("\tVIRGEN(-1)\n");
            printf("\t--------------\n\n");
        }
        else
            if (RAL[i].doc == -2){
                printf("\t--------------\n");
                printf("\tLIBRE(-2)\n");
                printf("\t--------------\n\n");
            }
            else {
                printf("\t--------------\n");
                show(RAL[i]);
                printf("\t--------------\n\n");
            }
        cont++;
        if (cont == 5){
            cont = 0;
            printf("\n\tPresione Enter para seguir mostrando.\n\t");
            getchar();
        }
    }
    printf("\n");
    printf("\t--------------\n");
    printf("\tFIN DE LA LISTA\n");
    printf("\t--------------\n\n");
}


/* ==================================================================== */
/*                          REBALSE SEPARADO                            */
/* ==================================================================== */


/* -------------------      DEFINICION     -------------------*/

// p = 1.84 <-> N = 110 y M = 59.58 ---> M = 60.

int M_RS = 60;
int cantRSAltas = 0;
int cantRSBajas = 0;
int cantRSEvoc = 0;
int cantRSEvocF = 0;
float sumaCostosRSaltas = 0;
float sumaCostosRSbajas = 0;
float sumaCostosRSevoc = 0;
float sumaCostosRSevocF = 0;

typedef struct nodo_t{
    vendedor dato;
    struct nodo_t *sig;
} nodo;

typedef struct{
    nodo *Acc;
} lista;

lista RS[60];   // Estructura.
int cantRS = 0;

void inicializarRS() {
    int i;
    for (i=0 ; i<M_RS ; i++)
        RS[i].Acc = NULL;   // Carga los M accesos a listas con NULL.
}

/* -------------------      LOCALIZAR      -------------------*/

int localizarRS(int x, int *indiceAcc, nodo **pos, costos *c_max, costos *c_med){
    int i = hashing(x,M_RS);    // Acceso a lista.
    nodo *p,*ant;
    p = RS[i].Acc;
    ant = NULL;    // *** Cuando no entra al while, ant es NULL.
    sumaCostosRSevoc++; // Costo de consultar la primera celda de la lista (No ejecuta el while)
    while (p != NULL && p->dato.doc != x){
        ant = p;
        p = p->sig;
        sumaCostosRSevoc++;
    }
    *indiceAcc = i;
    *pos = ant;    // Devuelvo el nodo anterior.
    if (p != NULL)
        return 1;   // Localizado.
    else
        return 0;   // No localizado.
}

/* -------------------         ALTA        -------------------*/

int altaRS(int x, vendedor y, costos *c_max, costos *c_med){
    int acc,ex;
    nodo *ant = NULL;
    if (strcmp(y.nom,"") == 0) // Si la nupla no tiene campos vacíos.
        return 2;
    ex = localizarRS(x,&acc,&ant,c_max,c_med);
    if (ex == 1)
        return 0;   // X repetido. Fracasa.
    nodo *aux = (nodo*)malloc(sizeof(nodo));
    if (aux == NULL)
        return -1;  // Estructura llena.
    strupr(y.nom);
    strupr(y.canalDeVenta);
    sumaCostosRSaltas = 0;
    aux->dato = y;
    aux->sig = RS[acc].Acc;
    sumaCostosRSaltas += 1;
    RS[acc].Acc = aux;  // Alta al principio de la lista vinculada.
    sumaCostosRSaltas += 1;
    cantRS++;
    return 1;   // Alta exitosa.
}

/* -------------------         BAJA        -------------------*/

int bajaRS(int x, vendedor y, costos *c_max, costos *c_med){
    int acc,ex,primerElem;
    int auxiliar = 0;
    nodo *ant = NULL;
    nodo *aux;
    strupr(y.nom);
    strupr(y.canalDeVenta);
    ex = localizarRS(x,&acc,&ant,c_max,c_med);
    if (ex != 1)
        return 0;  // No Localizado.
    if (ant == NULL)    // ant == NULL cuando se localiza el primer elemento de la lista.
        primerElem = 1;
    else
        primerElem = 0;
    sumaCostosRSbajas = 0;
    if (primerElem == 0){
        if (auxiliar != 1 && strcmp(ant->sig->dato.nom , y.nom) == 0 &&
                ant->sig->dato.doc == y.doc &&
                strcmp(ant->sig->dato.telefono , y.telefono) == 0 &&
                ant->sig->dato.montoVendido == y.montoVendido &&
                ant->sig->dato.cantidadVendida == y.cantidadVendida &&
                strcmp(ant->sig->dato.canalDeVenta , y.canalDeVenta) == 0){  // Es la misma nupla.
            if (ant->sig->sig == NULL){  // Eliminar el último elemento de la lista.
                ant->sig = NULL;    // Anteúltimo->sig = NULL;
                sumaCostosRSbajas += 1;
                free(ant->sig);     // Libero el último.
                cantRS--;
                return 1;   // Eliminación exitosa.
            }
            // El elemento a eliminar no es un extremo de la lista.
            aux = ant->sig->sig;    // Guardo el sig del nodo a eliminar.
            free(ant->sig);     // Libero el nodo a eliminar.
            ant->sig = aux;     // anterior->sig = anterior->sig->sig.
            sumaCostosRSbajas += 1;
            cantRS--;
            return 1;   // Eliminación exitosa.
        }
    }
    if (primerElem == 1){  // Eliminar el primer elemento de la lista.
        if (strcmp(RS[acc].Acc->dato.nom , y.nom) == 0 &&
            RS[acc].Acc->dato.doc == y.doc &&
            strcmp(RS[acc].Acc->dato.telefono , y.telefono) == 0 &&
            RS[acc].Acc->dato.montoVendido == y.montoVendido &&
            RS[acc].Acc->dato.cantidadVendida == y.cantidadVendida &&
            strcmp(RS[acc].Acc->dato.canalDeVenta , y.canalDeVenta) == 0){  // Es la misma nupla.
                aux = RS[acc].Acc->sig; // Guardo el sig del primero.
                free(RS[acc].Acc);  // Libero el primero.
                RS[acc].Acc = aux;  // Acceso = sig del eliminado.
                sumaCostosRSbajas += 1;
                cantRS--;
                return 1;   // Eliminación exitosa.
            }
    }
    return -1;
}

/* -------------------        EVOCAR       -------------------*/

int evocacionRS(int x, vendedor *y, costos *c_max, costos *c_med){
    int acc,ex;
    nodo *ant = NULL;
    sumaCostosRSevoc = 0;
    ex = localizarRS(x,&acc,&ant,c_max,c_med);
    if (ex == 0){
        if (ant==NULL)  // Salió por lista vacía.
            sumaCostosRSevoc = 0;
        cantRSEvocF++;
        return 0;   // No encontrado.
    }
    if (ant == NULL)   // Es el primer elemento de la lista.
        *y = RS[acc].Acc->dato;
    else    // No es el primer elemento de la lista.
        *y = ant->sig->dato;
    cantRSEvoc++;
    return 1;   // Encontrado.
}

/* -------------------       MOSTRAR       -------------------*/

void mostrarRS(){
    int i;
    int cont = 0;
    nodo *aux;
    for (i = 0 ; i < M_RS ; i++){
        fflush(stdin);
        printf("\n\tLISTA %d\n",i);
        if (RS[i].Acc == NULL){ // Lista vacía.
            printf("\t------\n");
            printf("\tVACIA\n");
            printf("\t------\n\n");
        }
        else {  // Lista no vacía. Muestro todos sus elementos.
            aux = RS[i].Acc;
            printf("\t--------------\n");
            while (aux != NULL){
                show(aux->dato);
                printf("\t--------------\n");
                aux = aux->sig;
            }
            printf("\n");
        }
        cont++;
        if (cont == 5){
            cont = 0;
            printf("\n\tPresione Enter para seguir mostrando.\n\t");
            getchar();
        }
    }
}

int lecturaOperaciones(costos *c_max, costos *c_medio) {
    vendedor auxiliar;
    char basura;
    int codigoOperador;
    float aux,aux2;
    FILE *archivo;
    if ((archivo = fopen("Operaciones.txt","r"))==NULL)
        return 0;
    while (!(feof(archivo))) {
        fscanf(archivo,"%d",&codigoOperador);
        if (codigoOperador==1 || codigoOperador==2){
            fscanf(archivo,"%d",&auxiliar.doc);
            fscanf(archivo,"%c",&basura);
            fgets(auxiliar.nom,52,archivo);
            arreglarString(auxiliar.nom);
            fgets(auxiliar.telefono,17,archivo);
            arreglarString(auxiliar.telefono);
            fscanf(archivo,"%f",&auxiliar.montoVendido);
            fscanf(archivo,"%d",&auxiliar.cantidadVendida);
            fscanf(archivo,"%c",&basura);
            fgets(auxiliar.canalDeVenta,102,archivo);
            arreglarString(auxiliar.canalDeVenta);
            if (codigoOperador==1){

                aux2 = sumaCostosLSOevoc;
                aux = sumaCostosLSOaltas;   // Guardo los costos acumulados.
                if (altaBS(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosLSOaltas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantLSOAltas++;
                    if (sumaCostosLSOaltas>c_max->costoLSOalta)
                        c_max->costoLSOalta = sumaCostosLSOaltas;
                    sumaCostosLSOaltas += aux;
                    c_medio->costoLSOalta = sumaCostosLSOaltas/cantLSOAltas;
                }
                sumaCostosLSOevoc = aux2;

                aux2 = sumaCostosLSOBBevoc;
                aux = sumaCostosLSOBBaltas;   // Guardo los costos acumulados.
                if (altaBB(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosLSOBBaltas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantLSOBBAltas++;
                    if (sumaCostosLSOBBaltas>c_max->costoLSOBBalta)
                        c_max->costoLSOBBalta = sumaCostosLSOBBaltas;
                    sumaCostosLSOBBaltas += aux;
                    c_medio->costoLSOBBalta = sumaCostosLSOBBaltas/cantLSOBBAltas;
                }
                sumaCostosLSOBBevoc = aux2;

                aux2 = sumaCostosABBevoc;
                aux = sumaCostosABBaltas;   // Guardo los costos acumulados.
                if (altaABB(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosABBaltas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantABBAltas++;
                    if (sumaCostosABBaltas>c_max->costoABBalta)
                        c_max->costoABBalta = sumaCostosABBaltas;
                    sumaCostosABBaltas += aux;
                    c_medio->costoABBalta = sumaCostosABBaltas/cantABBAltas;
                }
                sumaCostosABBevoc = aux2;

                aux2 = sumaCostosRALevoc;
                aux = sumaCostosRALaltas;   // Guardo los costos acumulados.
                if (altaRAL(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosRALaltas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantRALAltas++;
                    if (sumaCostosRALaltas>c_max->costoRALalta)
                        c_max->costoRALalta = sumaCostosRALaltas;
                    sumaCostosRALaltas += aux;
                    c_medio->costoRALalta = sumaCostosRALaltas/cantRALAltas;
                }
                sumaCostosRALevoc = aux2;

                aux2 = sumaCostosRSevoc;
                aux = sumaCostosRSaltas;   // Guardo los costos acumulados.
                if (altaRS(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosRSaltas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantRSAltas++;
                    if (sumaCostosRSaltas>c_max->costoRSalta)
                        c_max->costoRSalta = sumaCostosRSaltas;
                    sumaCostosRSaltas += aux;
                    c_medio->costoRSalta = sumaCostosRSaltas/cantRSAltas;
                }
                sumaCostosRSevoc = aux2;
            }
            if (codigoOperador==2){
                aux2 = sumaCostosLSOevoc;
                aux = sumaCostosLSObajas;   // Guardo los costos acumulados.
                if (bajaBS(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosLSObajas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantLSOBajas++;
                    if (sumaCostosLSObajas>c_max->costoLSObaja)
                        c_max->costoLSObaja = sumaCostosLSObajas;
                    sumaCostosLSObajas += aux;
                    c_medio->costoLSObaja = sumaCostosLSObajas/cantLSOBajas;
                }
                sumaCostosLSOevoc = aux2;

                aux2 = sumaCostosLSOBBevoc;
                aux = sumaCostosLSOBBbajas;   // Guardo los costos acumulados.
                if (bajaBB(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosLSOBBbajas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantLSOBBBajas++;
                    if (sumaCostosLSOBBbajas>c_max->costoLSOBBbaja)
                        c_max->costoLSOBBbaja = sumaCostosLSOBBbajas;
                    sumaCostosLSOBBbajas += aux;
                    c_medio->costoLSOBBbaja = sumaCostosLSOBBbajas/cantLSOBBBajas;
                }
                sumaCostosLSOBBevoc = aux2;

                aux2 = sumaCostosABBevoc;
                aux = sumaCostosABBbajas;   // Guardo los costos acumulados.
                if (bajaABB(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosABBbajas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantABBBajas++;
                    if (sumaCostosABBbajas>c_max->costoABBbaja)
                        c_max->costoABBbaja = sumaCostosABBbajas;
                    sumaCostosABBbajas += aux;
                    c_medio->costoABBbaja = sumaCostosABBbajas/cantABBBajas;
                }
                sumaCostosABBevoc = aux2;

                aux2 = sumaCostosRALevoc;
                aux = sumaCostosRALbajas;   // Guardo los costos acumulados.
                if (bajaRAL(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosRALbajas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantRALBajas++;
                    if (sumaCostosRALbajas>c_max->costoRALbaja)
                        c_max->costoRALbaja = sumaCostosRALbajas;
                    sumaCostosRALbajas += aux;
                    c_medio->costoRALbaja = sumaCostosRALbajas/cantRALBajas;
                }
                sumaCostosRALevoc = aux2;

                aux2 = sumaCostosRSevoc;
                aux = sumaCostosRSbajas;   // Guardo los costos acumulados.
                if (bajaRS(auxiliar.doc,auxiliar,c_max,c_medio)!=1) // Fracaso:
                    sumaCostosRSbajas = aux;   // No sumo los costos.
                else {  // Éxito:
                    cantRSBajas++;
                    if (sumaCostosRSbajas>c_max->costoRSbaja)
                        c_max->costoRSbaja = sumaCostosRSbajas;
                    sumaCostosRSbajas += aux;
                    c_medio->costoRSbaja = sumaCostosRSbajas/cantRSBajas;
                }
                sumaCostosRSevoc = aux2;
            }
        }
        else if (codigoOperador==3){
            fscanf(archivo,"%d",&auxiliar.doc);
            fscanf(archivo,"%c",&basura);
            aux = sumaCostosLSOevoc;   // Guardo los costos acumulados.
            if (evocacionBS(auxiliar.doc,&auxiliar,c_max,c_medio)!=1){ // Fracaso:
                if (sumaCostosLSOevoc>c_max->costoLSOevocarFracaso)
                    c_max->costoLSOevocarFracaso = sumaCostosLSOevoc;
                sumaCostosLSOevocF += sumaCostosLSOevoc;
                c_medio->costoLSOevocarFracaso = sumaCostosLSOevocF/cantLSOEvocF;
                sumaCostosLSOevoc = aux;
            }
            else {  // Éxito:
                if (sumaCostosLSOevoc>c_max->costoLSOevocar)
                    c_max->costoLSOevocar = sumaCostosLSOevoc;  // Máximo.
                sumaCostosLSOevoc += aux;   // Acumulo costos.
                c_medio->costoLSOevocar = sumaCostosLSOevoc/cantLSOEvoc;    // Medio.
            }

            aux = sumaCostosLSOBBevoc;   // Guardo los costos acumulados.
            if (evocacionBB(auxiliar.doc,&auxiliar,c_max,c_medio)!=1){ // Fracaso:
                if (sumaCostosLSOBBevoc>c_max->costoLSOBBevocarFracaso)
                    c_max->costoLSOBBevocarFracaso = sumaCostosLSOBBevoc;
                sumaCostosLSOBBevocF += sumaCostosLSOBBevoc;
                c_medio->costoLSOBBevocarFracaso = sumaCostosLSOBBevocF/cantLSOBBEvocF;
                sumaCostosLSOBBevoc = aux;
            }
            else {  // Éxito:
                if (sumaCostosLSOBBevoc>c_max->costoLSOBBevocar)
                    c_max->costoLSOBBevocar = sumaCostosLSOBBevoc;  // Máximo.
                sumaCostosLSOBBevoc += aux;   // Acumulo costos.
                c_medio->costoLSOBBevocar = sumaCostosLSOBBevoc/cantLSOBBEvoc;    // Medio.
            }

            aux = sumaCostosABBevoc;   // Guardo los costos acumulados.
            if (evocacionABB(auxiliar.doc,&auxiliar,c_max,c_medio)!=1){ // Fracaso:
                if (sumaCostosABBevoc>c_max->costoABBevocarFracaso)
                    c_max->costoABBevocarFracaso = sumaCostosABBevoc;
                sumaCostosABBevocF += sumaCostosABBevoc;
                c_medio->costoABBevocarFracaso = sumaCostosABBevocF/cantABBEvocF;
                sumaCostosABBevoc = aux;
            }
            else {  // Éxito:
                if (sumaCostosABBevoc>c_max->costoABBevocar)
                    c_max->costoABBevocar = sumaCostosABBevoc;  // Máximo.
                sumaCostosABBevoc += aux;   // Acumulo costos.
                c_medio->costoABBevocar = sumaCostosABBevoc/cantABBEvoc;    // Medio.
            }

            aux = sumaCostosRALevoc;   // Guardo los costos acumulados.
            if (evocacionRAL(auxiliar.doc,&auxiliar,c_max,c_medio)!=1){ // Fracaso:
                if (sumaCostosRALevoc>c_max->costoRALevocarFracaso)
                    c_max->costoRALevocarFracaso = sumaCostosRALevoc;
                sumaCostosRALevocF += sumaCostosRALevoc;
                c_medio->costoRALevocarFracaso = sumaCostosRALevocF/cantRALEvocF;
                sumaCostosRALevoc = aux;
            }
            else {  // Éxito:
                if (sumaCostosRALevoc>c_max->costoRALevocar)
                    c_max->costoRALevocar = sumaCostosRALevoc;  // Máximo.
                sumaCostosRALevoc += aux;   // Acumulo costos.
                c_medio->costoRALevocar = sumaCostosRALevoc/cantRALEvoc;    // Medio.
            }

            aux = sumaCostosRSevoc;   // Guardo los costos acumulados.
            if (evocacionRS(auxiliar.doc,&auxiliar,c_max,c_medio)!=1){ // Fracaso:
                if (sumaCostosRSevoc>c_max->costoRSevocarFracaso)
                    c_max->costoRSevocarFracaso = sumaCostosRSevoc;
                sumaCostosRSevocF += sumaCostosRSevoc;
                c_medio->costoRSevocarFracaso = sumaCostosRSevocF/cantRSEvocF;
                sumaCostosRSevoc = aux;
            }
            else {  // Éxito:
                if (sumaCostosRSevoc>c_max->costoRSevocar)
                    c_max->costoRSevocar = sumaCostosRSevoc;  // Máximo.
                sumaCostosRSevoc += aux;   // Acumulo costos.
                c_medio->costoRSevocar = sumaCostosRSevoc/cantRSEvoc;    // Medio.
            }
        }
        else
            printf("\n\tError: Codigo de operacion no reconocido.\n\t");
        codigoOperador=0;
    }
    fclose(archivo);
    return 1;
}


/* ==================================================================== */
/*                          PROGRAMA PRINCIPAL                          */
/* ==================================================================== */


int main() {
    // RESULTADOS:

    /*
        COSTOS LSO:
        Máximos
            Alta: 159.0 (En corrimientos de nuplas)
            Baja: 135.0 (En corrimientos de nuplas)
            Evocación: 110.0 (En celdas consultadas)
        Medios
            Alta: 26.560 (En corrimientos de nuplas)
            Baja: 47.482 (En corrimientos de nuplas)
            Evocación: 45.547 (En celdas consultadas)

        COSTOS LSOBB:
        Máximos
            Alta: 157.5 (En corrimientos de nuplas)
            Baja: 133.5 (En corrimientos de nuplas)
            Evocación: 7.0 (En celdas consultadas)
        Medios
            Alta: 25.060 (En corrimientos de nuplas)
            Baja: 45.982 (En corrimientos de nuplas)
            Evocación: 6.661 (En celdas consultadas)

        COSTOS ABB:
        Máximos
            Alta: 3.0 (En modificaciones de punteros)
            Baja: 2.5 (En modificaciones de punteros)
            Evocación: 27.0 (En celdas consultadas)
        Medios
            Alta: 3.0 (En modificaciones de punteros)
            Baja: 1.750 (En modificaciones de punteros)
            Evocación: 13.576 (En celdas consultadas)

        COSTOS RAL:
        Máximos
            Alta: 0.0 (En corrimientos de nuplas)
            Baja: 0.0 (En corrimientos de nuplas)
            Evocación: 63.0 (En celdas consultadas)
        Medios
            Alta: 0.0 (En corrimientos de nuplas)
            Baja: 0.0 (En corrimientos de nuplas)
            Evocación: 2.280 (En celdas consultadas)

        COSTOS RS:
        Máximos
            Alta: 2.0 (En modifaciones de punteros)
            Baja: 1.0 (En modifaciones de punteros)
            Evocación: 7.0 (En celdas consultadas)
        Medios
            Alta: 2.0 (En modifaciones de punteros)
            Baja: 1.0 (En modifaciones de punteros)
            Evocación: 1.989 (En celdas consultadas)
    */


    // ANÁLISIS:

    /* Con las funciones de costo dadas:
    - Corrimientos:
        Altas: El RAL es la mejor estructura con esta función
        de costo, ya que no necesita hacer corrimientos en altas.
        Entre las dos listas secuenciales, la LSOBB hace
        un corrimiento menos que la LSO porque a diferencia de ella
        no tiene un elemento ficticio.

        Bajas: El RAL también es la mejor para bajas porque tampoco
        necesita hacer corrimientos. De las dos listas secuenciales,
        nuevamente la LSOBB hace un corrimiento menos que la LSO
        por no tener que correr un elemento ficticio.

    - Cantidad de celdas consultadas:
        Evocaciones: Las mejores estructuras para evocaciones en este caso
        son la LSOBB y el RS. Tienen igual costo máximo (7), pero el RS
        tiene un mejor comportamiento esperado (aprox. 2 vs 6.6 en la LSOBB).
        Les siguen el RAL, que tiene un comportamiento esperado bueno (aprox. 2)
        pero un costo máximo malo en comparación a las dos anteriores (63), y
        el ABB que tiene costos decentes (27 máx y 13.5 esperado) pero es menos
        eficiente que las primeras dos.
        La LSO es la más costosa (110 max y 45.5 esperado).

    - Modificaciones de punteros:
        Altas: El RS le saca ventaja al ABB porque hace una modificacion menos. (2 vs 3 del ABB)

        Bajas: También es menos costoso el RS que el ABB en las bajas. Sobre todo
        porque al ABB se le suma el costo de copiar los datos y eso sube sus costos.

    CONCLUSIÓN: Considerando el análisis anterior, me parece que el RS es la mejor opción
    en general.
    Si tuviera que usar alguna de las otras, usaría el RAL porque le saca ventaja a las
    listas secuenciales y al ABB en CASI todo (si me pidieran específicamente que tenga
    en cuenta el peor caso de evocación usaría el ABB por sobre el RAL porque su costo
    máximo de evocación es menor).
    */

    int opc;
    int ex;
    inicializarRAL();
    inicializarRS();
    costos costosMax, costosMed;
    costosMax.costoLSOalta = 0;
    costosMax.costoLSObaja = 0;
    costosMax.costoLSOevocar = 0;
    costosMax.costoLSOBBalta = 0;
    costosMax.costoLSOBBbaja = 0;
    costosMax.costoLSOBBevocar = 0;
    costosMax.costoABBalta = 0;
    costosMax.costoABBbaja = 0;
    costosMax.costoABBevocar = 0;
    costosMax.costoRALalta = 0;
    costosMax.costoRALbaja = 0;
    costosMax.costoRALevocar = 0;
    costosMax.costoRSalta = 0;
    costosMax.costoRSbaja = 0;
    costosMax.costoRSevocar = 0;
    costosMed.costoLSOalta = 0;
    costosMed.costoLSObaja = 0;
    costosMed.costoLSOevocar = 0;
    costosMed.costoLSOBBalta = 0;
    costosMed.costoLSOBBbaja = 0;
    costosMed.costoLSOBBevocar = 0;
    costosMed.costoABBalta = 0;
    costosMed.costoABBbaja = 0;
    costosMed.costoABBevocar = 0;
    costosMed.costoRALalta = 0;
    costosMed.costoRALbaja = 0;
    costosMed.costoRALevocar = 0;
    costosMed.costoRSalta = 0;
    costosMed.costoRSbaja = 0;
    costosMed.costoRSevocar = 0;
    costosMax.costoLSOevocarFracaso = 0;
    costosMax.costoLSOBBevocarFracaso = 0;
    costosMax.costoABBevocarFracaso = 0;
    costosMax.costoRALevocarFracaso = 0;
    costosMax.costoRSevocarFracaso = 0;
    costosMed.costoLSOevocarFracaso = 0;
    costosMed.costoLSOBBevocarFracaso = 0;
    costosMed.costoABBevocarFracaso = 0;
    costosMed.costoRALevocarFracaso = 0;
    costosMed.costoRSevocarFracaso = 0;
    LSO[0].doc = INF;//+inf
    do{
        printf("\n");
        printf("\n\tMENU PRINCIPAL\n");
        printf("\t--------------\n");
        printf("\t1. Comparacion de Estructuras\n");
        printf("\t2. Mostrar LSO\n");
        printf("\t3. Mostrar LSOBB\n");
        printf("\t4. Mostrar ABB\n");
        printf("\t5. Mostrar RAL\n");
        printf("\t6. Mostrar RS\n");
        printf("\t7. Salir\n");
        printf("\t--------------\n");
        printf("\n\tSeleccione una opcion.\n\t");
        scanf("%d",&opc);
        switch(opc){
            case 1: if (cantLSO != 0 || cantLSOBB != 0 ||
                        cantABB != 0 || cantRAL != 0 || cantRS != 0) {
                        printf("\n\tERROR: Las estructuras deben estar vacias antes de la comparacion.\n\t");
                        break;
                    }
                    ex = lecturaOperaciones(&costosMax,&costosMed);
                    if (ex == 1){
                        fflush(stdin);
                        printf("\n\tComparacion exitosa.\n\n");

                        printf("\n\t=========================\n");
                        printf("\tLISTA SECUENCIAL ORDENADA\n");
                        printf("\t=========================\n");
                        printf("\tCOSTOS MAXIMOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMax.costoLSOalta);
                        printf("\t  BAJA: %0.1f\n",costosMax.costoLSObaja);
                        printf("\t  EVOCACION: %0.1f\n",costosMax.costoLSOevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMax.costoLSOevocarFracaso);
                        printf("\t-------------\n");
                        printf("\tCOSTOS MEDIOS\n");
                        printf("\t  ALTA: %0.3f\n",costosMed.costoLSOalta);
                        printf("\t  BAJA: %0.3f\n",costosMed.costoLSObaja);
                        printf("\t  EVOCACION: %0.3f\n",costosMed.costoLSOevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMed.costoLSOevocarFracaso);

                        printf("\n\n");

                        printf("\n\t==============================\n");
                        printf("\tLISTA SECUENCIAL ORDENADA (BB)\n");
                        printf("\t==============================\n");
                        printf("\tCOSTOS MAXIMOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMax.costoLSOBBalta);
                        printf("\t  BAJA: %0.1f\n",costosMax.costoLSOBBbaja);
                        printf("\t  EVOCACION: %0.1f\n",costosMax.costoLSOBBevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMax.costoLSOBBevocarFracaso);
                        printf("\t-------------\n");
                        printf("\tCOSTOS MEDIOS\n");
                        printf("\t  ALTA: %0.3f\n",costosMed.costoLSOBBalta);
                        printf("\t  BAJA: %0.3f\n",costosMed.costoLSOBBbaja);
                        printf("\t  EVOCACION: %0.3f\n",costosMed.costoLSOBBevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMed.costoLSOBBevocarFracaso);

                        printf("\n\n");

                        printf("\n\t=========================\n");
                        printf("\tARBOL BINARIO DE BUSQUEDA\n");
                        printf("\t=========================\n");
                        printf("\tCOSTOS MAXIMOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMax.costoABBalta);
                        printf("\t  BAJA: %0.1f\n",costosMax.costoABBbaja);
                        printf("\t  EVOCACION: %0.1f\n",costosMax.costoABBevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMax.costoABBevocarFracaso);
                        printf("\t-------------\n");
                        printf("\tCOSTOS MEDIOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMed.costoABBalta);
                        printf("\t  BAJA: %0.3f\n",costosMed.costoABBbaja);
                        printf("\t  EVOCACION: %0.3f\n",costosMed.costoABBevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMed.costoABBevocarFracaso);

                        printf("\n\n");

                        printf("\n\t======================\n");
                        printf("\tREBALSE ABIERTO LINEAL\n");
                        printf("\t======================\n");
                        printf("\tCOSTOS MAXIMOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMax.costoRALalta);
                        printf("\t  BAJA: %0.1f\n",costosMax.costoRALbaja);
                        printf("\t  EVOCACION: %0.1f\n",costosMax.costoRALevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMax.costoRALevocarFracaso);
                        printf("\t-------------\n");
                        printf("\tCOSTOS MEDIOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMed.costoRALalta);
                        printf("\t  BAJA: %0.1f\n",costosMed.costoRALbaja);
                        printf("\t  EVOCACION: %0.3f\n",costosMed.costoRALevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMed.costoRALevocarFracaso);

                        printf("\n\n");

                        printf("\n\t================\n");
                        printf("\tREBALSE SEPARADO\n");
                        printf("\t================\n");
                        printf("\tCOSTOS MAXIMOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMax.costoRSalta);
                        printf("\t  BAJA: %0.1f\n",costosMax.costoRSbaja);
                        printf("\t  EVOCACION: %0.1f\n",costosMax.costoRSevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMax.costoRSevocarFracaso);
                        printf("\t-------------\n");
                        printf("\tCOSTOS MEDIOS\n");
                        printf("\t  ALTA: %0.1f\n",costosMed.costoRSalta);
                        printf("\t  BAJA: %0.1f\n",costosMed.costoRSbaja);
                        printf("\t  EVOCACION: %0.3f\n",costosMed.costoRSevocar);
                        printf("\t  EVOCACION(Fracaso): %0.1f\n",costosMed.costoRSevocarFracaso);

                        printf("\n\n");
                    }
                    if (ex == 2)
                        printf("Error de carga.\n");
                    if (ex == 0)
                        printf("Error de archivo.\n");

                    //mostrar costos obtenidos por cada estructura. (Máximo y Promedio)
                    break;

            case 2: mostrarLSO();
                    break;

            case 3: mostrarLSOBB();
                    break;

            case 4: mostrarABB(arbol);
                    break;

            case 5: mostrarRAL();
                    break;

            case 6: mostrarRS();
                    break;

            default: break;
        }
    } while (opc != 7);
    return 0;
}
