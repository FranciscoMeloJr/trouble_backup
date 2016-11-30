unix:!macx: PRE_TARGETDEPS += $${top_builddir}/libtrouble/libtrouble.so

unix:!macx: LIBS += -L$${top_builddir}/libtrouble/ -ltrouble

INCLUDEPATH += $${top_srcdir}/libtrouble
DEPENDPATH += $${top_srcdir}/libtrouble
