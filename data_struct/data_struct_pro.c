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
    cout<<"test"<<endl;
}


//init function on link_list
//n:len
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
//travel around the link_list
void list_ergodic(link_list link_list_ergodic)
{
    cout<<"len:"<<link_list_ergodic.len<<endl;
    lnode *ldir_head=link_list_ergodic.link_list_head;
    for(int i = 0;i<=link_list_ergodic.len;i++){
        
        cout<<"list["<<i<<"]:"<<ldir_head->data<<endl;
        ldir_head=ldir_head->next_index;
    }
}
//insert element for what u input,in this index
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
//delete the element from where you index
void dele_element_ll(int index_todele,link_list ll_todele)
{
    lnode *dele_p=ll_todele.link_list_head;
    for(int index_move=0;index_move<ll_todele.len;index_move++)
    {
        if(index_move==index_todele-1)
        {
            lnode *node1=(lnode*)malloc(sizeof(lnode));
            lnode *old_index_to_dele=dele_p->next_index;
            dele_p->next_index=old_index_to_dele->next_index;
        }
        dele_p=dele_p->next_index;
    }

}
void insert_end_ll(int to_insert_end,link_list ll_toinsert_end)
{

    lnode *end_p=ll_toinsert_end.link_list_head;
    for(int index_move_toend=0;index_move_toend<=ll_toinsert_end.len;index_move_toend++)
    {
        cout<<index_move_toend<<endl;
        if(index_move_toend==ll_toinsert_end.len)
        {
            lnode *node_to_insert_end=(lnode*)malloc(sizeof(lnode));
            node_to_insert_end->data=to_insert_end;
            cout<<index_move_toend<<endl;
            end_p->next_index=node_to_insert_end;
        }
        end_p=end_p->next_index;
    }
    ll_toinsert_end.len+=1;
    cout<<ll_toinsert_end.len<<endl;
    //insert element to end of link_list

}

