/****************************************************************************
** Meta object code from reading C++ file 'ScaleWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtGUI/ScaleWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScaleWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ScaleWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      22,   12,   12,   12, 0x08,
      31,   12,   12,   12, 0x08,
      48,   40,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ScaleWindow[] = {
    "ScaleWindow\0\0scaleX()\0scaleY()\0scaleZ()\0"
    "visible\0setVisible(bool)\0"
};

void ScaleWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ScaleWindow *_t = static_cast<ScaleWindow *>(_o);
        switch (_id) {
        case 0: _t->scaleX(); break;
        case 1: _t->scaleY(); break;
        case 2: _t->scaleZ(); break;
        case 3: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ScaleWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ScaleWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ScaleWindow,
      qt_meta_data_ScaleWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ScaleWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ScaleWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ScaleWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScaleWindow))
        return static_cast<void*>(const_cast< ScaleWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int ScaleWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
