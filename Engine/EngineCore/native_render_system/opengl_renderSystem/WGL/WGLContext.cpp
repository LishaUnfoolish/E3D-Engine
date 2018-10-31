
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

HGLRC GL_Context::choosePixelFormat(HDC hDC)
{
	HGLRC m_hRC = 0;
	//	PIXELFORMATDESCRIPTOR pfd;
	//int nPixelFormat = -1;
	//int nPixCount = 0;
	//float fPixAttribs[] = { 0,0 };
	//int iPixAttribs[] =
	//{
	//	WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering
	//	WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window
	//	WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB, // must be HW accelerated
	//	WGL_COLOR_BITS_ARB,     32, // 8 bits of each R, G and B
	//	WGL_DEPTH_BITS_ARB,     24, // 24 bits of depth precision for window, �����������Ȼ���������24λ��,���˼�̨��������֧��32λ��Ȼ�����
	//	WGL_DOUBLE_BUFFER_ARB,GL_TRUE, // Double buffered context
	//	WGL_PIXEL_TYPE_ARB,      WGL_TYPE_RGBA_ARB, // pf should be RGBA type
	//	WGL_STENCIL_BITS_ARB, 8,//����ģ�建����,ģ�建����λ��=8
	//	WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // MSAA on,�������ز���
	//	WGL_SAMPLES_ARB,        4, // 4x MSAA ,���ز�����������Ϊ4
	//	0 }; // NULL termination
	//		 //����һ����ʱ����,ֻ��Ϊ�˳�ʼ��glew��
	//
	//tempWnd->Create(_T("STATIC"), _T("Hi"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), AfxGetMainWnd(), NULL);
	//HDC tempHdc = tempWnd->GetDC()->GetSafeHdc();
	//// Attempt to set the pixel format
	//if (!SetPixelFormat(tempHdc, 1, &pfd))//ÿ������ֻ������һ��
	//	return NULL;
	//HGLRC temphRC = wglCreateContext(tempHdc);//����һ����ʱ�Ļ���Ϊ�˳�ʼ��glew,��ʼ������ܹ�ʹ��wglChoosePixelFormatARB��wglCreateContextAttribsARB����
	//wglMakeCurrent(tempHdc, temphRC);//ֻ�����õ�ǰopengl��������ܹ���ʼ��glew��
	//GLenum err = glewInit();//��ʼ��glew��,һ�������г�ʼ��һ�ξͿ���,����Ҫÿ��opengl��������ʼ��
	//if (GLEW_OK != err)
	//	return NULL;
	//wglMakeCurrent(NULL, NULL);
	//wglDeleteContext(temphRC);
	//tempWnd->DestroyWindow();//������ʱ����
	//delete tempWnd;
	////�µĲ�ѯ���ظ�ʽ�ĺ���
	//wglChoosePixelFormatARB(hDC, iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	////���ز���ʱ,���Ӳ����֧�־�ʹ������Ĵ���رն��ز���
	//if (nPixelFormat == -1)
	//{
	//	// Try again without MSAA
	//	iPixAttribs[18] = 1;
	//	wglChoosePixelFormatARB(hDC, iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	//}
	//// Got a format, now set it as the current one
	//if (!SetPixelFormat(hDC, nPixelFormat, &pfd))
	//{
	//	return NULL;
	//}
	//GLint attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB,  3,//���汾3
	//	WGL_CONTEXT_MINOR_VERSION_ARB,  3,//�ΰ汾��3
	//	WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//Ҫ�󷵻ؼ���ģʽ����,�����ָ����ָ��ΪWGL_CONTEXT_CORE_PROFILE_BIT_ARB�᷵��ֻ�������Ĺ��ܵĻ���
	//	0 };
	//m_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	//if (m_hRC == NULL)
	//{
	//	printf("!!! Could not create an OpenGL 3.3 context.\n");
	//	attribs[3] = 2;
	//	m_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
	//	if (m_hRC == NULL)
	//	{
	//		printf("!!! Could not create an OpenGL 3.2 context.\n");//Ҫ��opengl3.2���ϻ���
	//		return NULL;
	//	}
	//}
	//wglMakeCurrent(NULL, NULL);
	return m_hRC;
}

BOOL GL_Context::OpenGLInit(HDC pDC)
{
	int pixelformat;

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines  
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_POLYGON_SMOOTH);     //����ο����  
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	return TRUE;
}
