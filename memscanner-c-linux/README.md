**Memory Scanner in C for Linux**

## Overview:
This is a Linux memory scanner written in C. It reads `/proc/[pid]/maps`, filters memory regions, scans for target values (called hits), and allows changing values of said hits.

## Usage:
Compile:
gcc -O2 main.c mapfile.c globaltypes.h memfile.c targetcomp.c userio.c -o memscanner



Use:
./memscanner [pid]

## Features:
- Parses Linux process memory maps and identifies readable regions.
- Reads memory contents.
- Scans for specific values supporting specific types (integer, long, float, double, string).
- Allows for rescans based on initial hits. 
- Can edit hits to new values.

## Disclaimer:
This software is for **research and learning purposes** only.
Use at your own risk. The author assumes no responsibility for misuse or damages.  
Commercial use or redistribution is prohibited.
Do not use this program on software you do not own or have explicit permission to interact with.

## License:
See [LICENSE](LICENSE) for full license details.

## Also:
See [Thoughts](thoughts/thoughts.txt) for my reflections.




