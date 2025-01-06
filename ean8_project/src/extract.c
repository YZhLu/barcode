#include "ean8_utils.h"
#include "pbm_utils.h"
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_PIXEL_WIDTH 2
#define DEFAULT_SPACING 10

void extract_bars(const PBMImage *image, int *bars, int num_bars, int pixel_width, int spacing) {
    int x_offset = spacing + pixel_width / 2; // Centraliza o cálculo nas barras
    for (int i = 0; i < num_bars; i++) {
        int black_pixel_count = 0;
        for (int x = 0; x < pixel_width; x++) {
            for (int y = 0; y < image->height; y++) {
                int pixel_x = x_offset + i * pixel_width + x;
                int byte_index = (pixel_x + y * image->width) / 8;
                int bit_index = 7 - (pixel_x % 8);
                if (image->data[byte_index] & (1 << bit_index)) {
                    black_pixel_count++;
                }
            }
        }
        // Define se a barra é preta ou branca
        bars[i] = (black_pixel_count >= (pixel_width * image->height / 2)) ? 1 : 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.pbm>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    PBMImage *image = load_pbm(input_file);
    if (!image) {
        fprintf(stderr, "Erro: Arquivo inválido ou não encontrado.\n");
        return 1;
    }

    if (image->width < DEFAULT_PIXEL_WIDTH * 95 + 2 * DEFAULT_SPACING) {
        fprintf(stderr, "Erro: Imagem não parece conter um código de barras válido.\n");
        free_pbm(image);
        return 1;
    }

    int bars[95];
    extract_bars(image, bars, 95, DEFAULT_PIXEL_WIDTH, DEFAULT_SPACING);

    // Reconstrói o identificador a partir das barras
    char identifier[9] = {0}; // 8 dígitos + terminador
    if (!decode_bars_to_ean8(bars, identifier)) {
        fprintf(stderr, "Erro: Não foi possível decodificar o identificador do código de barras.\n");
        free_pbm(image);
        return 1;
    }

    printf("Identificador extraído: %s\n", identifier);

    free_pbm(image);
    return 0;
}
