#include "glview.h"
#include <QDebug>
#define GL_SILENCE_DEPRECATION

GLView::GLView(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

void GLView::setStr(const QString& s) {
    str = s;
}

void GLView::initializeGL() {
    initializeOpenGLFunctions();
    glViewport(0, 0, width(), height());
}

void GLView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void GLView::paintGL() {
    glClearColor(red, green, blue, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projection) {
        glOrtho(-15, 15, -20, 20, -100, 100);
    } else {
        glFrustum(-1.5, 1.5, -2.0, 2.0, 3, 50);
        glTranslatef(0.0, 0.0, -3.0);
    }

    // преобразование в с-строку:
    qDebug() << str;
    QByteArray buf = str.toLocal8Bit();
    const char* f_name = buf.data();

    // запись в структуру
    data_t data;
    scan_obj_size(&data, f_name);
    n_vertexes = data.n_vertexes;
    n_facets = data.n_facets;
//    qDebug() << n_vertexes;
//    qDebug() << n_facets;
    struct_malloc(&data);
    scan_obj(&data, f_name);
    if(wx) turn_x(&data, wx);
    if(wy) turn_y(&data, wy);
    if(wz) turn_z(&data, wz);
    s21_move(&data, dx, dy, dz);

    //перебирает грани в структуре data
    int facet = 0;
    int wrong_vertex_flag = 0;
//    glScaled(kx, kx, kx);
    scale(&data, kx, kx, kx);
    glColor3d(line_red, line_green, line_blue);
    glLineWidth(line_width);
    glLineStipple(1, line_style);
    glVertexPointer(3, GL_FLOAT, 0, data.vertexes);
    glEnable(GL_LINE_STIPPLE);
    for (int j = 0; j < data.n_facets; j++) {
            //проверяет номера вершин на валидность
        for (int i = 0; i < data.vert_in_facet; i++) {
            if (data.facets[j][i] > data.n_vertexes) wrong_vertex_flag = 1;
        }
        if (wrong_vertex_flag) {
            wrong_vertex_flag = 0;
            continue;
        }
        glBegin(GL_LINES);
        //отрисовывает линии, из которых состоит грань
        for (int i = 0; i < data.vert_in_facet-1; i++) {
            facet = data.facets[j][i];
            glVertex3f(data.vertexes[facet][0], data.vertexes[facet][1], data.vertexes[facet][2]);
            facet = data.facets[j][i+1];
            glVertex3f(data.vertexes[facet][0], data.vertexes[facet][1], data.vertexes[facet][2]);
        }
        //отрисовывает последнюю линию
        facet = data.facets[j][data.vert_in_facet-1];
        glVertex3f(data.vertexes[facet][0], data.vertexes[facet][1], data.vertexes[facet][2]);
        facet = data.facets[j][0];
        glVertex3f(data.vertexes[facet][0], data.vertexes[facet][1], data.vertexes[facet][2]);
        glEnd();
    }
    glDisable(GL_LINE_STIPPLE);
    glColor3d(point_red, point_green, point_blue);
    glPointSize(point_width);
    if (point_style) {
        if (point_style == 1) glEnable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
        for (int j = 1; j <= data.n_vertexes; j++) {
            glVertex3d(data.vertexes[j][0], data.vertexes[j][1], data.vertexes[j][2]);
        }
        glEnd();
        if (point_style == 1) glDisable(GL_POINT_SMOOTH);
    }
    struct_free(&data);
}

void GLView::screenshot(QString filename) {
    this->grabFramebuffer().save(filename);
}
