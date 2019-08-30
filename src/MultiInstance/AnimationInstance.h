#pragma once

#include "DXUT.h"

#include "MeshHierarchy.h"

#include "Common.h"
#include "Config.h"

class AnimationContainer;

class AnimationInstance
{
protected:

    AnimationContainer* m_pMultiAnim;
    D3DXMATRIX m_mxWorld;

private:

    virtual void    UpdateFrames( D3DXFRAME* pFrame, D3DXMATRIX* pmxBase );
    virtual void    DrawFrames( D3DXFRAME* pFrame );
    virtual void    DrawMeshFrame( D3DXFRAME* pFrame );

public:
    LPD3DXANIMATIONCONTROLLER m_pAC;
    virtual HRESULT Setup( LPD3DXANIMATIONCONTROLLER pAC );

                    AnimationInstance( AnimationContainer* pMultiAnim );
    virtual         ~AnimationInstance();

    virtual void    Cleanup();

    AnimationContainer* GetMultiAnim();
    void            GetAnimController( LPD3DXANIMATIONCONTROLLER* ppAC );

    D3DXMATRIX      GetWorldTransform();
    void            SetWorldTransform( const D3DXMATRIX* pmxWorld );

    virtual HRESULT AdvanceTime( DOUBLE dTimeDelta, ID3DXAnimationCallbackHandler* pCH );
    virtual HRESULT ResetTime();
    virtual HRESULT Draw();
};

