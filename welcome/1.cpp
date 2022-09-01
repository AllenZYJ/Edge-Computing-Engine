#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main()
{
    fstream input_stream;
    input_stream.open(".txt", ios::in);

    char input[1000000];
    int cnt = 0;
    if (input_stream.is_open())
    {
        // cout<<"yes\n";
        while (!input_stream.eof())
        {
            input_stream.getline(input, 999);
            cout << input << endl;
        }
    }
    input_stream.close();
}