#include "DirectxHelper.h"


DWORD DirectxHelper::GetAnimationIndex(char sString[], LPD3DXANIMATIONCONTROLLER pAC)
{
	HRESULT hr;
	LPD3DXANIMATIONSET pAS;
	DWORD dwRet = 0;

	for( DWORD i = 0; i < pAC->GetNumAnimationSets(); ++ i )
	{
		hr = pAC->GetAnimationSet( i, &pAS );
		if( FAILED( hr ) )
			continue;

		if( pAS->GetName() &&
			!strncmp( pAS->GetName(), sString, min( strlen( pAS->GetName() ), strlen( sString ) ) ) )
		{
			dwRet = i;
			pAS->Release();
			break;
		}

		pAS->Release();
	}

	return dwRet;
}
