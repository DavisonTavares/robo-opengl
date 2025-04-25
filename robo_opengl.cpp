#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <SOIL/SOIL.h>

GLuint texMetal, texFace;
float angleArm = 0.0f;
float angleLeg = 0.0f;
float posX = 0.0f, posZ = 0.0f;
float rotationY = 0.0f;
float angleHead = 0.0f; // Nova variável para a rotação da cabeça

// Função para carregar as texturas
void loadTextures() {
    texMetal = SOIL_load_OGL_texture("textures/metal.jpeg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (texMetal == 0) {
        printf("Erro ao carregar textura metal.jpeg\n");
    }
    texFace = SOIL_load_OGL_texture("textures/face.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (texFace == 0) {
        printf("Erro ao carregar textura face.jpg\n");
    }

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Função para desenhar cubos texturizados
void drawTexturedCube(GLuint texID, float width, float height, float depth) {
    glBindTexture(GL_TEXTURE_2D, texID);
    glPushMatrix();
    glScalef(width, height, depth);

    glBegin(GL_QUADS);
    // Frente
    glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5,  0.5);
    glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5,  0.5);
    glTexCoord2f(1, 1); glVertex3f( 0.5,  0.5,  0.5);
    glTexCoord2f(0, 1); glVertex3f(-0.5,  0.5,  0.5);
    // Trás
    glTexCoord2f(0, 0); glVertex3f( 0.5, -0.5, -0.5);
    glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1, 1); glVertex3f(-0.5,  0.5, -0.5);
    glTexCoord2f(0, 1); glVertex3f( 0.5,  0.5, -0.5);
    // Lados (restante da geometria)
    glTexCoord2f(0, 0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5,  0.5);
    glTexCoord2f(1, 1); glVertex3f(-0.5,  0.5,  0.5);
    glTexCoord2f(0, 1); glVertex3f(-0.5,  0.5, -0.5);
    glTexCoord2f(0, 0); glVertex3f( 0.5, -0.5,  0.5);
    glTexCoord2f(1, 0); glVertex3f( 0.5, -0.5, -0.5);
    glTexCoord2f(1, 1); glVertex3f( 0.5,  0.5, -0.5);
    glTexCoord2f(0, 1); glVertex3f( 0.5,  0.5,  0.5);
    glEnd();

    glPopMatrix();
}

// Função para desenhar o robô
void drawRobot() {
    glPushMatrix();

    // Corpo
    glColor3f(1, 1, 1); // Branco para a textura aparecer corretamente
    drawTexturedCube(texMetal, 1.0, 1.5, 0.5);

    // Cabeça
    glPushMatrix();
    glTranslatef(0, 1.25, 0);
    glRotatef(angleHead, 0, 1, 0); // Cabeça girando
    drawTexturedCube(texFace, 0.6, 0.6, 0.6);
    glPopMatrix();

    // Braço esquerdo
    glPushMatrix();
    glTranslatef(-0.75, 0.5, 0);
    glRotatef(angleArm, 1, 0, 0);
    glTranslatef(0, -0.5, 0);
    glColor3f(0.8, 0.3, 0.3);
    drawTexturedCube(texMetal, 0.3, 1.0, 0.3);
    glPopMatrix();

    // Braço direito
    glPushMatrix();
    glTranslatef(0.75, 0.5, 0);
    glRotatef(-angleArm, 1, 0, 0);
    glTranslatef(0, -0.5, 0);
    glColor3f(0.8, 0.3, 0.3);
    drawTexturedCube(texMetal, 0.3, 1.0, 0.3);
    glPopMatrix();

    // Perna esquerda
    glPushMatrix();
    glTranslatef(-0.3, -1.0, 0);
    glRotatef(angleLeg, 1, 0, 0);
    glTranslatef(0, -0.5, 0);
    glColor3f(0.4, 0.4, 0.4);
    drawTexturedCube(texMetal, 0.4, 1.0, 0.4);
    glPopMatrix();

    // Perna direita
    glPushMatrix();
    glTranslatef(0.3, -1.0, 0);
    glRotatef(-angleLeg, 1, 0, 0);
    glTranslatef(0, -0.5, 0);
    glColor3f(0.4, 0.4, 0.4);
    drawTexturedCube(texMetal, 0.4, 1.0, 0.4);
    glPopMatrix();

    glPopMatrix();
}

// Função para a iluminação
void setupLighting() {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

// Função de exibição
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Ajuste da posição da câmera para que o robô fique visível
    gluLookAt(0, 3, 10, 0, 0, 0, 0, 1, 0);

    glTranslatef(posX, 0, posZ);
    glRotatef(rotationY, 0, 1, 0);

    drawRobot();

    glutSwapBuffers();
}

// Função de atualização para animação
void update(int value) {
    angleArm = 30 * sin(glutGet(GLUT_ELAPSED_TIME) / 300.0);
    angleLeg = 30 * sin(glutGet(GLUT_ELAPSED_TIME) / 300.0);
    angleHead += 0.1f;  // Cabeça girando continuamente

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// Função de controle pelo teclado
void keyboard(unsigned char key, int x, int y) {
    float step = 0.2f;
    switch (key) {
        case 'w':
            posZ -= step * cos(rotationY * 3.14159 / 180);
            posX -= step * sin(rotationY * 3.14159 / 180);
            break;
        case 's':
            posZ += step * cos(rotationY * 3.14159 / 180);
            posX += step * sin(rotationY * 3.14159 / 180);
            break;
        case 'a':
            rotationY += 5.0f;
            break;
        case 'd':
            rotationY -= 5.0f;
            break;
    }
    glutPostRedisplay();
}

// Função de inicialização
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    loadTextures();
    setupLighting();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Robo OpenGL - Mundo Kids");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
