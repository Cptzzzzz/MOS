/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Shdr *shdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }
	/*
	printf("Unsigned int:%ld\n",sizeof(unsigned int));
	printf("Ehdt:%ld\n",sizeof(Elf32_Ehdr));
	unsigned int *shoff=(unsigned int*)(binary+20);
	for(int i=0;i<11;i++){
		printf("%u ",shoff[i]);
	}
	printf("start read section\n");
	printf("offset %u %u\n",ehdr->e_shoff,ehdr->e_phoff);
	printf("e_shnum:%u e_phnum: %u\n",ehdr->e_shnum,ehdr->e_phnum);
        */
	/*
	unsigned int *shoff=(unsigned int*)(binary+20);
	printf("%u\n",shoff[5]);
	shdr=(Elf32_Shdr*)(binary+shoff[5]);
	int cnt=*((short*)(binary+48));
	printf("%d\n",cnt);
	for(int i=0;i<64;i++){
		printf("%d ",binary[i]);
		if((i+1)%8==0)printf("\n");
	}*/
	
	shdr=(Elf32_Shdr*)(binary+ehdr->e_shoff);
	int cnt=0;
        for(;cnt<(ehdr->e_shnum);cnt++){
//if(cnt>=2&&cnt<=3)
                printf("Read:%d:0x%x,0x%x\n",cnt,shdr[cnt].sh_offset,shdr[cnt].sh_addr);
        }
        // get section table addr, section header number and section header size.

        // for each section header, output section number and section addr. 
        // hint: section number starts at 0.


        return 0;
}

