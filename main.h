#include <iostream>
#include <fstream>
#include <string>
#include "sculptor.h"

int main(void){
    Sculptor exemplo(60,60,60);


    exemplo.putBox(15,20,36,40,30,36);
  exemplo.cutBox(17,19,38,40,30,32);

    exemplo.setColor(0,0,1,1);
  exemplo.putEllipsoid(5,23,35,6,10,6);
  exemplo.cutEllipsoid(5,30,30,6,10,6);

  exemplo.setColor(1,0,0, 0.5);
  exemplo.putSphere(29,40,30,10);
  exemplo.cutSphere(29,35,30,10);

  exemplo.writeOFF((char*)"exemplo.off");

}
