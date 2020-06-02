#pragma once

#include "gbuildernamespace.h"

#include <QGraphicsView>

class QWidget;
class QPaintEvent;
class QMouseEvent;
class QPainter;
class QRectF;

class PlaneScene;
class GraphicsLine;



class PlaneView : public QGraphicsView
{
    Q_OBJECT

public:
    // getters
	PlaneScene * GetScene() const { return m_scene; }

	PlaneView( QWidget * parent = nullptr );
protected:
    void mouseMoveEvent( QMouseEvent * event ) override;
private:
	PlaneScene * m_scene;

	void TestingPaint();
};
