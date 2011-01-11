//////////////////////////////////////////////////////////////////
//
// name: graphics.h
// func: һ��ʹ��D3D��3Dͼ������
//		 SDK �汾Ϊ Microsoft DirectX SDK (June 2008)
// date: 2011.01.01	
// lib : d3d9.lib��d3dx9.lib��winmm.lib��D3dxof.lib��DXGUID.LIB
//
///////////////////////////////////////////////////////////////////

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

class CPosition;
class CTexture;
class CFont;
class CLight;
class CCamera;
class CAnimation;
class CObject;
struct AnimationSet;

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
	float _x, _y, _z;     // 2-D ���� z��1
	float _rhw;           // rhw
	float _u, _v;         // ��������

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
	float _x, _y, _z;     // 3D ����
	float _u, _v;         // ��������

	static const DWORD FVF;
};

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

// ��������
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
// ֡
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




// �����Ǽ���DX��������Ҫ�Ľṹ��
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

// һ������

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

// ������

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


class CAnimation
{
public:

	CAnimation();
	~CAnimation();

	BOOL LoadXFile(const char *fileName, CMesh *pMapMesh = NULL);
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