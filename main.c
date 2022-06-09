#include "emulator.h"

unsigned long CPU_CLOCK = 0;
unsigned short breakpoint = breakpoint_default;  // default breakpoint to not interrupt program
unsigned short custom_PC = CLEAR;  // initially custom_PC location is clear so loader gets PC location from S-Records

FILE *infile;
char srec[MAX_REC_LEN];

int main()
{
    unsigned short IR, instr;
    char UI[MAX_USER_INPUT_LEN], filename[MAX_REC_LEN];

    printf("Enter filename\n");
    printf("> ");
    scanf("%s", filename);
    getchar();


    if (filename[0] == NUL)
    {
        printf("Invalid filename\n");
        return 0;
    }

    infile = fopen(filename, "r");

    initial_CPU_state();

    while (1)
    {
        printf("\n***** XMX Emulator - User Interface *****\n");
        printf("CPU clock cycles: %lu\n", CPU_CLOCK);
        printf("r   - run\n");
        printf("md  - Memory Dump\n");
        printf("drf - Display Register File\n");
        printf("dw  - Display PSW\n");
        printf("sb  - Set Breakpoint\n");
        printf("sp  - Set PC\n");
        printf("E   - Exit Emulator\n");

        printf("> ");
        scanf("%s", UI);
        getchar();


        if (strcmp(UI, "r") == 0)  // run
        {
            // load data/instructions into main memory
            loader();
            while (PC != breakpoint)
            {
                printf("\nCLOCK: %lu\n", CPU_CLOCK);
                IR = fetch();
                if (CEX.state == OFF_state)
                {
                    instr = decode(IR);
                    if (instr == END_i) break;
                    execute(instr, IR);
                }
                else  // CEX state is ON
                {
                    if (CEX.TorF == TRUE)
                    {
                        if (CEX.T_count > 0)  // dec and exec num of true count
                        {
                            instr = decode(IR);
                            execute(instr, IR);
                            CEX.T_count--;
                        }
                        else  // fetch but ignore exec num of false count
                        {
                            if (CEX.F_count > 0)
                                CEX.F_count--;
                        }
                        if (CEX.T_count == 0 && CEX.F_count == 0)
                            CEX.state = FALSE;
                    }
                    else  // CEX condition is FALSE
                    {
                        if (CEX.T_count > 0)  // fetch and ignore
                        {
                            CEX.T_count--;
                        }
                        else  // fetch and execute
                        {
                            instr = decode(IR);
                            execute(instr, IR);
                            CEX.F_count--;
                        }
                        if (CEX.T_count == 0 && CEX.F_count == 0)
                            CEX.state = FALSE;
                    }
                }
                display_regfile();
                printf("PSW -> C:%d V:%d Z:%d N:%d\n", PSW.C, PSW.V, PSW.Z, PSW.N);
            }
        }
        else if (strcmp(UI, "md") == 0)  // Memory dump
        {
            unsigned short start, end;
            printf("Specify Memory range\n");
            printf("Format: <start_adr> <end_adr>\n");
            printf("> ");
            scanf("%4hX %4hX", &start, &end);
            printf("\n");
            memory_dump(start, end);
        }
        else if (strcmp(UI, "drf") == 0)  // display register file
            display_regfile();
        else if (strcmp(UI, "dw") == 0)  // display PSW
            printf("PSW - C:%d V:%d Z:%d N:%d\n", PSW.C, PSW.V, PSW.Z, PSW.N);
        else if (strcmp(UI, "sb") == 0)  // set breakpoint
        {
            printf("Specify breakpoint location\n");
            printf("> ");
            scanf("%hX", &breakpoint);
            getchar();
        }
        else if (strcmp(UI, "sp") == 0)  // set custom PC
        {
            printf("> ");
            scanf("%hX", &PC);
            getchar();
            custom_PC = SET;
        }
        else if (strcmp(UI, "E") == 0)  // exit program
        {
            printf("Exiting...\n");
            return 0;
        }
        else
            printf("Invalid Command\n");
    }
}
