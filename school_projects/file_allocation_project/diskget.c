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

int contains_input(char *getMap_ptr, char *file_find);
u_int32_t create_lt_end(u_int32_t arr_b_end[], int len_arr);
int twelve_in_fat(char *bMap_ptr, int nth_entry);
int write_to_afile(char *wMap_ptr, int nth_entry_a, int f_size, char *a_name);
int logical_cluster(int dir_offset, char *getMap_pt);
char *nameof(char *tMap_ptr, int entry_offset);


int contains_input(char *getMap_ptr, char *file_find)
{

    int root_offset = 9728; // sector 19 * 512 0x2600
    int a_dir_offset = 9728;
	int temp_dir_offset;
	int bool_use_temp_dir_offset = 0;
	int a_search_done = 1;
    char a_hex_to_char;
	int loca_in_fat;
	int wrt_rt;
	int fl_len = 0;
    char *fl_nm_ptr;
	char *give_name;
	char *end_of_dir_indicator;
	char *free_dir_indicator;
    char a_name_of_file[13];
	int a_dir_index; // replace hex_index
	int frst_byte_Fname;
	unsigned int subDir_or_file_indc; // replace ahex_val
    unsigned int a_fst_byte;
    unsigned int a_show_char;
	unsigned int a_cur_dir_hex;
	int a_temp_index;
	int num_of_sub_dirs = 0;
	int a_lc_ind; // a local cluster index
	int a_f_l_cluster; // allows us to find the 1st 512 bytes of a dir entry and where the next sector of a dir.
	u_int32_t arr_frst_lc[2]; // array that holds the 2 bytes that store the value of the first logical cluster
	u_int32_t arr_fl_size[4]; // an array that holds the 4 bytes that represent/store the size of the file.
	struct node_dir dir_node;
	struct list_dir *li_sub_dirs = malloc(sizeof(struct list_dir)); //IT DOES NOT STORE THE ROOT dir! List that holds subdirectories.
	struct node_dir temp_sub_dir;

	int rt_fst_lc = 0;
	int a_no_dot = 1;
    int a_index_arr = 0;
	int a_sz_index = 0;
	int fl_get_sz = 0;
	fl_len = strlen(file_find);
	end_of_dir_indicator = "END";
	free_dir_indicator = "FREE";

    for (int i = 0; a_search_done > 0; i++)
    {
        for (int j = 0; j < 16; j++) //16 because each directory has 16 entries of 32 bytes each.
        {

            frst_byte_Fname = (unsigned)getMap_ptr[a_dir_offset] & 0xffU;
            a_fst_byte = (unsigned)getMap_ptr[a_dir_offset] & 0xffU;
			a_cur_dir_hex = (unsigned)getMap_ptr[a_dir_offset] & 0xffU;
			if (frst_byte_Fname != 0x00 && (frst_byte_Fname != 0xE5))
			{
			
				a_dir_index = a_dir_offset + 11;
				subDir_or_file_indc = (unsigned)getMap_ptr[a_dir_index] & 0xffU;
				if (a_fst_byte != 0xE5)// if true, then directory entry contains file(s) or dir(s)
				{
					//Need to print the name of the file
					
					a_f_l_cluster =  logical_cluster(a_dir_offset, getMap_ptr); //this is the value with offset +26
					if (subDir_or_file_indc == 0x00 && a_f_l_cluster != 0x00 && a_f_l_cluster != 1) // See comments below.
					{
						/* subDir_or_file_indc == 0 then this is a file. 
						a_first_logical_cluster/a_f_l_cluster == 0 then this is the ROOT directory 
							this, a_f_l_cluster, is also the 101 value in FATsearchtep*/

						give_name = nameof(getMap_ptr, a_dir_offset);
						// printf("\nFile to find: %s\n", a_name_of_file);
						if (strncmp(file_find, give_name, fl_len) == 0)
						{
							printf("\nThe file: %s was found\n", give_name);
							rt_fst_lc = a_f_l_cluster;
							a_sz_index = a_dir_offset + 28;
							arr_fl_size[0] = (unsigned)getMap_ptr[a_sz_index] & 0xffU;
							a_sz_index += 1;
							arr_fl_size[1] = (unsigned)getMap_ptr[a_sz_index] & 0xffU;
							a_sz_index += 1;
							arr_fl_size[2] = (unsigned)getMap_ptr[a_sz_index] & 0xffU;
							a_sz_index += 1;
							arr_fl_size[3] = (unsigned)getMap_ptr[a_sz_index] & 0xffU;
							fl_get_sz = create_lt_end(arr_fl_size, 4);
							// printf("\nFound the file: %s, 1st logical cluster is: %d\n", a_name_of_file, a_f_l_cluster);
							// loca_in_fat = twelve_in_fat(getMap_ptr, a_f_l_cluster); 
							wrt_rt = write_to_afile(getMap_ptr, a_f_l_cluster, fl_get_sz, give_name);
							a_search_done = 0;
							j = 15;
						}
						else
						{
							printf("\nThe file: %s was not found\n", give_name);
						}
					}
					else
					{
						//subDir_or_file_indc == 0x10 then this is a directory
						if (subDir_or_file_indc == 0x10 && a_f_l_cluster != 0x00 && a_f_l_cluster != 1)
						{
							printf("I'am a subdirectory: %d", a_dir_offset);
							give_name = nameof(getMap_ptr, a_dir_offset);
							if (0)
							{
								int temp_one = 0; // code that used to be here is now the else of if(frst_byte_name != 0)
							}
							else
							{
								temp_sub_dir.entry_offset = a_dir_offset;
								/*sets a_dir_offset to the location that contains the content of this sub directory
									by adding -32 to a_dir_offset, we will get current sub dir info which is a duplicate*/
								a_dir_offset = ((33 + a_f_l_cluster -2 )*512); // - 32 
								temp_sub_dir.dir_name = give_name;
								temp_sub_dir.my_fst_lc = a_f_l_cluster;
								add_to_list(li_sub_dirs, temp_sub_dir);
								num_of_sub_dirs += 1; // remember to decrement when you remove the list
								// temp_dir_offset = a_dir_offset;
								// bool_use_temp_dir_offset = 1;
								// remove_last(li_sub_dirs, 0);
								int check_with = 0;
							}
							printf("I'am a subdirectory: %s", give_name);
						}
					}
				}
			}
			else
			{
				if (frst_byte_Fname == 0x00)
				{
					// if have reached end of this sub dir backup to parent dir
					//16 dirEntries - num of entries encountered. i starts at 0.
					a_dir_offset = remove_last(li_sub_dirs, num_of_sub_dirs); //return last_entry's offset in li_sub_dirs
					num_of_sub_dirs -= 1;
					// if (num_of_sub_dirs == 0)
					// {
					// 	// ACTS as if we have actually stepped through all 16 entries of this sub directory 
					// 	int remain_entries = (16 - (j+ 1));
					// 	int pad_dir_offset = (remain_entries * 32);
					// 	a_dir_offset += pad_dir_offset;
					// 	j = 15; // to end the 16 entries for loop.
					// }
					
				}
				// else if (frst_byte_Fname == 0xE5)
				// {
				// 	// if this is an empty directory entry. I.e the next entry might not be empty 
				// 	a_dir_offset = get_last_entry_offset(li_sub_dirs, num_of_sub_dirs);
				// 	remove_last(li_sub_dirs, num_of_sub_dirs);
				// 	num_of_sub_dirs -= 1;
				// }

				// else
				// {
				// 	fprintf(stderr , "Unkown entry value, see first byte of Filename at offset %d", a_dir_offset);
				// 	exit(-1);
				// }
			}
			a_index_arr = 0;
			a_no_dot = 1;
            a_dir_offset += 32; // increment to step through a directories entry(32 bytes/entry)
        }
        a_dir_offset = root_offset + ((i + 1)*512); // offset to the next sector.
    }

    return rt_fst_lc;
}

int write_to_afile(char *wMap_ptr, int nth_entry_a, int f_size, char *a_name)
{
	int num_sectors = f_size /512; // number of sectors needed to store this file
	// the final number of bytes to read > 0 if f_size is not divisible by 512
	int final_bytes = f_size % 512;
	int total_bytes = (num_sectors * 512) + final_bytes;
	int nth_in_physc; // 33 + nth_in_physc - 2. Calculation for the physical sector value.
	unsigned int in;
	int fst_lg_sect;
	int data_offset;
	int read_to = 0;
	int count_sect = 0;
	int read_from = 0;
	fst_lg_sect = nth_entry_a;
	data_offset = (33 + fst_lg_sect - 2) * 512;
	unsigned int a_show_char;
	char a_hex_val;
	char to_show_char[11];

	int bool_go = 1;

	FILE *fl_p;
	fl_p = fopen(a_name, "a");
	if (fl_p == NULL)
	{
		perror("The given file could not be created");
		exit(0);
	}

	printf("FIRST LOGICAL CLUSTER IS: %d\n", nth_entry_a);

	while (total_bytes > 0)
	{
		if (count_sect == 0) // first sector of the current file.
		{
			// The 1st 512 bytes of this file are stored at location (physical sector) * 512
			data_offset = (33 + fst_lg_sect - 2) * 512; 
			nth_in_physc = fst_lg_sect;
			if (total_bytes > 512)
			{
				read_to = 512;
			}
			else{read_to = final_bytes;}
			for (int i = 0; i < read_to; i++)
			{
				if (i == 0){count_sect += 1;}
				read_from = data_offset + i;
				fprintf(fl_p, "%c", wMap_ptr[read_from]);
			}
		}
		else
		{
			nth_in_physc = twelve_in_fat(wMap_ptr, nth_in_physc);
			data_offset = (33 + nth_in_physc - 2) * 512;
			if (total_bytes > 512)
			{
				read_to = 512;
			}
			else{read_to = final_bytes;}
			for (int j = 0; j < read_to; j++)
			{	
				if (j == 0){count_sect += 1;}
				read_from = data_offset + j;
				fprintf(fl_p, "%c", wMap_ptr[read_from]);
			}
		}
		if (count_sect < 3)
		{
			printf("READ FROM: %d\n", data_offset);
		}

		total_bytes -= read_to;
	}
	fclose(fl_p);
	printf("READ SECTORES: %d\n", count_sect);
	return 0;
}

// Given an arrary of bytes, create/get the little endian number
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

char *nameof(char *tMap_ptr, int entry_offset)
{
	int temp_index = 0;
	unsigned int show_char; // show_char is a hexidecimal value.
	char hex_to_char; // stores char representation of a hex value.
	int index_arr = 0;
	char name_of_entry[13];
	char end_of_dir[4];
	char free_dir[5];
	int bool_no_dot = 1;
	char *rtd_name;
	int first_byte_of_namefield = 1;
	int bool_end_of_dir = 0;
	int bool_free_dir = 0;
	temp_index = entry_offset;
	end_of_dir[0] = 'E';
	end_of_dir[1] = 'N';
	end_of_dir[2] = 'D';
	end_of_dir[3] = '\0';
	free_dir[0] = 'F';
	free_dir[1] = 'R';
	free_dir[2] = 'E';
	free_dir[3] = 'E';
	free_dir[4] = '\0';
	/* l < 12 because the Filename and its extention are 11 bytes long*/
	for (int l = 0; l < 12; l++) /* assign a_name_of_file the name of the current file.*/ 
	{
		show_char = (unsigned)tMap_ptr[temp_index] & 0xffU;
		if (first_byte_of_namefield == 1 && (show_char == 0x00))
		{
			// Then this directory entry is free and all remaining directory entries in this directory are also free.
			l = 11;
			bool_end_of_dir = 1;
			first_byte_of_namefield = 0;
		}
		else if (first_byte_of_namefield == 1 && (show_char == 0xE5))
		{
			// Then this directory entry is free.
			l = 11;
			bool_free_dir = 1;
			first_byte_of_namefield = 0;
		}
		else
		{
			first_byte_of_namefield = 0;
			if (show_char != 0x20)
			{
				hex_to_char = show_char;
				name_of_entry[index_arr] = hex_to_char;
				index_arr += 1;
				if (index_arr == 8 && (bool_no_dot == 1))
				{
					// when index_arr = 8, this is the starting position of a file's extension.
					name_of_entry[index_arr] = '.';
					index_arr += 1;
					bool_no_dot = 0;
				}
				// printf("%c ", show_char);
			}
			else
			{
				if (bool_no_dot == 1)
				{
					name_of_entry[index_arr] = '.';
					bool_no_dot = 0;
					index_arr += 1;
				}
			}
		}
		temp_index += 1; // temp_index indexes the mapping pointer
	}
	name_of_entry[index_arr] = '\0';
	if (bool_end_of_dir == 1)
	{
		rtd_name = strdup(end_of_dir);
	}
	else if (bool_free_dir == 1)
	{
		rtd_name = strdup(free_dir);
	}
	else
	{
		rtd_name = strdup(name_of_entry);
	}

	return rtd_name;
}

// returns the value of the logical cluster, i.e the first logical cluster
int logical_cluster(int dir_offset, char *getMap_pt)
{
	int lc_offset;
	int lc_cluster;
	u_int32_t arr_frst_lc[2];

	lc_offset = dir_offset + 26;
	arr_frst_lc[0] = (unsigned)getMap_pt[lc_offset] & 0xffU;
	lc_offset = dir_offset + 27;
	arr_frst_lc[1] = (unsigned)getMap_pt[lc_offset] & 0xffU;
	lc_cluster =  create_lt_end(arr_frst_lc, 2); // i.e first logical cluster, i.e 101 in FATsearchsteps pdf

	return lc_cluster;
}

int twelve_in_fat(char *bMap_ptr, int nth_entry)
{
	int twlv_bit_fat = 0;
	int is_even = 0;
	int eitBits_offset;
	int entry_ind;
	int temp_int_eit;
	int temp_int_four;
	int fourBits_offset;
	int no_use = 0;
	char arr_8bit[3];
	char arr_4bit[3];
	unsigned int eitBits_hex;
	unsigned int fourBits_hex;
	char fn_arr[4];

	is_even = nth_entry % 2;

	if (is_even == 0)
	{
		eitBits_offset = (3*nth_entry)/2;
		entry_ind = eitBits_offset + 512;
		eitBits_hex = (unsigned)bMap_ptr[entry_ind] & 0xffU;
		sprintf(arr_8bit, "%x", eitBits_hex);
		temp_int_eit = (int) strtol(arr_8bit, NULL, 16);
		
		fourBits_offset = 1 + (3*nth_entry)/2;
		entry_ind = fourBits_offset + 512;
		fourBits_hex = (unsigned)bMap_ptr[entry_ind] & 0xffU;
		sprintf(arr_4bit, "%x", fourBits_hex);
		temp_int_four = (int) strtol(arr_4bit, NULL, 16);

		if (temp_int_eit < 10)
		{
			if (arr_8bit[1] == 0)
			{
				arr_8bit[1] = arr_8bit[0];
				arr_8bit[0] = '0';
			}
			else
			{
				no_use = 3;
			}
		}
		if (temp_int_four < 10)
		{
			if (arr_4bit[1] == 0)
			{
				arr_4bit[1] = arr_4bit[0];
				arr_4bit[0] = '0';
			}
			else
			{
				no_use = 3;
			}

		}
		fn_arr[0] = arr_4bit[1];
		fn_arr[1] = arr_8bit[0];
		fn_arr[2] = arr_8bit[1];
		fn_arr[3] = '\0';
		twlv_bit_fat = (int) strtol(fn_arr, NULL, 16);
		// printf("[[[[[\nThe 12bit are: %d, 0x%s\n]]]]\n", twlv_bit_fat, fn_arr);
	}
	else
	{
		eitBits_offset = 1 + ((3*nth_entry)/2);
		entry_ind = eitBits_offset + 512;
		eitBits_hex = (unsigned)bMap_ptr[entry_ind] & 0xffU;
		sprintf(arr_8bit, "%x", eitBits_hex);
		temp_int_eit = (int) strtol(arr_8bit, NULL, 16);
		
		fourBits_offset = (3*nth_entry)/2;
		entry_ind = fourBits_offset + 512;
		fourBits_hex = (unsigned)bMap_ptr[entry_ind] & 0xffU;
		sprintf(arr_4bit, "%x", fourBits_hex);
		temp_int_four = (int) strtol(arr_4bit, NULL, 16);

		if (temp_int_eit < 10)
		{
			if (arr_8bit[1] == 0)
			{
				arr_8bit[1] = arr_8bit[0];
				arr_8bit[0] = '0';
			}
			else
			{
				no_use = 3;
			}
		}
		if (temp_int_four < 10)
		{
			if (arr_4bit[1] == 0)
			{
				arr_4bit[1] = arr_4bit[0];
				arr_4bit[0] = '0';
			}
			else
			{
				no_use = 3;
			}

		}
		fn_arr[0] = arr_8bit[0];
		fn_arr[1] = arr_8bit[1];
		fn_arr[2] = arr_4bit[0];
		fn_arr[3] = '\0';
		twlv_bit_fat = (int) strtol(fn_arr, NULL, 16);
		// printf("]]]]\nThe 12bit are: %d, 0x%s\n[[[[\n", twlv_bit_fat, fn_arr);
	}


	return twlv_bit_fat;
}
int main(int argc, char *argv[])
{
	int fd;
	struct stat sb;

	char *file_to_open;
	char *file_to_read;

	// file_to_open = "testNew.IMA";
	// file_to_read = "ACS.H";
	
	// file_to_open = "disk.ima";
	// file_to_read = "ICEBERGS.TEX";

	fd = open(argv[1], O_RDWR);
	// fd = open(file_to_open, O_RDWR);
	fstat(fd, &sb);
	// printf("Size: %lu\n\n", (uint64_t)sb.st_size);

	char * b_ptr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
	if (b_ptr == MAP_FAILED) {
		printf("Error: failed to map memory\n");
		exit(1);
	}
	// p++;
	char *fname;
	fname = strdup(argv[2]);
	// fname = strdup(file_to_read);
	printf("The disk is: %s,\nAnd file to find is: %s\n", argv[1], fname);
	// printf("a hex char of: %1x \n", (unsigned)p[9] & 0xffU);
	// printf("The values are: %c\n", 0x30);
	int num_dir_in_list = 0;
	int phsc_cluster = 0;
	int temp_lc = 0;
	int complex_int = 0;// a do nothing int
	int shw_path;
	unsigned int t_hex;
	int is_back;
	char root_name[13];

	t_hex = (unsigned)b_ptr[9888] & 0xffU;
	// printf("Trex is: 0x%x \n", t_hex);	
	root_name[0] = 'r';
	root_name[1] = 'o';
	root_name[2] = 'o';
	root_name[3] = 't';
	root_name[4] = '\0';
	int counting = 0;
	// printf("Root\n\n");
	// printf("==================\n");
	is_back = contains_input(b_ptr, fname);
	printf("Back from contains_input\n");
	
	// printf("-- TESTING: 0x%x--\n", testa);
	// if (testa == 235)
	// {
	// 	printf(">> testa is 235 in decimal <<\n");
	// }
	// return 0;
	
	// printf("a hex char of: %1x \n", c);
	
	//p[0] = 0x00; // an example to manipulate the memory, be careful when you try to uncomment this line because your image will be modified
	
	// printf("modified the memory\n");
	munmap(b_ptr, sb.st_size); // the modifed the memory data would be mapped to the disk image
	close(fd);
	return 0;
}