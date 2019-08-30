#pragma once
#include "DXUT.h"


#include "Common.h"
#include "Config.h"

using namespace std;

class MeshHierarchy;
class AnimationInstance;

class AnimationContainer
{
protected:



	std::vector <AnimationInstance*> m_v_pAnimInstances;     // must be at lesat 1; otherwise, clear all

	LPD3DXANIMATIONCONTROLLER m_pAC;                  // AC that all children clone from -- to clone clean, no keys

	// useful data an app can retrieve
	float m_fBoundingRadius;

private:

	HRESULT             CreateInstance( AnimationInstance** ppAnimInstance );
	HRESULT             SetupBonePtrs( D3DXFRAME* pFrame );

public:
	D3DXFRAME* m_pFrameRoot;           // shared between all instances
	LPDIRECT3DDEVICE9 m_pDevice;

	LPD3DXEFFECT m_pEffect;
	char* m_sTechnique;           // character rendering technique

	DWORD m_dwWorkingPaletteSize;
	D3DXMATRIX* m_amxWorkingPalette;

						AnimationContainer();
	virtual             ~AnimationContainer();

	virtual HRESULT     Setup( LPDIRECT3DDEVICE9 pDevice, TCHAR sXFile[], TCHAR sFxFile[],
							   MeshHierarchy* pAH, LPD3DXLOADUSERDATA pLUD = NULL );
	virtual HRESULT     Cleanup( MeshHierarchy* pAH );

	LPDIRECT3DDEVICE9   GetDevice();
	D3DXMATRIX*			GetWorkingPalette();
	LPD3DXEFFECT        GetEffect();
	DWORD               GetNumInstances();
	AnimationInstance*		GetInstance( DWORD dwIdx );
	float               GetBoundingRadius();

	virtual HRESULT     CreateNewInstance( DWORD* pdwNewIdx );

	virtual void        SetTechnique( char* sTechnique );

	virtual HRESULT     Draw();

};

struct AnimationMeshContainer : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* m_apTextures;
	LPD3DXMESH m_pWorkingMesh;
	D3DXMATRIX* m_amxBoneOffsets;  // Bone offset matrices retrieved from pSkinInfo
	D3DXMATRIX** m_apmxBonePointers;  // Provides index to bone matrix lookup

	DWORD m_dwNumPaletteEntries;
	DWORD m_dwMaxNumFaceInfls;
	DWORD m_dwNumAttrGroups;
	LPD3DXBUFFER m_pBufBoneCombos;

	HRESULT SetupBonePtrs( D3DXFRAME* pFrameRoot );
};

