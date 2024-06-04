#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"
#include "MyGLWidget.h"

class A3GLWidget : public MyGLWidget
{
  Q_OBJECT

#define NUM_TORXES 6
#define HORA_MIN 8
#define HORA_MAX 20
#define NUM_FOCUS_FAR 2

  public:
    A3GLWidget (QWidget *parent=0);
    ~A3GLWidget ();

  public slots:
    void setHour(int hour);
    void setTorch1(bool on);
    void setTorch2(bool on);
    void setTorch3(bool on);
    void setTorch4(bool on);
    void setTorch5(bool on);
    void setTorch6(bool on);
    void setColorRed(int red);
    void setColorGreen(int green);
    void setColorBlue(int blue);

  protected:

    void modelTransformFar1();
    void modelTransformFar2();
    void modelTransformVaixell();
    void iniEscena ();
    void updateFarLights();
    void updateTorchPositions();
    void carregaShaders();

    virtual void paintGL ( );
    virtual void keyPressEvent (QKeyEvent *event);

    // Funciones para controlar la posición del sol
    float hourToAngle(int hour);
    glm::vec3 calculateSunPosition(int hour);
    void updateSunPosition();

    // Variables para controlar la hora y la posición del sol
    int currentHour;

    //variables para las luces del faro
    float farRotationAngle;
    glm::vec3 farLight1;
    glm::vec3 farLight2;
    
    glm::vec3 torchColor;
    bool torchesOn[6];

    //--------------------------------------------------------------
    // Posicions
    glm::vec3 VaixellPos;
    float VaixellRot;

    //--------------------------------------------------------------
    // Colors

    //--------------------------------------------------------------
    // MATRIUS DE TRANSFORMACIÓ
    glm::mat4 Vaixell_TG, Far2_TG;
};

