#pragma once
#include "ConeObject3D.h"
class DoubleConeObject3D :
	public ConeObject3D
{
public:
	DoubleConeObject3D(float radius, unsigned radialSegments, float top_height, float bottom_height, BaseMaterial* mat, ID3DXEffect* effect = NULL);
	virtual ~DoubleConeObject3D(){};
};

