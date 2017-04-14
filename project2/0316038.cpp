//
//  main.cpp
//  0316038
//
//  Created by 石瑾旋 on 2016/11/27.
//  Copyright © 2016年 石瑾旋. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

struct info{
    int print;
    string tags;
    string select;
    string parameters;

} data[50];

//functions
void process(char buffer[]);
void print();
void calculate(char buffer[],int i);
void func(char buffer[],int start);
int findtag(string target,int length);
void error();
void reset();

//global variables
string openfilename,outputfilename;
string dir1,dir2,dir3,target,line21;
fstream ofile;
char buffer[100];
int start=0,tail=0,init=0,pos1=0,pos2=0,line=1,counter=0,num=0,sign=0,flag;

int main()
{
    for(int i=1;i<=3;i++){
        //cout << "HI" << endl;
        stringstream ss;
        ss << i;
        fstream ifile;
        openfilename= "XSL-" + ss.str() + ".txt";
        ifile.open(openfilename.c_str(), ios::in);
        outputfilename= "Result" + ss.str() + ".txt";
        ofile.open(outputfilename.c_str(), ios::out);
        if(!ifile)
            cout << "Cannot open file!" << endl;
        else{
            do{
                ifile.getline(buffer,sizeof(buffer));
                process(buffer);
                line++;
            }while(!ifile.eof());
            ifile.close();
            print();
            ofile.close();
        }
        reset();

    }

}

void process(char buffer[])
{
    string str(buffer);
    int i=0;
    while(buffer[i]=='\t'){
        i++;
    }
    if(buffer[i]=='<'){
        if(str.substr(i+1,12)=="xsl:template"){
            data[line].print=1;
            int length=12;
            data[line].tags="xsl:template";
            if(findtag(data[line].tags,length)==1){
                calculate(buffer,i);
                if(str.substr(init,5)=="match"){
                    data[line].select=str.substr(start,tail-start);
                    dir1=data[line].select;    //"/shop"
                }
                data[line].parameters="null";
            }
            else{
                error();
                exit(1);
            }
        }
        else if(str.substr(i+1,12)=="xsl:for-each"){
            data[line].print=1;
            data[line].tags="xsl:for-each";
            calculate(buffer,i);
            if(str.substr(init,6)=="select"){
                if(buffer[tail-1]==']'){
                    func(buffer,start);
                    dir2=str.substr(start,pos1-start-1);
                    data[line].select=dir1+'/'+dir2;
                    data[line].parameters=str.substr(pos1,pos2-pos1);
                }
                else{
                    dir2=str.substr(start,tail-start);   //"part1/book"
                    data[line].select=dir1+'/'+dir2;
                    data[line].parameters="null";
                }
            }
        }
        else if(str.substr(i+1,8)=="xsl:sort"){
            data[line].print=1;
            data[line].tags="xsl:sort";
            calculate(buffer,i);
            data[line].select=dir1+'/'+dir2;
            if(str.substr(init,6)=="select"){
                data[line].parameters=str.substr(start,tail-start);
            }
        }
        else if(str.substr(i+1,12)=="xsl:value-of"){
            data[line].print=1;
            data[line].tags="xsl:value-of";
            calculate(buffer,i);
            if(str.substr(init,6)=="select"){
                dir3=str.substr(start,tail-start);
                data[line].select=dir1+'/'+dir2+'/'+dir3;
            }
            data[line].parameters="null";
        }
        else if(str.substr(i+1,6)=="xsl:if"){
            data[line].print=1;
            data[line].tags="xsl:if";
            calculate(buffer,i);
            data[line].select=dir1+'/'+dir2;
            if(str.substr(init,4)=="test"){
                data[line].parameters=str.substr(start,tail-start);
            }
        }
        else if(str.substr(i+1,8)=="xsl:text"){

        }
        else if(str.substr(i+1,13)=="/xsl:for-each"){
            dir2="";
        }
        else if(str.substr(i+1,7)=="/xsl:if"){

        }
        else if(str.substr(i+1,13)=="/xsl:template"){
            dir1="";
        }
        else if(str.substr(i+1,15)=="/xsl:stylesheet"){

        }
        else if(str.substr(i+1,4)=="?xml"){

        }
        else if(str.substr(i+1,14)=="xsl:stylesheet"){

        }
    }

}

void print()
 {
     for(int i=1;i<line;i++){
        if(data[i].print==1){
            sign++;
            if(sign!=1){
                ofile << endl << "----------" ;
                ofile << endl << "tags:" << data[i].tags;
                ofile << endl << "select:" << data[i].select;
                ofile << endl << "parameters:" << data[i].parameters;
            }
            else{
                ofile << "tags:" << data[i].tags;
                ofile << endl << "select:" << data[i].select;
                ofile << endl << "parameters:" << data[i].parameters;
            }
        }
     }
 }

void calculate(char buffer[],int i)
{
    while(buffer[i]!=' '){
        i++;
    }
    i++;
    init=i; //tag後面的第一個字元位置
    while(buffer[i]!='"'){
        i++;
    }
    i++;
    start=i; //雙引號後面的字元
    while(buffer[i]!='"'){
        i++;
    }
    tail=i; //第二雙引號的位置
    i++;
}

void func(char buffer[],int start)
{
    int j=start;
    while(buffer[j]!='['){
        j++;
    }
    j++;
    pos1=j; //[後面字元的位置
    while(buffer[j]!=']'){
        j++;
    }
    pos2=j; //]的位置
}

int findtag(string tags,int length)
{
    flag=0;
    char buffer2[100];
    target="</"+tags+">";
    fstream ifile2;
    ifile2.open(openfilename.c_str(),ios::in);
    while(!ifile2.eof())
    {
        counter++;
        ifile2.getline(buffer2,sizeof(buffer2));
        string str2(buffer2);
        if(str2.substr(0,length+1)=="<"+tags){
            num++;
        }
        if(num==2){
            line21=str2;
            break;
        }
        if(str2.substr(0,length+3)==target){
            flag=1;
        }
    }
    return flag;
}

void error()
{
    ofile << "Error" << endl;
    ofile << "line" << "\t" << line << "\t" << buffer << endl;
    ofile << "line" << "\t" << counter << "\t" << line21;
}

void reset()
{
    line=1;
    flag=0;
    counter=0;
    num=0;
    sign=0;
    for(int i=0;i<50;i++){
        data[i].print=0;
        data[i].tags="";
        data[i].select="";
        data[i].parameters="";
    }
}
