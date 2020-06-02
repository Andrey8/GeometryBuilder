#include "mainwindow.h"
#include "planeview.h"
#include "toolbar.h"
#include "planescene.h"
#include "DebugHelpers/debughelper.h"
#include "gbuildernamespace.h"
#include "interactionobjects.h"

#include <QPoint>
#include <QPalette>
#include <QSize>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsLineItem>



MainWindow::MainWindow( QWidget * parent )
    : QWidget( parent ),
      m_planeView( new PlaneView( this ) ),
      m_toolBar( new ToolBar( this ) )
{
    SetGeometry();
    SetColors();
    SetConnections();

    setAutoFillBackground( true );

    GB::mainWindowGeometryData.SetMainWindowRect( rect() );
    GB::mainWindowGeometryData.SetPlaneViewSize( m_planeView->size() );

    setWindowTitle( tr( "Geometry Builder" ) );
}



void MainWindow::SetGeometry()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->addWidget( m_planeView );
    mainLayout->addWidget( m_toolBar );
    mainLayout->setSpacing( 20 );
    mainLayout->setMargin( 20 );

    m_planeView->setMinimumWidth( 1000 );

    m_toolBar->setFixedWidth( 300 );

    setLayout( mainLayout );
    setGeometry( QRect( 50, 100, 1500, 800 ) );
}

void MainWindow::SetColors()
{
    // setStyleSheet( "background-color : green" );

    QPalette newPalette;
    newPalette.setColor( QPalette::Background, QColor( Qt::green ) );

    setPalette( newPalette );
}

void MainWindow::SetConnections()
{
    connect( m_toolBar->GetClearPlaneSceneButton(), SIGNAL( clicked() ),
             m_planeView->GetScene(), SLOT( Clear() ) );
    connect( m_planeView->GetScene(), SIGNAL( MovingLineSegmentsShowingPrepared() ),
             m_toolBar, SLOT( PrepareToMovingLineSegmentsShowing() ) );
    connect( m_planeView->GetScene(), SIGNAL( CharacteristicFieldShowingPrepared() ),
             m_toolBar, SLOT( PrepareToMovingCharacteristicField() ) );
    connect( m_toolBar, SIGNAL( NeedToShowMotionLineSegmentsForInsideCase() ),
             this, SLOT( ShowMotionLineSegmentsForInsideCase() ) );
    connect( m_toolBar, SIGNAL( NeedToShowMotionLineSegmentsForOutsideCase() ),
             this, SLOT( ShowMotionLineSegmentsForOutsideCase() ) );
    connect( m_toolBar, SIGNAL( NeedToShowCharacteristicRegionForInsideCase() ),
             this, SLOT( ShowCharacteristicRegionForInsideCase() ) );
    connect( m_toolBar, SIGNAL( NeedToShowCharacteristicRegionForOutsideCase() ),
             this, SLOT( ShowCharacteristicRegionForOutsideCase() ) );
    connect( m_toolBar, SIGNAL( NeedToRemoveMotionLineSegmentsFromScene() ),
             this, SLOT( RemoveMotionLineSegmentsFromScene() ) );
    connect( m_toolBar, SIGNAL( NeedToRemoveCharacteristicFieldFromScene() ),
             this, SLOT( RemoveCharacteristicRegionFromScene() ) );
}



void MainWindow::resizeEvent( QResizeEvent * event )
{
    QWidget::resizeEvent( event );

    GB::mainWindowGeometryData.SetMainWindowRect( rect() );
    GB::mainWindowGeometryData.SetPlaneViewSize( m_planeView->size() );
}



void MainWindow::ShowMotionLineSegmentsForInsideCase()
{
    m_toolBar->GenerateMotionLineSegmentsForInsideCase( m_planeView->GetScene() );

    m_toolBar->GetShowMovingLineSegmentsButton()->setDisabled( true );
    InteractionObjects::GetTwoPolygonsSelecter().DeselectPolygons();
}

void MainWindow::ShowMotionLineSegmentsForOutsideCase()
{
    m_toolBar->GenerateMotionLineSegmentsForOutsideCase( m_planeView->GetScene() );

    m_toolBar->GetShowMovingLineSegmentsButton()->setDisabled( true );
    InteractionObjects::GetTwoPolygonsSelecter().DeselectPolygons();
}

void MainWindow::RemoveMotionLineSegmentsFromScene()
{
    for ( QGraphicsLineItem * item : m_toolBar->GetMovingLineSegmentGraphicsItems() )
    {
        m_planeView->GetScene()->removeItem( item );
    }
}

void MainWindow::ShowCharacteristicRegionForInsideCase()
{
    m_toolBar->GenerateCharacteristicRegionForInsideCase( m_planeView->GetScene() );

    m_toolBar->GetShowCharacteristicRegionButton()->setDisabled( true );
    InteractionObjects::GetTwoPolygonsSelecter().DeselectPolygons();
}

void MainWindow::ShowCharacteristicRegionForOutsideCase()
{
    m_toolBar->GenerateCharacteristicRegionForOutsideCase( m_planeView->GetScene() );

    m_toolBar->GetShowCharacteristicRegionButton()->setDisabled( true );
    InteractionObjects::GetTwoPolygonsSelecter().DeselectPolygons();
}

void MainWindow::RemoveCharacteristicRegionFromScene()
{
    for ( QGraphicsPolygonItem * item : m_toolBar->GetCharacteristicRegionGraphicsPolygonsForInsideCase() )
    {
        m_planeView->GetScene()->removeItem( item );
    }
}




