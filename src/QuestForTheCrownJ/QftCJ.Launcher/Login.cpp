#include "Login.h"
#include "AuthStructs.h"
#include <WinSock2.h>
#include <msclr/marshal_cppstd.h>

using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

bool QftCJLauncher::Login::NetLogin(std::string userName, std::string hashedPassword)
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		lblStatus->Text = gcnew System::String("Não foi possível encontrar uma dll WinSock que possa ser utilizada.");
		WSACleanup();
		return false;
	}

	SOCKET mainSocket;
	mainSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mainSocket == INVALID_SOCKET) {
		lblStatus->Text = gcnew System::String("Erro ao criar socket: ") + WSAGetLastError();
		WSACleanup();
		return false;
	}

	sockaddr_in serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));

	IntPtr ipNativePtr = Marshal::StringToHGlobalAnsi(tbServerAddress->Text);
	char* ipAddress = static_cast<char*>(ipNativePtr.ToPointer());

	serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(12345);
	int server_size = sizeof(serverAddress);

	s_launcher_login_info login;
	login.header.type = PacketType::LAUNCHER_LOGIN_INFO;
	strcpy(login.login, userName.c_str());
	strcpy(login.hashedPassword, hashedPassword.c_str());

	r = sendto(mainSocket, (char*)&login, sizeof(login), 0, (SOCKADDR*)&serverAddress, server_size);
	if (r <= 0)
	{
		lblStatus->Text = gcnew System::String("Send error");
		return false;
	}

	s_launcher_login_response response;
	ZeroMemory(&response, sizeof(response));

	r = recv(mainSocket, (char*)&response, sizeof(response), 0);

	if (r <= 0)
	{
		lblStatus->Text = gcnew System::String("Receive error");
		return false;
	}

	if (r != sizeof(response))
	{
		lblStatus->Text = gcnew System::String("Invalid response");
		return false;
	}

	if (!response.authenticated)
	{
		lblStatus->Text = gcnew System::String("Invalid username or password");
		return false;
	}
	AuthCode = gcnew System::String(response.authKey);
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

System::Void QftCJLauncher::Login::ButtonExit_Click(System::Object^  sender, System::EventArgs^  e)
{
	Application::Exit();
}