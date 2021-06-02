# soc1prj_template

ldscript is buggy but it works.

Code starts at 0x1000.

gentextbin.sh extracts .text section and extents it to 1KB size.

The soc1_bootloader receives the bin through UART and stores it at memory 0x1000, then calls into the entry point also at address 0x1000.
