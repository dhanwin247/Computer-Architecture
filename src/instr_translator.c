#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "uPowerSim.h"
#include "simulator.h"

int i;

// 5 bit representations of register numbers
char *register_translator(char *t)
{
    if (strcmp(t, "R1") == 0 || strcmp(t, "R1\n") == 0)
        return "00001";
    else if (strcmp(t, "R2") == 0 || strcmp(t, "R2\n") == 0)
        return "00010";
    else if (strcmp(t, "R3") == 0 || strcmp(t, "R3\n") == 0)
        return "00011";
    else if (strcmp(t, "R4") == 0 || strcmp(t, "R4\n") == 0)
        return "00100";
    else if (strcmp(t, "R5") == 0 || strcmp(t, "R5\n") == 0)
        return "00101";
    else if (strcmp(t, "R6") == 0 || strcmp(t, "R6\n") == 0)
        return "00110";
    else if (strcmp(t, "R7") == 0 || strcmp(t, "R7\n") == 0)
        return "00111";
    else if (strcmp(t, "R8") == 0 || strcmp(t, "R8\n") == 0)
        return "01000";
    else if (strcmp(t, "R9") == 0 || strcmp(t, "R9\n") == 0)
        return "01001";
    else if (strcmp(t, "R10") == 0 || strcmp(t, "R10\n") == 0)
        return "01010";
    else if (strcmp(t, "R11") == 0 || strcmp(t, "R11\n") == 0)
        return "01011";
    else if (strcmp(t, "R12") == 0 || strcmp(t, "R12\n") == 0)
        return "01100";
    else if (strcmp(t, "R13") == 0 || strcmp(t, "R13\n") == 0)
        return "01101";
    else if (strcmp(t, "R14") == 0 || strcmp(t, "R14\n") == 0)
        return "01110";
    else if (strcmp(t, "R15") == 0 || strcmp(t, "R15\n") == 0)
        return "01111";
    else if (strcmp(t, "R16") == 0 || strcmp(t, "R16\n") == 0)
        return "10000";
    else if (strcmp(t, "R17") == 0 || strcmp(t, "R17\n") == 0)
        return "10001";
    else if (strcmp(t, "R18") == 0 || strcmp(t, "R18\n") == 0)
        return "10010";
    else if (strcmp(t, "R19") == 0 || strcmp(t, "R19\n") == 0)
        return "10011";
    else if (strcmp(t, "R20") == 0 || strcmp(t, "R20\n") == 0)
        return "10100";
    else if (strcmp(t, "R21") == 0 || strcmp(t, "R21\n") == 0)
        return "10101";
    else if (strcmp(t, "R22") == 0 || strcmp(t, "R22\n") == 0)
        return "10110";
    else if (strcmp(t, "R23") == 0 || strcmp(t, "R23\n") == 0)
        return "10111";
    else if (strcmp(t, "R24") == 0 || strcmp(t, "R24\n") == 0)
        return "11000";
    else if (strcmp(t, "R25") == 0 || strcmp(t, "R25\n") == 0)
        return "11001";
    else if (strcmp(t, "R26") == 0 || strcmp(t, "R26\n") == 0)
        return "11010";
    else if (strcmp(t, "R27") == 0 || strcmp(t, "R27\n") == 0)
        return "11011";
    else if (strcmp(t, "R28") == 0 || strcmp(t, "R28\n") == 0)
        return "11100";
    else if (strcmp(t, "R29") == 0 || strcmp(t, "R29\n") == 0)
        return "11101";
    else if (strcmp(t, "R30") == 0 || strcmp(t, "R30\n") == 0)
        return "11110";
    else if (strcmp(t, "R31") == 0 || strcmp(t, "R31\n") == 0)
        return "11111";
    else
        return NULL;
}

/* 
 * Utility function for binary encoding 
 * of the registers in the instructions of X format
 */
void src_trgts_reg_translate_for_x_format(char *instr_v[], char *bin_enc)
{
    char *temp1 = register_translator(instr_v[2]);
    char *temp2 = register_translator(instr_v[1]);
    char *temp3 = register_translator(instr_v[3]);
    for (i = 6; i <= 10; i++)
        bin_enc[i] = temp1[i - 6];
    for (i = 11; i <= 15; i++)
        bin_enc[i] = temp2[i - 11];
    if (strcmp(instr_v[0], "extsw") != 0)
    {
        for (i = 16; i <= 20; i++)
            bin_enc[i] = temp3[i - 16];
    }

    else
    {
        for (i = 16; i <= 20; i++)
            bin_enc[i] = '0';
    }
}

char * and (int instr_c, char *instr_v[])
{
    char *bin_enc = (char *)malloc(34 * sizeof(char));
    // PO  011111
    bin_enc[0] = '0';
    for (i = 1; i <= 5; i++)
    {
        bin_enc[i] = '1';
    }

    // XO  0000011100
    for (i = 21; i <= 25; i++)
    {
        bin_enc[i] = '0';
    }
    for (i = 26; i <= 28; i++)
    {
        bin_enc[i] = '1';
    }
    bin_enc[29] = '0';
    bin_enc[30] = '0';

    // Rc
    bin_enc[31] = '0';

    // RS RA RB
    src_trgts_reg_translate_for_x_format(instr_v, bin_enc);

    bin_enc[32] = ' ';
    bin_enc[33] = 'X';
    bin_enc[34] = '\0';

    return bin_enc;
}

char *nand(int instr_c, char *instr_v[])
{
    char *bin_enc = (char *)malloc(34 * sizeof(char));
    // PO 011111
    bin_enc[0] = '0';
    for (i = 1; i <= 5; i++)
    {
        bin_enc[i] = '1';
    }

    // XO   0111011100
    bin_enc[21] = '0';
    for (i = 22; i <= 24; i++)
    {
        bin_enc[i] = '1';
    }
    bin_enc[25] = '0';
    for (i = 26; i <= 28; i++)
    {
        bin_enc[i] = '1';
    }
    bin_enc[29] = '0';
    bin_enc[30] = '0';

    // Rc
    bin_enc[31] = '0';

    // RS RA RB
    src_trgts_reg_translate_for_x_format(instr_v, bin_enc);

    bin_enc[32] = ' ';
    bin_enc[33] = 'X';
    bin_enc[34] = '\0';

    return bin_enc;
}

char * or (int instr_c, char *instr_v[])
{
    char *bin_enc = (char *)malloc(34 * sizeof(char));
    // PO 011111
    bin_enc[0] = '0';
    for (i = 1; i <= 5; i++)
    {
        bin_enc[i] = '1';
    }

    // XO 0110111100
    bin_enc[21] = '0';
    bin_enc[22] = '1';
    bin_enc[23] = '1';
    bin_enc[24] = '0';
    for (i = 25; i <= 28; i++)
    {
        bin_enc[i] = '1';
    }
    bin_enc[29] = '0';
    bin_enc[30] = '0';

    // Rc
    bin_enc[31] = '0';

    // RS RA RB
    src_trgts_reg_translate_for_x_format(instr_v, bin_enc);

    bin_enc[32] = ' ';
    bin_enc[33] = 'X';
    bin_enc[34] = '\0';

    return bin_enc;
}

char * xor (int instr_c, char *instr_v[]) {
    char *bin_enc = (char *)malloc(34 * sizeof(char));
    // PO 011111
    bin_enc[0] = '0';
    for (i = 1; i <= 5; i++)
    {
        bin_enc[i] = '1';
    }

    // XO 0100111100
    bin_enc[21] = '0';
    bin_enc[22] = '1';
    bin_enc[23] = '0';
    bin_enc[24] = '0';
    for (i = 25; i <= 28; i++)
    {
        bin_enc[i] = '1';
    }
    bin_enc[29] = '0';
    bin_enc[30] = '0';

    // Rc
    bin_enc[31] = '0';

    // RS RA RB
    src_trgts_reg_translate_for_x_format(instr_v, bin_enc);

    bin_enc[32] = ' ';
    bin_enc[33] = 'X';
    bin_enc[34] = '\0';

    return bin_enc;
}

    char *add(int instr_c, char *instr_v[])
{
    int32_t instr_hex;
    char a;
    int rt, ra, rb, j, l, b = 0, k = 6;
    // The binary with registernumbers filled as 0 initially
    char instr[] = "01111100000000000000001000010100 XO";
    // Temporary storage of register numbers in binary
    int reg[5];

    for (i = 1; i < instr_c; i++)
    {
        char *temp = instr_v[i];

        // To extract register number
        if (temp[2] == '\0')
            l = 1;
        else
            l = 2;

        // Finding register number in decimal
        b = 0;
        for (j = l; j >= 1; j--)
        {
            a = instr_v[i][j];
            int a1 = a - 48;
            b = b + a1 * pow(10, l - j);
        }
        if (b > 31)
        {
            // Corner Case
            printf("Invalid register number.\n");
            return '\0';
        }
        for (j = 4; j >= 0; j--)
        {
            reg[j] = b % 2;
            b /= 2;
        }

        // Making the changes in the binary
        for (j = 0; j < 5; j++)
        {
            if (reg[j] == 0)
                instr[k + j] = '0';
            else
                instr[k + j] = '1';
        }
        k += 5;
    }

    char *bin_final = (char *)malloc(sizeof(instr));
    bin_final = strcpy(bin_final, instr);

    return bin_final;
}

char *addi(int instr_c, char *instr_v[])
{
    int32_t instr_hex;
    char a;
    int rt, ra, j, l, b = 0, k = 6;
    // The binary with register numbers filled as 0 initially
    char instr[] = "00111000000000000000000000000000 D";
    // Temporary storage of register numbers in binary
    int reg[5];
    // Temporary storage of immediate value
    int imm[16];

    for (i = 1; i < instr_c - 1; i++)
    {
        char *temp = instr_v[i];

        // To extract register number
        if (temp[2] == '\0')
            l = 1;
        else
            l = 2;

        // Finding register number in decimal
        b = 0;
        for (j = l; j >= 1; j--)
        {
            a = instr_v[i][j];
            int a1 = a - 48;
            b = b + a1 * pow(10, l - j);
        }
        if (b > 31)
        {
            // Corner Case
            printf("Invalid register number.\n");
            return '\0';
        }
        for (j = 4; j >= 0; j--)
        {
            reg[j] = b % 2;
            b /= 2;
        }

        // Making the changes in the binary
        for (j = 0; j < 5; j++)
        {
            if (reg[j] == 0)
                instr[k + j] = '0';
            else
                instr[k + j] = '1';
        }
        k += 5;
    }

    l = strlen(instr_v[3]);
    l--;
    b = 0;

    for (i = l - 1; i >= 0; i--)
    {
        // Extracting the immediate value integer form the string array
        a = instr_v[3][i];
        int a1 = a - 48;
        b = b + a1 * pow(10, l - 1 - i);
    }

    for (i = 15; i >= 0; i--)
    {
        // Converting immediate to a 16-bit binary
        imm[i] = b % 2;
        b /= 2;
    }

    k = 16;
    // Making the changes in the binary
    for (j = 0; j < 16; j++)
    {
        if (imm[j] == 0)
            instr[k + j] = '0';
        else
            instr[k + j] = '1';
    }

    char *bin_final = (char *)malloc(sizeof(instr));
    bin_final = strcpy(bin_final, instr);

    return bin_final;
}

char *beq(int instr_c, char *instr_v[], int curr_addr)
{
    char *temp;
    // Binary with BD,BI and BO parts filled with 0's
    char instr[] = "01001100000000000000000000000000 B";
    int addr = 0, j, k = 0, l, b = 0, a;
    int bd[14], reg[5];
    struct symbol_table_text *ptr = sym_tab_text_head;
    char instr_v3[] = "";

    while (ptr != NULL)
    {
        temp = ptr->label;
        strcat(temp, "\n");

        // Finding address of label to jump to
        if (strcmp(temp, instr_v[3]) == 0)
        {
            addr = ptr->rel_add;
            break;
        }
        ptr = ptr->next;
    }
    if (ptr == NULL)
    {
        printf("Invalid label\n");
        return "\0";
    }

    // The branch displacement
    b = curr_addr - addr;
    k = 16;
    // Branch displacement as a 14-bit binary
    if (b < 0)
    {
        for (i = 13; i >= 0; i--)
        {
            bd[i] = b % 2;
            b /= 2;
        }
        for (i = 0; i < 14; i++)
        {
            if (bd[i] == 0)
                instr[k + i] = '0';
            else
                instr[k + i] = '1';
        }
    }
    // If Branch Displacement<0, calculate binary using 1's complement of -b-1;
    else
    {
        b = b * (-1);
        b -= 1;
        for (i = 13; i >= 0; i--)
        {
            bd[i] = b % 2;
            b /= 2;
        }
        for (i = 0; i < 14; i++)
        {
            // The complements
            if (bd[i] == 0)
                instr[k + i] = '1';
            else
                instr[k + i] = '0';
        }
    }

    k = 6;
    for (i = 1; i <= 2; i++)
    {
        // Finding the register numbers to be compared.
        temp = instr_v[i];

        // To extract register number
        if (temp[2] == '\0')
            l = 1;
        else
            l = 2;
        // Finding register number in decimal
        b = 0;
        for (j = l; j >= 1; j--)
        {

            a = instr_v[i][j];
            int a1 = a - 48;
            b = b + a1 * pow(10, l - j);
        }

        if (b > 31)
        {
            // Corner Case
            printf("Invalid register number.\n");
            return NULL;
        }
        for (j = 4; j >= 0; j--)
        {
            reg[j] = b % 2;
            b /= 2;
        }
        // Making the changes in the binary
        for (j = 0; j < 5; j++)
        {
            if (reg[j] == 0)
                instr[k + j] = '0';
            else
                instr[k + j] = '1';
        }
        k += 5;
    }

    char *bin_final = (char *)malloc(sizeof(instr) * sizeof(char));
    bin_final = strcpy(bin_final, instr);

    return bin_final;
}

char *subf(int instr_c, char *instr_v[])
{
    char a;
    int rt, ra, rb, i, j, l, b = 0, k = 6;
    // The binary with registernumbers filled as 0 initially
    char instr[] = "01111100000000000000000001010000 XO";
    // Temporary storage of register numbers in binary
    int reg[5];

    for (i = 1; i < instr_c; i++)
    {
        char *temp = instr_v[i];

        // To extract register number
        if (temp[2] == '\0')
            l = 1;
        else
            l = 2;
        // Finding register number in decimal
        b = 0;
        for (j = l; j >= 1; j--)
        {
            a = instr_v[i][j];
            int a1 = a - 48;
            b = b + a1 * pow(10, l - j);
        }
        if (b > 31)
        {
            // Corner Case
            printf("Invalid register number.\n");
            return "\0";
        }
        for (j = 4; j >= 0; j--)
        {
            reg[j] = b % 2;
            b /= 2;
        }
        // Making the changes in the binary
        for (j = 0; j < 5; j++)
        {
            if (reg[j] == 0)
                instr[k + j] = '0';
            else
                instr[k + j] = '1';
        }
        k += 5;
    }

    char *bin_final = (char *)malloc(sizeof(instr));
    bin_final = strcpy(bin_final, instr);

    return bin_final;
}

char *ld(int instr_c, char *instr_v[])
{
    char instr[] = "11101000000000000000000000000000 DS";
    char *RT = register_translator(instr_v[1]);
    char *RA = register_translator(instr_v[3]);
    for (i = 0; i < (2 * strlen(RT)); i++)
    {
        if (i >= strlen(RT))
            instr[i + 6] = RA[i - strlen(RT)];
        else
            instr[i + 6] = RT[i];
    }
    int ds = atoi(instr_v[2]);
    int a[100];
    for (i = 0; ds > 0; i++)
    {
        a[i] = ds % 2;
        ds = ds / 2;
    }
    int length = i;
    int j = 29;
    for (i = 0; i < length; i++)
    {
        instr[j] = (char)(a[i] + '0');
        j--;
    }
    char *instr_bin = (char *)malloc(sizeof(instr));
    strcpy(instr_bin, instr);
    return instr_bin;
}

char *std(int instr_c, char *instr_v[])
{
    char instr[] = "11111000000000000000000000000000 DS";
    char *RS = register_translator(instr_v[1]);
    char *RA = register_translator(instr_v[3]);
    for (i = 0; i < (2 * strlen(RS)); i++)
    {
        if (i >= strlen(RS))
            instr[i + 6] = RA[i - strlen(RS)];
        else
            instr[i + 6] = RS[i];
    }
    int ds = atoi(instr_v[2]);
    int a[100];
    for (i = 0; ds > 0; i++)
    {
        a[i] = ds % 2;
        ds = ds / 2;
    }
    int length = i;
    int j = 29;
    for (i = 0; i < length; i++)
    {
        instr[j] = (char)(a[i] + '0');
        j--;
    }
    char *instr_bin = (char *)malloc(sizeof(instr));
    strcpy(instr_bin, instr);
    return instr_bin;
}