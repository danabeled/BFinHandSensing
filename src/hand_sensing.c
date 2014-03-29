#include <stdio.h>
#include "startup.h"
#include "hand_sensing.h"
#include "charger.h"
#include "cdefBF52x_base.h"

void handSensing(){
	if(fpga_outputToPortFEnable(TLLXBV1_INPUT_PINS)<0){
		printf("failed to enable FPGA Output pins\r\n");
		exit(-1);
	}
    charger_t charger;
    charger_init(&charger);
    charger_debug_enable();
    while(1){
    	printf("******New Measurement******\r\n");
        charger_run(&charger);
        charger.newDataFlag = 0;
    }
}
