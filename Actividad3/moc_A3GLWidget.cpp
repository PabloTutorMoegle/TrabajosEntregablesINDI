/****************************************************************************
** Meta object code from reading C++ file 'A3GLWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "A3GLWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'A3GLWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_A3GLWidget_t {
    QByteArrayData data[11];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_A3GLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_A3GLWidget_t qt_meta_stringdata_A3GLWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "A3GLWidget"
QT_MOC_LITERAL(1, 11, 7), // "setHour"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 4), // "hour"
QT_MOC_LITERAL(4, 25, 9), // "setTorch1"
QT_MOC_LITERAL(5, 35, 2), // "on"
QT_MOC_LITERAL(6, 38, 9), // "setTorch2"
QT_MOC_LITERAL(7, 48, 9), // "setTorch3"
QT_MOC_LITERAL(8, 58, 9), // "setTorch4"
QT_MOC_LITERAL(9, 68, 9), // "setTorch5"
QT_MOC_LITERAL(10, 78, 9) // "setTorch6"

    },
    "A3GLWidget\0setHour\0\0hour\0setTorch1\0"
    "on\0setTorch2\0setTorch3\0setTorch4\0"
    "setTorch5\0setTorch6"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_A3GLWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a /* Public */,
       4,    1,   52,    2, 0x0a /* Public */,
       6,    1,   55,    2, 0x0a /* Public */,
       7,    1,   58,    2, 0x0a /* Public */,
       8,    1,   61,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x0a /* Public */,
      10,    1,   67,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,

       0        // eod
};

void A3GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<A3GLWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setHour((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setTorch1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setTorch2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setTorch3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setTorch4((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setTorch5((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setTorch6((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject A3GLWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<MyGLWidget::staticMetaObject>(),
    qt_meta_stringdata_A3GLWidget.data,
    qt_meta_data_A3GLWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *A3GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *A3GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_A3GLWidget.stringdata0))
        return static_cast<void*>(this);
    return MyGLWidget::qt_metacast(_clname);
}

int A3GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MyGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
