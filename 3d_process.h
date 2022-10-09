#ifndef SRC_3D_PROCESS_H
#define SRC_3D_PROCESS_H

typedef struct data {
    int n_vertexes; // Количество вершин
    int n_facets; // Количество граней
    int vert_in_facet; // Количество вершин в грани (3 или 4)
    double **vertexes; //Массив вершин vertexes[№ вершины][x/y/z]
    int **facets; // Массив граней facets[№ грани][№-ра вершин, считая с 1]
} data_t;
// Массив вершин vertexes заполняется от 1 до n_vertexes + 1, vertexes[0] - пустая строчка
// Это из-за того, что в гранях указаны №-ра вершин, считая с 1, а не с 0


//Последовательность вызова: scan_obj_size, struct_malloc, scan_obj, struct_free

// Записывает в структуру data в переменные n_vertexes n_facets vert_in_facet
// данные из файла по адресу file_name, возвращает 1 если успешно
int scan_obj_size(data_t *data, const char *file_name);

// Вспомогательная функция. Записывает в data данные из одной строчки,
// str_type - ее начало (v, f), str_vals - вся оставшаяся строка (значения)
void update_n_values(data_t *data, char *str_type, char *str_vals); //

// Выделяет для массивов vertexes и facets память
// до n_vertexes + 1 и до n_facets соответственно
void struct_malloc(data_t *data);

// Освобождает память в структуре data
void struct_free(data_t *data);

// Заполняет массивы в data.
// Берет данные из файла по адресу file_name, возвращает 1 если успешно
int scan_obj(data_t *data, const char *file_name);

// Вспомогательная функция. Записывает в vertexes данные из одной строчки,
// str_vals - строчка с координатами вершин, i_vertex - номер вершины
int scan_vertex(data_t *data, char *str_vals, int i_vertex);

// Вспомогательная функция. Записывает в facets данные из одной строчки,
// str_vals - строчка с номерами вершин, i_facet - номер грани
int scan_facet(data_t *data, char *str_vals, int i_facet);

// Выводит все содержание data в stdio
void struct_print(data_t *data);

// Перемещение на dx, dy и dz вдоль соотв. осей
void s21_move(data_t *data, double dx, double dy, double dz);

// Вращение на углы wx, wy и wz радиан вокруг соотв. осей
void turn_x(data_t *data, double wx);
void turn_y(data_t *data, double wy);
void turn_z(data_t *data, double wz);

// Изменение размера на kx, ky и kz вдоль соотв. осей
void scale(data_t *data, double kx, double ky, double kz);

#endif // SRC_3D_PROCESS_H
