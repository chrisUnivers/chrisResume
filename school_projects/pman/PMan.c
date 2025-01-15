#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_p
{
	char *first_cmd;
	char *second_cmd;
	pid_t pro_id;
	struct node_p *next;
};

char *get_user_input()
{
	struct node_p *u_input;
	struct node_p anode;
	u_input = &anode;
	char *temp_u_input;
	char *substr_u_input;
	char arr_of_u_input[1000];
	int ind_arr = 0;
	size_t linecp = 0;
	size_t input_sz = 32;
	size_t input_len;


	
	int compared_val = 1;
	int len_input = 1; // length of user input without the new character at the end!

	temp_u_input = (char *)malloc(input_sz * sizeof(char));
	if (temp_u_input == NULL)
	{
		printf("**WARNING**\nSomething went wrong when allocating memory for input!\n");
	}

	printf("PMan:> ");
	input_len = getline(&temp_u_input, &input_sz, stdin);
	//initialize array of user input with user input
	
	return temp_u_input;
}
void smallfunc(struct node_p some_words)
{
	printf("\ninput one is: %s\n", some_words.first_cmd);
	printf("input two is: %s\n", some_words.second_cmd);
}
int main(){

	struct node_p *u_input;
	struct node_p anode;
	u_input = &anode;

	char *substr_u_input;
	char arr_of_u_input[1000];
	int ind_arr = 0;
	char *a_user_input;
	a_user_input = get_user_input();

//--------------------------------------------------------------------------------------------
/*
	* code btw the "-" used to be a function, however when that fuction
	* returned the struct node_p or a point to this struct
	* the string attributes of that returned struct would only be 3 characters long
	* no matter the user input size(length). 
	* putting this code in the main fuction seems to fix that issue(don't know why)
	* and when I pass the user input struct node_p as an argument to another fuction(smallfunc)
	* the string attribute content is not reduced to 3 characters. That's why this code
	* is no longer a function. Sorry for the long explanation!
*/
	while (*a_user_input != '\0')
	{
		if (*a_user_input == '\n')
		{
			a_user_input++;
		}else
		{
			arr_of_u_input[ind_arr] = *a_user_input++;
			ind_arr += 1;
		}
	}
	arr_of_u_input[ind_arr] = '\0';
	printf("** arr is: %s **\n", arr_of_u_input);
	substr_u_input = strtok(arr_of_u_input, " ");
	// put the 1st and 2nd(if any) user inputs in the a node u_input
	// printf("-- a --\n");
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			u_input->first_cmd = substr_u_input;
			printf("-- %lu --\n", strlen(u_input->first_cmd));
			if (u_input->first_cmd == NULL || (strlen(u_input->first_cmd) <= 1))
			{
				u_input->first_cmd = "Null";
				printf("u_input fst cmd is: %s\n", u_input->first_cmd);
			}
			// else
			// {
			// 	printf("u_input first cmd is: %s\n", u_input->first_cmd);
			// }
			substr_u_input = strtok(NULL, " ");
		}else
		{
			// printf("-- f --\n");
			u_input->second_cmd = substr_u_input;
			if (u_input->second_cmd == NULL)
			{
				u_input->second_cmd = "Null";
				printf("u_input snd cmd is: %s\n", u_input->second_cmd);
			}
			// else
			// {
			// 	printf("u_input second cmd is: %s\n", u_input->second_cmd);
			// }
			substr_u_input = strtok(NULL, " ");
		}
	}
	// printf("u_input first cmd is: %s\n", u_input->first_cmd);
	// printf("u_input second cmd is: %s\n", u_input->second_cmd);
	smallfunc(anode);
//----------------------------------------------------------------------------------------

	
	//get input
	/*
	while(1){	
		cmd = readline("PMan: > ");
		
		//  parse the input cmd (e.g., with strtok)
		
		
		if (cmd_type == CMD_BG){
			bg_entry(argv);
		}
		else if(cmd_type == CMD_BGLIST){
			bglist_entry();
		}
		else if(cmd_type == CMD_BGKILL || cmd_type == CMD_BGSTOP || cmd_type == CMD_BGCONT){
			bgsig_entry(pid, cmd_type);
		}
		else if(cmd_type == CMD_PSTAT){
			pstat_entry(pid);
		}
		else {
			usage_pman();
		}
		check_zombieProcess();
	}
	*/

	return 0;
}

void bg_entry(char **argv){
	
	pid_t pid;
	pid = fork();
	if(pid == 0){
		if(execvp(argv[0], argv) < 0){
			perror("Error on execvp");
		}
		exit(EXIT_SUCCESS);
	}
	else if(pid > 0) {
		// store information of the background child process in your data structures
	}
	else {
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
}

void check_zombieProcess(void){
	int status;
	int retVal = 0;
	
	while(1) {
		usleep(1000);
		// if(headPnode == NULL){
		// 	return ;
		// }
		retVal = waitpid(-1, &status, WNOHANG);
		if(retVal > 0) {
			//remove the background process from your data structure
		}
		else if(retVal == 0){
			break;
		}
		else{
			perror("waitpid failed");
			exit(EXIT_FAILURE);
		}
	}
	return ;
}