#include <stdio.h>
#include <stdlib.h>
#include "foo.h"


struct Argument ss, dd;
void trace(const char *  format, ...) {
    if(tracechecker == 1) {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
	}
        if(tracechecker == 0);
    };

void do_mov() {
    reg[dd.adress] = ss.val; // пишем в регистр значение
    w_write(dd.adress, ss.val); // просто пишем ss.val в слово с адресом dd.adress в ОЗУ
};

void do_halt() {
trace("------halted------\n");
trace(" R0 = %06o \n R1 = %06o \n R2 = %06o \n R3 = %06o \n R4 = %06o \n R5 = %06o \n R6 = %06o \n R7 = %06o \n" ,
	reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]); // выводим все регистры
	exit(0);
};

void do_add() {
    reg[dd.adress] = ss.val + dd. val; // пишем в регистр значение суммы
    w_write (dd.adress, ss.val + dd.val); // а тут просто складываем оба значения и записываем в ОЗУ
};


void run() {
    pc = 01000;
    while(1) {
        word w = w_read(pc);
        trace("<pc = %06o word = %06o>\n", pc, w);
        pc += 2;
        int i;
        for(i = 0; i < 3; i++) {
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                if (i == 0| i == 1) { // если это mov или add, то

                    trace("(pc = %06o word = %06o  ", pc, w);
                    dd = sixbittodata(w); // расшифровываем первые 6 бит (destination), возможно, инкрементируем регистр
                    trace("dest.adress = %06o dest.val = %06o ", dd.adress, dd.val );
                    ss = sixbittodata(w >> 6); // затем расшифровываем следующие 6 бит (source), возможно, инкрементируем регистр
                    trace("source.adress = %06o source.val = %06o)\n", ss.adress, ss.val );
                }
                cmd[i].do_func(); // в любом случае выполняем функцию
            }
        }
    }
};



