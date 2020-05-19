#include <stdio.h>
#include <stdlib.h>
#include "foo.h"

int rn; // ����� ��������, ������� ������������ � mul

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
    reg[dd.adress] = ss.val; // ����� � ������� ��������
    w_write(dd.adress, ss.val); // ������ ����� ss.val � ����� � ������� dd.adress � ���
};

void do_halt() {
trace("------------------------------halted--------------------------------\n");
trace("��������� �������� R0 = %06o \n R1 = %06o \n R2 = %06o \n R3 = %06o \n R4 = %06o \n R5 = %06o \n R6 = %06o \n R7 = %06o \n" ,
	reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]); // ������� ��� ��������
	exit(0);
};

void do_add() {
    reg[dd.adress] = ss.val + dd. val; // ����� � ������� �������� �����
    w_write (dd.adress, ss.val + dd.val); // � ��� ������ ���������� ��� �������� � ���������� � ���
};


void dumpreg() {
    trace(" R0 = %06o ; R1 = %06o ; R2 = %06o ; R3 = %06o ; R4 = %06o ; R5 = %06o ; R6 = %06o ; R7 = %06o \n" ,
	reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
}
void do_mul() {
    // ss.adress - ����� ���������
    // ss.val - �������� ������
    // dd.adress - ����� ���������� (����� ��������)
    // dd.val - ��� ��������
    unsigned int result ;// ��������� ��������� ���� �����
    word resultjun; // ������� 16 ���
    word resultsen; // ������� 16 ���
    rn = ss.adress; //����� ��������
    unsigned int src = reg[rn]; // ���������� 16- �������� ��������� src
    unsigned int dst = dd.val;
    result = src * dst;
    resultjun = (word) (result & 0xffff); // ������� 16 ���
    resultsen = (word) ((result >> 16) & 0xffff); //������� 16 ���
    trace("\n resultjun = %06o \n resultsen = %06o \n",resultjun, resultsen );
    if (rn % 2 == 1) { // ���� �������� ����� ��������
        trace("oddreg ");
        reg[rn] = resultjun;
    }
    else { // ���� ������ ����� ��������
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
                if (i == 0| i == 1) { // ���� ��� mov ��� add, ��
                    ss = sixbittodata(w >> 6); // ����� �������������� ��������� 6 ��� (source), ��������, �������������� �������
                    //trace("source.adress = %06o source.val = %06o)\n", ss.adress, ss.val );
                    //trace("(pc = %06o word = %06o  ", pc, w);
                    dd = sixbittodata(w); // �������������� ������ 6 ��� (destination), ��������, �������������� �������
                    //trace("dest.adress = %06o dest.val = %06o ", dd.adress, dd.val );


                }
                if (i == 3) { // ���� ������� mul
                //trace("(pc = %06o word = %06o  ", pc, w);
                    ss = sixbittodata(w >> 6); // ����� �������������� ��������� 6 ��� (source), ��������, �������������� �������
                    trace("Command mul rn = %06o source.val = %06o)\n", ss.adress, ss.val );
                    dd = sixbittodata(w); // �������������� ������ 6 ��� (destination), ��������, �������������� �������
                    trace("Command mul dest.adress = %06o dest.val = %06o ", dd.adress, dd.val );



                }
                cmd[i].do_func();
                trace("\n"); // � ����� ������ ��������� �������
                dumpreg();
            }
        }
    }
};



