#pragma once

#include <QObject>

#include "gbuildernamespace.h"

class GraphicsPolygon;



namespace InteractionObjects
{
	class BuildingButtonsData : public QObject
	{
		Q_OBJECT

	public:
		// getters
		GB::PushedBuildingButton GetPushedBuildingButton() const { return m_button; }
		GB::PushedBuildingButton GetPushedBuildingButtonInBuildingMode() const { return m_buttonInBuildingMode; }
		GB::PushedBuildingButton GetPushedBuildingButtonInPolygonAlgorithmsMode() const { return m_buttonInPolygonAlgorithmsMode; }

		// setters
		void SetPushedBuildingButton( GB::PushedBuildingButton b ) { m_button = b; }

		BuildingButtonsData()
			: m_buttonInBuildingMode( GB::PushedBuildingButton::None ),
			  m_buttonInPolygonAlgorithmsMode( GB::PushedBuildingButton::None ),
			  m_button( GB::PushedBuildingButton::None )
		{}
	public slots:
		void SetPushedBuildingButton_BuildingMode( int buttonId );
		void SetPushedBuildingButton_PolygonAlgorithmsMode( int buttonId );
	private:
		GB::PushedBuildingButton m_buttonInBuildingMode;
		GB::PushedBuildingButton m_buttonInPolygonAlgorithmsMode;

		GB::PushedBuildingButton m_button;

		void SetPushedBuildingButton( int buttonId, GB::BuildingButtonGroupObject group );
	};
	extern BuildingButtonsData buildingButtonsData;

	extern GB::UserActionMode userActionMode;

	class PointSelecter
	{
	public:

	private:

	};

	class TwoPolygonsSelecter
	{
	public:
		TwoPolygonsSelecter() : m_pol1( nullptr ), m_pol2( nullptr ) {}

		void SetFirstPolygon( GraphicsPolygon * pol ) { m_pol1 = pol; }
		void SetSecondPolygon( GraphicsPolygon * pol ) { m_pol2 = pol; }
		void SetPoint( GraphicsPoint * p ) { m_p = p; }

		GraphicsPolygon * GetFirstPolygon() const { return m_pol1; }
		GraphicsPolygon * GetSecondPolygon() const { return m_pol2; }
		GraphicsPoint * GetPoint() const { return m_p; }

		bool PolygonsAreSelected() const { return ( m_pol1 && m_pol2 ); }
		bool FirstPolygonIsSelected() const { return ( m_pol1 != nullptr ); }

		void Reset()
		{
			m_pol1 = nullptr;
			m_pol2 = nullptr;
			m_p = nullptr;
		}

		void Deselect();
		void DeselectPolygons();

	private:
		GraphicsPolygon * m_pol1;
		GraphicsPolygon * m_pol2;
		GraphicsPoint * m_p;
	};

    GB::CircleBuildingType & GetCircleBuildingType();
	GB::PerpendicularBuildingType & GetPerpendicularBuildingType();
	GB::ParallelBuildingType & GetParallelBuildingType();
	GB::TangentShapeBuildingType & GetTangentShapeBuildingType();

	TwoPolygonsSelecter & GetTwoPolygonsSelecter();
}
