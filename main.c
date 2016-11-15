#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <elf.h>

void dump_buffer(void* buffer){
	
}
	
void dump_elf_hdr(Elf64_Ehdr* elf_hdr){
	if(elf_hdr == NULL){
		return;	
	}

	unsigned char *e_ident = elf_hdr->e_ident;
	if(strncmp(e_ident,ELFMAG,SELFMAG) != 0){
		printf("that isn't a elf file \n");
		return;
	}

	switch(e_ident[EI_CLASS]){
		case ELFCLASSNONE:{
			break;
		}	
		case ELFCLASS32:{
			printf("32-bit objects \n");
			break;
		}
		case ELFCLASS64:{
			printf("64-bit objects \n");
			break;
		}
	}

	switch(e_ident[EI_DATA]){
		case ELFDATANONE:{
			printf("invalid data encoding \n");
			break;
		}
		case ELFDATA2LSB:{
			printf("2's complement,little endian \n");
			break;
		}
		case ELFDATA2MSB:{
			printf("2's complement,big endian \n");
			break;
		}
	}

	printf("e_ident[EI_VERSION] = %d \n",e_ident[EI_VERSION]);
	printf("e_ident[EI_PAD] = %d \n",e_ident[EI_PAD]);

	char* str_e_type = NULL;
	switch(elf_hdr->e_type){
		case ET_NONE:{
			str_e_type = "no file type";
			break;
		}
		case ET_REL:{
			str_e_type = "relocatable file";
			break;
		}
		case ET_EXEC:{
			str_e_type = "executable file";
			break;
		}
		case ET_DYN:{
			str_e_type = "shared object file";
			break;
		}
		case ET_CORE:{
			str_e_type = "core file";
			break;
		}
		case ET_LOPROC:
		case ET_HIPROC:{
			str_e_type = "processor-specific";
			break;
		}
	}
	
	char* str_e_machine = NULL;
	switch(elf_hdr->e_machine){
		case ET_NONE:{
			str_e_machine = "no machine";
			break;
		}
		case EM_M32:{
			str_e_machine = "AT&T WE32100A";
			break;
		}
		case EM_SPARC:{
			str_e_machine = "SPARC";
			break;
		}
		case EM_386:{
			str_e_machine = "intel architecture";
			break;
		}
		case EM_68K:{
			str_e_machine = "motorola 68000";
			break;
		}
		case EM_88K:{
			str_e_machine = "motorola 88000";
			break;
		}
		case EM_860:{
			str_e_machine = "intel 80860";
			break;
		}
		case EM_MIPS:{
			str_e_machine = "MIPS RS3000 Big-Endian";
			break;
		}
	/*	case EM_MIPS_RS4_BE:{
			str_e_machine = "MIPS RS4000 Big-Endian";
			break;
		}
		*/
	}
	
	printf("elf_hdr->e_type = %s \n",str_e_type);
	printf("elf_hdr->e_machine = %s \n",str_e_machine);
	printf("elf_hdr->e_machine = %d \n",elf_hdr->e_machine);
	printf("elf_hdr->e_version = %d \n",elf_hdr->e_version);
	printf("elf_hdr->e_entry = %#x \n",elf_hdr->e_entry);
	printf("elf_hdr->e_phoff = %d \n",elf_hdr->e_phoff);
	printf("elf_hdr->e_shoff = %d \n",elf_hdr->e_shoff);
	printf("elf_hdr->e_ehsize = %d \n",elf_hdr->e_ehsize);
	printf("elf_hdr->e_phentsize = %d bytes \n",elf_hdr->e_phentsize);
	printf("elf_hdr->e_phnum = %d \n",elf_hdr->e_phnum);
	printf("elf_hdr->e_shentsize = %d bytes \n",elf_hdr->e_shentsize);
	printf("elf_hdr->e_shnum = %d \n",elf_hdr->e_shnum);
	//printf("elf_hdr->shstrndx = ");
}

void dump_elf_section(Elf64_Ehdr* elf_hdr,FILE* elf_file){
	size_t all_section_size = elf_hdr->e_shnum * elf_hdr->e_shentsize;
	char* buffer = (char*)malloc(all_section_size);

	fseek(elf_file,elf_hdr->e_shoff,SEEK_SET);
    	if(ftell(elf_file) != elf_hdr->e_shoff){
       		printf("error \n");
        	exit(1);
    	}

	if(fread(buffer,all_section_size,1,elf_file) != 1){
		perror("dump_elf_section");
		exit(1);
	}

	Elf64_Shdr* shdr = (Elf64_Shdr*)buffer;

	for(int i = 0;i < elf_hdr->e_shnum;i++){
		printf("%d 's section name = %s \n",shdr->sh_name);
		shdr++;
	}

	free(buffer);
}

int main(int argc,char* argv[]){
	
	if(argc < 2){
		printf("invalid elf file name \n");
		exit(1);
	}

	char* elf_file_name = argv[1];
	FILE* elf_file = NULL;

	if((elf_file = fopen(elf_file_name,"r")) == NULL){
		perror(NULL);
		exit(1);
	}

	char* buffer[sizeof(Elf64_Ehdr)];

	if(fread(buffer,sizeof(Elf64_Ehdr),1,elf_file) != 1){
		perror(NULL);
		exit(1);
	}

	Elf64_Ehdr* elf_hdr = (Elf64_Ehdr*)buffer;

	dump_elf_hdr(elf_hdr);
	dump_elf_section(elf_hdr,elf_file);

	return 0;
}
