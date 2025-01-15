import argparse
import sys


def get_opt_values(label_list, coef_list, num_optm_vars, num_rows):
    # pvt_label_li, pvt_coef_li, num_opt_vars, num_rows
    # [x1,x2,x3,...,xn]

    # print("{}\n{}\n{}\n{}".format(label_list, coef_list, num_optm_vars, num_rows))
    # assert False, "current lists"
    num_opt_var_p_one = num_optm_vars + 1
    len_num_rows = num_rows + 1
    opt_values_li = ['a' for x in range(num_optm_vars)]
    opt_vals_found = 0
    # print(label_list)
    for i in range(num_opt_var_p_one): # set basic optimization variables to zero in opt_values_li
        # print("i: {}".format(i))
        if (i > 0):
            if (i in label_list[0]):
                # print("opt_values_li: {} len: {} i: {}".format(opt_values_li, len(opt_values_li), i))
                opt_values_li[i - 1] = 0
                opt_vals_found += 1
                # print("found x{}".format(i))
    
    # print("optimal value row: {}".format(label_list[0]))
    # print(opt_values_li)
    # print("num_opt_vars + 1: {}".format(num_opt_var_p_one))
    row_k = 0
    for li_k in label_list: # checking row k, k != optimal value row
        if (row_k > 0):
            for t in range(num_opt_var_p_one):
                # print("t: {}".format(t))
                # t = x1,2,3,...,n-1,xn
                if (t == li_k[0]): # li_k is a list in label
                    opt_values_li[t-1] = coef_list[row_k][0]
                    
                    # print(li_k)
                    # print("found x{}".format(t))
                    # print(opt_values_li)
                    t = num_opt_var_p_one # end this loop and check next row's labels.
                    opt_vals_found += 1
            row_k += 1
        else:
            row_k += 1
    if (opt_vals_found == num_optm_vars):
        return opt_values_li
    
    # print(opt_values_li)
    # print("opt_vals_found: {} num_opt_var: {}".format(opt_vals_found, num_optm_vars))
    assert False, "opt_valuse_found != num_optm_vars. Line 51, this should not happen. if everything works fine"
    return 0
def is_feasible(coef_li):
    bool_is_feasible = True
    r_count = 0
    for li_a in coef_li:
        if (r_count > 0):
            slk_val = li_a[0]
            if (slk_val < 0):
                bool_is_feasible = False
        r_count += 1
    return bool_is_feasible

def slv_orgnl_obj_fn(labels_li, coefs_li, num_opt_vars, num_rows, non_mod_opt_coef_li):

    slv_obj_fn_coef_li = list(coefs_li[0]) #lect 11 slide 100 [] = [0,0,0]
    ith_row = 0
    done_var_li = []
    orgn_lbls_li = []
    for t in range(num_opt_vars + 1):
        if (t > 0):
            orgn_lbls_li.append(t)
    # print(orgn_lbls_li)
    for li_coef in coefs_li:
        tmp_non_mod_coef = 0
        nw_li = []
        do_break = False
        # tmp_li = list(li_lbl)
        if (ith_row> 0): # skip optimal value's row
            for i in range(num_opt_vars + 1):
                if (i > 0): 
                    # check if x_i is in list li_lbl, i = 1,...,n=number of optimization variables
                    if ((i == labels_li[ith_row][0]) and (i not in done_var_li)):
                        # i will not equal 0, so xi will not be x0=variable of optimal value
                        done_var_li.append(i)
                        tmp_non_mod_coef = non_mod_opt_coef_li[i]
                        tmp_li = [tmp_non_mod_coef*x for x in li_coef]
                        nw_li = list(tmp_li)
                        # print("value of i is: {}".format(i))
                        # print("Anw_li: {}\n".format(nw_li))
                        for k in range(len(slv_obj_fn_coef_li)):
                            slv_obj_fn_coef_li[k] = slv_obj_fn_coef_li[k] + nw_li[k]
                            # print("li: {}\nnw_li: {}\nnon_mod_coef: {}\n".format(slv_obj_fn_coef_li, 
                            #                                                     nw_li, tmp_non_mod_coef))
                        do_break = True
                if (do_break):
                    break 
                 
        ith_row += 1
    nw_tmp_li = [x*0 for x in range(num_opt_vars + 1)]
    # print("solved original objective function using basis: {}".format(slv_obj_fn_coef_li))
    # print("A:\n")
    for t in range(num_opt_vars + 1):
        if ((t not in done_var_li) and (t > 0)):
            for k in range(num_opt_vars + 1):
                if (k == t):
                    # print(k)
                    nw_tmp_li[k] = non_mod_opt_coef_li[k]
                else:
                    # print("k is: {}".format(k))
                    nw_tmp_li[k] = 0
            for n in range(len(slv_obj_fn_coef_li)):
                slv_obj_fn_coef_li[n] = slv_obj_fn_coef_li[n] + nw_tmp_li[n]
    # print("solved original objective function using basis: {}".format(slv_obj_fn_coef_li))
    # assert False, "slv_obj_fn_coef_li"
    return slv_obj_fn_coef_li

def modified_dct(coef_list, num_opt_var):

    optm_row_coefs_len = num_opt_var + 1
    opt_row_coef_li = [0*x for x in range(optm_row_coefs_len)]
    # coef_list[0] = opt_row_coef_li
    
    # print(opt_row_coef_li)
    # assert False, "modified optimal row list"
    return opt_row_coef_li

def is_unbounded(label_li, coef_li, num_opt_vars, num_rows):
    a_pos_coef = False
    is_unbounded_= True
    col_unbounded = 0
    # print("Check unboundedness\nrow 0 coef: {}".format(coef_li[0]))
    for i in range(num_opt_vars + 1):
        if (i > 0):
            if (coef_li[0][i] > 0):
                a_pos_coef = True
                col_unbounded = i
    ith_row = 0
    if (a_pos_coef):
        for row_i in coef_li:
            if (ith_row > 0):
                unbounded_col_coef = row_i[col_unbounded]
                if ( unbounded_col_coef < 0): # i.e valid leaving variable
                    return False
            ith_row += 1
    else:
        return False # i.e no valid entering variable.
    
    return True

def upd_pvt_lbl_coef_li(pvt_info_tpl, pvt_label_li, pvt_coef_li, num_opt_vars, lv_var_row, num_rows):
    # pvt_info_tpl = 
        # pvt_info_tpl = (lowest_ent_nmd_lbl, ent_var_col, lowest_nmed_lv_var_lbl)
    ent_var_col = pvt_info_tpl[1]

    # a) increment obj_fn_val
    lv_var_const = pvt_coef_li[lv_var_row][0]
    lv_var_mult = pvt_coef_li[lv_var_row][ent_var_col]
    ent_var_coef  = pvt_coef_li[0][ent_var_col]
    
    # a) should be good

    # b) make ent_var the new slack_var and update obj_val row's label
    tmp_label = pvt_label_li[lv_var_row][0] # current slack variable label soon to be basic_var.
    pvt_label_li[lv_var_row][0] = pvt_info_tpl[0] # set new slack variable label
    pvt_label_li[lv_var_row][ent_var_col] = tmp_label # update basic_var in lv_var's row
    pvt_label_li[0][ent_var_col] = tmp_label # update obj_val row's label with old slack var now a basic var
    
    # print("\n{} Part b) {}\nand coef:    {}\n".format(pvt_info_tpl[2], pvt_label_li[0], pvt_coef_li[0]))
    
    # assert False, "Part b"
    # b) should be done

    # c) update coefficients in row lv_var_row.
    len_each_sub_list = num_opt_vars + 1
    lv_var_row_coef_li = pvt_coef_li[lv_var_row]
    lv_var_coef = -1 * lv_var_mult
    for i in range(len_each_sub_list):
        tmp_val = lv_var_row_coef_li[i]
        lv_var_row_coef_li[i] = tmp_val/lv_var_coef
    lv_var_row_coef_li[ent_var_col] = -1/lv_var_coef
    pvt_coef_li[lv_var_row] = lv_var_row_coef_li
    
    # print("\nPart c) {}\n".format(pvt_coef_li[lv_var_row]))
    
    # assert False, "Part c"
    # c) should be done
    
    # d) for each li in label_li, set label in ent_var_col to lv_var_label
    slak_to_bsc_lbl = pvt_label_li[0][ent_var_col]
    for k in range(num_rows + 1):
        if (k != 0):
            pvt_label_li[k][ent_var_col] = slak_to_bsc_lbl
    # print("\nPart d Ent_var_col: {}".format(ent_var_col))
    ro_k = 0
    for li_k in pvt_label_li:
        # print("row {}: {}".format(ro_k, li_k))
        ro_k += 1
    # assert False, "Part d"
    # d) should be done

    # e) for all non-lv_var_li in coef_li, update those coefficients 
        # i.e substitu ent_var in each non-lv_var_row
    
    # print("\n lv_var_coef_li: {}\n".format(lv_va_row_coef_li))
    # print("Part e):")
    for t in range(num_rows + 1):
        if (t != lv_var_row): # t != 0 and t != lv_var_row
            row_t_mult = pvt_coef_li[t][ent_var_col]
            # row_t_upd_li = [-2, 2, 0, 0] sl 45 lec 7 row of w1
            row_t_upd_li = [row_t_mult*x for x in lv_var_row_coef_li]
            
            # print("\nupt_row_t_li: {}\n".format(row_t_upd_li))
            
            row_t_coef_li = pvt_coef_li[t] # [8, -2, -4, 0] sl 45
            for s in range(len_each_sub_list):
                cur_coef = row_t_coef_li[s]
                if (s != ent_var_col):
                    row_t_coef_li[s] = cur_coef + row_t_upd_li[s]
                else:
                    row_t_coef_li[s] = row_t_upd_li[ent_var_col]
            
            # print("\nnon lv_var_row_coef_upd_li is:\n{}\n{} row: {}".format(pvt_label_li[t], row_t_coef_li, t))
    # assert False, "Part e"
    # e) should be done
    return (pvt_label_li, pvt_coef_li)

def gt_pvt_info_tpl(coef_li, label_li, num_opt_vars, num_rows):
    dct_kv_lbl_coef = dict()
    ent_var_col = 'a'
    ent_var_lbl_li = label_li[0]
    ent_var_coef_li = coef_li[0]
    # print("lbl_li: {}\nlbl_li[0][0]: {}".format(label_li, label_li[0][0]))
    ent_var_lbl_li = ent_var_lbl_li[1:] # remove opt_val label because it's not an ent_var label
    ent_var_coef_li = ent_var_coef_li[1:]
    # len_each_sublist = num_opt_vars + 1
    len_ent_var_li =  len(ent_var_lbl_li)
    cand_ent_var_lbl_li = []
    for t in range(len_ent_var_li): # get list of candidate entering variable
        #[opt_val, x1, x2, x3] by [1:] to [x1, x2, x3] lect 7 sl 46 s.p x6 = x1
        # [  0   ,  8,  4,  2] by [1:] to [-8,  4,  2] lect 7 sl 46 s.p x6 = x1
        if (ent_var_coef_li[t] >= 0): # CHECK Three
            cand_ent_var_lbl_li.append(ent_var_lbl_li[t]) # [-8, 4, 2] to [4, 2]
    lowest_ent_nmd_lbl = min(cand_ent_var_lbl_li) # min(x1, x2, x3)
    
    # print("lowest numberd ent_var label is: x{}".format(lowest_ent_nmd_lbl))
    
    for i in range(len(ent_var_lbl_li)): # get column number of entering variable
        if (lowest_ent_nmd_lbl == ent_var_lbl_li[i]):
            # THIS index will give me index of m in the ratio c/m. slide 61 simplex
            # TODO README: i + 1 because the label of the opt_var was removed
            ent_var_col = i + 1
            assert ent_var_col > 0, "ent_var_col should be > 0"
    
    # print("Entering variable: x{}, index: {}".format(lowest_ent_nmd_lbl, ent_var_col))
    
    # get label of leaving variable. Must use ent_var_col when get the ration c/m   
    for k in range(num_rows + 1):
        # print("Num row: {}".format(num_rows))
        if (k > 0): # ignore opt_var row
            lv_var_lbl = label_li[k][0]
            tmp_m = coef_li[k][ent_var_col] # the m in c/m ratio
            compute_cm = False
            if (True):
                # print("\ntmp_m: {}\ncoef_li[k][0]: {}, k: {}".format(tmp_m, coef_li[k][0], k))
                if ((tmp_m < 0) and (coef_li[k][0] > 0)):
                    compute_cm = True
                    # print("tmp_m: {}\ncoef_li[k][0]: {}, Ak: {}\n".format(tmp_m, coef_li[k][0], k))
                elif ((tmp_m > 0) and (coef_li[k][0] < 0)):
                    compute_cm = True
                else:
                    ipass = 0
            if (compute_cm): #CHECK 1   
                cm_ratio = coef_li[k][0]/coef_li[k][ent_var_col]
                # print("RATIO K: {}".format(cm_ratio))
                if (cm_ratio <= 0):
                    cm_ratio = abs(cm_ratio)
                    dct_kv_lbl_coef[lv_var_lbl] = cm_ratio # create key:value dictionary of c/m ratios 
    # bool_feasible = is_feasible(coef_li)
    # print("Slack Variable: cm_ratio")
    # print("is kv_lbl_coef: {}".format(dct_kv_lbl_coef))
    if (len(dct_kv_lbl_coef) < 1):
        print("unbounded")
        return False
    tightest_bound = min(iter(dct_kv_lbl_coef.values()))
    # print("tightest bound: {}".format(tightest_bound))
    dct_kv_valid_lv_vars = dict()
    for k, v in dct_kv_lbl_coef.items():
        # print(k, v) 
        # print("x{}:{}".format(k, v))
        if (v == tightest_bound):
            dct_kv_valid_lv_vars[k] = v # since x1 and x6 could have same ratio
    lowest_nmed_lv_var_lbl = min(iter(dct_kv_valid_lv_vars.keys()))
    # print("lowest_nmed_lv: {}".format(lowest_nmed_lv_var_lbl))
    # print("valid leaving variables:")
    
    for k, v in dct_kv_valid_lv_vars.items():
        # pass
        # print("x{}:{}".format(k, v))
        upass = 0
    # print("lowest numberd lv_var: x{} tigh: {}".format(lowest_nmed_lv_var_lbl, tightest_bound))

    return (lowest_ent_nmd_lbl, ent_var_col, lowest_nmed_lv_var_lbl)

def rt_lv_var_label_index(li_with_label_x, lbl_x):
    lbl_x_ind = 1
    count_ind = 0
    for li_i in li_with_label_x:
        if (count_ind > 0):
            if (li_i[0] == lbl_x):
                lv_var_row = lbl_x_ind
                
                # print("The leaving variable is: x{} in row: {} ".format(lbl_x, lv_var_row))
                return lv_var_row
            else:
                lbl_x_ind += 1
        count_ind += 1
    assert False, "rt_lv_var_label_index() loop ended without finding index of leaving variable"

def mk_pivot(prm_labels_li, prm_coef_li, num_rows, num_opt_vars, bool_dl_smplx, bool_on_dl, orgn_obj_fn_row):
    
    pvt_coef_li = prm_coef_li
    pvt_label_li = prm_labels_li
    pvt_num_opt_vars = num_opt_vars
    pvt_num_rows = num_rows
    is_contin = True
    is_optimal = False
    bool_true_once = True
    # print("non_mod_opt_coef: {}".format(non_mod_opt_coef))
    # assert False, "non-modified primal optimal coefficients"
    acount = 1
    # a) find lowest num_ed candidate fro entering variable.
    while (is_contin):
        # pvt_info_tpl = (lowest_ent_nmd_lbl, ent_var_col, lowest_nmed_lv_var_lbl)
        if (not bool_dl_smplx):
            bol_unbounded = is_unbounded(pvt_label_li, pvt_coef_li, num_opt_vars, num_rows)
            if ((bol_unbounded and (not bool_on_dl))):
                print("unbounded")
                return 0
            if ((bol_unbounded) and (bool_on_dl)):
                print("infeasible")
                return 0
        
        pvt_info_tpl = gt_pvt_info_tpl(pvt_coef_li, pvt_label_li, pvt_num_opt_vars, pvt_num_rows)
        if (not pvt_info_tpl):
            return 0
        
        # print("ent_var_lbl: x{} ent_var_col: {} lv_var_lbl: x{}".format(pvt_info_tpl[0], 
        #                                                                 pvt_info_tpl[1], pvt_info_tpl[2]))
        
        # b) solve for ent_var using lv_var and other basic_var
        #Note:
            # pvt_label_li[0], pvt_coef_li[0], pvt_label_li[lv_var_main_ind], pvt_coef_li[lv_var_main_ind]
        lv_var_row_ind = rt_lv_var_label_index(pvt_label_li, pvt_info_tpl[2])
        
        # print("lv_var_row_ind: {}".format(lv_var_row_ind))
        # returns tuple with updated pvt_label_li and pvt_coef_li
        # print("\nPIVOTS\n")
        lbl_coef_tpl = upd_pvt_lbl_coef_li(pvt_info_tpl, pvt_label_li, pvt_coef_li, 
                                            num_opt_vars, lv_var_row_ind, pvt_num_rows)
        pvt_label_li = lbl_coef_tpl[0] # label_coef_tuple
        pvt_coef_li = lbl_coef_tpl[1]  # label_coef_tuple
        bl_feasible = is_feasible(pvt_coef_li)
        
        if (bool_dl_smplx):
            # print("label_li: {}\ncoef_li: {}".format(pvt_label_li, pvt_coef_li))
            if (bl_feasible):
                # print("bool_dl_simplex: {}".format(bool_dl_smplx))
                solv_obj_li = slv_orgnl_obj_fn(pvt_label_li, pvt_coef_li, num_opt_vars, num_rows, 
                                                orgn_obj_fn_row)
                bool_dl_smplx = False
                pvt_coef_li[0] = list(solv_obj_li)
                # print("pvt_coef_li[0]: {}".format(pvt_coef_li[0]))
                # assert False, "found feasible dictionary"
                # print(solv_obj_li)
        num_ngtv_coef = 0
        if (pvt_coef_li[0][0] < 0): #CHECK 4
            is_contin = False # The objective value should not be negative if LP is feasible.
        else:    
            for t in range(num_opt_vars + 1):
                if (pvt_coef_li[0][t] < 0):
                    num_ngtv_coef += 1
            if (num_ngtv_coef == num_opt_vars):
                is_contin = False
                is_optimal = True

    if (bool_on_dl):
        dl_again = mk_dual_lbl_coef_li(pvt_label_li, pvt_coef_li, num_rows, num_opt_vars)
        len_dl_opt_val_list = len(dl_again[1][0])
        for t in range(len_dl_opt_val_list):
            if (t == 0):
                print("optimal\n{}".format(dl_again[1][0][0]))
            else:
                if (t < len_dl_opt_val_list):
                    print("{} ".format(dl_again[1][0][t]),end='')
                else:
                    print("{}".format(dl_again[1][0][t]),end='')
        print("")
        # dl_opt_val_list = get_opt_values(dl_again[0], dl_again[1], dl_again[3], dl_again[2])
        # len_dl_opt_val_list = len(dl_opt_val_list)
        # for i in range(len_dl_opt_val_list):
        #     if (i < num_opt_vars):
        #         print("{} ".format(dl_opt_val_list[i]),end='')
        #     else:
        #         print("{}".format(dl_opt_val_list[i]),end='')
    else:
        if (is_optimal):
            print("optimal\n{}".format(pvt_coef_li[0][0]))
            opt_val_list = get_opt_values(pvt_label_li, pvt_coef_li, num_opt_vars, num_rows)
            len_opt_val_list = len(opt_val_list)
        
        # print("{}".format(opt_val_list))
        
        for i in range(len_opt_val_list):
            if (i < num_opt_vars):
                print("{} ".format(opt_val_list[i]),end='')
            else:
                print("{}".format(opt_val_list[i]),end='')
        print("")

    return 0

def dl_labels_li(num_opt_vars):
    dl_list = []
    for i in range(num_opt_vars + 1):
        dl_list = ['a']
def mk_dual_lbl_coef_li(prm_label_li, prm_coef_li, prm_num_rows, prm_num_opt_vars):
    
    dl_row_k = 0
    p_row_k = 0
    # prm_num_rows is index at 0. sl 38 lect 7 4 prm rows gives 4 dual_opt_vars
    dl_num_opt_vars = prm_num_rows
    dl_num_rows = prm_num_opt_vars
    dl_coef_li = []
    dl_tmp_li = ['a']
    # print("prm_list: {}\n{}\n{}\n{}\n".format(prm_label_li, prm_coef_li, prm_num_rows, prm_num_opt_vars))
    dl_a_list = [x for x in range(dl_num_opt_vars + 1)] # + 1 to include label for optimal value
    # make dual label list of lists
    dl_label_li = []
    for i in range(dl_num_rows + 1): # + 1 to include the row for the optimal value i.e row 0
        dl_label_li.append(list(dl_a_list))
        if (i == 0):
            dl_label_li[0][0] = "dl_opt_var"
        else:
            dl_label_li[i][0] = i + dl_num_opt_vars
    #[[dl_opt_var,1,2,..,],[y5,y1,y2,...]]
    # print("row {}: {}".format(i, dl_label_li[i-1]))
    
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
    # start of can I salvage the time I spent writting mk_dual_lbl_coef_li? This function.
    # for k in range(dl_slk_coef_len):
    #     a_coef = nw_dl_slk_coef_li[k]
    #     if (a_coef > 0):
    #         all_slk_ngtv = False
    # if (all_slk_ngtv):
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
            #making one list per row from row 1 to row prm_num_ rows i.e(w1=x4, x1,x2,x3)
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
    # for k in range(row_k + 1):
    #     print("row {} labels: {}".format(k, prm_labels_li[k]))
    return prm_labels_li
   
def read_kbd_in():
    # print("Enter the Lp. When done enter the letter <d> for done and press enter")
    user_str = "Good"
    pr__kth_row_labels_li = []
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
                tmp_li = [int(x) for x in lp_input.split()]
                num_opt_var = len(tmp_li)
                tmp_li.insert(0,0)
                # li_two = [-1*y for y in tmp_li[0:num_opt_var]]
                # li_two.append(tmp_li[num_opt_var])
                
                # print("li_two is: {}".format(tmp_li))
                prm_coeff_li.append(tmp_li) # input split by spaces
                
                # print("sum is: {}".format(tmp_li[1] + tmp_li[2]))
                
                # pr_coeff_li[0].append('opt_val')
                ind_rows += 1
            else:
                # NOTE: The value of the slack_var is at the end of tmp_li so at tmp_li[len(tmp_li) - 1]
                tmp_li = [float(x) for x in lp_input.split()] 
                li_two = [-1*y for y in tmp_li[0:num_opt_var]] #li_two should not contain value of slack_var
                slack_val = tmp_li.pop(num_opt_var)
                li_two.insert(0, slack_val)
                
                # print("li_two is: {}".format(li_two))
                prm_coeff_li.append(li_two) # input split by spaces
                
                # print("sum is: {}".format(tmp_li[0] + tmp_li[1]))
                
                # pr_coeff_li[0].append('opt_val')
                ind_rows += 1
    prm_dict_num_rows = ind_rows - 1       
    # print("The sing list is: {}\n".format(sing_li))
    
    # for li_i in prm_coeff_li:
    #     print(li_i)
    # print("The input list is: \n{}".format(pr_coeff_li))
    
    # print("The number of rows is: {}".format(prm_dict_num_rows))
    # print(prm_coeff_li)
    # assert False, "list before any functions"
    prm_label_li = mk_pr_label_li(prm_dict_num_rows, num_opt_var)
    
    # for k in range(len(prm_label_li)):
    #     print("row {} labels: {} x1,x2,...,xn".format(k, prm_label_li[k]))
    #     print("row {} coeffi: {}".format(k, prm_coeff_li[k]))
    
    return [prm_label_li, prm_coeff_li, prm_dict_num_rows, num_opt_var]


def main():
    atoke = argparse.ArgumentParser()
    # atoke.add_argument('iLp',  )
    # print("read_kbd call")
    # num_list = [2, 5, 6, 9, 1, 12]
    # num_mlt_li = [3*x for x in num_list]
    # num_add_li = []
    # for i in range(len(num_mlt_li)):
    #     num_add_li.append(num_list[i] + num_mlt_li[i])
    # print("num_list: {}\nnum_mlt_li: {}\nnum_add_li: {}".format(num_list, num_mlt_li, num_add_li))
    # nws_li = list(num_list)
    # print(nws_li)
    # return 0
    lbl_coef_tpl = read_kbd_in() # lable_coefficient_numRows_numOptVar_tuple
    not_to_chg_li = list(lbl_coef_tpl[1][0]) # the coefficient row of the original primal lp
    dl_lbl_coef_tpl = mk_dual_lbl_coef_li(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], lbl_coef_tpl[3])
    # print(lbl_coef_tpl[1])
    prm_feasible = is_feasible(lbl_coef_tpl[1])
    dl_feasible = is_feasible(dl_lbl_coef_tpl[1])
    # print(lbl_coef_tpl[1])
    if (not prm_feasible):
        if (dl_feasible):
            # assert False, "not prm_feasible, dl_feasible"
            dl_pivots = mk_pivot(dl_lbl_coef_tpl[0], dl_lbl_coef_tpl[1], dl_lbl_coef_tpl[2], 
                                    dl_lbl_coef_tpl[3], False, True, not_to_chg_li)
        else:
            if (not dl_feasible):
                # initial dictionary is infeasible
                # assert False, "not prm_feasible, not dl_feasible"
                mod_opt_row_coef_li = modified_dct(lbl_coef_tpl[1], lbl_coef_tpl[3]) 
                # print("{}\n{}".format(mod_opt_row_coef_li, lbl_coef_tpl[1]))
                # print("initial dictionary is infeasible")
                lbl_coef_tpl[1][0] = mod_opt_row_coef_li
                # print(lbl_coef_tpl[1])
                pivots = mk_pivot(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], 
                                    lbl_coef_tpl[3], True, False, not_to_chg_li)
                # assert False, "initially not feasible"
            else:
                assert False, "not sure what's wrong. Something must have gone really wrong. Line 681"
        # assert False, "not prm_feasible, dl_feasible"
    else:
        if (prm_feasible):
            # assert False, "feasible (un)bounded"
            pivots = mk_pivot(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], 
                                lbl_coef_tpl[3], False, False, not_to_chg_li)
            # assert False, "feasible (un)bounded"
        else:
            print("infeasible")
    return 0
    assert False, "dl should have run"
    print(bool_feasible)
    if (not bool_feasible): #not false = true
        # initial dictionary is infeasible
        mod_opt_row_coef_li = modified_dct(lbl_coef_tpl[1], lbl_coef_tpl[3]) 
        # print("{}\n{}".format(mod_opt_row_coef_li, lbl_coef_tpl[1]))
        # print("initial dictionary is infeasible")
        lbl_coef_tpl[1][0] = mod_opt_row_coef_li
        print(lbl_coef_tpl[1])
        pivots = mk_pivot(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], lbl_coef_tpl[3], True)
        assert False, "initially not feasible"
    # pivots = mk_pivot(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], lbl_coef_tpl[3])
    
if __name__ == "__main__":
    main()