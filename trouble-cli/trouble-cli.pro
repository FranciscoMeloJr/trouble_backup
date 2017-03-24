QT += core
QT -= gui

TARGET = trouble-cli
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/opencv

TEMPLATE = app
#QMAKE_CXXFLAGS +=  -finstrument-functions -finstrument-functions-exclude-file-list=OpenCVTest::cleanUp,OpenCVTest::detectAndDraw
#QMAKE_CXXFLAGS += -finstrument-functions -finstrument-functions-exclude-function-list=load,store,cvRound \

SOURCES += main.cpp \
    opencvtest.cpp \
    JsonTest.cpp \
    cyg.cpp \
    inlinetest.cpp \
    pftest.cpp \
    casablanca.cpp

LIBS += -ldl -L/usr/local/lib  -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -ljsoncpp \
    -lcpprest \
    -lpthread


include(../common.pri)
include(../libtrouble.pri)

HEADERS += \
    opencvtest.h \
    JsonTest.h \
    cyg.h \
    inlinetest.h \
    pftest.h \
    casablanca.h
