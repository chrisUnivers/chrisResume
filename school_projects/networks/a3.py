from statistics import stdev
import struct
import argparse


class IP_Header:
    src_ip = None #<type 'str'>
    dst_ip = None #<type 'str'>
    ip_header_len = None #<type 'int'>
    total_len = None    #<type 'int'>
    my_id = None #<type 'int'>
    my_protocol = None
    my_time_leave = None
    ip_flags = None
    is_frgment = None
    frgment_off_set = None
    m_type = None # message type. Only 8 and 11
    def __init__(self):
        self.src_ip = None
        self.dst_ip = None
        self.ip_header_len = 0
        self.total_len = 0
        self.my_id = 0
        self.my_protocol = 0
        self.my_time_leave = 0
        self.ip_flags = {}
        self.frgment_off_set = -1
        self.is_frgment = False
        self.m_type = 0

    
    def ip_set(self,src_ip,dst_ip):
        self.src_ip = src_ip
        self.dst_ip = dst_ip
    
    def header_len_set(self,length):
        self.ip_header_len = length
    
    def total_len_set(self, length):
        self.total_len = length    

    def set_my_id(self, id_value):
        self.my_id = id_value

    def set_my_protocol(self, protc_buf):
        protc_val = struct.unpack('B', protc_buf)[0]
        # print("Hi {}".format(protc_buf))
        self.my_protocol = protc_val #17 = UDP, 1 = ICMP
    
    def set_my_time_lv(self, tl_value):
        self.my_time_leave = tl_value
    
    def set_ip_flags(self, flg_diction):
        self.ip_flags = flg_diction
    
    def set_frgment_off(self, frg_value):
        self.frgment_off_set = frg_value
    
    def set_mType(self, type_vl):
        self.m_type = type_vl
    
    def get_IP(self,buffer1,buffer2):
        src_addr = struct.unpack('BBBB',buffer1)
        dst_addr = struct.unpack('BBBB',buffer2)
        s_ip = str(src_addr[0])+'.'+str(src_addr[1])+'.'+str(src_addr[2])+'.'+str(src_addr[3])
        d_ip = str(dst_addr[0])+'.'+str(dst_addr[1])+'.'+str(dst_addr[2])+'.'+str(dst_addr[3])
        self.ip_set(s_ip, d_ip)
        
    def get_header_len(self,value):
        # result = struct.unpack('B', value)[0]
        # length = (result & 15)*4
        length = value
        self.header_len_set(length)

    def get_total_len(self,buffer):
        num1 = ((buffer[0]&240)>>4)*16*16*16
        num2 = (buffer[0]&15)*16*16
        num3 = ((buffer[1]&240)>>4)*16
        num4 = (buffer[1]&15)
        length = num1+num2+num3+num4
        self.total_len_set(length)
 
class UDP_header:
    src_port = 0
    dst_port = 0
    checksum = 0
    
    def __init__(self):
        self.src_port = 0
        self.dst_port = 0
        self.a_timestamp = 0
        self.checksum = 0
    
    def src_port_set(self, src):
        self.src_port = src
        
    def dst_port_set(self,dst):
        self.dst_port = dst
    
    def a_timestamp_set(self, tm_value):
        self.a_timestamp = tm_value

class ICMP_Header():

    icmp_type = 0
    icmp_code = 0
    icmp_checksum = 0 # has not been set
    pair_src_port = 0
    pair_dst_port = 0
    icmp_ttl = 0

    def __init__(self):
        self.icmp_type = 0
        self.icmp_code = 0
        self.icmp_checksum = 0
        self.pair_src_port = 0
        self.pair_dst_port = 0
        self.icmp_ttl = 0
    
    def set_pair_src_port(self, src_port_vl):
        self.pair_src_port = src_port_vl
    
    def set_pair_dst_port(self, dst_prot_vl):
        self.pair_dst_port = dst_prot_vl

    def set_icmp_ttl(self, tlv_val):
        self.icmp_ttl = tlv_val
   
class UDP_packet():
    ip_header = None
    udp_header = None

    def __init__(self):
        self.ip_header = IP_Header()
        self.udp_header = UDP_header()
    
    def set_ip_hdr(self, ip_hdr_obj):
        self.ip_header = ip_hdr_obj
    
    def set_udp_hdr(self, udp_hdr_obj):
        self.udp_header = udp_hdr_obj


class packet():
    
    #pcap_hd_info = None
    IP_header = None
    UDP_header = None
    ICMP_header = None
    timestamp = 0
    packet_No = 0
    RTT_value = 0
    RTT_flag = False
    buffer = None
    
    
    def __init__(self):
        self.IP_header = IP_Header()
        self.UDP_header = UDP_header() # the 'h' should be 'H' but I'm not changing it for now.
        self.ICMP_header = ICMP_Header()
        #self.pcap_hd_info = pcap_ph_info()
        self.timestamp = 0
        self.packet_No =0
        self.RTT_value = 0.0
        self.RTT_flag = False
        self.buffer = None
    # used to be: timestamp_set(self,buffer1,buffer2,orig_time):
    # couldn't figure out how to use orig_time
    def timestamp_set(self, tm_sec_mlSec):
        # seconds = struct.unpack('I',buffer1)[0]
        # microseconds = struct.unpack('<I',buffer2)[0]
        # self.timestamp = round(seconds+microseconds*0.000001-orig_time,6)
        self.timestamp = tm_sec_mlSec
        #print(self.timestamp,self.packet_No)
    def packet_No_set(self,number):
        self.packet_No = number
        #print(self.packet_No)
        
    def get_RTT_value(self,p):
        rtt = p.timestamp-self.timestamp
        self.RTT_value = round(rtt,8)

def pck_header_fnct(pck_hdr_bytes):
    # print("pck_hdr_bytes: {}".format(pck_hdr_bytes))
    # print(struct.unpack_from('cccc', pck_hdr_bytes, 0))
    # a_pckt_bytes[34:36]
    # ts_sec_buf = pck_hdr_bytes[0:4]
    # ts_usec_buf = pck_hdr_bytes[4:8]
    my_ts_sec = struct.unpack_from('I', pck_hdr_bytes, 0)
    my_ts_usec = struct.unpack_from('I', pck_hdr_bytes, 4)
    my_incl_len = struct.unpack_from('I', pck_hdr_bytes, 8)
    my_orig_len = struct.unpack_from('I', pck_hdr_bytes, 12)
    # pck header tuple
    pck_hdr_tpl = (my_ts_sec[0], my_ts_usec[0], my_incl_len[0], my_orig_len[0])
    
    # my_time_ts_sec = datetime.fromtimestamp(my_ts_sec[0]).strftime("%B %d, %Y %I:%M:%S")
    # my_time_ts_usec = datetime.fromtimestamp(my_ts_usec[0]).strftime("%B %d, %Y %I:%M:%S") 
   
    # print("{}".format(my_time_ts_sec))
    # print("{}".format(pck_hdr_tpl[0]))
    # print("pck_hdr_tuple is: {}".format(pck_hdr_tpl))
    return pck_hdr_tpl

class glb_cap_fl():

    udp_non_frg_list = None
    udp_frg_list = None
    udp_list = None
    icmp_list = None
    type_eleven_list = None
    type_eight_list = None
    src_dst_set = None
    glb_scr_ip = None
    glb_dst_ip = None

    def __init__(self):
        self.udp_frg_list = []
        self.udp_non_frg_list = []
        self.udp_list = []
        self.icmp_list = []
        self.type_eight_list = []
        self.type_eleven_list = []
        self.glb_scr_ip = None #<type = string>
        self.glb_dst_ip = None #<type = string>
    
    def add_to_udp_frg_li(self, pck_obj):
        self.udp_frg_list.append(pck_obj)
        self.udp_list.append(pck_obj)
    def add_to_udp_non_frg_li(self, n_pck_obj):
        self.udp_non_frg_list.append(n_pck_obj)
        self.udp_list.append(n_pck_obj)
    def add_to_icmp_li(self, pck_ob):
        self.icmp_list.append(pck_ob)

    def add_to_ty_eight_li(self, eit_pck_ob):
        self.type_eight_list.append(eit_pck_ob)
    def add_to_ty_elvn_li(self, elv_pck_ob):
        self.type_eleven_list.append(elv_pck_ob)
    
    def set_glb_scr(self, src_i):
        self.glb_scr_ip = src_i
    def set_gbl_dst(self, dst_i):
        self.glb_dst_ip = dst_i


def get_my_IHL(abyte_value):
    ihl_hex = abyte_value.hex()
    my_ihl = int(bin(int(ihl_hex, base = 16)), 2) & 0x0f
    return 4 * my_ihl

def i_initi_udp_header(a_pck_bys, original_pckt):
    u_tpl_rt = None
    if (original_pckt): # original pckt could be a non-fragmented packet of the fragment with port numbers
        src_port_buf = a_pck_bys[34:36]
        dst_port_buf = a_pck_bys[36:38]
        # print("s: {}, d: {}".format(src_port_buf, dst_port_buf))
    else:
        # This should never occur. If it does, something wrong with logic  
        raise NameError('AssertionError')
    # print(src_port_buf)
    u_tpl_rt = (src_port_buf, dst_port_buf)
    return u_tpl_rt
    
def i_initi_icmp_header(a_pck_byts):
    a_tpl_rt = None

    typ_buf = a_pck_byts[34:35]
    code_buf = a_pck_byts[35:36]
    src_port_buff = a_pck_byts[62:64]
    dst_port_buff = a_pck_byts[64:66]
    # print("Hio: {}, Heo: {}".format(src_buff, dst_buff))
    a_tpl_rt = (typ_buf, code_buf, src_port_buff, dst_port_buff)
    return a_tpl_rt


def i_initi_ip_header(a_pck_bytes):
    a_tpl_to_rt = None
    ihl_value = get_my_IHL(a_pck_bytes[14:15]) # [14:15] the ip version & header length bytes
    total_len = a_pck_bytes[16:18] # [17:19] the 16 bit total length in IP headervalue
    # print("MY VALUE: {}".format(total_len))
    id_buff = a_pck_bytes[18:20]
    flg_buff = a_pck_bytes[20:22] 
    frg_oSet_buff = a_pck_bytes[20:22] # same slice as line above thing to avoid confusion
    protc_buff = a_pck_bytes[23:24]
    tm_lv_buff = a_pck_bytes[22:23]
    src_buffer1 = a_pck_bytes[26:30] # scr_ip_adrs
    dest_buffer2 = a_pck_bytes[30:34] # scr_ip_dest
    msg_type_buf = a_pck_bytes[34:35]
    # print("src: {} , dst: {}".format(protc_buff, dest_buffer2))
    a_tpl_to_rt = (src_buffer1, dest_buffer2, ihl_value, total_len, id_buff, flg_buff, frg_oSet_buff, 
                    protc_buff, tm_lv_buff, msg_type_buf)
    # print("++++ The ip total length is: {} ++++".format())
    return a_tpl_to_rt

#return the offset of a give packet. a_buff is: b'\xyy\xyy' and need last 13 bits
def pckt_o_set(a_buff, nl_bits):
    t_buff = b'\x00\x02'

    aBuff_to_hex = a_buff.hex()
    # print("aBuff is: {}".format(aBuff_to_hex))
    
    offset_bits = bin(int(aBuff_to_hex, base = 16))
    bts_len = len(offset_bits)
    
    if (bts_len > 13): # number of bits of flags = 3
        bt_start = bts_len - nl_bits
        offset_val = int(offset_bits[bt_start:], 2)
    else:
        offset_val = int(offset_bits, 2)
    # actual offset
    act_offset = 8 * offset_val
    # print("act_offset: {}".format(act_offset))

    return act_offset

#return the value of the flags of the given packet. First 3 bits give this value
def pckt_flags_fnct(f_buff, m_num):
    fBuff_to_hex = f_buff.hex()
    
    # tBuff = b'\x10\x00'
    # tBuff_to_hex = tBuff.hex()
    # tflag_bits = bin(int(tBuff_to_hex, base=16))
    # tflag_vale = int(tflag_bits[:3], 2)
    
    flag_bits = bin(int(fBuff_to_hex, base = 16))
    i_bits = flag_bits[2:]
    if (len(i_bits) < 14):
        flag_vale = 0
    else:
        flag_vale = int(flag_bits[:4], 2)
    # if (m_num > 20 and m_num < 25):
    #         print("Flag bits: {}, {}".format(i_bits, len(i_bits)))
    #         print("Flag vale: {}".format(flag_vale))

    return flag_vale


def unpa_my_En_b(sBuff, sForm): # big endian
    # protc_val = struct.unpack('B', protc_buf)[0]
    # self.my_protocol = protc_val #17 = UDP, 1 = ICMP
    rt_val = struct.unpack( sForm, sBuff)[0]
    return rt_val
def unpa_my(uBuff, uForm):
    # protc_val = struct.unpack('B', protc_buf)[0]
    # self.my_protocol = protc_val #17 = UDP, 1 = ICMP
    rt_val = struct.unpack(uForm, uBuff)[0]
    return rt_val

def unpa_my_from(uBuff, uForma, uFrom):
    # protc_val = struct.unpack('B', protc_buf)[0]
    # self.my_protocol = protc_val #17 = UDP, 1 = ICMP
    rt_val = struct.unpack(uForma, uBuff, uFrom)[0]
    return rt_val

def assign_ip_header(a_pck_obj, ip_hdr_tpl):
    #ip_hdr_tpl = (src_buffer1, dest_buffer2, ihl_value, total_len, id_buff, flg_buff, frg_oSet_buff, 
    #                protc_buff, tm_lv_buff, type_buff)
    # a_tpl_to_rt = (src_buffer1, dest_buffer2, ihl_value, total_len)
    my_pckt_obj = packet()
    temp_buf = None
    temp_val = None
    a_pck_obj.IP_header.get_IP(ip_hdr_tpl[0], ip_hdr_tpl[1])
    a_pck_obj.IP_header.get_header_len(ip_hdr_tpl[2])
    a_pck_obj.IP_header.get_total_len(ip_hdr_tpl[3])
    a_pck_obj.IP_header.set_my_protocol(ip_hdr_tpl[7])
    temp_val = pckt_o_set(ip_hdr_tpl[6], 13) # returns a_pck_obj's fragement offset
    a_pck_obj.IP_header.frgment_off_set = temp_val
    temp_m_val = unpa_my(ip_hdr_tpl[9],'B')
    a_pck_obj.IP_header.set_mType(temp_m_val)
    
    # if (a_pck_obj.packet_No == 20):
    #     print("-- a_pck_obj type: {}, protocol: {}--".format(a_pck_obj.IP_header.m_type, 
    #         a_pck_obj.IP_header.my_protocol + 1))

    # if (a_pck_obj.packet_No > 20 and a_pck_obj.packet_No < 25):
    #     print("-------- #{}, {} ---------".format(a_pck_obj.packet_No, ip_hdr_tpl[5]))
    if ((a_pck_obj.IP_header.my_protocol != 1) and (a_pck_obj.IP_header.my_protocol != 17)):
        # print("Good Morning: {}".format(a_pck_obj.packet_No))
        return a_pck_obj
       
    if (a_pck_obj.IP_header.my_protocol == 1): # icmp packet
        # print("Hello: {}".format(a_pck_obj.packet_No))
        if ((a_pck_obj.IP_header.m_type != 8) and (a_pck_obj.IP_header.m_type != 11)
            and (a_pck_obj.IP_header.m_type != 3)):
            # if (a_pck_obj.packet_No == 53):
            #     print("---------------OK! {}".format(a_pck_obj.IP_header.src_ip))
            a_pck_obj.IP_header.my_protocol = -4 # non sensical value
            return a_pck_obj

    temp_val = pckt_flags_fnct(ip_hdr_tpl[5], a_pck_obj.packet_No) # return a_pck_obj flag value. Only intereseted in more frgment
    # print("packet fragement off set: {}".format(a_pck_obj.IP_header.frgment_off_set))
    if (a_pck_obj.IP_header.frgment_off_set > 0):
        a_pck_obj.IP_header.is_frgment = True
    else:
        if (a_pck_obj.IP_header.frgment_off_set < 0):
            raise NameError('AssertionError')
        # print("More fragment:True")
    # else:
        # print("More fragment: False")
        
    temp_buf = ip_hdr_tpl[4]
    # if (a_pck_obj.IP_header.my_protocol == 1):
    #     print("Hi there! {}".format(a_pck_obj.IP_header.src_ip))
    temp_val = unpa_my_En_b(temp_buf, '>H')
    a_pck_obj.IP_header.set_my_id(temp_val)
    temp_buf = ip_hdr_tpl[8]
    temp_val = unpa_my(temp_buf, 'B')
    a_pck_obj.IP_header.set_my_time_lv(temp_val)
    # print("packet: {}, ttl: {}".format(a_pck_obj.packet_No, a_pck_obj.IP_header.my_time_leave))
    # print("{}".format(temp_val))
    
    # print("protocol: {}".format(a_pck_obj.IP_header.my_protocol))
    
    # tmp_val = nLast_bits(ip_hdr_tpl[6],3)
    return a_pck_obj
def assign_udp_header(a_udp_pck, udp_hd_tpl):
    # udp_hd_tpl = (src_port_buf, dst_port_buf)
    u_pckt = packet()

    a_udp_pck.UDP_header.src_port = unpa_my_En_b(udp_hd_tpl[0], '>H')
    a_udp_pck.UDP_header.dst_port = unpa_my_En_b(udp_hd_tpl[1], '>H')

    # print("-------\nSource port: {}\nDestination port:  {}\n------".format(udp_hd_tpl[0], udp_hd_tpl[1]))
    
    return a_udp_pck
def assign_icmp_header(a_icmp_pck, icmp_hd_tpl):
    # icmp_hdr_tpl = (typ_buf, code_buf, src_port_buff, dst_port_buff)
    i_pckt = packet()    
    a_icmp_pck.ICMP_header.icmp_type = unpa_my(icmp_hd_tpl[0], 'B')
    a_icmp_pck.ICMP_header.icmp_code = unpa_my(icmp_hd_tpl[1], 'B')
    a_icmp_pck.ICMP_header.pair_src_port = unpa_my_En_b(icmp_hd_tpl[2], '>H')
    # print("dst port:: {}".format(icmp_hd_tpl[3]))
    a_icmp_pck.ICMP_header.pair_dst_port = unpa_my_En_b(icmp_hd_tpl[3], '>H')
    return a_icmp_pck

def assign_T_eight_seq(t_eight_pckt, pckt_bytes):

    eit_pckt = packet()
    seq_buf = pckt_bytes[40:42]

    return 0

def assign_T_elven_seq(t_elv_pckt, elv_pckt_bytes):

    return 0

def mk_nw_pckt(my_pckt_bytes, my_ts_sec, pckt_number, frg_pck_ids_set, wnds_bl):
    # part B tuple: (Source Address, Destination addres, Source Port, Destinatin Port)
    # src_ip_adrs = struct.unpack_from('BBBB', my_data_bytes, 26)
    # scr_ip_adrs starts at byte 26 when given the data_bytes of a packet
    partB_li = []
    nw_packet_obj = packet() 
    ip_hdr_tpl = i_initi_ip_header(my_pckt_bytes)
    tmp_frg_pckt_id = 0
    original_bl = False
    on_wnds = False
    
    nw_packet_obj.packet_No_set(pckt_number)
    
    nw_packet_obj = assign_ip_header(nw_packet_obj, ip_hdr_tpl)
    
    # pckt_time = datetime.fromtimestamp(my_ts_sec).strftime("%B %d, %Y %I:%M:%S")
    
    nw_packet_obj.timestamp_set(my_ts_sec)
    if (nw_packet_obj.IP_header.my_protocol == 1): # icmp ttl exceeded packet
        # assign icmp header
        # icmp_hdr_tpl = (typ_buf, code_buf, src_buff, dst_buff)
        if (nw_packet_obj.IP_header.m_type == 8):
            on_wnds = True
            nw_packet_obj = assign_T_eight_seq(nw_packet_obj, my_pckt_bytes)
        elif (wnds_bl):
            on_wnds = True
            nw_packet_obj = assign_T_elven_seq(nw_packet_obj, my_pckt_bytes)
        else:
            icmp_hdr_tupl = i_initi_icmp_header(my_pckt_bytes)
            nw_packet_obj = assign_icmp_header(nw_packet_obj, icmp_hdr_tupl)
        
        # print("Type: {}, code: {}, p_src_port: {}, p_dst_port: {}".format(nw_packet_obj.ICMP_header.icmp_type,
        #             nw_packet_obj.ICMP_header.icmp_code, nw_packet_obj.ICMP_header.pair_src_port,
        #             nw_packet_obj.ICMP_header.pair_dst_port))
    else:
        # print("------Packet: {}{}------".format(pckt_number,nw_packet_obj.IP_header.is_frgment))
        if(nw_packet_obj.IP_header.is_frgment and (nw_packet_obj.IP_header.my_protocol == 17)):
            # I'm assuming the fragments and the assembled packet appear in order(/are grouped together)
            # similar to the provided cap file with fragments
            frg_pck_ids_set.add(nw_packet_obj.IP_header.my_id)
        else:
            original_bl = True
        if ((nw_packet_obj.IP_header.my_protocol == 17) and (original_bl)): # udp packet
            # udp_hdr_tpl = (src_port_buf, dst_port_buf)
            # IP_header.is_frgment = not False means that this packet is 
            # the reassembled pckt and thus has port num fields
            if (nw_packet_obj.IP_header.my_id in frg_pck_ids_set):
                # nw_packet_obj is The original udp packet
                print(pckt_number)
                udp_hdr_tpl = i_initi_udp_header(my_pckt_bytes, True)
                nw_packet_obj = assign_udp_header(nw_packet_obj, udp_hdr_tpl)
            else:
                # nw_packet_obj is a never fragmented packet
                udp_hdr_tpl = i_initi_udp_header(my_pckt_bytes, True)
                nw_packet_obj = assign_udp_header(nw_packet_obj, udp_hdr_tpl)
                
            
            # print("UDP src port: {}, dst port: {}".format(nw_packet_obj.UDP_header.src_port, 
            #         nw_packet_obj.UDP_header.dst_port))
    
    # src_ip_adrs = struct.unpack('BBBB', my_buffer2)
    # print("The src ip adrs is: {}".format(src_ip_adrs))
    # partB_li.extend(tcp_hdr_tpl)
    # src_port = struct.unpack('>H', dest_port_buf)[0]
    # print("The port src is: {}".format(src_port))
    return (nw_packet_obj, frg_pck_ids_set, on_wnds)


def part_one_Rtwo(rtwo_gbl_ob_li):

    rtwo_cap = glb_cap_fl()

    for ith_cap in rtwo_gbl_ob_li:
        udp_li = ith_cap.udp_list
        icmp_li = ith_cap.icmp_list
    ttl_set = set()
    rtwo_pckt = packet()
    per_ttl_li = []
    dctn_ttl_prb = {}
    count_prob = 0
    for u_pckt in udp_li:
        tmp_ttl = u_pckt.IP_header.my_time_leave
        if (tmp_ttl not in dctn_ttl_prb):
            dctn_ttl_prb[tmp_ttl] = 1
        else:
            dctn_ttl_prb[tmp_ttl] += 1
    for i_pckt in icmp_li:
        tmp_two_ttl = i_pckt.IP_header.my_time_leave
        if (tmp_two_ttl not in dctn_ttl_prb):
            dctn_ttl_prb[tmp_two_ttl] = 1
        else:
            dctn_ttl_prb[tmp_two_ttl] += 1
    
    for k in dctn_ttl_prb:
        num_probes = dctn_ttl_prb[k]
        print("ttl: {} has: {} probes".format(k, num_probes))
    
    print("Done!\n")

    return


def part_two_Rtwo(prt_two_cap_ob_li):


    rth_pack = packet()
    src_gbl_pac = glb_cap_fl()
    # udp_li = gbl_capFl_ob.udp_list
    # icmp_li = gbl_capFl_ob.icmp_list
    udp_id_set = set()
    temp_id = 0
    temp_src_ip = None
    frg_diction = {}
    rt_vl = 0

    for i_gbl_cap in prt_two_cap_ob_li:
        print("The IP address of the source node: {}".format(i_gbl_cap.glb_scr_ip))
        print("The IP address of the ultimate destination: {}".format(i_gbl_cap.glb_dst_ip))
        udp_li = i_gbl_cap.udp_list
        icmp_li = i_gbl_cap.icmp_list
        # print("--- {} ---".format(icmp_li[0].packet_No))
        # print("---- icmp_li len: ----")

        # for m in udp_li:
        #     print("#: {}, src port: {}, dst port: {}".format(m.packet_No, 
        #     m.UDP_header.src_port, m.UDP_header.dst_port))
        # print()
        # for j in icmp_li:
        #     print("#: {}, src port: {}, dst port: {}".format(j.packet_No, 
        #     j.ICMP_header.pair_src_port, j.ICMP_header.pair_dst_port))
        oo_count = 0
        
        # for oo in udp_li:
        #     print("a+{}a+ ".format(oo.packet_No),end='')
        #     if (oo_count > 5):
        #         oo_count = 0
        #         print()
        #     oo_count += 1
        
        tmp_src_ip = i_gbl_cap.glb_scr_ip
        tmp_ult_dst_ip = i_gbl_cap.glb_dst_ip
        print("The IP addresses of the intermediate destination nodes:")
        pckt_dst_port_set = port_num_set(udp_li)
        rt_vl = intermedi_dst_node(icmp_li, tmp_src_ip, tmp_ult_dst_ip, pckt_dst_port_set)
    return rt_vl
def src_node_print(gbl_capFl_ob_li):
    #This function is called for every traceroute with a unique src,dst ip address
    rth_pack = packet()
    src_gbl_pac = glb_cap_fl()
    # udp_li = gbl_capFl_ob.udp_list
    # icmp_li = gbl_capFl_ob.icmp_list
    udp_id_set = set()
    temp_id = 0
    temp_src_ip = None
    frg_diction = {}
    rt_vl = 0

    for i_gbl_cap in gbl_capFl_ob_li:
        print("The IP address of the source node: {}".format(i_gbl_cap.glb_scr_ip))
        print("The IP address of the ultimate destination: {}".format(i_gbl_cap.glb_dst_ip))
        udp_li = i_gbl_cap.udp_list
        icmp_li = i_gbl_cap.icmp_list
        # print("--- {} ---".format(icmp_li[0].packet_No))
        # print("---- icmp_li len: ----")

        # for m in udp_li:
        #     print("#: {}, src port: {}, dst port: {}".format(m.packet_No, 
        #     m.UDP_header.src_port, m.UDP_header.dst_port))
        # print()
        # for j in icmp_li:
        #     print("#: {}, src port: {}, dst port: {}".format(j.packet_No, 
        #     j.ICMP_header.pair_src_port, j.ICMP_header.pair_dst_port))
        oo_count = 0
        
        # for oo in udp_li:
        #     print("a+{}a+ ".format(oo.packet_No),end='')
        #     if (oo_count > 5):
        #         oo_count = 0
        #         print()
        #     oo_count += 1
        
        tmp_src_ip = i_gbl_cap.glb_scr_ip
        tmp_ult_dst_ip = i_gbl_cap.glb_dst_ip
        print("The IP addresses of the intermediate destination nodes:")
        pckt_dst_port_set = port_num_set(udp_li)
        rt_vl = intermedi_dst_node(icmp_li, tmp_src_ip, tmp_ult_dst_ip, pckt_dst_port_set)
        
        for x_packt in udp_li: # CAUTION: these x_packts include fragemented packets and not just ''whole'' packets
            temp_id = x_packt.IP_header.my_id
            # print("Packet: {}, is fragment: {}".format(x_packt.packet_No, x_packt.IP_header.is_frgment))
            if ((temp_id not in udp_id_set) and (not x_packt.IP_header.is_frgment)):
                # if (x_packt.packet_No == 26 or x_packt.packet_No == 28):
                #     print("--Packet: {}, ttl is: {}".format(x_packt.packet_No, x_packt.IP_header.my_time_leave))
                
                # udp_id_set.add(temp_id) -- that way we do not print this src node ip again
                # print("The IP address of the source node: {}".format(x_packt.IP_header.src_ip))
                # print("The IP address of the ultimate destination: {}".format(x_packt.IP_header.dst_ip))
                # get a list of the fragments with the same id as this<udp_packet>
    
                frgm_li_pckts = frgment_li_up(temp_id, udp_li)
                # sort tmp_li_pckts by fragment off set of the packets in tmp_li_pckts
                if (len(frgm_li_pckts) > 1):
                    sorted(frgm_li_pckts, key=lambda e_pck: e_pck.IP_header.frgment_off_set)
                frg_diction[temp_id] = frgm_li_pckts
                # for a_pack in tmp_li_pckts:
                #     print("fragm offset is: {}, ".format(a_pack.IP_header.frgment_off_set), end='')

        print("The values in the protocol field of IP headers:")
        rt_vl = protocl_field(udp_li, icmp_li) # print relevant protocol fields
        # k = id number, v = frgm_li_pckts see above for loop

        for k in frg_diction:        
            li_pckt_len = len(frg_diction[k])
            pck_list = frg_diction[k]
            # add {} after original datagram and including k will show id of orignal datagram
            print("The number of fragments created form the original datagram is: {}".format(li_pckt_len))
            if (li_pckt_len > 1):
                end_pckt = pck_list[li_pckt_len - 1]
                print("The offset of the last fragment is: {}".format(end_pckt.IP_header.frgment_off_set))
            else:
                print("The offset of the last fragment is: 0")
        print("")
        li_o_li_o_icmp_pckts = group_icmp_by_src_ip(icmp_li)
        real_ip = i_gbl_cap.glb_scr_ip
        rrt_vl = set_rtt_dict(udp_li, li_o_li_o_icmp_pckts, real_ip)

    return 0

def icmp_ttl_setting(udp_list, icm_list):

    tmp_src_port = 0
    tmp_dst_port = 0
    tmp_ttl = 0
    rt_list = []
    lth_pack = packet()
    
    for x_u_pck in udp_list:
        pck_ind = 0
        lth_pack.ICMP_header.pair_dst_port
        # print("--pack Num: {} {}, {}---".format(x_u_pck.packet_No, tmp_src_port,tmp_dst_port))
        if (not x_u_pck.IP_header.is_frgment):
            tmp_src_port = x_u_pck.UDP_header.src_port
            tmp_dst_port = x_u_pck.UDP_header.dst_port
            tmp_ttl = x_u_pck.IP_header.my_time_leave
            for y_i_pck in icm_list:
                # print("{} -- {}".format(y_i_pck.packet_No, y_i_pck.ICMP_header.pair_dst_port))
                if ((y_i_pck.ICMP_header.pair_src_port == tmp_src_port) 
                and (y_i_pck.ICMP_header.pair_dst_port == tmp_dst_port)):
                    pck_ind = icm_list.index(y_i_pck)
                    y_i_pck.IP_header.my_time_leave = tmp_ttl
                    print("--pack Num: {}-- {} ---".format(x_u_pck.packet_No, y_i_pck.packet_No))
                    icm_list[pck_ind] = y_i_pck        
                    break
    
    return icm_list
def intermedi_dst_node(li_of_icmp_pckts, ip_adr, ult_dst, udp_dst_ports_set):
    
    #ip_adr: src ip of the host.
    j_packt = packet()
    tmp_icmp_id = 0
    i_count = 1
    tmp_rts = 0
    interme_src_ip_set = set()
    rt_li = []
    # print("packet id: {}, dst port: {}".format(a_src_port, a_dst_port))
    for x_icmp_pck in li_of_icmp_pckts:
        tmp_scr_port = x_icmp_pck.ICMP_header.pair_src_port
        tmp_dst_port = x_icmp_pck.ICMP_header.pair_dst_port
        tmp_dst_ip = x_icmp_pck.IP_header.dst_ip
        tmp_src_ip = x_icmp_pck.IP_header.src_ip
        if (tmp_dst_port in udp_dst_ports_set):
            if ((tmp_dst_ip == ip_adr) and (tmp_dst_ip != ult_dst) and (tmp_src_ip not in interme_src_ip_set)):
                # print("{mes: <6}".format(mes=''),end='')
                # print("router {}: {}".format(i_count, x_icmp_pck.IP_header.src_ip))
                rt_li.append(x_icmp_pck.IP_header.src_ip)
                interme_src_ip_set.add(tmp_src_ip)
                i_count += 1
    print("Hi:)")
    return rt_li

def protocl_field(li_udp, li_icmp):

    if (len(li_icmp) > 0):
        print("{my_mesg: <6}".format(my_mesg=''), end='')
        print("1: ICMP")
    if (len(li_udp) > 0):
        print("{my_msg: <6}".format(my_msg=''), end='')
        print("17: UDP")
    print("")
    
    return 0
def frgment_li_up(id_pck_val, udp_list):

    # o_pck = packet()
    rt_frgment_li = []
    o_count = 0
    for mth_pck in udp_list:
        # used to be if(... and (mth_pck.IP_header.is_frgment))
        # removed the "and" part because the last packet is the reassembled packet and last fragment    
        if (mth_pck.IP_header.my_id == id_pck_val):
            # list of packets with the same id as <id_pck_val>
            rt_frgment_li.append(mth_pck)  
            # o_li.append(mth_pck)
            o_count += 1
    return rt_frgment_li

def port_num_set(li_udp):

    rtn_set = set()
    here_pckt = packet()

    for j_pckt in li_udp:
        rtn_set.add(j_pckt.UDP_header.dst_port)
        # if (j_pckt.UDP_header.dst_port == 33477):
        #     print("--------------- {} --------------------".format(j_pckt.packet_No))
    return rtn_set

def group_icmp_by_src_ip(icm_li):

    li_li_index = 0
    li_li_icmp_pckts = []
    ind_src_ip = 0
    dct_src_ip_ind = {}
    for ith_pckt in icm_li:
        tmp_src_ip = ith_pckt.IP_header.src_ip
        if (tmp_src_ip in dct_src_ip_ind):
            ind_src_ip = dct_src_ip_ind[tmp_src_ip] # get index of tmp_src_ip
            rtrv_list = li_li_icmp_pckts[ind_src_ip] # retrieve(r.t.r.v) this list
            rtrv_list.append(ith_pckt) # rtrv_list = list of icmp packets with the same src ip address
            li_li_icmp_pckts[ind_src_ip] = rtrv_list # put back the retrieved list in the same position
        else:
            insrt_li = []
            insrt_li.append(ith_pckt) # only done for a new src ip of an icmp packet
            li_li_icmp_pckts.append(insrt_li) # only done for a new src ip of an icmp packet
            dct_src_ip_ind[tmp_src_ip] = li_li_index
            li_li_index += 1
    
    # for x in li_li_icmp_pckts:
    #     y_count = 0
    #     for y in x:
    #         if (y_count < (len(x) - 1)):
    #             print("p#: {}, {} ".format(y.packet_No, y.IP_header.src_ip),end='')
    #         else:
    #             print("p#: {}, {}".format(y.packet_No, y.IP_header.src_ip))
    #         y_count += 1

    return li_li_icmp_pckts

def set_rtt_dict(udp_li, li_li_o_icmp_pckts, scr_nd_ip):

    dct_src_ip_rtt = {}
    here_pckt = packet()
    here_li = []
    prt_bll = True
    bl_break = False
    # print("----------- {}, {} -------------".format(len(li_li_o_icmp_pckts), len(udp_li)))
    for ith_li in li_li_o_icmp_pckts:
        tmp_pckt = ith_li[0]
        empty_li = []
        dct_src_ip_rtt[tmp_pckt.IP_header.src_ip] = empty_li 
    
    for uth_pckt in udp_li:
        uth_dst_port = uth_pckt.UDP_header.dst_port
        uth_src_port = uth_pckt.UDP_header.src_port
        uth_time = uth_pckt.timestamp
        bl_break = False
        for jth_li in li_li_o_icmp_pckts:
            if (bl_break):
                break
            for ith_pckt in jth_li: # ith_pckt = an icmp packet
                if ((ith_pckt.ICMP_header.pair_dst_port == uth_dst_port) and 
                        (ith_pckt.ICMP_header.pair_src_port == uth_src_port)):
                    tmp_rtt_li = dct_src_ip_rtt[ith_pckt.IP_header.src_ip]
                        # print("packet #: {}, timestamp: {}, uPacket #: {}, uTime: {}".format(
                        #     ith_pckt.packet_No, ith_pckt.timestamp, uth_pckt.packet_No, uth_time))
                    tmp_rtt_val = ith_pckt.timestamp - uth_time
                    tmp_rtt_li.append(tmp_rtt_val)
                    dct_src_ip_rtt[ith_pckt.IP_header.src_ip] = tmp_rtt_li
                    bl_break = True
                    break
    
    
    here_rt = show_avg_sd_rtt(dct_src_ip_rtt, scr_nd_ip)
    return 0

def show_avg_sd_rtt(src_ip_rtt_dct, src_node_ip):

    #src_ip_rtt_dct = {icmp_pckt_src_ip1: [rtt1, rtt2, ...], ...}
    u_count = 0
    prt_bl = True
    sdvs = 0
    acum = 0
    is_val = 'is:'
    for ith_ip in src_ip_rtt_dct: #for k, v in dict
        # if (prt_bl):
        #     tm_li = src_ip_rtt_dct[ith_ip]
        #     print("first timestap {} {}".format(ith_ip, tm_li[0]))
        #     prt_bl = False
        rtt_li = src_ip_rtt_dct[ith_ip]
        avg_rtt = sum(rtt_li) / len(rtt_li)
        sdvs = stdev(rtt_li)
        print("The avg RTT between {:>2} and {:>2} is: {:>2} ms, the s.d. is: {:>2}".format(src_node_ip, 
        ith_ip, avg_rtt, sdvs))
        u_count += 1
    # print("u_count: {}".format(u_count))
    print("")
    return 0


def main():
    atoke = argparse.ArgumentParser()
    atoke.add_argument('iFile', metavar='arg_file', type=str, nargs='+')
    in_args = atoke.parse_args()
    k_count = 0
    z_len = len (in_args.iFile)
    print(z_len)
    same_li = []
    for zth_file in in_args.iFile:
        print("{} {}".format(k_count, zth_file))
        
        in_file = zth_file
        f = open(in_file, "rb")

        # print("File name is:{}".format(in_file))

        fl_cursor = f.read(24)
        # read the fst 24 bytes to get the glb header
        my_gbl_header = fl_cursor
        # mgc_tzone_tpl = glb_header_fnct(my_gbl_header) # (mgc_number, thiszone)
        # print("magic number thiszone tuple is: {}".format(mgc_tzone_tpl))
        # read the next 16 bytes to get the fst pck header 
        # track_a = True
        # track_b = True
        # track_c = False
        diction_connections = {}
        # track_d = not track_c
        # bt_a = b'\xd2'
        li_li  = []
        my_pckt_rd = packet()
        my_gbl_cap = glb_cap_fl()
        wnd_gbl_cap = glb_cap_fl()
        count_a = 1
        is_dns = 1
        val_dict = 0
        gbl_cap_li = []
        gbl_cap_index = 0
        src_dst_dict = {}
        frg_ids_set = set()
        # udp_pck_id_set = set()
        dctn_id_srcDst_port = {}
        on_winds = False
        while (fl_cursor):
            fl_cursor = f.read(16)
            if (fl_cursor):
                my_pck_header = fl_cursor # get next(1st when loop starts) packet header bytes
                pckt_hrd_tupl = pck_header_fnct(my_pck_header) # (ts_sec, ts_usec, incl_len, orig_len)
                pckt_incl_len = pckt_hrd_tupl[2]
                # break
                # print("incl_len is: {}".format(pckt_incl_len))
                # track_a = False
                fl_cursor = f.read(pckt_incl_len)
                my_pckt_data = fl_cursor
                # (src_address, dest_address, src_port, dest_port) tuple from IP header.
                # uniq_cnt_id = mk_partB_tpl(my_pckt_data)
            if (fl_cursor):
                
                # a_pckt = mk_nw_pckt(my_pckt_data, pckt_hrd_tupl[0], count_a)
                # my_pckt_rd = a_pckt
                if (True):
                    # print(count_a)
                    # if (count_a < 25):
                    #     print("{}.{}".format(pckt_hrd_tupl[0],pckt_hrd_tupl[1]))
                    # print("---- Count A: {} ----".format(count_a))
                    
                    # print("pckt_incl_len: {}".format(pckt_incl_len))
                    
                    # print("**** time: {} ****".format(a_pckt_time))
                    
                    #a_pckt_tpl = (a_pckt_obj, frg_ids_set)
                    a_pckt_time = pckt_hrd_tupl[0] + (pckt_hrd_tupl[1]/1000000000)
                    a_pckt_tpl = mk_nw_pckt(my_pckt_data, a_pckt_time, count_a, frg_ids_set, on_winds)
                    my_pckt_rd = a_pckt_tpl[0]
                    if (a_pckt_tpl[2]):
                        on_winds = True
                    if (count_a == 24):
                        print("----------- ttl is: {} --------------".format(my_pckt_rd.IP_header.my_time_leave))
                    # print(count_a)
                    
                    # above if stmt is To avoid dns packets
                    # ----
                    #   MIGHT HAVE TO DEAL WITH: Fragmented packets having their port # always equal to 0
                    # ----

                    # print("count_a: {}, dst_port: {}".format(count_a, my_pckt_rd.UDP_header.dst_port))

                    if (my_pckt_rd.IP_header.my_protocol == 17): # udp packet
                        # print(my_pckt_rd.UDP_header.dst_port)
                        if((my_pckt_rd.UDP_header.dst_port >= 33434) and (my_pckt_rd.UDP_header.dst_port <= 33529)):
                            temp_str = my_pckt_rd.IP_header.src_ip + " " + my_pckt_rd.IP_header.dst_ip
                            tmp_port_l = []
                            tmp_port_l.append(my_pckt_rd.UDP_header.src_port)
                            tmp_port_l.append(my_pckt_rd.UDP_header.dst_port)
                            dctn_id_srcDst_port[my_pckt_rd.IP_header.my_id] = tmp_port_l 
                            # print(temp_str)
                            if (temp_str not in src_dst_dict):
                                #this if statement will help me separate packets with different src ip
                                # src_dst_dict = {glb_cap_fl obj: index in gbl_cap_li}
                                src_dst_dict[temp_str] = val_dict
                                gbl_cap_index = val_dict
                                tmp_gbl_cap = glb_cap_fl()
                                tmp_gbl_cap.set_glb_scr(my_pckt_rd.IP_header.src_ip)
                                tmp_gbl_cap.set_gbl_dst(my_pckt_rd.IP_header.dst_ip)
                                gbl_cap_li.append(tmp_gbl_cap)
                                # print("{}, val_dict {}".format(len(gbl_cap_li)), val_dict)
                                val_dict += 1
                            else:
                                gbl_cap_index = src_dst_dict[temp_str] 
                    
                    if(my_pckt_rd.IP_header.my_protocol == 1):
                        # print(my_pckt_rd.IP_header.my_id)
                        # if (gbl_cap_index < len(gbl_cap_li)):
                        if (on_winds):
                            if (my_pckt_rd.IP_header.m_type == 8):
                                wnd_gbl_cap.add_to_ty_eight_li(my_pckt_rd)
                            if (my_pckt_rd.IP_header == 11):
                                wnd_gbl_cap.add_to_ty_elvn_li(my_pckt_rd)
                        else:
                            my_gbl_cap = gbl_cap_li[gbl_cap_index]
                            my_gbl_cap.add_to_icmp_li(my_pckt_rd)
                    if (my_pckt_rd.IP_header.my_protocol == 17):
                    # if (gbl_cap_index < len(gbl_cap_li)):
                        if ((my_pckt_rd.IP_header.is_frgment) and (my_pckt_rd.IP_header.my_id in dctn_id_srcDst_port)):
                            an_id = my_pckt_rd.IP_header.my_id
                            li_port_nums = dctn_id_srcDst_port[an_id]
                            my_pckt_rd.UDP_header.src_port = li_port_nums[0]
                            my_pckt_rd.UDP_header.dst_port = li_port_nums[1]
                        if ((my_pckt_rd.UDP_header.dst_port >= 33434) and (my_pckt_rd.UDP_header.dst_port <= 33529)):
                            my_gbl_cap = gbl_cap_li[gbl_cap_index]
                            if ((my_pckt_rd.IP_header.is_frgment)):
                                my_gbl_cap.add_to_udp_frg_li(my_pckt_rd)
                            else:
                                my_gbl_cap.add_to_udp_non_frg_li(my_pckt_rd)

                    vl_src_ip = my_pckt_rd.IP_header.src_ip
                    vl_dst_ip = my_pckt_rd.IP_header.dst_ip
                    vl_hd_len = my_pckt_rd.IP_header.ip_header_len
                    vl_total_len = my_pckt_rd.IP_header.total_len
                    
                    # print()
                    
                    # print("source ip address: {}".format(vl_src_ip))
                    # print("destinatio ip address: {}".format(vl_dst_ip))
                    # print("header len: {}".format(vl_hd_len))
                    # print("total len: {}".format(vl_total_len))
                    # print("packet timestamp: {}\n".format(my_pckt_rd.timestamp))
                # else:
                #     if (count_a > 81):
                #         break
                # diction_connections = updt_cnts_diction(diction_connections, a_pckt)
            count_a += 1
            # if (count_a > 23):
            #     track_a = False
            # else:
            #     print("count_a is: {}".format(count_a))
            #     count_a += 1
            # if (not fl_cursor):
            #     track_b = False
            # print("Track_b is: {}".format(track_b))
            # print("count_a is: {}".format(count_a))
        f.close()
        #print src node ip:
        
        # print("udp list:")
        # for u_pckt in my_gbl_cap.udp_list:
        #     print("Packet number: {}, src ip: {}, dst ip: {}".format(u_pckt.packet_No, u_pckt.IP_header.src_ip,
        #             u_pckt.IP_header.dst_ip))

        # print("icmp list:")
        # for i_pckt in my_gbl_cap.icmp_list:
        #     print("Packet number: {}, src ip: {}, dst ip: {}".format(i_pckt.packet_No, i_pckt.IP_header.src_ip,
        #             i_pckt.IP_header.dst_ip))
        
        #-------------IMPORTANT: CALL THIS FUNCTION FOR EVERY TRACE ROUTE WITH A DIFFERENT (SRC, DST)-----------
        tpckt = packet()
        # print("The length is: {}".format(len(gbl_cap_li)))
        if (on_winds):
            wnd_rt = ''
        else:
            for i_gbl_cap in gbl_cap_li:   
                for frg_pk in i_gbl_cap.udp_frg_list:
                    for non_frg in i_gbl_cap.udp_non_frg_list:
                        if (frg_pk.IP_header.my_id == non_frg.IP_header.my_id):
                            frg_pk.UDP_header.src_port_set(non_frg.UDP_header.src_port)
                            frg_pk.UDP_header.dst_port_set(non_frg.UDP_header.dst_port)
                        # print("packet number: {}".format(non_frg.packet_No))
            if (z_len == 1):
                end_rt = src_node_print(gbl_cap_li)
            else:
                # end_rt = part_one_Rtwo(gbl_cap_li)
                same_li.append(part_two_Rtwo(gbl_cap_li))
        
        # en_rt = pckt_flags_fnct(b'\x20\xb8')
    
        # rcv_rt = do_printouts(diction_connections)
        # for k, v in diction_connections.items():
        #     print(k)
        #     for p in v.pckt_list:
        #         print("{}".format(p.packet_No))
        # print(li_li)
    li_count = 1
    s_one = ""
    s_two = ""
    s_three = ""
    s_four = ""
    s_five = ""
    same_set = set()
    print(len(same_li))
    for ith_li in same_li:
        if (li_count == 1):
            for j in ith_li:
                s_one += j
            if (s_one not in same_set):
                same_set.add(s_one)
                print("1 True\n{}".format(s_one))
        if (li_count == 2):
            for j in ith_li:
                s_two += j
            if (s_two not in same_set):
                same_set.add(s_two)
                print("2 True \n{}".format(s_two))
            else:
                print("2 False")
        if (li_count == 3):
            for j in ith_li:
                s_three += j
            if (s_three not in same_set):
                same_set.add(s_three)
                print("3 True\n{}".format(s_three))
            else:
                print("3 False")
        if (li_count == 4):
            for j in ith_li:
                s_four += j
            if (s_four not in same_set):
                same_set.add(s_four)
                print("4 True \n{}".format(s_four))
            else:
                print("4 False")
        if (li_count == 5):
            for j in ith_li:
                s_five += j
            if (s_five not in same_set):
                same_set.add(s_five)
                print("5 True \n{}".format(s_five))
            else:
                print("5 False")
        li_count += 1
    print("\nDID YOU CALL src_node_print FOR EVERY TRACEROUTE WITH A UNIQUE SRC,DST IP ADDRESS?")
    return 0

if __name__ == "__main__":
    main()