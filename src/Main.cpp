#include "Application.h"

CApplication* g_appInstance = nullptr;

int main()
{
	CApplication application;
	g_appInstance = &application;

	application.Run();
}
