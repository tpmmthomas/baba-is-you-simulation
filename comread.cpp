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
	const char* device = "\\\\.\\COM8";
	
	uint32_t baud_rate = 9600;
	
	HANDLE port  = open_serial_port(device,baud_rate);
	if (port == INVALID_HANDLE_VALUE) { return 1; }
	uint8_t* buffer = (uint8_t*)malloc(1*sizeof(uint8_t));
	uint8_t* buffer2 = (uint8_t*)malloc(1*sizeof(uint8_t));
	memset(buffer,0,1*sizeof(uint8_t));
	int temp = 2;
	while(1){
		char x;
		cin>>x;
		int x2= 0;
		switch(x){
			case 's':
				x2 = 0xF1;
				break;
			case 'a':
				x2 = 0xF2;
				break;
			case 'd':
				x2 = 0xF3;
				break;
			case 'w':
				x2 = 0xF4;
				break;	
			case '1':
				x2 = 1;
				break;
			case '2':
				x2 = 2;
				break;
		}
		memset(buffer,x2,1*sizeof(uint8_t));
		int n = write_port(port,buffer,1);
		if(n==0) printf("Success\n");
		else printf("Failed\n");
		n = read_port(port,buffer2,1);
		if(n>0){
			for(int i=7;i>=0;i--){
			if((buffer2[0]>>i) & 1)
				printf("1");
			else
				printf("0");
			}
			printf("\nSuccessful read %d bytes.\n",n);
		}
		else cout<<"Timeout\n";
		
	}
	CloseHandle(port);
  	return 0;
}

