#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Min(x,y) ((x)<(y) ? (x) : (y))
#define Min3(x,y,z) Min(Min((x),(y)),(z))

int Levenshtein(const char *s1, const char *s2) {
    size_t t1 = strlen(s1), t2 = strlen(s2);
    if (t1 == 0) return t2;
    if (t2 == 0) return t1;

    // Usamos arreglos dinámicos para dos filas
    int *prev = malloc((t1+1) * sizeof(int));
    int *curr = malloc((t1+1) * sizeof(int));

    for (size_t i=0; i<=t1; ++i) prev[i] = i;

    for (size_t j=1; j<=t2; ++j) {
        curr[0] = j;
        for (size_t i=1; i<=t1; ++i) {
            /*
            // si no queremos hacer distincion entre masyusculas y minusculas
            int costo = (tolower(s1[i-1]) != tolower(s2[j-1]));
            */
            int costo = (tolower(s1[i-1]) != tolower(s2[j-1]));


            /*
            // si queremos hacer distincion entre mayusculas y minusculas:
            if (s1[i-1] == s2[j-1]) {
                costo = 0; // Los caracteres son iguales
            } else {
                costo = 1; // Los caracteres son diferentes
            }
            */
            curr[i] = Min3(
                prev[i] + 1,      // Inserción
                curr[i-1] + 1,    // Eliminación
                prev[i-1] + costo // Sustitución
            );
        }
        // Swap pointers
        int *temp = prev;
        prev = curr;
        curr = temp;
    }

    int res = prev[t1];
    free(prev); free(curr);
    return res;
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
        int distancia = Levenshtein(s1, s2);
        work_vals(
            distancia, getAfinidad(strlen(s1), strlen(s2), distancia),
            element
        );
        
    }

}

typedef struct Example_struct_complex_data {
    char* string;
    float afinidad;
} Example_struct_complex_data;

int main() {
    const char *s1 = "identificar";
    const char *s2 = "identify";
    
    int distancia = Levenshtein(s1, s2);
    float afinidad = getAfinidad(strlen(s1), strlen(s2), distancia);
    
    printf("Distancia: %d\nAfinidad: %.4f%%\n", 
           distancia, 
           afinidad * 100);


    Example_struct_complex_data data[] = {
        {
            .string = "Hola",
            .afinidad = 0
        },
        {
            .string = "Hello",
            .afinidad = 0
        },
        {
            .string = "Juan",
            .afinidad = 0
        },
        {
            .string = "hol",
            .afinidad = 0
        },
        {
            .string = "hollllllaaaaaaaaa",
            .afinidad = 0
        }
    };
    const char* get_string(Example_struct_complex_data *data) {
        return data->string;
    }
    void* get_next_element_arr(Example_struct_complex_data *data, size_t i) {
        return &(data[i]);
    }
    void work_vals(int distancia, float afinidad, void* element) {
        Example_struct_complex_data* data = (Example_struct_complex_data*)element;
        data->afinidad = afinidad;

        printf("Distancia: %d, Afinidad: %.4f%%. %s\n", 
            distancia, 
            data->afinidad * 100,
            data->string
        );
    }
    CallArr(
        "hola", 
        get_string, 
        &data, get_next_element_arr, 
        sizeof(data)/sizeof(Example_struct_complex_data),
        work_vals
    );

    puts("exit...");
    return 0;
}
