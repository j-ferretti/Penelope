// prova 4.cpp : file di progetto principale.

#include "stdafx.h"
#include "Main_window.h"

using namespace prova4;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Attivare gli effetti visivi di Windows XP prima di creare i controlli
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Creare la finestra principale ed eseguirla
	Application::Run(gcnew Main_window());
	return 0;
}
