#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#ifdef _WIN32
    #include <windows.h>
#endif

// para unicode y codificaciones:
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define Min(x,y) ((x)<(y) ? (x) : (y))
#define Min3(x,y,z) Min(Min((x),(y)),(z))

int LevenshteinUnicode(const wchar_t *str1, const wchar_t *str2, int caseSensitive);
int Levenshtein(const char *str1, const char *str2, int caseSensitive);
float getAfinidad(
    size_t sizeof_str1,
    size_t sizeof_str2, 
    size_t distancia_Levenshtein
);
void CallArr(
    const char* s1,                                 // palabra contra la que comprobar
    const char* (*get_s2)(void*),                   // funcion para obtener el string de un array
    void* data_s2_arr,                              // array con valores de cualquier tipo
    void* (*get_next_element_arr)(void*, size_t i), // obtener el siguiente elemento del array
    size_t size_elements_in_data_s2_arr,            // numero de elementos de un array
    void (*work_vals)(int, float, void*)            // int distancia, float afinidad, void*element
);

void CallArrUnicode(
    const wchar_t* s1,                              // palabra contra la que comprobar
    const wchar_t* (*get_s2)(void*),                // funcion para obtener el string de un array
    void* data_s2_arr,                              // array con valores de cualquier tipo
    void* (*get_next_element_arr)(void*, size_t i), // obtener el siguiente elemento del array
    size_t size_elements_in_data_s2_arr,            // numero de elementos de un array
    void (*work_vals)(int, float, void*)            // int distancia, float afinidad, void*element
);

#endif