
#include "WGLContext.h"
#include <src/RenderSystem/E3DRenderSystem.hpp>

BOOL GL_Context::SwapBuffer()
{
	glFlush();
	return ::SwapBuffers(DisplayID);
}

BOOL GL_Context::UseContext()
{
	return wglMakeCurrent(DisplayID,hglrc);
}


BOOL GL_Context::ChangeSurface(HDC pDc)
{
	return FALSE;
}

BOOL GL_Context::OpenGLInit(HDC pDC)
{
	int n;								//��ǰѡ������ظ�ʽ����
	DisplayID = pDC;
	static PIXELFORMATDESCRIPTOR pfd =
	{									//���ص��ʽ��Ϣ�����ṹ��
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |		//֧��Windows��OpenGL��������ͼ
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,			//ʹ��˫���潻����ʾͼ��
		PFD_TYPE_RGBA,			    // RGBA type 
		32,							// 32-bit color depth 
		0, 0, 0, 0, 0, 0,			// color bits ignored 
		0,							// no alpha buffer 
		0,							// shift bit ignored 
		0,							// no accumulation buffer 
		0, 0, 0, 0,					// accum bits ignored 
		32,							// 32-bit z-buffer     
		8,							// no stencil buffer 
		0,							// no auxiliary buffer 
		PFD_MAIN_PLANE,				// main layer 
		0,							// reserved 
		0, 0, 0						// layer masks ignored 
	};

	int pixelformat;
	if ((pixelformat = ChoosePixelFormat(pDC, &pfd)) == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(pDC, pixelformat, &pfd) == FALSE)
	{
		return FALSE;
	}

	n = ::GetPixelFormat(pDC);
	::DescribePixelFormat(pDC, n, sizeof(pfd), &pfd);

	hglrc = wglCreateContext(pDC);	//���������龰����

	wglMakeCurrent(pDC, hglrc);		//ѡ������龰����
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glPointSize(1);
	glLineWidth(1);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return TRUE;
}
