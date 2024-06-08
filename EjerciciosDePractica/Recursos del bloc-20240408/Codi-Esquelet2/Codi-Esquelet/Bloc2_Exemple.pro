TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp 
        
INCLUDEPATH += /home/pablo/Documents/INDI/OpenGLindi/HomerActivity/Codi-Esquelet/Model
SOURCES += /home/pablo/Documents/INDI/OpenGLindi/HomerActivity/Codi-Esquelet/Model/model.cpp
