#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>

/* Función para calcular la distancia de Levenshtein entre dos cadenas Unicode
 * Parámetros:
 *   str1: Primera cadena (wide string)
 *   str2: Segunda cadena (wide string)
 *   caseSensitive: 1 para distinguir mayúsculas/minúsculas, 0 para ignorar
 * Retorno:
 *   Entero con la distancia de edición mínima
 */
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
int main() {
    // Configurar locale para soportar caracteres Unicode
    setlocale(LC_ALL, "en_US.UTF-8");

    // Ejemplo con caracteres japoneses
    const wchar_t *japanese_str1 = L"こんにちは"; // "Hola" en japonés
    const wchar_t *japanese_str2 = L"こんばんは"; // "Buenas noches" en japonés

    // Ejemplo con caracteres franceses
    const wchar_t *french_str1 = L"notre";
    const wchar_t *french_str2 = L"nôtre";

    // Ejemplo con caracteres árabes
    const wchar_t *arabic_str1 = L"سلام";
    const wchar_t *arabic_str2 = L"عليكم";

    // Ejemplo con caracteres chinos
    const wchar_t *chinese_str1 = L"你好"; // "Hola" en chino
    const wchar_t *chinese_str2 = L"您好"; // "Hola formal" en chino

    // Calcular distancias
    int dist_japanese = LevenshteinUnicode(japanese_str1, japanese_str2, 0);
    int dist_french = LevenshteinUnicode(french_str1, french_str2, 0);
    int dist_arabic = LevenshteinUnicode(arabic_str1, arabic_str2, 0);
    int dist_chinese = LevenshteinUnicode(chinese_str1, chinese_str2, 0);

    // Mostrar resultados
    wprintf(L"Distancia entre cadenas japonesas: %d\n", dist_japanese);
    wprintf(L"Distancia entre cadenas francesas: %d\n", dist_french);
    wprintf(L"Distancia entre cadenas árabes: %d\n", dist_arabic);
    wprintf(L"Distancia entre cadenas chinas: %d\n", dist_chinese);

    return 0;
}
