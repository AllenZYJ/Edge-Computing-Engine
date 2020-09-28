/*
███████╗██████╗  ██████╗ ███████╗                ███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗
██╔════╝██╔══██╗██╔════╝ ██╔════╝                ██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝
█████╗  ██║  ██║██║  ███╗█████╗                  █████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗
██╔══╝  ██║  ██║██║   ██║██╔══╝                  ██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝
███████╗██████╔╝╚██████╔╝███████╗                ███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗
╚══════╝╚═════╝  ╚═════╝ ╚══════╝                ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝
Author：Edge
Web：likedge.top
Date：20200925
*/
#include<iostream>
#include<string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include<stdlib.h>
#define random(x) (rand()%x)
using namespace std;
int welcome(){
    fstream input_stream;
	const char* candidates[] = {"./welcome/2.txt","./welcome/3.txt","./welcome/big.txt"};
	srand((unsigned int)time(NULL));
    input_stream.open(candidates[rand()%3],ios::in);
    char input[1000000]; 
	int cnt=0;
    if(input_stream.is_open()){ 
        while(!input_stream.eof()){ 
            input_stream.getline(input,999);  
            cout<<input<<endl;  
        }  
    }
    input_stream.close();
	return 0;
}
