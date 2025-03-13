#pragma once

class CApplication
{
public:
	bool OnInit();
	void Run();
	void OnUpdate();
private:
	bool m_isRunning;
};