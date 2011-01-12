//////////////////////////////////////////////////////////////////////////////////////////
//
// name: graphics.h
// func: 一套使用D3D的3D图形引擎
//		 SDK 版本为 Microsoft DirectX SDK (June 2008)
// date: 2011.01.01	
// lib : d3d9.lib，d3dx9.lib，winmm.lib，D3dxof.lib，DXGUID.LIB
// 参考：C++ -Game  Programming RPGs With DirectX（主要）
//       Introduction to 3D Game Programming with DirectX.9
//       Skinned Mesh Character Animation with Direct3D 9.0c
//       
///////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------更新日志-----------------------------------------------/
//
// 2011.01.10 完成动画的加载
//
// 2011.01.11 将工程设置为Unicode版本，解决无法用CFont无法打印中文的问题
//
// 2011.01.12 补写各个类和结构体的主要说明介绍
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

// 释放COM对象
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
// 类名：CGraphics 
//
// 功能：图形库的基础类，实现各种DX的全局控制
//
// 主要功能说明：
//
//	       一，初始化：在使用前，首先应调用Init进行初始化，hWnd是关联的windows窗口句柄,默认情况下，
//     建立一个窗口形式的与关联窗口尺寸相同的32色深的窗口。可以通过更改默认参数设置需要的窗口类型。
//
//	       二，清屏：在绘制每一帧前，应先清除画面，此类中包含三种清除画面的方法Clear, ClearZBuffer，
//     ClearDisplay, ClearDiaplay值清除当前画面，一般用于2D，ClearZBuffer用于清除Z缓存，Clear使用
//     时会判断是否使用了Z缓存，如果没有使用，只调用ClearDisplay，否则调用以上两种。
//
//         三，绘图：在绘图前，必须调用BeginScene来锁定画面，在绘制结束后调用EndScene来解锁，最后通过
//     Display方法将缓冲画面绘制到屏幕上。
//
//         四，位置设定:SetPerspective可以设置视口的相关参数。SetWorldPos用于设定物体的世界坐标，
//     此方法接受两种参数，一种是D3DX矩阵的引用，另一种是CPostion的引用。SetCamera用于设定相机坐
//     标，其根本是视点的位置。
//
//         五，全局设定: SetAmbientLight 设定环境光；SetShadeMode设定着色形式；SetFillMode填充形式，
//     与具体的顶点类型配合；SetLignt，将某个光源加进图形引擎，并设定一个索引；
//
//         六，功能的开启，关闭：EnableLight，使某个索引的光源生效或失效；EnableLighting是否开启光照；
//     EnableZBuffer Z缓存；EnableAlphaBlending Alpha混合
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
// 各种顶点类型
//
//////////////////////////////////////////////////////////////////////////////////////

// 线框型顶点
// 使用此顶点类型时，需要将调用CGraphics的SetFillMode方法, 
// 将FillMode设置为D3DFILL_WIREFRAME

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

// 这是一种带有顶点颜色的的顶点类型

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

// 普通3D顶点，具有材质坐标

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

// 功能全面的3D顶点，拥有材质坐标和顶点颜色

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

// 用于绘制2D纹理的顶点类型

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
	float _u, _v;         // 纹理坐标

	static const DWORD FVF;
};

// 用于绘制3D纹理的顶点类型

struct TexVertex3D
{	
	TexVertex3D(){}

	TexVertex3D(float x, float y, float z, float u, float v)
	{
		_x  = x;  _y  = y;	_z  = z;
		_u = u; _v = v;
	}
	float _x, _y, _z;     // 3D 坐标
	float _u, _v;         // 纹理坐标

	static const DWORD FVF;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 类名：CVertex
//
// 功能：管理顶点数组，并按一定规则绘制，其中包括顶点缓存与顶点索引，如果没有创建顶点索引，则直接绘制
//       顶点缓存，否则，按照索引进行绘制，这个类不太完善，只能实现一些简单的顶点操作
//
// 主要功能说明：
//
//	       一，初始化：CreateBuffer用于创建一个顶点缓存，需要传进顶点数(vertexNum)，顶点尺寸
//     (vertexSize)此尺寸为每个顶点的内存大小，以及顶点标记(FVF)
//
//	       二，锁定：在进行填充前，必须先进行锁定LockBuffer，LockIndex，此方法一般为内部使用，当使用
//     底层DX进行操作时，可以调用，参数为锁定的起始顶点和锁定顶点的总数(从起始顶点开始)。在填充数据
//     之后，需要调用相应的结束锁定函数
//
//         三，填充：SetBuffer,SetIndex分别用来填充顶点缓存与顶点索引数组，填充类型需要与Create
//     时声明的类型相同
//
//         四，渲染：Render，startIndex为起始顶点索引，primitiveCount为每个面的三角形数量，type
//     为平面绘制参数默认为线性渲染
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
// 类名：CTexture
//
// 功能：管理纹理进行简单的纹理操作
//
// 主要功能说明：
//
//	       一，加载：LoadFromFile，从文件中加载一个纹理，colorKey可以指定关键色，format可以指定
//     顶点格式，默认为文件默认格式
//
//	       二，绘制：BLT，此函数利用ID3DXSprite进行简单的绘制，其方式类似于DDRAW方式,纹理还有一种与
//     纹理顶点配合绘制的方式，此类中暂时没有提供
//
//         三，变换：Rotate，Scale，Translation 代表旋转，缩放，平移
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
// 类名：CFont
//
// 功能：ID3DXFont字体，效率比较低下，在非UNICODE工程中，不能绘制中文
//
// 主要功能说明：
//
//	       一，初始化：Create,可以指定字体高度，字体名称(用来加载不同类型的字体)以及是否斜体(bItalic)
//
//	       二，绘制：可以指定文字的起始坐标，矩形框尺寸，默认为左对齐，上对齐，颜色为白色
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
// 类名：CLight
//
// 功能：管理DX中的各种光照，包括环境光，镜面反射光，漫反射光，点光源，方向光源和聚光灯
//
// 主要功能说明：
//
//		   设置类型  ：SetType D3DLIGHT_POINT(电光源), D3DLIGHT_SPOT(聚光灯),D3DLIGHT_DIRECTIONAL(方向光),
//	       环境光    ：SetAmbientColor 镜面发射光：SetSpecularColor 漫反射光  ：SetDiffuseColor
//         光照范围  ：Range 不影响方向光          聚光灯参数：SetFalloff 经常设置为1.0
//		   聚光灯外角：SetPhi  聚光灯内角：SetTheta 
//		   衰减参数  ：etAttenuation0,1,2，
//         设置光源位置：MoveTo移动到目标点(绝对位移) Move 移动增量，相对位移
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
// 类名：CCamera
//
// 功能：管理相机，通过相机调整观察位置
//
// 主要功能说明：
//
//		   一，调整相机位置和指向： SetLookAt，指定观察者位置和观察目标位置
//
//		   二，相机的位移，带有To的是绝对位移，不带To的为相对位移(与上一次)
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
// 结构体：Mesh
//
// 功能：  管理DX网格信息
//
// 主要功能介绍：
//
//		   一，网格的操作分为有骨骼信息和无骨骼信息两种
//
//		   二，对于无骨骼信息的加载 _pSkinInfo  和 _pDXSkinMesh 将被置为 NULL 此时，绘制网格应该用原始
//     网格 _pDXMesh，而此时的骨骼矩阵即为原始矩阵(_matOriginal)，而不是使用变换矩阵(_matTransformed)
//
//         三，网格结构体是一个链表的形式(_pNext)，用于连接与此网格关联在一起的网格，进行同步的更新和绘制
//
//		   四，网格包含材质数组指针(_pMaterials)与纹理指针数组(_apTextures)，
//
//         五，网格包含边界球半径(_radius)和边界框范围(_min,_max)
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

	ID3DXMesh          *_pDXMesh;			// 原始网格
	ID3DXMesh          *_pDXSkinMesh;		// 用于绘制的网格
	ID3DXSkinInfo      *_pSkinInfo;			// 网格信息
	D3DXMATRIX         *_pBoneMatrices;		// 骨骼变换矩阵，这个最终的矩阵数组
	D3DXMATRIX        **_apFrameMatrices;	// 帧矩阵数组，它起到一个中间层的作用，当用CAnimation和CMesh单独加载时，它用于关联两者

	D3DMATERIAL9       *_pMaterials;	// 材质数组
	DWORD               _materialNum;	// 材质数量

	IDirect3DTexture9 **_apTextures;	// 纹理指针数组

	Mesh               *_pNext;			// 关联一个mesh，构成列表

	D3DXVECTOR3         _min;			// 边界框
	D3DXVECTOR3         _max;
	float               _radius;		// 边界球半径
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 结构体：Frame
//
// 功能：  管理DX帧信息
//
// 主要功能介绍：
//
//		   一，帧的管理形式为二叉树，可以理解为，每个帧对应一块骨骼，在一个模型中，所有的骨骼有父，子，
//     兄弟的关系，以手臂为例，手臂分为上臂，前臂和手，当手运动时，不会影响到前臂和上臂的位置，因此手
//     充当子的角色，相反，如果上臂抬起，必然会引起前臂和手的位置的变化。这种变化是通过遗传来实现的。
//
//		   二，Frame结构中的矩阵，骨骼只是一个理想的东西，并不能直观的反应到画面上，因此骨骼必须与网格
//	   配合，即把骨头与网格的每一个部分一一对应起来。对应的方法就是矩阵，_matOriginal对应的是网格的初始
//     位置信息，_matTransformed对应骨骼的变化，_matCombined对应骨骼的最终位置信息，当加载网格时，若网格
//     包含骨骼信息，则使用_matTransformed来更新矩阵，若不包含，则使用_matOriginal来更新。_matCombined
//     与mesh中的最终变换矩阵相对应。
//
//         三，每一个frame都可以关联一个mesh也可以不关联
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

		// parent不用释放，都包含在孩子里了
	}

	char       *_pName;

	// 这个暂时只有parent拥有

	Mesh       *_pMesh;

	// 这些矩阵是FRAME的精要

	D3DXMATRIX  _matCombined;    // 合成矩阵，每一个骨骼最终的变换矩阵
    D3DXMATRIX  _matTransformed; // 当前变换矩阵
    D3DXMATRIX  _matOriginal;    // 原始矩阵

	// 二叉树
	Frame      *_pParent;		
	Frame      *_pSibling;
	Frame      *_pChild;
}; 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 类：  CMesh
//
// 功能：将mesh与frame关联在一起已经管理，用于从DX文件中加载网格与帧信息，并可以进行绘制
//
// 主要功能介绍：
//
//		   一，外部接口：外部接口只提供加载，销毁和绘制三种方法，LoadXFile使用时需要传入一个有效图形
//     设备的指针pGraphics，ANSI的文件名，以及ANSI的纹理存放位置的根目录，默认为当前目录。
//
//		   二，网格的渲染：Render，第一个参数CPosition为世界坐标，如果使用，则会移动到指定位置
//     第二个参数bTransformed标识是否使用变换矩阵进行更新，如Frame说明中所述，如果加载的网格不
//     包含蒙皮骨骼，则应该设置为FALSE，否则，应该使用TRUE；
//
//         三，渲染过程说明：渲染内部调用三个保护成员函数_UpdateFrames，_UpdateMeshes，_DrawMeshes
//     _UpdateFrames函数为动画的遗传过程，进行合成矩阵的更新，_UpdateMeshes用骨骼信息更新网格，根据
//     每一个骨骼的位置，来设置网格的位置_DrawMeshes进行最终的渲染，如果没有使用蒙皮骨骼，则使用Mesh
//     中的原始网格渲染，否则使用蒙皮网格渲染
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

	Mesh        *m_pMeshes;		// 顶级指针,这个指针存放着frame中的全部mash，因此不负责释放资源
	Frame       *m_pFrames;		// 各种关联的帧

	DWORD        m_frameNum;
	DWORD        m_meshNum;

	D3DXVECTOR3  m_min;
	D3DXVECTOR3  m_max;
	float        m_radius;
};	

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 结构体：XFileRotateKey，XFileScaleKey，XFilePositionKey，XFileMatrixKey
//         RotateKey，PositionKey，ScaleKey，MatrixKey
//
// 功能：前四个为加载DX文件时，读取内存时进行对齐时使用，后四个为将前四个转化成DX方便管理的格式后
//       在程序内部使用
//
// 主要功能介绍：
//
//		   一，键的理解，一个动画集(AnimationSet)来完成一组特定的动作，一个动作(Animation)就是一个画面
//     一个画面的位置由一个矩阵来决定，而矩阵由键来得到。
//
//         二，如果.X文件中提供了MatrixKey，则可以直接拿来使用，如果给的是RotateKey，PositionKey，
//     ScaleKey，则需合成DX矩阵后使用
//
//         三，关于_time，这个成员用来标记该动作在一组动作中的位置，也是动作中关键帧的位置如果给定的是
//     连续动作，则_time的值为 一组连续的值，比如1 2 3 4 5。此时无需进行插值，如果动作可以进行插值，
//     则_time的值为 1 3 5 这样，则可以利用插值原理进行插值，来计算中间的帧，实现动画的平滑
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
// 结构体：Animation
//
// 功能：一个动作，包含一系列的键，用来合成矩阵和进行插值
//
// 主要功能介绍：
//
//		   一，动作的结构，动作也是一个链表的结构，每一个动作对应一块骨头，一个链表连起一个模型
//     这个模型就是当前的整个画面
//
//         二，与Frame的关系，每一个 Animation 对应一个 Frame ，当键值更新时，可以直接反应到相应的
//     Frame 上边而 Frame 和 Animation 通过(_pFrameName)来进行关联。
//
//         三，循环标记(_bLoop)用来标记是否使用循环来播放动画
//
//         四，(_bLinear) 保留，未使用
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

	char        *_pName;			// 动画名，一般为空。
	Animation   *_pNext;			// 链表

	char        *_pFrameName;		// 此动画关联的帧名，用来关联帧
	Frame       *_pFrame;			// 关联的帧，不负责释放

	RotateKey   *_pRotateKeys;
	DWORD        _rotateKeysNum;
	
	PositionKey *_pPositionKeys;
	DWORD        _positionKeysNum;

	ScaleKey    *_pScaleKeys;
	DWORD        _scaleKeysNum;

	MatrixKey   *_pMatrixKeys;
	DWORD        _matrixKeyNum;

	BOOL         _bLoop;			// 标记是否循环
	BOOL         _bLinear;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 结构体：AnimationSet
//
// 功能：动画集用于存储一组动作，并进行管理
//
// 主要功能介绍：
//
//		   一，动作集的结构，动画集为链表结构以方便查找
//
//         二，_length，此成员标识此动画的最大长度，即Animation中，所有键的_time成员的最大值
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

	DWORD         _length;		// 一个动画的长度
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 类：  CAnimation
//
// 功能：用于动画的控制和管理
//
// 主要功能介绍：
//
//		   一，外部接口：LoadXFile： 从.X文件中加载动画信息，pFileName为ANSI的文件名，pMapMesh为与之
//                 关联的网格，如果指定，动画和网格需来自同一个文件；
//
//                       SetLoop：来设定将某个动画设置成循环模式，动画集的标识为ANSI的字符串，如果设置
//                成了循环，则每次更新时，时间将会进行取余操作，进行插值更新动画
//                  
//                       SetCurAnimSet：用来指定当前需要更新的动画集UpdateCurAnim就更新此动画集
//						
//                       UpdateCurAnim：更新指定的CurAnimSet的全部的矩阵信息，如果关联的Frame，此更新信息
//                将更新到Frame中
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
	DWORD         m_nAnimSet;			// 动画集数量
	AnimationSet *m_pCurAnimSet;		// 临时存储，不用释放

	DWORD         m_animStartTime;		// 动画的开始时间,用来计算差值
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 类：  CObject
//
// 功能：将CAnimatoin和CMesh统一起来方便管理，并且具有物体的特性，有自己的位置坐标，可以进行位置变换
//
// 主要功能介绍：
//
//		   一，外部接口：LoadXFile：加载.X文件中的动画和网格，.X文件可以不包含动画
//
//                       SetLoop：当.x文件中包含动画时，指定某个动画进行循环，否则不会起作用
//                  
//                       SetCurAnimSet：用来指定当前需要更新的动画集
//						
//                       Create：此方法用于间接创建一个object，此方法使用已经建立的网格(CMesh)和
//                 动画(CAnimation)，如果用此方法建立，Destroy中也不会释放引用的网格和动画的内存
//				   此方法主要用于创建重复的物体，减少资源占用
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
	BOOL          m_bLoaded;	// 标记是否是加载的，如果是，则释放内存，否则不释放

	CPosition     m_pos;
};

#endif