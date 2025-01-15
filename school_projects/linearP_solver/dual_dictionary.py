
def dl_to_prm_lbls(non_basic_li, basic_li, un_dl_li):
    num_rows = len(basic_li)

    un_dl_li.append(['a'])

    for i in range(num_rows + 1):
        if (i == 0):
            un_dl_li[0][0] = 'back_to_prm_config'
            un_dl_li[0].extend(non_basic_li)
        else:
            un_dl_li.append([basic_li[i - 1]])
            un_dl_li[i].extend(non_basic_li)

def mk_dl_lbl_li(dl_label_li, prm_label_li, prm_num_rows ,prm_num_opt_var):

    #NOTE: We know that this is a second time mk_dl_lbl_li is called because when it's 
    #called again we can change prm_label_li[0][0] form 'dl_opt_var' to 'dl_opt_var_again'
    dl_range = prm_num_rows + prm_num_opt_var
    # dl_range + 1 because in range(5) will do 0 to 4
    dl_non_basic_var_li = [x for x in range(dl_range + 1) if x > prm_num_opt_var]
    # dl_non_basic_li = [x for x in range(prm_num_opt_var + 1) if x > 0]

    # dl_label_li = []
    if (prm_label_li[0][0] != 'dl_opt_var_to_prm'):
        dl_label_li.append(['dl_opt_var'])
        for i in range(prm_num_opt_var + 1):
            if (i == 0):
                dl_label_li[0].extend(dl_non_basic_var_li)
            else:
                alist = [i]
                dl_label_li.append(alist)
                dl_label_li[i].extend(dl_non_basic_var_li)
    else:
        non_basic_li = []
        basic_li = []
        un_dl_li = []
        #once you find the list test python file see if you can turn this for loop into a function
        for i in range(prm_num_rows + 1): # create a non_basic and a basic list from dl_label_li
            if (i > 0):
                non_basic_li.append(prm_label_li[i][0])
        for t in range(prm_num_opt_var + 1):
            if (t > 0):
                basic_li.append(prm_label_li[0][t])
        dl_to_prm_lbls(non_basic_li, basic_li, dl_label_li)


def mk_dual_lbl_coef_li(prm_label_li, prm_coef_li, prm_num_rows, prm_num_opt_vars):
    
    dl_row_k = 0
    p_row_k = 0
    # prm_num_rows is index at 0. sl 38 lect 7 4 prm rows gives 4 dual_opt_vars
    dl_num_opt_vars = prm_num_rows
    dl_num_rows = prm_num_opt_vars
    dl_coef_li = []
    dl_tmp_li = ['a']
    
    dl_a_list = [x for x in range(dl_num_opt_vars + 1)] # + 1 to include label for optimal value
    # make dual label list of lists
    
    dl_label_li = []
    mk_dl_lbl_li(dl_label_li, prm_label_li, prm_num_rows, prm_num_opt_vars)
    
    #[[dl_opt_var,1,2,..,],[y5,y1,y2,...]]
    
    #make dual coef list of list
    dl_slack_li  = []
    tmp_dual_opt_coef_li = []
    tmp_dual_slk_coef_li = []
    i_count = 0
    for li_c in prm_coef_li:
        if (i_count == 0):
            tmp_dual_opt_coef_li.append(li_c[0]) # the initial value of the dual optimal variable.
            for t in range(len(li_c)):
                if (t > 0):
                    tmp_dual_slk_coef_li.append(li_c[t])
        else:
            tmp_dual_opt_coef_li.append(li_c[0])
        i_count += 1
    # print("tmp_slack_li: {}".format(tmp_slack_li))
    nw_dl_opt_row_coef_li = list(tmp_dual_opt_coef_li)
    nw_dl_slk_coef_li = list(tmp_dual_slk_coef_li)
    # print("nw_dl_opt_row_li: {}\nnw_dl_slk_coef_li: {}".format(nw_dl_opt_row_coef_li, nw_dl_slk_coef_li))
    for t in range(len(nw_dl_opt_row_coef_li)):
        nw_dl_opt_row_coef_li[t] = -1*nw_dl_opt_row_coef_li[t] #negate all coefficients for duality
    
    nw_dl_row_coef_li = []
    dl_ith_row = 0
    # currently the coef of basic_vars in the dual's optimal value row
    all_slk_ngtv = True
    dl_slk_coef_len = len(nw_dl_slk_coef_li)
   
    tmp_slk_li = list(nw_dl_slk_coef_li)
    nw_dl_slk_coef_li = [-1*x for x in tmp_slk_li]
    # end of trying to avoid wasting time i spent writting mk_dual_lbl_coef_li function. This function
    for r in range(dl_num_rows + 1):
        nw_row_col_coef_li = []
        if (r == 0):
            dl_coef_li.append(list(nw_dl_opt_row_coef_li))
        else:
            # print(r)
            dl_slack_val = nw_dl_slk_coef_li[r-1]
            # dl_ith_row += 1
            nw_row_col_coef_li.append(dl_slack_val) 
            for cl in range(dl_num_opt_vars + 1):
                if (cl > 0): # column 0 is the optimal value and slack value column
                    a_ngt_val = -1*prm_coef_li[cl][r]
                    nw_row_col_coef_li.append(a_ngt_val)
            dl_coef_li.append(list(nw_row_col_coef_li)) 
    s_row = 0
    # assert False, "dual labels"
    return (dl_label_li, dl_coef_li, dl_num_rows, dl_num_opt_vars)