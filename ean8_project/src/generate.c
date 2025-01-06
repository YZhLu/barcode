#include "ean8_utils.h"
#include "pbm_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_WIDTH 200
#define DEFAULT_HEIGHT 100
#define DEFAULT_SPACING 10

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <identificador> [opções]\n", argv[0]);
        return 1;
    }

    char *identifier = argv[1];
    if (!validate_ean8(identifier)) {
        fprintf(stderr, "Identificador EAN-8 inválido.\n");
        return 1;
    }

    int width = (argc > 2) ? atoi(argv[2]) : DEFAULT_WIDTH;
    int height = (argc > 3) ? atoi(argv[3]) : DEFAULT_HEIGHT;

    PBMImage image = {width, height, calloc(width * height / 8, 1)};
    if (!image.data) {
        fprintf(stderr, "Erro ao alocar memória para a imagem.\n");
        return 1;
    }

    int bars[83]; // Número total de barras no EAN-8
    ean8_to_bars(identifier, bars);

    // Lógica para desenhar barras na imagem
    int bar_width = (width - (DEFAULT_SPACING * 2)) / 83; // Ajuste da largura da barra
    int bar_height = height; // A altura das barras será igual à altura da imagem

    // Adiciona espaçamento à esquerda
    int x_offset = DEFAULT_SPACING;

    for (int i = 0; i < 83; i++) {
        if (bars[i] == 1) { // Se a barra estiver ativa (1)
            // Desenha a barra no formato PBM
            for (int j = 0; j < bar_width; j++) {
                // Para cada posição horizontal da barra
                for (int k = 0; k < bar_height; k++) {
                    // Calcula o índice do byte e a posição do bit para cada pixel
                    int pixel_index = (k * width + (x_offset + j)) / 8;
                    int bit_position = 7 - ((x_offset + j) % 8); // Posição do bit dentro do byte

                    // Ativa o bit (definindo como 1)
                    image.data[pixel_index] |= (1 << bit_position);
                }
            }
        }
        // Move para a próxima barra
        x_offset += bar_width;
    }

    if (!save_pbm("output.pbm", &image)) {
        fprintf(stderr, "Erro ao salvar a imagem.\n");
        free_pbm(&image);
        return 1;
    }

    free_pbm(&image);
    return 0;
}
