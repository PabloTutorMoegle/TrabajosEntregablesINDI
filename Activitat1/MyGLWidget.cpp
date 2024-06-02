//#include <GL/glew.h>
#include "MyGLWidget.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat

  // estat inicial;
  nuvolPos[0]= glm::vec3(0.6,0.0,0.0);
  nuvolPos[1]= glm::vec3(0.8,0.0,0.0);
  nuvolPos[2]= glm::vec3(0.7,0.05,0.0);
  nuvolPos[3]= glm::vec3(0.75,-0.05,0.0);
  nuvolPos[4]= glm::vec3(0.65,-0.05,0.0);
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (200/255.0, 220/255.0, 255/255.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();

  // Inicialització de variables d'estat
  rotSector = 0.0;
 }

void MyGLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retna.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

    glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
    
    //pintaNuvol();
    pintaSector();
    pintaCistella();
    pintaBase();

    // Desactivem el VAO actiu
    glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}


void MyGLWidget::pintaSector(){
    float angulo = 2 * M_PI / 16;
    for (int r = 0; r < 16; r++)
    {
    	float a = angulo * r;            
    	transformacioSector(a);
        glBindVertexArray(VAO_SECTOR);
        glDrawArrays(GL_TRIANGLES, 0, 21);
    }
}

void MyGLWidget::pintaBase(){
    transformacioBase();
    glBindVertexArray(VAO_BASE);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
void MyGLWidget::pintaCistella(){
    float angle = 2 * M_PI / 8;
    for (int i = 0; i < 8; i++)
    {
	float x = angle * i;
	transformacioCistella(x);
	glBindVertexArray(VAO_CISTELLA);
	glDrawArrays(GL_TRIANGLES, 0, 18);
    }
}

void MyGLWidget::pintaNuvol(){
    transformacioNuvol();
    glBindVertexArray(VAO_NUVOL);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    makeCurrent();
    switch (event->key()) {

        case Qt::Key_A: {
        	rotSector += 0.1;
            break;
        }
        case Qt::Key_D: {
        	rotSector -= 0.1;
            break;
        }
        default: event->ignore(); break;
    }
    update();
}


void MyGLWidget::transformacioNuvol(){
    glm::mat4 transform (1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioCistella(float angleRadians){
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, -0.33f, 0.0f));
    transform = glm::rotate(transform, rotSector + angleRadians, glm::vec3(0,0,1));
    transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));
    transform = glm::rotate(transform, -(rotSector + angleRadians), glm::vec3(0,0,1));
    transform = glm::scale(transform, glm::vec3(0.1f, 0.1f, 0.0f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioBase(){
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, -0.75f, 0.0f));
    transform = glm::scale(transform, glm::vec3(1.0f, 0.5f, 0.0f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioSector(float angleRadians){
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, -0.25f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.0f));
    transform = glm::rotate(transform, angleRadians + rotSector, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::creaBuffers (){
    creaBufferSector();
    creaBufferBase();
    creaBufferCistella();
    creaBufferNuvol(COLOR_BLANC);
}


void MyGLWidget::creaBufferNuvol(glm::vec3 color){

    glm::vec3 Vertices[4];  // Tres vèrtexs amb X, Y i Z
    Vertices[0] = glm::vec3(-1.0,  1.0, 0.0);
    Vertices[1] = glm::vec3(-1.0, -1.0, 0.0);
    Vertices[2] = glm::vec3( 1.0,  1.0, 0.0);
    Vertices[3] = glm::vec3( 1.0, -1.0, 0.0);

    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_NUVOL);
    glBindVertexArray(VAO_NUVOL);

    glm::vec3 Colors[4];
    for(int i=0;i<4;i++) {
        Colors[i] = color;//
    }

    createVBOs( sizeof(Vertices), Colors, Vertices );

    // retornem l'identificador de VAO creat
}

void MyGLWidget::createVBOs(int size,  glm::vec3 Colors[], glm::vec3 Vertices[] ){

    // Creació del buffer amb les dades dels vèrtexs
    GLuint VBO[2];
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, size, Vertices, GL_STATIC_DRAW);
    // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Creació del buffer de colors
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, size, Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);


    // Desactivem el VAO
    glBindVertexArray(0);
}


void MyGLWidget::creaBufferBase(){
    glm::vec3 Colors[3];
    glm::vec3 Vertices[3];
    int i=0,c=0;
    Vertices[i++] = glm::vec3(0.0f , 01.0f, 0.0f);
    Vertices[i++] = glm::vec3(-0.15, -1.0f, 0.0f);
    Vertices[i++] = glm::vec3(0.15, -1.0f, 0.0f);
    Colors[c++]=COLOR_GROC;
    Colors[c++]=COLOR_GROC;
    Colors[c++]=COLOR_GROC;

    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_BASE);
    glBindVertexArray(VAO_BASE);

    createVBOs( sizeof(Vertices), Colors, Vertices );

}

void MyGLWidget::creaBufferSector()
{							//16
    glm::vec3 Colors[21];
    glm::vec3 Vertices[21];
    int i=0,c=0;
    float alfa=glm::radians(angleSectorNoria);
    float r[]={0.4f, 0.7f,  1.0f};
    float gruix=0.05f;
    
    Vertices[i++] = glm::vec3(0.0f , 0.0f, 0.0f);
    Vertices[i++] = glm::vec3(1.0f, 0.0f, 0.0f);
    Vertices[i++] = glm::vec3((r[2]-gruix)*cos(alfa/6), (r[2]-gruix)*sin(alfa/6), 0.0f);
    Colors[c++]=COLOR_BLAU;
    Colors[c++]=COLOR_BLAU;
    Colors[c++]=COLOR_BLAU;

    for(int k=0;k<3;k++){
      Vertices[i++] = glm::vec3(r[k]*cos(alfa), r[k]*sin(alfa), 0.0);
      Vertices[i++] = glm::vec3((r[k]-gruix)*cos(alfa), (r[k]-gruix)*sin(alfa), 0.0);
      Vertices[i++] = glm::vec3(r[k]-gruix, 0, 0.0);

      Vertices[i++] = glm::vec3(r[k]*cos(alfa), r[k]*sin(alfa), 0.0);
      Vertices[i++] = glm::vec3(r[k]-gruix, 0, 0.0);
      Vertices[i++] = glm::vec3(r[k], 0, 0.0);

      for(int n=0;n<6;n++){    
      	if (c >= 3 && c <= 8)			Colors[c++] = COLOR_VERMELL;
      	else if (c >= 9 && c <= 14)		Colors[c++] = COLOR_GROC;
      	else if (c >= 15 && c <= 20)		Colors[c++] = COLOR_BLAU;
      }
    }
    
    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_SECTOR);
    glBindVertexArray(VAO_SECTOR);

    createVBOs(sizeof(Vertices), Colors, Vertices);
}




void MyGLWidget::creaBufferCistella()
{
  float w = 0.6;
  float h = 0.7;

  glm::vec3 Colors[18];
  glm::vec3 Vertices[18];
  int i=0;

  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3(-1, 0.0, 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3(-w,  h, 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3(-w, -h, 0.0);

  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3(-w,  h , 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3(-w, -h, 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3( w,  h, 0.0);

  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3( w,  h, 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3(-w, -h, 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3( w, -h, 0.0);

  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3( w, -h, 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3( w,  h, 0.0);
  Colors[i]=COLOR_BLAU_FLUIX; Vertices[i++] = glm::vec3( 1, 0.0, 0.0);


  Colors[i]=COLOR_BLANC; Vertices[i++] = glm::vec3(-w*0.8,  h*0.8 , 0.0);
  Colors[i]=COLOR_BLANC; Vertices[i++] = glm::vec3(-w*0.8, 0, 0.0);
  Colors[i]=COLOR_BLANC; Vertices[i++] = glm::vec3( w*0.8,  h*0.8, 0.0);

  Colors[i]=COLOR_BLANC; Vertices[i++] = glm::vec3( w*0.8,  h*0.8, 0.0);
  Colors[i]=COLOR_BLANC; Vertices[i++] = glm::vec3(-w*0.8, 0, 0.0);
  Colors[i]=COLOR_BLANC; Vertices[i++] = glm::vec3( w*0.8, 0, 0.0);


  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO_CISTELLA);
  glBindVertexArray(VAO_CISTELLA);


  createVBOs(sizeof(Vertices),  Colors, Vertices );

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc =  glGetAttribLocation (program->programId(), "color");
  transLoc =  glGetUniformLocation(program->programId(), "TG");
  valLoc =    glGetUniformLocation(program->programId(), "rotacio");
}

