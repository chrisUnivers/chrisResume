#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <byteswap.h>

// Please remember to give write permission to your argv[1] (the image you want map) by using chmod (if it doest not have the write permission by default), otherwise you will fail the map.
int num_files;

int val_in_fat_entry(int nth_entry, char *f_ptr_b, int which_fnct, int atest);
int count_free_size(char *free_sz_ptr);
unsigned int get_four_bit(int start_ofst, int stop_ofst, char *eit_ptr);
int one_hex(unsigned int one_hex_val, int which_job);
int two_hex(unsigned int fst_hex, unsigned int snd_hex, int aJob);
int num_of_files(int an_ofst, char *cool_f_ptr);
char dir_files(int cur_num_file, int dir_off_set, char *me_file_ptr);
int num_files_dir(int addr_sect, int now_num_files, int n_of_sectors, char *ab_file_pt);
u_int32_t make_lt_endian(u_int32_t arr_b_end[], int len_arr);



int num_of_files(int an_ofst, char *cool_f_ptr)
{
	/*
		an_ofst: will most likely be 0x0002600 = 9728 for root directory or the physical sector of a directory
		cool_f_ptr: pointer to the mapping function
	*/
	int r_dir_start;
	int dir_entry_ofst;
	unsigned int ahex_val;
	unsigned int hex_a;
	unsigned int hex_b;
	//logical cluster value
	// int lc_val;
	int is_dir_file;
	// int dir_ind = 26;
	char achar;
	achar = 'c';
	int file_off_set;
	int entry_off_set;
	int hex_index;
	file_off_set = 11;
	entry_off_set = 9728; // ROOT directory offset

	for (int i = 0; i < 14; i++)// 14 sectors in the root directory
	{
		for (int j = 0; j < 16; j++)// 16 directory entries of 32 bytes in each sector of a directory
		{
			ahex_val = (unsigned)cool_f_ptr[entry_off_set] & 0xffU;
			if (ahex_val == 0) 
			{
				// This directory *ENTRY is free and all other remaining directory entries in this directory are free
				break;
			}
			// check if file or directory at the 11th byte of each entry
			hex_index = entry_off_set + 11; // file and or dictorectory at the 11th byte of an entry
			ahex_val = (unsigned)cool_f_ptr[hex_index] & 0xffU;
			//get: 1 if it's a file, 0 for directory, -1 for otherwise
			// is_dir_file = one_hex(ahex_val, 2);
			if (ahex_val == 0) 
			{
				if (ahex_val != 0x0F) // check attributes byte is not 0x0F
				{
					ahex_val = (unsigned)cool_f_ptr[entry_off_set] & 0xffU;
					if (ahex_val != 0xE5) // check filename byte is not 0xE5;
					{
						hex_index = entry_off_set + 26;
						ahex_val = (unsigned)cool_f_ptr[hex_index] & 0xffU;
						if (ahex_val != 0)// 1st logical cluster is not 0 or 1
						{
							if (ahex_val != 1)
							{
								num_files += 1;
							}
						}
					}
				}
			}
			else if (ahex_val == 16)
			{				
				achar = dir_files(num_files, entry_off_set, cool_f_ptr);
			}
			else
			{
				// printf("-- ** The entry is something else -- **\n");
			}
			entry_off_set += 32;
		}
		entry_off_set = 9728 + ((i + 1) * 512);
	}
	printf("The number of files is: %d\n", num_files);
	return 0;
}

char dir_files(int cur_num_file, int dir_off_set, char *me_file_ptr)
{
	/*
		dir_off_set: the offset of the entry whose 11th byte(so attribute) is 16 == a subdirectory
	*/
	int new_f_count;
	int new_offset;
	int dir_fst_lc;
	int phy_sect;
	int rt_dir_fl;
	char to_rt;
	int n_sectors_in_dir;
	unsigned int un_used;
	int iam_even;
	u_int32_t arr_fst_lc[2];
	new_f_count = cur_num_file;
	new_offset = dir_off_set + 26;
	to_rt = 'A';
	arr_fst_lc[0] = (unsigned)me_file_ptr[new_offset] & 0xffU;
	
	new_offset = dir_off_set + 27;

	arr_fst_lc[1] = (unsigned)me_file_ptr[new_offset] & 0xffU;
	dir_fst_lc = make_lt_endian(arr_fst_lc, 2);
	phy_sect = 33 + dir_fst_lc - 2;
	un_used = me_file_ptr[9745] & 0xffU;
	rt_dir_fl = num_files_dir(phy_sect, new_f_count, n_sectors_in_dir, me_file_ptr);

	num_files = rt_dir_fl;
	return to_rt;
}
u_int32_t make_lt_endian(u_int32_t arr_b_end[], int len_arr)
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
	}
	if (num_bits == 16)
	{
		output_sixt_hex = __bswap_16 (fin_val);
		rt_int = output_sixt_hex;
	}
	else if (num_bits == 32)
	{
		output_hex = __bswap_32 (fin_val);
		rt_int = output_hex;
	}
	else 
	{
		printf("ERROR IN MAKE_LT_ENDIAN: UNWANTED NUMBER OF BITS");
	}
	return rt_int;
}
int num_files_dir(int addr_sect, int now_num_files, int n_of_sectors,char *ab_file_pt)
{
	/*
		addr_sect: the physical sector i.e: 33 + 101 - 2
	*/
	int offset_in_dir;
	int nothing_do;
	int rt_num_files;
	int rt_more_files;
	int search_done;
	char rt_char;
	int dir_index; // replace hex_index
	unsigned int ba_hex_val; // replace ahex_val
	unsigned int aname;
	unsigned int temp;
	unsigned int cur_dir;
	unsigned int par_dir;
	int temp_index;
	int parent_ind;
	ba_hex_val = 0x00;
	dir_index = 0;
	search_done = 1;
	rt_num_files = now_num_files;
	rt_more_files = now_num_files;
	offset_in_dir = addr_sect * 512;
	for (int k = 0; search_done > 0; k++)
	{
		for (int n = 0; n < 16; n++)
		{
			ba_hex_val = (unsigned)ab_file_pt[offset_in_dir] & 0xffU;
			cur_dir = (unsigned)ab_file_pt[offset_in_dir] & 0xffU;
			parent_ind = offset_in_dir + 32;
			par_dir = (unsigned)ab_file_pt[parent_ind] & 0xffU;
			if (ba_hex_val == 0)
			{
				search_done = 0;
				break;
			}
			dir_index = offset_in_dir + 11;
			ba_hex_val = (unsigned)ab_file_pt[dir_index] & 0xffU;
			if (ba_hex_val == 0x00)
			{
				ba_hex_val = (unsigned)ab_file_pt[offset_in_dir] & 0xffU;
				if (ba_hex_val != 0xE5)
				{
					dir_index = offset_in_dir + 26;
					ba_hex_val = (unsigned)ab_file_pt[dir_index] & 0xffU;
					if (ba_hex_val != 0x00)
					{
						if (ba_hex_val != 1)
						{
							num_files += 1;
							aname = (unsigned)ab_file_pt[offset_in_dir] & 0xffU;
						}
						else
						{
							nothing_do = 4;
						}
					}
					else
					{
						nothing_do = 3;
					}
				}
				else
				{
					nothing_do = 0;
				}
			}
			else
			{
				if (cur_dir != 0x2e)// do not consider current directory
				{
					if (ba_hex_val == 0x10)// its a subfolder
					{
						rt_char = dir_files(num_files, offset_in_dir, ab_file_pt);
					}
				}
				else
				{
					nothing_do = 5;
				}
			}
			offset_in_dir += 32; // each entry in a directory is 32 bytes
		}
		offset_in_dir = addr_sect + ((k + 1) * 512);// next sector;
	}
	return num_files;
}


int two_hex(unsigned int fst_hex, unsigned int snd_hex, int aJob)
{
	/*
		which_job values: 
			2: want to get the value of the logical cluster
	*/
	char arr_fst_byte[3];
	char arr_snd_byte[3];
	char arr_lc_byte[5]; // logical cluster: i.e 1st logical cluster or some other cluster
	unsigned int temp_a_hex;
	unsigned int temp_b_hex;
	int temp_int_a;
	int temp_int_b;
	int lc_location;
	temp_a_hex = fst_hex;
	temp_b_hex = snd_hex;

	if (aJob == 2)
	{
		sprintf(arr_fst_byte, "%x", fst_hex);
		sprintf(arr_snd_byte, "%x", snd_hex);
	}
	
	temp_int_a = (int) strtol(arr_fst_byte, NULL, 16);
	temp_int_b = (int) strtol(arr_snd_byte, NULL, 16);
	if (temp_int_a < 10)
	{
		printf("temp_int_a is < 10: false\n");
		if (arr_fst_byte[1] == 0)
		{
			arr_fst_byte[1] = arr_fst_byte[0];
			arr_fst_byte[0] = '0';
		}
	}
	if (temp_int_b < 10)
	{
		printf("temp_int_b is < 10: true\n");
		if (arr_snd_byte[1] == 0)
		{
			arr_snd_byte[1] = arr_snd_byte[0];
			arr_snd_byte[0] = '0';
		}
	}
	if (aJob == 2)
	{
		arr_lc_byte[0] = arr_snd_byte[0];
		arr_lc_byte[1] = arr_snd_byte[1];
		arr_lc_byte[2] = arr_fst_byte[0];
		arr_lc_byte[3] = arr_fst_byte[1];
		arr_lc_byte[4] = '\0';
		lc_location = (int) strtol(arr_lc_byte, NULL, 16);
		printf("** -- The first Logical cluster is: %d -- **\n", lc_location);
	}
	else
	{
		printf("ERROR IN TWO_HEX: ONLY CURRENTLY ACCEPTING JOB 2");
	}
	return lc_location;

}

int val_in_fat_entry(int nth_entry, char *f_ptr_b, int which_fnct, int atest)
{

	/*
		which_fnct equals:
			1: when the calling function is count_free_size
			2: when the calling function is most likely nxt_physical_entry()
		nth_entry: the nth_entry in the fat table. In the example it's 101

	*/
	int entry_base_ten;
	int is_even;
	int eitBit_offset;
	int entry_index;
	int fourBit_offset;
	char arr_8bits[3];
	char arr_4bits[3];
	int check_8bits;
	int check_4bits;
	unsigned int eitBit_hex;
	unsigned int fourBit_hex;
	unsigned int fourBit_hex_a;
	unsigned int fourBit_hex_b;
	char final_arr[4];

	is_even = nth_entry % 2;
	
	if (is_even == 0)
	{
		// get high eight bits
		eitBit_offset = (3*nth_entry)/2; // at location 152 see fatsearch
		entry_index = eitBit_offset + 512; // 1st sector of FAT table starts at 0x200 = 512
		eitBit_hex = (unsigned)f_ptr_b[entry_index] & 0xffU;
		sprintf(arr_8bits, "%x", eitBit_hex);
		check_8bits = (int) strtol(arr_8bits, NULL, 16);
		
		//get low four bits
		fourBit_offset = 1 + (3*nth_entry)/2; // at 151
		entry_index = fourBit_offset + 512; // 1st sector of FAT table starts at 0x200 = 512
		fourBit_hex = (unsigned)f_ptr_b[entry_index] & 0xffU;
		sprintf(arr_4bits, "%x", fourBit_hex);
		check_4bits = (int) strtol(arr_4bits, NULL, 16);
		
		//3)
		if (check_8bits < 10)
		{
			arr_8bits[1] = arr_8bits[0];
			arr_8bits[0] = '0';
			if (check_4bits < 10)
			{
				arr_4bits[1] = arr_4bits[0];
				arr_4bits[0] = '0';
			}
			
			final_arr[0] = arr_8bits[0];
			final_arr[1] = arr_8bits[1];
			final_arr[2] = arr_4bits[1];
			final_arr[3] = '\0';
			entry_base_ten = (int) strtol(final_arr, NULL, 16);
		}
		
		else
		{
			if (check_4bits < 10)
			{
				arr_4bits[1] = arr_4bits[0];
				arr_4bits[0] = '0';
			} 
			final_arr[0] = arr_8bits[0];
			final_arr[1] = arr_8bits[1];
			final_arr[2] = arr_4bits[1];
			final_arr[3] = '\0';
			entry_base_ten = (int) strtol(final_arr, NULL, 16);
		}
		
	}
	else
	{
		// get high eight bits
		eitBit_offset = 1 + ((3*nth_entry)/2); // at location 152 see fatsearch
		entry_index = eitBit_offset + 512; // 1st sector of FAT table starts at 0x200 = 512
		eitBit_hex = (unsigned)f_ptr_b[entry_index] & 0xffU;
		sprintf(arr_8bits, "%x", eitBit_hex);
		check_8bits = (int) strtol(arr_8bits, NULL, 16);
		
		//get low four bits
		fourBit_offset = (3*nth_entry)/2; // at 151
		entry_index = fourBit_offset + 512; // 1st sector of FAT table starts at 0x200 = 512
		fourBit_hex = (unsigned)f_ptr_b[entry_index] & 0xffU;
		sprintf(arr_4bits, "%x", fourBit_hex);
		check_4bits = (int) strtol(arr_4bits, NULL, 16);
		
		if (check_8bits < 10)
		{
			arr_8bits[1] = arr_8bits[0];
			arr_8bits[0] = '0';
			if (check_4bits < 10)
			{
				arr_4bits[1] = arr_4bits[0];
				arr_4bits[0] = '0';
			}
			final_arr[0] = arr_8bits[0];
			final_arr[1] = arr_8bits[1];
			final_arr[2] = arr_4bits[0]; //should now be 066, from example inf fat12search
			entry_base_ten = (int) strtol(final_arr, NULL, 16);
		}
		else
		{
			if (check_4bits < 10)
			{
				arr_4bits[1] = arr_4bits[0];
				arr_4bits[0] = '0';
			}
			final_arr[0] = arr_8bits[0];
			final_arr[1] = arr_8bits[1];
			final_arr[2] = arr_4bits[0]; //should now be 066
			entry_base_ten = (int) strtol(final_arr, NULL, 16);
		}

	}

	
	if (which_fnct == 1)
	{
		// the calling function wants to count free size of the disk. So checking if the entry is 0x00
		if (entry_base_ten == 0)
		{
			// if (atest == 1)
			// {
			// 	printf("\nThe nth entry is: %d\n", nth_entry);
			// }
			return 1;
		}
		else
		{
			return 0;
		} 
	}
	else if (which_fnct == 2)
	{
		/*
			calling function wants to get the next physical address
			most likely val_nth_fat_entry()
		*/
		return entry_base_ten; // in FatSearch this value corresponds to 102 in step 6
	}
	else
	{
		printf("\nERROR: No other function should be calling get_nxt_fat_entry()\n");
		exit(0);
	}

	return 0;
}
int count_free_size(char *free_sz_ptr)
{
	char *rt_str;
	char *ptr_temp;
	unsigned int ahex_val;
	unsigned int comp_hex = 0x00;
	int empty_sector = 0;
	int free_size = 0;
	int num_entries_in_fat;
	int rt_value;
	num_entries_in_fat = 3072; // (9*512*8)/12
	int s = 1;

	// ptr_temp = f_ptr;
	ahex_val = free_sz_ptr[6];
	// printf("eigh bits the value is: %1x\n", ahex_val);
	for (int i = 0; i < num_entries_in_fat; i++)
	{
		if (i > 1)
		{
			rt_value = val_in_fat_entry(i, free_sz_ptr, 1, s);
			if (rt_value == 1)
			{
				s = 0;
				empty_sector++;
			}
		}
	}
	free_size = (empty_sector - 223)*512;
	printf("Free size of the disk: %d\n\n", free_size);
	
	return 0;
	
}

int main(int argc, char *argv[])
{
	int fd;
	struct stat sb;

	fd = open(argv[1], O_RDWR);
	fstat(fd, &sb);
	// printf("Size: %lu\n\n", (uint64_t)sb.st_size);

	char * p = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
	char * p_temp = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		printf("Error: failed to map memory\n");
		exit(1);
	}
	// p++;
	int k = 0;
	int m = 0;
	char *fname;
	fname = strdup(argv[1]);
	// printf("a hex char of: %1x \n", (unsigned)p[9] & 0xffU);
	// printf("The values are: %c\n", 0x30);
	unsigned int anum;
	int onspace = 0;
	int os_count = 1;
	int is_true = 1;
	unsigned int testa;
	testa = (unsigned)p[9888] & 0xffU;
	// printf("-- TESTING: 0x%x--\n", testa);
	// if (testa == 235)
	// {
	// 	printf(">> testa is 235 in decimal <<\n");
	// }
	// return 0;
	printf("OS Name: ");
	while (is_true)
	{
		
		if (p[k] != 20)
		{
			anum = p[k];
			if (os_count > 3 && (os_count < 13))
			{
				printf("%c", anum);
			}
		}
		if (os_count > 14)
		{
			is_true = 0;
			unsigned int temp;
			p_temp = p + 9824;
			printf("\nLabel of the disk: ");
			for (int i = 0; i < 6; i++)
			{
				temp = p_temp[i];
				printf("%c", temp);
			}
			printf("\n");
			// printf("at 0x2600 the char is: %c\n", temp);
		}
		k++;
		m++;
		os_count++;
		onspace++;
	}
	printf("Total size of the disk: %lu\n", (uint64_t)sb.st_size);
	// printf("Now calling check eight\n");
	int s_rt;
	// s_rt = check_eight_bits(512, 136, p);
	// s_rt = val_in_fat_entry(101, p, 1);
	s_rt = count_free_size(p);
	unsigned int temp_ahex;
	unsigned int temp_bhex;
	temp_ahex = (unsigned)p[9771] & 0xffU;
	temp_bhex = (unsigned)p[9915] & 0xffU;
	// s_rt = two_hex(temp_ahex, temp_bhex, 2);
	printf("==============\n");
	s_rt = num_of_files(0, p);
	printf("Back from num_of_files\n");
	printf("\n==============\n");
	printf("Number of FAT copies: 2\n");
	printf("Sectors per FAT: 9\n");
	unsigned int c;
	c = (unsigned)p[663] & 0xffU;
	// printf("a hex char of: %1x \n", c);
	
	//p[0] = 0x00; // an example to manipulate the memory, be careful when you try to uncomment this line because your image will be modified
	
	// printf("modified the memory\n");
	munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
	close(fd);
	return 0;
}
