#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>

/*
Found `Nuvoton NCT6795D Super IO Sensors'              
    (address 0xa20, driver `nct6775')

name => "Nuvoton NCT6795D Super IO Sensors",
driver => "nct6775",
devid => 0xD350,
devid_mask => 0xFFF8,
logdev => 0x0b,
features => FEAT_IN | FEAT_FAN | FEAT_TEMP,



	# Switch to the sensor logical device
	outb($addrreg, $superio{logdevreg});
	outb($datareg, $chip->{logdev});

	# Get the IO base address
	outb($addrreg, $superio{basereg_msb});
	$addr = inb($datareg);
	outb($addrreg, $superio{basereg_lsb});
	$addr = ($addr << 8) | inb($datareg);


*/

int main(){

    short int EFIR = 0x2E;
    short int EFDR = 0x2F;
    int base_address;
    short int t1,t2;

    if (ioperm(EFIR, 2, 1))
       {perror("ioperm error");return 0;}
    
    //enter extended function mode
    printf("Entering extended mode\n");
    outb(0x87,EFIR);
    outb(0x87,EFIR);

    //configure the configuration registers
    //Fan monitor is logical device B
    printf("Select logical device B\n");
    outb(0x07,EFIR);//Set Device number register
    outb(0x0B,EFDR); //Select Device "B"



    //get base addresss of hardware monitor device B
    // from CR[60h] & CR[61h]
    printf("Reading hardware monitor base address\n");
    outb(0x60,EFIR);
    t1 = inb(EFDR);
    outb(0x61,EFIR);
    t2 = inb(EFDR);
    base_address = t1 << 8 | t2;
    printf("base_address: %x\n",base_address);


    //exit extended function mode
    printf("Exit extended mode\n");
    outb(0xaa,EFIR);

    return 0;
}