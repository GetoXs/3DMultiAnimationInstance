#pragma once

#include "DXUT.h"

#include <WinBase.h>

#include "Common.h"
#include "Config.h"

class AnimationContainer;

class MeshHierarchy : public ID3DXAllocateHierarchy
{
	// callback to create a D3DXFRAME-derived object and initialize it
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, 
							LPD3DXFRAME *ppNewFrame);
	// callback to create a D3DXMESHCONTAINER-derived object and initialize it
	STDMETHOD( CreateMeshContainer )( THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData,
							CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
							DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo,
							LPD3DXMESHCONTAINER * ppNewMeshContainer );
	// callback to release a D3DXFRAME-derived object
	STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );
	// callback to release a D3DXMESHCONTAINER-derived object
	STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );
public:
	MeshHierarchy();

	// Setup method
	STDMETHOD( SetMA )( THIS_ AnimationContainer *pMA );

private:
	AnimationContainer* m_pMA;
};

