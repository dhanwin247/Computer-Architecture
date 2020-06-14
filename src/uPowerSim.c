#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "uPowerSim.h"
#include "simulator.h"

struct symbol_table_text *sym_tab_text_head = NULL;
struct symbol_table_data *sym_tab_data_head = NULL;

int main(int argc, char *argv[])
{
    parse_command(argv);
    read_asm();
    pass_1_text();
    struct symbol_table_text *ptr;
    ptr = sym_tab_text_head;
    printf("Symbol Table - Instruction:------------------------------------------------------------------------\n");
    while (ptr != NULL)
    {
        printf("Label : %s\n", ptr->label);
        printf("Relative address : %d\n", ptr->rel_add);
        ptr = ptr->next;
    }

    pass_1_data();
    struct symbol_table_data *p;
    p = sym_tab_data_head;
    printf("Symbol Table - Data:-------------------------------------------------------------------------------\n");
    while (p != NULL)
    {
        printf("Label : %s\n", p->label);
        printf("Data : %s\n", p->data);
        p = p->next;
    }

    pass_2();
    initialize();
    printf("Registers after execution:-------------------------------------------------------------------------\n");
    display_registers();

    return 0;
}

void parse_command(char *argv[])
{
    cmd.command = argv[1];
    cmd.path = argv[1];
    return;
}

void read_asm()
{
    asm_file = fopen(cmd.path, "r");
    if (asm_file == NULL)
    {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char *buffer = NULL;
    size_t size = 0;
    int i = 0;

    while (getline(&buffer, &size, asm_file) != -1)
    {
        strcpy(f_lines[i].asm_line, buffer);
        if (f_lines[i].asm_line[strlen(f_lines[i].asm_line) - 2] == '\r')
            f_lines[i].asm_line[strlen(f_lines[i].asm_line) - 2] = '\0';
        i++;
    }

    n_lines = i;
    return;
}

void pass_1_text()
{
    int flag = 0, i;
    for (i = 0; i < n_lines; i++)
    {
        if (strcmp(f_lines[i].asm_line, ".text\n") == 0)
        {
            flag = 1;
            f_lines[i].type = 'D';
            continue;
        }

        if ((f_lines[i].asm_line[0] == '.') && (strcmp(f_lines[i].asm_line, ".text\n") != 0))
        {
            flag = 0;
            f_lines[i].type = 'D';
            continue;
        }

        // If the line is under .text and is not blank or newline
        if (flag == 1 && f_lines[i].asm_line[0] != '\n')
        {
            // Check for label
            if (f_lines[i].asm_line[strlen(f_lines[i].asm_line) - 2] == ':')
            {
                f_lines[i].type = 'L';
                struct symbol_table_text *s_temp;
                s_temp = (struct symbol_table_text *)malloc(sizeof(struct symbol_table_text));

                // Create a string with the label name without ':'
                char lname[strlen(f_lines[i].asm_line)];
                strcpy(lname, f_lines[i].asm_line);
                lname[strlen(lname) - 2] = '\0';
                char *temp = (char *)malloc(strlen(lname) * sizeof(char));
                strcpy(temp, lname);

                // Add data to the symbol table entry and connect it to the table
                s_temp->label = temp;
                s_temp->rel_add = n_instr;
                s_temp->next = sym_tab_text_head;
                sym_tab_text_head = s_temp;
                continue;
            }

            // Ignore Comments
            if (f_lines[i].asm_line[0] == '#')
            {
                f_lines[i].type = 'C';
                continue;
            }
            i_lines[n_instr].asm_line = f_lines[i].asm_line;
            // n_instr is the number of instructions actually written to the binary
            n_instr++;
        }
    }
}

/*
 * In the case of this assembler, the data is declared in the
 * following format:
 * .data
 * <label-name>:
 * <assembler-directive> <data>
 * 
 * For example,
 * .data
 * numbers:
 * .word 10
 */

void pass_1_data()
{
    int flag = 0, i;
    int llflag = 0;
    char *token;
    char *data_type, *data;
    struct symbol_table_data *ptr;
    for (i = 0; i < n_lines; i++)
    {
        if (strcmp(f_lines[i].asm_line, ".data\n") == 0)
        {
            flag = 1;
            f_lines[i].type = 'D';
            continue;
        }

        if ((f_lines[i].asm_line[0] == '.') &&
            (strcmp(f_lines[i].asm_line, ".data\n") != 0))
        {
            flag = 0;
            f_lines[i].type = 'D';
            continue;
        }

        // If the line is under .data and is not blank or newline
        if (flag == 1 && f_lines[i].asm_line[0] != '\n')
        {
            // Check for label
            if (f_lines[i].asm_line[strlen(f_lines[i].asm_line) - 2] == ':')
            {
                f_lines[i].type = 'L';
                struct symbol_table_data *s_temp;
                s_temp = (struct symbol_table_data *)malloc(sizeof(struct symbol_table_data));

                // Create a string with the label name without ':'
                char lname[sizeof(f_lines[i].asm_line)];
                strcpy(lname, f_lines[i].asm_line);
                lname[strlen(lname) - 2] = '\0';
                char *temp = (char *)malloc(strlen(lname) * sizeof(char));
                strcpy(temp, lname);

                // Move on to the next line
                i++;

                //Tokenization
                token = strtok(f_lines[i].asm_line, " \n");
                data_type = token;
                token = strtok(NULL, " \n");
                data = token;

                char *temp_data = (char *)malloc(strlen(data) * sizeof(char));
                strcpy(temp_data, data);

                // Add data to the symbol table entry and connect it to the table
                s_temp->label = temp;
                s_temp->type = data_type;
                s_temp->data = temp_data;
                s_temp->next = NULL;
                if (llflag == 0)
                {
                    llflag = 1;
                    sym_tab_data_head = s_temp;
                    ptr = s_temp;
                }
                else
                {
                    ptr->next = s_temp;
                    ptr = ptr->next;
                }
                continue;
            }

            // Ignore Comments
            if (f_lines[i].asm_line[0] == '#')
            {
                f_lines[i].type = 'C';
                continue;
            }
        }
    }
}

void pass_2()
{
    int i;
    char *instr_bin;
    bin_file = fopen("upower.bin", "w+");
    if (bin_file == NULL)
    {
        printf("Could not open file");
    }

    for (i = 0; i < n_instr; i++)
    {
        instr_bin = translate_instr(i_lines[i].asm_line, i);
        fprintf(bin_file, "%s\n", instr_bin);
    }
    fclose(bin_file);
}

/*
 * This function is called for every text instructon.
 * Every instruction is first parsed using strtok.
 * Then the specialized functions in instr_translator.c are
 * called depending on the instruction.
 */
char *translate_instr(char *instr, int cia)
{
    int i = 0, instr_c = 0;
    char *instr_bin;
    char *instr_v[100];
    char inst[100];
    char *instr_hex;
    strcpy(inst, instr);

    //Tokenization
    instr_v[0] = strtok(inst, ", ()");
    while (instr_v[i] != NULL)
    {
        i++;
        instr_c++;
        instr_v[i] = strtok(NULL, ", ()");
    }

    //Add new 'if' condition for adding a new instruction
    if (strcmp(instr_v[0], "add") == 0)
        instr_hex = add(instr_c, instr_v);
    if (strcmp(instr_v[0], "addi") == 0)
        instr_hex = addi(instr_c, instr_v);
    if (strcmp(instr_v[0], "beq") == 0)
        instr_hex = beq(instr_c, instr_v, cia);
    if (strcmp(instr_v[0], "and") == 0)
        instr_hex = and(instr_c, instr_v);
    if (strcmp(instr_v[0], "or") == 0)
        instr_hex = or (instr_c, instr_v);
    if (strcmp(instr_v[0], "xor") == 0)
        instr_hex = xor(instr_c, instr_v);
    if (strcmp(instr_v[0], "nand") == 0)
        instr_hex = nand(instr_c, instr_v);
    if (strcmp(instr_v[0], "subf") == 0)
        instr_hex = subf(instr_c, instr_v);
    if (strcmp(instr_v[0], "ld") == 0)
        instr_hex = ld(instr_c, instr_v);
    if (strcmp(instr_v[0], "std") == 0)
        instr_hex = std(instr_c, instr_v);

    return instr_hex;
}

int parse_line(char *line)
{
    int size = 64;
    char **args = malloc(size * sizeof(char *));
    char *token = strtok(line, " ,");
    int i = 0;
    while (token != NULL)
    {
        args[i] = token;
        token = strtok(NULL, " ,");
        i++;
    }
    args[i] = NULL;
    int instruction_binary;
    instruction_binary = translate_instruction(args);
}

int translate_instruction(char *args[])
{
    check_instruction_type(args[0]);
}

char check_instruction_type(char *inst)
{
    if (inst[0] == '.')
        return 'D';
    else if (inst[0] == '#')
        return 'C';
    else if (inst[(sizeof(*inst) - 1)] == ':')
        return 'L';
    else
        return 'I';
}

void sym_table_to_files()
{
    struct symbol_table_text *s_temp_text;
    s_temp_text = sym_tab_text_head;
    text_sym = fopen("text.sym", "w");
    while (s_temp_text != NULL)
    {
        fprintf(text_sym, "%s#%d", s_temp_text->label, s_temp_text->rel_add);
        s_temp_text = s_temp_text->next;
    }

    struct symbol_table_data *s_temp_data;
    s_temp_data = sym_tab_data_head;
    data_sym = fopen("data.sym", "w");
    while (s_temp_data != NULL)
    {
        fprintf(data_sym, "%s#%s#%s", s_temp_data->label, s_temp_data->type, s_temp_data->data);
        s_temp_data = s_temp_data->next;
    }

    return;
}