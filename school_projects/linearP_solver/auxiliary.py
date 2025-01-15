import pivot_info
import dual_dictionary


def aux_to_lp(lp_labels, lp_coeffs):

    aux_var_ind = lp_labels[0][0].index('aux')
    len_val_list = len(lp_labels[0])
    for i in range(len_val_list):
        lp_labels[0][i].pop(aux_var_ind)
        lp_coeffs[0][i].pop(aux_var_ind)

def insert_aux_var(prm_label_li, prm_coeff_li):

    fn_list = []
    kth_row = 0
    for label_li in prm_label_li:
        label_li.append('aux')
    
    for coef_li in prm_coeff_li:
        if (kth_row == 0):
            len_coef = len(coef_li)
            for i in range(len_coef):
                coef_li[i] = 0
            coef_li.append(-1) # Coeffience of the auxiliary variable.
            kth_row += 1
        else:
            coef_li.append(1)

def mk_auxiliary_dictonary(prm_label_li, prm_coeff_li):

    insert_aux_var(prm_label_li, prm_coeff_li)
    aux_var = True