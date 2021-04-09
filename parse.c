#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse_objects.h"
#include "parse_functions.h"

#define PAGE_SIZE 4096


int size_of_memmap_array(memmap* maps){
    int size = 0;
    while (maps[size].name != NULL){
        size++;
    }
    return size;
}

void parseline(char* line, memmap* mm){
    char map_name[100], fd[10], dummy[200];

    unsigned long int mem_start, mem_end;
    char r, w, x;

    // Formatted Input - Scanf (Section 7.4 in C book)
    // Read in all desired variables
    sscanf(line, "%lx-%lx %c%c%cp %s %s %s %s",
            &mem_start, &mem_end, &r, &w, &x, dummy, fd, dummy, map_name);


    // Populate memmap struct
    mm->start_addr = mem_start;
    mm->r = r;
    mm->w = w;
    mm->x = x;
    mm->end_addr = mem_end;

    // Have to allocate space to copy the string fields
    char* name = (char *) malloc(100);
    strcpy(name, map_name);
    mm->name = name;

    char* linecpy = (char *) malloc(200);
    strcpy(linecpy, line);
    mm->original_input = linecpy; 

    // STUDENT DEFINED FUNCTIONS
     mm->length = memmap_size(*mm);
}

unsigned long int memmap_size(memmap mm) {
    int byte_per_address = 4;
    return (int)((mm.end_addr - mm.start_addr) * byte_per_address);
}

// Print contents of a memmap struct in a nice way.
void pprint(memmap mm){
    printf("Original text: %s", mm.original_input);
    printf(" Map name: %s\n Start address: %lu\n End address: %lu\n Permissions: %c%c%c\n Length: %d\n",
            mm.name, mm.start_addr, mm.end_addr, mm.r, mm.w, mm.x, mm.length);
    printf("Dependent attributes:\n Section %d\n\n",
            mm.section);
}


struct memmap* populate_mmap(){
    char line[200];
    struct memmap* maps = malloc(200 * sizeof(struct memmap));

    int i = 0;

    unsigned long int num_dummy;
    char char_dummy;
    char string_dummy[200];
    char map_name[200];
    while (fgets(line, 200, stdin) != NULL){
        // determine if the memory mapping has a name 
        // in the last column of the proc maps file
        strcpy(map_name, "unpopulated");
        sscanf(line, "%lx-%lx %c%c%cp %s %s %s %s",
            &num_dummy, &num_dummy, &char_dummy, &char_dummy, &char_dummy, 
            string_dummy, string_dummy, string_dummy, map_name);

        // Only include mappings with names in the final array of structs
        if (strcmp(map_name, "unpopulated") != 0) { 
            parseline(line, &maps[i]);
            i++;
        }
    }

    // STUDENT DEFINED FUNCTIONS
    // assign_sections(maps);
    // assign_filetypes(maps);

    return maps;
}

int main(){
    memmap* maps = populate_mmap();
    int size = size_of_memmap_array(maps);

    for (int i=0; i<5; i++){
        pprint(maps[i]);
    }
    return 0;
}
