

//#define _STDCALL_SUPPORTED 
//#define _M_IX86 
//#define GLUT_DISABLE_ATEXIT_HACK

///////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <gl/gl.h>
#include "gl/glut.h"

#include "images.h"



#include "lib3ds/lib3ds.h"

//#include <stdbool.h>



#define GL_PI 3.14159265359




///////////////////////////////////////////////////////////

//static GLfloat 	xRot = 0.0f;
//static GLfloat 	yRot = 0.0f;
static GLint		mouse_x;
static GLint		mouse_y;

static GLdouble	eye_vangle;
static GLdouble	eye_x;
static GLdouble	eye_y;
static GLdouble	eye_z;
static GLdouble	eye_hangle;


static unsigned int	m_totalfaces;

static Lib3dsFile *	m_model;
static int		m_key[256];



void render_node(Lib3dsNode * node)
{
	Lib3dsMesh * 	mesh;
	int		i;
	int		index;
	Lib3dsMeshInstanceNode * n;
	int		j;
	float	(*normalL)[3];

	if(node->type == LIB3DS_NODE_MESH_INSTANCE)
	{
		n = (Lib3dsMeshInstanceNode*)node;
		
		if(strcmp(node->name, "$$$DUMMY") == 0)
		{
			return;
		}

		index = lib3ds_file_mesh_by_name(m_model, n->instance_name);
		if(index < 0)
		{
			index = lib3ds_file_mesh_by_name(m_model, node->name);
		}
		if(index < 0)
		{
			return;
		}
		mesh = m_model->meshes[index];
		
		if( mesh->user_id == 0 )
		{
			assert(mesh);
			//mesh->user_id = glGenLists(1);
			//glNewList(mesh->user_id, GL_COMPILE);

			normalL = (float(*)[3])malloc(3*3*sizeof(float)*mesh->nfaces);
			lib3ds_mesh_calculate_vertex_normals(mesh, normalL);
			glBegin(GL_TRIANGLES);
				for(j=0; j<mesh->nfaces; j++)
				{
				
					glColor4d(1.0, 1.0, 0.0, 1.0);
					for(i=0; i<3; i++)
					{
						glNormal3fv(normalL[3*j+i]);
						glVertex3fv(mesh->vertices[mesh->faces[j].index[i]]);
					}
				
				}
			glEnd();
		}

	}

}


void RendDraw()
{
	Lib3dsNode *	node;
	Lib3dsNode *	p;



	
	for(p = m_model->nodes; p!=NULL; p=p->next)
	{
		for(node=p->childs; node!=NULL; node=node->next)
		{
			render_node(node);
		}
		render_node(p);
	}
	
	
}


void myDisplay(void) 
{
	
	GLfloat dist = 100;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	
	glPushMatrix();
	
	gluLookAt(eye_x, eye_y, eye_z, 
			eye_x + dist * cos(eye_vangle) * cos(eye_hangle),
			eye_y + dist * sin(eye_hangle),
			eye_z + dist * sin(eye_vangle) * cos(eye_hangle),
			cos(eye_vangle) * cos(eye_hangle + GL_PI / 2.0),
			sin(eye_hangle + GL_PI / 2.0),
			sin(eye_vangle) * cos(eye_hangle + GL_PI / 2.0));
	
	/*
	printf("vangle = %f, hangle = %f \n", eye_vangle * 180.0f / GL_PI, eye_hangle * 180.0f / GL_PI);
	printf("观察点:(%f, %f, %f) \n观察向量:(%f, %f, %f) \n法向量(%f, %f, %f) \n内积 %f\n", 
		eye_x, eye_y, eye_z, 								//观察点
		cos(eye_vangle) * cos(eye_hangle), 
		sin(eye_hangle), 
		sin(eye_vangle) * cos(eye_hangle),						//观察向量
		cos(eye_vangle) * cos(eye_hangle + GL_PI / 2.0f), 
		sin(eye_hangle + GL_PI / 2.0f), 
		sin(eye_vangle) * cos(eye_hangle + GL_PI / 2.0f),		//法向量
		cos(eye_vangle)*cos(eye_hangle)*cos(eye_vangle)*cos(eye_hangle+GL_PI/2.0f) 
		+ sin(eye_hangle)*sin(eye_hangle+GL_PI/2.0f)
		+ sin(eye_vangle)*cos(eye_hangle)*sin(eye_vangle)*cos(eye_hangle+GL_PI/2.0f)
		);
	*/	
	

	RendDraw();
//	glutSolidTeapot(1);


	glPopMatrix();
	glutSwapBuffers();
	
}

void ResetCamera()
{
	eye_x = 0;
	eye_y = 100;
	eye_z = 400;
	eye_vangle = - GL_PI / 2.0;
	eye_hangle = - GL_PI / 15.0;
}

void SetupRC(void)
{
	Lib3dsMesh * 	mesh;
	Lib3dsNode *	node;
	int		i;

	
	ResetCamera();	
	glClearColor(0.2, 0.2, 0.2, 0);
	

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	//glShadeModel(GL_SMOOTH);
	//glFrontFace(GL_CW);
	//glPolygonMode(GL_BACK, GL_FILL);
	
	//glEnable(GL_TRIANGLE_SMOOTH);

	
	
	m_model = lib3ds_file_open("Sample.3ds");
	
	if( m_model == NULL )
	{
		printf("文件加载错误！");
		exit(1);
	}
	
	if ( !m_model->nodes )
	{
		for(i=0; i<m_model->nmeshes; i++)
		{
			mesh = m_model->meshes[i];
			node = lib3ds_node_new(LIB3DS_NODE_MESH_INSTANCE);
			strcpy(node->name, mesh->name);
			lib3ds_file_insert_node(m_model, node, NULL);
		}
	}
	

	lib3ds_file_eval(m_model, 0.0f);
	
	

}


// Respond to arrow keys
void SpecialKeys(int key, int x, int y)
{
	m_key[key] = 1;
}

void SpecialKeysUp(int key, int x, int y)
{
	m_key[key] = 0;
	
	if(key == GLUT_KEY_HOME)
	{
		ResetCamera();
	}

}

void processNormalKeys(unsigned char key,int x,int y)
{
	m_key[key] = 1;

	if(key == 27)
	{
		exit(0);
	}

	if(key == 'p')
	{
		images_save_bmp("screenshot.bmp");
	}
	
}


void processNormalKeysUp(unsigned char key,int x,int y)
{
	m_key[key] = 0;	
}

void MouseHandler(int button, int state, int x, int y)
{
	
	if (state == GLUT_DOWN)
	{
		//if(button == GLUT_LEFT_BUTTON)
		{
			mouse_x = x;
			mouse_y = y;
		}
	}
}

void MouseMotion(int x, int y)
{
	
	eye_vangle += (x - mouse_x)/200.0;
	mouse_x = x;
	
	eye_hangle -= (y - mouse_y)/200.0;
	mouse_y = y;
	
	if(eye_hangle <= 0 - GL_PI / 2.0)
	{
		eye_hangle = -GL_PI / 2.0;
	}
	
	if(eye_hangle >= GL_PI / 2.0)
	{
		eye_hangle = GL_PI / 2.0;
	}
	
	//glutPostRedisplay();
}

void PassiveMotion(int x, int y)
{

}

void ChangeSize(GLsizei w, GLsizei h)
{

	if(h==0)
	{
		h=1;
	}
	
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 1.0, 4000.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(0.0f, 0.0f, -400.0f);
	
}



void Timer(int value)
{
	GLdouble dist = 1.0;

	if(m_key[GLUT_KEY_UP] != 0)
	{
		eye_z += dist * sin(eye_vangle);
		eye_x += dist * cos(eye_vangle);
		eye_y += dist * sin(eye_hangle); 
	}
	if(m_key[GLUT_KEY_DOWN] != 0)
	{
		eye_z -= dist * sin(eye_vangle);
		eye_x -= dist * cos(eye_vangle);
		eye_y -= dist * sin(eye_hangle);
	}

	if(m_key[GLUT_KEY_LEFT] != 0)
	{
		eye_z += dist * sin(eye_vangle - GL_PI/2.0);
		eye_x += dist * cos(eye_vangle - GL_PI/2.0);
	}
	
	if(m_key[GLUT_KEY_RIGHT] != 0)
	{
		eye_z += dist * sin(eye_vangle + GL_PI/2.0);
		eye_x += dist * cos(eye_vangle + GL_PI/2.0);
	}
	
	if(m_key[(int)'.'] != 0) 
	{
		eye_y += 1;
	}
	
	if(m_key[(int)','] != 0)
	{
		eye_y -= 1;
	}
	


	glutTimerFunc(5, Timer, 0);
	//glutPostRedisplay();
}
    
///////////////////////////////////////////////////////////


int main(int argc, char *argv[]) 
{ 
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ACCUM); 
	glutInitWindowPosition(400, 200); 
	glutInitWindowSize(600, 480); 
	glutCreateWindow("lib3ds库加载3d模型"); 
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(myDisplay);
	

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	glutSpecialUpFunc(SpecialKeysUp);


	glutMouseFunc(MouseHandler);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(PassiveMotion);
	glutReshapeFunc(ChangeSize);
	
	glutTimerFunc(50, Timer, 0);

	SetupRC();
	
	glutMainLoop(); 

	return 0; 
}

















