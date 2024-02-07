#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INF 100000000
#define MAX 110

// CONTROL LISTA VACÍA - En main o usando otro caso de retorno en las funciones?
// PERTENECE - Al tener que pasarle una nupla completa, debo pedir todos los datos de un vendedor para buscarlo?
// CORRIMIENTOS DE MÁS (LSOBT) - El problema era que empezaba el proceso en la posición siguiente a la que debería?
// ELEMENTOS REPETIDOS MEM. PREVIA - Solo informar que hay elementos repetidos o mostrar los repetidos?



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
/*                                LISTAS                                */
/* ==================================================================== */

vendedor LSO[MAX+1]; //1 espacio extra para el +INF
vendedor LSOBT[MAX];
int cantLSO = 0;
int cantLSOBT = 0;


/* ==================================================================== */
/*                                FUNCIONES                             */
/* ==================================================================== */


void show (vendedor y){
    printf("\tDocumento: %d\n",y.doc);
    printf("\tNombre: %s\n",y.nom);
    printf("\tTelefono: %s\n",y.telefono);
    printf("\tMonto vendido: $%0.2f\n",y.montoVendido);
    printf("\tCantidad vendida: %d\n",y.cantidadVendida);
    printf("\tCanal de venta: %s\n",y.canalDeVenta);
}

void arreglarString (char str[]){
    int i;
    int tam = strlen(str);
    str[tam-1]='\0';
}

void pedirDatos (vendedor *aux){
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




/* ---------------------------------------------------------- */
/*                        LOCALIZAR                           */
/* ---------------------------------------------------------- */

int localizarBS (int x, int *pos){
    int i=0;
    while(x > LSO[i].doc)   // Mientras esté dentro de la estructura.
        i++;
    *pos = i;
    if (x < INF)    // No paró en la pos del +INF.
    {
        if (x == LSO[i].doc)
            return 1;    // Localizado exitosamente.
        else
            return 2;   // No localizado.
    }
    else   // Paró en la pos del +INF.
        return 2;    // No localizado.
}

int localizarBB(int x, int *pos){
    // TRISECCIÓN.
    // Limite inferior exclusivo.
    // Limite superior inclusivo.
    // Segmento mas grande a la derecha.

    int li = -1;
    int ls = cantLSOBT-1; // cant = cantidad de elementos. cant-1 = ultima pos de la lista.
    int m = ceil((li+ls)/2.0);

    while((li<ls) && (x != LSOBT[m].doc)){
        if (x > LSOBT[m].doc)
            li = m;
        else
            ls = m-1;
        m = ceil((li+ls)/2.0);
    }

    if (li<ls){
        *pos = m;  // pos del ultimo que comparó.
        return 1; // Localizado.
    }
    else {  //*** CORRECIÓN.
        if (x < LSOBT[m].doc)   // x menor -> pos actual.
            *pos = li+1;
        else                    // x mayor -> pos siguiente.
            *pos = ls+1;
        return 3; // No localizado.
    }
}


/* ---------------------------------------------------------- */
/*                           ALTA                             */
/* ---------------------------------------------------------- */

int altaBS(int x, vendedor y){
    strupr(y.nom);
    strupr(y.canalDeVenta);
    int i,pos,exito;
    if (strcmp(y.nom,"") != 0){
        if (cantLSO>=MAX)
            return -1;  // No hay espacio.
        exito = localizarBS(x,&pos);
        if (exito == 1)
            return 0;   // Documento ya registrado.
        else {
            for (i = cantLSO ; i >= pos ; i--)
                    LSO[i+1] = LSO[i];
            LSO[pos] = y;
            cantLSO++;
            return 1;   // Alta exitosa.
        }
    }else return 2; // Nupla con campos vacíos.
}

int altaBB(int x, vendedor y){
    strupr(y.nom);
    strupr(y.canalDeVenta);
    int i,pos,exito;
    if (strcmp(y.nom,"") != 0){
        if (cantLSOBT>=MAX)
            return -1;  // No hay espacio.
        exito = localizarBB(x,&pos);
        if (exito == 1)
            return 0;   // Documento ya registrado.
        else {      // Pos ocupada.
                for (i = cantLSOBT-1 ; i >= pos ; i--)  // Correción: cantLSOBT -> cantLSOBT-1
                    LSOBT[i+1] = LSOBT[i];
                LSOBT[pos] = y;
                cantLSOBT++;
                return 1;
            }
    } else return 2; // Nupla con campos vacíos.
}





/* ---------------------------------------------------------- */
/*                             BAJA                           */
/* ---------------------------------------------------------- */

int bajaBS(int x){
    int i, j, pos, exito;
    exito = localizarBS(x,&pos);
    if (exito == 1){
        printf("\n");
        show(LSO[pos]);
        printf("\n");
        do{
            printf("\n\t1. Eliminar\n\t2. Cancelar\n\t");
            scanf("%d",&j);
            if (j == 1){
                for (i = pos ; i < cantLSO ; i++)
                    {
                        LSO[i] = LSO[i+1];   //Eliminación.
                    }
                    cantLSO--;
                    return 1;   // Eliminación exitosa.
            }else   if (j == 2)
                        return -1;   // Eliminación cancelada.
                    else
                        printf("Ingrese una opcion valida.\n");
        } while (j != 1 && j != 2);
    } else
        return 0;  // No se encontró el elemento.
}

int bajaBB(int x){
    int i, j, pos, exito;
    exito = localizarBB(x,&pos);
    if (exito == 1){
        printf("\n");
        show(LSOBT[pos]);
        printf("\n");
        do{
            printf("\n\t1. Eliminar\n\t2. Cancelar\n\t");
            scanf("%d",&j);
            if (j == 1){
                for (i = pos ; i < cantLSOBT-1 ; i++)
                    {
                        LSOBT[i] = LSOBT[i+1];   //Eliminación.
                    }
                    cantLSOBT--;
                    return 1;   // Eliminación exitosa.
            }else   if (j == 2)
                        return -1;   // Eliminación cancelada.
                    else
                        printf("Ingrese una opcion valida.\n");
        }while (j != 1 && j != 2);
    }else
        return 0;  // No se encontró el elemento.
}



/* ---------------------------------------------------------- */
/*                        PERTENECE                           */
/* ---------------------------------------------------------- */

int pertenenciaBS(int x, vendedor y){
    int pos;
    if (localizarBS(x,&pos) == 1)
        if (strcmp(LSO[pos].nom,y.nom) == 0 &&
                LSO[pos].doc == y.doc &&
                strcmp(LSO[pos].telefono,y.telefono) == 0 &&
                LSO[pos].montoVendido == y.montoVendido &&
                LSO[pos].cantidadVendida == y.cantidadVendida &&
                strcmp(LSO[pos].canalDeVenta,y.canalDeVenta) == 0)  // Correción: Comparo los datos asociados.
            return 1;   // Es la nupla que se busca.
        else
            return 0;   // No es la nupla que se busca.
    else
        return 0;   // Nupla no encontrada.
}

int pertenenciaBB(int x, vendedor y){
    int pos;
    if (localizarBB(x,&pos) == 1)
        if (strcmp(LSOBT[pos].nom , y.nom) == 0 &&
                LSOBT[pos].doc == y.doc &&
                strcmp(LSOBT[pos].telefono , y.telefono) == 0 &&
                LSOBT[pos].montoVendido == y.montoVendido &&
                LSOBT[pos].cantidadVendida == y.cantidadVendida &&
                strcmp(LSOBT[pos].canalDeVenta , y.canalDeVenta) == 0)  // Correción: Comparo los datos asociados.
            return 1;   // Es la nupla que se busca.
        else
            return 0;   // No es la nupla que se busca.
    else
        return 0;   // X no encontrado.
}

/* ---------------------------------------------------------- */
/*                        EVOCACIÓN                           */
/* ---------------------------------------------------------- */

int evocacionBS(int x, vendedor *y){
    int pos,exito;
    exito = localizarBS(x,&pos);
    if (exito==1){
        *y = LSO[pos];
        return 1;     // Evocación exitosa.
    }
    else
        *y = LSO[pos];  // ******************
        return 0;  // Fracaso.
}

int evocacionBB(int x, vendedor *y){
    int pos,exito;
    exito = localizarBB(x,&pos);
    if (exito==1){
        *y = LSOBT[pos];
        return 1;     // Evocación exitosa.
    }
    else
        *y = LSOBT[pos];  // ******************
        return 0;  // Fracaso.
}



/* ---------------------------------------------------------- */
/*                   MEMORIZACIÓN PREVIA                      */
/* ---------------------------------------------------------- */

int memorizacion_previaLSO(int *bandera){
    int i;
    char basura;
    vendedor auxiliar;
    FILE *archivo;
    if ((archivo = fopen("Vendedores.txt","r")) == NULL)
        return -1;   // No se pudo abrir el archivo.
    else {
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
            //fscanf(archivo,"%c",&basura);
            i = altaBS(auxiliar.doc,auxiliar);
            if (i==(-1))
            {
                *bandera = auxiliar.doc;
                fclose(archivo);
                return 0;   // No hay más espacio.
            }
            else
                if (i==0) // Se encontró un codigo repetido.
                {
                    *bandera = auxiliar.doc;   // Continúa cargando (No retorna).
                    printf("\n\tDNI %d repetido. (Ya esta registrado)\n",*bandera);    // ***Corrección: Muestra elementos repetidos.
                }
        }
        fclose(archivo);
        return 1;  //Registro exitoso.
    }
}

int memorizacion_previaLSOBT(int *bandera){
    int i;
    char basura;
    vendedor auxiliar;
    FILE *archivo;
    if ((archivo = fopen("Vendedores.txt","r")) == NULL)
        return -1;   // No se pudo abrir el archivo.
    else {
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
            //fscanf(archivo,"%c",&basura);
            i = altaBB(auxiliar.doc,auxiliar);
            if (i==(-1))
            {
                *bandera = auxiliar.doc;
                fclose(archivo);
                return 0;   // No hay más espacio.
            }
            else
                if (i==0) // Se encontró un codigo repetido.
                {
                    *bandera = auxiliar.doc;   // Continúa cargando (No retorna).
                    printf("\n\tDNI %d repetido. (Ya esta registrado)\n",*bandera); //***Corrección: Muestra elementos repetidos.
                }
        }
        fclose(archivo);
        return 1;  //Registro exitoso.
    }
}

/* ---------------------------------------------------------- */



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

void mostrarLSOBT (void){ 
    int i;
    int cont = 0;
    printf("\n");
    printf("\n");
    for (i = 0 ; i < cantLSOBT ; i++)
    {
        fflush(stdin);
        printf("\n\tVendedor %d\n",i+1);
        printf("\t--------------\n");
        show(LSOBT[i]);
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
/*                          PROGRAMA PRINCIPAL                          */
/* ==================================================================== */

int main()
{
    int opc,opc2,opc3;
    LSO[0].doc = INF;//+inf
    int auxiliar;
    vendedor aux;
    do{
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n\tMENU PRINCIPAL\n");
        printf("\t--------------\n");
        printf("\t1. LSO\n");
        printf("\t2. LSO BT\n");
        printf("\t3. Salir\n");
        printf("\t--------------\n");
        printf("\n\tSeleccione la estructura con la que desea trabajar.\n\t");
        scanf("%d",&opc);
        switch(opc){
            case 1: do{
                        printf("\n\tLSO\n");
                        printf("\t--------------\n");
                        printf("\t1. Ingresar nuevo vendedor\n");
                        printf("\t2. Eliminar vendedor\n");
                        printf("\t3. Consulta de vendedor\n");
                        printf("\t4. Buscar en el registro\n");
                        printf("\t5. Mostrar estructura\n");
                        printf("\t6. Memorizacion previa\n");
                        printf("\t7. Volver al menu principal\n");
                        printf("\t--------------\n\t");
                        scanf("%d",&opc2);
                        switch (opc2){
                            case 1: //ALTA.
                                    pedirDatos(&aux);
                                    switch (altaBS(aux.doc,aux))
                                    {
                                        case 1: printf("Vendedor registrado con exito.\n");
                                                break;

                                        case -1: printf("No hay espacio para registrar al vendedor.\n");
                                                 break;

                                        case 0: printf("Ya existe un vendedor con ese nro de documento.\n");
                                                break;

                                        case 2: printf("No se registro el vendedor porque no se ingresaron todos los datos.\n");
                                                break;

                                        default: break;
                                    }
                                    printf("\n");

                                    fflush(stdin);
                                    break;

                            case 2: //BAJA.
                                    printf("\n\tIngrese el documento del vendedor: \n\t");
                                    scanf("%d",&auxiliar);
                                    printf("\n");
                                    if (cantLSO == 0){   // Lista vacía.
                                        printf("\t--------------\n");
                                        printf("\tERROR: Lista Vacia\n");
                                        printf("\t--------------\n\n");
                                        break;
                                    }
                                    switch (bajaBS(auxiliar))
                                    {
                                        case 1: printf("\nEliminado con exito.\n");
                                                break;

                                        case 0: printf("\nNo se encontro un vendedor con ese documento.\n");
                                                 break;

                                        case -1: printf("\nEliminacion cancelada.\n");

                                        default: break;
                                    }
                                    printf("\n");
                                    break;

                            case 3: //Evocación+Mostrar.
                                    printf("\n\tIngrese el documento del vendedor:\n\t");
                                    scanf("%d",&auxiliar);
                                    printf("\n");
                                    if (cantLSO == 0){
                                        printf("\t--------------\n");
                                        printf("\tERROR: Lista Vacia\n");
                                        printf("\t--------------\n\n");
                                        break;
                                    }
                                    switch (evocacionBS(auxiliar,&aux)){
                                        case 1: printf("\n");
                                                show(aux);
                                                printf("\n");
                                                break;

                                        case 0: printf("No se encontro al vendedor.\n");
                                                break;
                                    }
                                    printf("\n");
                                    break;

                            case 4: // Pertenencia.
                                    pedirDatos(&aux);
                                    if (pertenenciaBS(aux.doc,aux))
                                        printf("El vendedor esta en el registro.\n");
                                    else
                                        printf("El vendedor no esta en el registro.\n");
                                    printf("\n");
                                    break;

                            case 5: // Mostrar.
                                    if (cantLSO == 0){   // Lista vacía.
                                        printf("\n\n\t------------\n");
                                        printf("\tLISTA VACIA\n");
                                        printf("\t------------\n\n");
                                        break;
                                    }
                                    mostrarLSO();
                                    break;

                            case 6: // Memorizacion Previa.
                                    printf("\n");
                                    switch(memorizacion_previaLSO(&auxiliar))
                                    {
                                        case 1: printf("\nMemorizacion previa exitosa.\n");
                                                break;
                                        case -1: printf("\nError al abrir el archivo.\n");
                                                 break;
                                        case 0: printf("\nNo se pudo registrar desde el DNI %d en adelante porque no queda espacio disponible.\n",auxiliar);
                                                break;
                                    }
                                    printf("\n");
                                    break;
                        }
                    } while (opc2 != 7);
                    break;

            case 2: do{
                        printf("\n\tLSOBT\n");
                        printf("\t--------------\n");
                        printf("\t1. Ingresar nuevo vendedor\n");
                        printf("\t2. Eliminar vendedor\n");
                        printf("\t3. Consulta de vendedor\n");
                        printf("\t4. Buscar en el registro\n");
                        printf("\t5. Mostrar estructura\n");
                        printf("\t6. Memorizacion previa\n");
                        printf("\t7. Volver al menu principal\n");
                        printf("\t--------------\n\t");
                        scanf("%d",&opc3);
                        switch (opc3){
                            case 1: //ALTA.
                                    pedirDatos(&aux);
                                    switch (altaBB(aux.doc,aux))
                                    {
                                        case 1: printf("Vendedor registrado con exito.\n");
                                                break;

                                        case -1: printf("No hay espacio para registrar al vendedor.\n");
                                                 break;

                                        case 0: printf("Ya existe un vendedor con ese nro de documento.\n");
                                                break;

                                        case 2: printf("No se registro el vendedor porque no se ingresaron todos los datos.\n");
                                                break;

                                        default: break;
                                    }
                                    printf("\n");

                                    fflush(stdin);
                                    break;

                            case 2: //BAJA.
                                    printf("\n\tIngrese el documento del vendedor:\n\t");
                                    scanf("%d",&auxiliar);
                                    printf("\n");
                                    if (cantLSOBT == 0){   // Lista vacía.
                                        printf("\t--------------\n");
                                        printf("\tERROR: Lista Vacia\n");
                                        printf("\t--------------\n\n");
                                        break;
                                    }
                                    switch (bajaBB(auxiliar))
                                    {
                                        case 1: printf("\nEliminado con exito.\n");
                                                break;

                                        case 0: printf("\nNo se encontro un vendedor con ese documento.\n");
                                                 break;

                                        case -1: printf("\nEliminacion cancelada.\n");

                                        default: break;
                                    }
                                    printf("\n");
                                    break;

                            case 3: //Evocación+Mostrar.
                                    printf("\n\tIngrese el documento del vendedor:\n\t");
                                    scanf("%d",&auxiliar);
                                    printf("\n");
                                    if (cantLSOBT == 0){
                                        printf("\t--------------\n");
                                        printf("\tERROR: Lista Vacia\n");
                                        printf("\t--------------\n\n");
                                        break;
                                    }
                                    switch (evocacionBB(auxiliar,&aux)){
                                        case 1: printf("\n");
                                                show(aux);
                                                printf("\n");
                                                break;

                                        case 0: printf("No se encontro al vendedor.\n");
                                                break;
                                    }
                                    printf("\n");
                                    break;

                            case 4: // Pertenencia.
                                    if (cantLSOBT == 0){   // Lista vacía.
                                        printf("\t--------------\n");
                                        printf("\tERROR: Lista Vacia\n");
                                        printf("\t--------------\n\n");
                                        break;
                                    }
                                    pedirDatos(&aux);
                                    if (pertenenciaBB(aux.doc,aux))
                                        printf("El vendedor esta en el registro.\n");
                                    else
                                        printf("El vendedor no esta en el registro.\n");
                                    printf("\n");
                                    break;

                            case 5: // Mostrar.
                                    if (cantLSOBT == 0){   // Lista vacía.
                                        printf("\n\n\t------------\n");
                                        printf("\tLISTA VACIA\n");
                                        printf("\t------------\n\n");
                                        break;
                                    }
                                    mostrarLSOBT();
                                    break;

                            case 6: // Memorizacion Previa.
                                    printf("\n");
                                    switch(memorizacion_previaLSOBT(&auxiliar))
                                    {
                                        case 1: printf("\nMemorizacion previa exitosa.\n");
                                                break;
                                        case -1: printf("\nError al abrir el archivo.\n");
                                                 break;
                                        case 0: printf("\nNo se pudo registrar desde el DNI %d en adelante porque no queda espacio disponible.\n",auxiliar);
                                                break;
                                    }
                                    printf("\n");
                                    break;
                        }
                    } while (opc3 != 7);
                    break;

            default: break;
        }

    } while (opc != 3);
    return 0;
}
