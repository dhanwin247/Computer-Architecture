# Datapath_MIPS_-_uPowerISA
This repository contains datapath of the MIPS and uPowerISA.

It contains the files for Instruction parsing, reading memory, instructions and registers, the ALU, the Control Unit and a core file to call all these modules in order so as to realize the datapath of the ISA.

COA Assignment Team : 

(1) Sai Krishna Anand - 181CO244

(2) Dhanwin Rao       - 181CO217

(3) Arnav Nair        - 181CO209

(4) Suhas K S         - 181CO253


## To run the files : 

(1) For MIPS : 

    $ cd MIPS/
    $ iverilog MIPS_tb.v
    $ vvp a.out

(2) For uPower : 

    $ cd uPower/
    $ iverilog uPowerISA_tb.v
    $ vvp a.out
