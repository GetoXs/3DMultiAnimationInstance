//struktura wejœcia shadera wierzcho³ków
struct VSInput
{
	float4 modelPos		: POSITION;	
	float3 modelNormal	: NORMAL;	
	float3 blendWeights : BLENDWEIGHT;
	float4 blendIndices : BLENDINDICES;
	float2 tex0         : TEXCOORD0;
};
//struktura wyjœcia shadera wierzcho³ków
struct VSOutput
{
	float4  modelPos    : POSITION;
	float2  tex0        : TEXCOORD0;
};
//tekstura oraz konfiguracja samplera do tekstury
texture texScene;
sampler samScene = sampler_state{
		Texture = <texScene>;
		MinFilter = Linear;
		MagFilter = Linear;
		MipFilter = Point;
};

//macierz widoku-projekcji
float4x4 viewProjectionMatrix	: VIEWPROJECTION;
//macierz swiata
float4x4 worldMatrix	: WORLD;

//vertex shader do rysowania podstawowych modeli 
VSOutput VSMain(float4 modelPos : POSITION, float3 modelNormal : NORMAL, float2 tex0 : TEXCOORD0)
{
	VSOutput o;
	
	o.modelPos = mul(modelPos, worldMatrix);
	o.modelPos = mul(o.modelPos, viewProjectionMatrix);

	o.tex0 = tex0;

	float3 n = normalize(mul(modelNormal, (float3x3)worldMatrix));

	return o;
}
//prosty shader fragmentów próbkuj¹cy teksturê
float4 PSMain(float2 tex0 : TEXCOORD0) : COLOR0
{
	return tex2D(samScene, tex0);
	//return float4(1.0, 0.0, 0, 1);
}

//struktura wejœcia do metody skinowania
struct VSInputSkin
{
	float4      modelPos;
	float3      blendWeights;
	float4      blendIndices;
	float3      modelNormal;
};
//struktura wyjœcia z metody skinowania
struct VSOutputSkin
{
	float4 modelPos;
	float3 modelNormal;
};
//liczba macierzy
const int MATRIX_PALETTE_SIZE = 26;
//tabela macierzy 
float4x3 amPalette[26];
//metoda skinuj¹ca szkielet
VSOutputSkin VS_Skin(const VSInputSkin i, int iNumBones)
{
	VSOutputSkin o = (VSOutputSkin) 0;

	float fLastWeight = 1.0;
	float fWeight;
	float afBlendWeights[3] = (float[3]) i.blendWeights;
	int aiIndices[4] = (int[4]) D3DCOLORtoUBYTE4(i.blendIndices);
	
	for(int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++ iBone)
	{
		fWeight = afBlendWeights[iBone];
		fLastWeight -= fWeight;
		o.modelPos.xyz += mul(i.modelPos, amPalette[aiIndices[iBone]]) * fWeight;
		o.modelNormal     += mul(i.modelNormal, amPalette[aiIndices[iBone]]) * fWeight;
	}
	
	o.modelPos.xyz += mul(i.modelPos, amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;
	o.modelNormal     += mul(i.modelNormal, amPalette[aiIndices[iNumBones - 1]]) * fLastWeight;

	return o;
}

//VS skinujacy kosci
VSOutput VertSkinning(VSInput i, uniform int iNumBones)
{
	VSOutput   o;
	
	VSInputSkin vsi = { i.modelPos, i.blendWeights, i.blendIndices, i.modelNormal };
	VSOutputSkin vso = VS_Skin(vsi, iNumBones);

	o.modelPos = mul(float4(vso.modelPos.xyz, 1.0f), viewProjectionMatrix);
	
	o.tex0  = i.tex0.xy;

	return o;
}

//liczba koœci
int CurNumBones = 2;

VertexShader vsArray20[1] = { compile vs_2_0 VertSkinning(1) };

//renderowanie proste
technique RenderScene
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMain();
	}
}
//renderowanie z koœæmi
technique Skinning20
{
	pass p0
	{
		VertexShader = (vsArray20[CurNumBones]);
		PixelShader = compile ps_2_0 PSMain();
	}
}