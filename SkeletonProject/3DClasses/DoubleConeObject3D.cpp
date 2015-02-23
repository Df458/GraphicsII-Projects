#include "DoubleConeObject3D.h"


DoubleConeObject3D::DoubleConeObject3D(float radius, unsigned radialSegments, float top_height, float bottom_height, BaseMaterial* mat, ID3DXEffect* effect)
	: ConeObject3D(radius, radialSegments, top_height, mat, effect)
{
	m_Negative_Height = bottom_height;
}
