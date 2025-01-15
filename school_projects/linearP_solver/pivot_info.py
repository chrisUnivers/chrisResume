# Returns the list of entering varaible candidates
def candidate_ent_var(li_ent_var_lbs, li_ent_var_coef):
    cand_ent_var_lbl_li = []
    len_ent_var_li = len(li_ent_var_lbs)

    for t in range(len_ent_var_li): # get list of candidate entering variable
        #[opt_val, x1, x2, x3] by [1:] to [x1, x2, x3] lect 7 sl 46 s.p x6 = x1
        # [  0   ,  8,  4,  2] by [1:] to [-8,  4,  2] lect 7 sl 46 s.p x6 = x1
        if (li_ent_var_coef[t] >= 0): # CHECK Three
            cand_ent_var_lbl_li.append(li_ent_var_lbs[t]) # [-8, 4, 2] to [4, 2]
    
    return cand_ent_var_lbl_li

# Returns the column of the entering variable
def get_ent_var_col_index(li_ent_var_lb, lowest_ent_nmd_lb):
    
    ent_var_col_ind = 'a'
    for i in range(len(li_ent_var_lb)): # get column number of entering variable
        if (lowest_ent_nmd_lb == li_ent_var_lb[i]):
            # THIS index will give me index of m in the ratio c/m. slide 61 simplex
            # TODO README: i + 1 because the label of the opt_var was removed
            ent_var_col_ind = i + 1
            assert ent_var_col_ind > 0, "ent_var_col should be > 0"

    return ent_var_col_ind


def get_dct_lb_coef(num_rows, li_label, li_coef, ent_var_col, dct_kv_lb_coef):
    
    cm_ratio = None

    for k in range(num_rows + 1):
        if (k > 0): # ignore optimal variable row
            lv_var_lb = li_label[k][0]
            tmp_m = li_coef[k][ent_var_col]
            compute_cm = False
            if (True):
                # print("\ntmp_m: {}\ncoef_li[k][0]: {}, k: {}".format(tmp_m, coef_li[k][0], k))
                if ((tmp_m < 0) and (li_coef[k][0] > 0)):
                    compute_cm = True
                    # print("tmp_m: {}\nli_coef[k][0]: {}, Ak: {}\n".format(tmp_m, li_coef[k][0], k))
                elif ((tmp_m > 0) and (li_coef[k][0] < 0)):
                    compute_cm = True
                else:
                    ipass = 0
            if (compute_cm): #CHECK 1   
                cm_ratio = li_coef[k][0]/li_coef[k][ent_var_col]
                # print("RATIO K: {}".format(cm_ratio))
                if (cm_ratio <= 0):
                    cm_ratio = abs(cm_ratio)
                    dct_kv_lb_coef[lv_var_lb] = cm_ratio # create key:value dictionary of c/m ratios where the key is the lv_var_lb and the value is the c/m ratio 
                    
    #NOTE: lb_coef_dct does not need to be returned since it gets modified in the above if(cm_ratio <= 0) statement.
    # return dct_kv_lb_coef

# Modifies(returns) the dictionary of valid leaving variables
def get_lv_var_dct(dct_lb_coef, tightest_bound, dct_lv_vars):
    
    # dct_lb_coef = {x_i, some coef}
    for k, v in dct_lb_coef.items():
        if (v == tightest_bound):
            # NOTE: the dct_lv_vars is modified. I think I don't even have to return it.
            dct_lv_vars[k] = v # since x1 and x6 could have same ratio.

    # return dct_lv_vars

############################################################
def gt_pvt_info_tpl(coef_li, label_li, num_opt_vars, num_rows, objt_fn_was_mdf, gvn_ent_var_ind, use_aux):
    dct_kv_lb_coef = dict()
    dct_kv_lv_vars = dict()
    ent_var_col = 'a'
    ent_var_lb_li = label_li[0]
    ent_var_coef_li = coef_li[0]

    ent_var_lb_li = ent_var_lb_li[1:] # remove opt_val label because it's not an ent_var label
    ent_var_coef_li = ent_var_coef_li[1:] # remove opt_val coefficient as it is not use in pivots
    # len_each_sublist = num_opt_vars + 1
    len_ent_var_li =  len(ent_var_lb_li)

    #candidate_ent_var(ent_var_lb_li, ent_var_coef_li)
    if (use_aux):
        # Ignor the lowest entering numbered label when objective function was modified.
        # the index of the auxiliary variable i.e: (x2, x1, x3, aux). Choose aux instead of x1
        lowest_ent_nmd_lbl = 'aux' # gvn_ent_var_ind 
    else:
        ###NOTE: when vanderbei_exercise2.3.txt is used, at some point candidate_ent_var is called with
        # an all negative coefficient list meaning no there are no valid entering variables. Factor this in###
        cand_ent_var_lb_li = candidate_ent_var(ent_var_lb_li, ent_var_coef_li)
        # The lowest entering numbered label: min(x2, x1, x3) i.e Bland pivot rule.
        lowest_ent_nmd_lbl = min(cand_ent_var_lb_li) # Choose x1. use_aux = False
    
    if (use_aux):
        ent_var_col = gvn_ent_var_ind # column of the auxiliary variable
    else:
        # get the index of the enter variable column.
        ent_var_col = get_ent_var_col_index(ent_var_lb_li, lowest_ent_nmd_lbl)
    
    #NOTE: uncomment line below if get_dct_lb_coef returns a dictionary. Currently it modifies the dct_kv_lb_coef.
    # dct_kv_lb_coef = get_dct_lb_coef(num_rows, label_li, coef_li, ent_var_col, dct_kv_lb_coef)
    # initialize the label coefficient dictionary
    get_dct_lb_coef(num_rows, label_li, coef_li, ent_var_col, dct_kv_lb_coef)

    if (len(dct_kv_lb_coef) < 1):
        print("unbounded")
        return False
    
    tightest_bound = min(iter(dct_kv_lb_coef.values()))
    # print("tightest bound: {}".format(tightest_bound))

    get_lv_var_dct(dct_kv_lb_coef, tightest_bound, dct_kv_lv_vars)
    
    lowest_nmed_lv_var_lbl = min(iter(dct_kv_lv_vars.keys()))
    
    return (lowest_ent_nmd_lbl, ent_var_col, lowest_nmed_lv_var_lbl)
