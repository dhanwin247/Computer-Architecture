#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifndef UPOWERSIM_H
#define UPOWERSIM_H
//Structs
struct cmdline
{
    char *command;
    char *path;
    // Variables to be added for
    // options
};

struct line
{
    char asm_line[100];
    /*
    'C' = Comment
    'L' = Label
    'D' = Assembler Directive
    'I' = Instruction
    */
    char type;
};

struct instr_line
{
    char *asm_line;
};

/*
Symbol table is used to store pointers to labels.
Here, it is implemented using linked lists.
sym_table_head gives the start to the list.
*/

struct symbol_table_text
{
    char *label;
    //int *abs_add;
    int rel_add;
    struct symbol_table_text *next;
};

struct symbol_table_data
{
    char *label;

    /*
     * word - stores a word in memory in memory.
     */
    char *type;

    /*
     * Currently only integer data is able to be stored
     */
    char *data;
    struct symbol_table_data *next;
};

extern struct symbol_table_text *sym_tab_text_head;
extern struct symbol_table_data *sym_tab_data_head;
//struct symbol_table *sym_tab_current;

//Global Variables
FILE *asm_file;
FILE *bin_file;
FILE *text_sym;
FILE *data_sym;
struct cmdline cmd;
struct line f_lines[100];
struct instr_line i_lines[100];
int n_lines, n_instr;

//Functions
void parse_command(char *argv[]);
void read_asm(void);
void pass_1_text(void);
void pass_1_data(void);
void pass_2(void);
int parse_line(char *);
int translate_instruction(char *args[]);
char check_instruction_type(char *inst);

char *register_translator(char *t);
void src_trgts_reg_translate_for_x_format(char *instr_v[], char *);
char * and (int instr_c, char *instr_v[]);
char *nand(int instr_c, char *instr_v[]);
char * or (int instr_c, char *instr_v[]);
char * xor (int instr_c, char *instr_v[]);
char *extsw(int instr_c, char *instr_v[]);
char *sld(int instr_c, char *instr_v[]);
char *srd(int instr_c, char *instr_v[]);
char *srad(int instr_c, char *instr_v[]);
char *add(int instr_c, char *instr_v[]);
char *addi(int instr_c, char *instr_v[]);
char *beq(int instr_c, char *instr_v[], int curr_addr);
char *subf(int instr_c, char *instr_v[]);
char *ld(int instr_c, char *instr_v[]);
char *std(int instr_c, char *instr_v[]);
char *translate_instr(char *instr, int cia);

#endif