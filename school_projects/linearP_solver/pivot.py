import pivot_info
import dual_dictionary
import auxiliary

# Assigns non_bsc_vars_ind the indices of the non_bcs_variables in order i.e 1st index is for x1
def get_non_basic_vars_indices(label_li, non_bsc_vars_ind):

    kth_row = 0

    for cur_label_li in label_li:
        if (kth_row == 0):
            non_bsc_vars_ind.append(cur_label_li[0])
            kth_row += 1
        else:
            non_bsc_vars_ind.append(cur_label_li[0])


def get_non_basic_values(coeff_li, non_bsc_vls_li):

    kth_row = 0
    for cur_coef_li in coeff_li:
        if (kth_row == 0):
            kth_row += 1
        else:
            non_bsc_vls_li.append(cur_coef_li[0])


def has_valid_entering_variable(pvt_coef_li, num_opt_vars):

    num_ngtv_coef = 0
    valid_ent_variable = True
    
    for t in range(num_opt_vars + 1):
        if (pvt_coef_li[0][t] < 0 and (t > 0)):
            num_ngtv_coef += 1
    if (num_ngtv_coef == num_opt_vars):
        valid_ent_variable = False

    return valid_ent_variable

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

def upd_pvt_lbl_coef_li(pvt_info_tpl, pvt_label_li, pvt_coef_li, num_opt_vars, lv_var_row, aux_var_col, num_rows, use_aux, lp_labels, lp_coeffs, lp_ever_aux):
    
    # pvt_info_tpl:
        # pvt_info_tpl = (lowest_ent_nmd_lbl, ent_var_col, lowest_nmed_lv_var_lbl)

    #NOTE: must change this function to use the/check when to use the boolean: use_aux.
    ent_var_col = pvt_info_tpl[1]

    # a) increment obj_fn_val
    lv_var_const = pvt_coef_li[lv_var_row][0]
    # lv_var_mult = pvt_coef_li[lv_var_row][ent_var_col]
    ent_var_coef  = pvt_coef_li[0][ent_var_col]

    if (use_aux):
        ent_var_col = aux_var_col
        lv_var_mult = pvt_coef_li[lv_var_row][ent_var_col]
    else:
        ent_var_col = pvt_info_tpl[1]
        lv_var_mult = pvt_coef_li[lv_var_row][ent_var_col]
    # a) should be good

    #NOTE: must change this function to use the/check when to use the boolean: use_aux.
    # b) make ent_var the new slack_var and update obj_val row's label
    tmp_label = pvt_label_li[lv_var_row][0] # current slack variable label soon to be basic_var.
    pvt_label_li[lv_var_row][0] = pvt_info_tpl[0] # set new slack variable label
    
    # move old slack variable in the column of the entering variable.
    pvt_label_li[lv_var_row][ent_var_col] = tmp_label
    pvt_label_li[0][ent_var_col] = tmp_label # update obj_fn row's label with old slack var now a basic var
    # assert False, "Part b"
    # b) should be done
    # c) update coefficients in row lv_var_row.
    
    # if (use_aux):
    #     len_each_sub_list = num_opt_vars + 1
    # else:
    #     len_each_sub_list = num_opt_vars
    
    len_each_sub_list = len(pvt_label_li[0]) - 1 # minus 1 to not count the optimal value
        
    lv_var_row_coef_li = pvt_coef_li[lv_var_row] # Should be 1st row in lect 7 slide 53.
    lv_var_coef = -1 * lv_var_mult
    for i in range(len_each_sub_list + 1):
        tmp_val = lv_var_row_coef_li[i]
        lv_var_row_coef_li[i] = tmp_val/lv_var_coef
    lv_var_row_coef_li[ent_var_col] = -1/lv_var_coef
    pvt_coef_li[lv_var_row] = lv_var_row_coef_li
    
    # assert False, "Part c"
    # c) should be done
    
    # d) for each li in label_li, set label in ent_var_col to lv_var_label
    slak_to_bsc_lbl = pvt_label_li[0][ent_var_col]
    for k in range(num_rows + 1):
        if (k != 0):
            pvt_label_li[k][ent_var_col] = slak_to_bsc_lbl
    ro_k = 0
    for li_k in pvt_label_li:
        ro_k += 1
    # assert False, "Part d"
    # d) should be done

    # e) for all non-lv_var_li in coef_li, update those coefficients 
        # i.e substitu ent_var in each non-lv_var_row
    debug_var = 0
    for t in range(num_rows + 1):
        if (t != lv_var_row): # t != lv_var_row
            row_t_mult = pvt_coef_li[t][ent_var_col]
            # row_t_upd_li = [-2, 2, 0, 0] sl 45 lec 7 row of w1
            row_t_upd_li = [row_t_mult*x for x in lv_var_row_coef_li]
            
            row_t_coef_li = pvt_coef_li[t] # [8, -2, -4, 0] sl 45
            # update the coeff of each row based on the coeff on the variable that just entered, including objt_fn's row but excluding the row of the leaving variable.
            for s in range(len_each_sub_list + 1): # plus 1 because the value of the obj_fn is also updated
                cur_coef = row_t_coef_li[s]
                if (s != ent_var_col):
                    row_t_coef_li[s] = cur_coef + row_t_upd_li[s]
                else:
                    row_t_coef_li[s] = row_t_upd_li[ent_var_col]
    # e) should be done
    
    lp_labels[0] = pvt_label_li
    lp_coeffs[0] = pvt_coef_li
    # lp was once an auxiliary problem and now has objective fn == to 0 again.
    if (lp_ever_aux):
        if (pvt_coef_li[0][0] == 0):
            # Remove the auxiliary variable before returning!
            auxiliary.aux_to_lp(lp_labels, lp_coeffs) 
            return False # lp was once an aux problem and now objective_fn = 0
        else:
            # Since the lp was once an aux lp, then if optm val > 0 this means the lp is no longer an aux lp and we can use the regular simplex method. I think.
            if (pvt_coef_li[0][0] < 0):
                return True
            else:
                return False
    else:
        return False

# Reincorporates the original function using the lp dictionary obtained by using the auxiliary method
def reincorp_original_objt_fn(orgn_objt_fn_coef_li, mod_optm_coef_li, 
                              mod_optm_lb_li, reincorp_objt_fn_coef_li): 
    # This function is the only place where _optm_ abbriviates optimal. Else where, it stands for _optm_vars stands for optimization variables

    #reincorp_objt_fn_coef_li is originally: [0, 0, 0, ..., num_optmz_vars]
    
    ith_lb = 1 # 1 for label x1
    len_reincorp = len(reincorp_objt_fn_coef_li[0]) # number of optimization variables plus optimal value
    considered_ith_li = []

    non_bsc_var_lb_ind = []
    coeff_li = orgn_objt_fn_coef_li[1:]
    get_non_basic_vars_indices(mod_optm_lb_li, non_bsc_var_lb_ind) # non_bsc_var_lb_ind[0] = index of x1
    for coeff in coeff_li: # The first coeff will be the coeff of label/optmz_var x1
        try:
            ith_lb_ind = non_bsc_var_lb_ind.index(ith_lb)
            considered_ith_li.append(ith_lb)
            reincorp_objt_fn_coef_li[0] = [reincorp_objt_fn_coef_li[0][i] 
                                       + (coeff * mod_optm_coef_li[ith_lb_ind][i]) for i in 
                                       range(len_reincorp)]
        except ValueError:
            # ith optimization variable is a basic variable i.e x_i = 0 so we can a 0 to reincorp_objt_fn_coef_li or do nothing. 
            pass
        ith_lb += 1

def get_opt_values(label_list, coef_list, num_rows, num_optm_vars):
    # pvt_label_li, pvt_coef_li, num_opt_vars, num_rows
    # [x1,x2,x3,...,xn]
    num_opt_var_p_one = num_optm_vars + 1

    opt_vals_found = 0
    # set basic optimization variables to zero in opt_values_li
    for i in range(num_opt_var_p_one): 
        if (i > 0):
            if (i in label_list[0]):
                opt_vals_found += 1
    
    opt_vrs_vls_dict = {x: 0*x for x in range(num_optm_vars + 1) if x > 0}
    row_k = 0
    for li_k in label_list: # checking row k, k != optimal value row
        if (row_k > 0):
            for t in range(num_opt_var_p_one):
                # t = x1,2,3,...,n-1,xn
                if (t == li_k[0]): # li_k is a list in label
                    opt_vrs_vls_dict[t] = coef_list[row_k][0]
                    t = num_opt_var_p_one # end this loop and check next row's labels.
                    opt_vals_found += 1
            row_k += 1
        else:
            row_k += 1
    if (opt_vals_found == num_optm_vars):
        # return opt_values_li
        return opt_vrs_vls_dict
    
    assert False, "opt_valuse_found != num_optm_vars. Line 51, this should not happen. if everything works fine"
    return 0

def mk_pivot(prm_labels_li, prm_coef_li, num_rows, num_opt_vars, bool_dl_smplx, bool_on_dl, orgn_obj_fn_coef, mod_obj_fn):
    
    pvt_coef_li = prm_coef_li
    pvt_label_li = prm_labels_li
    pvt_num_opt_vars = num_opt_vars
    pvt_num_rows = num_rows
    is_contin = True
    is_optimal = False
    non_bsc_val_li = []
    nth_pivot = 0 #NOTE: nth_pivot might cause problems currently only being incremented once.
    use_aux = False
    aux_ent_col = 0
    lp_ever_aux = False
    lp_coeffs = [0]
    lp_labels = [1]
    lbl_coef_tpl = None
    reincorp_orgn_fn = False
    any_valid_entering_variable = False
    # a) find lowest num_ed candidate from entering variable.

    while (is_contin):
        if (not bool_dl_smplx):
            bol_unbounded = is_unbounded(pvt_label_li, pvt_coef_li, num_opt_vars, num_rows)
            if ((bol_unbounded and (not bool_on_dl))):
                print("unbounded")
                return 0
            if ((bol_unbounded) and (bool_on_dl)):
                print("infeasible")
                return 0
        
        if (mod_obj_fn and (nth_pivot == 0)): # modify objective function
            aux_ent_col = len(prm_coef_li[0]) - 1 # index/column of the auxiliary variable
            get_non_basic_values(prm_coef_li, non_bsc_val_li)
            use_aux = True
            lp_ever_aux = True
            reincorp_orgn_fn = True
            nth_pivot += 1
        else:
            use_aux = False
        
        pvt_info_tpl = pivot_info.gt_pvt_info_tpl(pvt_coef_li, pvt_label_li, pvt_num_opt_vars, pvt_num_rows, mod_obj_fn, aux_ent_col, use_aux)
        if (not pvt_info_tpl):
            return 0
        
        # b) solve for ent_var using lv_var and other basic_var
        if (use_aux):
            # plus 1 since non_basic_values_list is indexed at 0 but, non basic coeffients start 1 row below objt fn.
            lv_var_row_ind = non_bsc_val_li.index(min(non_bsc_val_li)) + 1
        else:
            lv_var_row_ind = rt_lv_var_label_index(pvt_label_li, pvt_info_tpl[2])
        
        # Will continue looping if the objective fn had an auxiliary variable and the current objt_fn > 0
        cnt_for_aux = True
        
        # updates the label and coefficient "dictionaries" for each pivot
        while (cnt_for_aux):
            cnt_for_aux = upd_pvt_lbl_coef_li(pvt_info_tpl, pvt_label_li, pvt_coef_li, 
                                                num_opt_vars, lv_var_row_ind, aux_ent_col, pvt_num_rows, use_aux, lp_labels, lp_coeffs, lp_ever_aux)
            use_aux = False # set to False after the auxiliary variable has been used as an entering variable
            
            if (cnt_for_aux and lp_ever_aux):
                any_valid_entering_variable = has_valid_entering_variable(pvt_coef_li, num_opt_vars)
                if (any_valid_entering_variable):
                    pvt_info_tpl = pivot_info.gt_pvt_info_tpl(pvt_coef_li, pvt_label_li, pvt_num_opt_vars, pvt_num_rows, mod_obj_fn, aux_ent_col, use_aux)
                    lv_var_row_ind = rt_lv_var_label_index(pvt_label_li, pvt_info_tpl[2])
                else:
                    cnt_for_aux = False

        lbl_coef_tpl = (lp_labels, lp_coeffs)
        pvt_label_li = lbl_coef_tpl[0][0] # label_coef_tuple
        pvt_coef_li = lbl_coef_tpl[1][0]  # label_coef_tuple
        # reincorporate the original objective function.
        if (reincorp_orgn_fn):
            reincorp_orgn_fn = False
            reincorp_objt_fn_coef_li = [[0 for x in range(num_opt_vars + 1)]]
            reincorp_original_objt_fn(orgn_obj_fn_coef, pvt_coef_li, pvt_label_li, reincorp_objt_fn_coef_li)
            pvt_coef_li[0] = reincorp_objt_fn_coef_li[0][:]
        bl_feasible = is_feasible(pvt_coef_li)


        num_ngtv_coef = 0
        if (pvt_coef_li[0][0] < 0): #CHECK 4
            # The objective value should not be negative if LP is feasible.
            # is_contin = False
            for t in range(num_opt_vars + 1):
                if (pvt_coef_li[0][t] < 0 and (t > 0)):
                    num_ngtv_coef += 1
            if (num_ngtv_coef == num_opt_vars):
                is_contin = False
                is_optimal = True
        else:    
            for t in range(num_opt_vars + 1):
                if (pvt_coef_li[0][t] < 0 and (t > 0)):
                    num_ngtv_coef += 1
            if (num_ngtv_coef == num_opt_vars):
                is_contin = False
                is_optimal = True

    if (bool_on_dl):
        pvt_label_li[0][0] = 'dl_opt_var_to_prm'
        dl_again = dual_dictionary.mk_dual_lbl_coef_li(pvt_label_li, pvt_coef_li, num_rows, num_opt_vars)
        #returns the values of the optimal variables
        vls_opt_dict = get_opt_values(dl_again[0], dl_again[1], dl_again[2], dl_again[3])
        len_dl_opt_val_list = len(dl_again[1][0])
        for t in range(len_dl_opt_val_list):
            if (t == 0):
                print("optimal\n{}".format(dl_again[1][0][0]))
            else:
                if (t < len_dl_opt_val_list):
                    print("{} ".format(vls_opt_dict[t]),end='')
                else:
                    print("{}".format(vls_opt_dict),end='')
        print("")
    else:
        if (is_optimal):
            print("optimal\n{}".format(pvt_coef_li[0][0]))
            opt_val_list = get_opt_values(pvt_label_li, pvt_coef_li, num_rows, num_opt_vars)
            len_opt_val_list = len(opt_val_list)
        
        for i in range(len_opt_val_list + 1):
            if (i > 0):
                print("{} ".format(opt_val_list[i]),end='')
        print("")

    return 0