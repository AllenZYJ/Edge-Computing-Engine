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
*/
#include "data_struct_pro.h"
#define random(x) (rand()%x)
link_list::link_list()
{
    // lnode node_1(1,2);
    // lnode node_2(9,3);
    // cout<<node_1.data<<endl;
    // cout<<node_1.next_index<<endl;
    cout<<"test"<<endl;
}
// link_list::link_list(int *head,int n )
// {
//     this->link_list_head =(lnode*)malloc(sizeof(lnode));
//     for(int index_link_node=0;index_link_node<n;index_link_node++)
//     {
//         _NODE_=(lnode*)malloc(sizeof(lnode));
//         // nodeindoor->data=index_link_node;
//         // cout<<"index_link_node:"<<nodeindoor->data<<endl;
//         this->_NODE_=(lnode*)malloc(sizeof(lnode));
//         this->link_list_head->next_index=&this->_NODE_;
//         this->_NODE_->data=index_link_node;
//         cout<<this->_NODE_->data<<endl;
//     }
//     cout<<"test2"<<endl;
// }


link_list::link_list(int n)
{
    this->len=n;
    this->link_list_head =(lnode*)malloc(sizeof(lnode));
    lnode *node1=(lnode*)malloc(sizeof(lnode));
    lnode *p= this->link_list_head;
    // this->link_list_head =new lnode(1,0);
    for(int index_link_node=0;index_link_node<n;index_link_node++)
    {
        lnode *node1=(lnode*)malloc(sizeof(lnode));
        p->next_index=node1;
        p->data=index_link_node;
        cout<<p->data<<endl;
        cout<<p->next_index<<endl;
        p=p->next_index;
        // cout<<"Node:"<<this->link_list_head->next_index<<endl;

    }
}
void list_ergodic(link_list link_list_ergodic)
{
    lnode *ldir_head=link_list_ergodic.link_list_head;
    for(int i = 0;i<link_list_ergodic.len;i++){
        
        cout<<"list["<<i<<"]:"<<ldir_head->data<<endl;
        ldir_head=ldir_head->next_index;
    }
}
void insert_element_ll(int index_toinsert,link_list ll_toinsert,int to_insertele)
{
    lnode *insert_p=ll_toinsert.link_list_head;
    for(int index_move = 0;index_move<ll_toinsert.len;index_move++){

        if(index_move==index_toinsert-1)
        {
            lnode *node1=(lnode*)malloc(sizeof(lnode));
            lnode *old_next_index=insert_p->next_index;
            insert_p->next_index=node1;
            insert_p->next_index->data=to_insertele;
            node1->next_index=old_next_index;

        }
        insert_p=insert_p->next_index;
    }
}
// void dele_element(int index_todele,link_list ll_toinsert,int to_insertele)
// {

// }

