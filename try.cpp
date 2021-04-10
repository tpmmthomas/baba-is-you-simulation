#include<windows.h>
#include<bits/stdc++.h>
int main(){
DWORD cbNeeded = 0,cReturned = 0;

EnumPortsA(0,1,0,0,&cbNeeded,&cReturned); // Detection of the necessary memory amount
if (cbNeeded)

{

unsigned char* info = new unsigned char[cbNeeded];

memset(info,0,cbNeeded);

if(EnumPorts(0,1,info,cbNeeded,&cbNeeded,&cReturned ) ) // Reading the information about ports

{

PORT_INFO_1* pi = (PORT_INFO_1*)info; // filling the structure

while(1)

{

	string str = pi->pName; // Port name

	if(str.Pos("COM"))

	{

		Strings->Add(str.Delete(str.Length(),1)); //adding the port to the list

}

pi ++;

}

delete []info;

}

}	

	
	
}
