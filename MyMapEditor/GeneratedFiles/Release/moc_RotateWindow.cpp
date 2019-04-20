/****************************************************************************
** Meta object code from reading C++ file 'RotateWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtGUI/RotateWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RotateWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RotateWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      24,   13,   13,   13, 0x08,
      34,   13,   13,   13, 0x08,
      44,   13,   13,   13, 0x08,
      66,   58,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RotateWindow[] = {
    "RotateWindow\0\0rotateX()\0rotateY()\0"
    "rotateZ()\0resetRotate()\0visible\0"
    "setVisible(bool)\0"
};

void RotateWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RotateWindow *_t = static_cast<RotateWindow *>(_o);
        switch (_id) {
        case 0: _t->rotateX(); break;
        case 1: _t->rotateY(); break;
        case 2: _t->rotateZ(); break;
        case 3: _t->resetRotate(); break;
        case 4: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RotateWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RotateWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_RotateWindow,
      qt_meta_data_RotateWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RotateWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RotateWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RotateWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RotateWindow))
        return static_cast<void*>(const_cast< RotateWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int RotateWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
