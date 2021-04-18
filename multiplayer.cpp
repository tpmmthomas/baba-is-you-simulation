#include<windows.h>
#include<bits/stdc++.h>
using namespace std;


void print_error(const char * context)
{
  DWORD error_code = GetLastError();
  char buffer[256];
  DWORD size = FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
    NULL, error_code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
    buffer, sizeof(buffer), NULL);
  if (size == 0) { buffer[0] = 0; }
  fprintf(stderr, "%s: %s\n", context, buffer);
}
 
// Opens the specified serial port, configures its timeouts, and sets its
// baud rate.  Returns a handle on success, or INVALID_HANDLE_VALUE on failure.
HANDLE open_serial_port(const char * device, uint32_t baud_rate)
{
  HANDLE port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (port == INVALID_HANDLE_VALUE)
  {
    print_error(device);
    return INVALID_HANDLE_VALUE;
  }
 
  // Flush away any bytes previously read or written.
  BOOL success = FlushFileBuffers(port);
  if (!success)
  {
    print_error("Failed to flush serial port");
    CloseHandle(port);
    return INVALID_HANDLE_VALUE;
  }
 
  // Configure read and write operations to time out after 100 ms.
  COMMTIMEOUTS timeouts = { 0 };
  timeouts.ReadIntervalTimeout = 0;
  timeouts.ReadTotalTimeoutConstant = 100;
  timeouts.ReadTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant = 100;
  timeouts.WriteTotalTimeoutMultiplier = 0;
 
  success = SetCommTimeouts(port, &timeouts);
  if (!success)
  {
    print_error("Failed to set serial timeouts");
    CloseHandle(port);
    return INVALID_HANDLE_VALUE;
  }
 
  DCB state;
  state.DCBlength = sizeof(DCB);
  success = GetCommState(port, &state);
  if (!success)
  {
    print_error("Failed to get serial settings");
    CloseHandle(port);
    return INVALID_HANDLE_VALUE;
  }
 
	state.BaudRate = baud_rate;
	state.ByteSize = 8;
	state.StopBits = 1;
	state.Parity = NOPARITY; 
  success = SetCommState(port, &state);
  if (!success)
  {
    print_error("Failed to set serial settings");
    CloseHandle(port);
    return INVALID_HANDLE_VALUE;
  }
 
  return port;
}
 
// Writes bytes to the serial port, returning 0 on success and -1 on failure.
int write_port(HANDLE port, uint8_t * buffer, size_t size)
{
  DWORD written;
  BOOL success = WriteFile(port, buffer, size, &written, NULL);
  if (!success)
  {
    print_error("Failed to write to port");
    return -1;
  }
  if (written != size)
  {
    print_error("Failed to write all bytes to port");
    return -1;
  }
  return 0;
}
 
// Reads bytes from the serial port.
// Returns after all the desired bytes have been read, or if there is a
// timeout or other error.
// Returns the number of bytes successfully read into the buffer, or -1 if
// there was an error reading.
SSIZE_T read_port(HANDLE port, uint8_t * buffer, size_t size)
{
  DWORD received;
  BOOL success = ReadFile(port, buffer, size, &received, NULL);
  if (!success)
  {
    print_error("Failed to read from port");
    return -1;
  }
  return received;
}

int main()
{
	int n;
	string device1 ="\\\\.\\COM", device2="\\\\.\\COM";
	string temp;
	cout<<"Enter Com number of device 1 (numbers only): ";
	cin>>temp;
	device1 += temp;
	cout<<"Enter Com number of device 2 (numbers only): ";
	cin>>temp;
	device2 += temp;
	uint32_t baud_rate = 9600;
	HANDLE port1  = open_serial_port(device1.c_str(),baud_rate);
	HANDLE port2  = open_serial_port(device2.c_str(),baud_rate);
	cout<<"OK\n";
	if (port1 == INVALID_HANDLE_VALUE) { return 1; }
	if (port2 == INVALID_HANDLE_VALUE) { return 1; }
	uint8_t* buffer = (uint8_t*)malloc(1*sizeof(uint8_t));
	uint8_t* buffer2 = (uint8_t*)malloc(1*sizeof(uint8_t));
	memset(buffer,1,1*sizeof(uint8_t));
	memset(buffer2,2,1*sizeof(uint8_t));
	n = write_port(port1,buffer,1);
	if(n==0) cout<<"Successful write to device 1.\n";
	else cout<<"Failed write to device 1.\n";
	n = write_port(port2,buffer2,1);
	if(n==0) cout<<"Successful write to device 2.\n";
	else cout<<"Failed write to device 2.\n";
	while(1){
		n = read_port(port1,buffer,1);
		if(n>0){
			cout<<"Action from device 1 detected.\n";
			n = write_port(port2,buffer,1);
			if(n==0) cout<<"Successful write to device 2.\n";
			else cout<<"Failed write to device 2.\n";
		}
		n = read_port(port2,buffer2,1);
		if(n>0){
			cout<<"Action from device 2 detected.\n";
			n = write_port(port1,buffer2,1);
			if(n==0) cout<<"Successful write to device 1.\n";
			else cout<<"Failed write to device 1.\n";
		}
		if(*buffer == 0xEE || *buffer2 == 0xEE){
			break;
		}
	}
	CloseHandle(port1);
	CloseHandle(port2);
  	return 0;
}

