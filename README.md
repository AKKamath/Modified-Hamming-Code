# Modified-Hamming-Code

Project implementing the journal paper [Modified Hamming Codes To Improve Burst Error](http://ieeexplore.ieee.org/document/6821090/?reload=true) using C++.

### Basic Outline ###
Classical Hamming Codes are implemented by taking bits that are located at powers of two as redundancy bits.
The value of these bits are calculated as follows:  
Bit 1: (BIT 3) XOR (BIT 5) XOR (BIT 7) XOR (BIT 9)...  
Bit 2: (BIT 3) XOR (BIT 6) XOR (BIT 7) XOR (BIT 10)...  
Bit 4: (BIT 5) XOR (BIT 6) XOR (BIT 7) XOR (BIT 12)...  
For instance, if we were to convert 10011010 to a 12 bit Hamming code, it would be represented as __1_001_1010 where _ have to be filled in with redundancy bits.  
Bit 1 = 1 XOR 0 XOR 1 XOR 1 XOR 1 = 0  
Bit 2 = 1 XOR 0 XOR 1 XOR 0 XOR 1 = 1  
Bit 4 = 0 XOR 0 XOR 1 XOR 0       = 1  
Bit 8 = 1 XOR 0 XOR 1 XOR 0       = 0
Thus, the final codeword would be 011100101010.  

Theoretically, Hamming Codes can correct any 1 bit error that occurs, and detect 2 bit errors. Howerever, practically, we can see that 2 bit burst errors may be incorrectly detected as 1 bit burst errors and corrected. By reordering the bits, we can attempt to maximize the detection of 2 bit and 3 bit errors.

### Implementation Details ###
This project is based on simulating a simple Stop and Wait ARQ to test the error detecting capabilities. The project contains two folder, Original Hamming and Modified Hamming, which implements each respectively. Check.cpp generates Hamming Codes for all 32 bit values and places it into a file Test.txt.  
1 bit, 2 bit and 3 bit burst errors are randomly introduced by the program. The sender displays how many errors are being generated, while the receiver displays the detected errors.  
H.txt contains the parity bit matrix being used. Errors.txt contains the codewords that would be received if 1 bit errors were produced. Checking against this file allows us to fix all 1 bit burst errors. If an error has occured not belonging to this file, then either a 2 bit or 3 bit burst error has occured.  

## Executing the Program ###
Compile both Server.cpp and Client.cpp. Run the executable for server, then for client. The server will ask which file to send to the client. The client then stores the transferred file as Out.txt.
