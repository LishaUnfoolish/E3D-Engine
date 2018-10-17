// EditorDll.h

#pragma once

#include <EngineAPI.h>
using namespace System;

#pragma  managed
namespace E3DEngine 
{
	public ref class EngineDelegateRef
	{
	public: 
		void InitilizeEngine();
		void StartAppliaction();
		void StopAppliaction();
		void SetupRenderSystem(IntPtr nativeWindow, int width, int height);
		void SetDebugLogOutFunc(IntPtr func);
		void SetAppDataPath(String^ path);
		void EngineUpdate();
		void DestoryEngine();
		void PauseEngine(bool bPause);
		void ChageFrameSize(int w, int h);
		void MouseDown(int mouseButtonID, float xPos, float yPos);
		void MouseUp(int mouseButtonID, float xPos, float yPos);
		void MouseMove(float xPos, float yPos);
		void KeyDown(char key);
		void KeyUp(char key);
		void CreateEditor();
		void ChangeSurface(IntPtr nativeWindow);
		void RunCurrentScene(bool run);
	};
}
#pragma unmanaged