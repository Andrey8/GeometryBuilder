#pragma once

#include <QWidget>
#include <QVector>

class QButtonGroup;
class QPushButton;
class QComboBox;
class QVBoxLayout;
class QGroupBox;
class QGraphicsScene;
class QGraphicsLineItem;
class QGraphicsPolygonItem;



class ToolBar : public QWidget
{
    Q_OBJECT

public:
    // getters
	QPushButton * GetClearPlaneSceneButton() const { return m_clearPlaneSceneButton; }
	QPushButton * GetShowMovingLineSegmentsButton() const { return m_showMovingLineSegmentsButton; }
	QPushButton * GetShowCharacteristicRegionButton() const { return m_showCharacteristicRegionButton; }
	std::list< QGraphicsLineItem * > const & GetMovingLineSegmentGraphicsItems() { return m_motionLineSegmentGraphicsItems; }
	std::list< QGraphicsPolygonItem * > const & GetCharacteristicRegionGraphicsPolygonsForInsideCase() { return m_characteristicRegionGraphicsPolygons; }

    ToolBar( QWidget * parent = nullptr );
signals:
	void NeedToShowMotionLineSegmentsForInsideCase();
	void NeedToShowMotionLineSegmentsForOutsideCase();

	void NeedToShowCharacteristicRegionForInsideCase();
	void NeedToShowCharacteristicRegionForOutsideCase();

	void NeedToRemoveMotionLineSegmentsFromScene();
	void NeedToRemoveCharacteristicFieldFromScene();
public slots:
	void PrepareToMovingLineSegmentsShowing();
	void PrepareToMovingCharacteristicField();
	void GenerateMotionLineSegmentsForInsideCase( QGraphicsScene * scene );
	void GenerateMotionLineSegmentsForOutsideCase( QGraphicsScene * scene );
	void GenerateCharacteristicRegionForInsideCase( QGraphicsScene * scene );
	void GenerateCharacteristicRegionForOutsideCase( QGraphicsScene * scene );
private slots:
    void ShowCircleTypeChoiceList();
    void HideCircleTypeChoiceList();
	void SetCircleBuildingType( int buttonid );

	void ShowPerpendicularTypeChioceList();
	void HidePerpendicularTypeChioceList();
	void SetPerpendicularBuildingType( int buttonid );

	void ShowParallelTypeChoiceList();
	void HideParallelTypeChoiceList();
	void SetParallelBuildingType( int buttonid );

	void ShowTangentShapeTypeChoiceList();
	void HideTangentShapeTypeChoiceList();
	void SetTangentShapeBuildingType( int buttonid );

	void SwitchWidget( int index );

	void ConnectWith_MovingLineSegmentsDialog();
	void DisconnectWith_MovingLineSegmentsDialog();
	void ConnectWith_CharacteristicRegionDialog();
	void DisconnectWith_CharacteristicRegionDialog();
private:
	QVBoxLayout * m_mainLayout;
	QComboBox * m_userModeSwitch;
	QWidget * m_buildingModeWidget;
	QWidget * m_polygonAlgorithmsUserModeWidget;

	QWidget * m_movingLineSegmentsAlgorithmDialog;
	QPushButton * m_showMovingLineSegmentsButton;
	std::list< QGraphicsLineItem * > m_motionLineSegmentGraphicsItems;

	QWidget * m_characteristicRegionAlgorithmDialog;
	QPushButton * m_showCharacteristicRegionButton;
	std::list< QGraphicsPolygonItem * > m_characteristicRegionGraphicsPolygons;

    QVBoxLayout * m_buildButtonGroupLayout;
	QButtonGroup * m_buildingModeButtonGroup;
	QButtonGroup * m_polygonAlgorithmsModeButtonGroup;
	QPushButton * m_addPointButton;
	QPushButton * m_addPointButtonCopy;
	QPushButton * m_drawLineSegmentButton;
	QPushButton * m_drawLineButton;

    QPushButton * m_addCircleButton;
    QGroupBox * m_circleBuildingTypeChoiceBox;
    QButtonGroup * m_circleButtonGroup;

	QPushButton * m_drawPolygonButton;
	QPushButton * m_drawPolygonButtonCopy;
	QPushButton * m_addIntersectionPointButton;
	QPushButton * m_extendLineSegmentButton;
    QPushButton * m_addLineSegmentMidpointButton;

	QPushButton * m_addPerpendicularButton;
	QGroupBox * m_perpendicularBuildingTypeChoiceBox;
	QButtonGroup * m_perpendicularButtonGroup;

	QPushButton * m_addParallelButton;
	QGroupBox * m_parallelBuildingTypeChoiceBox;
	QButtonGroup * m_parallelButtonGroup;

	QPushButton * m_addTangentButton;
	QGroupBox * m_tangentBuildingTypeChoiceBox;
	QButtonGroup * m_tangentButtonGroup;

	QPushButton * m_clearPlaneSceneButton;



	void CreateUserModeSwitch();
	void CreateBuildingModeButtonGroup();
	void CreatePolygonAlgorithmsModeButtonGroup();

    void CreateCircleTypeChoiceList();
	void CreatePerpendicularTypeChoiceList();
	void CreateParallelTypeChoiceList();
	void CreateTangentShapeTypeChoiceList();

	void CreateBuildingModeWidget();
	void CreatePolygonAlgorithmsUserModeWidget();
	void CreateMotionLineSegmentsAlgorithmDialog();
	void CreateCharacteristicRegionAlgorithmDialog();

	void SetStartingGeometry();
	void SetColors();
	void SetConnections();
};

