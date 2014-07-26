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
		IntPtr authCodeNativePtr = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(form.AuthCode);

		std::string auth_code = static_cast<char*>(authCodeNativePtr.ToPointer());
		system(("qftcj-game.exe " + auth_code).c_str());
		form.Close();
	}
}