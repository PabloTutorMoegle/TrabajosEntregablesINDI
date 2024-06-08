#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
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
  glEnable (GL_DEPTH_TEST);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  IniCamara();
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
  
  projectTransform();
  viewTransform();
}

void MyGLWidget::paintGL () 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Homer);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3);
  
  glBindVertexArray(VAO_Base);
  
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::IniCamara()
{
  pMin = glm::vec3(-2.5f, 0.0f, -2.5f);
  pMax = glm::vec3(2.5f, 1.0f, 2.5f);
  VRP = (pMin + pMax)/2.0f;
  R = glm::distance(pMin, pMax) / 2.0f;
  d = 4 * R;
  OBS = VRP + glm::vec3(0,0,1)*d;
  alfaInicial = asin(R/d);
  FOV = 2 * alfaInicial;
  zNear = d - R;
  zFar = d + R;
}

void MyGLWidget::viewTransform()
{
  UP = glm::vec3(0, 1, 0);
  glm::mat4 View = glm::lookAt (OBS, VRP, UP);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform()
{
  glm::mat4 Proj = glm::perspective(FOV, raW, zNear, zFar);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
  raV = (float)ample / alt;
  if (raV < 1)
  {
  	raW = raV;
  	alfaNuevo = atan(tan(alfaInicial)/raV);
  	FOV = 2 * alfaNuevo;
  }
  else 
  {
  	raW = raV;
  	FOV = 2 * alfaInicial;
  }
  projectTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffers () 
{
  m.load("../porsche.obj"); //mirar carpeta
  
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER,
    sizeof(GLfloat) * m.faces ().size () * 3 * 3,
    m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER,
    sizeof(GLfloat) * m.faces().size() * 3 * 3,
    m.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  
  glm::vec3 Vertices[6];
  Vertices[0] = glm::vec3(-2.5, -1.0, -2.5);
  Vertices[1] = glm::vec3(-2.5, -1.0, 2.5);
  Vertices[2] = glm::vec3(2.5, -1.0, 2.5);
  Vertices[3] = glm::vec3(2.5, -1.0, 2.5);
  Vertices[4] = glm::vec3(2.5, -1.0, -2.5);
  Vertices[5] = glm::vec3(-2.5, -1.0, -2.5); 
  
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Base);
  glBindVertexArray(VAO_Base);

  GLuint VBO_Base[2];
  glGenBuffers(2, VBO_Base);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Base[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glm::vec3 VerticesColor[6];
  VerticesColor[0] = glm::vec3(0.2, 0.8, 0.0);
  VerticesColor[1] = glm::vec3(0.2, 0.8, 0.0);
  VerticesColor[2] = glm::vec3(0.2, 0.8, 0.0);
  VerticesColor[3] = glm::vec3(0.2, 0.8, 0.0);
  VerticesColor[4] = glm::vec3(0.2, 0.8, 0.0);
  VerticesColor[5] = glm::vec3(0.2, 0.8, 0.0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Base[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesColor), VerticesColor, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
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
  program = new QOpenGLShaderProgram(this); 								//bzmhd
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "PM");
  viewLoc = glGetUniformLocation(program->programId(), "VM");
}

