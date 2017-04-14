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
#include <algorithm>

using namespace std;

struct ListNode
{
    string name;
    string dir;
    string bookdetail;
    int count;
    int order;
    int printtag;
    int selecttag;
    ListNode *parent=NULL;
    ListNode *siblings=NULL;
    ListNode *next[50];
};

struct printdata{
    string a;
    string b;
}printdata[10];

//functions
void ReadXML();
void processXML(char array1[]);
void process(char buffer[]);
void print();
void reset();

//global variables
ListNode List;
ListNode *p=&List,*q;
ListNode *r;
bool first=1;
string openfilename,outputfilename;
string dir1,dir2,dir3,target,line21;
string temp[20];
fstream ofile;
char buffer[100],array1[100];
int line=1;
int length=0;
int datacount=0;
int counting=0;
int sortflag=0;

bool cmp(string a, string b)
{
    if(a.compare(b) < 0)
        return true;
    else
        return false;
}

int main()
{
    ReadXML();

    for(int i=1;i<=3;i++){
        stringstream ss;
        ss << i;
        fstream ifile;
        openfilename= "XSL-" + ss.str() + ".txt";
        ifile.open(openfilename.c_str(), ios::in);
        outputfilename= "Result-" + ss.str() + ".txt";
        ofile.open(outputfilename.c_str(), ios::out);
        if(!ifile)
            cout << "Cannot open file!" << endl;
        else{
            do{
                ifile.getline(buffer,sizeof(buffer));
                if(line>3){
                    process(buffer);
                }
                line++;
            }while(!ifile.eof());
            ifile.close();
            print();
            ofile.close();
        }
        reset();
    }

}

void ReadXML()
{
    List.count=0;
    List.order=0;
    List.selecttag=0;
    List.printtag=0;
    fstream XMLfile;
    XMLfile.open("XML.txt", ios::in);
    if(!XMLfile){
        cout << "Cannot open XML file!" << endl;
    }
    else{
        do{
            XMLfile.getline(array1,sizeof(array1));
            processXML(array1);
        }while(!XMLfile.eof());

    }
}

void processXML(char array1[]){
    string str(array1);
    string name;
    string bookdetail;
    int i=0;

    while(array1[i]!='<'){
        i++;                    //i指到'<'的位置
    }
    i++;                        //i指到'<'後一個的位置
    if(array1[i]!='/'){
        while(array1[i]!='>'){
            name+=array1[i];    //temp存<>中間的字串
            i++;
        }                       //i指到'>'

        if(first==1){
            first=0;
            p->name=name;
            p->dir='/'+name;

        }

        else if(first!=1){
            q = new ListNode;       //create new
            q->count=0;
            q->order=0;
            q->selecttag=0;
            q->printtag=0;
            q->name=name;
            q->dir='/'+name;
            q->parent=p;            //建立上一層索引
            p->next[p->count]=q;    //建立下一層索引
            if(p->count!=0){
                p->next[p->count-1]->siblings=q;
            }
            p->count++;             //索引數目增加
            p=q;                    //準備讀下一個資料起始點
        }

        if(array1[i+1]=='\0' || array1[i+1]=='\r'){

        }
        else{   //還有資料要讀
            i++;
            while(array1[i]!='<'){
                bookdetail+=array1[i];
                i++;
            }

            p->bookdetail=bookdetail;
            if(array1[i+1]=='/'){
                p=p->parent;
            }
        }

    }

    else if(array1[i]=='/'){
        i++;
        string temp;
        while(array1[i]!='>'){
            temp+=array1[i];
            i++;
        }
        if(temp!=List.name){
            p=p->parent;
        }
    }

}

void process(char buffer[])
{
    string words[10];
    for(int k=0;k<10;k++){
        words[k]="";
    }
    string str(buffer);
    string tagname;
    int i=0,x=0,select=0;
    while(buffer[i]!='<'){
        i++;                //指到<的位置
    }

    if(str.substr(i+1,12)=="xsl:template"){
        while(buffer[i]!='"'){
            i++;
        }
        i++;
        while(buffer[i]!='"'){
            tagname+=buffer[i];
            i++;
        }
        r=&List;

    }
    else if(str.substr(i+1,12)=="xsl:for-each"){
        while(buffer[i]!='"'){
            i++;
        }
        i++;
        while(buffer[i]!='/'){
            tagname+=buffer[i];     //tagname:存part
            i++;
        }
        for(int k=0;k<r->count;k++){
            if(r->next[k]->name==tagname){
                r=r->next[k];       //r指到part
                break;
            }
        }
        i++;
        tagname="";
        while(buffer[i]!='"'){
            if(buffer[i]=='['){
                break;
            }
            tagname+=buffer[i];     //tagname:存book
            i++;
        }
        for(int k=0;k<r->count;k++){
            if(r->next[k]->name==tagname){
                r=r->next[k];       //r指到第一本book（k=0）
                break;
            }
        }
        ListNode *s=r;
        while(s!=NULL){
            length++;               //length存書本數量
            s=s->siblings;          //建立書本之間的link
        }

        if(buffer[i]=='['){
            string select;
            i++;
            while(buffer[i]!='='){
                select+=buffer[i];  //select存author
                i++;
            }
            while(buffer[i]!='\''){
                i++;
            }
            i++;
            tagname="";
            while(buffer[i]!='\''){
                tagname+=buffer[i]; //tagname存Uzi
                i++;
            }

            int k;
            for(k=0;k<r->count;k++){
                if(r->next[k]->name==select){
                    break;
                }
            }
            ListNode *s=r;
            while(s!=NULL){
                if(s->next[k]->bookdetail==tagname){
                    s->selecttag=1;
                }
                s=s->siblings;
            }

        }
        else{
            ListNode *s=r;
            while(s!=NULL){
                s->selecttag=1;
                s=s->siblings;
            }
        }

    }
    else if(str.substr(i+1,8)=="xsl:sort"){
        sortflag=1;
        while(buffer[i]!='"'){
            i++;
        }
        i++;
        while(buffer[i]!='"'){
            tagname+=buffer[i];     //tagname:title
            i++;
        }
        select=0;
        for(int k=0;k<r->count;k++){
            if(r->next[k]->name==tagname){
                select=k;          //select=要sort的資料是book的第幾項detail
            }
        }
        ListNode *s=r;
        while(s!=NULL){
            words[x]=s->next[select]->bookdetail;   //words[x]存書名
            x++;
            s=s->siblings;
        }
        sort(words,words+length,cmp);

        for(int k=0;k<length;k++){      //存book的order
            ListNode *t=r;
            while(t!=NULL){
                if(t->next[select]->bookdetail==words[k]){
                    t->order=k;
                }
                t=t->siblings;
            }
        }

    }

    else if(str.substr(i+1,12)=="xsl:value-of"){
        while(buffer[i]!='"'){
            i++;
        }
        i++;
        while(buffer[i]!='"'){
            tagname+=buffer[i]; //title
            i++;
        }

        for(int k=0;k<r->count;k++){
            if(r->next[k]->name==tagname){
                select=k;          //select=要印的資料是book的第幾項detail
            }
        }
        if(sortflag==1){
            for(int k=0;k<length;k++){
                ListNode *t=r;
                while(t!=NULL){
                    if(t->order==k){
                        if(counting==0){
                            printdata[datacount].a=t->next[select]->bookdetail;
                        }
                        else if(counting==1){
                            printdata[datacount].b=t->next[select]->bookdetail;

                        }
                    }
                    t=t->siblings;
                }
                datacount++;
            }
            datacount-=length;
            counting=1;
        }

        else if(sortflag==0){
            ListNode *t=r;
            while(t!=NULL){
                if(t->printtag==1){
                    if(counting==0){
                        printdata[datacount].a=t->next[select]->bookdetail;
                        datacount++;
                    }
                    else if(counting==1){

                        printdata[datacount].b=t->next[select]->bookdetail;
                        datacount++;
                    }

                }
                t=t->siblings;

            }
            datacount-=length;
            counting=1;
        }

    }
    else if(str.substr(i+1,13)=="/xsl:for-each"){
        ListNode *s=r;
        while(s!=NULL){
            s->order=0;
            s->printtag=0;
            s->selecttag=0;
            s=s->siblings;
        }
        counting=0;
        sortflag=0;
        datacount+=length;
        length=0;
        r=r->parent->parent;
    }
    else if(str.substr(i+1,6)=="xsl:if"){
        while(buffer[i]!='"'){
            i++;
        }
        i++;
        while(buffer[i]!=' '){
            tagname+=buffer[i];     //price
            i++;
        }
        i++;
        for(int k=0;k<r->count;k++){
            if(r->next[k]->name==tagname){
                select=k;          //select=要sort的資料是book的第幾項detail
            }
        }
        string condition;
        while(buffer[i]!=' '){
            condition+=buffer[i];   //condition存&gt;或&lt;
            i++;
        }
        i++;
        string compare;
        while(buffer[i]!='"'){
            compare+=buffer[i];     //compare存10
            i++;
        }
        double comp=atof(compare.c_str());
        double temp;
        length=0;
        if(condition=="&gt;"){
            ListNode *s=r;
            while(s!=NULL){
                //cout << s->selecttag << endl;
                temp=atof(s->next[select]->bookdetail.c_str());
                //cout << temp << endl;
                if(temp>comp && s->selecttag==1){
                    length++;
                    s->printtag=1;
                }
                s=s->siblings;
            }
        }
        else if(condition=="&lt;"){
            ListNode *s=r;
            while(s!=NULL){
                temp=atof(s->next[select]->bookdetail.c_str());
                if(temp<comp && s->selecttag==1){
                    length++;
                    s->printtag=1;
                }
                s=s->siblings;
            }
        }

    }

    else if(str.substr(i+1,13)=="/xsl:template"){
        ListNode *s=r;
        while(s!=NULL){
            s->order=0;
            s->printtag=0;
            s->selecttag=0;
            s=s->siblings;
        }
    }
}

void print()
{
    for(int i=0;i<10;i++){
        if(printdata[i].a!="" && printdata[i].b!=""){
            if(i==0){
                ofile <<printdata[i].a << " ";
                ofile << printdata[i].b ;
            }
            else{
                ofile << endl <<printdata[i].a << " ";
                ofile << printdata[i].b ;
            }

        }

    }
}

void reset()
{
    line=1;
    datacount=0;
    length=0;
    counting=0;
    sortflag=0;
    first=1;
    for(int j=0;j<10;j++){
        printdata[j].a="";
        printdata[j].b="";
    }
}
