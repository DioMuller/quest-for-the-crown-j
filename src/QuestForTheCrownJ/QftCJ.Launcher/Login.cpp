#include "Login.h"
#include "AuthStructs.h"
#include <WinSock2.h>
#include <msclr/marshal_cppstd.h>
#include "ClientChannel.h"
using namespace System::Runtime::InteropServices;

bool QftCJLauncher::Login::NetLogin(std::string userName, std::string hashedPassword)
{
	IntPtr ipNativePtr = Marshal::StringToHGlobalAnsi(tbServerAddress->Text);
	char* ipAddress = static_cast<char*>(ipNativePtr.ToPointer());

	try
	{
		qfcnet::ClientChannel channel = qfcnet::ClientChannel(ipAddress, 12345);
		channel.Login(userName, hashedPassword);
		AuthCode = gcnew System::String(channel.auth_token.c_str());
	}
	catch (const std::exception& ex)
	{
		lblStatus->Text = gcnew System::String(ex.what());
	}

	
	return true;
}

System::String^ sha256(System::String^ password)
{
	auto crypt = gcnew System::Security::Cryptography::SHA256Managed();
	auto hash = System::String::Empty;
	auto crypto = crypt->ComputeHash(System::Text::Encoding::ASCII->GetBytes(password), 0, System::Text::Encoding::ASCII->GetByteCount(password));
	for each(auto bit in crypto)
	{
		hash += bit.ToString("x2");
	}
	return hash;
}

System::Void QftCJLauncher::Login::ButtonLogin_Click(System::Object^ sender, System::EventArgs^ e)
{
	IntPtr userNativePtr = Marshal::StringToHGlobalAnsi(tbUser->Text);
	System::String^ passwordHashStr = sha256(tbPassword->Text);
	IntPtr passwordNativePtr = Marshal::StringToHGlobalAnsi(passwordHashStr);

	char* user = static_cast<char*>(userNativePtr.ToPointer());
	char* passwordHash = static_cast<char*>(passwordNativePtr.ToPointer());

	if (NetLogin(user, passwordHash))
		DialogResult = System::Windows::Forms::DialogResult::OK;
}