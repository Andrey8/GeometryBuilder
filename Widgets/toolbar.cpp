#include "toolbar.h"
#include "interactionobjects.cpp"
#include "DebugHelpers/debughelper.h"
#include "Shapes/polygon.h"
#include "polygonalgorithms.h"
#include "DebugHelpers/exception.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QColor>
#include <QPalette>
#include <QComboBox>
#include <QRadioButton>
#include <QListView>
#include <QStyleFactory>
#include <QMenu>
#include <QLabel>
#include <QPainter>
#include <QGraphicsScene>



ToolBar::ToolBar( QWidget * parent )
	: QWidget( parent )
{
	CreateUserModeSwitch();

	CreateBuildingModeButtonGroup();

	CreateCircleTypeChoiceList();
	CreatePerpendicularTypeChoiceList();
	CreateParallelTypeChoiceList();
	CreateTangentShapeTypeChoiceList();

	CreateBuildingModeWidget();
	CreateMotionLineSegmentsAlgorithmDialog();
	CreateCharacteristicRegionAlgorithmDialog();

	CreatePolygonAlgorithmsModeButtonGroup();
	CreatePolygonAlgorithmsUserModeWidget();

	SetStartingGeometry();
	SetColors();
	SetConnections();

	setAutoFillBackground( true );
}



void ToolBar::CreateUserModeSwitch()
{
	/*
	QAction * act1 = new QAction( tr( "Building" ) );
	QAction * act2 = new QAction( tr( "Polygon algorithms" ) );

	QMenu * menu = new QMenu;
	menu->addAction( act1 );
	menu->addAction( act2 );
	*/

	m_userModeSwitch = new QComboBox( this );
	m_userModeSwitch->setFixedHeight( 40 );
	m_userModeSwitch->addItem( "  Building" );
	m_userModeSwitch->addItem( "  Polygon algorithms" );
}

void ToolBar::CreateBuildingModeButtonGroup()
{
	m_addPointButton = new QPushButton( tr( "Add point" ) );
	m_drawLineSegmentButton = new QPushButton( tr( "Draw line segment" ) );
	m_drawLineButton = new QPushButton( tr( "Draw line" ) );
    m_addCircleButton = new QPushButton( tr( "Add circle" ) );
	m_drawPolygonButton = new QPushButton( tr( "Draw polygon" ) );
	m_addIntersectionPointButton = new QPushButton( tr( "Add intersection point" ) );
	m_extendLineSegmentButton = new QPushButton( tr( "Extend line segment" ) );
    m_addLineSegmentMidpointButton = new QPushButton( tr( "Add line segment midpoint" ) );
	m_addPerpendicularButton = new QPushButton( tr( "Add Perpendicular" ) );
	m_addParallelButton = new QPushButton( tr( "Add Parallel" ) );
	m_addTangentButton = new QPushButton( tr( "Add Tangent" ) );
	m_clearPlaneSceneButton = new QPushButton( tr( "Clear" ) );

	m_buildingModeButtonGroup = new QButtonGroup( this );
	m_buildingModeButtonGroup->addButton( m_addPointButton, 1 );
	m_buildingModeButtonGroup->addButton( m_drawLineSegmentButton, 2 );
	m_buildingModeButtonGroup->addButton( m_drawLineButton, 3 );
	m_buildingModeButtonGroup->addButton( m_addCircleButton, 4 );
	m_buildingModeButtonGroup->addButton( m_drawPolygonButton, 5 );
	m_buildingModeButtonGroup->addButton( m_addIntersectionPointButton, 6 );
	m_buildingModeButtonGroup->addButton( m_extendLineSegmentButton, 7 );
	m_buildingModeButtonGroup->addButton( m_addLineSegmentMidpointButton, 8 );
	m_buildingModeButtonGroup->addButton( m_addPerpendicularButton, 9 );
	m_buildingModeButtonGroup->addButton( m_addParallelButton, 10 );
	m_buildingModeButtonGroup->addButton( m_addTangentButton, 11 );
	m_buildingModeButtonGroup->setExclusive( true );
	for ( QAbstractButton * button : m_buildingModeButtonGroup->buttons() )
	{
		button->setCheckable( true );
	}
}

void ToolBar::CreatePolygonAlgorithmsModeButtonGroup()
{
	m_drawPolygonButtonCopy = new QPushButton( tr( "Draw polygon" ) );
	m_addPointButtonCopy = new QPushButton( tr( "Add point" ) );

	m_polygonAlgorithmsModeButtonGroup = new QButtonGroup( this );
	m_polygonAlgorithmsModeButtonGroup->addButton( m_drawPolygonButtonCopy, 1 );
	m_polygonAlgorithmsModeButtonGroup->addButton( m_addPointButtonCopy, 2 );
	m_polygonAlgorithmsModeButtonGroup->setExclusive( true );
	for ( QAbstractButton * button : m_polygonAlgorithmsModeButtonGroup->buttons() )
	{
		button->setCheckable( true );
		button->setStyleSheet( "QPushButton:checked { border : none }" );
	}
}

void ToolBar::CreateCircleTypeChoiceList()
{
	QRadioButton * rb1 = new QRadioButton( tr( "Only circle" ) );
	QRadioButton * rb2 = new QRadioButton( tr( "With center" ) );
	rb2->setChecked( true );
	QRadioButton * rb3 = new QRadioButton( tr( "Through three points" ) );

	rb1->setStyleSheet( " indicator { "
                         "width: 20px; "
                         "height: 20px; "
                         "}" );

    m_circleButtonGroup = new QButtonGroup;
	m_circleButtonGroup->addButton( rb1, 1 );
	m_circleButtonGroup->addButton( rb2, 2 );
	m_circleButtonGroup->addButton( rb3, 3 );

    QVBoxLayout * vLayout = new QVBoxLayout;
    vLayout->setSpacing( 3 );
	vLayout->addWidget( rb1 );
	vLayout->addWidget( rb2 );
	vLayout->addWidget( rb3 );

    m_circleBuildingTypeChoiceBox = new QGroupBox();
    m_circleBuildingTypeChoiceBox->setLayout( vLayout );
}

void ToolBar::CreatePerpendicularTypeChoiceList()
{
	QRadioButton * rb1 = new QRadioButton( tr( "Line Segment" ) );
	rb1->setChecked( true );
	QRadioButton * rb2 = new QRadioButton( tr( "Line" ) );

	rb1->setStyleSheet( " indicator { "
						 "width: 20px; "
						 "height: 20px; "
						 "}" );

	m_perpendicularButtonGroup = new QButtonGroup();
	m_perpendicularButtonGroup->addButton( rb1, 1 );
	m_perpendicularButtonGroup->addButton( rb2, 2 );

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->setSpacing( 3 );
	vLayout->addWidget( rb1 );
	vLayout->addWidget( rb2 );

	m_perpendicularBuildingTypeChoiceBox = new QGroupBox();
	m_perpendicularBuildingTypeChoiceBox->setLayout( vLayout );
}

void ToolBar::CreateParallelTypeChoiceList()
{
	QRadioButton * rb1 = new QRadioButton( tr( "Line Segment" ) );
	rb1->setChecked( true );
	QRadioButton * rb2 = new QRadioButton( tr( "Line" ) );

	rb1->setStyleSheet( " indicator { "
						 "width: 20px; "
						 "height: 20px; "
						 "}" );

	m_parallelButtonGroup = new QButtonGroup();
	m_parallelButtonGroup->addButton( rb1, 1 );
	m_parallelButtonGroup->addButton( rb2, 2 );

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->setSpacing( 3 );
	vLayout->addWidget( rb1 );
	vLayout->addWidget( rb2 );

	m_parallelBuildingTypeChoiceBox = new QGroupBox();
	m_parallelBuildingTypeChoiceBox->setLayout( vLayout );
}

void ToolBar::CreateTangentShapeTypeChoiceList()
{
	QRadioButton * rb1 = new QRadioButton( tr( "Line Segments" ) );
	rb1->setChecked( true );
	QRadioButton * rb2 = new QRadioButton( tr( "Lines" ) );

	rb1->setStyleSheet( " indicator { "
						 "width: 20px; "
						 "height: 20px; "
						 "}" );

	m_tangentButtonGroup = new QButtonGroup();
	m_tangentButtonGroup->addButton( rb1, 1 );
	m_tangentButtonGroup->addButton( rb2, 2 );

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->setSpacing( 3 );
	vLayout->addWidget( rb1 );
	vLayout->addWidget( rb2 );

	m_tangentBuildingTypeChoiceBox = new QGroupBox();
	m_tangentBuildingTypeChoiceBox->setLayout( vLayout );
}

void ToolBar::CreateBuildingModeWidget()
{
	m_buildButtonGroupLayout = new QVBoxLayout;
	m_buildButtonGroupLayout->setSpacing( 5 );
	m_buildButtonGroupLayout->addWidget( m_addPointButton );
	m_buildButtonGroupLayout->addWidget( m_drawLineSegmentButton );
	m_buildButtonGroupLayout->addWidget( m_drawLineButton );
	m_buildButtonGroupLayout->addWidget( m_addCircleButton );
	m_buildButtonGroupLayout->addWidget( m_drawPolygonButton );
	m_buildButtonGroupLayout->addWidget( m_addIntersectionPointButton );
	m_buildButtonGroupLayout->addWidget( m_extendLineSegmentButton );
	m_buildButtonGroupLayout->addWidget( m_addLineSegmentMidpointButton );
	m_buildButtonGroupLayout->addWidget( m_addPerpendicularButton );
	m_buildButtonGroupLayout->addWidget( m_addParallelButton );
	m_buildButtonGroupLayout->addWidget( m_addTangentButton );

	QVBoxLayout * buildingModeWidgetLayout = new QVBoxLayout;
	buildingModeWidgetLayout->setSpacing( 10 );
	buildingModeWidgetLayout->addLayout( m_buildButtonGroupLayout );
	buildingModeWidgetLayout->addWidget( m_clearPlaneSceneButton );

	m_buildingModeWidget = new QWidget();
	m_buildingModeWidget->setLayout( buildingModeWidgetLayout );
}

void ToolBar::CreatePolygonAlgorithmsUserModeWidget()
{
	QVBoxLayout * buttonGroupLayout = new QVBoxLayout;
	buttonGroupLayout->setSpacing( 5 );
	buttonGroupLayout->addWidget( m_drawPolygonButtonCopy );
	buttonGroupLayout->addWidget( m_addPointButtonCopy );

	QAction * act1 = new QAction( tr( "Motion line segments" ) );
	QAction * act2 = new QAction( tr( "Characteristic region" ) );
	QMenu * menu = new QMenu();
	menu->addAction( act1 );
	menu->addAction( act2 );
	QPushButton * algorithmsButton = new QPushButton( tr( "Algorithms" ) );
	algorithmsButton->setMenu( menu );

	QVBoxLayout * polygonAlgorithmsModeWidgetLayout = new QVBoxLayout;
	polygonAlgorithmsModeWidgetLayout->setSpacing( 10 );
	polygonAlgorithmsModeWidgetLayout->addLayout( buttonGroupLayout );
	polygonAlgorithmsModeWidgetLayout->addWidget( algorithmsButton );

	m_polygonAlgorithmsUserModeWidget = new QWidget();
	m_polygonAlgorithmsUserModeWidget->setLayout( polygonAlgorithmsModeWidgetLayout );



	connect( act1, SIGNAL( triggered() ),
			 this, SLOT( ConnectWith_MovingLineSegmentsDialog() ) );
	connect( act2, SIGNAL( triggered() ),
			 this, SLOT( ConnectWith_CharacteristicRegionDialog() ) );
}

void ToolBar::CreateMotionLineSegmentsAlgorithmDialog()
{
	QLabel * messageLabel = new QLabel( tr( "Select first polygon." ) );
	messageLabel->setAlignment( Qt::AlignHCenter );

	QAction * act1 = new QAction( tr( "Inside case" ) );
	QAction * act2 = new QAction( tr( "Outside case" ) );
	QMenu * menu = new QMenu();
	menu->addAction( act1 );
	menu->addAction( act2 );
	m_showMovingLineSegmentsButton = new QPushButton( tr( "Show" ) );
	m_showMovingLineSegmentsButton->setDisabled( true );
	m_showMovingLineSegmentsButton->setMenu( menu );
	QPushButton * cancelButton = new QPushButton( tr( "Cancel" ) );
	QHBoxLayout * hlayout = new QHBoxLayout;
	hlayout->setMargin( 20 );
	hlayout->addWidget( m_showMovingLineSegmentsButton );
	hlayout->addSpacing( 5 );
	hlayout->addWidget( cancelButton );

	QVBoxLayout * vlayout = new QVBoxLayout;
	vlayout->addWidget( messageLabel );
	vlayout->addSpacing( 10 );
	vlayout->addLayout( hlayout );

	m_movingLineSegmentsAlgorithmDialog = new QWidget;
	m_movingLineSegmentsAlgorithmDialog->setLayout( vlayout );



	connect( act1, SIGNAL( triggered() ),
			 this, SIGNAL( NeedToShowMotionLineSegmentsForInsideCase() ) );
	connect( act2, SIGNAL( triggered() ),
			 this, SIGNAL( NeedToShowMotionLineSegmentsForOutsideCase() ) );
	connect( cancelButton, SIGNAL( clicked() ),
			 this, SLOT( DisconnectWith_MovingLineSegmentsDialog() ) );
}

void ToolBar::CreateCharacteristicRegionAlgorithmDialog()
{
	QLabel * messageLabel = new QLabel( tr( "Select first polygon." ) );
	messageLabel->setAlignment( Qt::AlignHCenter );

	QAction * act1 = new QAction( tr( "Inside case" ) );
	QAction * act2 = new QAction( tr( "Outside case" ) );
	QMenu * menu = new QMenu();
	menu->addAction( act1 );
	menu->addAction( act2 );
	m_showCharacteristicRegionButton = new QPushButton( tr( "Show" ) );
	m_showCharacteristicRegionButton->setDisabled( true );
	m_showCharacteristicRegionButton->setMenu( menu );
	QPushButton * cancelButton = new QPushButton( tr( "Cancel" ) );
	QHBoxLayout * hlayout = new QHBoxLayout;
	hlayout->setMargin( 20 );
	hlayout->addWidget( m_showCharacteristicRegionButton );
	hlayout->addSpacing( 5 );
	hlayout->addWidget( cancelButton );

	QVBoxLayout * vlayout = new QVBoxLayout;
	vlayout->addWidget( messageLabel );
	vlayout->addSpacing( 10 );
	vlayout->addLayout( hlayout );

	m_characteristicRegionAlgorithmDialog = new QWidget;
	m_characteristicRegionAlgorithmDialog->setLayout( vlayout );



	connect( act1, SIGNAL( triggered() ),
			 this, SIGNAL( NeedToShowCharacteristicRegionForInsideCase() ) );
	connect( act2, SIGNAL( triggered() ),
			 this, SIGNAL( NeedToShowCharacteristicRegionForOutsideCase() ) );
	connect( cancelButton, SIGNAL( clicked() ),
			 this, SLOT( DisconnectWith_CharacteristicRegionDialog() ) );
}



void ToolBar::SetStartingGeometry()
{    
	m_mainLayout = new QVBoxLayout();
	m_mainLayout->setAlignment( Qt::AlignTop );

	m_mainLayout->addWidget( m_userModeSwitch );
	m_mainLayout->addSpacing( 50 );
	m_mainLayout->addWidget( m_buildingModeWidget );

	setLayout( m_mainLayout );
}

void ToolBar::SetColors()
{
    QPalette newPalette;
    newPalette.setColor( QPalette::Background, QColor( Qt::blue ) );
	setPalette( newPalette );

	QListView * lv = new QListView();
	lv->setStyleSheet(
					   "QListView::item { "
					   "background-color : blue;"
					   "height : 40px; }"
					   "QListView::item:selected { background-color : green; }" );
	m_userModeSwitch->setView( lv );
	m_userModeSwitch->setStyle( QStyleFactory::create( "Fusion" ) );
	QPalette p;
	p.setColor( QPalette::Button, QColor( Qt::blue ) );
	m_userModeSwitch->setPalette( p );

	for ( QAbstractButton * button : m_buildingModeButtonGroup->buttons() )
	{
		button->setStyleSheet( "QPushButton:checked { border : none }" );
	}
}

void ToolBar::SetConnections()
{
	connect( m_userModeSwitch, SIGNAL( currentIndexChanged( int ) ),
			 this, SLOT( SwitchWidget( int ) ) );

	connect( m_buildingModeButtonGroup, SIGNAL( buttonClicked( int ) ),
			 &InteractionObjects::buildingButtonsData, SLOT( SetPushedBuildingButton_BuildingMode( int ) ) );
	connect( m_polygonAlgorithmsModeButtonGroup, SIGNAL( buttonClicked( int ) ),
			 &InteractionObjects::buildingButtonsData, SLOT( SetPushedBuildingButton_PolygonAlgorithmsMode( int ) ) );

    connect( m_addCircleButton, SIGNAL( clicked() ),
             this, SLOT( ShowCircleTypeChoiceList() ) );
    connect( m_addCircleButton, SIGNAL( toggled( bool ) ),
             this, SLOT( HideCircleTypeChoiceList() ) );
    connect( m_circleButtonGroup, SIGNAL( buttonClicked( int ) ),
             this, SLOT( SetCircleBuildingType( int ) ) );

	connect( m_addPerpendicularButton, SIGNAL( clicked() ),
			 this, SLOT( ShowPerpendicularTypeChioceList() ) );
	connect( m_addPerpendicularButton, SIGNAL( toggled( bool ) ),
			 this, SLOT( HidePerpendicularTypeChioceList() ) );
	connect( m_perpendicularButtonGroup, SIGNAL( buttonClicked( int ) ),
			 this, SLOT( SetPerpendicularBuildingType( int ) ) );

	connect( m_addParallelButton, SIGNAL( clicked() ),
			 this, SLOT( ShowParallelTypeChoiceList() ) );
	connect( m_addParallelButton, SIGNAL( toggled( bool ) ),
			 this, SLOT( HideParallelTypeChoiceList() ) );
	connect( m_parallelButtonGroup, SIGNAL( buttonClicked( int ) ),
			 this, SLOT( SetParallelBuildingType( int ) ) );

	connect( m_addTangentButton, SIGNAL( clicked() ),
			 this, SLOT( ShowTangentShapeTypeChoiceList() ) );
	connect( m_addTangentButton, SIGNAL( toggled( bool ) ),
			 this, SLOT( HideTangentShapeTypeChoiceList() ) );
	connect( m_tangentButtonGroup, SIGNAL( buttonClicked( int ) ),
			 this, SLOT( SetTangentShapeBuildingType( int ) ) );
}



void ToolBar::PrepareToMovingLineSegmentsShowing()
{
	m_showMovingLineSegmentsButton->setEnabled( true );
}

void ToolBar::PrepareToMovingCharacteristicField()
{
	m_showCharacteristicRegionButton->setEnabled( true );
}

void ToolBar::GenerateMotionLineSegmentsForInsideCase( QGraphicsScene * scene )
{
	InteractionObjects::TwoPolygonsSelecter & selecter = InteractionObjects::GetTwoPolygonsSelecter();
	try
	{
		GeomShapes::Polygon const pol1( selecter.GetFirstPolygon()->GetVerticesPositions() );
		GeomShapes::Polygon const pol2( selecter.GetSecondPolygon()->GetVerticesPositions() );
		QPointF const p( selecter.GetPoint()->GetPosition() );

		std::list< GeomShapes::LineSegment > const lineSegments = PolygonAlgorithms::GetMotionLineSegmentsForInside( pol1, pol2, p, Math::CircleDirection::Clockwise );

		std::list< QGraphicsLineItem * > lineItems;
		for ( GeomShapes::LineSegment const & ls : lineSegments )
		{
			QGraphicsLineItem * item = scene->addLine( ls.GetQLineF(), QPen( QBrush( Qt::lightGray ), 4, Qt::DashLine ) );
			item->setZValue( 8 );

			lineItems.push_back( item );
		}
		m_motionLineSegmentGraphicsItems = lineItems;
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void ToolBar::GenerateMotionLineSegmentsForOutsideCase( QGraphicsScene * scene )
{
	InteractionObjects::TwoPolygonsSelecter & selecter = InteractionObjects::GetTwoPolygonsSelecter();
	try
	{
		GeomShapes::Polygon const pol1( selecter.GetFirstPolygon()->GetVerticesPositions() );
		GeomShapes::Polygon const pol2( selecter.GetSecondPolygon()->GetVerticesPositions() );
		QPointF const p( selecter.GetPoint()->GetPosition() );

		std::list< GeomShapes::LineSegment > const lineSegments = PolygonAlgorithms::GetMotionLineSegmentsForOutside( pol1, pol2, p, Math::CircleDirection::Clockwise );

		std::list< QGraphicsLineItem * > lineItems;
		for ( GeomShapes::LineSegment const & ls : lineSegments )
		{
			QGraphicsLineItem * item = scene->addLine( ls.GetQLineF(), QPen( QBrush( Qt::lightGray ), 4, Qt::DashLine ) );
			item->setZValue( 8 );

			lineItems.push_back( item );
		}
		m_motionLineSegmentGraphicsItems = lineItems;
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void ToolBar::GenerateCharacteristicRegionForInsideCase( QGraphicsScene * scene )
{
	InteractionObjects::TwoPolygonsSelecter & selecter = InteractionObjects::GetTwoPolygonsSelecter();
	try
	{
		GeomShapes::Polygon const pol1( selecter.GetFirstPolygon()->GetVerticesPositions() );
		GeomShapes::Polygon const pol2( selecter.GetSecondPolygon()->GetVerticesPositions() );
		QPointF const p( selecter.GetPoint()->GetPosition() );

		std::list< GeomShapes::Polygon > const polygons = PolygonAlgorithms::GetCharacteristicRegionForInside( pol1, pol2, p, Math::CircleDirection::Clockwise );

		std::list< QGraphicsPolygonItem * > polygonItems;
		for ( GeomShapes::Polygon const & pol : polygons )
		{
			QGraphicsPolygonItem * item = scene->addPolygon( GeomShapes::ToPolygonF( pol ), QPen( QBrush( Qt::darkGray ), 4 ), QBrush( Qt::lightGray ) );
			item->setZValue( 8 );

			polygonItems.push_back( item );
		}
		m_characteristicRegionGraphicsPolygons = polygonItems;
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void ToolBar::GenerateCharacteristicRegionForOutsideCase( QGraphicsScene * scene )
{
	InteractionObjects::TwoPolygonsSelecter & selecter = InteractionObjects::GetTwoPolygonsSelecter();
	try
	{
		GeomShapes::Polygon const pol1( selecter.GetFirstPolygon()->GetVerticesPositions() );
		GeomShapes::Polygon const pol2( selecter.GetSecondPolygon()->GetVerticesPositions() );
		QPointF const p( selecter.GetPoint()->GetPosition() );

		std::list< GeomShapes::Polygon > const polygons = PolygonAlgorithms::GetComplementToCharacteristicRegionForOutside( pol1, pol2, p, Math::CircleDirection::Clockwise );

		std::list< QGraphicsPolygonItem * > polygonItems;
		for ( GeomShapes::Polygon const & pol : polygons )
		{
			QGraphicsPolygonItem * item = scene->addPolygon( GeomShapes::ToPolygonF( pol ), QPen( QBrush( Qt::darkGray ), 4 ), QBrush( Qt::lightGray ) );
			item->setZValue( 0 );

			polygonItems.push_back( item );
		}
		m_characteristicRegionGraphicsPolygons = polygonItems;
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}



void ToolBar::ShowCircleTypeChoiceList()
{
    m_buildButtonGroupLayout->insertWidget( 4, m_circleBuildingTypeChoiceBox );
    m_circleBuildingTypeChoiceBox->show();
}

void ToolBar::HideCircleTypeChoiceList()
{
    m_buildButtonGroupLayout->removeWidget( m_circleBuildingTypeChoiceBox );
    m_circleBuildingTypeChoiceBox->hide();
}

void ToolBar::SetCircleBuildingType( int buttonId )
{
    switch ( buttonId )
    {
    case 1 :
    {
        InteractionObjects::GetCircleBuildingType() = GB::CircleBuildingType::OnlyCircle;

        break;
    }
    case 2 :
    {
        InteractionObjects::GetCircleBuildingType() = GB::CircleBuildingType::WithCenter;

        break;
    }
    case 3 :
    {
        InteractionObjects::GetCircleBuildingType() = GB::CircleBuildingType::ThroughThreePoints;

        break;
    }
    }
}

void ToolBar::ShowPerpendicularTypeChioceList()
{
	m_buildButtonGroupLayout->insertWidget( 9, m_perpendicularBuildingTypeChoiceBox );
	m_perpendicularBuildingTypeChoiceBox->show();
}

void ToolBar::HidePerpendicularTypeChioceList()
{
	m_buildButtonGroupLayout->removeWidget( m_perpendicularBuildingTypeChoiceBox );
	m_perpendicularBuildingTypeChoiceBox->hide();
}

void ToolBar::SetPerpendicularBuildingType( int buttonid )
{
	switch ( buttonid )
	{
	case 1 :
	{
		InteractionObjects::GetPerpendicularBuildingType() =
				GB::PerpendicularBuildingType::LineSegment;

		break;
	}
	case 2 :
	{
		InteractionObjects::GetPerpendicularBuildingType() =
				GB::PerpendicularBuildingType::Line;

		break;
	}
	}
}

void ToolBar::ShowParallelTypeChoiceList()
{
	m_buildButtonGroupLayout->insertWidget( 10, m_parallelBuildingTypeChoiceBox );
	m_parallelBuildingTypeChoiceBox->show();
}

void ToolBar::HideParallelTypeChoiceList()
{
	m_buildButtonGroupLayout->removeWidget( m_parallelBuildingTypeChoiceBox );
	m_parallelBuildingTypeChoiceBox->hide();
}

void ToolBar::SetParallelBuildingType( int buttonid )
{
	switch ( buttonid )
	{
	case 1 :
	{
		InteractionObjects::GetParallelBuildingType() =
				GB::ParallelBuildingType::LineSegment;

		break;
	}
	case 2 :
	{
		InteractionObjects::GetParallelBuildingType() =
				GB::ParallelBuildingType::Line;

		break;
	}
	}
}

void ToolBar::ShowTangentShapeTypeChoiceList()
{
	m_buildButtonGroupLayout->insertWidget( 13, m_tangentBuildingTypeChoiceBox );
	m_tangentBuildingTypeChoiceBox->show();
}

void ToolBar::HideTangentShapeTypeChoiceList()
{
	m_buildButtonGroupLayout->removeWidget( m_tangentBuildingTypeChoiceBox );
	m_tangentBuildingTypeChoiceBox->hide();
}

void ToolBar::SetTangentShapeBuildingType( int buttonid )
{
	switch ( buttonid )
	{
	case 1 :
	{
		InteractionObjects::GetTangentShapeBuildingType() =
				GB::TangentShapeBuildingType::LineSegments;

		break;
	}
	case 2 :
	{
		InteractionObjects::GetTangentShapeBuildingType() =
				GB::TangentShapeBuildingType::Lines;

		break;
	}
	}
}



void ToolBar::SwitchWidget( int index )
{
	switch ( index )
	{
	case 0 :
	{
		//m_mainLayout->removeWidget( m_polygonAlgorithmsUserModeWidget );
		m_polygonAlgorithmsUserModeWidget->hide();
		m_mainLayout->insertWidget( 2, m_buildingModeWidget );
		m_buildingModeWidget->show();

		InteractionObjects::buildingButtonsData.SetPushedBuildingButton(
					InteractionObjects::buildingButtonsData.GetPushedBuildingButtonInBuildingMode() );

		break;
	}
	case 1 :
	{
		//m_mainLayout->removeWidget( m_buildingModeWidget );
		m_buildingModeWidget->hide();
		m_mainLayout->insertWidget( 2, m_polygonAlgorithmsUserModeWidget );
		m_polygonAlgorithmsUserModeWidget->show();

		InteractionObjects::buildingButtonsData.SetPushedBuildingButton(
					InteractionObjects::buildingButtonsData.GetPushedBuildingButtonInPolygonAlgorithmsMode() );

		break;
	}
	}
}

void ToolBar::ConnectWith_MovingLineSegmentsDialog()
{
	InteractionObjects::userActionMode = GB::UserActionMode::PolygonSelection;

	m_mainLayout->addWidget( m_movingLineSegmentsAlgorithmDialog );
	m_movingLineSegmentsAlgorithmDialog->show();

	m_userModeSwitch->setDisabled( true );
	m_polygonAlgorithmsUserModeWidget->setDisabled( true );
}

void ToolBar::DisconnectWith_MovingLineSegmentsDialog()
{
	InteractionObjects::GetTwoPolygonsSelecter().Deselect();

	emit NeedToRemoveMotionLineSegmentsFromScene();
	m_motionLineSegmentGraphicsItems.clear();

	InteractionObjects::userActionMode = GB::UserActionMode::Building;

	m_movingLineSegmentsAlgorithmDialog->hide();
	m_userModeSwitch->setEnabled( true );
	m_polygonAlgorithmsUserModeWidget->setEnabled( true );
	m_showMovingLineSegmentsButton->setDisabled( true );
}

void ToolBar::ConnectWith_CharacteristicRegionDialog()
{
	InteractionObjects::userActionMode = GB::UserActionMode::PolygonSelection;

	m_mainLayout->addWidget( m_characteristicRegionAlgorithmDialog );
	m_characteristicRegionAlgorithmDialog->show();

	m_userModeSwitch->setDisabled( true );
	m_polygonAlgorithmsUserModeWidget->setDisabled( true );
}

void ToolBar::DisconnectWith_CharacteristicRegionDialog()
{
	InteractionObjects::GetTwoPolygonsSelecter().Deselect();

	emit NeedToRemoveCharacteristicFieldFromScene();

	InteractionObjects::userActionMode = GB::UserActionMode::Building;

	m_characteristicRegionAlgorithmDialog->hide();
	m_userModeSwitch->setEnabled( true );
	m_polygonAlgorithmsUserModeWidget->setEnabled( true );
	m_showCharacteristicRegionButton->setDisabled( true );
}






