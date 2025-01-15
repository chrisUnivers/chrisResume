from os import sep
import struct
import argparse
from datetime import datetime

class IP_Header:
    src_ip = None #<type 'str'>
    dst_ip = None #<type 'str'>
    ip_header_len = None #<type 'int'>
    total_len = None    #<type 'int'>
    
    def __init__(self):
        self.src_ip = None
        self.dst_ip = None
        self.ip_header_len = 0
        self.total_len = 0
    
    def ip_set(self,src_ip,dst_ip):
        self.src_ip = src_ip
        self.dst_ip = dst_ip
    
    def header_len_set(self,length):
        self.ip_header_len = length
    
    def total_len_set(self, length):
        self.total_len = length    
        
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
 
class TCP_Header:
    src_port = 0
    dst_port = 0
    seq_num = 0
    ack_num = 0
    data_offset = 0
    flags = {}
    window_size =0
    checksum = 0
    ugp = 0
    
    def __init__(self):
        self.src_port = 0
        self.dst_port = 0
        self.seq_num = 0
        self.ack_num = 0
        self.data_offset = 0
        self.flags = {}
        self.window_size =0
        self.checksum = 0
        self.ugp = 0
    
    def src_port_set(self, src):
        self.src_port = src
        
    def dst_port_set(self,dst):
        self.dst_port = dst
        
    def seq_num_set(self,seq):
        self.seq_num = seq
        
    def ack_num_set(self,ack):
        self.ack_num = ack
        
    def data_offset_set(self,data_offset):
        self.data_offset = data_offset
        
    def flags_set(self,ack, rst, syn, fin):
        self.flags["ACK"] = ack
        self.flags["RST"] = rst
        self.flags["SYN"] = syn
        self.flags["FIN"] = fin
    
    def win_size_set(self,size):
        self.window_size = size
        
    def get_src_port(self,buffer):
        num1 = ((buffer[0]&240)>>4)*16*16*16
        num2 = (buffer[0]&15)*16*16
        num3 = ((buffer[1]&240)>>4)*16
        num4 = (buffer[1]&15)
        port = num1+num2+num3+num4
        self.src_port_set(port)
        #print(self.src_port)
        return None
    
    def get_dst_port(self,buffer):
        num1 = ((buffer[0]&240)>>4)*16*16*16
        num2 = (buffer[0]&15)*16*16
        num3 = ((buffer[1]&240)>>4)*16
        num4 = (buffer[1]&15)
        port = num1+num2+num3+num4
        self.dst_port_set(port)
        #print(self.dst_port)
        return None
    
    def get_seq_num(self,buffer):
        seq = struct.unpack(">I",buffer)[0]
        self.seq_num_set(seq)
        #print(seq)
        return None
    
    def get_ack_num(self,buffer):
        ack = struct.unpack('>I',buffer)[0]
        self.ack_num_set(ack)
        return None
    
    def get_flags(self,buffer):
        value = struct.unpack("B",buffer)[0]
        fin = value & 1
        syn = (value & 2)>>1
        rst = (value & 4)>>2
        ack = (value & 16)>>4
        self.flags_set(ack, rst, syn, fin)
        return None
    def get_window_size(self,buffer1): # used to be: buffer1, buffer2. But I do not understand the need for buffer2
        buffer = buffer1
        size = struct.unpack('H',buffer)[0]
        self.win_size_set(size)
        return None
        
    def get_data_offset(self,buffer):
        # value = struct.unpack("B",buffer)[0]
        # length = ((value & 240)>>4)*4
        length = buffer # buffer here is an int value
        self.data_offset_set(length)
        # print(self.data_offset)
        return None
    
    def relative_seq_num(self,orig_num):
        if(self.seq_num>=orig_num):
            relative_seq = self.seq_num - orig_num
            self.seq_num_set(relative_seq)
        #print(self.seq_num)
        
    def relative_ack_num(self,orig_num):
        if(self.ack_num>=orig_num):
            relative_ack = self.ack_num-orig_num+1
            self.ack_num_set(relative_ack)
   

class packet():
    
    #pcap_hd_info = None
    IP_header = None
    TCP_header = None
    timestamp = 0
    packet_No = 0
    RTT_value = 0
    RTT_flag = False
    buffer = None
    
    
    def __init__(self):
        self.IP_header = IP_Header()
        self.TCP_header = TCP_Header()
        #self.pcap_hd_info = pcap_ph_info()
        self.timestamp = 0
        self.packet_No =0
        self.RTT_value = 0.0
        self.RTT_flag = False
        self.buffer = None
    # used to be: timestamp_set(self,buffer1,buffer2,orig_time):
    # couldn't figure out how to use orig_time
    def timestamp_set(self,a_datetime):
        # seconds = struct.unpack('I',buffer1)[0]
        # microseconds = struct.unpack('<I',buffer2)[0]
        # self.timestamp = round(seconds+microseconds*0.000001-orig_time,6)
        self.timestamp = a_datetime
        #print(self.timestamp,self.packet_No)
    def packet_No_set(self,number):
        self.packet_No = number
        #print(self.packet_No)
        
    def get_RTT_value(self,p):
        rtt = p.timestamp-self.timestamp
        self.RTT_value = round(rtt,8)

class TCP_Connect_ion():
    syn_count = 0
    fin_count = 0
    src_adrs = None # this tcp connection source address...
    dest_adrs = None
    src_port = None
    dest_port = None
    cnt_status = None
    start_time = 0
    end_time = 0
    dura_time = 0
    num_packets_src = 0
    num_packets_dest = 0
    total_packts = 0
    num_data_bytes_src = 0 # data bytes 4 * data offset
    num_data_bytes_dest = 0 # data bytes 4 * data offset
    total_bytes = 0 # data bytes
    is_a_rst_cnt = 0 
    is_a_cmplt_cnt = 0
    my_rtt_val = 0
    rcv_win_sz = 0
    uniq_tpl = None
    pckt_list = []
    
    def __init__(self):
        self.syn_count = 0
        self.fin_count = 0
        self.dura_time = 0
        self.end_time = 0
        self.start_time = 0
        self.is_a_cmplt = False
        self.is_a_rst = False
        self.dest_adrs = 0
        self.dest_port = 0
        self.src_adrs = 0
        self.src_port = 0
        self.rtt_val = 0
        self.cnt_status = None
        self.num_data_bytes_dest = 0
        self.num_data_bytes_src = 0
        self.num_packets_dest = 0
        self.num_packets_src = 0
        self.total_packts = 0
        self.total_bytes = 0
        self.rcv_win_sz = 0
        self.uniq_tpl = None
        self.pckt_list = []
    #(ip_src_adrs, src_port, ip_dest_adrs, dest_port)
    def uniq_four_tpl_set(self, four_tpl):
        self.src_adrs = four_tpl[0]
        self.src_port = four_tpl[1]
        self.dest_adrs = four_tpl[2]
        self.dest_port = four_tpl[3]
    def syn_count_increment(self):
        self.syn_count += 1
    def fin_count_increment(self):
        self.fin_count += 1
    def start_time_set(self, s_time):
        self.start_time = s_time
    def end_time_set(self, e_time):
        self.end_time = e_time
    def dura_time_set(self):
        self.dura_time = self.end_time - self.start_time
    def cmplt_status_set(self, bl_cmplt):
        self.is_a_cmplt = bl_cmplt
    def rst_status_set(self, bl_rst):
        self.is_a_rst = bl_rst
    def rtt_val_set(self, r_value):
        self.rtt_val = r_value
    def cnt_status_set(self, s_status):
        self.cnt_status = s_status
    
    def dest_data_bts_set(self, num_bytes):
        self.num_data_bytes_dest = num_bytes
    def src_data_bts_set(self, num_byts):
        self.num_data_bytes_src = num_byts
    def dest_packets_set(self, num_packts):
        self.num_packets_dest = num_packts
    def src_packets_set(self, num_packets):
        self.num_packets_src = num_packets
    
    def total_bytes_set(self, val_add):
        self.total_bytes += val_add
    def total_packets_set(self, val_bytes):
        self.total_packts += val_bytes
    def rcv_wind_sz_set(self, wind_sz_value):
        self.rcv_win_sz = wind_sz_value
    
    def get_uniq_four_tuple(self):
        self.uniq_tpl = (self.src_adrs, self.src_port, self.dest_adrs, self.dest_port)
        return self.uniq_tpl
    def get_syn_count(self):
        return self.syn_count
    def get_fin_count(self): # S#F#
        return self.fin_count 
    def get_start_time(self):
        return self.start_time
    def get_end_time(self):
        return self.end_time
    def get_dura_time(self):
        return self.dura_time
    def get_cmplt_cnt(self):
        return self.is_a_cmplt
    def get_rst_cnt(self):
        return self.is_a_rst
    def get_rtt_val(self):
        return self.rtt_val
    def get_cnt_status(self):
        return self.cnt_status
    def get_dest_data_bts(self):
        return self.num_data_bytes_dest
    def get_src_data_bts(self):
        return self.num_data_bytes_src
    def get_dest_packets_set(self):
        return self.num_packets_dest
    def get_src_packets(self):
        return self.num_packets_src
    def get_total_bytes(self):
        return self.total_bytes
    def get_total_packets(self):
        return self.total_packts
    def get_rcv_wind_sz(self):
        return self.rcv_win_sz

# get stuff need from the global header
def glb_header_fnct(glb_hdr_bytes):
    # print("The byte ordering is:")
    # print(struct.unpack_from('cccc', glb_hdr_bytes, 0))
    byt_ordering = struct.unpack_from('I', glb_hdr_bytes, 0)[0]
    lc_time_zones = struct.unpack_from('I', glb_hdr_bytes, 8)[0] 
    # print("magic number value is: {}".format(byt_ordering))
    # print("thiszone value is: {}".format(lc_time_zones))
    glb_hdr_tpl = (byt_ordering, lc_time_zones) # glb header tuple
    return glb_hdr_tpl

# get and return need info form the packet header
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

# make a set out of: src_ip, src_port, dest_ip, dest_port)
def mk_set(a_tpl):
    my_li = [[a_tpl[0], a_tpl[1]], [a_tpl[2], a_tpl[3]]]
    rt_set = set(tuple(k) for k in my_li)
    return rt_set
# check if this entry(in this case the uniq_id_4tuple) is an old key in give dictionary
# nw_entry = {(src_address, src_port), (dest_address, dest_port)} -> is a set
# tcp_cnct_diction = {(src_ip, src_port, dest_ip, dest_port): tcp_obj}
def check_if_old_key(tcp_cnct_diction, nw_entry):
    # temp_tcp_cnct = TCP_Connect_ion()
    rslt = False # result
    cmp_set = set()
    for k, v in tcp_cnct_diction.items():
        cmp_set = mk_set(k)
        if (nw_entry == cmp_set):
            rslt = True
            return rslt
    return rslt

# update a tcp connection obj's S#F# counts and reset status
def upt_state_counts(a_tcp_cntn, a_pckt):
    rt_tcp_cnct = a_tcp_cntn
    if ((a_pckt.TCP_header.flags['SYN'] == 1) or ((a_pckt.TCP_header.flags['SYN'] == 1) 
            and (a_pckt.TCP_header.flags['ACK'] == 1))):
        rt_tcp_cnct.syn_count_increment()
    if (a_pckt.TCP_header.flags['FIN'] == 1):
        rt_tcp_cnct.fin_count_increment()
    if (a_pckt.TCP_header.flags['RST'] == 1):
        val_a = True
        rt_tcp_cnct.rst_status_set(val_a)
    return rt_tcp_cnct

# initialize a tcp connection. The initial "k" in tcp and packet doesn't mean anything
def i_initi_tcp_connection(k_tcp_cnct,k_packet):
    # NOTE: the tcp connection k_tcp_cnct already has its src ip, src  port, dest ip and dest port set
    # NOTE: k_packet is the first packet in k_tcp_cnct
    
    #intialize k_tcp_cnct's flags
    rt_tcp_cnct = k_tcp_cnct
    rt_tcp_cnct.pckt_list.append(k_packet)
    rt_tcp_cnct = upt_state_counts(rt_tcp_cnct, k_packet)
    rt_tcp_cnct.start_time_set(k_packet.timestamp)
    k_dt_bytes = get_data_size(k_packet)
    rt_tcp_cnct.num_data_bytes_src += k_dt_bytes
    rt_tcp_cnct.num_packets_src += 1
    rt_tcp_cnct.total_bytes_set(k_dt_bytes)
    rt_tcp_cnct.total_packets_set(1)
    return rt_tcp_cnct

# return the data size of this packet
def get_data_size(some_pckt):
    pckt_data_size = some_pckt.IP_header.total_len - some_pckt.IP_header.ip_header_len 
    pckt_data_size += -1 * some_pckt.TCP_header.data_offset
    return pckt_data_size
 
# update this tcp connection
def tcp_cntn_update(a_tcp_cnct, a_pckt):
    # NOTE: All tcp connection endtimes will be assigned after the file has been read
    # same with duration
    rt_tcp_cnct = a_tcp_cnct
    bol_value = True
    pckt_data_size = 0
    # append a_pckt to to a_tcp_cntn
    rt_tcp_cnct.pckt_list.append(a_pckt)
    #update the state of this tcp connection
    rt_tcp_cnct = upt_state_counts(rt_tcp_cnct, a_pckt)
    if ((rt_tcp_cnct.syn_count > 1) and (rt_tcp_cnct.fin_count > 1)):
        rt_tcp_cnct.cmplt_status_set(bol_value)
    # if my_tcp_connection src address is not None. Safe guard to avoid errors
    if (not rt_tcp_cnct.src_adrs):
        #update size of data from src 
        if (a_pckt.IP_header.src_ip == rt_tcp_cnct.src_adrs):
            num_dt_bytes = get_data_size(a_pckt)
            rt_tcp_cnct.num_data_bytes_src += num_dt_bytes
            rt_tcp_cnct.num_packets_src += 1
            rt_tcp_cnct.total_bytes_set(num_dt_bytes)
            rt_tcp_cnct.total_packets_set(1)
    # if my_tcp_connection destination address is not NONE. Safe guard to avoid errors
    if (not rt_tcp_cnct.dest_adrs):
        if (a_pckt.IP_header.dst_ip == rt_tcp_cnct.dest_adrs):
            num_dst_dt_bytes = get_data_size(a_pckt)
            rt_tcp_cnct.num_data_bytes_dest += num_dst_dt_bytes
            rt_tcp_cnct.num_packets_dest += 1
            rt_tcp_cnct.total_bytes_set(num_dst_dt_bytes)
            rt_tcp_cnct.total_packets_set(1)
    
    # update rcv window sz
    my_win_size = a_pckt.TCP_header.window_size
    rt_tcp_cnct.rcv_wind_sz_set(my_win_size)
    
    return rt_tcp_cnct
# update tcp_connection_dictionary: {uniq_id_tupe:tcp_cnt_obj}
def updt_cnts_diction(my_cncts_diction, s_pckt):
    real_tcp_cnct = TCP_Connect_ion()
    # src_tpl = (s_pckt.IP_header.src_ip, s_pckt.TCP_header.src_port)
    # dst_tpl = (s_pckt.IP_header.dst_ip, s_pckt.TCP_header.dst_port)
    li = [[s_pckt.IP_header.src_ip, s_pckt.TCP_header.src_port],
            [s_pckt.IP_header.dst_ip, s_pckt.TCP_header.dst_port]] # used this instead of mk_set for my own sake
    # for checking a tcp uniqueness
    # tcp_uniqueness_tpl = (src_tpl, dst_tpl)
    uniqueness_set = set(tuple(i) for i in li) 
    an_id_tpl = (s_pckt.IP_header.src_ip, s_pckt.TCP_header.src_port,
                    s_pckt.IP_header.dst_ip, s_pckt.TCP_header.dst_port
    )
    other_id_tpl = (s_pckt.IP_header.dst_ip, s_pckt.TCP_header.dst_port,
                s_pckt.IP_header.src_ip, s_pckt.TCP_header.src_port
    )
    bol_an_id = False
    bol_other_id = False
    ith_connect = len(my_cncts_diction)
    temp_tcp_cntion = TCP_Connect_ion()
    if (ith_connect == 0):
        #call i_initi_tcp_connectio() below this comment
        real_tcp_cnct = i_initi_tcp_connection(temp_tcp_cntion, s_pckt)
        my_cncts_diction[an_id_tpl] = real_tcp_cnct
        # print("FIRST CONNECTION\n{}\n".format(my_cncts_diction))
        return my_cncts_diction
    else:
        is_old_key = check_if_old_key(my_cncts_diction, uniqueness_set)
        if (is_old_key):
            #get the old tcp connection
            try:
                a_tcp_connection = my_cncts_diction[an_id_tpl]
                bol_an_id = True
            except Exception as e:
                a_tcp_connection = my_cncts_diction[other_id_tpl]
                bol_other_id = True
            #update and return the old_tcp connection
            real_tcp_cnct = tcp_cntn_update(a_tcp_connection, s_pckt) 
            # replace the old_tcp connection with the updated tcp connection in the dictionary
            if (bol_an_id):    
                my_cncts_diction[an_id_tpl] = real_tcp_cnct
            if (bol_other_id):
                my_cncts_diction[other_id_tpl] = real_tcp_cnct
            # print("OLD CONNECTION")
            return my_cncts_diction
        else:
            # new_tcp_obj = initialize_tcp_obj()
            # my_cncts_diction[an_id_tuple] = temp_tcp_cntion
            real_tcp_cnct = i_initi_tcp_connection(temp_tcp_cntion, s_pckt)
            my_cncts_diction[an_id_tpl] = real_tcp_cnct
            # print("NEW CONNECTION\n{}\n".format(list(my_cncts_diction)))
            return my_cncts_diction

# Total length found in the IP header
def get_my_total_len(abyte_value):
    ihl_hex = abyte_value.hex()
    my_ihl = int(bin(int(ihl_hex, base = 16)), 2) & 0x0f
    return 4 * my_ihl
def get_my_IHL(abyte_value):
    ihl_hex = abyte_value.hex()
    my_ihl = int(bin(int(ihl_hex, base = 16)), 2) & 0x0f
    return 4 * my_ihl
def get_my_dataoffset(abyte_val):
    # print("MY DATA OFFSET IS: {}".format(abyte_val))
    data_offset_hex = abyte_val.hex()
    my_dt_offset = int(bin(int(data_offset_hex, base = 16)), 2) >> 4
    # print("MY DATA OFFSET IS: {}".format(my_dt_offset))
    return 4 * my_dt_offset

# initialize IP Header for a packet
def i_initi_ip_header(a_pck_bytes):
    a_tpl_to_rt = None
    ihl_value = get_my_IHL(a_pck_bytes[14:15]) # [14:15] the ip version & header length bytes
    total_len = a_pck_bytes[16:18] # [17:19] the 16 bit total length in IP headervalue
    # print("MY VALUE: {}".format(total_len))
    src_buffer1 = a_pck_bytes[26:30] # scr_ip_adrs
    dest_buffer2 = a_pck_bytes[30:34] # scr_ip_dest
    a_tpl_to_rt = (src_buffer1, dest_buffer2, ihl_value, total_len)
    return a_tpl_to_rt

# initialize TCP Header for a packet
def i_initi_tcp_header(a_pckt_bytes):
    a_tcp_tpl_to_rt = None
    src_port_buf = a_pckt_bytes[34:36] # src_port_buf in b'\x' form
    dest_port_buf = a_pckt_bytes[36:38] # dest_port_buf in b'\x' form
    r_seq_num_buf = a_pckt_bytes[38:42]
    r_ack_num_buf = a_pckt_bytes[42:46]
    data_off_set = get_my_dataoffset(a_pckt_bytes[46:47]) # return is an int value
    pckt_flags = a_pckt_bytes[47:48]
    pckt_win_sz = a_pckt_bytes[48:50] 
    pckt_check_sum = a_pckt_bytes[50:52]
    pck_urg_ptr = a_pckt_bytes[52:54]
    # temp_win_sz = struct.unpack('>H', data_off_set)
    # print("The packets windows size is: {}".format(data_off_set))
    a_tcp_tpl_to_rt = (src_port_buf, dest_port_buf, r_seq_num_buf, r_ack_num_buf
                        , data_off_set, pckt_flags, pckt_win_sz, pckt_check_sum, pck_urg_ptr)
    return a_tcp_tpl_to_rt
# assign corresponding ip header values to this packet
def assign_ip_header(a_pck_obj, ip_hdr_tpl):
    # a_tpl_to_rt = (src_buffer1, dest_buffer2, ihl_value, total_len)
    my_pckt_obj = packet()
    a_pck_obj.IP_header.get_IP(ip_hdr_tpl[0], ip_hdr_tpl[1])
    a_pck_obj.IP_header.get_header_len(ip_hdr_tpl[2])
    a_pck_obj.IP_header.get_total_len(ip_hdr_tpl[3])
    return a_pck_obj
# assign corresponding tcp value to this packet
def assign_tpc_header(b_pck_obj, tcp_hdr_tpl):
    #(src_port_buf, dest_port_buf, r_seq_num_buf, r_ack_num_buf
    #data_off_set, pckt_flags, pckt_win_sz, pckt_check_sum, pck_urg_ptr)
    b_pck_obj.TCP_header.get_src_port(tcp_hdr_tpl[0])
    b_pck_obj.TCP_header.get_dst_port(tcp_hdr_tpl[1])
    b_pck_obj.TCP_header.get_seq_num(tcp_hdr_tpl[2])
    b_pck_obj.TCP_header.get_ack_num(tcp_hdr_tpl[3])
    b_pck_obj.TCP_header.get_data_offset(tcp_hdr_tpl[4])
    b_pck_obj.TCP_header.get_flags(tcp_hdr_tpl[5])
    b_pck_obj.TCP_header.get_window_size(tcp_hdr_tpl[6])
    return b_pck_obj
# create packet object for some packet
def mk_new_pckt(my_pckt_bytes, my_ts_sec, pckt_number):
    # part B tuple: (Source Address, Destination addres, Source Port, Destinatin Port)
    # src_ip_adrs = struct.unpack_from('BBBB', my_data_bytes, 26)
    # scr_ip_adrs starts at byte 26 when given the data_bytes of a packet
    partB_li = []
    nw_packet_obj = packet() 
    ip_hdr_tpl = i_initi_ip_header(my_pckt_bytes)
    tcp_hdr_tpl = i_initi_tcp_header(my_pckt_bytes)
    nw_packet_obj = assign_ip_header(nw_packet_obj, ip_hdr_tpl)
    nw_packet_obj = assign_tpc_header(nw_packet_obj, tcp_hdr_tpl)
    nw_packet_obj.packet_No_set(pckt_number)
    pckt_time = datetime.fromtimestamp(my_ts_sec).strftime("%B %d, %Y %I:%M:%S")
    nw_packet_obj.timestamp_set(pckt_time)
    # src_ip_adrs = struct.unpack('BBBB', my_buffer2)
    # print("The src ip adrs is: {}".format(src_ip_adrs))
    partB_li.extend(tcp_hdr_tpl)
    # src_port = struct.unpack('>H', dest_port_buf)[0]
    # print("The port src is: {}".format(src_port))
    return nw_packet_obj

# print the B output
def do_B_printout(my_tcp, tcp_num, fnl_tcp, a_li):
    rn_tcp = TCP_Connect_ion()
    # rn_pck = packet()
    temp_cmplt_tcp_li = a_li
    
    print("Connection {}:".format(tcp_num))
    print("Source Address: {}".format(my_tcp.src_adrs))
    print("Destination address: {}".format(my_tcp.dest_adrs))
    print("Source Port: {}".format(my_tcp.src_port))
    print("Destination Port: {}:".format(my_tcp.dest_port))
    print("Status: S{}F{}".format(my_tcp.syn_count, my_tcp.fin_count))
    print()
    if (my_tcp.is_a_cmplt):
        temp_cmplt_tcp_li.append(my_tcp)
        print("Start time: {}".format(my_tcp.start_time))
        print("End Time: {}".format("No End Time Yet"))
        print("Duration: {}".format("No Duration Yet"))
        print("Number of packets sent from Source to Destination: {}".format(my_tcp.num_packets_src))
        print("Number of packets sent from Destination to Source: {}".format(my_tcp.num_packets_dest))
        print("Total number of packets: {}".format(my_tcp.total_packts))
        print("Number of data bytes sent from Source to Destination: {}".format(my_tcp.num_data_bytes_src))
        print("Numebr of data bytes sent from Destination to Source: {}".format(my_tcp.num_data_bytes_dest))
        print("Total number of data bytes: {}".format(my_tcp.total_bytes))
        print("END")
    if (fnl_tcp):
        rcv_val = prt_hrzt_lines("-", 20)
    else:
        rcv_val = prt_hrzt_lines("+", 20)
    return temp_cmplt_tcp_li

def do_C_printout(cmplt_count, rst_count, open_count):
    print("Total number of complete TCP connections: {}".format(cmplt_count))
    print("Total number of reset TCP connections: {}".format(rst_count))
    print("Number of TCP connections that were still open", sep='')
    print("when the trace capture ended: {}".format(open_count))
    return 0

def do_D_printout(a_tcp_li):
    # [a, b, c,...]
    all_duration_li = []
    all_packets_li = []
    dura_min = 0
    dura_max = 0
    dura_mean = 0
    my_tcp = TCP_Connect_ion()
    for i in a_tcp_li:
        all_duration_li.append(my_tcp.dura_time)
        all_packets_li.append(my_tcp.total_packts)
    dura_min = min(all_duration_li)
    dura_max = max(all_duration_li)
    dura_mean = sum(all_duration_li) / len(all_duration_li)
    print("Minimum time duration: {}".format(dura_min))
    print("Mean time duration: {}".format(dura_mean))
    print("Maximum time duration: {}".format(dura_max))
    print("No info for RTT value")

    return 0

def prt_hrzt_lines(a_symbl, lne):
    for i in range(lne):
        print(a_symbl,end='')
    print()
    return 0
# print the output
def do_printouts(my_dictiona):
    my_pack = packet()
    # my_tcp_cnct = TCP_Connect_ion()
    # my_tcp_cnct.pckt_list.append(my_pack)
    my_dictio_len = len(my_dictiona)
    count_aa = 1
    count_is_cmplt = 0
    count_is_rst = 0
    count_was_open = 0
    cmplt_tcp_li = []
    partA = 1
    rcv_rts = 0
    print("A) Total number of connections: {}".format(my_dictio_len))
    rcv_rts = prt_hrzt_lines("-", 20)
    for sctn in range(4):
        count_aa = 1
        # v is a tcp connection
        for k, v in my_dictiona.items():
            # print(k)
            # print(sctn)
            if (sctn == 1):
                if (v.is_a_cmplt):
                    count_is_cmplt += 1
                else:
                    count_was_open += 1
                if (v.is_a_rst):
                    count_is_rst += 1

                if (count_aa == 1):
                    print("B) Connections' details:")
                    print(v.src_adrs)
                    cmplt_tcp_li = do_B_printout(v, count_aa, False, cmplt_tcp_li)
                elif (count_aa > 1 and (count_aa < my_dictio_len)):
                    cmplt_tcp_li = do_B_printout(v, count_aa, False, cmplt_tcp_li)
                elif (count_aa == my_dictio_len):
                    cmplt_tcp_li = do_B_printout(v, count_aa, True, cmplt_tcp_li)
                else:
                    pass
                count_aa += 1
            if (sctn == 2):
                rcv_rts = do_C_printout(count_is_cmplt, count_is_rst, count_was_open)
                break
            # if (sctn == 3):
            #     if (count_aa == 1):
            #         print("D) Complete TCP Connections:")
            #         rcv_rts = do_D_printout(cmplt_tcp_li)
        # for p in my_tcp_cnct:
        #     print 

    return 0

def main():
    atoke = argparse.ArgumentParser()
    atoke.add_argument('iFile', metavar='arg_file', type=str, help='enter a .cap file')
    in_args = atoke.parse_args()

    in_file = in_args.iFile
    
    f = open(in_file, "rb")

    # print("File name is:{}".format(in_file))

    pck1 = packet()
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
    
    count_a = 1
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
            li_li.append(count_a)
            a_pckt = mk_new_pckt(my_pckt_data, pckt_hrd_tupl[0], count_a)
            diction_connections = updt_cnts_diction(diction_connections, a_pckt)
        
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
    rc_rt = None
    rcv_rt = do_printouts(diction_connections)
    # for k, v in diction_connections.items():
    #     print(k)
    #     for p in v.pckt_list:
    #         print("{}".format(p.packet_No))
    # print(li_li)
    return 0

if __name__ == "__main__":
    main()
    