#pragma once

class GraphicsLineSegment;
class GraphicsLine;



namespace GSBuilders
{
	namespace Additional
	{
		class LineSegmentExtender
		{
		public:
			LineSegmentExtender()
				: m_lineSegment( nullptr ),
				  m_line( nullptr )
			{}

			void SetLineSegment( GraphicsLineSegment * ls );
			void CreateLine();
			GraphicsLine * GetLine();

			void DestroyLineSegment();

			void Reset();
		private:
			GraphicsLineSegment * m_lineSegment;
			GraphicsLine * m_line;
		};
	}
}
