#include <stdio.h>
#include "charger.h"

void handSensing(){
    charger_t *charger = malloc(sizeof(charger_t));
    charger_init(charger);
    while(1){
        charger_run(charger);
        printf("x time: %d; y time: %d; z time: %d",
                charger->xTime, charger->yTime, charge->zTime);
        charger->newDataFlag = 0;
    }
}
