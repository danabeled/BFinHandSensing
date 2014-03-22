#include <stdio.h>
#include "hand_sensing.h"
#include "charger.h"

void handSensing(){
    charger_t charger;
    charger_init(&charger);
    while(1){
    	printf("testing\n\r");
        charger_run(&charger);
        printf("x time: %d; y time: %d; z time: %d\n\r",
                charger.xTime, charger.yTime, charger.zTime);
        charger.newDataFlag = 0;
    }
}
