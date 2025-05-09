#ifndef LEVENSHTEIN_C
#define LEVENSHTEIN_C

#include "Levenshtein_c.h"

int LevenshteinUnicode(const wchar_t *str1, const wchar_t *str2, int caseSensitive) {
    size_t len1 = wcslen(str1);
    size_t len2 = wcslen(str2);

    // Casos base para cadenas vacías
    if (len1 == 0) return (int)len2;
    if (len2 == 0) return (int)len1;

    // Procesamiento de mayúsculas/minúsculas si no es sensible a mayúsculas
    wchar_t *processedStr1 = (wchar_t *)str1;
    wchar_t *processedStr2 = (wchar_t *)str2;
    wchar_t *lowerStr1 = NULL;
    wchar_t *lowerStr2 = NULL;

    if (!caseSensitive) {
        lowerStr1 = (wchar_t *)malloc((len1 + 1) * sizeof(wchar_t));
        lowerStr2 = (wchar_t *)malloc((len2 + 1) * sizeof(wchar_t));

        for (size_t i = 0; i < len1; ++i)
            lowerStr1[i] = towlower(str1[i]);
        lowerStr1[len1] = L'\0';

        for (size_t i = 0; i < len2; ++i)
            lowerStr2[i] = towlower(str2[i]);
        lowerStr2[len2] = L'\0';

        processedStr1 = lowerStr1;
        processedStr2 = lowerStr2;
    }

    // Inicializar matriz dinámica para calcular la distancia
    int *currentRow = (int *)malloc((len1 + 1) * sizeof(int));

    for (size_t i = 0; i <= len1; ++i)
        currentRow[i] = (int)i;

    for (size_t j = 1; j <= len2; ++j) {
        int previousDiagonal = currentRow[0];
        currentRow[0] = (int)j;

        for (size_t i = 1; i <= len1; ++i) {
            int substitutionCost =
                previousDiagonal + ((processedStr1[i - 1] != processedStr2[j - 1]) ? 1 : 0);
            int insertionCost = currentRow[i - 1] + 1;
            int deletionCost = currentRow[i] + 1;

            previousDiagonal = currentRow[i];
            currentRow[i] =
                substitutionCost < insertionCost ? substitutionCost : insertionCost;
            currentRow[i] =
                currentRow[i] < deletionCost ? currentRow[i] : deletionCost;
        }
    }

    int result = currentRow[len1];

    // Liberar memoria dinámica
    free(currentRow);
    if (!caseSensitive) {
        free(lowerStr1);
        free(lowerStr2);
    }

    return result;
}

int Levenshtein(const char *str1, const char *str2, int caseSensitive) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    // Casos base para cadenas vacías
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;

    // Optimización: usar siempre la cadena más corta como fila
    if (len1 > len2) {
        const char *temp = str1;
        str1 = str2;
        str2 = temp;
        size_t tempLen = len1;
        len1 = len2;
        len2 = tempLen;
    }

    // Si no se tiene en cuenta las mayúsculas/minúsculas, convertir las cadenas a minúsculas
    const char *processedStr1 = str1;
    const char *processedStr2 = str2;

    if (!caseSensitive) {
        char *lowerStr1 = (char *)malloc(len1 + 1);
        char *lowerStr2 = (char *)malloc(len2 + 1);

        for (size_t i = 0; i < len1; ++i) lowerStr1[i] = tolower(str1[i]);
        lowerStr1[len1] = '\0';
        for (size_t i = 0; i < len2; ++i) lowerStr2[i] = tolower(str2[i]);
        lowerStr2[len2] = '\0';

        processedStr1 = lowerStr1;
        processedStr2 = lowerStr2;
    }

    // Usamos un solo array para almacenar los costos actuales y anteriores
    int *currentRow = (int *)malloc((len1 + 1) * sizeof(int));

    // Inicializamos la primera fila (costos para transformar una cadena vacía)
    for (size_t i = 0; i <= len1; ++i) {
        currentRow[i] = i;
    }

    // Iteramos sobre cada carácter de la cadena objetivo
    for (size_t j = 1; j <= len2; ++j) {
        int previousDiagonal = currentRow[0]; // Costo diagonal anterior
        currentRow[0] = j;                    // Costo inicial para esta fila

        // Iteramos sobre cada carácter de la cadena fuente
        for (size_t i = 1; i <= len1; ++i) {
            int substitutionCost =
                previousDiagonal + (processedStr1[i - 1] != processedStr2[j - 1]);
            int insertionCost = currentRow[i - 1] + 1;
            int deletionCost = currentRow[i] + 1;

            // Calculamos el costo mínimo entre las tres operaciones posibles
            previousDiagonal = currentRow[i];
            currentRow[i] =
                substitutionCost < insertionCost ? substitutionCost : insertionCost;
            currentRow[i] =
                currentRow[i] < deletionCost ? currentRow[i] : deletionCost;
        }
    }

    // El último valor en el array representa la distancia de Levenshtein
    int result = currentRow[len1];

    // Liberamos memoria dinámica utilizada
    free(currentRow);
    if (!caseSensitive) {
        free((char *)processedStr1);
        free((char *)processedStr2);
    }

    return result;
}

float getAfinidad(
    size_t sizeof_str1,
    size_t sizeof_str2, 
    size_t distancia_Levenshtein
) {
    float longitud = sizeof_str1 > sizeof_str2 ? sizeof_str1 : sizeof_str2;
    return 1 - ((float)distancia_Levenshtein / longitud);
}


void CallArr(
    const char* s1,                                 // palabra contra la que comprobar
    const char* (*get_s2)(void*),                   // funcion para obtener el string de un array
    void* data_s2_arr,                              // array con valores de cualquier tipo
    void* (*get_next_element_arr)(void*, size_t i), // obtener el siguiente elemento del array
    size_t size_elements_in_data_s2_arr,            // numero de elementos de un array
    void (*work_vals)(int, float, void*)            // int distancia, float afinidad, void*element
) {
    for (int i = 0; i < size_elements_in_data_s2_arr; i++) {
        void* element = get_next_element_arr(data_s2_arr, i);

        // obtener los datos del puntero data_s2 como el usuario quiera
        const char *s2 = get_s2(element);
        int distancia = Levenshtein(s1, s2, 0);
        work_vals(
            distancia, getAfinidad(strlen(s1), strlen(s2), distancia),
            element
        );
        
    }
}

void CallArrUnicode(
    const wchar_t* s1,                              // palabra contra la que comprobar
    const wchar_t* (*get_s2)(void*),                // funcion para obtener el string de un array
    void* data_s2_arr,                              // array con valores de cualquier tipo
    void* (*get_next_element_arr)(void*, size_t i), // obtener el siguiente elemento del array
    size_t size_elements_in_data_s2_arr,            // numero de elementos de un array
    void (*work_vals)(int, float, void*)            // int distancia, float afinidad, void*element
) {
    for (int i = 0; i < size_elements_in_data_s2_arr; i++) {
        void* element = get_next_element_arr(data_s2_arr, i);

        // obtener los datos del puntero data_s2 como el usuario quiera
        wchar_t *s2 = get_s2(element);
        int distancia = LevenshteinUnicode(s1, s2, 0);
        work_vals(
            distancia, getAfinidad(strlen(s1), strlen(s2), distancia),
            element
        );
        
    }
}

#endif