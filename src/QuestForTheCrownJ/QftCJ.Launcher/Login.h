#pragma once

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
	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ButtonLogin = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// ButtonLogin
			// 
			this->ButtonLogin->Location = System::Drawing::Point(12, 196);
			this->ButtonLogin->Name = L"ButtonLogin";
			this->ButtonLogin->Size = System::Drawing::Size(75, 23);
			this->ButtonLogin->TabIndex = 0;
			this->ButtonLogin->Text = L"button1";
			this->ButtonLogin->UseVisualStyleBackColor = true;
			this->ButtonLogin->Click += gcnew System::EventHandler(this, &Login::ButtonLogin_Click);
			// 
			// Login
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->ButtonLogin);
			this->Name = L"Login";
			this->Text = L"Login";
			this->ResumeLayout(false);

		}

		private: System::Void ButtonLogin_Click(System::Object^  sender, System::EventArgs^  e);
#pragma endregion
	};
}
