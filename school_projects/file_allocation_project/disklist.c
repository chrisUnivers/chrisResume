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


typedef struct list_path
{
	char *rel_path;
	struct list_path *next;
} list_path;

typedef struct node_directory
{
    int my_fst_lc;
    int iam_active;
	char *dir_name;
	list_path *my_path_li;
} node_directory;

typedef struct list_directories
{
    node_directory val_adirecto;
    int count;
    struct list_directories *next;
}list_directories;

u_int32_t create_lt_end(u_int32_t arr_b_end[], int len_arr);
struct list_directories *add_to_list(struct list_directories *the_list, struct node_directory new_node, char sub_nm[]);
struct list_directories *show_content(int dir_phyc_cl, struct list_directories *dir_li, char *amap_ptr);
int shift_a_value(u_int32_t a_hex_value, int s_amount, int val_get);
struct list_directories *create_path_li(int srch_lg_cl, int mtch_lg_cl, struct list_directories *given_li);
int show_path(int lg_cl_find, struct list_directories *a_li_dir);


struct list_directories *add_to_list(struct list_directories *the_list, struct node_directory new_node, char sub_nm[])
{
    struct list_directories *current = malloc(sizeof(struct list_directories));
    current = the_list;
    struct list_directories *temp_dir = malloc(sizeof(struct list_directories));
	struct list_path *temp_path = malloc(sizeof(struct list_path));


    if (the_list->count == 0)
    {
        the_list->val_adirecto.my_fst_lc = new_node.my_fst_lc;
        the_list->val_adirecto.iam_active = 1;
		the_list->val_adirecto.my_path_li = temp_path;
		the_list->val_adirecto.my_path_li->rel_path = strdup(sub_nm);
        the_list->count += 1;
        the_list->next = NULL;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = temp_dir;
        temp_dir->val_adirecto.my_fst_lc = new_node.my_fst_lc;
        temp_dir->val_adirecto.iam_active = 1;
		temp_dir->val_adirecto.dir_name = strdup(sub_nm);
		temp_dir->val_adirecto.my_path_li = temp_path;
		temp_dir->val_adirecto.my_path_li->rel_path = strdup(sub_nm);
        temp_dir->next = NULL;
        the_list->count += 1;
		// printf("first logical cluster: %d, %d\n", temp_dir->val_adirecto.my_fst_lc, the_list->count);

        
    }
	return the_list;
}
struct list_directories *show_content(int dir_phyc_cl, struct list_directories *dir_li, char *amap_ptr)
{
	/*
		dir_phyc_cl: the physical sector i.e: 33 + 101 - 2 = 132 so
		dir_phyc_cl = 132 or 33 + 17 - 2 = 48 for testNew.im a
		for the root directory dir_phyc_cl = 19
	*/
	int dir_offset;
	int simple_int;
	int search_done;
	int a_file_size;
	int sz_index;
	int date_ind;
	int a_year, a_mon;
	int a_day, a_hour, a_min;
    char hex_to_char;
    char *fl_nm_ptr;
	char *dir_nm_ptr;
    char name_of_file[13];
	char name_of_dir[13];
	int dir_index; // replace hex_index
	unsigned int value_hex; // replace ahex_val
    unsigned int fst_byte;
    unsigned int show_char;
	unsigned int cur_dir_hex;
	int temp_index;
	int lc_ind;
	int pa_lg_cl;
	int f_l_cluster;
	int no_dot = 1;
    int index_arr = 0;
	u_int32_t arr_frst_lc[2];
	u_int32_t arr_date[2];
	u_int32_t arr_fl_sz[4];
	u_int32_t file_cr_dt_hex;
    struct node_directory temp_dir_node;
	value_hex = 0x00;
	dir_index = 0;
	search_done = 1;
	dir_offset = dir_phyc_cl * 512;
	pa_lg_cl = (dir_phyc_cl + 2) - 33;
	for (int i = 0; search_done > 0; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			value_hex = (unsigned)amap_ptr[dir_offset] & 0xffU;
            fst_byte = (unsigned)amap_ptr[dir_offset] & 0xffU;
			cur_dir_hex = (unsigned)amap_ptr[dir_offset] & 0xffU;
			if (value_hex == 0)
			{
				search_done = 0;
				break;
			}
			dir_index = dir_offset + 11;
			value_hex = (unsigned)amap_ptr[dir_index] & 0xffU;
			if (fst_byte != 0xE5)// checks if the directory entry is not free, free meaning the entry is empty.
			{
                //Need to print the name of the file
                lc_ind = dir_offset + 26;
				arr_frst_lc[0] = (unsigned)amap_ptr[lc_ind] & 0xffU;
				lc_ind = dir_offset + 27;
				arr_frst_lc[1] = (unsigned)amap_ptr[lc_ind] & 0xffU;
				f_l_cluster =  create_lt_end(arr_frst_lc, 2);
                if (value_hex == 0x00 && f_l_cluster != 0x00 && f_l_cluster != 1)// value_hex == 0 mean this is a file
                {
                    // f_l_cluster == 0 then this is the root directory
                    temp_index = dir_offset;
                    for (int l = 0; l < 12; l++)
                    {
                        show_char = (unsigned)amap_ptr[temp_index] & 0xffU;
                        if (show_char != 0x20)
                        {
                            hex_to_char = show_char;
                            name_of_file[index_arr] = hex_to_char;
							index_arr += 1;
							if (index_arr == 8 && (no_dot == 1))
							{
								name_of_file[index_arr] = '.';
								index_arr += 1;
								no_dot = 0;
							}
                            // printf("%c ", show_char);
                        }
                        else
                        {
							if (no_dot == 1)
							{
								name_of_file[index_arr] = '.';
								no_dot = 0;
								index_arr += 1;
							}
                        }
						temp_index += 1; // temp_index indexes the mapping pointer
                    }
					sz_index = dir_offset + 28;
					arr_fl_sz[0] = (unsigned)amap_ptr[sz_index] & 0xffU;
					sz_index += 1;
					arr_fl_sz[1] = (unsigned)amap_ptr[sz_index] & 0xffU;
					sz_index += 1;
					arr_fl_sz[2] = (unsigned)amap_ptr[sz_index] & 0xffU;
					sz_index += 1;
					arr_fl_sz[3] = (unsigned)amap_ptr[sz_index] & 0xffU;
					a_file_size = create_lt_end(arr_fl_sz, 4);
					/* DATES */
					date_ind = dir_offset + 16;
					arr_date[0] = (unsigned)amap_ptr[date_ind] & 0xffU;
					date_ind += 1;
					arr_date[1] = (unsigned)amap_ptr[date_ind] & 0xffU;
					file_cr_dt_hex = create_lt_end(arr_date, 2);
					a_year = shift_a_value(file_cr_dt_hex, 9, 1);
					a_mon = shift_a_value(file_cr_dt_hex, 5, 2);
					a_day = shift_a_value(file_cr_dt_hex, 0, 3);
					a_year += 1980;

					date_ind = dir_offset + 14;
					arr_date[0] = (unsigned)amap_ptr[date_ind] & 0xffU;
					date_ind += 1;
					arr_date[1] = (unsigned)amap_ptr[date_ind] & 0xffU;
					file_cr_dt_hex = create_lt_end(arr_date, 2);
					a_hour = shift_a_value(file_cr_dt_hex, 11, 4);
					a_min = shift_a_value(file_cr_dt_hex, 0, 5);
                    name_of_file[index_arr] = '\0';
                    fl_nm_ptr = strdup(name_of_file);

                    printf("%-5s%-10d%-15sYY:%-5dMM:%d DD:%-2d %d:%d", "F", a_file_size,fl_nm_ptr, 
								a_year, a_mon, a_day, a_hour, a_min);
                    printf("\n");
                }
				//value_hex == 16 means this is a directory
                else if (value_hex == 0x10 && (f_l_cluster != 0x00) && (f_l_cluster != 1))
                {
                    if (cur_dir_hex != 0x2e) // 0x2e ->ascii => "." so current directory.
                    {
                        // printf("DIRECTORY   ** %d **\n", f_l_cluster);
						temp_index = dir_offset;
						for (int l = 0; l < 12; l++) // assigns name of directory to name_of_dir
						{
							show_char = (unsigned)amap_ptr[temp_index] & 0xffU;
							if (show_char != 0x20)
							{
								hex_to_char = show_char;
								name_of_dir[index_arr] = hex_to_char;
								index_arr += 1;
								if (index_arr == 8 && (no_dot == 1))
								{
									// name_of_dir[index_arr] = '.';
									index_arr += 1;
									no_dot = 0;
								}
								// printf("%c ", show_char);
							}
							else
							{
								if (no_dot == 1)
								{
									// name_of_dir[index_arr] = '.';
									no_dot = 0;
									index_arr += 1;
								}
							}
							temp_index += 1; // temp_index indexes the mapping pointer
						}
						date_ind = dir_offset + 24;
						arr_date[0] = (unsigned)amap_ptr[date_ind] & 0xffU;
						date_ind += 1;
						arr_date[1] = (unsigned)amap_ptr[date_ind] & 0xffU;
						file_cr_dt_hex = create_lt_end(arr_date, 2);
						a_year = shift_a_value(file_cr_dt_hex, 9, 1);
						a_mon = shift_a_value(file_cr_dt_hex, 5, 2);
						a_day = shift_a_value(file_cr_dt_hex, 0, 3);
						a_year += 1980;

						date_ind = dir_offset + 22;
						arr_date[0] = (unsigned)amap_ptr[date_ind] & 0xffU;
						date_ind += 1;
						arr_date[1] = (unsigned)amap_ptr[date_ind] & 0xffU;
						file_cr_dt_hex = create_lt_end(arr_date, 2);
						a_hour = shift_a_value(file_cr_dt_hex, 11, 4);
						a_min = shift_a_value(file_cr_dt_hex, 0, 5);
						name_of_dir[index_arr] = '\0';
						dir_nm_ptr = strdup(name_of_dir);
						printf("%-5s%-10d%-15sYY:%-5dMM:%d DD:%-2d %d:%d", "D", 0, dir_nm_ptr, 
								a_year, a_mon, a_day, a_hour, a_min);
						// printf("%-5s%-10d%-15s%-5s", "D", 0, dir_nm_ptr, "DATE");
						printf("\n");
                        temp_dir_node.my_fst_lc = f_l_cluster; //f_l_cluster = 17 in testNewIm for SUB1
                        temp_dir_node.iam_active = 1;
                        /*
                            add the directory to the list of directories
                            the content of the directories in this
                            list will be printed separately. I think
                        */
						/* Creating the list of this subdirectory path */
						for (int a = 0; a < strlen(name_of_dir); a++)
						{
							name_of_dir[a] = tolower(name_of_dir[a]);
						}
						if (dir_phyc_cl == 19)// parent is the root directory
						{
							/* name_of_dir = name of a subdirectory in the root dir*/
							dir_li = add_to_list(dir_li, temp_dir_node, name_of_dir);
							// sub_dirs = show_path(f_l_cluster, dir_li);
						}
						else
						{
							dir_li = add_to_list(dir_li, temp_dir_node, name_of_dir);
							dir_li = create_path_li(pa_lg_cl, f_l_cluster, dir_li);
							// sub_dirs = show_path(f_l_cluster, dir_li);
						}
                    }
                    else
                    {
                        simple_int = 2;
                    }
                }
                else
                {
                    simple_int = 2;
                }
			}
			else
			{
				simple_int = 5;
			}
			index_arr = 0;
			no_dot = 1;
			dir_offset += 32;
		}
		dir_offset = dir_phyc_cl + ((i + 1) * 512);// next sector;
	}
	return dir_li;
}

struct list_directories *create_path_li(int srch_lg_cl, int mtch_lg_cl, struct list_directories *given_li)
{

	struct list_directories *paren_dir = malloc(sizeof(struct list_directories));
	struct list_directories *chil_dir = malloc(sizeof(struct list_directories));
	struct list_path *temp_path = malloc(sizeof(struct list_path));
	struct list_path *cur_path = malloc(sizeof(struct list_path));
	struct list_path *nw_path_node = malloc(sizeof(struct list_path));
	int cntnue = 0;
	int cntnue_b = 0;

	if (paren_dir == NULL)
	{
		printf("ERROR IN CREATE_PATH_LI WHEN\n");
		exit(0);
	}
	else if (chil_dir == NULL)
	{
		printf("ERROR IN CREATE_PATH_LI WHEN\n");
		exit(0);
	}
	else if (temp_path == NULL)
	{
		printf("ERROR IN CREATE_PATH_LI WHEN\n");
		exit(0);
	}
	else
	{
		cntnue = 1;
		cntnue_b = 1;
	}

	paren_dir = given_li;
	chil_dir = given_li;

	while (paren_dir != NULL && (cntnue == 1))
	{
		if (paren_dir->val_adirecto.my_fst_lc == srch_lg_cl)
		{  
			// found the parent directory
			if (paren_dir->val_adirecto.my_path_li != NULL)
			{
				// printf("Now what\n");
				temp_path = paren_dir->val_adirecto.my_path_li;
				// printf("Now what 2\n");
			}
			else
			{
				printf("This is what\n");
				// temp_path->rel_path = strdup(paren_dir->val_adirecto.dir_name);
				// printf("This is what 2\n");
			}
			cntnue = 0;

		}
		else
		{
			paren_dir = paren_dir->next;
		}
	}

	while (chil_dir != NULL && (cntnue_b == 1))
	{
		if (chil_dir->val_adirecto.my_fst_lc == mtch_lg_cl)
		{
			// found the child directory;
			chil_dir->val_adirecto.my_path_li = temp_path;
			cur_path = chil_dir->val_adirecto.my_path_li;
			while (cur_path->next != NULL)
			{
				cur_path = cur_path->next;
			}
			if (cur_path->next == NULL)
			{
				cur_path->next = nw_path_node;
				nw_path_node->rel_path = strdup(chil_dir->val_adirecto.dir_name);
			}
			cntnue_b = 0;
		}
		else
		{
			chil_dir = chil_dir->next;
		}
	}
	
	

	return given_li;
}

int show_path(int lg_cl_find, struct list_directories *a_li_dir)
{

	struct list_directories *curr = malloc(sizeof(struct list_directories));
	struct list_path *sub_pat = malloc(sizeof(struct list_path));
	int to_find = 0;
	int keep_going = 1;

	if (curr == NULL || (sub_pat == NULL))
	{
		printf("ERROR IN SHOW_PATH\n");
		exit(0);
	}
	else
	{
		to_find = lg_cl_find;
	}
	// printf("\n****** PRINTING SUB PATHS *******\n");
	curr = a_li_dir;
	while (curr != NULL && (keep_going == 1))
	{
		if (curr->val_adirecto.my_fst_lc == to_find)
		{
			if (curr->val_adirecto.my_path_li != NULL)
			{
				sub_pat = curr->val_adirecto.my_path_li;
				while (sub_pat != NULL)
				{
					printf("/%s", sub_pat->rel_path);
					sub_pat = sub_pat->next;
				}
				
				keep_going = 0;
			}
			else
			{
				printf("ERROR IN SHOW_PATH ON sub_pat\n");
				exit(0);
			}
		}
		else
		{
			curr = curr->next;
		}
	}
	printf("\n\n==================\n\n");
	// printf("\n****** DONE PRINTING SUB PATHS *******\n");

	

	return 0;
}


int shift_a_value(u_int32_t a_hex_value, int s_amount, int val_get)
{
	/*
		val_get:
			1: get year
			2: get month
			3: get day, 4: get hour 5: get minute
	*/
	u_int32_t b_rt_value = 0x00;
	u_int32_t c_hex_val;
	c_hex_val = a_hex_value;
	int fn_rt;
	if (val_get == 1)
	{
		c_hex_val = c_hex_val >> s_amount;
		b_rt_value = b_rt_value | c_hex_val;
		fn_rt = b_rt_value;
	}
	else if (val_get == 2)
	{
		b_rt_value = 0x00F;
		c_hex_val = c_hex_val >> s_amount;
		b_rt_value = b_rt_value & c_hex_val;
		fn_rt = b_rt_value;
	}
	else if (val_get == 3)
	{
		b_rt_value = 0x0F;
		c_hex_val = c_hex_val >> s_amount;
		b_rt_value = b_rt_value & c_hex_val;
		fn_rt = b_rt_value;
	}
	else if (val_get == 4)
	{
		b_rt_value = 0x0F;
		c_hex_val = c_hex_val >> s_amount;
		b_rt_value = b_rt_value & c_hex_val;
		fn_rt = b_rt_value;
	}
	else
	{
		b_rt_value = 0x2F;
		c_hex_val = c_hex_val >> s_amount;
		b_rt_value = b_rt_value & c_hex_val;
		fn_rt = b_rt_value;
	}
	return fn_rt;
}
u_int32_t create_lt_end(u_int32_t arr_b_end[], int len_arr)
{
	u_int32_t fin_val = 0x00;
	u_int32_t output_hex;
	u_int16_t output_sixt_hex;
	int num_bits;
	int rt_int;
	num_bits = 8 * len_arr;
	for (int i = 0; i < len_arr; i++)
	{
		fin_val = fin_val | arr_b_end[i];
		if (i < (len_arr - 1))
		{
			fin_val = fin_val << 8;
		}
		// printf("---- 0x%x", fin_val);
	}
	// printf("\n--- fin_val: 0x%x ----\n", fin_val);
	if (num_bits == 16)
	{
		output_sixt_hex = __bswap_16 (fin_val);
		// printf("\n--- output_hex: 0x%x ----\n", output_sixt_hex);
		rt_int = output_sixt_hex;
		// printf("---\nMAKE LT ENDIAN will return: %d\n---\n", rt_int);
	}
	else if (num_bits == 32)
	{
		output_hex = __bswap_32 (fin_val);
		// printf("\n--- output_hex: 0x%x ----\n", output_hex);
		rt_int = output_hex;
		// printf("---\nMAKE LT ENDIAN will return: %d\n---\n", rt_int);
	}
	else 
	{
		printf("ERROR IN MAKE_LT_ENDIAN: UNWANTED NUMBER OF BITS");
	}
	return rt_int;
}
int main(int argc, char *argv[])
{
	int fd;
	struct stat sb;

	fd = open(argv[1], O_RDWR);
	fstat(fd, &sb);
	// printf("Size: %lu\n\n", (uint64_t)sb.st_size);

	char * a_ptr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
	if (a_ptr == MAP_FAILED) {
		printf("Error: failed to map memory\n");
		exit(1);
	}
	// p++;
	char *fname;
	fname = strdup(argv[1]);
	// printf("a hex char of: %1x \n", (unsigned)p[9] & 0xffU);
	// printf("The values are: %c\n", 0x30);
	int num_dir_in_list = 0;
	int phsc_cluster = 0;
	int temp_lc = 0;
	int complex_int = 0;// a do nothing int
	int shw_path;
	unsigned int t_hex;
	char root_name[13];
	struct list_directories *my_dir_list = malloc(sizeof(struct list_directories));
	struct list_directories *cur = malloc(sizeof(struct list_directories));
	struct node_directory root_directory;

	t_hex = (unsigned)a_ptr[9888] & 0xffU;
	// printf("Trex is: 0x%x \n", t_hex);	
	root_name[0] = 'r';
	root_name[1] = 'o';
	root_name[2] = 'o';
	root_name[3] = 't';
	root_name[4] = '\0';
	root_directory.my_fst_lc = 19;
	my_dir_list = add_to_list(my_dir_list, root_directory, root_name);
	cur = my_dir_list;
	num_dir_in_list = my_dir_list->count;

	if (cur == NULL)
	{
		printf("\nERROR: cur and thus my_dir_list is null\n");
		return 0;
	}
	int counting = 0;
	// printf("Root\n\n");
	// printf("==================\n");
	while (cur != NULL)
	{
		// printf("co: %d\n", counting);
		if (num_dir_in_list > 0)
		{
			if (cur->val_adirecto.my_fst_lc == 19)
			{
				printf("Root\n\n");
				printf("==================\n\n");
				phsc_cluster = 19;
				my_dir_list = show_content(phsc_cluster, my_dir_list, a_ptr);
				// printf("*** BACK ***\n");
				cur->val_adirecto.iam_active = 0;
				// printf("*** REALLY BACK ***\n");
			}
			else if (cur->val_adirecto.my_fst_lc <= 1)
			{
				printf("\nERROR: THE LOGICAL CLUSTER OF <= 1 SHOULD NOT");
				printf("BE ALLOWED SINCE THESE SECTORS ITS RESERVED\n");
				return 0;
			}
			else
			{
				temp_lc = cur->val_adirecto.my_fst_lc;
				phsc_cluster = 33 + temp_lc - 2;
				// printf("Currently be here\n");
				printf("\n");
				shw_path = show_path(temp_lc, my_dir_list);
				my_dir_list = show_content(phsc_cluster, my_dir_list, a_ptr);
			}

		}
		else
		{
			complex_int = 2;
		}
		my_dir_list->count -= 1;
		num_dir_in_list = my_dir_list->count;
		// printf("[[[ Cur lc: %d, dir_list count: %d ]]]]\n", cur->val_adirecto.my_fst_lc, my_dir_list->count);
		cur = cur->next;
		counting++;
	}
	
	// printf("-- TESTING: 0x%x--\n", testa);
	// if (testa == 235)
	// {
	// 	printf(">> testa is 235 in decimal <<\n");
	// }
	// return 0;
	
	// printf("a hex char of: %1x \n", c);
	
	//p[0] = 0x00; // an example to manipulate the memory, be careful when you try to uncomment this line because your image will be modified
	
	// printf("modified the memory\n");
	munmap(a_ptr, sb.st_size); // the modifed the memory data would be mapped to the disk image
	close(fd);
	return 0;
}