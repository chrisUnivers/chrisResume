import init_lp
import pivot
import dual_dictionary
import auxiliary

def main():
    #init_lp return: [prm_label_li, prm_coeff_li, prm_dict_num_rows, num_opt_var]
    lbl_coef_tpl = init_lp.init_input() # lable_coefficient_numRows_numOptVar_tuple
    not_to_chg_li = list(lbl_coef_tpl[1][0]) # the coefficient row of the original primal lp
    dl_lbl_coef_tpl = dual_dictionary.mk_dual_lbl_coef_li(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], lbl_coef_tpl[3])
    prm_feasible = pivot.is_feasible(lbl_coef_tpl[1])
    dl_feasible = pivot.is_feasible(dl_lbl_coef_tpl[1])
    if (not prm_feasible):
        if (dl_feasible):
            # assert False, "not prm_feasible, dl_feasible"
            dl_pivots = pivot.mk_pivot(dl_lbl_coef_tpl[0], dl_lbl_coef_tpl[1], dl_lbl_coef_tpl[2], 
                                    dl_lbl_coef_tpl[3], False, True, not_to_chg_li, False)
        else:
            if (not dl_feasible):
                # initial dictionary is infeasible
                # assert False, "not prm_feasible, not dl_feasible"
                org_objt_fn_coef = lbl_coef_tpl[1][0][:]
                auxiliary.mk_auxiliary_dictonary(lbl_coef_tpl[0], lbl_coef_tpl[1]) 
                pivots = pivot.mk_pivot(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], 
                                    lbl_coef_tpl[3], True, False, org_objt_fn_coef, True)
                # assert False, "initially not feasible"
            else:
                assert False, "not sure what's wrong. Something must have gone really wrong. Line 681"
        # assert False, "not prm_feasible, dl_feasible"
    else:
        if (prm_feasible):
            # assert False, "feasible (un)bounded"
            pivots = pivot.mk_pivot(lbl_coef_tpl[0], lbl_coef_tpl[1], lbl_coef_tpl[2], 
                                lbl_coef_tpl[3], False, False, not_to_chg_li, False)
            # assert False, "feasible (un)bounded"
        else:
            print("infeasible")
    return 0

if __name__ == "__main__":
    main()