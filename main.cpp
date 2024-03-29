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

void readSensor(short int msb,short int lsb, short int index, short int data){
    outb(msb, index);
    int t1 = inb(data);
    outb(lsb, index);
    int t2 = inb(data);
    printf("%d\n",t1 << 8 | t2);  
}

int main(){

    short int EFIR = 0x4E;
    short int EFDR = 0x4F;
    short int base_address;
    short int t1,t2;

    if (ioperm(EFIR, 2, 1))
       {perror("ioperm error");return 0;}
    
    outb(0xaa,EFIR);

    //enter extended function mode
    printf("Entering extended mode\n");
    outb(0x87,EFIR);
    outb(0x87,EFIR);

    //read device ID
    printf("Reading device ID\n");
    outb(0x20,EFIR);
    t1 = inb(EFDR);
    outb(0x21,EFIR);
    t2 = inb(EFDR);
    printf("%x\n",t1 << 8 | t2);  

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

    /*printf("Reading raw CPU fan value\n");
    //3 means it's 31.3 khz output frequency
    int cpu_fan_value_offset = 0x03;
    int cpu_fan_value;
    outb(index+cpu_fan_value_offset,EFIR);
    cpu_fan_value = inb(EFDR);
    printf("%x\n",cpu_fan_value);

    printf("Reading CPU fan clock divider signal\n");
    //3 means it's 31.3 khz output frequency
    int cpu_fan_divider_offset = 0x02;
    outb(index+cpu_fan_divider_offset,EFIR);
    int cpu_fan_divider = inb(EFDR);
    printf("%x\n",cpu_fan_divider);*/

    //Sensor base address
    short int index = base_address + 5;
    short int data = base_address + 6;

    if (ioperm(index, 2, 1))
       {perror("ioperm error");return 0;}

    printf("Select bank 4 to read CPU RPM\n");
    int bank_set_offset = 0x4e;
    outb(0x4e ,index);
    outb(0x04,data);
  
    printf("Reading CPU fan RPM\n");
    readSensor(0xC2,0xC3,index,data);

    printf("Reading Sys fan RPM\n");
    readSensor(0xC0,0xC1,index,data); 

    printf("Reading AUX0 fan RPM\n");
    readSensor(0xC4,0xC5,index,data);  

     printf("Reading AUX1 fan RPM\n");
    readSensor(0xC6,0xC7,index,data);  

     printf("Reading AUX2 fan RPM\n");
    readSensor(0xC8,0xC9,index,data);  

     printf("Reading AUX3 fan RPM\n");
    readSensor(0xCA,0xCB,index,data);  

     printf("Reading AUX4 fan RPM\n");
    readSensor(0xCE,0xCF,index,data);                  

    //exit extended function mode
    printf("Exit extended mode\n");
    outb(0xaa,EFIR);

    return 0;
}