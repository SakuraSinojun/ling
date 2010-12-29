
#ifndef GRAPHICS_H_101225
#define GRAPHICS_H_101225

// 释放COM对象
#define ReleaseCOM(x) if(x) { x->Release(); x = NULL; }

class CPosition;
class CTexture;
class CFont;
class CLight;
class CCamera;

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


	BOOL Init(HWND hWnd, BOOL bWindow = TRUE, DWORD width = 0, DWORD height = 0, char bpp = 32);
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

	void _WriteError(const char *error);
	char _GetFormatBPP(const D3DFORMAT &format);
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
	char              m_bpp;

	BYTE              m_ambientRed;
	BYTE              m_ambientGreen;
	BYTE              m_ambientBlue;	
};

//////////////////////////////////////////////////////////////////////////////////////
//
// 各种顶点类型
//
//////////////////////////////////////////////////////////////////////////////////////

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

struct TexVertex2D
{	
	TexVertex2D(){}

	TexVertex2D(float x, float y, float z, float rhw, float u, float v)
	{
		_x  = x;  _y  = y;	_z  = z;
		_rhw = rhw;
		_u = u; _v = v;
	}
	float _x, _y, _z;     // 2-D 坐标 z置1
	float _rhw;           // rhw
	float _u, _v;         // 材质坐标

	static const DWORD FVF;
};

struct TexVertex3D
{	
	TexVertex3D(){}

	TexVertex3D(float x, float y, float z, float u, float v)
	{
		_x  = x;  _y  = y;	_z  = z;
		_u = u; _v = v;
	}
	float _x, _y, _z;     // 3D 坐标
	float _u, _v;         // 材质坐标

	static const DWORD FVF;
};

class CVertex
{
public:
	CVertex();
	~CVertex();

	void *GetBufferPtr()        const {return m_pBuffer;}
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

	void _WriteError(const char *error);

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

class CTexture
{
public:

	CTexture();
	~CTexture();

	IDirect3DTexture9 *GetTexture() {return m_pTexture;}
	DWORD GetWidth()  const {m_d3dsd.Width;}
	DWORD GetHeight() const {m_d3dsd.Height;}

	BOOL LoadFromFile(CGraphics *pGraphics, const char *fileName, DWORD colorKey = 0, D3DFORMAT format = D3DFMT_UNKNOWN);
	BOOL Destroy();

	BOOL Blt(long destX, long destY, long srcX = 0, long srcY = 0, long width = 0, long height = 0, 
		FLOAT xScale = 1.0f, FLOAT yScale = 1.0f, D3DCOLOR color = 0xFFFFFFFF);

	BOOL Rotate(float centerX, float centerY, FLOAT angle);
	BOOL Scale(float centerX, float centerY, FLOAT scaleX, FLOAT scaleY);
	BOOL Translation(float x, float y);

protected:

	void _WriteError(const char *error);

protected:

	CGraphics         *m_pGraphics;
	IDirect3DTexture9 *m_pTexture;
	D3DSURFACE_DESC    m_d3dsd;

	BOOL  m_bLoaded;

};

class CFont
{
public:

	CFont();
	~CFont();

	BOOL Create(CGraphics *pGraphics, INT height = 24, const char *fontName = NULL, BOOL bItalic = FALSE);
	void Destroy();

	BOOL Print(const char *text, long xPos, long yPos, long width, long height, DWORD format = DT_CENTER | DT_VCENTER, D3DCOLOR color = 0xffffffff);

protected:

	void _WriteError(const char *error);

private:

	ID3DXFont *m_pFont;
};



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

	const D3DXMATRIX *GetMatrix();

	void MoveTo(float xPos, float yPos, float zPos);
	void Move(float x, float y, float z);
	void RotateTo(float xRot, float yRot, float zRot);
	void Rotate(float x, float y, float z);
	void ScaleTo(float xRot, float yRot, float zRot);
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


class CCamera : public CPosition
{
public:

	CCamera();
	~CCamera();

	void SetLookAt(float xEye, float yEye, float zEye, float xAt, float yAt, float zAt);

protected:

};

struct Mesh
{
	Mesh()
	{
		_pName           = NULL;  // Clear all structure data

		_pDXMesh         = NULL;
		_pDXSkinMesh     = NULL;
		_pSkinInfo       = NULL;

		_materialNum     = 0;
		_pMaterials      = NULL;

		_apTextures      = NULL;

		_pNext           = NULL;
	}
	~Mesh()
	{
		delete [] _pName;

		ReleaseCOM(_pDXMesh);
		ReleaseCOM(_pDXSkinMesh);
		ReleaseCOM(_pSkinInfo);

		delete [] _pMaterials;

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

	ID3DXMesh          *_pDXMesh;
	ID3DXMesh          *_pDXSkinMesh;
	ID3DXSkinInfo      *_pSkinInfo;

	D3DMATERIAL9       *_pMaterials;
	DWORD               _materialNum;

	IDirect3DTexture9 **_apTextures;

	D3DXMATRIX          _matMatrix;

	Mesh               *_pNext;
};

struct Frame
{
	Frame()
	{
		_pName    = NULL;
		_pMesh    = NULL;
		_pSibling = NULL;
		_pChild   = NULL;
	}
	~Frame()
	{
		delete[] _pName;
		delete   _pMesh;
		delete   _pSibling;
		delete   _pChild;
	}

	char  *_pName;
	Mesh  *_pMesh;
	Frame *_pSibling;
	Frame *_pChild;
};

class CFrame
{
public:

	CFrame();
	~CFrame();

	BOOL LoadXFile(CGraphics *pGraphics, const char *fileName);

	BOOL Render();

protected:

	CGraphics *m_pGraphics;

	void _WriteError(const char *error);

	BOOL _DrawFrame(Frame *pFrame);

	BOOL ParseXFile(const char *fileName);
	void ParseXFileData(ID3DXFileData *pDataObj, Frame *pFrame);

	Frame *m_pFrame;

};

#endif