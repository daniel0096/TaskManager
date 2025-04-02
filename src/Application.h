#pragma once

class CApplication
{
public:
	bool OnInit();
	void Run();
	void OnUpdate();
	void Stop();  // Add this

#if !defined(_DEBUG) && defined(_WIN32)
	void HideDebugBuildConsole();
#endif

	bool IsRunning() const { return m_isRunning; }

private:
	bool m_isRunning;
};

extern CApplication* g_appInstance;
