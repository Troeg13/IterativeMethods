#include "IterMet.h" 
#include <Windows.h> 
using namespace IterativeMethods; // Название проекта 

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	IterativeMethods::IterMet form;
	Application::Run(% form);
	return 0;
}
