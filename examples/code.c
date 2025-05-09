#include "Levenshtein_c.h"

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

        #ifdef MOSTRAR_OPERACIONES
        printf("Distancia: %d, Afinidad: %.4f%%. %s\n", 
            distancia, 
            data->afinidad * 100,
            data->string
        );
        #endif
    }


    void work_vals1(int distancia, float afinidad, void* element) {
        Example_struct_complex_data* data = (Example_struct_complex_data*)element;
        data->afinidad = afinidad;

        printf("Distancia: %d, Afinidad: %.4f%%. %s\n", 
            distancia, 
            data->afinidad * 100,
            data->string
        );
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
    printf("Buscando password_alt: ");
    CallArr(
        "password_alt", 
        get_string, 
        &data, get_next_element_arr, 
        sizeof(data)/sizeof(Example_struct_complex_data),
        work_vals1
    );


    // Calcular el tiempo transcurrido en segundos
    double elapsedTimeSeconds = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    long long elapsedTimeNanoseconds = (long long)(elapsedTimeSeconds * 1e9);


    // Mostrar el tiempo transcurrido
    printf("Tiempo de ejecucion: %.6f segundos o %lld ns\n", elapsedTimeSeconds, elapsedTimeNanoseconds);

    puts("exit...");
    return 0;
}
