#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <locale.h>

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
