#pragma once

#include <QWidget>

class PlaneView;
class ToolBar;

class QResizeEvent;



class MainWindow : public QWidget
{
    Q_OBJECT

public:
	void resizeEvent( QResizeEvent * event ) override;

	explicit MainWindow( QWidget * parent = nullptr );
private slots:
	void ShowMotionLineSegmentsForInsideCase();
	void ShowMotionLineSegmentsForOutsideCase();

	void ShowCharacteristicRegionForInsideCase();
	void ShowCharacteristicRegionForOutsideCase();

	void RemoveMotionLineSegmentsFromScene();
	void RemoveCharacteristicRegionFromScene();
private:
    PlaneView * m_planeView;
    ToolBar * m_toolBar;

    void SetGeometry();
	void SetColors();
	void SetConnections();
};
