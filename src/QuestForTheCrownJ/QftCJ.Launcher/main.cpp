#include "Login.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	QftCJLauncher::Login form;
	if (form.ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		//TODO: Launch game, form.AuthCode form.tbServerAddress->Text
		form.Close();
	}
}