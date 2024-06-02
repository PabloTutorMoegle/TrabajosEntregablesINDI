#include 'MyGLWidgetAux.h'

MyGLWidgetAux::MyGLWidgetAux(QWidget *parent=0):MyGLWidget(parent)
{}

void MyGLWidgetAux::setTrees(int n)
{
    serTrees(n);
}