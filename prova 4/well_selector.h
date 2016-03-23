#pragma once
#include<Windows.h>

enum printing_mode;

namespace prova4 {
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class well_selector : public System::Windows::Forms::Form
	{
	public:
		well_selector(void)
		{
			InitializeComponent();
			ws_picbox->BackColor = Color::White;
		}
	protected:
		/// <summary>
		/// Liberare le risorse in uso.
		/// </summary>
		~well_selector()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  ws_picbox;
	protected: 

	private:
		array<array<bool>^>^ mw_flag;
		array<int>^ row_mask;
		int nrow;
		int ncol;
		int dx;
		int dy;
		int row_sel;
		int col_sel;

	private: System::Windows::Forms::Button^  ws_ok;
	private: System::Windows::Forms::Button^  ws_cancel;

	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metodo necessario per il supporto della finestra di progettazione. Non modificare
		/// il contenuto del metodo con l'editor di codice.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ws_picbox = (gcnew System::Windows::Forms::PictureBox());
			this->ws_ok = (gcnew System::Windows::Forms::Button());
			this->ws_cancel = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ws_picbox))->BeginInit();
			this->SuspendLayout();
			// 
			// ws_picbox
			// 
			this->ws_picbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ws_picbox->Location = System::Drawing::Point(12, 12);
			this->ws_picbox->Name = L"ws_picbox";
			this->ws_picbox->Size = System::Drawing::Size(492, 337);
			this->ws_picbox->TabIndex = 0;
			this->ws_picbox->TabStop = false;
			this->ws_picbox->ClientSizeChanged += gcnew System::EventHandler(this, &well_selector::ws_picbox_ClientSizeChanged);
			this->ws_picbox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &well_selector::ws_picbox_Paint);
			this->ws_picbox->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &well_selector::ws_picbox_MouseClick);
			// 
			// ws_ok
			// 
			this->ws_ok->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->ws_ok->Location = System::Drawing::Point(12, 355);
			this->ws_ok->Name = L"ws_ok";
			this->ws_ok->Size = System::Drawing::Size(171, 60);
			this->ws_ok->TabIndex = 1;
			this->ws_ok->Text = L"Ok";
			this->ws_ok->UseVisualStyleBackColor = true;
			this->ws_ok->Click += gcnew System::EventHandler(this, &well_selector::ws_ok_Click);
			// 
			// ws_cancel
			// 
			this->ws_cancel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->ws_cancel->Location = System::Drawing::Point(333, 355);
			this->ws_cancel->Name = L"ws_cancel";
			this->ws_cancel->Size = System::Drawing::Size(171, 60);
			this->ws_cancel->TabIndex = 2;
			this->ws_cancel->Text = L"Cancel";
			this->ws_cancel->UseVisualStyleBackColor = true;
			this->ws_cancel->Click += gcnew System::EventHandler(this, &well_selector::ws_cancel_Click);
			// 
			// well_selector
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(516, 427);
			this->Controls->Add(this->ws_cancel);
			this->Controls->Add(this->ws_ok);
			this->Controls->Add(this->ws_picbox);
			this->Name = L"well_selector";
			this->Text = L"Well Selector";
			this->Shown += gcnew System::EventHandler(this, &well_selector::well_selector_Shown);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ws_picbox))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion


	public: System::Void send_data(array<array<bool>^>^ flag, printing_mode ptw, array<int>^ rm){
				mw_flag = flag;
				row_mask = rm;
				
				switch(ptw){
					case 2:
						nrow = 2;
						ncol = 3;
						break;
					case 3:
						nrow = 4;
						ncol = 6;
						break;
					case 4:
						nrow = 8;
						ncol = 12;
						break;
					default:
						return;
				}
				
				dy = ws_picbox->Height/nrow;
				dx = ws_picbox->Width/ncol;

				row_sel = -1;
				//flg = true;
				return;
			}
	public: array<array<bool>^>^ get_data(){ return mw_flag; }
	
	private: bool flag_check(int n){
				 for(int i=0; i<row_mask->Length; i++)
					 if(row_mask[i] == n)
						 return true;
				 return false;
			 }

	private: System::Void init_grid(){
				 for(int i=0; i<row_mask->Length; i++){
					 row_sel = row_mask[i];
					 for(col_sel=0; col_sel<ncol; col_sel++){
						 fill_rect();
					 }
				 }
				 draw_grid();
				 row_sel = -1;
			 }


	private: System::Void ws_ok_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = Windows::Forms::DialogResult::OK;
				 well_selector::Close();
			 }
	private: System::Void ws_cancel_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = Windows::Forms::DialogResult::Cancel;
				 well_selector::Close();
			 }


	private: System::Void ws_picbox_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if( e->Button == System::Windows::Forms::MouseButtons::Right ) return;
				 row_sel = e->Y/dy;
				 col_sel = e->X/dx;
				 if(!flag_check(row_sel)){
					 row_sel = -1;
					 return;
				 }
				 mw_flag[row_sel][col_sel] = !mw_flag[row_sel][col_sel];
				 InvokePaint(ws_picbox,nullptr);
			 }


	private: System::Void ws_picbox_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 //if(flg) return;
				 if(row_sel != -1)
					 fill_rect();
				 row_sel = -1;
				 draw_grid();
			 }

	private: System::Void draw_grid(){
				 Graphics^ g = ws_picbox->CreateGraphics();
				 
				 Pen^ myPen = gcnew Pen(Color::Black);
				 myPen->Width = 2;
				 
				 for(int i=1; i<nrow; i++)
					 g->DrawLine(myPen, 0, i*dy, ws_picbox->Width, i*dy);
				 for(int j=1; j<ncol; j++)
					 g->DrawLine(myPen, j*dx, 0, j*dx, ws_picbox->Height);
			 }
	private: System::Void fill_rect(){
				 Graphics^ g = ws_picbox->CreateGraphics();
				 SolidBrush^ myBrush;
				 if(mw_flag[row_sel][col_sel])
					 myBrush = gcnew SolidBrush(Color::Green);
				 else
					 myBrush = gcnew SolidBrush(Color::Red);
				 g->FillRectangle(myBrush, RectangleF(col_sel*dx, row_sel*dy, dx, dy));
				 write_txt();

			 }
	private: System::Void write_txt(){
				 Graphics^ g = ws_picbox->CreateGraphics();
				 SolidBrush^ myBrush = gcnew SolidBrush(Color::Black);
				 System::Drawing::Font^ myFont = gcnew System::Drawing::Font("Times New Roman", 14);
				 wchar_t chr = 'A' + row_sel; 
				 g->DrawString(String::Concat(chr,col_sel+1), myFont, myBrush, RectangleF(col_sel*dx, row_sel*dy, dx, dy));
			 }

private: System::Void well_selector_Shown(System::Object^  sender, System::EventArgs^  e) {
			 //DoEvents();
			 //InvokePaint(ws_picbox,nullptr);
			 
			 this->ws_picbox->MouseEnter += gcnew System::EventHandler(this, &well_selector::ws_picbox_MouseEnter);
		 }

//private: void DoEvents(){
//			MSG	Msg;
//			BOOL bRet;
//			HWND m_hWnd;
//			//HICON
//			//analog DoEvents() v VB
//			if (PeekMessage(&Msg,m_hWnd,0,0,PM_NOREMOVE )){
//					if ((bRet=GetMessage(&Msg,m_hWnd,0,0)))
//						DispatchMessage(&Msg);
//			}
//		 }

private: System::Void ws_picbox_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
			 init_grid();
			 this->ws_picbox->MouseEnter -= gcnew System::EventHandler(this, &well_selector::ws_picbox_MouseEnter);
		 }

private: System::Void ws_picbox_ClientSizeChanged(System::Object^  sender, System::EventArgs^  e) {
			 dy = ws_picbox->Height/nrow;
			 dx = ws_picbox->Width/ncol;
			 ws_picbox->Refresh();
			 init_grid();
		 }
};
}
