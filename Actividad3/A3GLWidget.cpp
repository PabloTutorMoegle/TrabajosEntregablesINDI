#include "A3GLWidget.h"

#include <iostream>
#include <cmath>

using namespace std;

A3GLWidget::A3GLWidget (QWidget* parent) : MyGLWidget(parent), currentHour(14), farRotationAngle(0.0f)
{
}

A3GLWidget::~A3GLWidget()
{
}

void A3GLWidget::setHour(int hour) {
    currentHour = hour;
    updateSunPosition();
    update();
}

float A3GLWidget::hourToAngle(int hour) {
    return M_PI * (1.0f - (hour - 8) / 12.0f);
}

glm::vec3 A3GLWidget::calculateSunPosition(int hour) {
    float angle = hourToAngle(hour);
    float x = 40.0f * cos(angle);
    float y = 40.0f * sin(angle);
    return glm::vec3(x, y, 0.0f);
}

void A3GLWidget::updateSunPosition() {
    glm::vec3 sunPosition = calculateSunPosition(currentHour);
    glUniform3f(glGetUniformLocation(program->programId(), "lightPos"), sunPosition.x, sunPosition.y, sunPosition.z);
}

void A3GLWidget::setTorch1(bool on) {
    torchesOn[0] = on;
    update();
}
void A3GLWidget::setTorch2(bool on) {
    torchesOn[1] = on;
    update();
}
void A3GLWidget::setTorch3(bool on) {
    torchesOn[2] = on;
    update();
}
void A3GLWidget::setTorch4(bool on) {
    torchesOn[3] = on;
    update();
}
void A3GLWidget::setTorch5(bool on) {
    torchesOn[4] = on;
    update();
}
void A3GLWidget::setTorch6(bool on) {
    torchesOn[5] = on;
    update();
}
void A3GLWidget::setColorRed(int red) {
    makeCurrent();
    torchColor.x = static_cast<float>(red) / 255.0f;
    glUniform3f(glGetUniformLocation(program->programId(), "torchColor"), torchColor.x, torchColor.y, torchColor.z);
    update();
}
void A3GLWidget::setColorGreen(int green) {
    makeCurrent();
    torchColor.y = static_cast<float>(green) / 255.0f;
    glUniform3f(glGetUniformLocation(program->programId(), "torchColor"), torchColor.x, torchColor.y, torchColor.z);
    update();
}
void A3GLWidget::setColorBlue(int blue) {
    makeCurrent();
    torchColor.z = static_cast<float>(blue) / 255.0f;
    glUniform3f(glGetUniformLocation(program->programId(), "torchColor"), torchColor.x, torchColor.y, torchColor.z);
    update();
}

void A3GLWidget::updateTorchPositions() {
    glm::vec3 torchPositions[6] = {
        glm::vec3(-7.39, 1.95, -6.68),
        glm::vec3(-9.95, 1.95, -0.56),
        glm::vec3(-7.47, 1.95, 5.64),
        glm::vec3(4.38, 1.95, 5.26),
        glm::vec3(6.68, 1.95, 0.38),
        glm::vec3(4.15, 1.95, -6.97)
    };

    // Transformar posiciones al sistema de coordenadas del mundo
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), VaixellPos) * glm::rotate(glm::mat4(1.0f), VaixellRot, glm::vec3(0, 1, 0));
    glm::vec3 worldTorchPositions[6];
    for (int i = 0; i < 6; ++i) {
        worldTorchPositions[i] = glm::vec3(modelMatrix * glm::vec4(torchPositions[i], 1.0));
    }

    glUniform3fv(glGetUniformLocation(program->programId(), "posFocusTorxes"), 6, &worldTorchPositions[0][0]);
}

void A3GLWidget::iniEscena ()
{
    creaBuffersModels();

    VaixellPos = glm::vec3(-10, 0.0, 0.0);
    VaixellRot = 0.0;

    //--------------------------------------------------
    centreEsc = glm::vec3 (0, 0, 0);

    radiEsc = 20; // Tret de la màniga :-P

    for (int i = 0; i < 6; ++i) {
        torchesOn[i] = true;
    }

    updateFarLights();

    // Establecer el color de la luz ambiental
    glUniform3f(glGetUniformLocation(program->programId(), "ambientLightColor"), 0.1f, 0.1f, 0.1f);

    // Inicializar la posición y color del foco de luz (sol)
    updateSunPosition();
    glUniform3f(glGetUniformLocation(program->programId(), "lightColor"), 0.6f, 0.6f, 0.6f);

    // Actualizar las posiciones de las antorchas
    updateTorchPositions();

    // Color para las antorchas
    torchColor = glm::vec3(1.0f, 1.0f, 0.0f);
    glUniform3f(glGetUniformLocation(program->programId(), "torchColor"), torchColor.x, torchColor.y, torchColor.z);
}

void A3GLWidget::updateFarLights()
{
    farLight1 = glm::vec3(0.363f, 4.695f, 0.357f);
    farLight2 = glm::vec3(-0.357f, 4.695f, -0.348f);

    glm::mat4 TGFar = glm::translate(glm::mat4(1.0f), glm::vec3(2, 1, 4));
    TGFar = glm::scale(TGFar, glm::vec3(escalaModels[FAR_1]));

    farLight1 = glm::vec3(TGFar * glm::vec4(farLight1, 1.0f));
    farLight2 = glm::vec3(TGFar * glm::vec4(farLight2, 1.0f));

    // Crear una matriz de transformación que aplique solo la rotación
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), farRotationAngle, glm::vec3(0, 1, 0));

    // Aplicar la rotación a las coordenadas de los focos
    farLight1 = glm::vec3(rotationMatrix * glm::vec4(farLight1, 1.0f));
    farLight2 = glm::vec3(rotationMatrix * glm::vec4(farLight2, 1.0f));

    // Dirección de los focos
    glm::vec3 farLight1Dir = glm::normalize(farLight2 - farLight1);
    glm::vec3 farLight2Dir = glm::normalize(farLight1 - farLight2);

    // Pasar las nuevas posiciones de los focos al shader
    glUniform3fv(glGetUniformLocation(program->programId(), "farLight1Pos"), 1, &farLight1[0]);
    glUniform3fv(glGetUniformLocation(program->programId(), "farLight2Pos"), 1, &farLight2[0]);

    // Pasar las direcciones de los focos al shader
    glUniform3fv(glGetUniformLocation(program->programId(), "farLight1Dir"), 1, &farLight1Dir[0]);
    glUniform3fv(glGetUniformLocation(program->programId(), "farLight2Dir"), 1, &farLight2Dir[0]);
}


void A3GLWidget::paintGL ()
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

    projectTransform ();
    viewTransform ();

    // Esborrem el frame-buffer i el depth-buffer
    glClearColor(0.8f, 0.8f, 1.0f, 1.f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Actualizar la posición del sol
    updateSunPosition();

    // Actualizar las posiciones de las antorchas
    updateTorchPositions();

    GLint torchOnArray[6];
    for (int i = 0; i < 6; ++i) {
        torchOnArray[i] = torchesOn[i];
    }

    glUniform1iv(glGetUniformLocation(program->programId(), "torchOn"), 6, torchOnArray);

    //--------------------------------------------------------
    // Activem el VAO per a pintar el vaixell
    glBindVertexArray (VAO_models[VAIXELL]);
    // Transformació geometrica
    modelTransformVaixell();
    
    glDrawArrays(GL_TRIANGLES, 0, models[VAIXELL].faces().size()*3);
    //--------------------------------------------------------
    // Activem el VAO per a pintar el moll
    glBindVertexArray (VAO_models[MOLL]);
    // pintem les espelmes, cadascuna amb el seu transform
    modelTransformMoll();
    
    glDrawArrays(GL_TRIANGLES, 0, models[MOLL].faces().size()*3);
    //--------------------------------------------------------
    // Activem el VAO per a pintar el far (primera part)
    glBindVertexArray (VAO_models[FAR_1]);
    // pintem les espelmes, cadascuna amb el seu transform
    modelTransformFar1();
    
    glDrawArrays(GL_TRIANGLES, 0, models[FAR_1].faces().size()*3);
    //--------------------------------------------------------
    // Activem el VAO per a pintar el far (segona part)
    glBindVertexArray (VAO_models[FAR_2]);
    // pintem les espelmes, cadascuna amb el seu transform
    modelTransformFar2();
    
    glDrawArrays(GL_TRIANGLES, 0, models[FAR_2].faces().size()*3);
    //------------------------------------------------------------
    // Pintem terra
    glBindVertexArray (VAO_Terra);
    modelTransformTerra();
    
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pintem Mar
    glBindVertexArray (VAO_Mar);
    modelTransformMar();
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //--------------------------------------------------------

    glBindVertexArray(0);
}

void A3GLWidget::modelTransformFar1()
{
    // Codi per a la TG necessària
    glm::mat4 TG = glm::mat4(1.0f);
    TG = glm::translate(TG, glm::vec3(2, 1, 4));
    TG = glm::scale(TG, glm::vec3(escalaModels[FAR_1]) );
    TG = glm::translate(TG, -centreBaseModels[FAR_1]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void A3GLWidget::modelTransformFar2()
{
    // Codi per a la TG necessària
    Far2_TG = glm::mat4(1.0f);
    Far2_TG = glm::translate(Far2_TG, glm::vec3(2, 1, 4));
    Far2_TG = glm::rotate(Far2_TG, farRotationAngle, glm::vec3(0, 1, 0)); 
    Far2_TG = glm::scale(Far2_TG, glm::vec3(escalaModels[FAR_1]) );
    Far2_TG = glm::translate(Far2_TG, -centreBaseModels[FAR_1]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Far2_TG[0][0]);
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(glm::transpose(glm::mat3(View * Far2_TG)))));
}

void A3GLWidget::modelTransformVaixell()
{
    // Codi per a la TG necessària
    Vaixell_TG = glm::mat4(1.0f);
    Vaixell_TG = glm::translate(Vaixell_TG, VaixellPos);
    Vaixell_TG = glm::rotate(Vaixell_TG, VaixellRot, glm::vec3(0, 1, 0)); 
    Vaixell_TG = glm::scale(Vaixell_TG, glm::vec3(escalaModels[VAIXELL]) );
    Vaixell_TG = glm::translate(Vaixell_TG,
                        -glm::vec3(centreBaseModels[VAIXELL].x,
                                   0,
                                   centreBaseModels[VAIXELL].z)); // el vaixell ja esta a la línia de flotació....no toquem en alçada
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Vaixell_TG[0][0]);
}

void A3GLWidget::keyPressEvent(QKeyEvent* event)
{
    makeCurrent();

    switch (event->key()) {
        case Qt::Key_S: {
            float advance = -1.0f;
            VaixellPos[2] -= advance;
            break;
        }
        case Qt::Key_W: {
            float advance = 1.0f;
            VaixellPos[2] -= advance;
            break;
        }
        case Qt::Key_A: {
            farRotationAngle += glm::radians(5.0f); 
            updateFarLights();
            break;
        }
        case Qt::Key_D: {
            farRotationAngle -= glm::radians(5.0f); 
            updateFarLights();
            break;
        }
        case Qt::Key_Up: {
            if (currentHour < 20) {
                currentHour++;
                updateSunPosition();
            }
            break;
        }
        case Qt::Key_Down: {
            if (currentHour > 8) {
                currentHour--;
                updateSunPosition();
            }
            break;
        }
        default: 
            event->ignore(); 
            break;
    }
    update();
}

void A3GLWidget::carregaShaders()
{
    // Creem els shaders per al fragment shader i el vertex shader
    QOpenGLShader fs (QOpenGLShader::Fragment, this);
    QOpenGLShader vs (QOpenGLShader::Vertex, this);
    // Carreguem el codi dels fitxers i els compilem
    fs.compileSourceFile("./shaders/basicLlumShader.frag");
    vs.compileSourceFile("./shaders/basicLlumShader.vert");
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
    // Obtenim identificador per a l'atribut “normal” del vertex shader
    normalLoc = glGetAttribLocation (program->programId(), "normal");
    // Obtenim identificador per a l'atribut “matamb” del vertex shader
    matambLoc = glGetAttribLocation (program->programId(), "matamb");
    // Obtenim identificador per a l'atribut “matdiff” del vertex shader
    matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
    // Obtenim identificador per a l'atribut “matspec” del vertex shader
    matspecLoc = glGetAttribLocation (program->programId(), "matspec");
    // Obtenim identificador per a l'atribut “matshin” del vertex shader
    matshinLoc = glGetAttribLocation (program->programId(), "matshin");

    // Demanem identificadors per als uniforms del vertex shader
    transLoc = glGetUniformLocation (program->programId(), "TG");
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");

    // Establecer el color de la luz ambiental
    glUniform3f(glGetUniformLocation(program->programId(), "ambientLightColor"), 0.1f, 0.1f, 0.1f);

    // Establecer la posición y color del foco de luz (sol)
    glUniform3f(glGetUniformLocation(program->programId(), "lightPos"), 0.0f, 40.0f, 0.0f);
    glUniform3f(glGetUniformLocation(program->programId(), "lightColor"), 0.6f, 0.6f, 0.6f);

    updateFarLights();
}
