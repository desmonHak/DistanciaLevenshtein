#include <windows.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Min(x, y) ((x) < (y) ? (x) : (y))
#define Min3(x, y, z) Min(Min((x), (y)), (z))

// Función para calcular la distancia de Levenshtein entre dos cadenas
int Levenshtein(const char *str1, const char *str2, int caseSensitive) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 == 0) return len2;
    if (len2 == 0) return len1;

    if (len1 > len2) {
        const char *temp = str1;
        str1 = str2;
        str2 = temp;
        size_t tempLen = len1;
        len1 = len2;
        len2 = tempLen;
    }

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

    int *currentRow = (int *)malloc((len1 + 1) * sizeof(int));
    for (size_t i = 0; i <= len1; ++i) {
        currentRow[i] = i;
    }

    for (size_t j = 1; j <= len2; ++j) {
        int previousDiagonal = currentRow[0];
        currentRow[0] = j;

        for (size_t i = 1; i <= len1; ++i) {
            int substitutionCost =
                previousDiagonal + (processedStr1[i - 1] != processedStr2[j - 1]);
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
    free(currentRow);
    if (!caseSensitive) {
        free((char *)processedStr1);
        free((char *)processedStr2);
    }

    return result;
}

float getAfinidad(size_t sizeof_str1, size_t sizeof_str2, size_t distancia_Levenshtein) {
    float longitud = sizeof_str1 > sizeof_str2 ? sizeof_str1 : sizeof_str2;
    return 1 - ((float)distancia_Levenshtein / longitud);
}

// Estructura para pasar datos a los hilos
typedef struct ThreadData {
    const char *s1;                                 // palabra contra la que comprobar
    const char* (*get_s2)(void*);                   // función para obtener el string de un array
    void* data_s2_arr;                              // array con valores de cualquier tipo
    void* (*get_next_element_arr)(void*, size_t i); // función para obtener el siguiente elemento del array
    size_t startIndex;                              // índice inicial para este hilo
    size_t endIndex;                                // índice final para este hilo
    void (*work_vals)(int, float, void*);           // función para procesar valores calculados
} ThreadData;

// Función que ejecutará cada hilo
void* threadFunction(void* arg) {
    ThreadData* threadData = (ThreadData*)arg;

    for (size_t i = threadData->startIndex; i < threadData->endIndex; ++i) {
        void* element = threadData->get_next_element_arr(threadData->data_s2_arr, i);
        const char *s2 = threadData->get_s2(element);

        int distancia = Levenshtein(threadData->s1, s2, 0);
        float afinidad =
            getAfinidad(strlen(threadData->s1), strlen(s2), distancia);

        threadData->work_vals(distancia, afinidad, element);
    }

    return NULL;
}

// Función principal CallArr con soporte multihilo
void CallArr(
    const char* s1,
    const char* (*get_s2)(void*),
    void* data_s2_arr,
    void* (*get_next_element_arr)(void*, size_t i),
    size_t size_elements_in_data_s2_arr,
    void (*work_vals)(int, float, void*)
) {
    int numThreads = 2; // Número de hilos a utilizar
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    size_t elementsPerThread =
        size_elements_in_data_s2_arr / numThreads;

    for (int t = 0; t < numThreads; ++t) {
        threadData[t].s1 = s1;
        threadData[t].get_s2 = get_s2;
        threadData[t].data_s2_arr = data_s2_arr;
        threadData[t].get_next_element_arr =
            get_next_element_arr;
        threadData[t].work_vals =
            work_vals;

        threadData[t].startIndex =
            t * elementsPerThread;

        threadData[t].endIndex =
            t == numThreads - 1 ? size_elements_in_data_s2_arr : ((t + 1) * elementsPerThread);

        pthread_create(&threads[t], NULL, threadFunction, &threadData[t]);
    }

    for (int t = 0; t < numThreads; ++t)
       pthread_join(threads[t], NULL);
}


typedef struct Example_struct_complex_data {
    char* string;
    float afinidad;
} Example_struct_complex_data;

int main() {
    const char *s1 = "identificar";
    const char *s2 = "identify";
    
    int distancia = Levenshtein(s1, s2, 1);
    float afinidad = getAfinidad(strlen(s1), strlen(s2), distancia);
    
    printf("Distancia: %d\nAfinidad: %.4f%%\n", 
           distancia, 
           afinidad * 100);
           Example_struct_complex_data data[] = {
            { .string = "config_path", .afinidad = 0 },
            { .string = "log_level", .afinidad = 0 },
            { .string = "max_connections", .afinidad = 0 },
            { .string = "timeout_duration", .afinidad = 0 },
            { .string = "enable_cache", .afinidad = 0 },
            { .string = "cache_size", .afinidad = 0 },
            { .string = "thread_count", .afinidad = 0 },
            { .string = "db_host", .afinidad = 0 },
            { .string = "db_port", .afinidad = 0 },
            { .string = "db_username", .afinidad = 0 },
            { .string = "db_password", .afinidad = 0 },
            { .string = "api_key", .afinidad = 0 },
            { .string = "api_secret", .afinidad = 0 },
            { .string = "session_timeout", .afinidad = 0 },
            { .string = "retry_attempts", .afinidad = 0 },
            { .string = "retry_delay", .afinidad = 0 },
            { .string = "log_file_path", .afinidad = 0 },
            { .string = "temp_directory", .afinidad = 0 },
            { .string = "backup_interval", .afinidad = 0 },
            { .string = "enable_logging", .afinidad = 0 },
            { .string = "default_language", .afinidad = 0 },
            { .string = "supported_languages", .afinidad = 0 },
            { .string = "date_format", .afinidad = 0 },
            { .string = "time_zone", .afinidad = 0 },
            { .string = "max_upload_size", .afinidad = 0 },
            { .string = "allowed_file_types", .afinidad = 0 },
            { .string = "maintenance_mode", .afinidad = 0 },
            { .string = "admin_email", .afinidad = 0 },
            { .string = "smtp_server", .afinidad = 0 },
            { .string = "smtp_port", .afinidad = 0 },
            { .string = "smtp_username", .afinidad = 0 },
            { .string = "smtp_password", .afinidad = 0 },
            { .string = "enable_ssl", .afinidad = 0 },
            { .string = "oauth_client_id", .afinidad = 0 },
            { .string = "oauth_client_secret", .afinidad = 0 },
            { .string = "session_cookie_name", .afinidad = 0 },
            { .string = "csrf_token", .afinidad = 0 },
            { .string = "password_salt", .afinidad = 0 },
            { .string = "encryption_key", .afinidad = 0 },
            { .string = "enable_debug_mode", .afinidad = 0 },
            { .string = "static_files_path", .afinidad = 0 },
            { .string = "templates_path", .afinidad = 0 },
            { .string = "enable_compression", .afinidad = 0 },
            { .string = "compression_algorithm", .afinidad = 0 },
            { .string = "default_currency", .afinidad = 0 },
            { .string = "payment_gateway_url", .afinidad = 0 },
            { .string = "enable_two_factor_auth", .afinidad = 0 },
            { .string = "max_login_attempts", .afinidad = 0 },
            { .string = "account_lockout_duration", .afinidad = 0 },
            { .string = "password_reset_token_expiry", .afinidad = 0 }
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

        //printf("Distancia: %d, Afinidad: %.4f%%. %s\n", 
        //    distancia, 
        //    data->afinidad * 100,
        //    data->string
        //);
    }
    
    // Variables para almacenar los contadores de inicio y fin
    LARGE_INTEGER start, end, frequency;

    // Obtener la frecuencia del contador de alta precisión
    QueryPerformanceFrequency(&frequency);

    // Obtener el contador inicial
    QueryPerformanceCounter(&start);
    CallArr(
        "projet_name", 
        get_string, 
        &data, get_next_element_arr, 
        sizeof(data)/sizeof(Example_struct_complex_data),
        work_vals
    );
    // Obtener el contador final
    QueryPerformanceCounter(&end);

    // Calcular el tiempo transcurrido en segundos
    double elapsedTimeSeconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    long long elapsedTimeNanoseconds = (long long)(elapsedTimeSeconds * 1e9);


    // Mostrar el tiempo transcurrido
    printf("Tiempo de ejecucion: %.6f segundos o %lld ns\n", elapsedTimeSeconds, elapsedTimeNanoseconds);

    puts("exit...");
    return 0;
}
