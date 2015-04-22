/************************************************************************
							XModel3D.h
Author: Johnathan O'Malia © 2015
Description:	Model Mesh Storage class

TODO:
/************************************************************************/
#ifndef XMODEL3D_H_
#define XMODEL3D_H_

#include "MeshObject3D.h"

class XModel3D : public MeshObject3D
{
public:
	XModel3D(LPD3DXMESH pMesh);
	~XModel3D(){};

protected:
	virtual void buildMeshBuffers(){};
	virtual void buildUVBuffer(){};
};

#endif // !XMODEL3D_H_