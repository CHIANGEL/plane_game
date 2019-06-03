/****************************************************************************
** Meta object code from reading C++ file 'gui_app.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui_app.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui_app.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_gui_app_t {
    QByteArrayData data[10];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_gui_app_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_gui_app_t qt_meta_stringdata_gui_app = {
    {
QT_MOC_LITERAL(0, 0, 7), // "gui_app"
QT_MOC_LITERAL(1, 8, 8), // "TimeDraw"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 10), // "TimeBullet"
QT_MOC_LITERAL(4, 29, 9), // "TimeEnemy"
QT_MOC_LITERAL(5, 39, 10), // "TimeReward"
QT_MOC_LITERAL(6, 50, 16), // "StartButtonPress"
QT_MOC_LITERAL(7, 67, 15), // "LoadButtonPress"
QT_MOC_LITERAL(8, 83, 17), // "ReviewButtonPress"
QT_MOC_LITERAL(9, 101, 16) // "MusicButtonPress"

    },
    "gui_app\0TimeDraw\0\0TimeBullet\0TimeEnemy\0"
    "TimeReward\0StartButtonPress\0LoadButtonPress\0"
    "ReviewButtonPress\0MusicButtonPress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_gui_app[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void gui_app::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        gui_app *_t = static_cast<gui_app *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->TimeDraw(); break;
        case 1: _t->TimeBullet(); break;
        case 2: _t->TimeEnemy(); break;
        case 3: _t->TimeReward(); break;
        case 4: _t->StartButtonPress(); break;
        case 5: _t->LoadButtonPress(); break;
        case 6: _t->ReviewButtonPress(); break;
        case 7: _t->MusicButtonPress(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject gui_app::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_gui_app.data,
      qt_meta_data_gui_app,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *gui_app::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *gui_app::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_gui_app.stringdata0))
        return static_cast<void*>(const_cast< gui_app*>(this));
    return QWidget::qt_metacast(_clname);
}

int gui_app::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
