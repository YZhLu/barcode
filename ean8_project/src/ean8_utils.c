#include "ean8_utils.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

bool validate_ean8(const char *identifier) {
    if (strlen(identifier) != 8) {
        fprintf(stderr, "Identificador deve conter exatamente 8 dígitos.\n");
        return false;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(identifier[i])) {
            fprintf(stderr, "Identificador contém caracteres não numéricos.\n");
            return false;
        }
    }
    char expected_check_digit = calculate_check_digit(identifier);
    if (identifier[7] != expected_check_digit) {
        fprintf(stderr, "Dígito verificador inválido.\n");
        return false;
    }
    return true;
}

char calculate_check_digit(const char *identifier) {
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        int digit = identifier[i] - '0';
        sum += (i % 2 == 0) ? 3*digit : 1 * digit;
    }
    int remainder = sum % 10;
    return (remainder == 0) ? '0' : '0' + (10 - remainder);
}

void ean8_to_bars(const char *identifier, char *bars) {
    // Definição simplificada do padrão EAN-8
    char l_patterns[10][7] = {
        {'0', '0', '0', '1', '1', '0', '1'}, // L-code para '0'
        {'0', '0', '1', '1', '0', '0', '1'}, // L-code para '1'
        {'0', '0', '1', '0', '0', '1', '1'}, // L-code para '2'
        {'0', '1', '1', '1', '1', '0', '1'}, // L-code para '3'
        {'0', '1', '0', '0', '0', '1', '1'}, // L-code para '4'
        {'0', '1', '1', '0', '0', '0', '1'}, // L-code para '5'
        {'0', '1', '0', '1', '1', '1', '1'}, // L-code para '6'
        {'0', '1', '1', '1', '0', '1', '1'}, // L-code para '7'
        {'0', '1', '1', '0', '1', '1', '1'}, // L-code para '8'
        {'0', '0', '0', '1', '0', '1', '1'}  // L-code para '9'
    };

    char r_patterns[10][7] = {
        {'1', '1', '1', '0', '0', '1', '0'}, // R-code para '0'
        {'1', '1', '0', '0', '1', '1', '0'}, // R-code para '1'
        {'1', '1', '0', '1', '1', '0', '0'}, // R-code para '2'
        {'1', '0', '0', '0', '0', '1', '0'}, // R-code para '3'
        {'1', '0', '1', '1', '1', '0', '0'}, // R-code para '4'
        {'1', '0', '0', '1', '1', '1', '0'}, // R-code para '5'
        {'1', '0', '1', '0', '0', '0', '1'}, // R-code para '6'
        {'1', '0', '0', '0', '1', '0', '0'}, // R-code para '7'
        {'1', '0', '0', '1', '0', '0', '0'}, // R-code para '8'
        {'1', '1', '1', '0', '1', '0', '0'}  // R-code para '9'
    };

    // Barra inicial
    bars[0] = '1'; bars[1] = '0'; bars[2] = '1';

    int pos = 3; // Posição atual no array de barras

    // Gerar barras para os primeiros 4 dígitos (L-codes)
    for (int i = 0; i < 4; i++) {
        int digit = identifier[i] - '0';
        for (int j = 0; j < 7; j++) {
            bars[pos] = l_patterns[digit][j];
            pos++;
        }
    }

    // Barra de separação (meio do código de barras)
    bars[pos++] = '0'; bars[pos++] = '1'; bars[pos++] = '0'; bars[pos++] = '1'; bars[pos++] = '0';

    // Gerar barras para os últimos 4 dígitos (R-codes)
    for (int i = 4; i < 8; i++) {
        int digit = identifier[i] - '0';
        for (int j = 0; j < 7; j++) {
            bars[pos++] = r_patterns[digit][j];
        }
    }

    // Barra final
    bars[pos++] = '1'; bars[pos++] = '0'; bars[pos++] = '1';
}

// Função para decodificar as barras em um identificador EAN-8
bool decode_bars_to_ean8(const int *bars, char *identifier) {
    if (!bars || !identifier) {
        return false;
    }

    // Simplificação para fins de demonstração (substitua com lógica completa)
    for (int i = 0; i < 8; i++) {
        // Apenas uma lógica fictícia que precisa ser ajustada conforme a codificação do EAN-8
        identifier[i] = '0' + i; // Substituir pela lógica real de mapeamento
    }

    identifier[8] = '\0'; // Terminador da string
    return true;
}