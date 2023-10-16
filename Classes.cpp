#include <iostream>
#include "myconsole.h"
#include "mygraphics.h"
#include <Windows.h>
#include <string>
#include <string.h>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <ctime>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
			
using namespace std;

//Patterns used: Singleton pattern and Factory Method pattern
//Also made sure to increase flexibility by using generalized functions


void readingFile(string fileArr[20],int& totalCount,string filename)	//Populates given array from given filename
{
	string line;
	ifstream readfile(filename);
	int count=0;
	while(getline(readfile,line))
	{
		fileArr[count]=line;
		count++;
	}
	totalCount=count;
}		

void saveOutlet(string outletid,string location,int status)		//Saves single outlet in file
{
	ofstream writefile("Outlets.txt");
	writefile<<outletid<<","<<location<<","<<status<<endl;
	writefile.close();
}

bool compareStr(string str1,string str2)	//Comapares 2 strings
{
	bool same=true;
	int l1,l2,size;
	l1=str1.size();
	l2=str2.size();
	if(l1!=l2)
		return false;

	for(int i=0;i<l2;i++)
	{
		if(str1[i]!=str2[i])
			same=false;
	}
	return same;
}

bool checkStaffAvailability()		//Tells if any staff available or not
{
	bool anyoneFree=false;
	string line;
	ifstream readfile("staffAvailable.txt");
	while(getline(readfile,line))
	{
		if(line[0]=='1')
		{
			anyoneFree=true;
			break;
		}
	}
	return anyoneFree;
}

bool getDetail(string id,string filename,string detailStr[],int& totalBookings)		//Give detials about bookings
{
	string line;
	string idstr;
	bool idfound=false;
	ifstream readfile(filename);
	int count=0,bookingcount=0;
	while(getline(readfile,line))
	{
		for(int i=0;line[i]!=',';i++)
		{
			count++;
		}
		idstr=line.substr(0,count);
		if(compareStr(id,idstr)==true)
		{
			detailStr[bookingcount]=line;
			bookingcount++;
			idfound=true;
		}
		count=0;
	}

	totalBookings=bookingcount;
	return idfound;
}

class user			//Customer class
{
	int points;
	int expenditure;
	double mileage;
	double dailyAvg;
	string uname;
	string id;
	string upass;
	string uemail;
	string date;
	string number;
	string *maintenanceHistory;
public:
	user(int pts,int exp,double mil,double dAvg,string name,string ID,string pass,string email,string dt,string num)
	{
		points=pts;
		expenditure=exp;
		mileage=mil;
		dailyAvg=dAvg;
		uname=name;
		id=ID;
		upass=pass;
		uemail=email;
		date=dt;
		number=num;
	}
	void viewDashboard()
	{
		string bookingDetail[10];
		int totalBookings=0;

		cout<<"------Personal Details------"<<endl;
		cout<<"    ID    :"<<id<<endl;
		cout<<"   Name   :"<<uname<<endl;
		cout<<"   Email  :"<<uemail<<endl;
		cout<<"   Phone  :"<<number<<endl;

		cout<<endl;
		cout<<"------Statistical Details-------"<<endl;
		cout<<"  Points   :"<<points<<endl;
		cout<<" Mileage   :"<<mileage<<endl;
		cout<<"Expenditure:"<<expenditure<<endl;
		
		cout<<endl;
		if(getDetail(id,"Bookings.txt",bookingDetail,totalBookings)==true)
		{
			cout<<"-----Bookings-----"<<endl;

			cout<<"Total bookings:"<<totalBookings<<endl;
			for(int i=0;i<totalBookings;i++)
			{
				cout<<bookingDetail[i]<<endl;
			}
			cout<<endl;
		}
		else
		{
			cout<<"No bookings yet"<<endl;
		}
	}
	void bookAppointment()
	{
		int option,choice;
		bool staffAvailable;
		string paymentoption,bookingdetails,bookingArr[50];

		staffAvailable=checkStaffAvailability();		//Checking staff availability

		if(staffAvailable==true)
		{
			cout<<"--Enter your preferred payment method--"<<endl;		//taking payment method
			cout<<"1-Online Payment"<<endl;
			cout<<"2-Cash Payment"<<endl;
			cin>>option;
			if(option==1)
			{
				paymentoption="Online";
			}
			else if(option==2)
			{
				paymentoption="Cash";
			}

			std::time_t t = std::time(0);		// get time now
			std::tm* now = std::localtime(&t);

			cout<<"Booking time:";
			std::cout << now->tm_mday << '-'
				<< (now->tm_mon + 1) << '-'
				<< (now->tm_year + 1900)
				<< "\n";

			string time;

			std::stringstream ss1;
			ss1 << now->tm_mday;
			time = time + ss1.str() + '-';

			std::stringstream ss2;
			ss2 << (now->tm_mon + 1);
			time = time + ss2.str() + '-';

			std::stringstream ss3;
			ss3 << (now->tm_year + 1900);
			time = time + ss3.str();

			bookingdetails= id + "," + paymentoption + "," + time + ",";

			string line;
			ifstream readfile("Bookings.txt");		//Updating bookinngs file to add this booking
			int count=0;
			while(getline(readfile,line))
			{
				bookingArr[count]=line;
				count++;
			}

			bookingArr[count]=bookingdetails;

			ofstream writefile("Bookings.txt");
			for(int i=0;i<count+1;i++)
			{
				writefile<<bookingArr[i]<<endl;
			}
			cout<<"Booking successful!"<<endl;

			cout<<"Press 1 to get prediction cost"<<endl;			//Predicting the cost of work based on mileage
			cin>>choice;
			if(choice==1)
			{
				if(mileage>0 && mileage<1000)
				{
					cout<<"Predicted cost : 5000-9000 Rupees"<<endl;
				}
				else if(mileage>999 && mileage<10000)
				{
					cout<<"Predicted cost : 10,000-25,000 Rupees"<<endl;
				}
				else if(mileage>9999)
				{
					cout<<"Predicted cost : 25,000+ "<<endl;
				}
			}
		}
		else
		{
			cout<<"Sorry! Staff is not available"<<endl;
		}
	}
	void updateMileage()		//Updataes customer's car mileage and edits dashbpoard+file
	{
		string line,cutId,customerLine;
		bool validMil=false;
		double mil;
		string customerArr[20];

		while(validMil==false)		//Make sure mileage is not less than 0
		{
			cout<<"Enter new mileage:";
			cin>>mil;
			if(mil>0)
			{
				validMil=true;
				mileage=mil;
			}
		}

		ifstream readfile("Customers.txt");		//Copying all lines from file in arr
		int count=0;
		while(getline(readfile,line))
		{
			customerArr[count]=line;
			count++;
		}
		readfile.close();

		string workStr;
		int pos;

		for(int i=0;i<count;i++)	
		{
			cutId=customerArr[i].substr(0,3);
			if(compareStr(id,cutId)==true)
			{
				pos=i;							//storing position of string to be updated
				workStr=customerArr[i];			//taking out string to update it
			}
		}


		string updateStr="";
		string firstSub;
		string secondSub;

		std::ostringstream streamObj;		//Converting double to string
		streamObj << mileage;
		std::string milg = streamObj.str();

		int commacount=0;
		int start=0,len;
		bool commafound=false;
		for(int i=0;workStr[i]!='\0';i++)		//taking substrings to update string
		{
			if(workStr[i]==',')
				commacount++;
			if(commacount==2 && commafound==false)
			{
				commafound=true;
				firstSub=workStr.substr(start,i+1);
				updateStr=updateStr + firstSub + milg;
			}
			if(commacount==3)
			{
				start=i;
				secondSub=workStr.substr(start,workStr.size()-start);
				updateStr=updateStr + secondSub;
				break;
			}
		}
		customerArr[pos]=updateStr;		//store updated string in stringArr

		ofstream writeToFile("Customers.txt");
		for(int i=0;i<count;i++)
		{
			writeToFile<<customerArr[i]<<endl;
		}
	}
};

class cashier
{
	string cname;
	string cpass;
public:
	cashier(string cname, string cpass)
	{
		//cout<<"Cashier created"<<endl;
	};
	int recordService(int userId,string userDetail);
};

class staff
{
	string sname;
	int jobsdone;
	int id;
	string spass;
	int leaves;
	int staffType;
public:
	void requestLeave(string leaveRequestArr[]);
};

class floorManager
{
	string fname;
	string femail;
	string fpass;
public:
	floorManager(string fname, string femail, string fpass)
	{
		//cout<<"Floor Manager created"<<endl;
	};
	void checkJobs(int upcomingWorkArr[]);
	void checkPerformance();
	void returnItem(string item);
	void viewStaff(staff staffobjArr[]);
};

class workshopManager
{
	string wname;
	string wemail;
	string wpass;
public:
	workshopManager(string wname, string wemail, string wpass)
	{
		//cout<<"Workshop Manager created"<<endl;
	};
	void manageShifts(int upcomingWorkArr[]);
	void updateLeaveQuotas();
	void handleMaintenanceSchedule();
	void approveLeaves(string leaveReqArr[]);
	int predictJobLoad();
	void viewReports();
};

class workshop
{
	workshopManager WMObj;
public:
	workshop(string wname, string wemail, string wpass): WMObj(wname,wemail,wpass)
	{
		//cout<<"Workshop created"<<endl;
	};
};

class inventoryManager
{
	string name;
	string email;
	int id;
	string pass;
public:
	void allocateInventory(int id);
	void updateReorderLevel(int id);
	void checkStockLevel(int id);
};

class inventory
{
	int stockLevel;
	int reorderLevel;
	int tyres;
	int suspensionKits;
	int windscreens;
	int bumpers;
	int sideMirrors;
	int rims;
public:
	inventory(int stockLevel, int reorderLevel, int tyres, int suspensionKits, int windscreens, int bumpers, int sideMirrors, int rims)
	{
		//cout<<"Inventory created"<<endl;
	};
};

class outlet		//Main class that is composed of other objects but those functionalities aren't used here that much
{
	string outletId;
	string location;
	inventory iObj;
	workshop wObj;
	floorManager fmObj;
	staff* sObj;
	cashier cObj;
	char status;
public:
	outlet(string outletId, string location, int stockLevel, int reorderLevel, int tyres, int suspensionKits, int windscreens, int bumpers, int sideMirrors, int rims, string wname, string wemail, string wpass, string fname, string femail, string fpass, string cname, string cpass): iObj(stockLevel, reorderLevel, tyres, suspensionKits, windscreens, bumpers, sideMirrors, rims), wObj(wname, wemail, wpass), fmObj(fname, femail, fpass), cObj(cname, cpass)
	{
		this->outletId=outletId;
		this->location=location;
		status='1';
	}
	void statusSetter(char status)
	{
		this->status=status;
	}
	void locationSetter(string location)
	{
		this->location=location;
	}
};

class outletAdmin		//Admin who controls all outlets
{
	string oname;
	string oemail;
	string opass;

	static outletAdmin* instance;		//Using singleton pattern as we dont allow more than one outletadmin

	outletAdmin()		//Default constructor to fill outlet data from file
	{
		string line;
		ifstream readfile("outletAdmin.txt");
		getline(readfile, line);
		opass = line;

		getline(readfile, line);
		oname = line;

		getline(readfile, line);
		oemail = line;

		readfile.close();
	}

public:
	static outletAdmin* getInstance()		//Singleton pattern used
	{
		if (instance ==  nullptr) 
		{
			instance = new outletAdmin;
		}
		return instance;
	}
	bool validatePass(string password)		//Tells if the password is valid or not
	{
		if (compareStr(password, opass) == true)
		{
			return true;
		}
		return false;
	}
	outlet* createOutlet(string outletId, string location)		//Outlet's object is made in outlet admin class and is then sent so its factory method
	{
		outlet outletObj(outletId, location, 1, 1, 1, 1, 1, 1, 1, 1, "W", "we", "wp", "f", "fe", "fp", "c", "cp");

		int totalOutlets;
		string outletarr[20], line;
		readingFile(outletarr, totalOutlets, "Outlets.txt");		//Calling function to fill array with outlets from file

		line = outletId + "," + location + ",1";
		outletarr[totalOutlets] = line;
		totalOutlets++;
		for (int i = 0; i < totalOutlets; i++)
		{
			cout << outletarr[i] << endl;
		}

		ofstream writefile("Outlets.txt");		//Writing updated information on file
		for (int i = 0; i < totalOutlets; i++)
		{
			writefile << outletarr[i] << endl;
		}
		writefile.close();

		return &outletObj;
	};
	bool updateOutlet(string id, string location)		//Update the location of the outlet whose id is provided
	{
		bool idfound = false;
		int totalOutlets, len = 0, pos;
		string outletarr[20], line, cutId;
		readingFile(outletarr, totalOutlets, "Outlets.txt");		//Calling function to fill array with outlets from file

		for (int i = 0; i < totalOutlets; i++)		//Loop to find id that has to be updated
		{
			line = outletarr[i];
			for (int j = 0; line[j] != ','; j++)
			{
				len++;
			}
			cutId = line.substr(0, len);
			if (compareStr(cutId, id) == true)
			{
				idfound = true;
				pos = i;
				break;
			}
			else
			{
				len = 0;
			}
		}

		if (idfound == false)
			return false;

		string status, newline;
		int size, start;
		size = line.size();
		int commacount = 0;
		bool commafound = false;
		for (int i = 0; commacount != 2; i++)		//To take status out of the string to use later
		{
			if (line[i] == ',')
				commacount++;
			if (commacount == 2 && commafound == false)
			{
				commafound = true;
				start = i;
			}
		}
		status = line.substr(start + 1, size - start);

		newline = id + "," + location + "," + status;		//Concatenating new line to update outlet details

		outletarr[pos] = newline;
		ofstream writefile("Outlets.txt");		//Writing updated information on file
		for (int i = 0; i < totalOutlets; i++)
		{
			writefile << outletarr[i] << endl;
		}
		writefile.close();
		return idfound;

	}	
	bool deleteOutlet(string id)		//Permanently delete an outlet using its id
	{
		bool idfound = false;
		int totalOutlets, len = 0, pos;
		string outletarr[20], line, cutId;
		string newarr[20];
		readingFile(outletarr, totalOutlets, "Outlets.txt");		//Calling function to fill array with outlets from file

		for (int i = 0; i < totalOutlets; i++)		//Loop to find id that has to be updated
		{
			line = outletarr[i];
			for (int j = 0; line[j] != ','; j++)
			{
				len++;
			}
			cutId = line.substr(0, len);
			if (compareStr(cutId, id) == true)
			{
				idfound = true;
				pos = i;
				break;
			}
			else
			{
				len = 0;
			}
		}

		if (idfound == false)
			return false;

		for (int i = 0, j = 0; i < totalOutlets; i++, j++)
		{
			if (i == pos)
				j++;
			newarr[i] = outletarr[j];
		}


		ofstream writefile("Outlets.txt");		//Writing updated information on file
		for (int i = 0; i < totalOutlets - 1; i++)
		{
			writefile << newarr[i] << endl;
		}
		writefile.close();
		return idfound;
	}
	bool blockOutlet(string id)		//Change the status of outlet to blocked
	{
		int totalOutlets, len = 0, pos;
		string outletarr[20], line, cutId;
		readingFile(outletarr, totalOutlets, "Outlets.txt");		//Calling function to fill array with outlets from file
		bool idfound = false;

		for (int i = 0; i < totalOutlets; i++)		//Loop to find id that has to be updated
		{
			line = outletarr[i];
			for (int j = 0; line[j] != ','; j++)
			{
				len++;
			}
			cutId = line.substr(0, len);
			if (compareStr(cutId, id) == true)
			{
				idfound = true;
				pos = i;
				break;
			}
			else
			{
				len = 0;
			}
		}
		if (idfound == false)
			return false;

		string loc, newline;
		int end, start;
		int commacount = 0;
		bool commafound = false;
		for (int i = 0; commacount != 2; i++)		//To take status out of the string to use later
		{
			if (line[i] == ',')
				commacount++;
			if (commacount == 1 && commafound == false)
			{
				commafound = true;
				start = i;
			}
			else if (commacount == 2)
			{
				end = i;
				break;
			}
		}
		loc = line.substr(start + 1, end - start - 1);

		newline = id + "," + loc + ",0";		//Concatenating new line to update outlet details

		outletarr[pos] = newline;
		ofstream writefile("Outlets.txt");		//Writing updated information on file
		for (int i = 0; i < totalOutlets; i++)
		{
			writefile << outletarr[i] << endl;
		}
		writefile.close();
		return idfound;
	}
	void transferStaff();
	void deactivateAccount(int userId);
	void addInventoryParts();
	void updateWorkingSchedules();
};

void drawcarmenu(int x,int y,int carcolor)	//This function draws the menu where we have to choose our taxi color
{
	COLORREF black = RGB(50,50,50);
	COLORREF tcolor;

	if(carcolor==1)
		tcolor = RGB (29,104,142);		
	else if(carcolor==2)
		tcolor = RGB (200,0,0);			

	myRect(x-50,y-25,x+50,y+25,tcolor,tcolor);		//Draws upper half of car
	myRect(x-100,y,x+100,y+50,tcolor,tcolor);		//Draws lower half of car
	myEllipse(x-55,y+40,x-20,y+75,black,black);		//Draws left tyre of car
	myEllipse(x+20,y+40,x+55,y+75,black,black);		//Draws right tyre of car
}

bool checkCustomer(string id,string pass,string& customerLine)  //Check if customer details are valid
{
	string str;
	string idstr;
	string strpass;
	string foundStr;
	int commasfound=0;
	int startPos,endPos;
	bool idfound=false,validCustomer=false;
	int count=0;
	ifstream ReadCustomer("Customers.txt");
	while(getline(ReadCustomer,str))
	{
		for(int i=0;str[i]!=',';i++)
		{
			count++;
		}
		idstr=str.substr(0,count);
		if(compareStr(id,idstr)==true)
		{
			foundStr=str;
			idfound=true;
			break;
		}
		else
		{
			count=0;
		}
	}
	ReadCustomer.close();

	if(idfound==true)
	{
		for(int i=0;commasfound!=8;i++)
		{
			if(str[i]==',')
			{
				commasfound++;
				if(commasfound==7)
					startPos=i;
				if(commasfound==8)
					endPos=i;
			}
		}

		strpass=str.substr(startPos+1,(endPos-startPos)-1);
		if(compareStr(strpass,pass)==true)
		{
			customerLine=str;
			validCustomer=true;
		}
	}
	return validCustomer;	
}

bool checknewId(string id,string filename)		//Confirms that the id chosen is not already in use
{
	string line;
	string cutId;
	ifstream checkId(filename);
	while(getline(checkId,line))
	{
		cutId=line.substr(0,3);
		if(compareStr(id,cutId)==true)
		{
			checkId.close();
			return false;
		}
	}
	checkId.close();
	return true;
}

void convertVariables(string customerLine,string& cName,string& cEmail,string& cPhone,string& cID,string& lastUpdated,string& cPass,double& cAvg,double& cMileage,int& cPoints,int& expenditure) //Converts variables read from file to their original data types
{
	int start=0,len,commacount=0;
	string subLine;
	int size=customerLine.size();
	for(int i=0;customerLine[i]!='\0';i++)
	{
		if(customerLine[i]==',')
		{
			len=(i-start);
			if(commacount==0)
			{
				cID=customerLine.substr(start,len);
			}
			else if(commacount==1)
			{
				subLine=customerLine.substr(start,len);
				stringstream geek(subLine);
				geek>>cPoints;
			}
			else if(commacount==2)
			{
				subLine=customerLine.substr(start,len);
				stringstream geek(subLine);
				geek>>cMileage;
			}
			else if(commacount==3)
			{
				subLine=customerLine.substr(start,len);
				stringstream geek(subLine);
				geek>>cAvg;
			}
			else if(commacount==4)
			{
				subLine=customerLine.substr(start,len);
				stringstream geek(subLine);
				geek>>expenditure;
			}
			else if(commacount==5)
			{
				cName=customerLine.substr(start,len);
			}
			else if(commacount==6)
			{
				cEmail=customerLine.substr(start,len);
			}
			else if(commacount==7)
			{
				cPass=customerLine.substr(start,len);
			}
			else if(commacount==8)
			{
				lastUpdated=customerLine.substr(start,len);
			}
			start=i+1;
			commacount++;
			if(commacount==9)
			{
				cPhone=customerLine.substr(start,size-(start+1));
				break;
			}
		}
	}
}

void addCustomer(string cName,string cEmail,string cPhone,string cID, string cPass)		//Used to add a new customer to system and file upon signup
{
	string customerArr[20];
	string str;
	ifstream readfile("Customers.txt");
	int count=0;
	while(getline(readfile,str))
	{
		customerArr[count]=str;
		count++;
	}

	string line="";
	line=cID + ",0," +"0.0," + "0.0," + "0," + cName + "," + cEmail + "," + cPass + ",-," + cPhone + ","; 
	customerArr[count]=line;

	ofstream writefile("Customers.txt");
	for(int i=0;i<count+1;i++)
	{
		writefile<<customerArr[i]<<endl;
	}
}

outletAdmin* outletAdmin::instance = nullptr;

int main()
{
	COLORREF grey = RGB(128,128,128);			//Colors defined to be used in graphics
	COLORREF white = RGB(255,255,255);
	COLORREF black = RGB(00,00,00);
	COLORREF text = RGB(64,128,128);
	COLORREF darkgrey = RGB(50,50,50);
	COLORREF green = RGB(0,204,0);
	COLORREF peach = RGB(255,178,102);
	COLORREF yellow = RGB (255,255,0);
	COLORREF red = RGB(255,0,0);
	COLORREF manag = RGB(255,155,0);
	COLORREF custom = RGB(250,65,0);
	COLORREF red1 = RGB(250,16,12);


	for(int i=0;i<200;i=i+2)			//Starting screen
	{
		COLORREF grey = RGB(i,i,i);
		myDrawTextWithFont(220,140,150," SJ Motors ",black,grey);
		Sleep(50);
	}

	drawcarmenu(110,200,1);
	drawcarmenu(850,200,2);

	myDrawText(400,450,10,"Press any key to continue",white,black);
	_getch();
	myDrawText(400,450,10,"Press any key to continue",black,black);
	myDrawTextWithFont(0,140,300," SJ Motors ",black,black);

	inventoryManager IMobj;		//Creating inventory manager
	outlet* outletArr;			//Creating outlet array which will be filled by outletadmin upon calling createOutlet function

	int option;

	//Sleep(1000);

	bool exit1=false;

	while(exit1==false)		//This exit ends the game when true
	{
		system("CLS");
		myDrawTextWithFont(170,0,50,"------ Welcome to SJ Motor System ------",black,white);
		HANDLE myconsole=GetStdHandle(STD_OUTPUT_HANDLE);	//to set our cursor 
		COORD mycursor;

		mycursor.X=0;			
		mycursor.Y=13;
		SetConsoleCursorPosition(myconsole,mycursor);

		myDrawTextWithFont(10,100,30,"1-Customer",custom,black);
		myDrawTextWithFont(10,130,30,"2-Management",manag,black);
		myDrawTextWithFont(10,160,30,"3-Exit System",red1,black);

		string pass;
		string id;
		int choice,choice2;
		bool login=false;


		string cName,cEmail,cPhone,cID,lastUpdated=" ",cPass;
		double cAvg=0.0,cMileage=0.0;
		int cPoints=0,expenditure=0;
		bool validID=false;

		cin>>option;
		system("CLS");
		if(option==2)
		{
			myDrawTextWithFont(200,0,50,"------ Management System ------",black,white);
			myDrawTextWithFont(10,100,30,"1-Outlet Admin",manag,black);
			myDrawTextWithFont(10,130,30,"2-Inventory Manager",manag,black);
			myDrawTextWithFont(10,160,30,"3-Workshop Manager",manag,black);
			myDrawTextWithFont(10,190,30,"4-Floor Manager",manag,black);
			myDrawTextWithFont(10,220,30,"5-Staff",manag,black);
			myDrawTextWithFont(10,250,30,"6-Cashier",manag,black);

			mycursor.X=0;			
			mycursor.Y=20;
			SetConsoleCursorPosition(myconsole,mycursor);
			cin>>choice;
			if(choice==1)
			{
				bool exit2=false;
				while(exit2==false)			//This exit takes us back to admin's menu
				{
					string adminpass;
					bool adminlogin=false;
					cin.ignore();
					while(adminlogin==false)
					{
						system("CLS");
						mycursor.X=0;			
						mycursor.Y=7;
						SetConsoleCursorPosition(myconsole,mycursor);
																			//Using singleton pattern as we dont allow more than one outletadmin
						outletAdmin* OAobj = OAobj->getInstance();			//Creating outlet admin	
						myDrawTextWithFont(200,0,50,"------ Outlet Admin Login ------",black,manag);
						myDrawTextWithFont(5,65,30,"Enter Password : ",white,black);
						getline(cin,adminpass);


						//myDrawTextWithFont(10,250,30,adminpass,manag,black);
						if(OAobj->validatePass(adminpass)==true)
						{
							bool exit100=false;
							while(exit100==false)
							{
								system("CLS");
								mycursor.X=0;			
								mycursor.Y=10;
								SetConsoleCursorPosition(myconsole,mycursor);
								adminlogin=true;
								myDrawTextWithFont(200,0,50,"------ Outlet Admin Menu ------",black,manag);
								myDrawTextWithFont(20,70,30,"1-Create Outlet",white,black);
								myDrawTextWithFont(20,100,30,"2-Delete Outlet",white,black);
								myDrawTextWithFont(20,130,30,"3-Update Outlet",white,black);
								myDrawTextWithFont(320,70,30,"4-Block Outlet",white,black);
								myDrawTextWithFont(320,100,30,"5-View Outlets",white,black);
								myDrawTextWithFont(320,130,30,"6-View Bookings",white,black);
								myDrawTextWithFont(620,70,30,"7-View Customers",white,black);
								myDrawTextWithFont(620,100,30,"8-Logout",white,black);

								cin>>choice2;
								if(choice2==1)
								{
									bool validOutletId=false;
									string outletid,location;
									cin.ignore();
									while(validOutletId==false)
									{
										system("CLS");
										mycursor.X=0;			
										mycursor.Y=7;
										SetConsoleCursorPosition(myconsole,mycursor);
										myDrawTextWithFont(200,0,50,"------ Create Outlet ------",black,manag);
										myDrawTextWithFont(10,70,25,"Enter a 3 digit outlet ID",white,black);
										getline(cin,outletid);
										if(outletid.size()==3)
										{
											validOutletId=true;
										}
				
										if(checknewId(outletid,"Outlets.txt")==true && validOutletId==true)
										{
											validOutletId=true;
										}
										else
										{
											validOutletId=false;
										}
									}
									mycursor.X=0;			
									mycursor.Y=11;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(10,130,25,"Enter outlet's location",white,black);
									getline(cin,location);

									outletArr=OAobj->createOutlet(outletid,location);
								}
								else if(choice2==2)
								{
									cin.ignore();
									string outletid;
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=7;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(200,0,50,"------ Delete Outlet ------",black,manag);
									myDrawTextWithFont(10,70,25,"Enter id of outlet to delete",white,black);
									getline(cin,outletid);
				
									if(OAobj->deleteOutlet(outletid)==false)
										cout<<"Id not found"<<endl;
									else
										cout<<"Outlet with id:"<<outletid<<"  successfully deleted!"<<endl;
								}
								else if(choice2==3)
								{
									cin.ignore();
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=7;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(200,0,50,"------ Update Outlet ------",black,manag);
									myDrawTextWithFont(10,70,25,"Enter id of outlet to update",white,black);
									string outletid,location;
									getline(cin,outletid);

									mycursor.X=0;			
									mycursor.Y=10;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(10,130,25,"Enter outlet's new location",white,black);
									getline(cin,location);

									if(OAobj->updateOutlet(outletid,location)==false)
										cout<<"Id not found"<<endl;
									else
										cout<<"Outlet's location successfully updated!"<<endl;
								}
								else if(choice2==4)
								{
									cin.ignore();
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=7;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(200,0,50,"------ Block Outlet ------",black,manag);
									myDrawTextWithFont(10,70,25,"Enter id of outlet to block",white,black);
									string outletid;
									getline(cin,outletid);

									if(OAobj->blockOutlet(outletid)==false)
										cout<<"Id not found"<<endl;
									else
										cout<<"Outlet with id:"<<outletid<<"  successfully blocked!"<<endl;
								}
								else if(choice2==5)
								{
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=5;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(200,0,50,"------ List of Outlets ------",black,manag);
									cout<<"-ID-Location-Status-"<<endl<<endl;
									string outletarr[20];
									int totalOutlets=0;
									readingFile(outletarr,totalOutlets,"Outlets.txt");
									for(int i=0;i<totalOutlets;i++)
									{
										cout<<outletarr[i]<<endl<<endl;
									}
								}
								else if(choice2==6)
								{
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=5;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(200,0,50,"-----List of Bookings-----",black,manag);
									cout<<endl<<"-ID--Payment Method--Date"<<endl<<endl;
									string bookingArr[20];
									int totalBookings=0;
									readingFile(bookingArr,totalBookings,"Bookings.txt");
									for(int i=0;i<totalBookings;i++)
									{
										cout<<bookingArr[i]<<endl<<endl;
									}
								}
								else if(choice2==7)
								{
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=7;
									SetConsoleCursorPosition(myconsole,mycursor);
									string customerArr[20];
									int totalCustomers=0;
									readingFile(customerArr,totalCustomers,"Customers.txt");
									for(int i=0;i<totalCustomers;i++)
									{
										cout<<customerArr[i]<<endl<<endl;
									}
									myDrawTextWithFont(500,0,50,"-----Customers List-----",black,manag);
								}
								else if(choice2==8)
								{
									exit2=true;
									exit100=true;
								}
								getch();
							}
						}
						else
						{
							myDrawTextWithFont(300,190,25,"Invalid details!",red,black);
							getch();
							myDrawTextWithFont(300,190,25,"Invalid details!",black,black);
						}
					}
				}
			}
		}
		else if(option==1)
		{
			bool exit3=false;
			while(exit3==false)			//This exit takes us back to our main menu
			{
				login=false;
				while(login==false)		//This condition keeps on asking for password until correct one is entered
				{
					system("CLS");
					mycursor.X=0;			
					mycursor.Y=10;
					SetConsoleCursorPosition(myconsole,mycursor);
					myDrawTextWithFont(200,0,50,"-----Customer Account-----",black,white);
					myDrawTextWithFont(10,60,30,"1-Sign In",custom,black);
					myDrawTextWithFont(10,100,30,"2-Sign Up",custom,black);
					myDrawTextWithFont(10,140,30,"3-Log out",custom,black);


					cin>>choice;
					if(choice==1)
					{
						system("CLS");
						mycursor.X=0;			
						mycursor.Y=7;
						SetConsoleCursorPosition(myconsole,mycursor);
						myDrawTextWithFont(200,0,50,"-----Customer Sign in-----",black,custom);
						myDrawTextWithFont(10,70,25,"Enter your id",white,black);
						string customerLine;
						cin.ignore();
						getline(cin,id);


						mycursor.X=0;			
						mycursor.Y=11;
						SetConsoleCursorPosition(myconsole,mycursor);
						myDrawTextWithFont(10,140,25,"Enter your password",white,black);
						getline(cin,pass);

						if(checkCustomer(id,pass,customerLine)==true)
						{
							login=true;
							convertVariables(customerLine,cName,cEmail,cPhone,cID,lastUpdated,cPass,cAvg,cMileage,cPoints,expenditure);

							user uCustomer(cPoints, expenditure, cMileage, cAvg, cName, cID, cPass, cEmail, lastUpdated, cPhone);

							myDrawTextWithFont(10,190,25,"Login succesful!",white,black);
							getch();

							bool exit4=false;
							while(exit4==false)
							{
								system("CLS");
								mycursor.X=0;			
								mycursor.Y=10;
								SetConsoleCursorPosition(myconsole,mycursor);
								myDrawTextWithFont(200,0,50,"------ Customer Menu ------",black,custom);
								myDrawTextWithFont(20,70,30,"1-View Dashboard",white,black);
								myDrawTextWithFont(20,100,30,"2-Update Mileage",white,black);
								myDrawTextWithFont(320,70,30,"3-Book Appointment",white,black);
								myDrawTextWithFont(320,100,30,"4-Log out",white,black);

								cin>>choice2;
								if(choice2==1)
								{
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=5;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(200,0,50,"------ Dashboard ------",black,custom);
									uCustomer.viewDashboard();
								}
								else if(choice2==2)
								{
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=5;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(200,0,50,"------ Update Mileage ------",black,custom);
									uCustomer.updateMileage();
								}
								else if(choice2==3)
								{
									system("CLS");
									mycursor.X=0;			
									mycursor.Y=5;
									SetConsoleCursorPosition(myconsole,mycursor);
									myDrawTextWithFont(300,0,50,"------ Book Appointment ------",black,custom);
									uCustomer.bookAppointment();
								}
								else if(choice2==4)
								{
									exit4=true;
								}
								getch();
							}
						}
						else
						{
							//cout<<"Invalid details"<<endl;
							myDrawTextWithFont(10,190,25,"Invalid details",white,black);
							getch();
							login=false;
							system("CLS");
						}
					}
					else if(choice==2)
					{
						login=true;
						cin.ignore();
						cout<<"Enter your name:";
						getline(cin,cName);
						cout<<endl<<"Enter your email:";
						getline(cin,cEmail);
						cout<<endl<<"Enter your phone number:";
						getline(cin,cPhone);
				
						while(validID==false)
						{
							cout<<endl<<"Enter a 3 digit ID:";
							getline(cin,cID);
							if(cID.size()==3)
							{
								validID=true;
							}
							if(checknewId(cID,"Customers.txt")==true && validID==true)
							{
								validID=true;
							}
							else
							{
								myDrawTextWithFont(300,190,25,"Invalid ID!",red,black);
								getch();
								myDrawTextWithFont(300,190,25,"Invalid ID!",black,black);
								validID=false;
							}
						}
						cout<<endl<<"Choose a password:";
						getline(cin,cPass);
						addCustomer(cName,cEmail,cPhone,cID,cPass);
						cout<<"Signed up successfully!"<<endl;
						getch();
					}
					else if(choice==3)
					{
						login=true;
						//exit4=true;
						exit3=true;
					}
				}
			}
		}
		else if(option==3)
		{
			exit1=true;
		}
	}
	myRect(-10,-10,1000,700,manag,manag);
	myDrawTextWithFont(220,140,150," GoodBye! ",black,manag);
	getch();
	system("pause");
}