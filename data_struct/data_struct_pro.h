/*
███████╗██████╗  ██████╗ ███████╗                ███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
██╔════╝██╔══██╗██╔════╝ ██╔════╝                ██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
█████╗  ██║  ██║██║  ███╗█████╗                  █████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗
██╔══╝  ██║  ██║██║   ██║██╔══╝                  ██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝
███████╗██████╔╝╚██████╔╝███████╗                ███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
╚══════╝╚═════╝  ╚═════╝ ╚══════╝                ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝
Author：Edge
Web：likedge.top
Date：20201213

Data_struct_define
link_list: ll
graph


---------------------------------------------------------------
if you have the better answer on it , it is nothing, just test~
---------------------------------------------------------------

*/
#ifndef DATA_STRUCT_PRO
#define DATA_STRUCT_PRO
// #pragma once

struct lnode{
    int data;
    lnode *next_index;
    lnode(int n,lnode next)    // 构造函数
    {
        next_index=&next;
        data = n;
    }
};
class link_list{
    public:
    int len;
    lnode *NODE_link_list;
    lnode *link_list_head;
    link_list();
    link_list(int n);
    void list_ergodic(link_list link_list_ergodic);
    void insert_element_ll(int index_toinsert,link_list ll_toinsert);
};
#endif
