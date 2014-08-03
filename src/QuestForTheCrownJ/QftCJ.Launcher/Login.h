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
	private: System::Windows::Forms::GroupBox^  groupBox1;
	protected:
	private: System::Windows::Forms::Button^  ButtonExit;
	public: System::Windows::Forms::TextBox^  tbServerAddress;
	private:
	private: System::Windows::Forms::Label^  lblServerAddress;
	public:
	private: System::Windows::Forms::Label^  lblStatus;
	private: System::Windows::Forms::TextBox^  tbPassword;
	private: System::Windows::Forms::Label^  lblPassword;
	private: System::Windows::Forms::Label^  lblUser;
	private: System::Windows::Forms::TextBox^  tbUser;
	private: System::Windows::Forms::Button^  ButtonLogin;














	public:
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Login::typeid));
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->ButtonExit = (gcnew System::Windows::Forms::Button());
			this->tbServerAddress = (gcnew System::Windows::Forms::TextBox());
			this->lblServerAddress = (gcnew System::Windows::Forms::Label());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->tbPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->lblUser = (gcnew System::Windows::Forms::Label());
			this->tbUser = (gcnew System::Windows::Forms::TextBox());
			this->ButtonLogin = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::Transparent;
			this->groupBox1->Controls->Add(this->ButtonExit);
			this->groupBox1->Controls->Add(this->tbServerAddress);
			this->groupBox1->Controls->Add(this->lblServerAddress);
			this->groupBox1->Controls->Add(this->lblStatus);
			this->groupBox1->Controls->Add(this->tbPassword);
			this->groupBox1->Controls->Add(this->lblPassword);
			this->groupBox1->Controls->Add(this->lblUser);
			this->groupBox1->Controls->Add(this->tbUser);
			this->groupBox1->Controls->Add(this->ButtonLogin);
			this->groupBox1->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->groupBox1->Location = System::Drawing::Point(173, 103);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(279, 131);
			this->groupBox1->TabIndex = 10;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Login";
			// 
			// ButtonExit
			// 
			this->ButtonExit->BackColor = System::Drawing::Color::Green;
			this->ButtonExit->Location = System::Drawing::Point(193, 100);
			this->ButtonExit->Name = L"ButtonExit";
			this->ButtonExit->Size = System::Drawing::Size(75, 23);
			this->ButtonExit->TabIndex = 18;
			this->ButtonExit->Text = L"Quit";
			this->ButtonExit->UseVisualStyleBackColor = false;
			this->ButtonExit->Click += gcnew System::EventHandler(this, &Login::ButtonExit_Click);
			// 
			// tbServerAddress
			// 
			this->tbServerAddress->Location = System::Drawing::Point(67, 74);
			this->tbServerAddress->Name = L"tbServerAddress";
			this->tbServerAddress->Size = System::Drawing::Size(201, 20);
			this->tbServerAddress->TabIndex = 17;
			this->tbServerAddress->Text = L"127.0.0.1";
			// 
			// lblServerAddress
			// 
			this->lblServerAddress->AutoSize = true;
			this->lblServerAddress->Location = System::Drawing::Point(8, 77);
			this->lblServerAddress->Name = L"lblServerAddress";
			this->lblServerAddress->Size = System::Drawing::Size(51, 13);
			this->lblServerAddress->TabIndex = 16;
			this->lblServerAddress->Text = L"Server IP";
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->lblStatus->Location = System::Drawing::Point(13, 103);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 13);
			this->lblStatus->TabIndex = 15;
			// 
			// tbPassword
			// 
			this->tbPassword->Location = System::Drawing::Point(67, 48);
			this->tbPassword->Name = L"tbPassword";
			this->tbPassword->PasswordChar = '*';
			this->tbPassword->Size = System::Drawing::Size(201, 20);
			this->tbPassword->TabIndex = 14;
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Location = System::Drawing::Point(8, 51);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(53, 13);
			this->lblPassword->TabIndex = 13;
			this->lblPassword->Text = L"Password";
			// 
			// lblUser
			// 
			this->lblUser->AutoSize = true;
			this->lblUser->Location = System::Drawing::Point(8, 25);
			this->lblUser->Name = L"lblUser";
			this->lblUser->Size = System::Drawing::Size(29, 13);
			this->lblUser->TabIndex = 12;
			this->lblUser->Text = L"User";
			// 
			// tbUser
			// 
			this->tbUser->Location = System::Drawing::Point(67, 22);
			this->tbUser->MaxLength = 40;
			this->tbUser->Name = L"tbUser";
			this->tbUser->Size = System::Drawing::Size(201, 20);
			this->tbUser->TabIndex = 11;
			// 
			// ButtonLogin
			// 
			this->ButtonLogin->BackColor = System::Drawing::Color::Green;
			this->ButtonLogin->Location = System::Drawing::Point(112, 100);
			this->ButtonLogin->Name = L"ButtonLogin";
			this->ButtonLogin->Size = System::Drawing::Size(75, 23);
			this->ButtonLogin->TabIndex = 10;
			this->ButtonLogin->Text = L"Login";
			this->ButtonLogin->UseVisualStyleBackColor = false;
			this->ButtonLogin->Click += gcnew System::EventHandler(this, &Login::ButtonLogin_Click);
			// 
			// Login
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(464, 241);
			this->Controls->Add(this->groupBox1);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(480, 280);
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(480, 280);
			this->Name = L"Login";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Quest for the Crown J Online - Login";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}

		private: 
			System::Void ButtonLogin_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void ButtonExit_Click(System::Object^  sender, System::EventArgs^  e);
#pragma endregion
	
};
}
