import socket
import argparse
import ssl
import re
import sys
from time import sleep


def support_h2(ahostname):
    acontext = ssl.create_default_context()

def get_cookies(srv_rsp_cookies):
    # srv_cook_r = re.compile(r'\bSet-Cookie: \b.*\n?')
    # srv_cookies = srv_cook_r.findall(srv_rsp_text)
    # print(len(srv_rsp_cookies))
    # jfile = open("xyzcookies.txt", "a")
    if len(srv_rsp_cookies) > 0:
        for co in srv_rsp_cookies:
            # jfile.write("%s"%co)
            print("%s"%co, end="")
        # jfile.close()
    print("3. Password-protected: no")

    

def parse_rsp(rsp_text):
    rsp_cook_r = re.compile(r'\bSet-Cookie: \b.*\n?')
    new_move_r = re.compile(r'\b(Location: )\b((https*:[/]{2})(.*\n?))')
    rsp_code_r = re.compile(r'\b((HTTP/)(\d.\d))\b (\d{3})(.*\n?)')
    new_move = new_move_r.search(rsp_text)
    rsp_code = rsp_code_r.search(rsp_text)
    rsp_cookies = rsp_cook_r.findall(rsp_text)
    rsp_status_code = int(rsp_code.group(4))
    rt_li = []
    # print("Parsing a file code: %s\n"%rsp_status_code)
    # hfile = open("xyefcookies.txt", "a")
    # hfile.write(rsp_text)
    # rsp_status_code = 302
    if rsp_status_code == 200:
        rt_li.append(rsp_status_code)
        if len(rsp_cookies) > 0:
            # for c in rsp_cookies:
                # hfile.write("%s"%c)
                # rt_li.append(c)
            rt_li.append(rsp_cookies)
        # hfile.close()
        # rt_li = get_cookies(rsp_text)
        return rt_li
    elif rsp_status_code == 400:
        #hfile.write("%s"%new_move.group())
        rt_li.append(rsp_status_code)
        return rt_li
    elif rsp_status_code == 404:
        #hfile.write("%s"%new_move.group())
        rt_li.append(rsp_status_code)
        return rt_li
    elif rsp_status_code == 401:
        rt_li.append(rsp_status_code)
        return rt_li
    elif rsp_status_code == 301:
        rt_li.append(rsp_status_code)
        rt_li.append(new_move.group(4))
        if len(rsp_cookies) > 0:
            rt_li.append(rsp_cookies)
        return rt_li #return status to main
    elif rsp_status_code == 302:
        rt_li.append(rsp_status_code)
        rt_li.append(new_move.group(4))
        if len(rsp_cookies) > 0:
            rt_li.append(rsp_cookies)
        return rt_li
    else:
        x = 0
        x = 2
    # hfile.close()

def fnc_connect(conn, user_url, acontex):
    conn.connect((user_url, 443))

def has_hp2(conn, user_url, acontex):
    acontex.set_alpn_protocols(['h2'])
    conn.connect((user_url, 443))
    agree_protocol = conn.selected_alpn_protocol()
    # print(agree_protocol)
    return agree_protocol

def handle_response(aconn):
    decode_form = 'utf-8'
    newdat = ''
    newdat = aconn.recv(10000).decode(decode_form)
    # print("Out of handle_respone()")
    return newdat
    
def mysend_rqst(myConnect,myMessage):
    encode_form = 'utf-8'
    myConnect.send(myMessage.encode(encode_form))

def iteronstr(astring):
    achar = ""
    h_str = "" #the host part 
    f_str = "" #the file part if any
    ret_li = []
    cntl = True
    # print("The string: %s has arrived"%astring)
    for c in astring:
        achar = c
        if achar == "/":
            cntl = False
        if achar != "/" and cntl:
            h_str = h_str + achar
        elif achar != "/":
            f_str = f_str + achar
        else:
            x = 6 + 2
    ret_li.append(h_str)
    if not cntl:
        ret_li.append(f_str)
    print("Website: %s"%ret_li[0]) #print the website:
    return ret_li

    
def main():
    atoke = argparse.ArgumentParser()
    atoke.add_argument('iURL', metavar='arg_url', type=str, help='enter a url')
    in_args = atoke.parse_args()
    encode_form = 'utf-8'
    rsp_string = ""
    onwards = True
    which_protocol = ""

    in_url = in_args.iURL
    host_url = ""
    my_context = ssl.create_default_context()
    myUrl_li = iteronstr(in_url)
    host_url = myUrl_li[0]
    user_conn = my_context.wrap_socket(socket.socket(socket.AF_INET),server_hostname=host_url)

    if len(myUrl_li) > 1:
        url_file = myUrl_li[1]
        which_protocol = has_hp2(user_conn, host_url, my_context)
        rqst_msg = "GET /{} HTTP/1.1\r\nHost: {}\r\n\r\n".format(url_file, host_url)
        user_conn.close()
    else:
        which_protocol = has_hp2(user_conn, host_url, my_context)
        rqst_msg = "GET / HTTP/1.1\r\nHost: {}\r\n\r\n".format(host_url)
        user_conn.close()
    
    # sleep(1)
    if which_protocol == 'h2':
        print("1. Supports htt2: yes")
    else:
        print("1. Supports http2: no")
    my_context = ssl.create_default_context()
    user_conn = my_context.wrap_socket(socket.socket(socket.AF_INET),server_hostname=host_url)
    which_protocol = fnc_connect(user_conn, host_url, my_context)
    # exit()
    icount = 0
    while onwards:
        mysend_rqst(user_conn, rqst_msg)
        rsp_string = handle_response(user_conn)
        rsp_li = parse_rsp(rsp_string)
        rsp_stat_code = rsp_li[0]
        if (rsp_stat_code == 301 or rsp_stat_code == 302):
            new_url = rsp_li[1]
            new_url_li = re.split('/|\r|\n', new_url)
            rqst_msg = "GET /{} HTTP/1.1\r\nHost: {}\r\n\r\n".format(new_url_li[1], in_url)
            if len(rsp_li) > 2:
                print("2. List of Cookies:")
                get_cookies(rsp_li[2]) #rsp_li should be a list
        elif rsp_stat_code == 200:
            onwards = False
            print("2. List of Cookies:")
            if len(rsp_li) > 1:
                get_cookies(rsp_li[1])
            else:
                print("3. Password-protected: no")
        elif rsp_stat_code == 401:
            onwards = False
            print("2. List of Cookies: None") 
            print("3. Password-protected: yes")
        else:
            onwards = False
        if icount > 500:
            onwards = False
            print("On false")
        icount += 1
    
    print("Done!")

if __name__ == "__main__":
    main()