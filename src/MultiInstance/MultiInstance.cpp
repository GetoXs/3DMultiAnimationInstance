#include "MultiInstance.h"

using namespace std;

#pragma region GlobalVariables


ID3DXEffect*            g_pEffect = NULL;
CFirstPersonCamera      g_Camera;
ID3DXMesh*              g_pMeshFloor = NULL;

D3DXMATRIXA16           g_mxFloor;
D3DMATERIAL9            g_MatFloor;

double                  g_LastAnimTime = 0.0;
AnimationContainer		g_MultiAnim;            // the MultiAnim class for holding Tiny's mesh and frame hierarchy
vector <Model*>         v_pModels;

int hCount = 35;
int wCount = 20;
float hRate = 0.03f;
float wRate = 0.03f;
#pragma endregion

#pragma region WinMain

INT WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
	DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
	DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
	DXUTSetCallbackD3D9FrameRender( OnFrameRender );
	DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
	DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( KeyboardProc );
	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	
	DXUTSetCursorSettings( true, true );
	DXUTCreateWindow( L"MultiInstance" );
	
	InitApp();
	DXUTInit( true, true ); // Parse the command line and show msgboxes
	DXUTSetHotkeyHandling( true, true, true );  // handle the defaul hotkeys

	
	DXUTCreateDevice( true, 640, 480 );
	DXUTMainLoop();
	
	return DXUTGetExitCode();
}

#pragma endregion

void InitApp()
{
	//kasowanie modeli
	vector <Model*>::iterator itB, itE = v_pModels.end();
	for( itB = v_pModels.begin(); itB != itE; ++ itB )
	{
		( *itB )->Dispose();
		delete *itB;
	}
	v_pModels.clear();
	
	//ustawienie kamery
	g_Camera.SetViewParams(new D3DXVECTOR3(0.5f, 0.55f, -0.2f), new D3DXVECTOR3(0.5f, 0.1f, 0.5f));
}

#pragma region DeviceSetting

bool CALLBACK IsDeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	return true;
}

bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return true;
}

HRESULT CALLBACK OnCreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	HRESULT hr;
	String s;
	D3DXMATRIX mx;

	//dwShaderFlags |= D3DXSHADER_DEBUG;
	LPD3DXBUFFER compileErrors = 0;
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	//³adowanie shadera
	if(FAILED(D3DXCreateEffectFromFile(pd3dDevice, SHADER_FILE, NULL, NULL, flags, NULL, &g_pEffect, &compileErrors)))
	{
		string d = string((char *)compileErrors->GetBufferPointer());
		OutputDebugStringA(d.data());
		return E_FAIL;
	}

	//ustawienia macierzy pod³o¿a
	D3DXMatrixRotationX(&g_mxFloor, -D3DX_PI/2.0f);		//w górê
	D3DXMatrixRotationY(&mx, D3DX_PI / 4.0f );			//w bok
	D3DXMatrixMultiply( &g_mxFloor, &g_mxFloor, &mx );
	D3DXMatrixTranslation( &mx, 0.5f, 0.0f, 0.5f );		//przesuniecie
	D3DXMatrixMultiply( &g_mxFloor, &g_mxFloor, &mx );
	
	g_MatFloor.Diffuse = D3DXCOLOR( 1.f, 1.f, 1.f, 1.f );
	g_MatFloor.Ambient = D3DXCOLOR( 1.f, 1.f, 1.f, 1.f );
	g_MatFloor.Specular = D3DXCOLOR( 0.f, 0.f, 0.f, 1.f );
	g_MatFloor.Emissive = D3DXCOLOR( 0.f, 0.f, 0.f, 0.f );
	g_MatFloor.Power = 0.f;

	return S_OK;
}

HRESULT CALLBACK OnResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	MeshHierarchy AH;
	AH.SetMA(&g_MultiAnim);

	g_MultiAnim.Setup(pd3dDevice, MODEL_FILE, SHADER_FILE, &AH);
	g_MultiAnim.SetTechnique("Skinning20");

	vector <Model*>::iterator itCurCP, itEndCP = v_pModels.end();
	for( itCurCP = v_pModels.begin(); itCurCP != itEndCP; ++ itCurCP )
	{
		( *itCurCP )->OnResetDevice(pd3dDevice);
	}

	if( v_pModels.size() == 0 )
	{
		Model* pTiny;
		for(int h=0; h<hCount; h++)
		{
			for(int w=0; w<wCount; w++)
			{
				pTiny = new Model;
				D3DXVECTOR3* v = new D3DXVECTOR3(0.23f+w*wRate, 0.25f, 0.1f+h*hRate);

				pTiny->Setup( &g_MultiAnim, 0.f, v);
				v_pModels.push_back(pTiny);

				delete v;
			}
		}
	}
	
	if(g_pEffect)
		g_pEffect->OnResetDevice();

	ID3DXEffect* pMAEffect = g_MultiAnim.GetEffect();
	if(pMAEffect)
	{
		pMAEffect->OnResetDevice();
		pMAEffect->Release();
	}

	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams(D3DX_PI/3, fAspectRatio, 0.001f, 1000.f );
	
	g_LastAnimTime = DXUTGetGlobalTimer()->GetTime();;

	return S_OK;
}

void CALLBACK OnLostDevice(void* pUserContext)
{
	if( g_pEffect )
		g_pEffect->OnLostDevice();ID3DXEffect* pMAEffect = g_MultiAnim.GetEffect();
	if( pMAEffect )
	{
		pMAEffect->OnLostDevice();
		pMAEffect->Release();
	}
	vector <Model*>::iterator itCurCP, itEndCP = v_pModels.end();
	for( itCurCP = v_pModels.begin(); itCurCP != itEndCP; ++ itCurCP )
	{
		(*itCurCP)->Dispose();
	}
	MeshHierarchy AH;
	AH.SetMA( &g_MultiAnim );
	g_MultiAnim.Cleanup( &AH );
}

void CALLBACK OnDestroyDevice(void* pUserContext)
{
	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pMeshFloor);
}

#pragma endregion

#pragma region Frame

void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	vector <Model*>::iterator itB, itE = v_pModels.end();
	for(itB = v_pModels.begin(); itB != itE; ++ itB)
	{
		(*itB)->Animate(fTime - g_LastAnimTime);
	}
	g_LastAnimTime = fTime;

	g_Camera.FrameMove(fElapsedTime);
}

void CALLBACK OnFrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
	D3DXMATRIXA16 mx;
	
	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 100, 100, 100), 1.0f, 0);

	pd3dDevice->BeginScene();
	//ustawienia macierzy widoku oraz projekcji
	pd3dDevice->SetTransform(D3DTS_VIEW, g_Camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, g_Camera.GetProjMatrix());
	
	//ustawienie shaderów
	D3DXMatrixMultiply(&mx, g_Camera.GetViewMatrix(), g_Camera.GetProjMatrix());
	//g_pEffect->SetMatrix("viewProjectionMatrix", &mx);
	pd3dDevice->SetTransform( D3DTS_VIEW, g_Camera.GetViewMatrix() );
	pd3dDevice->SetTransform( D3DTS_PROJECTION, g_Camera.GetProjMatrix()) ;

	ID3DXEffect* pMAEffect = g_MultiAnim.GetEffect();
	if( pMAEffect )
		pMAEffect->SetMatrix( "viewProjectionMatrix", &mx );
	SAFE_RELEASE( pMAEffect );

	//ustawienie ustalonej liczby wyst¹pieñ shadera dla pod³ogi
	//pd3dDevice->SetFVF(g_pMeshFloor->GetFVF());
	//g_pEffect->SetMatrix( "worldMatrix", &g_mxFloor) ;
	
	vector <Model*>::iterator itB, itE = v_pModels.end();
	for(itB = v_pModels.begin(); itB != itE; ++ itB)
	{
		(*itB)->Advance(fElapsedTime);

		(*itB)->Draw();
	}
	pd3dDevice->EndScene();
	
}

#pragma endregion

#pragma region Input

LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
	g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );
	return 0;
}

void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}

#pragma endregion