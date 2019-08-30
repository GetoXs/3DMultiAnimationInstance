#include "Model.h"


Model::Model(void) : m_pMA( NULL ), m_dwMultiAnimIdx( 0 ), m_pAI( NULL ), m_fAngle(0.f)
{
}


HRESULT Model::Setup( AnimationContainer* pMA,
					  double dTimeCurrent, D3DXVECTOR3 *vPosition )
{
	m_pMA = pMA;
	m_dTimeCurrent = m_dTimePrev = dTimeCurrent;

	HRESULT hr;
	hr = m_pMA->CreateNewInstance( &m_dwMultiAnimIdx );
	if( FAILED( hr ) )
		return E_OUTOFMEMORY;

	

	// compute reorientation matrix based on default orientation and bounding radius
	D3DXMATRIX mx;
	float fScale = 1.f / m_pMA->GetBoundingRadius() / 30.f;
	D3DXMatrixScaling( &mx, fScale, fScale, fScale );
	m_mxOrientation = mx;
	D3DXMatrixRotationX( &mx, -D3DX_PI / 2.0f );
	D3DXMatrixMultiply( &m_mxOrientation, &m_mxOrientation, &mx );
	D3DXMatrixRotationY( &mx, D3DX_PI / 2.0f );
	D3DXMatrixMultiply( &m_mxOrientation, &m_mxOrientation, &mx );
	
	D3DXMatrixTranslation(&m_mxPosition, vPosition->x, vPosition->y, vPosition->z);


	m_pAI = m_pMA->GetInstance( m_dwMultiAnimIdx );

	LPD3DXANIMATIONCONTROLLER pAC;
	m_pAI->GetAnimController( &pAC );

	//pobranie id animacji chodzenia
	m_dwAnimIdWalk = DirectxHelper::GetAnimationIndex("Walk", pAC);
	if(m_dwAnimIdWalk == 0 )
		return E_FAIL;
	pAC->Release();
		
	SetWalkState();
	
	m_pAI->GetAnimController( &pAC );
	//ustawianie pierwszego postepu
	pAC->AdvanceTime( dTimeCurrent, NULL );
	pAC->Release();
	
	return S_OK;
}

void Model::Animate( double dTimeDelta )
{
	D3DXMATRIX mxWorld, mx;

	
	// compute world matrix based on pos/face
	D3DXMatrixRotationY( &mxWorld, -m_fAngle);
	//D3DXMatrixTranslation(&m_mxPosition, 0.5f, 0.f, 0.5f);
	D3DXMatrixMultiply( &mxWorld, &mxWorld, &m_mxPosition );
	D3DXMatrixMultiply( &mxWorld, &m_mxOrientation, &mxWorld );
	//D3DXMatrixIdentity(&mx);
	m_pAI->SetWorldTransform(&mxWorld);
}

void Model::Advance(double dTimeDelta)
{
	m_dTimePrev = m_dTimeCurrent;
	m_dTimeCurrent += dTimeDelta;

	m_pAI->AdvanceTime(dTimeDelta, NULL);
}


void Model::Draw()
{
	m_pAI->Draw();

}
Model::~Model(void)
{
}

void Model::Dispose()
{
	// Save the current track's animation set name
	// so we can reset it again in RestoreDeviceObjects later.
	LPD3DXANIMATIONCONTROLLER pAC = NULL;
	m_pAI->GetAnimController( &pAC );
	if( pAC )
	{
		SAFE_RELEASE( pAC );
	}

}

void Model::OnResetDevice(IDirect3DDevice9* pd3dDevice)
{

}

void Model::SetWalkState()
{
	m_bWaiting = false;
	m_bWalking = true;

	/*
	if( rand() % 5 )
		m_fSpeed = m_fSpeedWalk;
	else
		m_fSpeed = m_fSpeedJog;
	*/

	SetWalkKey();
}

void Model::SetWalkKey()
{
	DWORD dwNewTrack = 0;
	LPD3DXANIMATIONCONTROLLER pAC;
	LPD3DXANIMATIONSET pAS;
	m_pAI->GetAnimController( &pAC );

	pAC->GetAnimationSet( m_dwAnimIdWalk, &pAS );

	pAC->SetTrackAnimationSet( dwNewTrack, pAS );
	pAS->Release();

	//pAC->UnkeyAllTrackEvents( m_dwCurrentTrack );
	pAC->UnkeyAllTrackEvents( dwNewTrack );

	/*pAC->KeyTrackEnable( m_dwCurrentTrack, FALSE, m_dTimeCurrent + MOVE_TRANSITION_TIME );
	pAC->KeyTrackSpeed( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR );
	pAC->KeyTrackWeight( m_dwCurrentTrack, 0.0f, m_dTimeCurrent, MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR );*/

	pAC->SetTrackEnable( dwNewTrack, TRUE );
	pAC->KeyTrackSpeed( dwNewTrack, 1.0f, m_dTimeCurrent, MOVE_TIME, D3DXTRANSITION_LINEAR );
	pAC->KeyTrackWeight( dwNewTrack, 1.0f, m_dTimeCurrent, MOVE_TIME, D3DXTRANSITION_LINEAR );

	//m_dwCurrentTrack = dwNewTrack;

	pAC->Release();
}