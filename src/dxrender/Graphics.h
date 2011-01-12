//////////////////////////////////////////////////////////////////////////////////////////
//
// name: graphics.h
// func: һ��ʹ��D3D��3Dͼ������
//		 SDK �汾Ϊ Microsoft DirectX SDK (June 2008)
// date: 2011.01.01	
// lib : d3d9.lib��d3dx9.lib��winmm.lib��D3dxof.lib��DXGUID.LIB
// �ο���C++ -Game  Programming RPGs With DirectX����Ҫ��
//       Introduction to 3D Game Programming with DirectX.9
//       Skinned Mesh Character Animation with Direct3D 9.0c
//       
///////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------������־-----------------------------------------------/
//
// 2011.01.10 ��ɶ����ļ���
//
// 2011.01.11 ����������ΪUnicode�汾������޷���CFont�޷���ӡ���ĵ�����
//
// 2011.01.12 ��д������ͽṹ�����Ҫ˵������
//
//--------------------------------------------------------------------------------------------


#ifndef GRAPHICS_H_101225
#define GRAPHICS_H_101225

#include <d3dx9.h>
#include <tchar.h>


#define FILE_PATH_LENGTH 100


extern unsigned char D3DRM_XTEMPLATES[];
#define D3DRM_XTEMPLATE_BYTES 3278

#define FILE_PATH_LENGTH 100

// �ͷ�COM����
#define ReleaseCOM(x) if(x) { x->Release(); x = NULL; }

class CGraphics;
class CPosition;
class CTexture;
class CFont;
class CLight;
class CCamera;
class CAnimation;
class CObject;
struct Mesh;
struct Frame;
struct Animation;
struct AnimationSet;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������CGraphics 
//
// ���ܣ�ͼ�ο�Ļ����࣬ʵ�ָ���DX��ȫ�ֿ���
//
// ��Ҫ����˵����
//
//	       һ����ʼ������ʹ��ǰ������Ӧ����Init���г�ʼ����hWnd�ǹ�����windows���ھ��,Ĭ������£�
//     ����һ��������ʽ����������ڳߴ���ͬ��32ɫ��Ĵ��ڡ�����ͨ������Ĭ�ϲ���������Ҫ�Ĵ������͡�
//
//	       �����������ڻ���ÿһ֡ǰ��Ӧ��������棬�����а��������������ķ���Clear, ClearZBuffer��
//     ClearDisplay, ClearDiaplayֵ�����ǰ���棬һ������2D��ClearZBuffer�������Z���棬Clearʹ��
//     ʱ���ж��Ƿ�ʹ����Z���棬���û��ʹ�ã�ֻ����ClearDisplay����������������֡�
//
//         ������ͼ���ڻ�ͼǰ���������BeginScene���������棬�ڻ��ƽ��������EndScene�����������ͨ��
//     Display���������廭����Ƶ���Ļ�ϡ�
//
//         �ģ�λ���趨:SetPerspective���������ӿڵ���ز�����SetWorldPos�����趨������������꣬
//     �˷����������ֲ�����һ����D3DX��������ã���һ����CPostion�����á�SetCamera�����趨�����
//     �꣬��������ӵ��λ�á�
//
//         �壬ȫ���趨: SetAmbientLight �趨�����⣻SetShadeMode�趨��ɫ��ʽ��SetFillMode�����ʽ��
//     �����Ķ���������ϣ�SetLignt����ĳ����Դ�ӽ�ͼ�����棬���趨һ��������
//
//         �������ܵĿ������رգ�EnableLight��ʹĳ�������Ĺ�Դ��Ч��ʧЧ��EnableLighting�Ƿ������գ�
//     EnableZBuffer Z���棻EnableAlphaBlending Alpha���
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CGraphics
{
public:
	CGraphics();
	~CGraphics();
	
	IDirect3DDevice9 *GetDevice() const {return m_pDevice;} 
	ID3DXSprite      *GetSprite() const {return m_pSprite;}
	DWORD             GetWidth()  const {return m_width;}
	DWORD             GetHeight() const {return m_height;}
	DWORD             GetBPP()    const {return m_bpp;}
	HWND              GetWnd()    const {return m_hWnd;}


	BOOL Init(HWND hWnd, BOOL bWindow = TRUE, DWORD width = 0, DWORD height = 0, BYTE bpp = 32);
	BOOL ShutDown();

	BOOL Clear(DWORD color = 0, FLOAT zBuffer = 1.0f);
	BOOL ClearZBuffer(float zBuffer = 1.0f);
	BOOL ClearDisplay(DWORD color = 0);

    BOOL BeginScene();
    BOOL EndScene();
	BOOL Display();

	// SetTransform
	BOOL SetPerspective(FLOAT fov, FLOAT aspect, FLOAT nz, FLOAT fz);
	BOOL SetWorldPos(const D3DXMATRIX &worldPos);
	BOOL SetWorldPos(CPosition &pos);
	BOOL SetCamera(CCamera &camera);

	// SetRenderState
	BOOL SetAmbientLight(BYTE red, BYTE green, BYTE blue);
	BOOL SetShadeMode(DWORD shadeMode = D3DSHADE_FLAT);
	BOOL SetFillMode(DWORD fillMode = D3DFILL_WIREFRAME);
	BOOL SetLignt(DWORD num, const CLight &light);
	
	BOOL EnableLight(DWORD num, BOOL bEnable = TRUE);
	BOOL EnableLighting(BOOL bEnable = TRUE);
	BOOL EnableZBuffer(BOOL bEnable = TRUE);
	BOOL EnableAlphaBlending(BOOL Enable = TRUE, DWORD srcValue = D3DBLEND_SRCALPHA, DWORD destValue = D3DBLEND_INVSRCALPHA);

protected:

	BYTE _GetFormatBPP(const D3DFORMAT &format);
	BOOL _CheckFormat(const D3DFORMAT &format, BOOL bWindow, BOOL bHAL);

protected:

	HWND              m_hWnd;
	IDirect3D9       *m_pD3D;
	IDirect3DDevice9 *m_pDevice;
	D3DDISPLAYMODE    m_d3ddm;
	ID3DXSprite      *m_pSprite;

	BOOL              m_bWindow;
	BOOL              m_bHAL;
	BOOL              m_bZBuffer;

	DWORD             m_width;
	DWORD             m_height;
	BYTE              m_bpp;

	BYTE              m_ambientRed;
	BYTE              m_ambientGreen;
	BYTE              m_ambientBlue;	
};

//////////////////////////////////////////////////////////////////////////////////////
//
// ���ֶ�������
//
//////////////////////////////////////////////////////////////////////////////////////

// �߿��Ͷ���
// ʹ�ô˶�������ʱ����Ҫ������CGraphics��SetFillMode����, 
// ��FillMode����ΪD3DFILL_WIREFRAME

struct WireVertex
{
	WireVertex(){}

	WireVertex(float x, float y, float z)
	{
		_x = x;	 _y = y;  _z = z;
	}

	float _x, _y, _z;

	static const DWORD FVF;
};

// ����һ�ִ��ж�����ɫ�ĵĶ�������

struct ColorVertex
{
	ColorVertex(){}

	ColorVertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x;	 _y = y;  _z = z;  _color = c;
	}

	float _x, _y, _z;
	D3DCOLOR _color;

	static const DWORD FVF;
};

// ��ͨ3D���㣬���в�������

struct NormalVertex
{
	NormalVertex(){}

	NormalVertex(float x, float y, float z, float nx, float ny, float nz)
	{
		_x  = x;  _y  = y;	_z  = z;
		_nx = nx; _ny = ny; _nz = nz;
	}
	float  _x,  _y,  _z;
	float _nx, _ny, _nz;
	static const DWORD FVF;
};

// ����ȫ���3D���㣬ӵ�в�������Ͷ�����ɫ

struct Vertex
{
	Vertex(){}

	Vertex(float x, float y, float z, float nx, float ny, float nz, D3DCOLOR color)
	{
		_x  = x;  _y  = y;	_z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_color = color;
	}
	float  _x,  _y,  _z;
	float _nx, _ny, _nz;
	D3DCOLOR _color;

	static const DWORD FVF;
};

// ���ڻ���2D����Ķ�������

struct TexVertex2D
{	
	TexVertex2D(){}

	TexVertex2D(float x, float y, float z, float rhw, float u, float v)
	{
		_x  = x;  _y  = y;	_z  = z;
		_rhw = rhw;
		_u = u; _v = v;
	}
	float _x, _y, _z;     // 2-D ���� z��1
	float _rhw;           // rhw
	float _u, _v;         // ��������

	static const DWORD FVF;
};

// ���ڻ���3D����Ķ�������

struct TexVertex3D
{	
	TexVertex3D(){}

	TexVertex3D(float x, float y, float z, float u, float v)
	{
		_x  = x;  _y  = y;	_z  = z;
		_u = u; _v = v;
	}
	float _x, _y, _z;     // 3D ����
	float _u, _v;         // ��������

	static const DWORD FVF;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������CVertex
//
// ���ܣ����������飬����һ��������ƣ����а������㻺���붥�����������û�д���������������ֱ�ӻ���
//       ���㻺�棬���򣬰����������л��ƣ�����಻̫���ƣ�ֻ��ʵ��һЩ�򵥵Ķ������
//
// ��Ҫ����˵����
//
//	       һ����ʼ����CreateBuffer���ڴ���һ�����㻺�棬��Ҫ����������(vertexNum)������ߴ�
//     (vertexSize)�˳ߴ�Ϊÿ��������ڴ��С���Լ�������(FVF)
//
//	       �����������ڽ������ǰ�������Ƚ�������LockBuffer��LockIndex���˷���һ��Ϊ�ڲ�ʹ�ã���ʹ��
//     �ײ�DX���в���ʱ�����Ե��ã�����Ϊ��������ʼ������������������(����ʼ���㿪ʼ)�����������
//     ֮����Ҫ������Ӧ�Ľ�����������
//
//         ������䣺SetBuffer,SetIndex�ֱ�������䶥�㻺���붥���������飬���������Ҫ��Create
//     ʱ������������ͬ
//
//         �ģ���Ⱦ��Render��startIndexΪ��ʼ����������primitiveCountΪÿ�����������������type
//     Ϊƽ����Ʋ���Ĭ��Ϊ������Ⱦ
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CVertex
{
public:
	CVertex();
	~CVertex();

	void *GetBufferPtr()  const {return m_pBuffer;}
	DWORD GetVertexNum()  const {return m_vertexNum;}
	DWORD GetVertexSize() const {return m_vertexSize;}
	DWORD GetFVF()        const {return m_FVF;}

	BOOL CreateBuffer(CGraphics *pGraphics, DWORD vertexNum, DWORD faceNum, DWORD vectexSize, DWORD FVF);
	BOOL CreateIndex(CGraphics *pGraphics, DWORD indexNum, DWORD indexSize, D3DFORMAT format);
	BOOL Destroy();

	void *LockBuffer(DWORD startVertex = 0, DWORD vertexNum = 0);
	void *LockIndex(DWORD startIndex = 0, DWORD indexNum = 0);
	BOOL UnlockBuffer();
	BOOL UnlockIndex();

	BOOL SetBuffer(DWORD startVertex, DWORD vertexNum, void *vertexList);
	BOOL SetIndex(DWORD startIndex, DWORD indexNum, void *indexList);

	BOOL Render(DWORD startVertex, UINT primitiveCount = 0, D3DPRIMITIVETYPE type = D3DPT_TRIANGLELIST, CTexture *pTexture = NULL);

protected:

	CGraphics              *m_pGraphics;
	IDirect3DVertexBuffer9 *m_pVB;
	IDirect3DIndexBuffer9  *m_pIB;
	void                   *m_pBuffer;
	void                   *m_pIndex;

	DWORD                   m_vertexNum;
	DWORD                   m_indexNum;
	DWORD                   m_vertexSize;
	DWORD                   m_indexSize;
	DWORD                   m_FVF;
	DWORD                   m_faceNum;

	
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������CTexture
//
// ���ܣ�����������м򵥵��������
//
// ��Ҫ����˵����
//
//	       һ�����أ�LoadFromFile�����ļ��м���һ������colorKey����ָ���ؼ�ɫ��format����ָ��
//     �����ʽ��Ĭ��Ϊ�ļ�Ĭ�ϸ�ʽ
//
//	       �������ƣ�BLT���˺�������ID3DXSprite���м򵥵Ļ��ƣ��䷽ʽ������DDRAW��ʽ,������һ����
//     ��������ϻ��Ƶķ�ʽ����������ʱû���ṩ
//
//         �����任��Rotate��Scale��Translation ������ת�����ţ�ƽ��
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CTexture
{
public:

	CTexture();
	~CTexture();

	IDirect3DTexture9 *GetTexture() const {return m_pTexture;}
	DWORD              GetWidth()   const {m_d3dsd.Width;}
	DWORD              GetHeight()  const {m_d3dsd.Height;}

	BOOL LoadFromFile(CGraphics *pGraphics, LPCTSTR fileName, DWORD colorKey = 0, D3DFORMAT format = D3DFMT_UNKNOWN);
	BOOL Destroy();

	BOOL Blt(long destX, long destY, long srcX = 0, long srcY = 0, long width = 0, long height = 0, 
		FLOAT xScale = 1.0f, FLOAT yScale = 1.0f, D3DCOLOR color = 0xFFFFFFFF);

	BOOL Rotate(float centerX, float centerY, FLOAT angle);
	BOOL Scale(float centerX, float centerY, FLOAT scaleX, FLOAT scaleY);
	BOOL Translation(float x, float y);


protected:

	CGraphics         *m_pGraphics;
	IDirect3DTexture9 *m_pTexture;
	D3DSURFACE_DESC    m_d3dsd;

	BOOL  m_bLoaded;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������CFont
//
// ���ܣ�ID3DXFont���壬Ч�ʱȽϵ��£��ڷ�UNICODE�����У����ܻ�������
//
// ��Ҫ����˵����
//
//	       һ����ʼ����Create,����ָ������߶ȣ���������(�������ز�ͬ���͵�����)�Լ��Ƿ�б��(bItalic)
//
//	       �������ƣ�����ָ�����ֵ���ʼ���꣬���ο�ߴ磬Ĭ��Ϊ����룬�϶��룬��ɫΪ��ɫ
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CFont
{
public:

	CFont();
	~CFont();

	BOOL Create(CGraphics *pGraphics, INT height = 24, LPCTSTR  fontName = NULL, BOOL bItalic = FALSE);
	void Destroy();

	BOOL Print(LPCTSTR text, long xPos, long yPos, long width, long height, DWORD format = DT_LEFT | DT_TOP, D3DCOLOR color = 0xffffffff);

private:

	ID3DXFont *m_pFont;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������CLight
//
// ���ܣ�����DX�еĸ��ֹ��գ����������⣬���淴��⣬������⣬���Դ�������Դ�;۹��
//
// ��Ҫ����˵����
//
//		   ��������  ��SetType D3DLIGHT_POINT(���Դ), D3DLIGHT_SPOT(�۹��),D3DLIGHT_DIRECTIONAL(�����),
//	       ������    ��SetAmbientColor ���淢��⣺SetSpecularColor �������  ��SetDiffuseColor
//         ���շ�Χ  ��Range ��Ӱ�췽���          �۹�Ʋ�����SetFalloff ��������Ϊ1.0
//		   �۹����ǣ�SetPhi  �۹���ڽǣ�SetTheta 
//		   ˥������  ��etAttenuation0,1,2��
//         ���ù�Դλ�ã�MoveTo�ƶ���Ŀ���(����λ��) Move �ƶ����������λ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CLight
{
public:
	CLight();
	~CLight();

	const D3DLIGHT9 *GetLight() const {return &m_light;}

	void SetType(D3DLIGHTTYPE type) {m_light.Type  = type;}
	void SetRange(float range)      {m_light.Range = range;}
	void SetFalloff(float falloff)  {m_light.Falloff = falloff;}
	void SetTheta(float theta)      {m_light.Theta = theta;}
	void SetPhi(float phi)          {m_light.Phi = phi;}
	void SetAttenuation0(float attenuation) {m_light.Attenuation0 = attenuation;}
	void SetAttenuation1(float attenuation) {m_light.Attenuation1 = attenuation;}
	void SetAttenuation2(float attenuation) {m_light.Attenuation2 = attenuation;}

	void SetAmbientColor(BYTE red, BYTE green, BYTE blue);
	void SetDiffuseColor(BYTE red, BYTE green, BYTE blue);
	void SetSpecularColor(BYTE red, BYTE green, BYTE blue);

	void MoveTo(float xPos, float yPos, float zPos);
	void Move(float xPos, float yPos, float zPos);

protected:

	D3DLIGHT9 m_light;
};

class CPosition
{
public:

	CPosition();
	~CPosition();

	const D3DXMATRIX &GetMatrix()  {Update(); return m_matWorld;}

	void MoveTo(float xPos, float yPos, float zPos);
	void Move(float x, float y, float z);
	void RotateTo(float xRot, float yRot, float zRot);
	void Rotate(float x, float y, float z);
	void ScaleTo(float xScale, float yScale, float zScale);
	void Scale(float x, float y, float z);

	void Update();
	
protected:

	FLOAT m_xPos,   m_yPos,   m_zPos;
	FLOAT m_xScale, m_yScale, m_zScale;
	FLOAT m_xRot,   m_yRot,   m_zRot;

	D3DXMATRIX m_matWorld;
    D3DXMATRIX m_matScale;
    D3DXMATRIX m_matRotation;
    D3DXMATRIX m_matTranslation;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������CCamera
//
// ���ܣ����������ͨ����������۲�λ��
//
// ��Ҫ����˵����
//
//		   һ���������λ�ú�ָ�� SetLookAt��ָ���۲���λ�ú͹۲�Ŀ��λ��
//
//		   ���������λ�ƣ�����To���Ǿ���λ�ƣ�����To��Ϊ���λ��(����һ��)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CCamera
{
public:

	CCamera();
	~CCamera();

	float GetXRotation() const {return m_xRot;}
	float GetYRotation() const {return m_yRot;}
	float GetZRotation() const {return m_zRot;}
	float GetXPosition() const {return m_xPos;}
	float GetYPosition() const {return m_yPos;}
	float GetZPosition() const {return m_zPos;}

	const D3DXMATRIX &GetMatrix() {Update();return m_matView;}

	void SetLookAt(float xEye, float yEye, float zEye, float xAt, float yAt, float zAt);

	void MoveTo(float xPos, float yPos, float zPos);
	void Move(float x, float y, float z);
	void RotateTo(float xRot, float yRot, float zRot);
	void Rotate(float x, float y, float z);

	void Update();

protected:

	float m_xPos,   m_yPos,   m_zPos;
	float m_xRot,   m_yRot,   m_zRot;

	D3DXMATRIX m_matView;
    D3DXMATRIX m_matTranslation;
    D3DXMATRIX m_matRotation;


};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ṹ�壺Mesh
//
// ���ܣ�  ����DX������Ϣ
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ������Ĳ�����Ϊ�й�����Ϣ���޹�����Ϣ����
//
//		   ���������޹�����Ϣ�ļ��� _pSkinInfo  �� _pDXSkinMesh ������Ϊ NULL ��ʱ����������Ӧ����ԭʼ
//     ���� _pDXMesh������ʱ�Ĺ�������Ϊԭʼ����(_matOriginal)��������ʹ�ñ任����(_matTransformed)
//
//         ��������ṹ����һ���������ʽ(_pNext)����������������������һ������񣬽���ͬ���ĸ��ºͻ���
//
//		   �ģ����������������ָ��(_pMaterials)������ָ������(_apTextures)��
//
//         �壬��������߽���뾶(_radius)�ͱ߽��Χ(_min,_max)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct Mesh
{
	Mesh()
	{
		_pName           = NULL; 

		_pDXMesh         = NULL;
		_pDXSkinMesh     = NULL;
		_pSkinInfo       = NULL;
		_pBoneMatrices   = NULL;
		_apFrameMatrices = NULL;

		_materialNum     = 0;
		_pMaterials      = NULL;

		_apTextures      = NULL;

		_pNext           = NULL;


	}
	~Mesh()
	{
		if(NULL != _pName)
		{
			delete [] _pName;
		}

		if(NULL != _pMaterials)
		{
			delete [] _pMaterials;
		}

		if(NULL != _apFrameMatrices)
		{
			delete [] _apFrameMatrices;
		}

		if(NULL != _pBoneMatrices)
		{
			delete [] _pBoneMatrices;
		}

		ReleaseCOM(_pDXMesh);
		ReleaseCOM(_pDXSkinMesh);
		ReleaseCOM(_pSkinInfo);

		if(NULL != _apTextures) 
		{
			for(DWORD i=0; i<_materialNum; i++) 
			{
				ReleaseCOM(_apTextures[i]);
			}
			delete [] _apTextures;
		}

		delete _pNext;  // Delete next mesh in list
	}

	char               *_pName;

	ID3DXMesh          *_pDXMesh;			// ԭʼ����
	ID3DXMesh          *_pDXSkinMesh;		// ���ڻ��Ƶ�����
	ID3DXSkinInfo      *_pSkinInfo;			// ������Ϣ
	D3DXMATRIX         *_pBoneMatrices;		// �����任����������յľ�������
	D3DXMATRIX        **_apFrameMatrices;	// ֡�������飬����һ���м������ã�����CAnimation��CMesh��������ʱ�������ڹ�������

	D3DMATERIAL9       *_pMaterials;	// ��������
	DWORD               _materialNum;	// ��������

	IDirect3DTexture9 **_apTextures;	// ����ָ������

	Mesh               *_pNext;			// ����һ��mesh�������б�

	D3DXVECTOR3         _min;			// �߽��
	D3DXVECTOR3         _max;
	float               _radius;		// �߽���뾶
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ṹ�壺Frame
//
// ���ܣ�  ����DX֡��Ϣ
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ��֡�Ĺ�����ʽΪ���������������Ϊ��ÿ��֡��Ӧһ���������һ��ģ���У����еĹ����и����ӣ�
//     �ֵܵĹ�ϵ�����ֱ�Ϊ�����ֱ۷�Ϊ�ϱۣ�ǰ�ۺ��֣������˶�ʱ������Ӱ�쵽ǰ�ۺ��ϱ۵�λ�ã������
//     �䵱�ӵĽ�ɫ���෴������ϱ�̧�𣬱�Ȼ������ǰ�ۺ��ֵ�λ�õı仯�����ֱ仯��ͨ���Ŵ���ʵ�ֵġ�
//
//		   ����Frame�ṹ�еľ��󣬹���ֻ��һ������Ķ�����������ֱ�۵ķ�Ӧ�������ϣ���˹�������������
//	   ��ϣ����ѹ�ͷ�������ÿһ������һһ��Ӧ��������Ӧ�ķ������Ǿ���_matOriginal��Ӧ��������ĳ�ʼ
//     λ����Ϣ��_matTransformed��Ӧ�����ı仯��_matCombined��Ӧ����������λ����Ϣ������������ʱ��������
//     ����������Ϣ����ʹ��_matTransformed�����¾���������������ʹ��_matOriginal�����¡�_matCombined
//     ��mesh�е����ձ任�������Ӧ��
//
//         ����ÿһ��frame�����Թ���һ��meshҲ���Բ�����
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct Frame
{
	Frame()
	{
		_pName     = NULL;
		_pMesh     = NULL;
		_pParent   = NULL;
		_pSibling  = NULL;
		_pChild    = NULL;

		D3DXMatrixIdentity(&_matCombined);
		D3DXMatrixIdentity(&_matTransformed);
		D3DXMatrixIdentity(&_matOriginal);
	}
	~Frame()
	{
		
		if(NULL != _pName)
		{
			delete _pName;
		}
		
		if(NULL != _pMesh)
		{
			delete   _pMesh;
		}	
		
		if(NULL != _pChild)
		{
			delete   _pChild;
		}
		
		if(NULL != _pSibling)
		{
			delete   _pSibling;
		}

		// parent�����ͷţ��������ں�������
	}

	char       *_pName;

	// �����ʱֻ��parentӵ��

	Mesh       *_pMesh;

	// ��Щ������FRAME�ľ�Ҫ

	D3DXMATRIX  _matCombined;    // �ϳɾ���ÿһ���������յı任����
    D3DXMATRIX  _matTransformed; // ��ǰ�任����
    D3DXMATRIX  _matOriginal;    // ԭʼ����

	// ������
	Frame      *_pParent;		
	Frame      *_pSibling;
	Frame      *_pChild;
}; 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ࣺ  CMesh
//
// ���ܣ���mesh��frame������һ���Ѿ��������ڴ�DX�ļ��м���������֡��Ϣ�������Խ��л���
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ���ⲿ�ӿڣ��ⲿ�ӿ�ֻ�ṩ���أ����ٺͻ������ַ�����LoadXFileʹ��ʱ��Ҫ����һ����Чͼ��
//     �豸��ָ��pGraphics��ANSI���ļ������Լ�ANSI��������λ�õĸ�Ŀ¼��Ĭ��Ϊ��ǰĿ¼��
//
//		   �����������Ⱦ��Render����һ������CPositionΪ�������꣬���ʹ�ã�����ƶ���ָ��λ��
//     �ڶ�������bTransformed��ʶ�Ƿ�ʹ�ñ任������и��£���Frame˵����������������ص�����
//     ������Ƥ��������Ӧ������ΪFALSE������Ӧ��ʹ��TRUE��
//
//         ������Ⱦ����˵������Ⱦ�ڲ���������������Ա����_UpdateFrames��_UpdateMeshes��_DrawMeshes
//     _UpdateFrames����Ϊ�������Ŵ����̣����кϳɾ���ĸ��£�_UpdateMeshes�ù�����Ϣ�������񣬸���
//     ÿһ��������λ�ã������������λ��_DrawMeshes�������յ���Ⱦ�����û��ʹ����Ƥ��������ʹ��Mesh
//     �е�ԭʼ������Ⱦ������ʹ����Ƥ������Ⱦ
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CMesh
{
public:
	CMesh();
	~CMesh();
	
	Frame *GetParentFrame() const {return m_pFrames;}
	Mesh  *GetHeadMesh()    const {return m_pMeshes;}

	Frame *GetFrame(LPSTR frameName);

	BOOL LoadXFile(CGraphics *pGraphics, const char *pFileName, const char *pTexturePath = ".\\");
	void Destroy();

	BOOL Render(CPosition *pos = NULL, BOOL bTransformed = TRUE);

protected:

	Frame *_FindFrame(Frame *pFrame, const char *pFrameName);

	BOOL _UpdateMeshes();
	void _UpdateFrames(Frame *pFrame, const D3DXMATRIX &mat, BOOL bTransformed = FALSE);

	void _AddMeshToFrame(Frame *pFrame, Mesh *pMesh);
	void _MapFramesToBones(Frame *pFrame);

	void _ResetFrame(Frame *pFrame);

	BOOL _ParseXFile(const char *fileName, const char *pTexturePath);
	void _ParseXFileData(ID3DXFileData *pDataObj, Frame *pFrame, const char *texturePath);

	BOOL _DrawMeshes(CPosition *pos = NULL);

protected:

	CGraphics   *m_pGraphics;

	Mesh        *m_pMeshes;		// ����ָ��,���ָ������frame�е�ȫ��mash����˲������ͷ���Դ
	Frame       *m_pFrames;		// ���ֹ�����֡

	DWORD        m_frameNum;
	DWORD        m_meshNum;

	D3DXVECTOR3  m_min;
	D3DXVECTOR3  m_max;
	float        m_radius;
};	

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ṹ�壺XFileRotateKey��XFileScaleKey��XFilePositionKey��XFileMatrixKey
//         RotateKey��PositionKey��ScaleKey��MatrixKey
//
// ���ܣ�ǰ�ĸ�Ϊ����DX�ļ�ʱ����ȡ�ڴ�ʱ���ж���ʱʹ�ã����ĸ�Ϊ��ǰ�ĸ�ת����DX�������ĸ�ʽ��
//       �ڳ����ڲ�ʹ��
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ��������⣬һ��������(AnimationSet)�����һ���ض��Ķ�����һ������(Animation)����һ������
//     һ�������λ����һ���������������������ɼ����õ���
//
//         �������.X�ļ����ṩ��MatrixKey�������ֱ������ʹ�ã����������RotateKey��PositionKey��
//     ScaleKey������ϳ�DX�����ʹ��
//
//         ��������_time�������Ա������Ǹö�����һ�鶯���е�λ�ã�Ҳ�Ƕ����йؼ�֡��λ�������������
//     ������������_time��ֵΪ һ��������ֵ������1 2 3 4 5����ʱ������в�ֵ������������Խ��в�ֵ��
//     ��_time��ֵΪ 1 3 5 ��������������ò�ֵԭ����в�ֵ���������м��֡��ʵ�ֶ�����ƽ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct XFileRotateKey
{
	DWORD _time;
	DWORD _floats;
	float _w;
	float _x;
	float _y;
	float _z;
};

struct XFileScaleKey
{
	DWORD	    _time;
	DWORD       _floats;	
	D3DXVECTOR3	_scale;	
};

struct XFilePositionKey
{
	DWORD       _time;
	DWORD       _floats;	
	D3DXVECTOR3	_pos;	
};

struct XFileMatrixKey
{
	DWORD	    _time;
	DWORD       _floats;	
	D3DXMATRIX	_matrix;
};

struct RotateKey
{
	DWORD          _time;
	D3DXQUATERNION _quaternion;
};

struct PositionKey
{
	DWORD     	_time;
	D3DXVECTOR3	_pos;
	D3DXVECTOR3	_posInterpolation;
};

struct ScaleKey
{
	DWORD	    _time;
	D3DXVECTOR3	_scale;
	D3DXVECTOR3	_scaleInterpolation;
};

struct MatrixKey
{
	DWORD	   _time;
	D3DXMATRIX _matrix;
	D3DXMATRIX _matInterpolation;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ṹ�壺Animation
//
// ���ܣ�һ������������һϵ�еļ��������ϳɾ���ͽ��в�ֵ
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ�������Ľṹ������Ҳ��һ������Ľṹ��ÿһ��������Ӧһ���ͷ��һ����������һ��ģ��
//     ���ģ�;��ǵ�ǰ����������
//
//         ������Frame�Ĺ�ϵ��ÿһ�� Animation ��Ӧһ�� Frame ������ֵ����ʱ������ֱ�ӷ�Ӧ����Ӧ��
//     Frame �ϱ߶� Frame �� Animation ͨ��(_pFrameName)�����й�����
//
//         ����ѭ�����(_bLoop)��������Ƿ�ʹ��ѭ�������Ŷ���
//
//         �ģ�(_bLinear) ������δʹ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct Animation
{
	Animation()
	{
		_pName         = NULL;
		_pFrame        = NULL;
		_pFrameName    = NULL;
		_pNext         = NULL;
		_pRotateKeys   = NULL;
		_pPositionKeys = NULL;
		_pScaleKeys    = NULL;
		_pMatrixKeys   = NULL;

		_bLoop         = FALSE;
		_bLinear       = FALSE;

		_rotateKeysNum   = 0;
		_positionKeysNum = 0;
		_scaleKeysNum    = 0;
		_matrixKeyNum    = 0;

	}
	~Animation()
	{
		
		if(NULL != _pName)
		{
			delete [] _pName;
		}
		if(NULL != _pFrameName)
		{
			delete [] _pFrameName;
		}
		

		if(NULL != _pRotateKeys)
		{
			delete [] _pRotateKeys;
		}

		if(NULL != _pPositionKeys)
		{
			delete [] _pPositionKeys;
		}
		if(NULL != _pScaleKeys)
		{
			delete [] _pScaleKeys;
		}

		if(NULL != _pMatrixKeys)
		{
			delete [] _pMatrixKeys;
		}

		
		if(NULL != _pNext)
		{
			delete _pNext;
		}
	}

	char        *_pName;			// ��������һ��Ϊ�ա�
	Animation   *_pNext;			// ����

	char        *_pFrameName;		// �˶���������֡������������֡
	Frame       *_pFrame;			// ������֡���������ͷ�

	RotateKey   *_pRotateKeys;
	DWORD        _rotateKeysNum;
	
	PositionKey *_pPositionKeys;
	DWORD        _positionKeysNum;

	ScaleKey    *_pScaleKeys;
	DWORD        _scaleKeysNum;

	MatrixKey   *_pMatrixKeys;
	DWORD        _matrixKeyNum;

	BOOL         _bLoop;			// ����Ƿ�ѭ��
	BOOL         _bLinear;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ṹ�壺AnimationSet
//
// ���ܣ����������ڴ洢һ�鶯���������й���
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ���������Ľṹ��������Ϊ����ṹ�Է������
//
//         ����_length���˳�Ա��ʶ�˶�������󳤶ȣ���Animation�У����м���_time��Ա�����ֵ
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct AnimationSet
{
	AnimationSet()
	{
		_pName  = NULL;
		_pNext  = NULL;
		_pAnim  = NULL;
		_length = 0;
	}
	~AnimationSet()
	{
		if(NULL != _pName)
		{
			delete [] _pName;
		}
		
		if(NULL != _pAnim)
		{
			delete _pAnim;
		}

		if(NULL != _pNext)
		{
			delete _pNext;
		}
	}

	char         *_pName;

	AnimationSet *_pNext;
	Animation    *_pAnim;

	DWORD         _length;		// һ�������ĳ���
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ࣺ  CAnimation
//
// ���ܣ����ڶ����Ŀ��ƺ͹���
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ���ⲿ�ӿڣ�LoadXFile�� ��.X�ļ��м��ض�����Ϣ��pFileNameΪANSI���ļ�����pMapMeshΪ��֮
//                 �������������ָ��������������������ͬһ���ļ���
//
//                       SetLoop�����趨��ĳ���������ó�ѭ��ģʽ���������ı�ʶΪANSI���ַ������������
//                ����ѭ������ÿ�θ���ʱ��ʱ�佫�����ȡ����������в�ֵ���¶���
//                  
//                       SetCurAnimSet������ָ����ǰ��Ҫ���µĶ�����UpdateCurAnim�͸��´˶�����
//						
//                       UpdateCurAnim������ָ����CurAnimSet��ȫ���ľ�����Ϣ�����������Frame���˸�����Ϣ
//                �����µ�Frame��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CAnimation
{
public:

	CAnimation();
	~CAnimation();

	BOOL LoadXFile(const char *pFileName, CMesh *pMapMesh = NULL);
	void Destroy();

	BOOL SetLoop(const char *pAnimSetName, BOOL bLoop);
	BOOL SetCurAnimSet(const char *pAnimSetName, DWORD startTime);
	BOOL UpdateCurAnim(DWORD time, BOOL bSmooth);

protected:

	void _ParseXFileData(ID3DXFileData *pDXData, AnimationSet *pParentSet, Animation *pCurAnim);
	BOOL _MapToMesh(CMesh *pMesh);
	void _UpdateAnimationSet(AnimationSet *pAnimSet, DWORD time, BOOL bSmooth);
	void _UpdateAnimation(Animation *pAnim, DWORD time, BOOL  bSmooth);

	AnimationSet *_FindAnimSet(AnimationSet *pAnimSet, const char *pAnimName);

protected:

	
	AnimationSet *m_pAnimSet;
	DWORD         m_nAnimSet;			// ����������
	AnimationSet *m_pCurAnimSet;		// ��ʱ�洢�������ͷ�

	DWORD         m_animStartTime;		// �����Ŀ�ʼʱ��,���������ֵ
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ࣺ  CObject
//
// ���ܣ���CAnimatoin��CMeshͳһ��������������Ҿ�����������ԣ����Լ���λ�����꣬���Խ���λ�ñ任
//
// ��Ҫ���ܽ��ܣ�
//
//		   һ���ⲿ�ӿڣ�LoadXFile������.X�ļ��еĶ���������.X�ļ����Բ���������
//
//                       SetLoop����.x�ļ��а�������ʱ��ָ��ĳ����������ѭ�������򲻻�������
//                  
//                       SetCurAnimSet������ָ����ǰ��Ҫ���µĶ�����
//						
//                       Create���˷������ڼ�Ӵ���һ��object���˷���ʹ���Ѿ�����������(CMesh)��
//                 ����(CAnimation)������ô˷���������Destroy��Ҳ�����ͷ����õ�����Ͷ������ڴ�
//				   �˷�����Ҫ���ڴ����ظ������壬������Դռ��
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CObject
{
public:
	CObject();
	~CObject();

	void Destroy();

	void MoveTo(float xPos, float yPos, float zPos);
	void Move(float x, float y, float z);
	void RotateTo(float xRot, float yRot, float zRot);
	void Rotate(float x, float y, float z);
	void ScaleTo(float xRot, float yRot, float zRot);
	void Scale(float x, float y, float z);

	BOOL LoadXFile(CGraphics *pGraphics, const char *pFileName, const char *pTexturePath = ".\\");

	BOOL Create(CMesh *pMesh, CAnimation *pAnim = NULL);

	BOOL SetAnimation(const char *pAnimName, DWORD startTime);
	BOOL SetLoop(const char *pAnimSetName, BOOL bLoop);

	BOOL Render(DWORD curTime = 0, BOOL bSmooth = TRUE);


protected:
	
	CAnimation   *m_pAnim;
	CMesh        *m_pMesh;
	BOOL          m_bLoaded;	// ����Ƿ��Ǽ��صģ�����ǣ����ͷ��ڴ棬�����ͷ�

	CPosition     m_pos;
};

#endif