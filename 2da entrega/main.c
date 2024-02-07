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

void pedirDatos (vendedor *aux) {
    fflush(stdin);
    do{
        printf("\n\tIngrese el numero de documento:\n\t");
        scanf("%d",&aux->doc);
        if (aux->doc<0 || aux->doc >= INF)
            printf("El valor debe estar entre 0 y 99.999.999\n");
    }while (aux->doc<0 || aux->doc >= INF);
    fflush(stdin);
    printf("\n\tIngrese nombre y apellido:\n\t");
    fgets(aux->nom,52,stdin);
    arreglarString(aux->nom);
    fflush(stdin);
    printf("\n\tIngrese el numero de telefono:\n\t");
    fgets(aux->telefono,17,stdin);
    arreglarString(aux->telefono);
    fflush(stdin);
    do{
        printf("\n\tIngrese el monto vendido:\n\t");
        scanf("%f",&aux->montoVendido);
        if (aux->montoVendido<0)
            printf("El monto debe ser positivo.\n");
    }while (aux->montoVendido<0);
    fflush(stdin);
    do{
        printf("\n\tIngrese la cantidad de productos vendidos:\n\t");
        scanf("%d",&aux->cantidadVendida);
        if (aux->cantidadVendida<0)
            printf("La cantidad debe ser positiva.\n");
    }while (aux->cantidadVendida<0);
    fflush(stdin);
    printf("\n\tIngrese el canal de venta:\n\t");
    fgets(aux->canalDeVenta,102,stdin);
    arreglarString(aux->canalDeVenta);
    fflush(stdin);
    strupr(aux->nom);
    strupr(aux->canalDeVenta);
    printf("\n");
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

/* -------------------      LOCALIZAR      -------------------*/

Nodet* localizarABB (int x, int *mayorMenor, int *exito, Nodet** ant) {
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
        *ant = p;
        if (p->vend.doc < x){
            *mayorMenor = 1;
            p = p->hd;
        }
        else {
            *mayorMenor = -1;
            p = p->hi;
        }
    }
    if (p == NULL)
        *exito = 0; // No Localizado.
    else
        *exito = 1; // localizado.
    return p;
}

/* -------------------         ALTA        -------------------*/

int altaABB (int x, vendedor y) {
    int exito ,mayorMenor;
    Nodet *pos = NULL;
    Nodet *ant = NULL;
    strupr(y.nom);
    strupr(y.canalDeVenta);
    pos = localizarABB(x,&mayorMenor,&exito,&ant);
    if (strcmp(y.nom,"") == 0)
        return 2; // La nupla tiene campos vacíos.
    if (exito==1)
        return 0; // Nupla repetida.
    Nodet *nuevo = (Nodet*)malloc(sizeof(Nodet));
    if (nuevo == NULL)
        return -1;  // No hay espacio.
    nuevo->vend = y; // Prepara el nuevo nodo.
    nuevo->hi = NULL;
    nuevo->hd = NULL;
    if (exito == -1){ // Arbol vacío.
       arbol = nuevo;
       cantABB++;
       return 1;
    }
    // Arbol no vacío.
    if (mayorMenor>0)
        ant->hd = nuevo;
    else
        ant->hi = nuevo;
    cantABB++;
    return 1;
}

/* -------------------         BAJA        -------------------*/

int bajaABB (int x) {
    int exito, mayorMenor;
    Nodet* pos = NULL;
    Nodet *ant = NULL;
    pos = localizarABB(x,&mayorMenor,&exito,&ant);
    if (exito != 1)
        return 0;   // No localizado.
    int k;
    Nodet *menorDeMayores;
    int comparacion;
    printf("\n");
    show(pos->vend);
    printf("\n");
    printf("\n");
    do{
        printf("\t1- Eliminar\n\t2- Cancelar\n");
        scanf("%d",&k);
        switch(k){
            case 1: if (ant == NULL)
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
                        free(pos);
                        cantABB--;
                        return 1;
                    }
                    if (pos->hi != NULL && pos->hd != NULL){ // Nodo con dos hijos - (Política de reemplazo: Menor de los mayores).
                        Nodet *antMenMayores = pos;
                        menorDeMayores = pos->hd; // Mayor...
                        while (menorDeMayores->hi != NULL){
                            antMenMayores = menorDeMayores; // Guardo el anterior.
                            menorDeMayores = menorDeMayores->hi; // Menor de los mayores.
                        }
                        pos->vend = menorDeMayores->vend;   // Reemplazo la nupla (Ahora hay 2 nuplas iguales).
                        if (menorDeMayores->hd == NULL){ // Si el reemplazante no tiene hijo.
                            if (pos->hd->vend.doc != menorDeMayores->vend.doc) // Si el reemplazo no es hijo der del nodo a eliminar...
                                 antMenMayores->hi = NULL;
                            else //El reemplazo es hijo der del nodo a eliminar (pos).
                                 pos->hd = NULL;
                        }
                        else {  // El reemplazante tiene hijo.
                            if (pos->hd->vend.doc != menorDeMayores->vend.doc) //Si el reemplazo no es hijo der del nodo a eliminar...
                                antMenMayores->hi = menorDeMayores->hd;
                            else
                                pos->hd = menorDeMayores->hd;
                        }
                        free(menorDeMayores);
                        cantABB--;
                        return 1;
                    }
            case 2: return -1;
            default: printf("\tIngrese una opcion valida.\n");
        }
    } while (k!=1 && k!=2);
    return -1;
}

/* -------------------        EVOCAR       -------------------*/

int evocacionABB(int x, vendedor *y) {
    int exito,mayorMenor;
    Nodet *ant = NULL;
    Nodet *pos = localizarABB(x,&mayorMenor,&exito,&ant);
    if (exito == 1){    // Localizado.
        *y = pos->vend;
        return 1;
    }
    else    // No localizado.
        return 0;
}

/* -------------------     MEM. PREVIA     -------------------*/

//(***) El archivo Vendedores.txt tiene una linea en blanco al final,
// esto hace que el último DNI se intente cargar dos veces
// al no cumplirse (feof(archivo)) despues de cargar el último.
// No afecta a la estructura, pero detecta el ultimo elemento como repetido.
// Borrando la linea en blanco del final funciona bien.

int memorizacion_previaABB(int *bandera) {
    int i;
    char basura;
    vendedor auxiliar;
    FILE *archivo;
    if ((archivo = fopen("Vendedores.txt","r")) == NULL)
        return -1;   // No se pudo abrir el archivo.
    fseek(archivo,0,SEEK_SET);
    while (!(feof(archivo))){
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
        i = altaABB(auxiliar.doc,auxiliar);
        if (i==(-1)){
            *bandera = auxiliar.doc;
            fclose(archivo);
            return 0;   // No hay más espacio.
        }
        if (i==0){ // Se encontró un codigo repetido.
            *bandera = auxiliar.doc;   // Continúa cargando (No retorna).
            printf("\n\tDNI %d repetido. (Ya esta registrado)\n",*bandera);
        }
    }
    fclose(archivo);
    return 1;  // Registro exitoso.
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

int M_RAC = 149;
vendedor RAC[149];   // Estructura.
int cantRAC = 0;

void inicializarRAC() {
    int i;
    for (i=0 ; i<M_RAC ; i++)
        RAC[i].doc = -1;    // Carga las M celdas con marca de vírgen (-1).
}

/* -------------------      LOCALIZAR      -------------------*/

int localizarRAC(int x, int *pos) {// -1 -> Celda virgen , -2 -> Celda libre.
    int i,flag;
    int condFlag = 0;
    int contador = 0;
    int k = 1;
    i = hashing(x,M_RAC);
    while ((contador < M_RAC) && (RAC[i].doc != -1) && (RAC[i].doc != x)){  // Mientras no mire M baldes, no esté en una celda virgen y no sea el x que busco.
        if (condFlag != 1){
            if (RAC[i].doc == -2){
                flag = i;
                condFlag = 1;   // Si ya guardó la primera libre, la condición del if no se cumple más.
            }
        }
        i = (i+k) % M_RAC;  // Avance.
        k++;
        contador++;
    }
    if (RAC[i].doc == x){   // Xb == X.
        *pos = i;
        return 1;   // Localizado.
    }
    if (contador < M_RAC){  // Si NO recorrió M baldes...
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

int altaRAC(int x, vendedor y) {
    int pos,ex;
    if (cantRAC >= M_RAC)
        return -1;   // No hay espacio.
    if (strcmp(y.nom,"") == 0)
        return 2;   // La nupla tiene campos vacíos.
    ex = localizarRAC(x,&pos);
    if (ex == 1)
        return 0;   // Documento repetido.
    if (ex == -1)
        return -2;  // No hay espacio para la lista que empieza en h(x).
    // Alta.
    RAC[pos] = y;
    cantRAC++;
    return 1; // Alta exitosa.
}

/* -------------------         BAJA        -------------------*/

int bajaRAC(int x) {
    int pos,ex,k;
    ex = localizarRAC(x,&pos);
    if (ex != 1)
        return 0;   // No se encontró el elemento.
    printf("\n");
    show(RAC[pos]);
    printf("\n");
    do{
        printf("\n\t1. Eliminar\n\t2. Cancelar\n");
        scanf("%d",&k);
        switch(k){
            case 1: RAC[pos].doc = -2; // Celda libre.
                    cantRAC--;
                    return 1;   // Eliminación exitosa.
            case 2: return -1; // Eliminación cancelada.
            default: printf("\tIngrese una opcion valida:\n");
                     break;
        }
    } while (k!=1 && k!=2);
    return -1;
}

/* -------------------        EVOCAR       -------------------*/

int evocacionRAC(int x, vendedor *y) {
    int pos,ex;
    ex = localizarRAC(x,&pos);
    if (ex == 1){
        *y = RAC[pos];
        return 1; // Encontrado.
    }
    else
        return 0; // No encontrado.
}

/* -------------------     MEM. PREVIA     -------------------*/

//(***)
int memorizacion_previaRAC(int *bandera) {
    int i;
    char basura;
    vendedor auxiliar;
    FILE *archivo;
    if ((archivo = fopen("Vendedores.txt","r")) == NULL)
        return -1;   // No se pudo abrir el archivo.
    fseek(archivo,0,SEEK_SET);
    while (!(feof(archivo))){
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
        i = altaRAC(auxiliar.doc,auxiliar);
        if (i==(-1)){  // No hay más espacio en la estructura.
            *bandera = auxiliar.doc;
            fclose(archivo);
            return 0;
        }
        if (i==0){  // Se encontró un codigo repetido.
            *bandera = auxiliar.doc;   // Continúa cargando (No retorna).
            printf("\n\tDNI %d repetido. (Ya esta registrado)\n",*bandera);
        }
        if (i==(-2)){ // No hay más espacio para la lista que empieza en h(x).
            *bandera = auxiliar.doc;   // Continúa cargando (No retorna).
            printf("\n\tDNI %d no se pudo registrar por falta de espacio.\n",*bandera);
        }
    }
    fclose(archivo);
    return 1;  // Registro exitoso.
}

/* -------------------       MOSTRAR       -------------------*/

void mostrarRAC() {
    int i;
    int cont = 0;
    for (i=0 ; i<M_RAC ; i++){
        fflush(stdin);
        printf("\n\tCELDA %d\n",i);
        if (RAC[i].doc == -1){
            printf("\t--------------\n");
            printf("\tVIRGEN(-1)\n");
            printf("\t--------------\n\n");
        }
        else
            if (RAC[i].doc == -2){
                printf("\t--------------\n");
                printf("\tLIBRE(-2)\n");
                printf("\t--------------\n\n");
            }
            else {
                printf("\t--------------\n");
                show(RAC[i]);
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

// p = 1.84 <-> N = 110 y M = 59.58 ---> M tiene que ser primo. M = 61.

int M_RS = 61;

typedef struct nodo_t{
    vendedor dato;
    struct nodo_t *sig;
} nodo;

typedef struct{
    nodo *Acc;
} lista;

lista RS[61];   // Estructura.
int cantRS = 0;

void inicializarRS() {
    int i;
    for (i=0 ; i<M_RS ; i++)
        RS[i].Acc = NULL;   // Carga los M accesos a listas con NULL.
}

/* -------------------      LOCALIZAR      -------------------*/

int localizarRS(int x, int *indiceAcc, nodo **pos){
    int i = hashing(x,M_RS);    // Acceso a lista.
    nodo *p,*ant;
    p = RS[i].Acc;
    ant = NULL;    // *** Cuando no entra al while, ant es NULL.
    while (p != NULL && p->dato.doc != x){
        ant = p;
        p = p->sig;
    }
    *indiceAcc = i;
    *pos = ant;    // Devuelvo el nodo anterior.
    if (p != NULL)
        return 1;   // Localizado.
    else
        return 0;   // No localizado.
}

/* -------------------         ALTA        -------------------*/

int altaRS(int x, vendedor y){
    int acc,ex;
    nodo *ant = NULL;
    if (strcmp(y.nom,"") == 0) // Si la nupla no tiene campos vacíos.
        return 2;
    ex = localizarRS(x,&acc,&ant);
    if (ex == 1)
        return 0;   // X repetido. Fracasa.
    nodo *aux = (nodo*)malloc(sizeof(nodo));
    if (aux == NULL)
        return -1;  // Estructura llena.
    aux->dato = y;
    aux->sig = RS[acc].Acc;
    RS[acc].Acc = aux;  // Alta al principio de la lista vinculada.
    cantRS++;
    return 1;   // Alta exitosa.
}

/* -------------------         BAJA        -------------------*/

int bajaRS(int x){
    int acc,ex,k,primerElem;
    nodo *ant = NULL;
    nodo *aux;
    ex = localizarRS(x,&acc,&ant);
    if (ex != 1)
        return 0;  // No Localizado.
    if (ant == NULL)    // ant == NULL cuando se localiza el primer elemento de la lista.
        primerElem = 1;
    else
        primerElem = 0;
    printf("\n");
    if (primerElem == 0)   // No es el primer elemento.
        show(ant->sig->dato);
    else    // Es el primer elemento.
        show(RS[acc].Acc->dato);
    printf("\n");
    do{
        printf("\n\t1. Eliminar\n\t2. Cancelar\n");
        scanf("%d",&k);
        switch(k){
            case 1: if (primerElem == 1){  // Eliminar el primer elemento de la lista.
                        aux = RS[acc].Acc->sig; // Guardo el sig del primero.
                        free(RS[acc].Acc);  // Libero el primero.
                        RS[acc].Acc = aux;  // Acceso = sig del eliminado.
                        cantRS--;
                        return 1;   // Eliminación exitosa.
                    }
                    if (ant->sig->sig == NULL){  // Eliminar el último elemento de la lista.
                        ant->sig = NULL;    // Anteúltimo->sig = NULL;
                        free(ant->sig);     // Libero el último.
                        cantRS--;
                        return 1;   // Eliminación exitosa.
                    }
                    // El elemento a eliminar no es un extremo de la lista.
                    aux = ant->sig->sig;    // Guardo el sig del nodo a eliminar.
                    free(ant->sig);     // Libero el nodo a eliminar.
                    ant->sig = aux;     // anterior->sig = anterior->sig->sig.
                    cantRS--;
                    return 1;   // Eliminación exitosa.

            case 2: return -1; // Eliminación cancelada.
            default: printf("\tIngrese una opcion valida:\n");
                     break;
        }
    } while (k!=1 && k!=2);
    return -1;
}

/* -------------------        EVOCAR       -------------------*/

int evocacionRS(int x, vendedor *y){
    int acc,ex;
    nodo *ant = NULL;
    ex = localizarRS(x,&acc,&ant);
    if (ex == 0)
        return 0;   // No encontrado.
    if (ant == NULL)   // Es el primer elemento de la lista.
        *y = RS[acc].Acc->dato;
    else    // No es el primer elemento de la lista.
        *y = ant->sig->dato;
    return 1;   // Encontrado.
}

/* -------------------     MEM. PREVIA     -------------------*/

//(***)
int memorizacion_previaRS(int *bandera){
    int i;
    char basura;
    vendedor auxiliar;
    FILE *archivo;
    if ((archivo = fopen("Vendedores.txt","r")) == NULL)
        return -1;   // No se pudo abrir el archivo.
    fseek(archivo,0,SEEK_SET);
    while (!(feof(archivo))){
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
        i = altaRS(auxiliar.doc,auxiliar);
        if (i==(-1)){
            *bandera = auxiliar.doc;
            fclose(archivo);
            return 0;   // No hay más espacio.
        }
        if (i==0){ // Se encontró un codigo repetido.
            *bandera = auxiliar.doc;   // Continúa cargando (No retorna).
            printf("\n\tDNI %d repetido. (Ya esta registrado)\n",*bandera);
        }
    }
    fclose(archivo);
    return 1;  // Registro exitoso.
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


/* ==================================================================== */
/*                          PROGRAMA PRINCIPAL                          */
/* ==================================================================== */


int main() {
    int opc,opc2,opc3;
    int auxiliar;
    vendedor aux;
    inicializarRAC();
    inicializarRS();
    do{
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\n\tMENU PRINCIPAL\n");
        printf("\t--------------\n");
        printf("\t1. ABB\n");
        printf("\t2. RAC\n");
        printf("\t3. RS\n");
        printf("\t4. Salir\n");
        printf("\t--------------\n");
        printf("\n\tSeleccione la estructura con la que desea trabajar.\n\t");
        scanf("%d",&opc);
        switch(opc){
            case 1: printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                    do{
                        printf("\n\tARBOL BINARIO DE BUSQUEDA\n");
                        printf("\t-------------------------\n");
                        printf("\t1. Ingresar nuevo vendedor\n");
                        printf("\t2. Eliminar vendedor\n");
                        printf("\t3. Consulta de vendedor\n");
                        printf("\t4. Mostrar estructura\n");
                        printf("\t5. Memorizacion previa\n");
                        printf("\t6. Volver al menu principal\n");
                        printf("\t-------------------------\n\t");
                        scanf("%d",&opc2);
                        switch (opc2){
                            case 1: //ALTA.
                                    pedirDatos(&aux);
                                    switch (altaABB(aux.doc,aux)){
                                        case 1: printf("\tVendedor registrado con exito.\n");
                                                break;

                                        case -1: printf("\tNo hay espacio para registrar al vendedor.\n");
                                                 break;

                                        case 0: printf("\tYa existe un vendedor con ese nro de documento.\n");
                                                break;

                                        case 2: printf("\tNo se registro el vendedor porque no se ingresaron todos los datos.\n");
                                                break;

                                        default: break;
                                    }
                                    printf("\n");

                                    fflush(stdin);
                                    break;

                            case 2: //BAJA.
                                    fflush(stdin);
                                    do{
                                        printf("\n\tIngrese el numero de documento:\n\t");
                                        scanf("%d",&auxiliar);
                                        if (auxiliar<0 || auxiliar >= INF)
                                        printf("\tEl valor debe estar entre 0 y 99.999.999\n");
                                    } while (auxiliar<0 || auxiliar >= INF);
                                    fflush(stdin);
                                    printf("\n");
                                    if (cantABB == 0){   // Estructura vacía.
                                        printf("\n\t-----------------------\n");
                                        printf("\tERROR: Estructura Vacia\n");
                                        printf("\t-----------------------\n\n");
                                        break;
                                    }
                                    switch (bajaABB(auxiliar)){
                                        case 1: printf("\n\tEliminado con exito.\n");
                                                break;

                                        case 0: printf("\n\tNo se encontro un vendedor con ese documento.\n");
                                                 break;

                                        case -1: printf("\n\tEliminacion cancelada.\n");

                                        default: break;
                                    }
                                    printf("\n");
                                    break;

                            case 3: //Evocación+Mostrar.
                                    fflush(stdin);
                                    do{
                                        printf("\n\tIngrese el numero de documento:\n\t");
                                        scanf("%d",&auxiliar);
                                        if (auxiliar<0 || auxiliar >= INF)
                                        printf("\tEl valor debe estar entre 0 y 99.999.999\n");
                                    } while (auxiliar<0 || auxiliar >= INF);
                                    fflush(stdin);
                                    printf("\n");
                                    if (cantABB == 0){
                                        printf("\n\t-----------------------\n");
                                        printf("\tERROR: Estructura Vacia\n");
                                        printf("\t-----------------------\n\n");
                                        break;
                                    }
                                    switch (evocacionABB(auxiliar,&aux)){
                                        case 1: printf("\n");
                                                show(aux);
                                                printf("\n");
                                                break;

                                        case 0: printf("\tNo se encontro al vendedor.\n");
                                                break;
                                    }
                                    printf("\n");
                                    break;

                            case 4: // Mostrar.
                                    if (cantABB == 0){   // Lista vacía.
                                        printf("\n\n\t----------------\n");
                                        printf("\tESTRUCTURA VACIA\n");
                                        printf("\t----------------\n\n");
                                        break;
                                    }
                                    contVend = 0;
                                    contAux = 0;
                                    mostrarABB(arbol);
                                    break;

                            case 5: // Memorizacion Previa.
                                    printf("\n");
                                    switch(memorizacion_previaABB(&auxiliar)){
                                        case 1: printf("\n\tMemorizacion previa exitosa.\n");
                                                break;
                                        case -1: printf("\n\tError al abrir el archivo.\n");
                                                 break;
                                        case 0: printf("\n\tNo se pudo registrar desde el DNI %d en adelante porque no queda espacio disponible.\n",auxiliar);
                                                break;
                                    }
                                    printf("\n");
                                    break;
                        }
                    } while (opc2 != 6);
                    break;

            case 2: printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                    do{
                        printf("\n\tREBALSE ABIERTO CUADRATICO\n");
                        printf("\t--------------------------\n");
                        printf("\t1. Ingresar nuevo vendedor\n");
                        printf("\t2. Eliminar vendedor\n");
                        printf("\t3. Consulta de vendedor\n");
                        printf("\t4. Mostrar estructura\n");
                        printf("\t5. Memorizacion previa\n");
                        printf("\t6. Volver al menu principal\n");
                        printf("\t--------------------------\n\t");
                        scanf("%d",&opc3);
                        switch (opc3){
                            case 1: //ALTA.
                                    pedirDatos(&aux);
                                    switch (altaRAC(aux.doc,aux)){
                                        case 1: printf("\tVendedor registrado con exito.\n");
                                                break;

                                        case -1: printf("\tLa lista esta llena.\n");
                                                 break;

                                        case -2: printf("\tNo hay espacio para registrar al vendedor.\n");
                                                 break;

                                        case 0: printf("\tYa existe un vendedor con ese nro de documento.\n");
                                                break;

                                        case 2: printf("\tNo se registro el vendedor porque no se ingresaron todos los datos.\n");
                                                break;

                                        default: break;
                                    }
                                    printf("\n");

                                    fflush(stdin);
                                    break;

                            case 2: //BAJA.
                                    fflush(stdin);
                                    do{
                                        printf("\n\tIngrese el numero de documento:\n\t");
                                        scanf("%d",&auxiliar);
                                        if (auxiliar<0 || auxiliar >= INF)
                                        printf("\tEl valor debe estar entre 0 y 99.999.999\n");
                                    } while (auxiliar<0 || auxiliar >= INF);
                                    fflush(stdin);
                                    printf("\n");
                                    if (cantRAC == 0){   // Estructura vacía.
                                        printf("\n\t-----------------------\n");
                                        printf("\tERROR: Estructura Vacia\n");
                                        printf("\t-----------------------\n\n");
                                        break;
                                    }
                                    switch (bajaRAC(auxiliar)){
                                        case 1: printf("\n\tEliminado con exito.\n");
                                                break;

                                        case 0: printf("\n\tNo se encontro un vendedor con ese documento.\n");
                                                 break;

                                        case -1: printf("\n\tEliminacion cancelada.\n");

                                        default: break;
                                    }
                                    printf("\n");
                                    break;

                            case 3: //Evocación+Mostrar.
                                    fflush(stdin);
                                    do{
                                        printf("\n\tIngrese el numero de documento:\n\t");
                                        scanf("%d",&auxiliar);
                                        if (auxiliar<0 || auxiliar >= INF)
                                        printf("\tEl valor debe estar entre 0 y 99.999.999\n");
                                    } while (auxiliar<0 || auxiliar >= INF);
                                    fflush(stdin);
                                    printf("\n");
                                    if (cantRAC == 0){
                                        printf("\n\t-----------------------\n");
                                        printf("\tERROR: Estructura Vacia\n");
                                        printf("\t-----------------------\n\n");
                                        break;
                                    }
                                    switch (evocacionRAC(auxiliar,&aux)){
                                        case 1: printf("\n");
                                                show(aux);
                                                printf("\n");
                                                break;

                                        case 0: printf("\tNo se encontro al vendedor.\n");
                                                break;
                                    }
                                    printf("\n");
                                    break;

                            case 4: // Mostrar.
                                    mostrarRAC();
                                    if (cantRAC==0)
                                        printf("\n\t(ESTRUCTURA VACIA)\n\n");
                                    break;

                            case 5: // Memorizacion Previa.
                                    printf("\n");
                                    switch(memorizacion_previaRAC(&auxiliar)){
                                        case 1: printf("\t\nMemorizacion previa exitosa.\n");
                                                break;
                                        case -1: printf("\t\nError al abrir el archivo.\n");
                                                 break;
                                        case 0: printf("\t\nNo se pudo registrar desde el DNI %d en adelante porque no queda espacio disponible.\n",auxiliar);
                                                break;
                                    }
                                    printf("\n");
                                    break;
                        }
                    } while (opc3 != 6);
                    break;

            case 3: printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                    do{
                        printf("\n\tREBALSE SEPARADO\n");
                        printf("\t----------------\n");
                        printf("\t1. Ingresar nuevo vendedor\n");
                        printf("\t2. Eliminar vendedor\n");
                        printf("\t3. Consulta de vendedor\n");
                        printf("\t4. Mostrar estructura\n");
                        printf("\t5. Memorizacion previa\n");
                        printf("\t6. Volver al menu principal\n");
                        printf("\t----------------\n\t");
                        scanf("%d",&opc3);
                        switch (opc3){
                            case 1: //ALTA.
                                    pedirDatos(&aux);
                                    switch (altaRS(aux.doc,aux)){
                                        case 1: printf("\tVendedor registrado con exito.\n");
                                                break;

                                        case -1: printf("\tNo hay espacio para registrar al vendedor.\n");
                                                 break;

                                        case 0: printf("\tYa existe un vendedor con ese nro de documento.\n");
                                                break;

                                        case 2: printf("\tNo se registro el vendedor porque no se ingresaron todos los datos.\n");
                                                break;

                                        default: break;
                                    }
                                    printf("\n");

                                    fflush(stdin);
                                    break;

                            case 2: //BAJA.
                                    fflush(stdin);
                                    do{
                                        printf("\n\tIngrese el numero de documento:\n\t");
                                        scanf("%d",&auxiliar);
                                        if (auxiliar<0 || auxiliar >= INF)
                                        printf("\tEl valor debe estar entre 0 y 99.999.999\n");
                                    } while (auxiliar<0 || auxiliar >= INF);
                                    fflush(stdin);
                                    printf("\n");
                                    if (cantRS == 0){   // Estructura vacía.
                                        printf("\n\t-----------------------\n");
                                        printf("\tERROR: Estructura Vacia\n");
                                        printf("\t-----------------------\n\n");
                                        break;
                                    }
                                    switch (bajaRS(auxiliar)){
                                        case 1: printf("\n\tEliminado con exito.\n");
                                                break;

                                        case 0: printf("\n\tNo se encontro un vendedor con ese documento.\n");
                                                 break;

                                        case -1: printf("\n\tEliminacion cancelada.\n");

                                        default: break;
                                    }
                                    printf("\n");
                                    break;

                            case 3: //Evocación+Mostrar.
                                    fflush(stdin);
                                    do{
                                        printf("\n\tIngrese el numero de documento:\n\t");
                                        scanf("%d",&auxiliar);
                                        if (auxiliar<0 || auxiliar >= INF)
                                        printf("\tEl valor debe estar entre 0 y 99.999.999\n");
                                    } while (auxiliar<0 || auxiliar >= INF);
                                    fflush(stdin);
                                    printf("\n");
                                    if (cantRS == 0){
                                        printf("\n\t-----------------------\n");
                                        printf("\tERROR: Estructura Vacia\n");
                                        printf("\t-----------------------\n\n");
                                        break;
                                    }
                                    switch (evocacionRS(auxiliar,&aux)){
                                        case 1: printf("\n");
                                                show(aux);
                                                printf("\n");
                                                break;

                                        case 0: printf("\tNo se encontro al vendedor.\n");
                                                break;
                                    }
                                    printf("\n");
                                    break;

                            case 4: // Mostrar.
                                    mostrarRS();
                                    if (cantRS==0)
                                        printf("\n\t(ESTRUCTURA VACIA)\n\n");
                                    break;

                            case 5: // Memorizacion Previa.
                                    printf("\n");
                                    switch(memorizacion_previaRS(&auxiliar)){
                                        case 1: printf("\n\tMemorizacion previa exitosa.\n");
                                                break;
                                        case -1: printf("\n\tError al abrir el archivo.\n");
                                                 break;
                                        case 0: printf("\n\tNo se pudo registrar desde el DNI %d en adelante porque no queda espacio disponible.\n",auxiliar);
                                                break;
                                    }
                                    printf("\n");
                                    break;
                        }
                    } while (opc3 != 6);
                    break;

            default: break;
        }
    } while (opc != 4);
    return 0;
}
