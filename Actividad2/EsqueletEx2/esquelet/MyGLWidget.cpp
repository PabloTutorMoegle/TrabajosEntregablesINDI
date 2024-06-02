#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <random>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0): QOpenGLWidget(parent), program(NULL)
{
  srand (time(NULL));
}
int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::setNumTrees(int n)
{
  numtrees = n;
  setTrees(numtrees);
}

void MyGLWidget::restartScene()
{
  makeCurrent();
  rotLuke = 0.0f;
  dirLuke = glm::vec3(0.0f);
  posLuke = glm::vec3(0.0f);
  numtrees = 5;
  rotTrees = 0.0f;
  iniEscena();
  iniCamera();
  update();
}

void MyGLWidget::rotTreesRight()
{
  makeCurrent();
  rotTrees += M_PI/4;
  update();
}
void MyGLWidget::rotTreesLeft()
{
  makeCurrent();
  rotTrees -= M_PI/4;
  update();
}

void MyGLWidget::changeCamera()
{
  makeCurrent();
  ortho = !ortho;
  update();
}

void MyGLWidget::initializeGL ()
{
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersModels();
  creaBuffersTerra();
  iniEscena();
  iniCamera();
  setTrees(numtrees);
  ortho = true;
}

void MyGLWidget::iniEscena ()
{
  pMin = glm::vec3(-5.0f, 0.0f, -5.0f);
  pMax = glm::vec3(5.0f, 2.0f, 5.0f);
  centreEscena = (pMin+pMax)/2.0f;
  radiEscena = glm::distance(pMin, pMax) / 2.0f;
  d=2*radiEscena;

  dirLuke = glm::vec3(0.0f);
  posLuke = glm::vec3(0.0f);
  rotLuke = 0.0f;

  numtrees = 5;
}

void MyGLWidget::iniCamera() {
  angleY = 0;
  angleX = M_PI/4;

  left = -radiEscena;
  right = radiEscena;
  bottom = -radiEscena;
  top = radiEscena;
  
  alfainicial = asin(radiEscena/d);
  fov = 2*alfainicial;
  znear = d - radiEscena;
  zfar  = d + radiEscena;

  viewTransform();
  projectTransform();
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  if (ortho == true)
  {
    vrp = centreEscena;
    obs = vrp + (glm::vec3(0,0,1)*d);
    up = glm::vec3(0,1,0);
    View = glm::translate(View, glm::vec3(0.0f, 0.0f, -d));
    View = glm::rotate(View, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    View = glm::translate(View, glm::vec3(-vrp.x, -vrp.y, -vrp.z));
  }
  else
  {
    vrp = glm::vec3(0.0f);
    obs = glm::vec3(0, 2 * radiEscena, 0);
    up = glm::vec3(0, 0, 1);
    View = glm::lookAt(obs, vrp, up);
  }

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if(ortho == true)
  {
    Proj = glm::perspective(fov, ra, znear, zfar);
  }
  else
  {
    Proj = glm::ortho(left, right, bottom, top, znear, zfar);
  }

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  viewTransform();
  projectTransform();

  glBindVertexArray (VAO_models[TREE]);
  for(int i = 0; i <= numtrees; i++)
  {
    treeTransform(i);
    glDrawArrays(GL_TRIANGLES, 0, models[TREE].faces().size()*3);
  }

  glBindVertexArray (VAO_models[LUKE]);
  LukeTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[LUKE].faces().size()*3);

  // Terra
  glBindVertexArray (VAO_Terra);
  terraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  #ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
  #endif
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
  ra = (float)ample / alt;
  if (ra < 1)
  {
  	raW = ra;
  	alfanou = atan(tan(alfainicial)/ra);
  	fov = 2 * alfanou;
  }
  else 
  {
  	raW = ra;
  	fov = 2 * alfainicial;
  }
  projectTransform();
}


void MyGLWidget::treeTransform(int i)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(posicioArbres[i]));
  TG = glm::rotate(TG, rotTrees, glm::vec3(0.0f, 1.0f, 0.0f));
  TG = glm::rotate(TG, rotacioArbres[i], glm::vec3(0.0f, 1.0f, 0.0f));
  TG = glm::scale(TG, glm::vec3(escalaArbres[i]));
  TG = glm::translate(TG, -centreBaseModels[TREE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::LukeTransform()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posLuke);
  TG = glm::rotate(TG, rotLuke, glm::vec3(0.0f, 1.0f, 0.0f));
  TG = glm::scale(TG, glm::vec3(escalaModels[LUKE]));
  TG = glm::translate(TG, -centreBaseModels[LUKE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::terraTransform ()
{
  glm::mat4 TG(1.0f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: {
        float advance = 1.0f;
        posLuke.x += sin(rotLuke)*advance;
        posLuke.z += cos(rotLuke)*advance;
        if(posLuke.x > 5.0f || posLuke.x < -5.0f ||
            posLuke.z > 5.0f || posLuke.z < -5.0f)
        {
          posLuke.x -= sin(rotLuke)*advance;
          posLuke.z -= cos(rotLuke)*advance;
        }
      break;
    }
    case Qt::Key_Left: { 
        rotLuke += -M_PI/4;
      break;
    }
    case Qt::Key_Right: {
        rotLuke += M_PI/4;
      break;
    }
    case Qt::Key_C: { 
        ortho = !ortho;
      break;
        }
    case Qt::Key_R: { // reset
        rotLuke = 0.0f;
        dirLuke = glm::vec3(0.0f);
        posLuke = glm::vec3(0.0f);
        numtrees = 5;
        rotTrees = 0.0f;
        iniEscena();
        iniCamera();
      break;
        }  
    case Qt::Key_Minus: {
        rotTrees -= M_PI/4;
      break;
    }
    case Qt::Key_Plus: {
        rotTrees += M_PI/4;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}


void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();
  auxX = angleX;
  auxY = angleY;
  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  if (ortho == true)
  {
    if (DoingInteractive == ROTATE) {
      angleY = e->x() - xClick;
      angleX = e->y() - yClick;

      float angleXRadians = angleX * M_PI / 180.0f;
      float angleYRadians = angleY * M_PI / 180.0f;

      angleX += float(angleXRadians + auxX);
      angleY += float(angleYRadians + auxY);

      update();
    }
  }
}

float MyGLWidget::randNumber (float min, float max)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

void MyGLWidget::setTrees(int numtrees)
{
  for (int i = 0; i < numtrees; i++)
  {
    escalaArbres[i] = glm::vec3(randNumber(0.08, 0.13));
    posicioArbres[i] = glm::vec3(randNumber(-5, 5), 0.0f, randNumber(-5, 5));
    rotacioArbres[i] = randNumber(0.0f, 2 * M_PI);
  }
  update();
}

void MyGLWidget::creaBuffersTerra ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posTerra[4] = {
        glm::vec3(-5.0, 0.0, -5.0),
        glm::vec3(-5.0, 0.0,  5.0),
        glm::vec3( 5.0, 0.0, -5.0),
        glm::vec3( 5.0, 0.0,  5.0)
  };

  glm::vec3 c(0.65, 0.2, 0.05);
  glm::vec3 colTerra[4] = { c, c, c, c };

  // VAO
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posTerra), posTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colTerra), colTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }

  escala = alcadaDesitjada/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::creaBuffersModels ()
{
  // Càrrega dels models
  models[TREE].load("./models/tree.obj");
  models[LUKE].load("./models/luke_jedi.obj");

  // Creació de VAOs i VBOs per pintar els models
  glGenVertexArrays(NUM_MODELS, &VAO_models[0]);

  float alcadaDesitjada[NUM_MODELS] = {1,1};//,1,1,1.5,1};

  for (int i = 0; i < NUM_MODELS; i++)
  {
	  // Calculem la capsa contenidora del model
	  calculaCapsaModel (models[i], escalaModels[i], alcadaDesitjada[i], centreBaseModels[i]);

	  glBindVertexArray(VAO_models[i]);

	  GLuint VBO[2];
	  glGenBuffers(2, VBO);

	  // geometria
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_vertices(), GL_STATIC_DRAW);
	  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(vertexLoc);

	  // color
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_matdiff(), GL_STATIC_DRAW);
	  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(colorLoc);
  }

  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile(":shaders/basicShader.frag");
  vs.compileSourceFile(":shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");
}


