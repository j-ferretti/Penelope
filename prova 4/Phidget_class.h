// define
#define id008 157825
#define coil1 0
#define coil2 1
#define ENABLE 0
#define DISABLE 1
#define swcoil 2
#define BACKWARD 0
#define FORWARD 1
#define papsensor 3

#define id888 171483
#define front 0
#define back 1

#define idASC 169838
#define servon 7
#define smax 21
#define smin 2
#define stmax 8
#define stmin 0

using namespace std;
int __stdcall at(CPhidgetHandle phid, void *pt);
int __stdcall inchange888(CPhidgetInterfaceKitHandle IFK, void *pt, int Index, int State);
int __stdcall ASCposchange(CPhidgetAdvancedServoHandle phid, void *userPtr, int index, double position);
int __stdcall inchange888_v2(CPhidgetInterfaceKitHandle IFK, void *pt, int Index, int State);


public class phidclass{

public:
	phidclass(void){
		initialize();		
	}
//protected:
public:
	~phidclass(){
		ced();
	}

public:
		CPhidgetInterfaceKitHandle phid888;
		CPhidgetInterfaceKitHandle phid008;
		CPhidgetAdvancedServoHandle phidASC;
		bool ready;
		double posizione;
		double pmin,pmax;
		bool switch_888;
		

void disable_motor(){
	CPhidgetInterfaceKit_setOutputState(phid008,coil1,DISABLE);
	CPhidgetInterfaceKit_setOutputState(phid008,coil2,DISABLE);
	return;
}
void enable_motor(){
	CPhidgetInterfaceKit_setOutputState(phid008,coil1,ENABLE);
	CPhidgetInterfaceKit_setOutputState(phid008,coil2,ENABLE);
	return;
}
void bypass_page_sensor(){
	CPhidgetInterfaceKit_setOutputState(phid008,papsensor,false);
	Sleep(1500);
	CPhidgetInterfaceKit_setOutputState(phid008,papsensor,true);
	return;
}

void move_servo(double pos){
	if(pos<pmin || pos>pmax) return;
	CPhidgetAdvancedServo_setEngaged(phidASC, servon, true);
	CPhidgetAdvancedServo_setPosition(phidASC, servon, pos);
	while(posizione!=pos){Sleep(100);}
	Sleep(100);
	CPhidgetAdvancedServo_setEngaged(phidASC, servon, false);
	return;
}
void test_servo(){
	CPhidgetAdvancedServo_setEngaged(phidASC, servon, true);
	CPhidgetAdvancedServo_setPosition(phidASC, servon, smax);
	Sleep(4000);
	CPhidgetAdvancedServo_setPosition(phidASC, servon, smin);
	Sleep(4000);
	CPhidgetAdvancedServo_setEngaged(phidASC, servon, false);
	return;

}

void initialize(){
	posizione=0;
	pmin=smin;
	pmax=smax;
	switch_888=false;

	CPhidgetInterfaceKit_create(&phid888);
	CPhidget_open((CPhidgetHandle)phid888,id888);

	CPhidgetInterfaceKit_create(&phid008);
	CPhidget_open((CPhidgetHandle)phid008,id008);

	CPhidgetAdvancedServo_create(&phidASC);
	CPhidget_open((CPhidgetHandle)phidASC,idASC);

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)phid888, at, NULL);
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)phid008, at, NULL);
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)phidASC, at, NULL);
	
	CPhidgetInterfaceKit_set_OnInputChange_Handler(phid888,inchange888,this);

	CPhidgetInterfaceKit_setOutputState(phid008, papsensor, false);
	CPhidgetInterfaceKit_setOutputState(phid008, swcoil, FORWARD);
	//inchange888(phid888,this,0,0);
	CPhidgetAdvancedServo_set_OnPositionChange_Handler(phidASC, ASCposchange,this);

	enable_motor();

	//è inutile mettere qui il movimento del servo, non funziona... deve essere richiamato successivamente
}

void switch_888_handler(){
	if(switch_888){
		CPhidgetInterfaceKit_set_OnInputChange_Handler(phid888,inchange888,this);
		switch_888=false;
		return;
	}
	CPhidgetInterfaceKit_set_OnInputChange_Handler(phid888,inchange888_v2,this);
	switch_888=true;
	return;
}

void ced(){

	CPhidget_close((CPhidgetHandle)phid888);
	CPhidget_delete((CPhidgetHandle)phid888);

	CPhidget_close((CPhidgetHandle)phid008);
	CPhidget_delete((CPhidgetHandle)phid008);

	CPhidget_close((CPhidgetHandle)phidASC);
	CPhidget_delete((CPhidgetHandle)phidASC);

	return;
}
void ced(int *e){

	int er;
	*e=0;

	er=CPhidget_close((CPhidgetHandle)phid888);
	if (er!=0) *e=1;
	er=CPhidget_delete((CPhidgetHandle)phid888);
	if (er!=0) *e=1;

	er=CPhidget_close((CPhidgetHandle)phid008);
	if (er!=0) *e=2;
	er=CPhidget_delete((CPhidgetHandle)phid008);
	if (er!=0) *e=2;

	er=CPhidget_close((CPhidgetHandle)phidASC);
	if (er!=0) *e=3;
	er=CPhidget_delete((CPhidgetHandle)phidASC);
	if (er!=0) *e=3;

	return;
}

void initialize(int *e){

	int er;
	*e=0;
	er=0;

	er = CPhidgetInterfaceKit_create(&phid888);
	if (er!=0) *e = 1;
	er=CPhidget_open((CPhidgetHandle)phid888,id888);
	if (er!=0) *e = 1;

	er = CPhidgetInterfaceKit_create(&phid008);
	if (er!=0) *e = 2;
	er=CPhidget_open((CPhidgetHandle)phid008,id008);
	if (er!=0) *e = 2;

	er = CPhidgetAdvancedServo_create(&phidASC);
	if (er!=0) *e = 3;
	er=CPhidget_open((CPhidgetHandle)phidASC,idASC);
	if (er!=0) *e = 3;

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)phid888, at, NULL);
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)phid008, at, NULL);
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)phidASC, at, NULL);

	
	CPhidgetInterfaceKit_set_OnInputChange_Handler(phid888,inchange888,this);

	CPhidgetInterfaceKit_setOutputState(phid008, papsensor, false);
	CPhidgetInterfaceKit_setOutputState(phid008, swcoil, BACKWARD);
	inchange888(phid888,this,0,0);

	enable_motor();
	
	CPhidgetAdvancedServo_setEngaged(phidASC, servon, true);
	CPhidgetAdvancedServo_setPosition(phidASC, servon, smax);
	Sleep(4000);
	CPhidgetAdvancedServo_setEngaged(phidASC, servon, false);

	return;
}

};


int __stdcall at(CPhidgetHandle phid, void *pt){
	char *ty;
	int id;
	CPhidget_getDeviceType(phid,(const char **)&ty); 
	CPhidget_getSerialNumber(phid,&id);
	if(id==idASC){
		CPhidgetAdvancedServo_setServoType((CPhidgetAdvancedServoHandle)phid,7,PHIDGET_SERVO_FIRGELLI_L12_50_100_06_R);
	}
	return 0;
}

int __stdcall inchange888(CPhidgetInterfaceKitHandle IFK, void *pt, int Index, int State){
	phidclass *ob = (phidclass *)pt;

	if(State == 0) return 0;
	if(Index ==	front){
		CPhidgetInterfaceKit_setOutputState(ob->phid008,swcoil,BACKWARD);
	}
	if(Index ==	back){
		ob->disable_motor();
		CPhidgetInterfaceKit_setOutputState(ob->phid008,swcoil,FORWARD);
		ob->bypass_page_sensor();
		Sleep(2500);
		//Sleep(400);
		ob->bypass_page_sensor();
		ob->enable_motor();
		ob->ready=true;
	}
	return 0;
}

int __stdcall ASCposchange(CPhidgetAdvancedServoHandle phid, void *userPtr, int index, double position){
	phidclass *ob = (phidclass *)userPtr;
	if(index!=7) return 0;
	ob->posizione=position;
	return 0;
}

int __stdcall inchange888_v2(CPhidgetInterfaceKitHandle IFK, void *pt, int Index, int State){
	phidclass *ob = (phidclass *)pt;

	if(State == 0) return 0;
	ob->disable_motor();
	if(Index ==	front)
		CPhidgetInterfaceKit_setOutputState(ob->phid008,swcoil,BACKWARD);
	else
		CPhidgetInterfaceKit_setOutputState(ob->phid008,swcoil,FORWARD);
	ob->bypass_page_sensor();
	Sleep(2500);
	ob->bypass_page_sensor();
	ob->enable_motor();
	ob->ready=true;

	return 0;
}