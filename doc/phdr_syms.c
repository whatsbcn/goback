/*
   Copyright Chris Rohlf 2007
   http://em386.blogspot.com

   Updated: 7 September 2007

   This simple tool parses an ELF objects
   symbol table using the program header.
   It is useful on binaries that do not
   have a section header.
*/

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// x86 Linux Base Address
#define BASEADDR 0x8048000

#define ERR -1

int read_hash_nchain();
int loop_symbols(int count);

unsigned char *base_ptr = 0;

Elf32_Shdr strtab_section;
Elf32_Shdr dynsym_section;
Elf32_Shdr dynamic_section;
Elf32_Shdr hash_section;

int main(int argc, char *argv[])
{

int fd;
int count;

size_t len;

struct stat binary;

	if(argc != 2)
	{
		printf("%s [binary]\n", argv[0]);
		return ERR;
	}

        if((fd = open(argv[1], O_RDONLY)) == ERR)
        {
                fprintf(stdout, "Could not open the file %s\n", argv[1]);
                printf("[!] open()\n");
                return ERR;
        }

        if((fstat(fd, &binary)))
        {
                close(fd);
                printf("[!] fstat()\n");
                return ERR;
        }

        if((base_ptr = (unsigned char *)malloc(binary.st_size)) == NULL)
        {
                close(fd);
                printf("[!] malloc()\n");
                return ERR;
        }
                memset(base_ptr, 0x0, binary.st_size);

        if((read(fd, base_ptr, binary.st_size)) < binary.st_size)
        {
                free(base_ptr);
                close(fd);
                printf("[!] read()\n");
                return ERR;
        }

Elf32_Ehdr *ehdr = (Elf32_Ehdr *) base_ptr;
Elf32_Phdr *phdr = (Elf32_Phdr *) base_ptr + ehdr->e_phoff;
Elf32_Dyn  *dyn  = 0;

        if(ehdr->e_ident[EI_MAG0] != 0x7f && ehdr->e_ident[EI_MAG1] != 'E' &&
                        ehdr->e_ident[EI_MAG2] != 'L' && ehdr->e_ident[EI_MAG3] != 'F')
        {
                printf("[!] Not an ELF object\n");
                return ERR;
        }

/*
 Iterate through the program headers to find dynamic segment
*/
for(count=0; count < ehdr->e_phnum; count++)
{
        phdr = (Elf32_Phdr *)(base_ptr + ehdr->e_phoff + (count * ehdr->e_phentsize));

        if(phdr->p_type == PT_DYNAMIC)
        {
		/* The dynamic section is the same address of DYNAMIC segments p_vaddr */
                dynamic_section.sh_offset = phdr->p_vaddr;
                printf("\nDynamic Segment Found at %08x\n", phdr->p_vaddr);
                break;
        }
}

if(phdr != 0)
{
	/* Iterate through the dynamic segment */
        for(count=0; count < phdr->p_filesz / sizeof(Elf32_Dyn); count++)
        {
                dyn = (Elf32_Dyn *)(base_ptr + phdr->p_offset + (count * sizeof(Elf32_Dyn)));

                if(dyn->d_tag == DT_NULL)
                {
                        break;
                }

                switch(dyn->d_tag)
                {
		   /* String table */
     	           case DT_STRTAB:

                        if(ehdr->e_type == ET_EXEC)
                        {
                                strtab_section.sh_offset = (dyn->d_un.d_val - BASEADDR);
                        }
                        if(ehdr->e_type == ET_DYN)
                        {
                                strtab_section.sh_offset = dyn->d_un.d_val;
                        }

                        printf("Strtab section found at %08x\n", strtab_section.sh_offset);

        	    break;

		    case DT_HASH:

                        if(ehdr->e_type == ET_EXEC)
                        {
                                hash_section.sh_offset = (dyn->d_un.d_val - BASEADDR);
                        }
                        if(ehdr->e_type == ET_DYN)
                        {
                                hash_section.sh_offset = dyn->d_un.d_val;
                        }
			
                        printf("Hash table section found at %08x\n", hash_section.sh_offset);
	
		    break;

   		    /* Symbol Table */
                    case DT_SYMTAB:

                        if(ehdr->e_type == ET_EXEC)
                        {
                                dynsym_section.sh_offset = dyn->d_un.d_val - BASEADDR;
                        }
                        if(ehdr->e_type == ET_DYN)
                        {
                                dynsym_section.sh_offset = dyn->d_un.d_val;
                        }

                        printf("Symbol table section found at %08x\n", dynsym_section.sh_offset);

                    break;
                }
        }

	if(strtab_section.sh_offset != 0 && dynsym_section.sh_offset != 0 && hash_section.sh_offset != 0)
	{
	        len = read_hash_nchain();
	        loop_symbols(len);
	}
}

printf("... Done\n");

free(base_ptr);
close(fd);

return 0;
}

/* Read the nchain value from the hash table */
int read_hash_nchain()
{

int *sym_count = (int *)((char *)base_ptr + hash_section.sh_offset + sizeof(int));

return *sym_count;
}

int loop_symbols(int entries)
{

int count;

Elf32_Sym *sym = 0;

printf("\n# Value\t Size\t  Bind\t Type\tName\n");
printf("----------------------------------------\n");

for(count=0; count < entries; count++)
{
	sym = (Elf32_Sym *)(base_ptr + dynsym_section.sh_offset + (count * sizeof(Elf32_Sym)));
	
	printf("%d %08x %08d ", count, sym->st_value, sym->st_size);

	switch(ELF32_ST_BIND(sym->st_info))
	{
		case STB_LOCAL: printf("LOCAL "); break;
		case STB_GLOBAL: printf("GLOBAL "); break;
		case STB_WEAK: printf("WEAK "); break;
		case STB_NUM: printf("NUM "); break;
		case STB_LOOS: printf("LOOS "); break;
		case STB_HIOS: printf("HIOS "); break;
		case STB_LOPROC: printf("LOPROC "); break;
		case STB_HIPROC: printf("HIPROC "); break;
		default: printf("UNKNOWN "); break;
	}

	switch(ELF32_ST_TYPE(sym->st_info))
	{
		case STT_NOTYPE: printf("NOTYPE "); break;
		case STT_OBJECT: printf("OBJECT "); break;
		case STT_FUNC:  printf("FUNC "); break;
		case STT_SECTION: printf("SECTION "); break;
		case STT_FILE: printf("FILE "); break;
		case STT_COMMON: printf("COMMON "); break;
		case STT_TLS: printf("TLS "); break;
		case STT_NUM: printf("NUM "); break;
		case STT_LOOS: printf("LOOS "); break;
		case STT_HIOS: printf("HIOS "); break;
		case STT_LOPROC: printf("LOPROC "); break;
		case STT_HIPROC: printf("HIPROC "); break;
		default: printf("UNKNOWN "); break;
	}

	printf("%s \n", base_ptr + strtab_section.sh_offset + sym->st_name);
}

return 0;
}
