#pragma once

class CApplication
{
public:
	bool OnInit();
	void Run();
	void OnUpdate();
#if !defined(_DEBUG) && defined(_WIN32)
	void HideDebugBuildConsole();
#endif
private:
	bool m_isRunning;
};