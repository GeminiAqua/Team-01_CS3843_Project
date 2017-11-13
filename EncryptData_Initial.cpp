// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to encrypt the data as specified by the project assignment
int encryptData(char *data, int dataLength)
{
	int resulti = 1;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	// You can not declare any local variables in C, but should use resulti to indicate any errors
	// Set up the stack frame and assign variables in assembly if you need to do so
	// access the parameters BEFORE setting up your own stack frame
	// Also, you cannot use a lot of global variables - work with registers

    // you will need to reference some of these global variables
    // (gptrPasswordHash or gPasswordHash), (gptrKey or gkey), gNumRounds

	__asm 
	{
        lea esi, gkey;				    // put the ADDRESS of gkey into esi
        mov esi, gptrKey;			    // put the ADDRESS of gkey into esi (since *gptrKey = gkey)
        lea	esi, gPasswordHash;		    // put ADDRESS of gPasswordHash into esi
        mov esi, gptrPasswordHash;	    // put ADDRESS of gPasswordHash into esi (since unsigned char *gptrPasswordHash = gPasswordHash)
        mov edi, data;                  // put ADDRESS of data into edi
        xor ecx, ecx;                   // set ecx = 0 for loop counter

    //PART D                            (#D) swap half nibbles
        /*
        rotate 4 bits by 2 to swap the half-nibbles. 
        Before this you'll have to separate the 8 bits to 2 nibbles. This can be done with bit masking,
        bitwise AND and shifting. Then you can assemble the final result using bitwise OR.
        //*/
        mov al, byte ptr[edi + ecx];    // move data address to al register
    LOOP_A1:
		//lower 4 bits
        mov bl, al;		                        //bl has contents of al
        and al, 0x0F;		                    //AND the first 4 bits of al by Fh (1111)
        mov dl, al;		                        //cl has contents of al
        and al, 0x03;		                    //AND bits 1 and 2 of al by 3, 0011
        and dl, 0x0C;		                    //AND bits 3 and 4 of cl by c, 1100
        shl al, 2;		                        //shift al left by 2
        shr dl, 2;		                        //shift cl right by 2
        or al, dl;		                        //combine al and dl together				al has final 4 bits

		//upper 4 bits
        and bl, 0xF0;		                    //and the last 4 bits of bl by F0, 1111 0000
        mov dl, bl;                             //cl has last 4 bits of bl
        and bl, 0x30;		                    //and bits 5 and 6 by 3, 0011 0000
        and dl, 0xC0;                           //and bits 7 and 8 by c, 1100 0000
        shl bl, 2;		                        //shift bl left by 2
        shr dl, 2;		                        //shift cl right by 2
        or bl, dl;		                        //combine bl and cl toghether
        or al, bl;		                        //combine al and bl toghether

	//Part B                                    (#B) swap nibbles
        ror al, 4;                              // swap nibbles
        
    //Part C                                    (#C) reverse bit order
        xor ebx, ebx;                           //set ebx = 0 for 8-bit counter
        // xor edx, edx;	                    //clears the edx register
	    //mov ebx, 8	            	        //setting ebx to 8 to run loop rev 8 times
        clc;                                    // clear carry flag
	LOOP_REV:
        shl al, 1;                              //logical shift left on al, puts MSB into carry flag
        rcr dl, 1;		                        //move value in carry flag into dl
        inc ebx;			                    //increment ebx
        cmp ebx, 7;		                        //compare ebx to 7
        jle LOOP_REV;	                        //if ebx is less than or equal to 7, re-loop LOOP_REV

    //Part E                                    (#E) code table swap
        xor edx, edx;                           // set edx = 0
        mov dl, al;                             // copy current 8-bit into dl
        mov al, byte ptr[gEncodeTable + edx];   // copy code to al

    //Part A                                    (#A) rotate 1 bit to right        
        ror al, 1;					            // rotate bits of al register right (data addy)
        
    // CHECK
        mov byte ptr[edi + ecx], al;            // move 8-bit al back into destination addy
        add ecx, 8;                             // increment ecx counter
        //inc ecx;                                // is it supposed to be by 1 or 8?!?!?
        /*mov ebx, dataLength;                    // move dataLength into ebx
        cmp ecx, ebx;					        // checks if end of dataLength is reached, it not
        //*/
        cmp ecx, dataLength;
        jl LOOP_A1;			        	        // re-loop LOOP_A1
	}

	return resulti;
} // encryptData