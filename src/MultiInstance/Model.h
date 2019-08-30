#pragma once

#include "DXUT.h"
#include "Common.h"
#include "AnimationContainer.h"
#include "AnimationInstance.h"

class Model
{
	AnimationContainer* m_pMA;               // pointer to mesh-type-specific object
	DWORD m_dwMultiAnimIdx;    // index identifying which CAnimInstance this object uses
	AnimationInstance* m_pAI;               // pointer to CAnimInstance specific to this object
	
	D3DXMATRIX m_mxOrientation;     // transform that gets the mesh into a common world space
	D3DXMATRIX m_mxPosition;     // transform that gets the mesh into a common world space
	float m_fAngle;           // current direction the character is facing -- in our sample, it's 2D only
	
	double m_dTimePrev;         // global time value before last update
	double m_dTimeCurrent;      // current global time value

	DWORD m_dwAnimIdWalk;
		
	bool m_bWaiting;
	bool m_bWalking;
public:
	virtual HRESULT Setup( AnimationContainer* pMA, double dTimeCurrent, D3DXVECTOR3 *vPosition  );

	void Dispose();
	void OnResetDevice(IDirect3DDevice9* pd3dDevice);
	
	virtual void Animate(double dTimeDelta);
	virtual void Advance(double dTimeDelta);
	void Draw();

	void SetWalkState();

	void SetWalkKey();

	Model(void);
	~Model(void);
};

