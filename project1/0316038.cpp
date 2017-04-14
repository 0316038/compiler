#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(){
    string line;
    char temp;
    int i = 0;
    int line_num = 0;
    int default_flag = 0;
    fstream in("input.xml");
    fstream file;
    file.open("output.txt" , ios::out);
    while(in >> line){
        i = 0;
        if(line_num != 0 ) file << endl;
        for(i = 0;i < line.length(); ){
            default_flag = 0;
            temp = line.at(i);
            if(temp != ' ' && i != 0 && temp != '\t') file << endl;
            switch(temp){
            case '<':
                file << "<" ;
                i++;
                break;
            case '>':
                file << ">" ;
                i++;
                break;
            case '?':
                if(line.at(i-1) == '<' || line.at(i+1) == '>'){
                    file << "?" ;
                    i++;
                }
                break;
            case '/':
                if(i-1>=0){
                    if(line.at(i-1) == '<'  ){
                        file << "/" ;
                        i++;
                    }
                }
                if(line.at(i+1) == '>'){
                        file << "/" ;
                        i++;
                }
                break;
            case 'x':
                if(line.substr(i,3) == "xml"){
                    if(line.substr(i,5) == "xmlns"){
                        file << "xmlns" ;
                        i+=5;
                    }
                    else {
                        file << "xml" ;
                        i+=3;
                    }
                }
                else if(line.substr(i,3) == "xsl"){
                    file << "xsl" ;
                    i+=3;
                }
                else {
                    default_flag = 1;
                }
                break;
            case 'v':
                if(line.substr(i,7) == "version"){
                    file << "version" ;
                    i+=7;
                }
                else if(line.substr(i,8) =="value-of"){
                    file << "value-of";
                    i+=8;
                }
                else {
                    default_flag = 1;
                }
                break;
            case 'e':
                if(line.substr(i,8) == "encoding"){
                    file << "encoding" ;
                    i+=8;
                }
                else {
                    default_flag = 1;
                }
                break;
            case ':':
                file << ":" ;
                i++;
                break;
            case ' ':
                i++;
                break;
            case '\t':
                i++;
                break;
            case '"':
                file << "\"" << endl;
                i++;
                for(i; i<line.length() ;i++){
                    if(line.at(i) == '"') {
                        if(i==line.length()-1){
                            file << endl <<"\"";
                            break;
                        }
                        else if(line.at(i+1) == '>' || line.at(i+1) == '?'){
                            file << endl <<"\"";
                            break;
                        }
                    }
                    else {
                        file << line.at(i);
                    }
                }
                i++;
                break;
            case '=':
                file << "=";
                i++;
                break;
            case 's':
                if(line.substr(i,10) == "stylesheet"){
                    file << "stylesheet";
                    i+=10;
                }
                else if(line.substr(i,4) == "sort"){
                    file < "sort";
                    i+=4;
                }
                else if(line.substr(i,6) == "select"){
                    file << "select";
                    i+=6;
                }
                else {
                    default_flag = 1;
                }
                break;
            case 't':
                if(line.substr(i,8) == "template"){
                    file << "template";
                    i+=8;
                }
                else{
                    default_flag = 1;
                }
                break;
            case 'm':
                if(line.substr(i,5) =="match"){
                    file << "match";
                    i+=5;
                }
                else {
                    default_flag = 1;
                }
                break;
            case 'f':
                if(line.substr(i,8) == "for-each"){
                    file << "for-each";
                    i+=8;
                }
                else {
                    default_flag = 1;
                }
                break;
            case 'i':
                if(line.substr(i,2) == "if"){
                    file << "if";
                    i+=2;
                }
                else {
                    default_flag = 1;
                }
                break;
            default:
                default_flag = 1;
                break;
            }//end case
            if(default_flag == 1){
                if(line.at(i-1) == '<' || line.substr(i-2,2) == "</"){
                    for(i;;i++){
                        if(line.at(i) == '>') break;
                        file << line.at(i);
                    }
                }
            }
        }
    line_num++;
    }

}

