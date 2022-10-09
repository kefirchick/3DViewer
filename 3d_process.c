#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "3d_process.h"

int scan_obj_size(data_t *data, const char *file_name) {
    FILE *fp = NULL;
    int result = 1;
    data->n_vertexes = 0;
    data->n_facets = 0;

    if ((fp=fopen(file_name, "r")) != NULL) {
        while (1) {
            char str_type[256];
            char str_vals[256];

            fscanf(fp, "%255s", str_type);
            fscanf(fp, "%255[^\n]s", str_vals);
            update_n_values(data, str_type, str_vals);
            if (feof(fp)) break;
        }

        fclose(fp);
    } else {
        result = 0;
    }

    return result;
}

void update_n_values(data_t *data, char *str_type, char *str_vals) {
    char s[256][4];

    if (strcmp(str_type, "v") == 0) {
        data->n_vertexes++;
    } else if (strcmp(str_type, "f") == 0) {
        data->n_facets++;
        int result = sscanf(str_vals, "%255s%255s%255s%255s", s[0], s[1], s[2], s[3]);
        if (result == 3 || result == 4) data->vert_in_facet = result;
    }
}

void struct_malloc(data_t *data) {
    data->vertexes = (double**)calloc(data->n_vertexes + 1, sizeof(double*));
    if (data->vertexes) {
        for (int i = 0; i < data->n_vertexes + 1; i++)
            data->vertexes[i] = (double*)calloc(3, sizeof(double));
    }

    data->facets = (int**)calloc(data->n_facets, sizeof(int*));
    if (data->facets) {
        for (int i = 0; i < data->n_facets; i++)
            data->facets[i] = (int*)calloc(data->vert_in_facet, sizeof(int));
    }
}

void struct_free(data_t *data) {
    for (int i = 0; i < data->n_vertexes + 1; i++)
        free(data->vertexes[i]);
    free(data->vertexes);

    for (int i = 0; i < data->n_facets; i++)
        free(data->facets[i]);
    free(data->facets);
}

int scan_obj(data_t *data, const char *file_name) {
    FILE *fp = NULL;
    int result = 1;

    if ((fp=fopen(file_name, "r")) != NULL) {
        int i_vertex = 1, i_facet = 0;

        while (1) {
            char str_type[256];
            char str_vals[256];

            fscanf(fp, "%255s", str_type);
            fscanf(fp, "%255[^\n]s", str_vals);
            if (strcmp(str_type, "v") == 0) {
                scan_vertex(data, str_vals, i_vertex);
                i_vertex++;
            } else if (strcmp(str_type, "f") == 0) {
                scan_facet(data, str_vals, i_facet);
                i_facet++;
            }
            if (feof(fp)) break;
        }

        fclose(fp);
    } else {
        result = 0;
    }

    return result;
}

int scan_vertex(data_t *data, char *str_vals, int i_vertex) {
    char s[3][256];
    int result = 0;

    sscanf(str_vals, "%255s%255s%255s", s[0], s[1], s[2]);
    for (int j = 0; j < 3; j++)
        result += sscanf(s[j], "%255lf", &(data->vertexes[i_vertex][j]));

    return result == 3;
}

int scan_facet(data_t *data, char *str_vals, int i_facet) {
    char s[4][256];
    int result = 0;

    sscanf(str_vals, "%255s%255s%255s%255s", s[0], s[1], s[2], s[3]);
    for (int j = 0; j < data->vert_in_facet; j++)
        result += sscanf(s[j], "%255d", &(data->facets[i_facet][j]));

    return result == data->vert_in_facet;
}

void s21_move(data_t *data, double dx, double dy, double dz) {
    for (int i = 1; i < data->n_vertexes + 1; i++) {
        data->vertexes[i][0] += dx;
        data->vertexes[i][1] += dy;
        data->vertexes[i][2] += dz;
    }
}

void turn_x(data_t *data, double wx) {
    for (int i = 1; i < data->n_vertexes + 1; i++) {
        double y = data->vertexes[i][1];
        double z = data->vertexes[i][2];
        data->vertexes[i][1] = y * cos(wx) - z * sin(wx);
        data->vertexes[i][2] = z * cos(wx) + y * sin(wx);
    }
}

void turn_y(data_t *data, double wy) {
    for (int i = 1; i < data->n_vertexes + 1; i++) {
        double x = data->vertexes[i][0];
        double z = data->vertexes[i][2];
        data->vertexes[i][0] = x * cos(wy) + z * sin(wy);
        data->vertexes[i][2] = z * cos(wy) - x * sin(wy);
    }
}

void turn_z(data_t *data, double wz) {
    for (int i = 1; i < data->n_vertexes + 1; i++) {
        double x = data->vertexes[i][0];
        double y = data->vertexes[i][1];
        data->vertexes[i][0] = x * cos(wz) - y * sin(wz);
        data->vertexes[i][1] = y * cos(wz) + x * sin(wz);
    }
}

void scale(data_t *data, double kx, double ky, double kz) {
    for (int i = 1; i < data->n_vertexes + 1; i++) {
        if (kx && ky && kz) {
            data->vertexes[i][0] *= kx;
            data->vertexes[i][1] *= ky;
            data->vertexes[i][2] *= kz;
        }
    }
}
