#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INF 99999999
#define MAX 110

// Grupo 19 - Axel Gonzalo Marconi Vejar.

typedef struct {
    int doc;
    char nom[51];
    char telefono[16];
    float montoVendido;
    int cantidadVendida;
    char canalDeVenta[21];
} vendedor;


// Estructuras:
vendedor LSO[MAX+1]; //1 espacio extra para el +INF
vendedor LSOBT[MAX];
int cantLSO = 0;
int cantLSOBT = 0;


// Rutinas:
void show(vendedor y)
{
    printf("\tDocumento: %d\n",y.doc);
    printf("\tNombre: %s\n",y.nom);
    printf("\tTelefono: %s\n",y.telefono);
    printf("\tMonto vendido: $%0.2f\n",y.montoVendido);
    printf("\tCantidad vendida: %d\n",y.cantidadVendida);
    printf("\tCanal de venta: %s\n",y.canalDeVenta);
}

int localizarBS(int x, int *pos)
{
    int i=0;
    while(x > LSO[i].doc)   // Mientras esté dentro de la estructura.
        i++;

    if (x == LSO[i].doc) // Encuentra el elemento.
    {
        *pos = i;      // Devuelve la pos del elemento encontrado.
        return 1;    // Localizado exitosamente.
    }
    else {   // No encuentra el elemento.
        *pos = i;      // Devuelve la pos del ultimo elemento que comparó.
        return 2;    // Fracaso 2 (pos ocupada)
    }
}

int localizarBB(int x, int *pos)
{
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
    else {
        if (m == -1){   // El elemento debería estar al inicio de la lista.
            *pos = 0;
            return 3;   // No localizado.
        }
        if (li == cantLSOBT-1)     // El elemento debería estar al final de la lista.
            *pos = m+1;
        else {      // No debería estar en ninguno de los extremos de la lista.
            if (x > LSOBT[m].doc)  // x mayor que el último que se comparó.
                *pos = m+1; // Debería estar en la pos siguiente.
            else   // x menor que el último que se comparó.
                *pos = m;   // Debería estar en la pos actual.
        }
        return 3;  // No localizado.
    }
}

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
                    LSO[i+1] = LSO[i];    //shift
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
                for (i = cantLSOBT ; i >= pos ; i--)
                    LSOBT[i+1] = LSOBT[i];    //shift
                LSOBT[pos] = y;
                cantLSOBT++;
                return 1;
            }
    } else return 2; // Nupla con campos vacíos.
}

int bajaBS(int x){
    int i, j, pos, exito;
    exito = localizarBS(x,&pos);
    if (exito == 1){
        printf("\n");
        show(LSO[pos]);
        printf("\n");
        do{
            printf("1 - Eliminar\n2 - Cancelar\n");
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
        }while (j != 1 && j != 2);
    }else
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
            printf("1 - Eliminar\n2 - Cancelar\n");
            scanf("%d",&j);
            if (j == 1){
                for (i = pos ; i < cantLSOBT ; i++)
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

int pertenenciaBS(int x){
    int pos;
    if (localizarBS(x,&pos) == 1)
        return 1;   // Elemento encontrado.
    else
        return 0;   // Elemento no encontrado.
}

int pertenenciaBB(int x){
    int pos;
    if (localizarBB(x,&pos) == 1)
        return 1;   // Elemento encontrado.
    else
        return 0;   // Elemento no encontrado.
}

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

void arreglarString(char str[])
{
    int i;
    int tam = strlen(str);
    str[tam-1]='\0';
}

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
                }
        }
        fclose(archivo);
        return 1;  //Registro exitoso.
    }
}

void mostrarLSO(void){
    int i;
    printf("\n");
    printf("\n");
    for (i = 0 ; i < cantLSO ; i++)
    {
        fflush(stdin);
        printf("\n\tVendedor %d\n",i+1);
        printf("\t--------------\n");
        show(LSO[i]);
        printf("\t--------------\n\n");
    }
    printf("\n");
    printf("\n");
}

void mostrarLSOBT(void){
    int i;
    printf("\n");
    printf("\n");
    for (i = 0 ; i < cantLSOBT ; i++)
    {
        fflush(stdin);
        printf("\n\tVendedor %d\n",i+1);
        printf("\t--------------\n");
        show(LSOBT[i]);
        printf("\t--------------\n\n");
    }
    printf("\n");
    printf("\n");
}

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
        printf("\t1. LSO con terminacion por contenido\n");
        printf("\t2. LSO con Busqueda Binaria\n");
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
                                    fflush(stdin);
                                    do{
                                        printf("Ingrese el numero de documento: ");
                                        scanf("%d",&aux.doc);
                                        if (aux.doc<0 || aux.doc >= INF)
                                            printf("El valor debe estar entre 0 y 99.999.998\n");
                                    }while (aux.doc<0 || aux.doc >= INF);
                                    fflush(stdin);

                                    printf("Ingrese nombre y apellido: ");
                                    fgets(aux.nom,52,stdin);
                                    arreglarString(aux.nom);
                                    fflush(stdin);

                                    printf("Ingrese el numero de telefono: ");
                                    fgets(aux.telefono,17,stdin);
                                    arreglarString(aux.telefono);
                                    fflush(stdin);

                                    do{
                                        printf("Ingrese el monto vendido: ");
                                        scanf("%f",&aux.montoVendido);
                                        if (aux.montoVendido<0)
                                            printf("El monto debe ser positivo.\n");
                                    }while (aux.montoVendido<0);
                                    fflush(stdin);

                                    do{
                                        printf("Ingrese la cantidad de productos vendidos: ");
                                        scanf("%d",&aux.cantidadVendida);
                                        if (aux.cantidadVendida<0)
                                            printf("La cantidad debe ser positiva.\n");
                                    }while (aux.cantidadVendida<0);
                                    fflush(stdin);

                                    printf("Ingrese el canal de venta: ");
                                    fgets(aux.canalDeVenta,102,stdin);
                                    arreglarString(aux.canalDeVenta);
                                    fflush(stdin);

                                    printf("\n");
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
                                    printf("Ingrese el documento del vendedor: \n");
                                    //fflush(stdin);
                                    scanf("%d",&auxiliar);
                                    //fflush(stdin);
                                    printf("\n");
                                    switch (bajaBS(auxiliar))
                                    {
                                        case 1: printf("Eliminado con exito.\n");
                                                break;

                                        case 0: printf("No se encontro un vendedor con ese documento.\n");
                                                 break;

                                        case -1: printf("Eliminacion cancelada.\n");

                                        default: break;
                                    }
                                    printf("\n");
                                    break;

                            case 3: //Evocación+Mostrar.
                                    printf("Ingrese el documento del vendedor: \n");
                                    //fflush(stdin);
                                    scanf("%d",&auxiliar);
                                    //fflush(stdin);
                                    printf("\n");
                                    if (evocacionBS(auxiliar,&aux))
                                    {
                                        printf("\n");
                                        show(aux);
                                        printf("\n");
                                    }
                                    else
                                        printf("No se encontro al vendedor.\n");
                                    printf("\n");
                                    break;

                            case 4: // Pertenencia.
                                    printf("Ingrese el documento del vendedor: \n");
                                    //fflush(stdin);
                                    scanf("%d",&auxiliar);
                                    //fflush(stdin);
                                    printf("\n");
                                    if (pertenenciaBS(auxiliar))
                                        printf("El vendedor esta en el registro.\n");
                                    else
                                        printf("El vendedor no esta en el registro.\n");
                                    printf("\n");
                                    break;

                            case 5: // Mostrar.
                                    mostrarLSO();
                                    break;

                            case 6: // Memorizacion Previa.
                                    printf("\n");
                                    switch(memorizacion_previaLSO(&auxiliar))
                                    {
                                        case 1: printf("Memorizacion previa exitosa.\n");
                                                break;
                                        case -1: printf("Error al abrir el archivo.\n");
                                                 break;
                                        case 0: printf("No se pudo cargar desde el documento %d en adelante porque no queda espacio disponible.\n",auxiliar);
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
                                    fflush(stdin);
                                    do{
                                        printf("Ingrese el numero de documento: ");
                                        scanf("%d",&aux.doc);
                                        if (aux.doc<0 || aux.doc >= INF)
                                            printf("El valor debe estar entre 0 y 99.999.998\n");
                                    }while (aux.doc<0 || aux.doc >= INF);
                                    fflush(stdin);

                                    printf("Ingrese nombre y apellido: ");
                                    fgets(aux.nom,52,stdin);
                                    arreglarString(aux.nom);
                                    fflush(stdin);

                                    printf("Ingrese el numero de telefono: ");
                                    fgets(aux.telefono,17,stdin);
                                    arreglarString(aux.telefono);
                                    fflush(stdin);

                                    do{
                                        printf("Ingrese el monto vendido: ");
                                        scanf("%f",&aux.montoVendido);
                                        if (aux.montoVendido<0)
                                            printf("El monto debe ser positivo.\n");
                                    }while (aux.montoVendido<0);
                                    fflush(stdin);

                                    do{
                                        printf("Ingrese la cantidad de productos vendidos: ");
                                        scanf("%d",&aux.cantidadVendida);
                                        if (aux.cantidadVendida<0)
                                            printf("La cantidad debe ser positiva.\n");
                                    }while (aux.cantidadVendida<0);
                                    fflush(stdin);

                                    printf("Ingrese el canal de venta: ");
                                    fgets(aux.canalDeVenta,102,stdin);
                                    arreglarString(aux.canalDeVenta);
                                    fflush(stdin);

                                    printf("\n");
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
                                    printf("Ingrese el documento del vendedor: \n");
                                    //fflush(stdin);
                                    scanf("%d",&auxiliar);
                                    //fflush(stdin);
                                    printf("\n");
                                    switch (bajaBB(auxiliar))
                                    {
                                        case 1: printf("Eliminado con exito.\n");
                                                break;

                                        case 0: printf("No se encontro un vendedor con ese documento.\n");
                                                 break;

                                        case -1: printf("Eliminacion cancelada.\n");

                                        default: break;
                                    }
                                    printf("\n");
                                    break;

                            case 3: //Evocación+Mostrar.
                                    printf("Ingrese el documento del vendedor: \n");
                                    //fflush(stdin);
                                    scanf("%d",&auxiliar);
                                    //fflush(stdin);
                                    printf("\n");
                                    if (evocacionBB(auxiliar,&aux))
                                    {
                                        printf("\n");
                                        show(aux);
                                        printf("\n");
                                    }
                                    else
                                        printf("No se encontro al vendedor.\n");
                                    printf("\n");
                                    break;

                            case 4: // Pertenencia.
                                    printf("Ingrese el documento del vendedor: \n");
                                    //fflush(stdin);
                                    scanf("%d",&auxiliar);
                                    //fflush(stdin);
                                    printf("\n");
                                    if (pertenenciaBB(auxiliar))
                                        printf("El vendedor esta en el registro.\n");
                                    else
                                        printf("El vendedor no esta en el registro.\n");
                                    printf("\n");
                                    break;

                            case 5: // Mostrar.
                                    mostrarLSOBT();
                                    break;

                            case 6: // Memorizacion Previa.
                                    printf("\n");
                                    switch(memorizacion_previaLSOBT(&auxiliar))
                                    {
                                        case 1: printf("Memorizacion previa exitosa.\n");
                                                break;
                                        case -1: printf("Error al abrir el archivo.\n");
                                                 break;
                                        case 0: printf("No se pudo cargar desde el documento %d en adelante porque no queda espacio disponible.\n",auxiliar);
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
