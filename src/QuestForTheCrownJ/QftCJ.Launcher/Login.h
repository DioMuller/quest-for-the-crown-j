#pragma once
#include <string>

namespace QftCJLauncher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Login
	/// </summary>
	public ref class Login : public System::Windows::Forms::Form
	{
	public: System::String^ AuthCode;

	public:
		Login(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Login()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  ButtonLogin;
	private: System::Windows::Forms::TextBox^  tbUser;


	private: System::Windows::Forms::Label^  lblUser;
	private: System::Windows::Forms::Label^  lblPassword;
	private: System::Windows::Forms::TextBox^  tbPassword;


	private: System::Windows::Forms::Label^  lblStatus;

	private: System::Windows::Forms::Label^  lblServerAddress;
	public: System::Windows::Forms::TextBox^  tbServerAddress;
	private:

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

	private: bool NetLogin(std::string, std::string);

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ButtonLogin = (gcnew System::Windows::Forms::Button());
			this->tbUser = (gcnew System::Windows::Forms::TextBox());
			this->lblUser = (gcnew System::Windows::Forms::Label());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->tbPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->lblServerAddress = (gcnew System::Windows::Forms::Label());
			this->tbServerAddress = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// ButtonLogin
			// 
			this->ButtonLogin->Location = System::Drawing::Point(197, 90);
			this->ButtonLogin->Name = L"ButtonLogin";
			this->ButtonLogin->Size = System::Drawing::Size(75, 23);
			this->ButtonLogin->TabIndex = 0;
			this->ButtonLogin->Text = L"Login";
			this->ButtonLogin->UseVisualStyleBackColor = true;
			this->ButtonLogin->Click += gcnew System::EventHandler(this, &Login::ButtonLogin_Click);
			// 
			// tbUser
			// 
			this->tbUser->Location = System::Drawing::Point(71, 12);
			this->tbUser->MaxLength = 40;
			this->tbUser->Name = L"tbUser";
			this->tbUser->Size = System::Drawing::Size(201, 20);
			this->tbUser->TabIndex = 1;
			// 
			// lblUser
			// 
			this->lblUser->AutoSize = true;
			this->lblUser->Location = System::Drawing::Point(12, 15);
			this->lblUser->Name = L"lblUser";
			this->lblUser->Size = System::Drawing::Size(29, 13);
			this->lblUser->TabIndex = 2;
			this->lblUser->Text = L"User";
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Location = System::Drawing::Point(12, 41);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(53, 13);
			this->lblPassword->TabIndex = 3;
			this->lblPassword->Text = L"Password";
			// 
			// tbPassword
			// 
			this->tbPassword->Location = System::Drawing::Point(71, 38);
			this->tbPassword->Name = L"tbPassword";
			this->tbPassword->PasswordChar = '*';
			this->tbPassword->Size = System::Drawing::Size(201, 20);
			this->tbPassword->TabIndex = 4;
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->lblStatus->Location = System::Drawing::Point(12, 95);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 13);
			this->lblStatus->TabIndex = 5;
			// 
			// lblServerAddress
			// 
			this->lblServerAddress->AutoSize = true;
			this->lblServerAddress->Location = System::Drawing::Point(12, 67);
			this->lblServerAddress->Name = L"lblServerAddress";
			this->lblServerAddress->Size = System::Drawing::Size(51, 13);
			this->lblServerAddress->TabIndex = 7;
			this->lblServerAddress->Text = L"Server IP";
			// 
			// tbServerAddress
			// 
			this->tbServerAddress->Location = System::Drawing::Point(71, 64);
			this->tbServerAddress->Name = L"tbServerAddress";
			this->tbServerAddress->Size = System::Drawing::Size(201, 20);
			this->tbServerAddress->TabIndex = 8;
			this->tbServerAddress->Text = L"127.0.0.1";
			// 
			// Login
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 125);
			this->Controls->Add(this->tbServerAddress);
			this->Controls->Add(this->lblServerAddress);
			this->Controls->Add(this->lblStatus);
			this->Controls->Add(this->tbPassword);
			this->Controls->Add(this->lblPassword);
			this->Controls->Add(this->lblUser);
			this->Controls->Add(this->tbUser);
			this->Controls->Add(this->ButtonLogin);
			this->Name = L"Login";
			this->Text = L"QFTC - Login";
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		private: 
			System::Void ButtonLogin_Click(System::Object^  sender, System::EventArgs^  e);
#pragma endregion
	};
}
