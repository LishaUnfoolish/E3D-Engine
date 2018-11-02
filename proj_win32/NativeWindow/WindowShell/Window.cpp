// EditorCore.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Window.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING] = "MAINWINDOW";			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND				MainWindowHwnd;
int					WindowHeight = 800;
int					WindowWidth = 1000;
DWORD				WindowStyle = 0;
bool				IsMouseDown = false;
bool				IsRButtonDown = false;
POINT				LastMousePoint;
float				CameraPitch = 0;
float				CameraYaw = 0;
bool				MButtonDown = false;
bool				bExit = false;
std::fstream		InFile;
unsigned int		threadID = 0;
std::string logFileName = "E3DLog.log";
HANDLE g_hConsoleHandle = 0;
CRITICAL_SECTION            gPhysicsSection;
HANDLE						hPhysicsThread = NULL;


#ifdef _DEBUG
#define EXE_NAME "WinClient_d.exe"
#endif
#ifdef NDEBUG
#define EXE_NAME "WinClient.exe"
#endif

#define ONE_INSTANCE 1
std::string strPath = "";
bool isShowWithEditor = false;
HWND editorHandle = nullptr;
std::string startScenePath = "";

DWORD nRet = 0;
void LogOutput(const char* log)
{
	if (isShowWithEditor)
	{
		COPYDATASTRUCT cds;
		cds.dwData = 0;
		cds.lpData = (PVOID)log;
		SendMessage(editorHandle, WM_COPYDATA, 0, (LPARAM)&cds);
	}
	else
	{
		::WriteConsole(g_hConsoleHandle, log, strlen(log), &nRet, NULL);
	}
	InFile << log;
}

void InitEngine(HWND hWnd)
{
	InitializeCriticalSection(&gPhysicsSection);
	MainWindowHwnd = hWnd;

	RECT rect;
	::GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	InFile.open(logFileName.data(), std::ios::out);
	if (!isShowWithEditor)
	{
		::AllocConsole();
		g_hConsoleHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	}
	::SetAppDataPath(strPath.c_str()); 
	::InitilizeEngine(false);	
	// TODO
	::SetMonoPath("../Data/E3DAssembly",strPath + "../Library/AssemblyCSharp.dll", "../Data/E3DAssembly/E3DEngine.dll");
	::SetDebugLogOutFunc(LogOutput);
	void * renderSystem = CreateGLRenderSystem(hWnd, width, height);
	::SetRenderSystem(renderSystem);
	::StartAppliaction(startScenePath.c_str());
}

std::vector<std::string> Split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result; 
	str += pattern;//��չ�ַ����Է������
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = (int)(pos + pattern.size()) - 1;
		}
	}
	return result;
}

int APIENTRY initWindow(LPTSTR lpCmdLine)
{

#ifdef ONE_INSTANCE //��������ģʽ  	
	HANDLE   hMutex = ::CreateMutex(NULL, TRUE, EXE_NAME);
	if (hMutex != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL, "��������Ѿ�����!", "����", NULL);
			CloseHandle(hMutex);
			return   1;
		}
	}
#endif

	WindowStyle = WS_OVERLAPPEDWINDOW;
	if (strlen(lpCmdLine) != 0)
	{
		std::vector<std::string> cmdLine = Split(lpCmdLine, ",");
		if (cmdLine.size() > 0)
		{
			strPath = cmdLine[0];
		}
		if (cmdLine.size() > 1)
		{
			int handle = atoi(cmdLine[1].c_str());
			editorHandle = (HWND)handle;
			isShowWithEditor = true;
		}
		if (cmdLine.size() > 2)
		{
			startScenePath = cmdLine[2];
		}
		
	}
	else
	{
		strPath = "App_Data/";
	}

	return 0;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	if (initWindow(lpCmdLine) == 1)
	{
		return 1;
	}


 	// TODO:  �ڴ˷��ô��롣
	MSG msg = {};
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_E3DENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_E3DENGINE));

	// ����Ϣѭ��: 
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg); 		
			DispatchMessage(&msg); 
		}
		else
		{
			::LogicUpdate();
		}
	}
	bExit = true;
	CloseHandle(hPhysicsThread);
	DestoryEngine();
	return (int) msg.wParam;
}

unsigned __stdcall PhysicsMain(void*)
{
	while (!bExit)
	{
		EnterCriticalSection(&gPhysicsSection);
		::UpdatePhysics();
		LeaveCriticalSection(&gPhysicsSection);
		Sleep(1);
	}
	return 0;
}

POINT GetMousePosition()
{
	POINT MousePosition = { };
	GetCursorPos(&MousePosition);
	ScreenToClient(MainWindowHwnd, &MousePosition);
	return MousePosition;
}

//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_E3DENGINE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_E3DENGINE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WindowStyle,
	   0, 0, WindowWidth, WindowHeight, NULL, NULL, hInstance, NULL);
   DWORD err = GetLastError();
   if (!hWnd)
   {
      return FALSE;
   }
   
   InitEngine(hWnd);
   hPhysicsThread = (HANDLE)_beginthreadex(NULL, 0, &PhysicsMain, NULL, 0, &threadID);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int particleID = 0;
int particleNameIndex = 0;

void procecssKey(char key)
{
	KeyDown(key);
}

void processLButton()
{
	IsMouseDown = true;
	//LogicMain::GetInstance().EnableSelectParticle(false);
	POINT mousePos = GetMousePosition();
	MouseDown(0, mousePos.x, mousePos.y);
}

void processMouseMove()
{
	POINT mousePos = GetMousePosition();
	MouseMove(mousePos.x, mousePos.y);
}

void processMouseWheel(WPARAM wParam, LPARAM lParam)
{
	//WORD fwKeys = LOWORD(wParam);
	//int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);/*   wheel   rotation   */
	////short xPos = (short)LOWORD(lParam); /*   horizontal   position   of   pointer   */
	////short yPos = (short)HIWORD(lParam);	/*   vertical   position   of   pointer   */
	//vec3f curPos = GetCamera(MainCameraName)->GetPosition() ;
	//vec3f dir = GetCamera(MainCameraName)->GetForwardVector() * (zDelta / abs(zDelta)) * 2;
	//dir.normalize();
	//vec3f newPos = curPos + dir;
	//GetCamera(MainCameraName)->SetPosition(newPos);
}

void SizeChange()
{
	RECT rect;
	::GetClientRect(MainWindowHwnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	::ChageFrameSize(width, height);
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		{
			//LogicMain::GetInstance().ApplicationShutdown();
			PostQuitMessage(0);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			processLButton();

		}
		break;
	case WM_MOUSEMOVE:
		{
			processMouseMove();
		}
		break;
	case WM_LBUTTONUP:
		{
			IsMouseDown = false;

			POINT curPos = GetMousePosition();
			MouseUp(0, curPos.x, curPos.y);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			IsRButtonDown = true;
			LastMousePoint = GetMousePosition();
			MouseDown(1, LastMousePoint.x, LastMousePoint.y);
		}
		break;
	case WM_RBUTTONUP:
		{
			IsRButtonDown = false;
			POINT curPos = GetMousePosition();
			MouseUp(1, curPos.x, curPos.y);
		}
		break;
	case WM_CHAR:
		{
			char key = wParam;
			procecssKey(key);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			//processMouseWheel(wParam, lParam);
		}
		break;
	case WM_MBUTTONDOWN:
		{
			MButtonDown = true;
			LastMousePoint = GetMousePosition();
			MouseDown(2, LastMousePoint.x, LastMousePoint.y);
		}
		break;
	case WM_MBUTTONUP:
		{
			MButtonDown = false;
			POINT curPos = GetMousePosition();
			MouseUp(2, curPos.x, curPos.y);
		}
		break;
	case WM_SIZE:
		{
			SizeChange();
		}
		break;
	case WM_COPYDATA:
		{
			COPYDATASTRUCT *data = (COPYDATASTRUCT*)lParam;
			if (data->dwData == 1)
			{
				if (data->cbData)
				{
					::PauseEngine(true);
				}
				else
				{
					::PauseEngine(false);
				}
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

