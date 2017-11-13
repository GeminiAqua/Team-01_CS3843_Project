// DecryptData.cpp
//
// THis file uses the input data and key information to decrypt the input data
//

#include "Main.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// code to decrypt the data as specified by the project assignment
int decryptData(char *data, int dataLength)
{
	int resulti = 1;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	// You can not declare any local variables in C, but should use resulti to indicate any errors
	// Set up the stack frame and assign variables in assembly if you need to do so
	// access the parameters BEFORE setting up your own stack frame
	// Also, you cannot use a lot of global variables - work with registers

	__asm {

        xor ecx, ecx;
        mov edi, data;

		LOOP_A1 :
        mov al, byte ptr[edi + ecx];

    // Part A                                   (#A) rotate 1 bit to right        
        rol al, 1;					            // rotate bits of al register left (data addy)

    // Part E
        xor edx, edx;                           // set edx = 0;
        mov dl, al;                             // copy al to dl
        mov al, byte ptr[gDecodeTable + edx];   // copy decode to al

    //Part C                                    (#C) reverse bit order
        xor ebx, ebx;		                    //clears the ebx register
        xor edx, edx;		                    //clears the edx register
        clc;                                    // clear carry flag
    LOOP_REV:
        shl al, 1;		                        //logical shift left on al, sets carry flag
        rcr dl, 1;		                        //move value in carry flag into bl
        inc ebx;			                    //decrement ebx
        cmp ebx, 7;		                        //compare ebx to 1
        jle LOOP_REV;	                        //if ebx is less than or equal to 7, re-loop LOOP_REV

        mov al, dl;                             // move reversed 8-bit section back into al

    //Part B                                    (#B) swap nibbles
        ror al, 4;                              // swap nibbles

	//Part D                                    (#D) swap half nibbles
		//lower 4 bits
        mov bl, al;		                        //bl has contents of al
        and al, 0x0F;	                        //and the first 4 bits of al by f, 1111
        mov dl, al;		                        //cl has contents of al
        and al, 0x03;	                        //and bits 1 and 2 of al by 3, 0011
        and dl, 0x0C;	                        //and bits 3 and 4 of cl by c, 1100
        SHL al, 2;		                        //shift al left by 2
        SHR dl, 2;		                        //shift cl right by 2
        or al, dl;		                        //combine al and cl together				al has final 4 bits

		//upper 4 bits
        and bl, 0xF0;	                        //and the last 4 bits of bl by F0, 1111 0000
        mov dl, bl;		                        //cl has last 4 bits of bl
        and bl, 0x30;	                        //and bits 5 and 6 by 3, 0011 0000
        and dl, 0xC0;                           //and bits 7 and 8 by c, 1100 0000
        shl bl, 2;		                        //shift bl left by 2
        shr dl, 2;		                        //shift cl right by 2
        or bl, dl;		                        //combine bl and cl toghether

        or al, bl;		                        //combine al and bl toghether 
        
        // CHECK
        mov byte ptr[edi + ecx], al;            // move 8-bit al back into destination addy
        add ecx, 8;                             // increment ecx counter
        // inc ecx;                             // is it supposed to be by 1 or 8?!?!?
        /*mov ebx, dataLength;                    // move dataLength into ebx
        cmp ecx, ebx;					        // checks if end of dataLength is reached, it not
        //*/
        cmp ecx, dataLength;
        jl LOOP_A1;			        	        // re-loop LOOP_A1
	}

	return resulti;
} // decryptData

