#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <byteswap.h>
#include "dirNodes.h"

void add_to_list(struct list_dir *the_list, struct node_dir new_node)
{
    struct list_dir *current = malloc(sizeof(struct list_dir));
    current = the_list;
    struct list_dir *temp_dir = malloc(sizeof(struct list_dir));
	


    if (the_list->count == 0)
    {
        the_list->ith_dir.my_fst_lc = new_node.my_fst_lc;
        the_list->ith_dir.dir_name = new_node.dir_name;
        the_list->ith_dir.entry_offset = new_node.entry_offset;
        the_list->count += 1;
        the_list->next = NULL;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = temp_dir; // allocate memory for current->next
        temp_dir->ith_dir.my_fst_lc = new_node.my_fst_lc;
		// temp_dir->ith_dir.dir_name = strdup(sub_nm);
        temp_dir->next = NULL;
        the_list->count += 1;
		// printf("first logical cluster: %d, %d\n", temp_dir->ith_dir.my_fst_lc, the_list->count);

    }
}
/**Returns the last_entry's offset in the_list*/
int remove_last(list_dir *the_list, int the_list_count)
{
    list_dir *current = malloc(sizeof(struct list_dir));
    int count = 0;
    int bool_return = 0;
    int rt_offset = 0;
    current = the_list;

    if (the_list_count > 1)
    {
        while ((count + 1) < the_list_count)
        {
            current = current->next;
            count++;
        }
        rt_offset = current->ith_dir.entry_offset;
        the_list->count -= 1;
        free(current->next);

    }
    else
    {
        the_list->count -= 1;
        rt_offset = current->ith_dir.entry_offset;
        current = NULL;
    }
    return rt_offset;
}

int get_last_entry_offset(struct list_dir *the_list, int the_list_count)
{
    list_dir *current = malloc(sizeof(struct list_dir));
    int track = 0;
    int bool_return = 0;
    int rt_entry_offset = 0;
    current = the_list;

    if (the_list_count > 0)
    {
        while ((track + 1) < the_list_count)
        {
            current = current->next;
            track++;
        }
        rt_entry_offset = current->ith_dir.entry_offset;
        return rt_entry_offset;
    }
    else
    {
        fprintf(stderr , "The list of directories is empty");
        exit(-1);
    }
}

