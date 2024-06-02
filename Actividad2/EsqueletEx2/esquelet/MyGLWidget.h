#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core  {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  public slots:
    void setNumTrees(int n);
    void restartScene();
    void rotTreesRight();
    void rotTreesLeft();
    void changeCamera();
  
  protected:
    virtual void initializeGL ( );
    virtual void paintGL ( );
    virtual void resizeGL(int, int);

    virtual void keyPressEvent (QKeyEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mousePressEvent (QMouseEvent *e);

    virtual void treeTransform(int i);
    virtual void LukeTransform();

    virtual void terraTransform();
  
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void viewTransform ();
    virtual void projectTransform ();

    int numtrees;
    glm::vec3 posicioArbres[30];
    glm::vec3 escalaArbres[30];
    float rotacioArbres[30];
  
    glm::vec3 dirLuke, posLuke;
    float rotLuke, rotTrees;

    

    void calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &CentreBase);
    // creaBuffersModels - Carreguem els fitxers obj i fem la inicialització dels diferents VAOS i VBOs
    void creaBuffersModels ();
    float randNumber(float min, float max);
    //setTrees - Pinta la cuantitat d'arbres que volem 
    void setTrees(int numtrees);
    // creaBuffersTerra - Inicialitzem el VAO i els VBO d'un model fet a mà que representa un terra
    void creaBuffersTerra ();
    // carregaShaders - Carreguem els shaders, els compilem i els linkem. També busquem els uniform locations que fem servir.
    void carregaShaders ();
    


  private:
  
    int printOglError(const char file[], int line, const char func[]);
   
    // variables per interacció de càmera amb angles d'Euler
    float angleX, angleY;
    
    // bools per controlar si càmera en planta o no
    bool ortho;


    GLuint vertexLoc, colorLoc;

    // uniform locations
    GLuint transLoc, viewLoc, projLoc;

    // enum models - els models estan en un array de VAOs (VAO_models), aquest enum és per fer més llegible el codi.
    typedef enum { TREE = 0, LUKE = 1, NUM_MODELS = 2} ModelType;

    // VAO names
    GLuint VAO_models[NUM_MODELS];
    GLuint VAO_Terra;


     QOpenGLShaderProgram *program;

    // Viewport
    GLint ample, alt;

    // Mouse interaction
    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float factorAngleX, factorAngleY, auxX, auxY;

    // Internal vars
    float radiEscena;
    float fov, ra=1, raW, znear, zfar, d, alfainicial, alfanou;
    glm::vec3 centreEscena, obs, vrp, up;
    glm::vec3 pMin, pMax;
    float left, right, bottom, top;
    float distAvanza;

    // Models, capses contenidores i escales
    Model models[NUM_MODELS];
    glm::vec3 centreBaseModels[NUM_MODELS];
    float escalaModels[NUM_MODELS];
};
