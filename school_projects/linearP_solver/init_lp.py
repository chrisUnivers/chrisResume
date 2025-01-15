

def mk_pr_label_li(prm_num_rows, num_opt_vars):
    prm_labels_li = []
    to_coef_li = []
    row_k = 0
    for ind_i in range(prm_num_rows + 1): #TO prm_coef_lst
        if (row_k == 0): # contains label for Zeta
            tmp_li = []
            # tmp_li.extend(li_i) #TO  
            for i in range(num_opt_vars + 1): # make labes x1 to xn, range(4) = 0 1 2 3
                if (i == 0):
                    tmp_li.append('opt_val') # 
                else:
                    tmp_li.append(i) # i = 1 labels x1
            prm_labels_li.append(tmp_li)
            row_k += 1
        else:
            # making one list per row from row 1 to row prm_num_ rows i.e(w1=x4, x1,x2,x3)
            tmp_li = []
            for t in range(num_opt_vars + 1):
                if (t == 0):
                    # for labels, I care about the number and not the 'x' part.
                    tmp_li.append(row_k + num_opt_vars) # if num_opt_var = 3, this is labe x4
                else:
                    tmp_li.append(t) # if row_k == 1 at the end tmp_li should be [x4, x1, x2, x3]
            prm_labels_li.append(tmp_li)
            if (row_k < (prm_num_rows)):
                row_k += 1

    return prm_labels_li

def init_input():
    # print("Enter the Lp. When done enter the letter <d> for done and press enter")
    user_str = "Good"
    prm_coeff_li = []
    sing_li = []
    ind_rows = 0
    num_opt_var = 0

    while (user_str != "d"):
        # print("Enter new row")
        try:
            lp_input = input()
            user_str = lp_input.split(' ')[0]
        except EOFError:
            user_str = "d"
        
        # print("1st entry: {}".format(user_str))
        sing_li.append(lp_input) # input as entered
        if (user_str != "d"):
            if (ind_rows == 0): # row k = 0 is Zeta's row
                tmp_li = [float(x) for x in lp_input.split()]
                num_opt_var = len(tmp_li)
                tmp_li.insert(0,0)
                
                prm_coeff_li.append(tmp_li) # input split by spaces
                ind_rows += 1
            else:
                # NOTE: The value of the slack_var is at the end of tmp_li so at tmp_li[len(tmp_li) - 1]
                tmp_li = [float(x) for x in lp_input.split()] 
                li_two = [-1*y for y in tmp_li[0:num_opt_var]] #li_two should not contain value of slack_var
                slack_val = tmp_li.pop(num_opt_var)
                li_two.insert(0, slack_val)
                
                prm_coeff_li.append(li_two) # input split by spaces
                
                ind_rows += 1
    prm_dict_num_rows = ind_rows - 1       
    # print("The sing list is: {}\n".format(sing_li))
    
    # assert False, "list before any functions"
    prm_label_li = mk_pr_label_li(prm_dict_num_rows, num_opt_var)
    
    return [prm_label_li, prm_coeff_li, prm_dict_num_rows, num_opt_var]