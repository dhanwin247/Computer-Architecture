#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

#include "uPowerSim.h"
#include "simulator.h"

void initialize()
{
    CIA = 0;
    NIA = 0;
    char ch;
    int displacement;
    read_bin();
    load_memory();
    printf("Assembly complete. Binary ready for execution. Do you want to execute it step by step (y or n): ");
    fflush(stdin);
    ch = getchar();
    int flag = 0;
    while (CIA != n_instr)
    {
        if (ch == 'y')
        {
            if (flag == 0)
            {
                printf("Press enter to start the execution.....");
                flag = 1;
            }
            else
                printf("Press enter to continue to the next instruction.....");
            fflush(stdin);
            getchar();
        }
        displacement = 1;
        if (strcmp(bin_lines[CIA].type, "X") == 0)
        {
            instr_typ_x(bin_lines[CIA].instr);
        }

        if (strcmp(bin_lines[CIA].type, "XO") == 0)
        {
            instr_typ_xo(bin_lines[CIA].instr);
        }

        if (strcmp(bin_lines[CIA].type, "D") == 0)
        {
            instr_typ_d(bin_lines[CIA].instr);
        }

        if (strcmp(bin_lines[CIA].type, "B") == 0)
        {
            displacement = instr_typ_b(bin_lines[CIA].instr);
        }

        if (strcmp(bin_lines[CIA].type, "DS") == 0)
        {
            instr_typ_ds(bin_lines[CIA].instr);
        }
        NIA += displacement;
        if (ch == 'y')
        {
            display_registers();
        }
        CIA += displacement;
    }
    return;
}

void load_memory()
{
    struct symbol_table_data *temp;
    temp = sym_tab_data_head;
    int i;
    i = 0;
    while (temp != NULL && i != 100)
    {
        strcpy(MainMemory[i], temp->data);
        temp = temp->next;
        i++;
    }
}

void read_bin()
{
    int i = 0;
    int j, k;
    char *buffer = NULL;
    size_t size = 0;
    char ins[100], typ[100];
    bin_file = fopen("upower.bin", "r");
    if (bin_file == NULL)
    {
        perror("Binary file not found");
        return;
    }
    while (getline(&buffer, &size, bin_file) != -1)
    {
        j = 0;
        while (buffer[j] != ' ')
        {
            ins[j] = buffer[j];
            j++;
        }
        ins[j] = '\0';

        k = 0;
        j++;
        while (buffer[j] != '\0' && buffer[j] != '\r' && buffer[j] != '\n')
        {
            typ[k] = buffer[j];
            k++;
            j++;
        }
        typ[k] = '\0';
        strcpy(bin_lines[i].instr, ins);
        strcpy(bin_lines[i].type, typ);
        i++;
    }
}

void display_registers()
{
    printf("Special Registers:\n");
    printf("LR: %ld \tCIA: %ld\n", LR, CIA);
    printf("NIA: %ld\tSRR0: %ld\n", NIA, SRR0);
    printf("CR: %d\n", CR);
    printf("General Purpose Registers:\n");
    int i;
    for (i = 0; i < 31; i += 2)
    {
        printf("R%d: %ld\tR%d: %ld\n", i, R[i], i + 1, R[i + 1]);
    }
}

int bin_to_int(char *bin)
{
    int l = strlen(bin);
    int i, j, b = 0;
    char a;
    for (i = l - 1; i >= 0; i--)
    {
        a = bin[i];
        int a1 = a - 48;
        b = b + a1 * pow(2, l - i - 1);
    }
    return b;
}

void instr_typ_x(char *bin_instr)
{
    char sRS[6], sRA[6], sRB[6], sRc[2];
    int RS, RA, RB, Rc, PO, XO;
    char sPO[7], sXO[11];
    int i;
    for (i = 0; i < 32; i++)
    {
        if (i < 6)
            sPO[i] = bin_instr[i];
        if (i == 6)
            sPO[i] = '\0';

        if (i >= 6 && i < 11)
            sRS[i - 6] = bin_instr[i];
        if (i == 11)
            sRS[i - 6] = '\0';

        if (i >= 11 && i < 16)
            sRA[i - 11] = bin_instr[i];
        if (i == 16)
            sRA[i - 11] = '\0';

        if (i >= 16 && i < 21)
            sRB[i - 16] = bin_instr[i];
        if (i == 21)
            sRB[i - 16] = '\0';

        if (i >= 21 && i < 31)
            sXO[i - 21] = bin_instr[i];
        if (i == 31)
            sXO[i - 21] = '\0';

        if (i == 31)
            sRc[0] = bin_instr[i];
        sRc[1] = '\0';
    }

    PO = bin_to_int(sPO);
    RS = bin_to_int(sRS);
    RA = bin_to_int(sRA);
    RB = bin_to_int(sRB);
    XO = bin_to_int(sXO);
    Rc = bin_to_int(sRc);

    // NAND
    if (PO == 31 && XO == 476)
    {
        R[RA] = ~(R[RS] & R[RB]);
    }

    // OR
    if (PO == 31 && XO == 444)
    {
        R[RA] = R[RS] | R[RB];
    }

    // AND
    if (PO == 31 && XO == 28)
    {
        R[RA] = R[RS] & R[RB];
    }

    // XOR
    if (PO == 31 && XO == 316)
    {
        R[RA] = R[RS] ^ R[RB];
    }
}

void instr_typ_xo(char *bin_instr)
{
    char sRT[6], sRA[6], sRB[6], sRc[2], sOE[2];
    int RS, RA, RB, Rc, PO, XO, OE, RT;
    char sPO[7], sXO[10];
    int i;
    for (i = 0; i < 32; i++)
    {
        if (i < 6)
            sPO[i] = bin_instr[i];
        if (i == 6)
            sPO[i] = '\0';

        if (i >= 6 && i < 11)
            sRT[i - 6] = bin_instr[i];
        if (i == 11)
            sRT[i - 6] = '\0';

        if (i >= 11 && i < 16)
            sRA[i - 11] = bin_instr[i];
        if (i == 16)
            sRA[i - 11] = '\0';

        if (i >= 16 && i < 21)
            sRB[i - 16] = bin_instr[i];
        if (i == 21)
            sRB[i - 16] = '\0';

        if (i == 21)
            sOE[0] = bin_instr[i];
        sOE[1] = '\0';

        if (i >= 22 && i < 31)
            sXO[i - 22] = bin_instr[i];
        if (i == 31)
            sXO[i - 22] = '\0';

        if (i == 31)
            sRc[0] = bin_instr[i];
        sRc[1] = '\0';
    }

    PO = bin_to_int(sPO);
    RT = bin_to_int(sRT);
    RA = bin_to_int(sRA);
    RB = bin_to_int(sRB);
    OE = bin_to_int(sOE);
    XO = bin_to_int(sXO);
    Rc = bin_to_int(sRc);

    // Add
    if (PO == 31 && OE == 0 && Rc == 0 && XO == 266)
    {
        R[RT] = R[RA] + R[RB];
    }

    // Subtract
    if (PO == 31 && OE == 0 && Rc == 0 && XO == 40)
    {
        R[RT] = R[RB] - R[RA];
    }
}

void instr_typ_d(char *bin_instr)
{
    char sRT[6], sRA[6], sSI[17];
    int RT, RA, SI, PO;
    char sPO[7];
    int i;
    for (i = 0; i < 32; i++)
    {
        if (i < 6)
            sPO[i] = bin_instr[i];
        if (i == 6)
            sPO[i] = '\0';

        if (i >= 6 && i < 11)
            sRT[i - 6] = bin_instr[i];
        if (i == 11)
            sRT[i - 6] = '\0';

        if (i >= 11 && i < 16)
            sRA[i - 11] = bin_instr[i];
        if (i == 16)
            sRA[i - 11] = '\0';

        if (i >= 16 && i <= 31)
            sSI[i - 16] = bin_instr[i];
    }
    if (i == 32)
        sSI[i - 16] = '\0';

    PO = bin_to_int(sPO);
    RT = bin_to_int(sRT);
    RA = bin_to_int(sRA);
    SI = bin_to_int(sSI);

    // Add immediate
    if (PO == 14)
    {
        R[RT] = R[RA] + SI;
    }
}

int instr_typ_b(char *bin_instr)
{
    char sPO[7], sBO[6], sBI[6], sBD[15], sAA[2], sLK[2];
    int PO, BO, BI, BD, AA, LK;
    int i;
    for (i = 0; i < 32; i++)
    {
        if (i < 6)
            sPO[i] = bin_instr[i];
        if (i == 6)
            sPO[i] = '\0';

        if (i >= 6 && i < 11)
            sBO[i - 6] = bin_instr[i];
        if (i == 11)
            sBO[i - 6] = '\0';

        if (i >= 11 && i < 16)
            sBI[i - 11] = bin_instr[i];
        if (i == 16)
            sBI[i - 11] = '\0';

        if (i >= 16 && i < 30)
            sBD[i - 16] = bin_instr[i];
        if (i == 30)
            sBD[i - 16] = '\0';

        if (i == 30)
            sAA[0] = bin_instr[i];
        sAA[1] = '\0';

        if (i == 31)
            sLK[0] = bin_instr[i];
        sLK[1] = '\0';
    }

    PO = bin_to_int(sPO);
    BO = bin_to_int(sBO);
    BI = bin_to_int(sBI);
    BD = bin_to_int(sBD);
    AA = bin_to_int(sAA);
    LK = bin_to_int(sLK);

    if (PO == 19 && AA == 0 && LK == 0)
    {
        if (R[BO] == R[BI])
        {
            return BD;
        }
    }
}

void instr_typ_ds(char *bin_instr)
{
    char sRT[6], sRA[6], sDS[15], sXO[3];
    int RT, RA, DS, PO, XO;
    char sPO[7];
    int i;
    for (i = 0; i < 32; i++)
    {
        if (i < 6)
            sPO[i] = bin_instr[i];
        if (i == 6)
            sPO[i] = '\0';

        if (i >= 6 && i < 11)
            sRT[i - 6] = bin_instr[i];
        if (i == 11)
            sRT[i - 6] = '\0';

        if (i >= 11 && i < 16)
            sRA[i - 11] = bin_instr[i];
        if (i == 16)
            sRA[i - 11] = '\0';

        if (i >= 16 && i < 30)
            sDS[i - 16] = bin_instr[i];
        if (i == 30)
            sDS[i - 16] = '\0';

        if (i == 30)
        {
            sXO[0] = bin_instr[i];
            sXO[1] = bin_instr[i + 1];
            sXO[2] = '\0';
            break;
        }
    }

    PO = bin_to_int(sPO);
    RT = bin_to_int(sRT);
    RA = bin_to_int(sRA);
    DS = bin_to_int(sDS);
    XO = bin_to_int(sXO);

    // Load doubleword
    if (PO == 58 && XO == 0)
    {
        int target_address = R[RA] + DS;
        int32_t low, high;
        sscanf(MainMemory[target_address], "%" PRId32 "", &low);
        sscanf(MainMemory[target_address + 1], "%" PRId32 "", &high);
        R[RT] = ((int64_t)high) << 32 | low;
    }

    if (PO == 62 && XO == 0)
    {
        int ctr = 0;
        char highstr[20], lowstr[20];
        int target_address = R[RA] + DS;
        int32_t high, low;
        high = (int32_t)((R[RT] & 0xFFFFFFFF00000000LL) >> 32);
        low = (int32_t)(R[RT] & 0xFFFFFFFFLL);
        sprintf(highstr, "%" PRId32 "", high);
        sprintf(lowstr, "%" PRId32 "", low);
        strcpy(MainMemory[target_address + 1], highstr);
        strcpy(MainMemory[target_address], lowstr);
    }
}