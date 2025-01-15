#ifndef _DIRNODES_H_
#define _DIRNODES_H_

typedef struct node_dir
{
    int my_fst_lc;
    int entry_offset; // each entry in a directory is 32 bytes.
	char *dir_name;
} node_dir;

typedef struct list_dir
{
    node_dir ith_dir;
    int count;
    struct list_dir *next;
}list_dir;


void add_to_list(struct list_dir *the_list, struct node_dir new_node);
/**Returns the last_entry's offset in the_list*/
int remove_last(struct list_dir *the_list, int the_list_count);
int get_last_entry_offset(struct list_dir *the_list, int the_list_count);

#endif