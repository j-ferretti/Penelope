#pragma once

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Printing;

public value struct immagine{
	String^ name;
	int rep;
	bool adatta;
	bool ritaglia;
	float x;
	float y;
	int w;
	int h;
	float wh_ratio;
};

public ref class Immagini{

public:
	Immagini(){
		im = gcnew array<immagine>(max_image);
		n = -1;
	}
protected:
	~Immagini(){ }
private:
	array<array<bool>^>^ mw_flag;
	int mw_row,mw_col;

public:
	array <immagine>^ im;
	int n;

public: System::Void mw_flag_init(printing_mode ptw){
			switch(ptw){
				case 2:
					mw_row = 2;
					mw_col = 3;
					break;
				case 3:
					mw_row = 4;
					mw_col = 6;
					break;
				case 4:
					mw_row = 8;
					mw_col = 12;
					break;
				default:
					return;
			}
			mw_flag = gcnew array<array<bool>^>(mw_row);
			for(int i=0;i<mw_row;i++){
				mw_flag[i] = gcnew array<bool>(mw_col);
				for(int j=0;j<mw_col;j++)
					mw_flag[i][j] = true;
			}
		}
public: array<array<bool>^>^ get_flag(){ 
			array<array<bool>^>^ mw_f = gcnew array<array<bool>^>(mw_row);
			for(int i=0;i<mw_row;i++){
				mw_f[i] = gcnew array<bool>(mw_col);
				for(int j=0;j<mw_col;j++)
					mw_f[i][j] = mw_flag[i][j];
			}

			return mw_f;
		}
public: System::Void set_flag(array<array<bool>^>^ mw_f){ mw_flag = mw_f; }

public: System::Void spostas(int pp){
			 immagine b;

			 b = im[pp];
			 im[pp] = im[pp-1];
			 im[--pp] = b;
			 
			 return;
		 }
public: System::Void spostad(int pp){
			 immagine b;

			 b = im[pp];
			 im[pp] = im[pp+1];
			 im[++pp] = b;

			 return;
		 }	

public: System::Void scalas(int i){
		     if(i==n) return;
			 immagine b;

			 b = im[i];
			 im[i] = im[i+1];
			 im[i+1] = b;
			 
			 scalas(i+1);
			 return;
		 }
public: System::Void scalad(int i){
		     if(i>=n) return;
			 scalad(i+1);
			 im[i+1] = im[i];
			 return;
		 }

public: bool find_n(String^ s, int *i, int *f){
			 *i=0;
			 *f=0;
			 for ( int j = s->Length-4 ; j > s->LastIndexOf("\\") ; j-- ){
				 if(j==0) break;
				 if ( isdigit(s[j]) )
					 if(*f==0){
						 *f=j;
						 *i=j;
					 }
					 else{
						 *i=j;
					 }
				 else{
					 if(*f!=0) return true;
				 }
			 }
			 if(*i==0 && *f==0) return false;
			 return true;
		 }

public: bool add(String^ s, int pp){
			 /*try{
				 Image::FromFile(s);
			 }
			 catch(Exception^ e){
				 if(dynamic_cast<FileNotFoundException^>(e)) return false;
			 }*/
			 if(!File::Exists(s)) return false;
			 Image^ imag = Image::FromFile(s);
			 n++;
			 scalad(++pp);
			 im[pp].name = s;
			 im[pp].rep=1;
			 im[pp].adatta = false;
			 im[pp].ritaglia = false;
			 im[pp].x = 0;
			 im[pp].y = 0;
			 im[pp].h = imag->Height;
			 im[pp].w = imag->Width;
			 im[pp].wh_ratio = (float)imag->Width / (float)imag->Height;
			 delete imag;
			 return true;
		 }

public: int image_verify(Image^ imag){
			 if(imag->Height>hei && imag->Width>wid)
				 return 2;
			 else{
				 if(imag->Height>hei){
					 return 1;
				 }
				 else{
					 if(imag->Width>wid){
						 return -1;
					 }
					 else{
						 return 0;
					 }
				 }
			 }
		 }
public: bool dimension_warning(){
			for(int i=0; i<=n; i++){
				if(image_verify(Image::FromFile(im[i].name))!=0) return true;
			}
			return false;
		}

public: bool save_work(String^ str){
			StreamWriter^ strwrtr;
			try{
				strwrtr = File::CreateText(str);
			}
			catch(...){
				return false;
			}
			for(int i=0; i<=n; i++){
				strwrtr->WriteLine(im[i].name);
				if(im[i].rep!=1) strwrtr->WriteLine(String::Concat("r = ",im[i].rep));
				if(im[i].x!=0) strwrtr->WriteLine(String::Concat("x = ",im[i].x));
				if(im[i].y!=0) strwrtr->WriteLine(String::Concat("y = ",im[i].y));
				if(im[i].adatta)
					strwrtr->WriteLine("adatta");
				else
					strwrtr->WriteLine("ritaglia");
			}
			strwrtr->Close();
			return true;
		}
		
public: int open_work(String^ str){
			StreamReader^ strdr;
			String^ rd;
			Image^ imag;
			double d;
			bool allvalid = true;

			try{
				strdr = File::OpenText(str);
			}
			catch(...){
				return 3;
			}

			n=-1;

			while(!strdr->EndOfStream){
				rd=strdr->ReadLine();

				if(String::IsNullOrWhiteSpace(rd)) continue;
				
				if( rd->StartsWith("r = ") && n!=-1 ){
					rd = rd->Remove(0,4);
					if( Double::TryParse(rd, d) )
						im[n].rep = (int)d;
					continue;
				}
				if( rd->StartsWith("x = ") && n!=-1 ){
					rd = rd->Remove(0,4);
					if( Double::TryParse(rd, d) ){
						im[n].x = (float)d;
						if( im[n].x + wid > imag->Width )
							im[n].x = imag->Width - wid;
					}
					continue;
				}
				if( rd->StartsWith("y = ") && n!=-1 ){
					rd = rd->Remove(0,4);
					if( Double::TryParse(rd, d) ){
						im[n].y = (float)d;
						if( im[n].y + hei > imag->Height )
							im[n].y = imag->Height - hei;
					}
					continue;
				}
				if( rd->Equals("adatta") && n!=-1 )
					continue;
				if( rd->Equals("ritaglia") && n!=-1 ){
					im[n].adatta = false;
					im[n].ritaglia = true;
					continue;
				}

				if(File::Exists(rd)){
					if(n==max_image-1){
						strdr->Close();
						return 2;
					}
					im[++n].name = rd;
					im[n].x = 0;
					im[n].y = 0;
					im[n].rep = 1;
					im[n].adatta = false;
					im[n].ritaglia = false;
					imag = Image::FromFile(im[n].name);
					continue;
				}
				else
					allvalid = false;
			}
			strdr->Close();
			if(allvalid) return 0;
			return 1;
		}

};