// Eric Omar Espinosa de los Monteros Martinez
#include <stdio.h>
#include <stdlib.h>

// Defino la estructura que guardara los estados
struct estado
{
    char nombre;
    int aceptacion; // igual a 0 si no es de aceptacion e igual a 1 si es de aceptacion
    int incial;     // igual a 1 si es inicial e igual a 0 si no es incial
};

// Defino la estructura que conformara al automata donde se guardan las transiciones entre cada estado
struct transiciones
{
    struct estado Origen;
    struct estado Destino;
    char transicion;
} *automata, *auxi, *auxi2; // Se crea un arreglo de transiciones

char *cadenaVacia = NULL; // Guardara '1' si el automata acepta la cadena vacia y '0' si no la acepta

int recorrerCadena(char *cadenaValidar, int numeroTransiciones, char destino);

int validadorCadena(char *cadenaValidar, int numeroTransiciones, int tamanoCadenaValidar);

// Esta funcion guarda las transiciones del automata en la estructura de transiciones
void crearAutomata(int numeroTransiciones, int tamanoCadenaAceptacion, char *cadenaTransiciones, char *cadenaEstadosAceptacion, char *estadoInicial);

int main()
{
    FILE *archivo;
    char *cadenaTransiciones = NULL;      // Cadena que guardara las transiciones leidas del archivo txt
    int caracteresLeidosCT;               // Longitud del string cadenaTransiciones
    char *cadenaEstadosAceptacion = NULL; // Cadena que guardara los estados de transiciones
    int caracteresLeidosCEA;              // Longitud del string cadenaEstadosAceptacion leidos del archivo txt
    char *estadoInicial = NULL;           // Guarda el estado inicial leido del archgivo txt
    int caracteresEI;
    char *cadenaValidar = NULL; // Contiene la cadena que va a validara el automata
    int caracteresLeidosCV;
    int caracteresLeidosCVA;

    size_t longitud1 = 0; // Variable para indicar a la funcion getline que trabaje con memoria dinamica
    int aceptado;         // Variable para guardar el retorno de la funcion validarCadena
    // Lee la cadena a validar
    printf("\nIngrese la cadena a validar: ");
    caracteresLeidosCV = getline(&cadenaValidar, &longitud1, stdin);
    if (caracteresLeidosCV == -1)
    {
        printf("\nNo se pudo leer la cadena a validar\n");
        return 1;
    }

    // Abrir el archivo
    archivo = fopen("automata.txt", "r");
    if (archivo == NULL)
    {
        printf("\nNo se pudo abrir el archivo\n");
        return 1;
    }
    // Lee la cadena de transiciones del archivo txt
    caracteresLeidosCT = getline(&cadenaTransiciones, &longitud1, archivo);
    if (caracteresLeidosCT == -1)
    {
        printf("\nNo se pudieron leer las transiciones\n");
        return 1;
    }
    // Lee los estados de aceptacion del arhivo txt
    caracteresLeidosCEA = getline(&cadenaEstadosAceptacion, &longitud1, archivo);
    if (caracteresLeidosCEA == -1)
    {
        printf("\nNo se pudieron leer los estados de aceptacion\n");
        return 1;
    }
    // Lee el estado incial del archivo txt
    caracteresEI = getline(&estadoInicial, &longitud1, archivo);
    if (caracteresEI == -1)
    {
        printf("\nNo se pudo leer el estado inicial\n");
        return 1;
    }
    caracteresLeidosCVA = getline(&cadenaVacia, &longitud1, archivo);
    if (caracteresLeidosCVA == -1)
    {
        printf("\nNo se pudo leer si acepta la cadena vacia");
        return 1;
    }

    // Cerrar el archivo
    fclose(archivo);

    // Imprimir las líneas leídas
    printf("\nLas transiciones son: %s\n", cadenaTransiciones);
    printf("Los estados de aceptacion son: %s\n", cadenaEstadosAceptacion);
    printf("El estado incial es: %s\n\n", estadoInicial);

    int numeroDeTransiciones = caracteresLeidosCT / 4;

    automata = malloc(sizeof(struct transiciones) * numeroDeTransiciones); // Se le asigna la memoria al arreglo de transiciones
    crearAutomata(numeroDeTransiciones, caracteresLeidosCEA, cadenaTransiciones, cadenaEstadosAceptacion, estadoInicial);

    aceptado = validadorCadena(cadenaValidar, numeroDeTransiciones, caracteresLeidosCV);
    if (aceptado == 0)
    {
        printf("\033[32m"); // establece el color verde
        printf("\nLa cadena es aceptada por el automata\n\n");
        printf("\033[0m"); // restablece el color por defecto
    }
    else
    {
        printf("\033[31m"); // establece el color rojo
        printf("\nLa cadena no es aceptada por el automata\n\n");
        printf("\033[0m"); // restablece el color por defecto
    }
    //  Liberar el búfer dinámico
    free(cadenaTransiciones);
    printf("\n");
    system("pause");

    return 0;
} // Fin de la funcion main

int recorrerCadena(char *cadenaValidar, int numeroTransiciones, char destino)
{
    int contador = 1;
    auxi2 = auxi; // Guarda el estado actual, para lo comprobacion de si ya termino la cadena d evalidacion
    auxi = automata;
    // Buscando un estado que conincida con el destino y la cadena a validar
    while ((contador <= numeroTransiciones) && ((auxi->transicion != (*cadenaValidar)) || (auxi->Origen.nombre != destino)))
    {
        auxi++;
        contador++;
    }
    if ((*cadenaValidar) == '\n') // Se llego al final de la cadena a validar
    {
        if (auxi2->Destino.aceptacion == 1)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        if ((contador <= numeroTransiciones) == 0)
        { // Si ya se recorrieron todas las transiciones y no se encotro un estado de incio que coincida con la cadena
            return -1;
        }
        else // Aun falta recorrer la cadena
        {
            cadenaValidar++;
            recorrerCadena(cadenaValidar, numeroTransiciones, auxi->Destino.nombre);
        }
    }
}

int validadorCadena(char *cadenaValidar, int numeroTransiciones, int tamanoCadenaValidar)
{
    int contador = 1;
    int salida;
    auxi = automata;
    if (((*cadenaValidar) == '\n') && (*cadenaVacia == '1') && (tamanoCadenaValidar == 1)) //En caso de que se el AFD acepte la cadena vacia
    {
        return 0;
    }
    else
    {
        // Buscando el estado de incio que coincida con la cadena a validar
        while ((contador <= numeroTransiciones) && ((auxi->Origen.incial != 1) || (auxi->transicion != (*cadenaValidar))))
        {
            auxi++;
            contador++;
        }
        if ((contador <= numeroTransiciones) == 0)
        { // Si ya se recorrieron todas las transiciones y no se encotro un estado de incio que coincida con la cadena
            return -1;
        }
        else // if (auxi->Origen.incial == 1)
        {    // Existe un estado de incio que coincide con la transicion de la cadena a validar
            cadenaValidar++;
            if ((*cadenaValidar) != '\n')
            {
                salida = recorrerCadena(cadenaValidar, numeroTransiciones, auxi->Destino.nombre);
                return salida;
            }
            else
            {
                return 0;
            }
        }
    }
}

// Esta funcion guarda las transiciones del automata en la estructura de transiciones
void crearAutomata(int numeroTransiciones, int tamanoCadenaAceptacion, char *cadenaTransiciones, char *cadenaEstadosAceptacion, char *estadoInicial)
{
    auxi = automata;
    int i;
    for (i = 0; i < numeroTransiciones; i++) // Recorre cada parte del arreglo de la variable automata y lo llena de valores
    {
        automata->Origen.nombre = *cadenaTransiciones;
        cadenaTransiciones++;
        automata->transicion = *cadenaTransiciones;
        cadenaTransiciones++;
        automata->Destino.nombre = *cadenaTransiciones;
        automata->Destino.aceptacion = 0;
        automata->Origen.aceptacion = 0;
        automata->Destino.incial = 0;
        automata->Origen.incial = 0;
        cadenaTransiciones += 2;
        automata++;
    }
    automata = auxi;
    int len1 = tamanoCadenaAceptacion - 1;
    int len2 = numeroTransiciones;
    for (i = 0; i < len1 * len2; i++)
    {
        if (cadenaEstadosAceptacion[i % len1] == automata[i % len2].Origen.nombre)
            automata[i % len2].Origen.aceptacion = 1; // Si la cadena de aceptacion coincide con el estado de origen se asigna 1 a la variable de aceptacion
        if (cadenaEstadosAceptacion[i % len1] == automata[i % len2].Destino.nombre)
            automata[i % len2].Destino.aceptacion = 1; // Si la cadena de aceptacion coincide con el estado de destino se asigna 1 a la variable de aceptacion
    }
    for (i = 0; i < numeroTransiciones; i++)
    {
        if ((*estadoInicial) == automata[i].Origen.nombre)
            automata[i].Origen.incial = 1;
        if ((*estadoInicial) == automata[i].Destino.nombre)
            automata[i].Destino.incial = 1;
    }
}