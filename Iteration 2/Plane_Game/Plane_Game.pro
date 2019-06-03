HEADERS += \
    bullet.h \
    enemy.h \
    entity.h \
    explosion.h \
    factory.h \
    game_logic.h \
    gui_app.h \
    musicplay.h \
    mybutton.h \
    plane.h \
    reward.h \
    review_app.h

SOURCES += \
    bullet.cpp \
    enemy.cpp \
    entity.cpp \
    explosion.cpp \
    factory.cpp \
    game_logic.cpp \
    gui_app.cpp \
    main.cpp \
    musicplay.cpp \
    mybutton.cpp \
    plane.cpp \
    reward.cpp \
    review_app.cpp

QT += widgets
QT += xml
QT += core gui widgets
QT += multimedia
QT += gui

RESOURCES += \
    rescource.qrc
