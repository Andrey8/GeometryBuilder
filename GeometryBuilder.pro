#-------------------------------------------------
#
# Project created by QtCreator 2017-08-20T15:54:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeometryBuilder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG += warn_off
CONFIG += object_parallel_to_source

DISTFILES +=

HEADERS += \
    BSEHandlers/bsehandlerforcircle.h \
    BSEHandlers/bsehandlerforline.h \
    BSEHandlers/bsehandlerforlinesegment.h \
    BSEHandlers/bsehandlerforpoint.h \
    BSEHandlers/bsehandlerforscene.h \
    BSEHandlers/buildingstepseventshandler.h \
    BSEHandlers/objects.h \
    DebugHelpers/debughelper.h \
    DebugHelpers/exception.h \
    GraphicsShapesBuilders/circlebuilder.h \
    GraphicsShapesBuilders/linebuilder.h \
    GraphicsShapesBuilders/linesegmentbuilder.h \
    GraphicsShapesBuilders/objects.h \
    GraphicsShapesBuilders/pointbuilder.h \
    GraphicsShapesBuilders/polygonbuilder.h \
    Helpers/objects.h \
    Shapes/circle.h \
    Shapes/line.h \
    Shapes/linesegment.h \
    Shapes/polygon.h \
    Widgets/mainwindow.h \
    Widgets/planeview.h \
    Widgets/toolbar.h \
    coordsystem.h \
    gbpointpair.h \
    gbuilderhelper.h \
    gbuildernamespace.h \
    gvector.h \
    interactionobjects.h \
    mathnamespace.h \
    planescene.h \
    polygonalgorithms.h \
    GraphicsShapesBuilders/Additional/linesegmentextender.h \
    GraphicsShapesBuilders/Additional/linesegmentmidpointbuilder.h \
    GraphicsShapesBuilders/Additional/intersectionpointbuilder.h \
    IntersectionPointsBSEHandlers/intersectionpointsbuildingstepseventshandler.h \
    IntersectionPointsBSEHandlers/handlerforcircle.h \
    IntersectionPointsBSEHandlers/objects.h \
    IntersectionPointsBSEHandlers/handlerforline.h \
    IntersectionPointsBSEHandlers/handlerforlinesegment.h \
    BSEHandlers/perpendicularlsbsehandler.h \
    GraphicsShapesBuilders/Additional/perpendicularlinesegmentbuilder.h \
    BSEHandlers/parallellsbsehandler.h \
    GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.h \
    BSEHandlers/tangentlsbsehandler.h \
    GraphicsShapesBuilders/Additional/tangentlinesegmentbuilder.h \
    GraphicsPointMoving/movingdata.h \
    GraphicsPointMoving/movingpointsgroup.h \
    Helpers/constructiondata.h \
    GraphicsPointMoving/movingmanager.h \
    GraphicsPointMoving/objects.h \
    GraphicsShapesIntersections/intersectionsmanager.h \
    GraphicsShapesIntersections/objects.h \
    GraphicsShapes/constants.h \
    GraphicsShapes/graphicscircle.h \
    GraphicsShapes/graphicsline.h \
    GraphicsShapes/graphicslinesegment.h \
    GraphicsShapes/graphicspoint.h \
    GraphicsShapes/graphicspolygon.h \
    GraphicsShapes/graphicsray.h \
    GraphicsShapes/helpers.h \
    Helpers/graphicslinesegmentdata.h \
    DebugHelpers/constructioninfowriter.h \
    DebugHelpers/objects.h

SOURCES += \
    BSEHandlers/bsehandlerforcircle.cpp \
    BSEHandlers/bsehandlerforline.cpp \
    BSEHandlers/bsehandlerforlinesegment.cpp \
    BSEHandlers/bsehandlerforpoint.cpp \
    BSEHandlers/bsehandlerforscene.cpp \
    BSEHandlers/objects.cpp \
    DebugHelpers/debughelper.cpp \
    GraphicsShapesBuilders/circlebuilder.cpp \
    GraphicsShapesBuilders/linebuilder.cpp \
    GraphicsShapesBuilders/linesegmentbuilder.cpp \
    GraphicsShapesBuilders/objects.cpp \
    GraphicsShapesBuilders/pointbuilder.cpp \
    GraphicsShapesBuilders/polygonbuilder.cpp \
    Helpers/objects.cpp \
    Shapes/linesegment.cpp \
    Shapes/polygon.cpp \
    Widgets/mainwindow.cpp \
    Widgets/planeview.cpp \
    Widgets/toolbar.cpp \
    coordsystem.cpp \
    gbuilderhelper.cpp \
    gbuildernamespace.cpp \
    gvector.cpp \
    interactionobjects.cpp \
    main.cpp \
    mathnamespace.cpp \
    planescene.cpp \
    polygonalgorithms.cpp \
    GraphicsShapesBuilders/Additional/linesegmentextender.cpp \
    GraphicsShapesBuilders/Additional/linesegmentmidpointbuilder.cpp \
    GraphicsShapesBuilders/Additional/intersectionpointbuilder.cpp \
    IntersectionPointsBSEHandlers/handlerforcircle.cpp \
    IntersectionPointsBSEHandlers/objects.cpp \
    IntersectionPointsBSEHandlers/handlerforline.cpp \
    IntersectionPointsBSEHandlers/handlerforlinesegment.cpp \
    GraphicsShapesBuilders/Additional/perpendicularlinesegmentbuilder.cpp \
    GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.cpp \
    GraphicsShapesBuilders/Additional/tangentlinesegmentbuilder.cpp \
    GraphicsPointMoving/movingdata.cpp \
    GraphicsPointMoving/movingpointsgroup.cpp \
    Helpers/constructiondata.cpp \
    GraphicsPointMoving/movingmanager.cpp \
    GraphicsPointMoving/objects.cpp \
    GraphicsShapesIntersections/intersectionsmanager.cpp \
    GraphicsShapesIntersections/objects.cpp \
    GraphicsShapes/graphicscircle.cpp \
    GraphicsShapes/graphicsline.cpp \
    GraphicsShapes/graphicslinesegment.cpp \
    GraphicsShapes/graphicspoint.cpp \
    GraphicsShapes/graphicspolygon.cpp \
    GraphicsShapes/helpers.cpp \
    DebugHelpers/constructioninfowriter.cpp \
    DebugHelpers/objects.cpp
