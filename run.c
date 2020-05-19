#include <stdio.h>
#include <stdlib.h>
#include "foo.h"

int rn; // номер регистра, который используется в mul

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
trace("------------------------------halted--------------------------------\n");
trace("Финальные регистры R0 = %06o \n R1 = %06o \n R2 = %06o \n R3 = %06o \n R4 = %06o \n R5 = %06o \n R6 = %06o \n R7 = %06o \n" ,
	reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]); // выводим все регистры
	exit(0);
};

void do_add() {
    reg[dd.adress] = ss.val + dd. val; // пишем в регистр значение суммы
    w_write (dd.adress, ss.val + dd.val); // а тут просто складываем оба значения и записываем в ОЗУ
};


void dumpreg() {
    trace(" R0 = %06o ; R1 = %06o ; R2 = %06o ; R3 = %06o ; R4 = %06o ; R5 = %06o ; R6 = %06o ; R7 = %06o \n" ,
	reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
}
void do_mul() {
    // ss.adress - адрес источника
    // ss.val - значение оттуда
    // dd.adress - адрес назначения (номер регистра)
    // dd.val - его значение
    unsigned int result ;// результат умножения двух чисел
    word resultjun; // младшие 16 бит
    word resultsen; // старшие 16 бит
    rn = ss.adress; //номер регистра
    unsigned int src = reg[rn]; // содержимое 16- битового источника src
    unsigned int dst = dd.val;
    result = src * dst;
    resultjun = (word) (result & 0xffff); // младшие 16 бит
    resultsen = (word) ((result >> 16) & 0xffff); //старшие 16 бит
    trace("\n resultjun = %06o \n resultsen = %06o \n",resultjun, resultsen );
    if (rn % 2 == 1) { // если нечетный номер регистра
        trace("oddreg ");
        reg[rn] = resultjun;
    }
    else { // если четный номер регистра
        reg[rn] = resultsen;
        reg[rn + 1] = resultjun;
    }


 //       11 100 111 111   1 100 011 011 110 101

 //0 000 011 100 111 111 % 1 100 011 011 110 101

};

void run() {
    pc = 01000;
    while(1) {
        word w = w_read(pc);
        pc += 2;
        trace("%06o %06o ", pc, w);
        int i;
        for(i = 0; i < 4; i++) {
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                trace("%s ", cmd[i].name);
                if (i == 0| i == 1) { // если это mov или add, то
                    ss = sixbittodata(w >> 6); // затем расшифровываем следующие 6 бит (source), возможно, инкрементируем регистр
                    //trace("source.adress = %06o source.val = %06o)\n", ss.adress, ss.val );
                    //trace("(pc = %06o word = %06o  ", pc, w);
                    dd = sixbittodata(w); // расшифровываем первые 6 бит (destination), возможно, инкрементируем регистр
                    //trace("dest.adress = %06o dest.val = %06o ", dd.adress, dd.val );


                }
                if (i == 3) { // если команда mul
                //trace("(pc = %06o word = %06o  ", pc, w);
                    ss = sixbittodata(w >> 6); // затем расшифровываем следующие 6 бит (source), возможно, инкрементируем регистр
                    trace("Command mul rn = %06o source.val = %06o)\n", ss.adress, ss.val );
                    dd = sixbittodata(w); // расшифровываем первые 6 бит (destination), возможно, инкрементируем регистр
                    trace("Command mul dest.adress = %06o dest.val = %06o ", dd.adress, dd.val );



                }
                cmd[i].do_func();
                trace("\n"); // в любом случае выполняем функцию
                dumpreg();
            }
        }
    }
};



