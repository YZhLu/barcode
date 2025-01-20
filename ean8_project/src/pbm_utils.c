#include "pbm_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PBMImage *load_pbm(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
        return NULL;
    }

    char format[3];
    if (fscanf(file, "%2s", format) != 1 || strcmp(format, "P4") != 0) {
        fprintf(stderr, "Arquivo não é um PBM válido.\n");
        fclose(file);
        return NULL;
    }

    PBMImage *image = malloc(sizeof(PBMImage));
    if (!image) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d %d", &image->width, &image->height) != 2) {
        fprintf(stderr, "Erro ao ler dimensões da imagem.\n");
        free(image);
        fclose(file);
        return NULL;
    }

    fgetc(file); // Consumir o caractere de nova linha após o cabeçalho

    size_t data_size = image->width * image->height / 8;
    image->data = malloc(data_size);
    if (!image->data) {
        fprintf(stderr, "Erro ao alocar memória para os dados.\n");
        free(image);
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(image->data, 1, data_size, file);
    if (bytes_read != data_size) {
        fprintf(stderr, "Erro ao ler os dados da imagem.\n");
        free(image->data);
        free(image);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return image;
}

bool save_pbm(const char *filename, const PBMImage *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Erro ao criar o arquivo %s.\n", filename);
        return false;
    }

    fprintf(file, "P1\n%d %d\n", image->width, image->height);
    size_t data_size = image->width * image->height * sizeof(char);
    fwrite(image->data, 1, data_size, file);
    fclose(file);
    return true;
}

void free_pbm(PBMImage *image) {
    if (image) {
        free(image->data);
        free(image);
    }
}
