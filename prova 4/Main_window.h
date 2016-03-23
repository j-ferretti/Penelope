//define
#define wid 150			// 3D mode Width
#define hei 185			// 3D mode Height
#define SF 1.233		// scale factor = hei/wid
#define mw_6_wid 80		// Multiwell 6 width
#define mw_6_hei 80		// Height
#define mw_24_wid 38	// Multiwell 24 width
#define mw_24_hei 38	// Height
#define mw_96_wid 10	// Multiwell 96 width
#define mw_96_hei 10	// Height

#define ox 330			// Coordinates of 3d print
#define oy 0
//#define oy 30
#define mw_6_ox 214		// Coordinates of MW6 print
#define mw_6_oy 12		//
#define mw_24_ox 206	// Coordinates of MW24 print
#define mw_24_oy 72		//
#define mw_96_ox 210	// Coordinates of MW96 print
#define mw_96_oy 32		//
#define mw_6_d 154
#define mw_24_d 78
#define mw_96_d 36
#define y_slide_i 41	// Coordinates of slide
#define y_slide_f 141	//
#define width_slide 200
#define slide_SF 0.5

#define max_image 100	// Max images

enum printing_mode { print_in_3d = 0, print_on_slide, multiwell_6, multiwell_24, multiwell_96 };

public value struct print_options{
	printing_mode p_mode;
	int Cd;
	bool eject_on_finish;
};

#pragma once
#include <Windows.h>
#include <phidget21.h>
#include "Phidget_class.h"
#include "Image_class.h"
#include "well_selector.h"


namespace prova4 {

	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Printing;

	public ref class Main_window : public System::Windows::Forms::Form
	{
	public:
		Main_window(void){
			InitializeComponent();
			init();
			initbgw();
		}

	protected:
		~Main_window(){
			if(!pc->switch_888){ // Get back the drawer
				//button10_Click(this,nullptr); // it doesn't work if the printer is off
				Sleep(1000);
			}
			if (components){
				delete components;
			}
		}

	private:
		
		Image^ printing_image;		// Image for printing backgroundworker/main form comunication
		bool b7;					// Flag for Print/Cancel Print button
		static int k;				// Printing backgroundworker current image index
		double za;					// Servo starting position
		double z1,z2,z3;			// Servo positions
		bool ez;					// Flag for servo movement
		int p;						// Image class index
		Immagini^ IC;				// Image class handler
		phidclass *pc;				// Phidget class
		bool traslate;				// Flag for image crop
		int starting_x, starting_y;	// Positioning for image crop
		String^ printer_name;		// Printer name
		print_options^ p_opt;		// Print options struct
		well_selector^ ws_f;		// Well selector class handler
		array<int>^ mw_rowmask;		// Multiwell Mask
		float wh_ratio;				// Picturebox Width/Height ratio
		Image^ imag;				// Image handler, needed to avoid reloading it from file everytime picturebox refresh in case of image crop

		//form handler
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::PictureBox^  pictureBox;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::TrackBar^  trackBar2;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::TrackBar^  trackBar5;
	private: System::Windows::Forms::TrackBar^  trackBar4;
	private: System::Windows::Forms::TrackBar^  trackBar3;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStrip_Adatta;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStrip_Ritaglia;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStrip_Dimensioni;



	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  button11;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button12;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Button^  ws;
	private: System::Windows::Forms::Label^  printing_label;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStrip_Centra;






	private: System::Drawing::Printing::PrintDocument^  printDocument;

		//form designer
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metodo necessario per il supporto della finestra di progettazione. Non modificare
		/// il contenuto del metodo con l'editor di codice.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Main_window::typeid));
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->printDocument = (gcnew System::Drawing::Printing::PrintDocument());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->ws = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->trackBar5 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar4 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar3 = (gcnew System::Windows::Forms::TrackBar());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->printing_label = (gcnew System::Windows::Forms::Label());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->ToolStrip_Centra = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStrip_Adatta = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStrip_Ritaglia = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStrip_Dimensioni = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox))->BeginInit();
			this->contextMenuStrip1->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog1";
			// 
			// tabPage2
			// 
			this->tabPage2->BackColor = System::Drawing::SystemColors::Control;
			this->tabPage2->Controls->Add(this->ws);
			this->tabPage2->Controls->Add(this->label2);
			this->tabPage2->Controls->Add(this->comboBox2);
			this->tabPage2->Controls->Add(this->checkBox4);
			this->tabPage2->Controls->Add(this->label1);
			this->tabPage2->Controls->Add(this->button11);
			this->tabPage2->Controls->Add(this->comboBox1);
			this->tabPage2->Controls->Add(this->textBox4);
			this->tabPage2->Controls->Add(this->textBox3);
			this->tabPage2->Controls->Add(this->textBox2);
			this->tabPage2->Controls->Add(this->textBox1);
			this->tabPage2->Controls->Add(this->trackBar5);
			this->tabPage2->Controls->Add(this->trackBar4);
			this->tabPage2->Controls->Add(this->trackBar3);
			this->tabPage2->Controls->Add(this->label7);
			this->tabPage2->Controls->Add(this->label6);
			this->tabPage2->Controls->Add(this->numericUpDown3);
			this->tabPage2->Controls->Add(this->numericUpDown2);
			this->tabPage2->Controls->Add(this->button9);
			this->tabPage2->Controls->Add(this->trackBar2);
			this->tabPage2->Controls->Add(this->button8);
			this->tabPage2->Controls->Add(this->numericUpDown1);
			this->tabPage2->Controls->Add(this->checkBox2);
			this->tabPage2->Controls->Add(this->checkBox1);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(956, 563);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Impostazioni avanzate";
			// 
			// ws
			// 
			this->ws->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->ws->Enabled = false;
			this->ws->Location = System::Drawing::Point(681, 84);
			this->ws->Name = L"ws";
			this->ws->Size = System::Drawing::Size(92, 38);
			this->ws->TabIndex = 25;
			this->ws->Text = L"Selezione Well";
			this->ws->UseVisualStyleBackColor = true;
			this->ws->Click += gcnew System::EventHandler(this, &Main_window::ws_Click);
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(678, 53);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(95, 13);
			this->label2->TabIndex = 24;
			this->label2->Text = L"Modalità di stampa";
			// 
			// comboBox2
			// 
			this->comboBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
				L"3D Print", L"Print on slide", L"Multiwell (6)",
					L"Multiwell (24)", L"Multiwell (96)"
			});
			this->comboBox2->Location = System::Drawing::Point(779, 50);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(139, 21);
			this->comboBox2->TabIndex = 23;
			this->comboBox2->Text = L"3D Print";
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &Main_window::comboBox2_SelectedIndexChanged);
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Location = System::Drawing::Point(6, 73);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(165, 17);
			this->checkBox4->TabIndex = 22;
			this->checkBox4->Text = L"Espelli cassetto a fine stampa";
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &Main_window::checkBox4_CheckedChanged);
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(693, 10);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 13);
			this->label1->TabIndex = 20;
			this->label1->Text = L"Coda di stampa";
			// 
			// button11
			// 
			this->button11->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button11->Location = System::Drawing::Point(924, 7);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(33, 21);
			this->button11->TabIndex = 19;
			this->button11->Text = L"Up";
			this->button11->UseVisualStyleBackColor = true;
			this->button11->Click += gcnew System::EventHandler(this, &Main_window::button11_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(779, 7);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(139, 21);
			this->comboBox1->TabIndex = 18;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Main_window::comboBox1_SelectedIndexChanged);
			// 
			// textBox4
			// 
			this->textBox4->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->textBox4->Enabled = false;
			this->textBox4->Location = System::Drawing::Point(835, 228);
			this->textBox4->MaxLength = 4;
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(43, 20);
			this->textBox4->TabIndex = 17;
			this->textBox4->Text = L"21000";
			this->textBox4->TextChanged += gcnew System::EventHandler(this, &Main_window::textBox4_TextChanged);
			// 
			// textBox3
			// 
			this->textBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->textBox3->Enabled = false;
			this->textBox3->Location = System::Drawing::Point(298, 309);
			this->textBox3->MaxLength = 4;
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(45, 20);
			this->textBox3->TabIndex = 16;
			this->textBox3->Text = L"0";
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &Main_window::textBox3_TextChanged);
			// 
			// textBox2
			// 
			this->textBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->textBox2->Enabled = false;
			this->textBox2->Location = System::Drawing::Point(179, 309);
			this->textBox2->MaxLength = 4;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(45, 20);
			this->textBox2->TabIndex = 15;
			this->textBox2->Text = L"0";
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &Main_window::textBox2_TextChanged);
			// 
			// textBox1
			// 
			this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->textBox1->Enabled = false;
			this->textBox1->Location = System::Drawing::Point(65, 309);
			this->textBox1->MaxLength = 4;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(45, 20);
			this->textBox1->TabIndex = 14;
			this->textBox1->Text = L"0";
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Main_window::textBox1_TextChanged);
			// 
			// trackBar5
			// 
			this->trackBar5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->trackBar5->Enabled = false;
			this->trackBar5->Location = System::Drawing::Point(298, 335);
			this->trackBar5->Maximum = 2000;
			this->trackBar5->Name = L"trackBar5";
			this->trackBar5->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->trackBar5->Size = System::Drawing::Size(45, 161);
			this->trackBar5->TabIndex = 13;
			this->trackBar5->TickFrequency = 200;
			this->trackBar5->ValueChanged += gcnew System::EventHandler(this, &Main_window::trackBar5_ValueChanged);
			// 
			// trackBar4
			// 
			this->trackBar4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->trackBar4->Enabled = false;
			this->trackBar4->Location = System::Drawing::Point(179, 335);
			this->trackBar4->Maximum = 2000;
			this->trackBar4->Name = L"trackBar4";
			this->trackBar4->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->trackBar4->Size = System::Drawing::Size(45, 161);
			this->trackBar4->TabIndex = 12;
			this->trackBar4->TickFrequency = 200;
			this->trackBar4->ValueChanged += gcnew System::EventHandler(this, &Main_window::trackBar4_ValueChanged);
			// 
			// trackBar3
			// 
			this->trackBar3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->trackBar3->Enabled = false;
			this->trackBar3->Location = System::Drawing::Point(65, 335);
			this->trackBar3->Maximum = 2000;
			this->trackBar3->Name = L"trackBar3";
			this->trackBar3->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->trackBar3->Size = System::Drawing::Size(45, 161);
			this->trackBar3->TabIndex = 11;
			this->trackBar3->TickFrequency = 200;
			this->trackBar3->ValueChanged += gcnew System::EventHandler(this, &Main_window::trackBar3_ValueChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Enabled = false;
			this->label7->Location = System::Drawing::Point(3, 169);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(46, 13);
			this->label7->TabIndex = 9;
			this->label7->Text = L"Set Max";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Enabled = false;
			this->label6->Location = System::Drawing::Point(3, 127);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(43, 13);
			this->label6->TabIndex = 8;
			this->label6->Text = L"Set Min";
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Enabled = false;
			this->numericUpDown3->Location = System::Drawing::Point(65, 167);
			this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 21000, 0, 0, 0 });
			this->numericUpDown3->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3000, 0, 0, 0 });
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(84, 20);
			this->numericUpDown3->TabIndex = 7;
			this->numericUpDown3->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 21000, 0, 0, 0 });
			this->numericUpDown3->ValueChanged += gcnew System::EventHandler(this, &Main_window::numericUpDown3_ValueChanged);
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Enabled = false;
			this->numericUpDown2->Location = System::Drawing::Point(65, 125);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 20000, 0, 0, 0 });
			this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2000, 0, 0, 0 });
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(84, 20);
			this->numericUpDown2->TabIndex = 6;
			this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2000, 0, 0, 0 });
			this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &Main_window::numericUpDown2_ValueChanged);
			// 
			// button9
			// 
			this->button9->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button9->Enabled = false;
			this->button9->Location = System::Drawing::Point(882, 220);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(74, 45);
			this->button9->TabIndex = 5;
			this->button9->Text = L"Move Servo";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Main_window::button9_Click);
			// 
			// trackBar2
			// 
			this->trackBar2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->trackBar2->Enabled = false;
			this->trackBar2->LargeChange = 1000;
			this->trackBar2->Location = System::Drawing::Point(6, 220);
			this->trackBar2->Maximum = 21000;
			this->trackBar2->Minimum = 2000;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(823, 45);
			this->trackBar2->SmallChange = 200;
			this->trackBar2->TabIndex = 4;
			this->trackBar2->TickFrequency = 200;
			this->trackBar2->Value = 21000;
			this->trackBar2->ValueChanged += gcnew System::EventHandler(this, &Main_window::trackBar2_ValueChanged);
			// 
			// button8
			// 
			this->button8->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button8->Enabled = false;
			this->button8->Location = System::Drawing::Point(882, 271);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(74, 45);
			this->button8->TabIndex = 3;
			this->button8->Text = L"Test Servo";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Main_window::button8_Click);
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Enabled = false;
			this->numericUpDown1->Location = System::Drawing::Point(65, 6);
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(84, 20);
			this->numericUpDown1->TabIndex = 2;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &Main_window::numericUpDown1_ValueChanged);
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(6, 96);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(59, 17);
			this->checkBox2->TabIndex = 1;
			this->checkBox2->Text = L"Asse Z";
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &Main_window::checkBox2_CheckedChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(6, 7);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(53, 17);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"Ripeti";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Main_window::checkBox1_CheckedChanged);
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::SystemColors::Control;
			this->tabPage1->Controls->Add(this->printing_label);
			this->tabPage1->Controls->Add(this->button12);
			this->tabPage1->Controls->Add(this->button10);
			this->tabPage1->Controls->Add(this->button2);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->trackBar1);
			this->tabPage1->Controls->Add(this->button3);
			this->tabPage1->Controls->Add(this->button4);
			this->tabPage1->Controls->Add(this->pictureBox);
			this->tabPage1->Controls->Add(this->button7);
			this->tabPage1->Controls->Add(this->button6);
			this->tabPage1->Controls->Add(this->button1);
			this->tabPage1->Controls->Add(this->button5);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(956, 563);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Principale";
			// 
			// printing_label
			// 
			this->printing_label->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->printing_label->AutoSize = true;
			this->printing_label->Location = System::Drawing::Point(7, 543);
			this->printing_label->Name = L"printing_label";
			this->printing_label->Size = System::Drawing::Size(126, 13);
			this->printing_label->TabIndex = 17;
			this->printing_label->Text = L"Nessuna stampa in corso";
			// 
			// button12
			// 
			this->button12->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button12->Location = System::Drawing::Point(750, 514);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(97, 43);
			this->button12->TabIndex = 16;
			this->button12->Text = L"Salva";
			this->button12->UseVisualStyleBackColor = true;
			this->button12->Click += gcnew System::EventHandler(this, &Main_window::button12_Click);
			// 
			// button10
			// 
			this->button10->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button10->Location = System::Drawing::Point(853, 514);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(97, 43);
			this->button10->TabIndex = 15;
			this->button10->Text = L"espelli cassetto";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &Main_window::button10_Click);
			// 
			// button2
			// 
			this->button2->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->button2->Location = System::Drawing::Point(6, 7);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(77, 175);
			this->button2->TabIndex = 3;
			this->button2->Text = L"<";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Main_window::button2_Click);
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(872, 457);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(24, 13);
			this->label5->TabIndex = 14;
			this->label5->Text = L"0/0";
			// 
			// trackBar1
			// 
			this->trackBar1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->trackBar1->Location = System::Drawing::Point(89, 457);
			this->trackBar1->Maximum = 0;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(777, 45);
			this->trackBar1->TabIndex = 13;
			this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &Main_window::trackBar1_ValueChanged);
			// 
			// button3
			// 
			this->button3->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->button3->Location = System::Drawing::Point(6, 369);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(77, 67);
			this->button3->TabIndex = 4;
			this->button3->Text = L"Aggiungi";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Main_window::button3_Click);
			// 
			// button4
			// 
			this->button4->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->button4->Location = System::Drawing::Point(6, 188);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(77, 175);
			this->button4->TabIndex = 5;
			this->button4->Text = L"<-";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Main_window::button4_Click);
			// 
			// pictureBox
			// 
			this->pictureBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox->BackColor = System::Drawing::Color::White;
			this->pictureBox->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox.BackgroundImage")));
			this->pictureBox->ContextMenuStrip = this->contextMenuStrip1;
			this->pictureBox->Location = System::Drawing::Point(89, 6);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(777, 443);
			this->pictureBox->TabIndex = 1;
			this->pictureBox->TabStop = false;
			this->pictureBox->ClientSizeChanged += gcnew System::EventHandler(this, &Main_window::pictureBox_ClientSizeChanged);
			this->pictureBox->Click += gcnew System::EventHandler(this, &Main_window::pictureBox_Click);
			this->pictureBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Main_window::pictureBox_Paint);
			this->pictureBox->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Main_window::pictureBox_MouseDown);
			this->pictureBox->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Main_window::pictureBox_MouseMove);
			this->pictureBox->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Main_window::pictureBox_MouseUp);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->ToolStrip_Centra,
					this->ToolStrip_Adatta, this->ToolStrip_Ritaglia, this->ToolStrip_Dimensioni
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(135, 92);
			// 
			// ToolStrip_Centra
			// 
			this->ToolStrip_Centra->Name = L"ToolStrip_Centra";
			this->ToolStrip_Centra->Size = System::Drawing::Size(134, 22);
			this->ToolStrip_Centra->Text = L"Centra";
			this->ToolStrip_Centra->Click += gcnew System::EventHandler(this, &Main_window::ToolStripEvent_Centra);
			// 
			// ToolStrip_Adatta
			// 
			this->ToolStrip_Adatta->Name = L"ToolStrip_Adatta";
			this->ToolStrip_Adatta->Size = System::Drawing::Size(134, 22);
			this->ToolStrip_Adatta->Text = L"Adatta";
			this->ToolStrip_Adatta->Click += gcnew System::EventHandler(this, &Main_window::ToolStripEvent_Adatta);
			// 
			// ToolStrip_Ritaglia
			// 
			this->ToolStrip_Ritaglia->Name = L"ToolStrip_Ritaglia";
			this->ToolStrip_Ritaglia->Size = System::Drawing::Size(134, 22);
			this->ToolStrip_Ritaglia->Text = L"Ritaglia";
			this->ToolStrip_Ritaglia->Click += gcnew System::EventHandler(this, &Main_window::ToolStripEvent_Ritaglia);
			// 
			// ToolStrip_Dimensioni
			// 
			this->ToolStrip_Dimensioni->Name = L"ToolStrip_Dimensioni";
			this->ToolStrip_Dimensioni->Size = System::Drawing::Size(134, 22);
			this->ToolStrip_Dimensioni->Text = L"Dimensioni";
			this->ToolStrip_Dimensioni->Click += gcnew System::EventHandler(this, &Main_window::ToolStripEvent_Dimensioni);
			// 
			// button7
			// 
			this->button7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button7->Location = System::Drawing::Point(6, 444);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(77, 67);
			this->button7->TabIndex = 8;
			this->button7->Text = L"stampa";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Main_window::button7_Click);
			// 
			// button6
			// 
			this->button6->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button6->Location = System::Drawing::Point(874, 369);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(76, 66);
			this->button6->TabIndex = 7;
			this->button6->Text = L"Rimuovi";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Main_window::button6_Click);
			// 
			// button1
			// 
			this->button1->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button1->Location = System::Drawing::Point(872, 7);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(78, 175);
			this->button1->TabIndex = 2;
			this->button1->Text = L">";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Main_window::button1_Click);
			// 
			// button5
			// 
			this->button5->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->button5->Location = System::Drawing::Point(873, 188);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(77, 175);
			this->button5->TabIndex = 6;
			this->button5->Text = L"->";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Main_window::button5_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(964, 589);
			this->tabControl1->TabIndex = 13;
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			// 
			// Main_window
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(984, 611);
			this->Controls->Add(this->tabControl1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MinimumSize = System::Drawing::Size(800, 640);
			this->Name = L"Main_window";
			this->Text = L"Penelope GUI";
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox))->EndInit();
			this->contextMenuStrip1->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion


		// Various Functions
private: System::Void init(){
		 
			 lfp();
			 pictureBox->SizeMode = PictureBoxSizeMode::CenterImage;//Zoom
			 pictureBox->BackColor = Color::White;
			 openFileDialog->FileName = "";
			 p = -1;
			 z1 = 0;
			 z2 = 0;
			 z3 = 0;
			 b7 = true;
			 ez = false;
			 IC = gcnew Immagini;
			 pc = new phidclass;
			 wh_ratio = (float)pictureBox->Width / (float)pictureBox->Height;
			 p_opt = gcnew print_options;
			 p_opt->eject_on_finish = false;
			 p_opt->p_mode = print_in_3d;
			 p_opt->Cd = (int)((pictureBox->Width-pictureBox->Height/SF)/2);
			 traslate = false;
			 ws_f = gcnew well_selector;
		 }
private: System::Void pictureBox_ClientSizeChanged(System::Object^  sender, System::EventArgs^  e) {
			if(IC->n==-1 ||  pictureBox->Image == nullptr) return;
			
			wh_ratio = (float)pictureBox->Width / (float)pictureBox->Height;

			if(p_opt->p_mode == 1)
				p_opt->Cd = (int)(y_slide_i*pictureBox->Height/hei);
			else
				p_opt->Cd = (int)((pictureBox->Width-pictureBox->Height/SF)/2);
			pictureBox->Refresh();
		 }
private: Image^ get_imag(){
			 Bitmap^ tmp = gcnew Bitmap(pictureBox->Width,pictureBox->Height);
			 pictureBox->DrawToBitmap(tmp, pictureBox->ClientRectangle);
			 return (Image^)tmp;
		 }

private: bool select_printer(PrintDocument^ p){
			 p->PrinterSettings->PrinterName=printer_name;
			 if(p->PrinterSettings->IsValid) return true;
				 return false;
		 }
private: System::Void lfp(){		//look for printer
			 PrintDocument^ p = gcnew PrintDocument;
			 comboBox1->BeginUpdate();
			 comboBox1->Items->Clear();
			 for ( int i = 0; i < PrinterSettings::InstalledPrinters->Count; i++ ){
				 if(PrinterSettings::InstalledPrinters[i]->Contains("HP DeskJet 340")){
					 comboBox1->Items->Add(PrinterSettings::InstalledPrinters[i]);
				 }
			 }
			 comboBox1->EndUpdate();
			 if(comboBox1->Items->Count==0) return;
			 comboBox1->SelectedIndex=0;
			 return;
		 }
private: System::Void button_enable(){
			 button1->Enabled=true;
			 button2->Enabled=true;
			 button3->Enabled=true;
			 button4->Enabled=true;
			 button5->Enabled=true;
			 button6->Enabled=true;
			 button7->Enabled=true;
			 button10->Enabled=true;
			 trackBar1->Enabled=true;
		 }
private: System::Void button_disable(){
			 button1->Enabled=false;
			 button2->Enabled=false;
			 button3->Enabled=false;
			 button4->Enabled=false;
			 button5->Enabled=false;
			 button6->Enabled=false;
			 button7->Enabled=false;
			 button10->Enabled=false;
			 trackBar1->Enabled=false;
			 return;
		 }

		 
		 //add-remove
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 int start,end,m,mi;
			 int error;
			 String^ ster = "";
			 openFileDialog->Filter="File immagine|*.*|Lavoro salvato (*.txt)|*.txt";
			 openFileDialog->ShowDialog();
			 if(openFileDialog->SafeFileName->EndsWith(".txt")){
				 if(IC->n!=-1){
					 if(MessageBox::Show("Si è scelto di caricare un nuovo lavoro, così facendo si perderanno i dati del lavoro attuale. Continuare?",
						 "Warning",MessageBoxButtons::YesNo,MessageBoxIcon::Question) == ::System::Windows::Forms::DialogResult::No){
							 openFileDialog->FileName="";
							 return;
					 }
					 delete IC;
					 IC = gcnew Immagini;
				 }
				 error = IC->open_work(openFileDialog->FileName);
				 switch (error) {
				 case 0:
					 ster = "";
					 break;
				 case 1:
					 ster = "Alcuni elementi non sono stati caricati, path o file non validi.";
					 break;
				 case 2:
					 ster = "Troppi elementi, alcuni non sono stati caricati.";
					 break;
				 case 3:
					 ster = "Impossibile aprire il file di lavoro";
					 break;
				 }
				 if(!String::IsNullOrEmpty(ster)) MessageBox::Show(ster,"warning");
				 p=IC->n;
				 if(p!=-1)
					 MessageBox::Show(String::Concat("Sono stati aggiunti ",IC->n+1," elementi"));
			 }
			 else{
				 if(IC->n==max_image-1) return;
				 if(String::IsNullOrEmpty(openFileDialog->FileName)) return;			 
				 if(IC->find_n(openFileDialog->FileName,&start,&end)){
					 int cm=openFileDialog->FileName->LastIndexOf("\\");				
					 if (MessageBox::Show(
						 String::Concat("Aprire tutti i file :", openFileDialog->FileName->Substring(cm+1,start-cm-1), 
						 String::Concat("*",openFileDialog->FileName->Substring(end+1,openFileDialog->FileName->Length-end-1),"?")),
						 openFileDialog->SafeFileName, 
						 MessageBoxButtons::YesNo,MessageBoxIcon::Question) == ::System::Windows::Forms::DialogResult::Yes){
							 String^ str=openFileDialog->FileName;
							 Int32::TryParse(str->Substring(start,end-start+1), m);
							 mi=m;
							 while(1==1){
								 openFileDialog->FileName=String::Concat(str->Substring(0,start),m,str->Substring(end+1,str->Length-end-1));
								 if(!IC->add(openFileDialog->FileName,p++)){
									 p--;
									 break;
								 }
								 m++;
							 }
							 MessageBox::Show(String::Concat("Sono stati aggiunti ",m-mi," elementi"));
					 }
					 else{
						 if(!IC->add(openFileDialog->FileName,p++)){
							 MessageBox::Show("File non trovato");
							 p--;
						 }
					 }
				 }
				 else{
					 if(!IC->add(openFileDialog->FileName,p++)){
						 MessageBox::Show("File non trovato");
						 p--;
					 }
				 }
			 }
			 openFileDialog->FileName="";
			 if(p==-1){
				 MessageBox::Show("Non è stato aggiunto nulla");
				 return;
			 }

			 pictureBox->Load(IC->im[p].name);
			 imag = pictureBox->Image;
			 if(IC->im[p].rep==1){
				  checkBox1->Checked=false;
			      numericUpDown1->Enabled=false;
				  numericUpDown1->Value=1;
			 }
			 else{
				  checkBox1->Checked=true;
			      numericUpDown1->Enabled=true;
				  numericUpDown1->Value=IC->im[p].rep;
			 }
			 trackBar1->Maximum=IC->n;
			 trackBar1->Value=p;
			 label5->Text=String::Concat(p+1,"/",IC->n+1);

		 }
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(IC->n==-1) return;

			 IC->scalas(p);

			 if(p==IC->n) p--;
			 IC->n--;

			 if(IC->n!=-1){
				 pictureBox->Load(IC->im[p].name);
				 imag = pictureBox->Image;
				 trackBar1->Maximum=IC->n;
				 trackBar1->Value=p;
			 }
			 else{
				 pictureBox->Image = nullptr;
				 imag = nullptr;
				 trackBar1->Maximum=0;
				 trackBar1->Value=0;
			 }
			 label5->Text=String::Concat(p+1,"/",IC->n+1);
		 }

		 //navigate image
private: System::Void naviga(int pos){
			 pictureBox->Load(IC->im[pos].name);
			 imag = pictureBox->Image;
			 if (IC->im[pos].adatta)
				 im_adatta();
			 else
				 if (IC->im[pos].ritaglia)
					 im_ritaglia();
				 else
					 im_centra();
			 if(IC->im[pos].rep==1){
				 checkBox1->Checked=false;
				 numericUpDown1->Enabled=false;
				 numericUpDown1->Value=1;
			 }
			 else{
				 checkBox1->Checked=true;
				 numericUpDown1->Enabled=true;
				 numericUpDown1->Value=IC->im[pos].rep;
			 }
			 label5->Text=String::Concat(p+1,"/",IC->n+1);
			 if (b7)
				 printing_label->Text = L"Nessuna stampa in corso";
			 else{
				 printing_label->Text = String::Concat("Stampa in corso della ripetizione", k+1, "/", IC->im[pos].rep);
			 }
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(p==IC->n || IC->n==-1) return;
			 naviga(++p);
			 trackBar1->Value=p;
			 if (IC->im[p].adatta)
				 im_adatta();
			 else
				 if (IC->im[p].ritaglia)
					 im_ritaglia();
				 else
					 im_centra();
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(p==0 || IC->n==-1) return;
			 naviga(--p);
			 trackBar1->Value=p;
			 if(IC->im[p].adatta)
				 im_adatta();
			 else
				 if (IC->im[p].ritaglia)
					 im_ritaglia();
				 else
					 im_centra();
		 }

		 //change image order
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(p==0 || IC->n==-1) return;
			 IC->spostas(p--);
			 pictureBox->Load(IC->im[p].name);
			 imag = pictureBox->Image;
			 trackBar1->Value=p;
			 label5->Text=String::Concat(p+1,"/",IC->n+1);
		 }
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(p==IC->n || IC->n==-1) return;
			 IC->spostad(p++);
			 pictureBox->Load(IC->im[p].name);
			 imag = pictureBox->Image;
			 trackBar1->Value=p;
			 label5->Text=String::Concat(p+1,"/",IC->n+1);
		 }

		 //print
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (IC->n == -1) return;

			 if(pc->posizione-((z1-z2+z3)/1000)*IC->n < pc->pmin && ez==true){
				 MessageBox::Show(String::Concat("Impossibile stampare il numero di strati impostato\nPosizione attuale [μm] : ",
					 pc->posizione*1000,"\nPosizione finale [μm] : ", pc->posizione-(z1-z2+z3)*IC->n,"\nPosizione minima [μm] : ",pc->pmin*1000));
				 return;
			 }
			 if(b7==false){
				 button7->Enabled=false;
				 backgroundWorker1->CancelAsync();
				 MessageBox::Show("Annullamento stampa in corso");
			 }
			 else{/*
				 if(IC->dimension_warning()){
					 if(MessageBox::Show("Alcune immagini superano le dimensioni consentite, continuare la stampa?","Warning",
						 MessageBoxButtons::YesNo,MessageBoxIcon::Question) == ::System::Windows::Forms::DialogResult::No) return;
				 }*/
				 if(pc->switch_888) pc->switch_888_handler();
				 b7=false;
				 button7->Text=L"Annulla stampa";
				 button_disable();
				 button7->Enabled=true;
				 backgroundWorker1->RunWorkerAsync();
			 }

		 }

		  //repeat
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 numericUpDown1->Enabled=checkBox1->Checked;
			 if(checkBox1->Checked==false) numericUpDown1->Value=1;
		 }
private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 IC->im[p].rep=(int)numericUpDown1->Value;
		 }

		 //trackbar control
private: System::Void trackBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 p=trackBar1->Value;
			 naviga(p);
		 }
		 
		 //printing background worker
private: System::Void initbgw(){
			 backgroundWorker1->DoWork += gcnew DoWorkEventHandler( this, &Main_window::backgroundWorker1_DoWork );
			 backgroundWorker1->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &Main_window::backgroundWorker1_RunWorkerCompleted );
			 backgroundWorker1->ProgressChanged += gcnew ProgressChangedEventHandler( this, &Main_window::backgroundWorker1_ProgressChanged );
		 }
private: System::Void backgroundWorker1_DoWork(Object^ sender, DoWorkEventArgs^ e){
				 BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
				 PrintDocument^ pd = gcnew PrintDocument;
				 Image^ tmpimg;
				 int p1=p;
				 
				 //pc->move_servo(za/1000);
				 p=0;
				 pc->ready=false;

				 pd->PrinterSettings->PrinterName = "HP DeskJet 340";
				 if(!select_printer(pd)){
					 MessageBox::Show(pd->PrinterSettings->PrinterName);
					 //MessageBox::Show("stampante non valida");
					 return;
				 }
				 pd->DefaultPageSettings->Color = false;

				 pd->PrinterSettings->PrinterName = "Microsoft XPS Document Writer";	//debug purpose only

				 for(p=0;p<=IC->n;p++){
					 for(k=0;k<IC->im[p].rep;k++){
						 worker->ReportProgress(NULL);
						 if(worker->CancellationPending){
							 e->Cancel=true;
							 p=p1;
							 return;
						 }
						 pd->PrintPage += gcnew PrintPageEventHandler( this, &Main_window::pd_PrintPage );
						 pd->Print();
						 while(pc->ready==false && !worker->CancellationPending){
							 //Sleep(100);
							 DWORD Start_time = GetTickCount();
							 MessageBox::Show(String::Concat(GetTickCount()));
							 while (GetTickCount() < Start_time + 100){
								 Void;
							 }
							 MessageBox::Show(String::Concat(GetTickCount()));
						 }
						 pc->ready=false;
					 }
					 if(worker->CancellationPending){
						 e->Cancel=true;
						 p=p1;
						 return;
					 }
					 if((z1-z2)>0 && ez==true){
						 pc->move_servo(pc->posizione-(z1-z2)/1000);
						 Sleep(500);
					 }
					 if((z3-z2)>0 && ez==true){
						 pc->move_servo(pc->posizione+(z3-z2)/1000);
						 Sleep(500);
					 }
				 }
				 p=p1;
				 return;
		 }
private: System::Void pd_PrintPage( Object^ sender, PrintPageEventArgs^ ev ){
			 RectangleF dest_rect;
			 GraphicsUnit units = GraphicsUnit::Pixel;
			 RectangleF rect;
			 array<array<bool>^>^ mw_flag = IC->get_flag();

			 printing_image = Image::FromFile( IC->im[p].name );
			 float xd,yd; // starting point
			 
			 if (p_opt->p_mode == print_in_3d){
				 dest_rect = RectangleF(ox, oy, wid, hei);
				 if (IC->im[p].adatta){
					 xd = (float)printing_image->Width - (float)(printing_image->Height / SF);
					 xd /= 2;
					 yd = 0;
					 rect = RectangleF((int)xd, yd, (int)(printing_image->Height / SF), printing_image->Height);
				 }
				 else{
					 if (!IC->im[p].ritaglia){
						 float g, h, i, l;
						 g = pictureBox->Height - printing_image->Height;
						 g /= 2;
						 h = pictureBox->Width - printing_image->Width;
						 h /= 2;
						 i = (float)pictureBox->Width - (float)pictureBox->Height / SF;
						 i /= 2;
						 l = h - i;
						 rect = RectangleF((int)-l, (int)-g, (int)(pictureBox->Height / SF), pictureBox->Height);
					 }
					 else{
						 float i, l;
						 i = (float)pictureBox->Width - (float)pictureBox->Height / SF;
						 i /= 2;
						 l = -IC->im[p].x - i;
						 rect = RectangleF((int)-l, (int)IC->im[p].y, (int)(pictureBox->Height / SF), pictureBox->Height);
					 }
				 }
				 ev->Graphics->DrawImage(printing_image, dest_rect, rect, units);
			 }
			 if (p_opt->p_mode == print_on_slide){
				 dest_rect = RectangleF(ox, oy + y_slide_i, width_slide, y_slide_f - y_slide_i);
				 if (IC->im[p].adatta){
					 yd = (float)printing_image->Height - (float)(printing_image->Width * slide_SF);
					 yd /= 2;
					 xd = 0;
					 rect = RectangleF((int)xd, yd, printing_image->Width, (int)printing_image->Width * slide_SF);
				 }
				 else{
					 if (!IC->im[p].ritaglia){
						 float g, h, i, l;
						 h = pictureBox->Height - printing_image->Height;
						 h /= 2;
						 g = pictureBox->Width - printing_image->Width;
						 g /= 2;
						 i = (float)pictureBox->Height - (float)pictureBox->Width * slide_SF;
						 i /= 2;
						 l = h - i;
						 rect = RectangleF((int)-g, (int)-l, pictureBox->Width, (int)pictureBox->Width * slide_SF);
					 }
					 else{
						 float i, l;
						 i = (float)pictureBox->Height - (float)pictureBox->Width * slide_SF;
						 i /= 2;
						 l = -IC->im[p].y - i;
						 rect = RectangleF((int)IC->im[p].x, (int)-l, pictureBox->Width, (int)pictureBox->Width * slide_SF);
					 }
				 }
				 ev->Graphics->DrawImage(printing_image, dest_rect, rect, units);
				 
				 
				 /*
				 float rate_y = printing_image->Height/hei;
				 dest_rect = RectangleF(ox, oy + y_slide_i, width_slide, y_slide_f - y_slide_i);
				 rect = RectangleF( 0, int(rate_y*y_slide_i), width_slide, int(rate_y*(y_slide_f - y_slide_i)));
				 ev->Graphics->DrawImage(printing_image,dest_rect,rect,units);*/
			 }
			 if (p_opt->p_mode == multiwell_6 || p_opt->p_mode == multiwell_24 || p_opt->p_mode == multiwell_96){
				 if (IC->im[p].adatta){
					 xd = (float)printing_image->Width - (float)(printing_image->Height / SF);
					 xd /= 2;
					 yd = 0;
					 rect = RectangleF((int)xd, yd, (int)(printing_image->Height / SF), printing_image->Height);
				 }
				 else{
					 if (!IC->im[p].ritaglia){
						 float g, h, i, l;
						 g = pictureBox->Height - printing_image->Height;
						 g /= 2;
						 h = pictureBox->Width - printing_image->Width;
						 h /= 2;
						 i = (float)pictureBox->Width - (float)pictureBox->Height;
						 i /= 2;
						 l = h - i;
						 rect = RectangleF((int)-l, (int)-g, pictureBox->Height, pictureBox->Height);
					 }
					 else{
						 float i, l;
						 i = (float)pictureBox->Width - (float)pictureBox->Height;
						 i /= 2;
						 l = -IC->im[p].x - i;
						 rect = RectangleF((int)-l, (int)IC->im[p].y, pictureBox->Height, pictureBox->Height);
					 }
				 }
			 }
			 if( p_opt->p_mode == multiwell_6 ){
				 for( int i = 0; i < mw_rowmask->Length; i++)
					 for( int j = 0; j < 3; j++){
						 if(!mw_flag[i][j]) continue;
						 dest_rect = RectangleF(mw_6_ox + j*mw_6_d, mw_6_oy + mw_rowmask[i]*mw_6_d, mw_6_wid, mw_6_hei);
						 ev->Graphics->DrawImage(printing_image,dest_rect,rect,units);
					 }
			 }
			 if( p_opt->p_mode == multiwell_24 ){
				 for( int i = 0; i < mw_rowmask->Length; i++)
					 for( int j = 0; j < 6; j++){
						 if(!mw_flag[i][j]) continue;
						 dest_rect = RectangleF(mw_24_ox + j*mw_24_d, mw_24_oy + mw_rowmask[i]*mw_24_d, mw_24_wid, mw_24_hei);
						 ev->Graphics->DrawImage(printing_image,dest_rect,rect,units);
					 }
			 }
			 if( p_opt->p_mode == multiwell_96 ){
				 for( int i = 0; i < mw_rowmask->Length; i++)
					 for( int j = 0; j < 12; j++){
						 if(!mw_flag[i][j]) continue;
						 dest_rect = RectangleF(mw_96_ox + j*mw_96_d, mw_96_oy + mw_rowmask[i]*mw_96_d, mw_96_wid, mw_96_hei);
						 ev->Graphics->DrawImage(printing_image,dest_rect,rect,units);
					 }
			 }

			 //ev->PageSettings->Margins = margins;
			 ev->PageSettings->Color = false;

			 ev->HasMorePages = false;

		 }
private: System::Void backgroundWorker1_RunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e){
			 button7->Text=L"Stampa";
			 b7=true;
			 button_enable();
			 naviga(0);
			 if(e->Cancelled)
				 MessageBox::Show("Stampa annullata");
			 else
				 if(p_opt->eject_on_finish)
					 button10_Click(this,nullptr);
			 naviga(p);
			 return;
		 }
private: System::Void backgroundWorker1_ProgressChanged( Object^ sender, ProgressChangedEventArgs^ e ){
			 naviga(p);
			 return;
		 }

		//servo control
private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e){
			 button8->Enabled=checkBox2->Checked;
			 button9->Enabled=checkBox2->Checked;
			 trackBar2->Enabled=checkBox2->Checked;
			 numericUpDown2->Enabled=checkBox2->Checked;
			 numericUpDown3->Enabled=checkBox2->Checked;
			 textBox1->Enabled=checkBox2->Checked;
			 textBox2->Enabled=checkBox2->Checked;
			 textBox3->Enabled=checkBox2->Checked;
			 trackBar3->Enabled=checkBox2->Checked;
			 trackBar4->Enabled=checkBox2->Checked;
			 trackBar5->Enabled=checkBox2->Checked;
			 textBox4->Enabled=checkBox2->Checked;
			 ez=checkBox2->Checked;

			 if(checkBox2->Checked==false) return;
			 trackBar2->Minimum=smin*1000;
			 trackBar2->Maximum=smax*1000;
			 pc->pmin=smin;
			 pc->pmax=smax;
			 za=(double)trackBar2->Value;
			 return;
		 }
private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
			 pc->test_servo();
			 return;
		 }
private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {
			 pc->move_servo(trackBar2->Value/1000);
			 return;
		 }
private: System::Void numericUpDown2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 trackBar2->Minimum=(int)numericUpDown2->Value;
			 numericUpDown3->Minimum=numericUpDown2->Value;
			 pc->pmin=(double)numericUpDown2->Value;
		 }
private: System::Void numericUpDown3_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 trackBar2->Maximum=(int)numericUpDown3->Value;
			 numericUpDown2->Maximum=numericUpDown3->Value;
			 pc->pmax=(double)numericUpDown3->Value;
		 }
private: System::Void trackBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 trackBar2->Value=trackBar2->Value-trackBar2->Value%200;
			 textBox4->Text=String::Concat(trackBar2->Value);
			 za=(double)trackBar2->Value;
		 }
private: System::Void textBox4_TextChanged(System::Object^  sender, System::EventArgs^  e) { 
			 int d;
			 if(INT32::TryParse(textBox4->Text, d) || !String::IsNullOrWhiteSpace(textBox4->Text)){
				 if(d<(int)numericUpDown2->Value){
					 textBox4->Text=String::Concat(numericUpDown2->Value);
					 trackBar2->Value=(int)numericUpDown2->Value;
				 }
				 else{
					 if(d>(int)numericUpDown3->Value){
						 textBox4->Text=String::Concat(numericUpDown3->Value);
						 trackBar2->Value=(int)numericUpDown3->Value;
					 }
					 else
						 trackBar2->Value=(int)d;
				 }
			 }
			 else{
				 textBox4->Text=String::Concat(numericUpDown2->Value);
				 trackBar2->Value=(int)numericUpDown2->Value;
			 }
			 za=trackBar2->Value;

		 }
private: System::Void trackBar3_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 trackBar3->Value=trackBar3->Value-trackBar3->Value%200;
			 textBox1->Text=String::Concat(trackBar3->Value);
			 z1=trackBar3->Value;
		 }
private: System::Void trackBar4_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 trackBar4->Value=trackBar4->Value-trackBar4->Value%200;
			 textBox2->Text=String::Concat(trackBar4->Value);
			 z2=trackBar4->Value;
		 }
private: System::Void trackBar5_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 trackBar5->Value=trackBar5->Value-trackBar5->Value%200;
			 textBox3->Text=String::Concat(trackBar5->Value);
			 z3=trackBar5->Value;
		 }
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 double d;
			 if(Double::TryParse(textBox1->Text, d) || !String::IsNullOrWhiteSpace(textBox1->Text)){
				 if(d<0){
					 textBox1->Text="0";
					 trackBar3->Value=0;
				 }
				 else{
					 if(d>2000){
						 textBox1->Text="2000";
						 trackBar3->Value=2000;
					 }
					 else
						 trackBar3->Value=(int)d;
				 }
			 }
			 else{
				 textBox1->Text="0";
				 trackBar3->Value=0;
			 }
			 z1=trackBar3->Value;
		 }
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 double d;
			 if(Double::TryParse(textBox2->Text, d) || !String::IsNullOrWhiteSpace(textBox2->Text)){
				 if(d<0){
					 textBox2->Text="0";
					 trackBar4->Value=0;
				 }
				 else{
					 if(d>2000){
						 textBox2->Text="2000";
						 trackBar4->Value=2000;
					 }
					 else
						 trackBar4->Value=(int)d;
				 }
			 }
			 else{
				 textBox2->Text="0";
				 trackBar4->Value=0;
			 }
			 z2=trackBar4->Value;
		 }
private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 double d;
			 if(Double::TryParse(textBox3->Text, d) || !String::IsNullOrWhiteSpace(textBox3->Text)){
				 if(d<0){
					 textBox3->Text="0";
					 trackBar5->Value=0;
				 }
				 else{
					 if(d>2000){
						 textBox3->Text="2000";
						 trackBar5->Value=2000;
					 }
					 else
						 trackBar5->Value=(int)d;
				 }
			 }
			 else{
				 textBox3->Text="0";
				 trackBar5->Value=0;
			 }
			 z3=trackBar5->Value;
		 }

		 //picturebox toolstrip menu
private: System::Void im_adatta(){
			IC->im[p].adatta=true;
			IC->im[p].ritaglia=false;
			pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
			pictureBox->Refresh();
			return;
		 }
private: System::Void im_centra(){
			IC->im[p].adatta = false;
			IC->im[p].ritaglia = false;
			pictureBox->SizeMode = PictureBoxSizeMode::CenterImage;
			pictureBox->Refresh();
			return;
}
private: System::Void im_ritaglia(){
			IC->im[p].adatta=false;
			IC->im[p].ritaglia=true;
			pictureBox->SizeMode = PictureBoxSizeMode::Normal;
			pictureBox->Refresh();
			return;
		 }
private: System::Void ToolStripEvent_Adatta(System::Object^  sender, System::EventArgs^  e) {
			im_adatta();
			return;
		 }
private: System::Void ToolStripEvent_Ritaglia(System::Object^  sender, System::EventArgs^  e) {
			im_ritaglia();
			return;
		 }
private: System::Void ToolStripEvent_Dimensioni(System::Object^  sender, System::EventArgs^  e) {
			if(IC->n==-1) return;
			Image^ image1;
			float altezza,larghezza;
			String^ str;
			image1 = Image::FromFile( IC->im[p].name );
			altezza = 2540*image1->Height/image1->VerticalResolution;
			larghezza = 2540*image1->Width/image1->HorizontalResolution;
			if(image1->Height>hei && image1->Width>wid)
				str="\n\nImmagine fuori dimensione";
			else
				str="\n\nDimensioni accettabili";
			MessageBox::Show(String::Concat("Altezza [μm] ",altezza,"\n Larghezza [μm] ",larghezza,str),"Dimensioni immagine");
			return;
		 }
private: System::Void ToolStripEvent_Centra(System::Object^  sender, System::EventArgs^  e) {
			im_centra();
			return;
}

		 //move image
private: System::Void pictureBox_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if( e->Button == System::Windows::Forms::MouseButtons::Right || IC->n==-1 ) return;
			 if(!IC->im[p].ritaglia) return;
			 //starting coordinates for move
			 starting_x = e->X;
			 starting_y = e->Y;
			 traslate = true;
			 return;
		 }
private: System::Void pictureBox_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if( traslate == false ) return;
			 IC->im[p].x += (e->X - starting_x) / 1.5;
			 IC->im[p].y += (e->Y - starting_y) / 1.5;
			 starting_x = e->X;
			 starting_y = e->Y;
			 /*
			 if (IC->im[p].x + pictureBox->Image->Width < 0) IC->im[p].x = 500; //pictureBox->Image->Width;
			 if (IC->im[p].y + pictureBox->Image->Height < 0) IC->im[p].y = 500;// pictureBox->Image->Height;
			 if( IC->im[p].x + pictureBox->Width > pictureBox->Image->Width   ) IC->im[p].x = pictureBox->Image->Width - pictureBox->Width;
			 if( IC->im[p].y + pictureBox->Height > pictureBox->Image->Height ) IC->im[p].y = pictureBox->Image->Height - pictureBox->Height;
 */
             pictureBox->Refresh();
			 return;
		 }
private: System::Void pictureBox_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if( e->Button == System::Windows::Forms::MouseButtons::Right || IC->n==-1 ) return;
			 if(!IC->im[p].ritaglia) return;
             traslate = false;
             pictureBox->Refresh();
             
		 }
private: System::Void pictureBox_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 if(IC->n==-1 ||  pictureBox->Image == nullptr) return;

			 Graphics^ g = e->Graphics;
			 //Image^ imag = pictureBox->Image;
			 //Image^ imag = Image::FromFile(IC->im[p].name);
			 if (imag == nullptr) imag = Image::FromFile(IC->im[p].name); //probabilmente non serve più, ma così è più robusto
			 if (IC->im[p].ritaglia){
				pictureBox->Image = pictureBox->BackgroundImage;
				RectangleF dstrect = RectangleF(0, 0, pictureBox->Width, pictureBox->Height);
				RectangleF srcrect = RectangleF(IC->im[p].x, IC->im[p].y, pictureBox->Width, pictureBox->Height);
				GraphicsUnit units = GraphicsUnit::Pixel;

				g->DrawImage(imag,dstrect,srcrect,units);
			 }
			 

			 // Draw shapes over the image

			 if(p_opt->p_mode == print_in_3d){
				 Pen^ redPen = gcnew Pen(Color::Red, 2.5f);
				 // Rectangle
				 if (wh_ratio > (float)imag->Width / (float)imag->Height || !IC->im[p].adatta){
					 p_opt->Cd = (int)((pictureBox->Width - pictureBox->Height / SF) / 2);
					 g->DrawRectangle(redPen, p_opt->Cd, 0, (int)(pictureBox->Height / SF), pictureBox->Height);
				 }
				 else{ // solo adatta con wh_ratio <
					 float a, b, c, d, e, f;
					 b = pictureBox->Height;
					 c = pictureBox->Width;
					 a = c * (float)imag->Height / (float)imag->Width;
					 d = a / SF;
					 e = (b - a) / 2;
					 f = (c - d) / 2;
					 g->DrawRectangle(redPen, (int)f, (int)e, (int)(a / SF), (int)a);
				 }
				 
				 return;
			 }
			
			 if(p_opt->p_mode == print_on_slide){
				 // Slide
				 Pen^ greenPen = gcnew Pen(Color::Green, 2.5f);

				 if (wh_ratio < (float)imag->Width/ (float)imag->Height || !IC->im[p].adatta){
					 p_opt->Cd = (int)((pictureBox->Height - pictureBox->Width * slide_SF) / 2);
					 g->DrawRectangle(greenPen, 0, p_opt->Cd, pictureBox->Width, (int)(pictureBox->Width * slide_SF));
				 }
				 else{
					 float a, b, c, d, e, f;
					 c = pictureBox->Height;
					 b = pictureBox->Width;
					 a = c * (float)imag->Width / (float)imag->Height;
					 d = a * slide_SF;
					 e = (b - a) / 2;
					 f = (c - d) / 2;
					 g->DrawRectangle(greenPen, (int)e, (int)f, (int)a, (int)(a * slide_SF));
				 }
				 //g->DrawRectangle(greenPen, 0, p_opt->Cd, pictureBox->Width, (int)((y_slide_f - y_slide_i)*pictureBox->Height/hei));
				 return;
			 }

			 if( p_opt->p_mode == multiwell_6 || p_opt->p_mode == multiwell_24 || p_opt->p_mode == multiwell_96 ){
				 // Square
				 Pen^ bluePen = gcnew Pen(Color::Blue, 2.5f);
				 if (wh_ratio > (float)imag->Width / (float)imag->Height || !IC->im[p].adatta){
					 p_opt->Cd = (int)((pictureBox->Width - pictureBox->Height) / 2);
					 g->DrawRectangle(bluePen, p_opt->Cd, 0, pictureBox->Height, pictureBox->Height);
				 }
				 else{
					 float a, b, c, e, f;
					 b = pictureBox->Height;
					 c = pictureBox->Width;
					 a = c * (float)imag->Height / (float)imag->Width;
					 e = (b - a) / 2;
					 f = (c - a) / 2;
					 g->DrawRectangle(bluePen, (int)f, (int)e, (int)a, (int)a);
				 }
				 return;
			 }
			 
			 return;

		 }

		 //click
private: System::Void pictureBox_Click(System::Object^  sender, System::EventArgs^  e) {		 

		 }


		 //eject
private: System::Void blank_page(){
			 PrintDocument^ pd = gcnew PrintDocument;
			 pd->PrinterSettings->PrinterName=printer_name;
			 //pd->PrinterSettings->PrinterName = "Microsoft XPS Document Writer";
			 if(!pd->PrinterSettings->IsValid){
				 MessageBox::Show("stampante non valida");
				 return;
			 }
			 
			 pd->DefaultPageSettings->Color = false;
			 pd->PrintPage += gcnew PrintPageEventHandler( this, &Main_window::Print_blank_Page );
			 pd->Print();
			 return;
		 }
private: System::Void Print_blank_Page( Object^ sender, PrintPageEventArgs^ ev ){
			 
			 System::Drawing::Font^ drawFont = gcnew System::Drawing::Font( "Arial",16 );
			 SolidBrush^ drawBrush = gcnew SolidBrush( Color::Black );
             PointF drawPoint = PointF(30.0F,10.0F);
			 ev->Graphics->DrawString( " ", drawFont, drawBrush, drawPoint );
			 ev->PageSettings->Color = false;
			 ev->HasMorePages = false;
			 return;
		 }
private: System::Void open_close(){
			 pc->ready=false;
			 pc->switch_888_handler();
			 blank_page();
			 while(pc->ready==false){ Sleep(100); }
			 return;
		 }
private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {
			 static bool es=true;
			 if(es==true){
				 button10->Text=L"Richiama cassetto";
				 button_disable();
				 open_close();
				 button10->Enabled=true;
				 es=false;
				 return;
			 }
			 button10->Text=L"Espelli cassetto";
			 open_close();
			 button_enable();
			 es=true;
			 return;			 
		 }

		 
		 //print queue selection
private: System::Void button11_Click(System::Object^  sender, System::EventArgs^  e) {
			 lfp();
		 }
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 printer_name=String::Concat(comboBox1->SelectedItem);
			 return;
		 }


		 //save work
private: System::Void button12_Click(System::Object^  sender, System::EventArgs^  e) {
			 saveFileDialog1->Filter="File txt (*.txt)|*.txt";
			 saveFileDialog1->ShowDialog();
			 if(!IC->save_work(saveFileDialog1->FileName))
				 MessageBox::Show("Impossibile salvare il file");
			 return;
		 }

		 //print option
private: System::Void checkBox4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 p_opt->eject_on_finish = checkBox4->Checked;
			 return;
		 }
private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 p_opt->p_mode = static_cast<printing_mode> (comboBox2->SelectedIndex);
			 switch (comboBox2->SelectedIndex){
				 case 0:
					 p_opt->Cd = (int)((pictureBox->Width-pictureBox->Height/SF)/2);
					 ws->Enabled = false;
					 return;
				 case 1:
					 p_opt->Cd = (int)(y_slide_i*pictureBox->Height/hei);
					 ws->Enabled = false;
					 return;
				 case 2:
					 p_opt->Cd = (int)((pictureBox->Width-pictureBox->Height)/2);
					 {	 mw_rowmask = gcnew array<int> { 0, 1 };	}
					 ws->Enabled = true;
					 IC->mw_flag_init(p_opt->p_mode);
					 return;
				 case 3:
					 p_opt->Cd = (int)((pictureBox->Width-pictureBox->Height)/2);
					 {   mw_rowmask = gcnew array<int> { 0, 1 };	}
					 ws->Enabled = true;
					 IC->mw_flag_init(p_opt->p_mode);
					 return;
				 case 4:
					 p_opt->Cd = (int)((pictureBox->Width-pictureBox->Height)/2);
					 {	 mw_rowmask = gcnew array<int>{ 0, 1, 2, 3, 4, 5 };	}
					 ws->Enabled = true;
					 IC->mw_flag_init(p_opt->p_mode);
					 return;
				 default:
					 MessageBox::Show("Printing mode not supported", "error");
					 return;
			 }
			 return;
		 }

		 //well selection
private: System::Void ws_Click(System::Object^  sender, System::EventArgs^  e) {
			 ws_f->send_data(IC->get_flag(), p_opt->p_mode, mw_rowmask);
			 if(ws_f->ShowDialog() == Windows::Forms::DialogResult::OK)
				 IC->set_flag(ws_f->get_data());
			 return;
		 }
};
}