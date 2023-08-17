#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sys/stat.h>
using namespace std;

bool sortcol(const vector<string>& v1, const vector<string>& v2)
{
    return v1[0] < v2[0];
}




void server_side(int server , string unique_id,vector<string> my_files,char* arg2){
    if(listen(server,20) == 0){
        for(int i=0;i < 2000; i++){
            struct sockaddr_in their_addr;
            socklen_t sin_size = sizeof(struct sockaddr_in);
            int a = accept(server,(struct sockaddr *)&their_addr, &sin_size);
        //    cout << 29 << endl;
             char idk[256]; // no of files to search
            //  int m=recv(a,idk,256,0);
            // cout << 32 << endl;
             send(a,&unique_id[0],256,0);
                recv(a,idk,sizeof(int),0);
                // cout << 35 << endl;
                for(int k=0;k<atoi(idk);k++){
                    // cout << "no of files is "<< idk << endl;
                    // cout << 37 << endl;
                    char buffer[256];
                    recv(a,buffer,256,0);
                    string name = buffer;
                    // cout << 41 << endl;
                    bool found=find(my_files.begin(),my_files.end(),name)!=my_files.end();
                    // cout << unique_id << "have or not "<< name << "-----"<< found << endl;
                    string yes="1";
                    string no="0";
                    // cout << 43 << endl;
                    if(found){
                        // cout << "sending yes\n";
                        int yy=send(a,yes.c_str(),strlen(yes.c_str()),0);
                        // cout << yy<< "--sent yes\n";
                        string file_path=arg2+name;
                        ifstream file(file_path,ios::binary);
                        int size=file.tellg();
                        file.seekg(0,ios::beg);
                        char* file_data = new char[size];
                        file.read(file_data,size);
                        file.close();
                        int* file_size=&size;
                        // cout << "sending file data\n";
                        int ffff=send(a,(char *)file_size,sizeof(int),0);

                        // cout<< ffff << "sent file data\n";
                        send(a,file_data,size,0);
                        // cout << 55 << endl;
                    }
                    else{
                        // cout << "sending no\n";
                        int ssss=send(a,no.c_str(),strlen(no.c_str()),0);
                        // cout << ssss << "sent no\n";
                        
                    }
                }
                // cout << 59 << endl;
            // stringstream ss(idk);
            //     string s;
            //     char del=' ';
            //     string ch=unique_id+" ";
            //     while(getline(ss,s,del)){
            //         bool found = find(my_files.begin(),my_files.end(),s)!=my_files.end();
            //         ch=ch+to_string(found)+" ";
            //     }
            //     buffer=&ch[0];
            // send(a,buffer,256,0);

        }

    }
    return;
}

void client_side(vector<int> ports,int neighbours,string files_names,string unique_id,char* arg2){
    int cli_soc[neighbours];
    // cout << 79 << endl;
    vector<string> un;
    vector<vector<string>> ree;
    stringstream fi(files_names);
        vector<string> fil;
        string bu;
        while(getline(fi,bu,' ')){
            fil.push_back(bu);
        }
        vector<int> fil_ex[fil.size()];
        vector<char *> f_d[fil.size()];
        vector<int> f_s[fil.size()];
        
    for(int i=0;i<neighbours;i++){
        cli_soc[i]=socket(PF_INET,SOCK_STREAM,0);
        struct sockaddr_in their_addr;
        their_addr.sin_family=AF_INET;
        their_addr.sin_port=htons(ports[2*i+1]);
        their_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        memset(&(their_addr.sin_zero),'\0',8);
        socklen_t sin_size = sizeof(struct sockaddr_in);
        int con = connect(cli_soc[i],(struct sockaddr *)&their_addr, sin_size);
        while (con < 0 ){
            con = connect(cli_soc[i],(struct sockaddr *)&their_addr, sin_size);

        }

        
        
        char buffer[256];
        
        vector<string> res;
        
        
        recv(cli_soc[i],buffer,256,0);
        send(cli_soc[i],to_string(fil.size()).c_str(),sizeof(int),0);
        un.push_back(buffer);
        cout << "connected to " << ports[2*i] << " with unique id " << buffer << " on port " << ports[2*i+1] << endl;

        // for(int j=0;j<fil.size();j++){

        //         char* msg;
        //         char pre[256];
        //         send(cli_soc[i],fil[j].c_str(),fil[j].length(),0);
        //         int rrr=recv(cli_soc[i],pre,256,0);
                
        //         string pres=pre;
        //         cout << pres << "_------------------_\n";
        //         if(pres=="1"){
        //                 fil_ex[j].push_back(atoi(buffer));
        //                 int fil_si=0;
                        
        //                 int kkk=recv(cli_soc[i],(char *)fil_si,sizeof(fil_si),0);
        //                 cout << kkk<< "---------file size" << endl;
        //                 char* fil_da=new char[fil_si];
        //                 memset(fil_da,'\0',fil_si);
        //                 recv(cli_soc[i],fil_da,fil_si,MSG_WAITALL);
        //                 f_d[j].push_back(fil_da);
        //                 f_s[j].push_back(fil_si);
        //                 cout << fil_da << endl;

        //         }

        // }

        
        }
        for(int i=0;i<fil.size();i++){
            string name=fil[i];
            char pre[256];
            for(int j=0;j<neighbours;j++){
                send(cli_soc[j],name.c_str(),strlen(name.c_str()),0);
                recv(cli_soc[j],pre,256,0);
                string present=pre;
                if(present == "1"){
                    fil_ex[i].push_back(stoi(un[j]));
                    int fil_si=0;
                        
                        int kkk=recv(cli_soc[j],(char *)fil_si,sizeof(fil_si),0);
                        // cout << kkk<< "---------file size" << endl;
                        char* fil_da=new char[fil_si];
                        memset(fil_da,'\0',fil_si);
                        recv(cli_soc[i],fil_da,fil_si,MSG_WAITALL);
                        f_d[i].push_back(fil_da);
                        f_s[i].push_back(fil_si);

                }
            }
        }
        
    
    
    int min_ind = 0;
    for(int i=0;i<fil.size();i++){
        string name = fil[i];
        if(fil_ex[i].size()>0){
            min_ind=min_element(fil_ex[i].begin(),fil_ex[i].end()) - fil_ex[i].begin();
            char* dat=f_d[i][min_ind];
            string down="Downloaded/";
            mkdir((arg2+down).c_str(),0777);
            ofstream my_file;
            my_file.open(arg2+down+name,ios::binary);
            my_file.write(dat,f_s[i][min_ind]);
            delete[] dat;
            my_file.close();
            cout << name << " at "<<*min_element(fil_ex[i].begin(),fil_ex[i].end()) << endl;
        }
        else cout << name <<  " at "<< "0\n";
    }
    

    // cout << 158 << endl;

    return;

}

int main(int argc , char* argv[]){
    // cout << 165 << endl;
    vector<string> inputs;
   fstream files;
   files.open(argv[1],ios::in);
   if (files.is_open()){
       string tp;
       while(getline(files,tp)){
           inputs.push_back((tp));
       }
       files.close();
   }
   istringstream ss(inputs[0]);
   vector<int> info;
   string word;
   while(ss >> word){
       info.push_back(stoi(word));
   }
   istringstream s(inputs[2]);
   vector<int> neighbours;
   while(s >> word){
        neighbours.push_back(stoi(word));
   }
   int no_neighbours=stoi(inputs[1]);
    int no_files=stoi(inputs[3]);
    string files_names="";
    for(int i=0;i<no_files;i++){
            files_names=files_names+(inputs[4+i])+" ";
    }

   struct dirent* d;
   vector<string> my_files;
   DIR* directory;
   directory=opendir(argv[2]);
   if(directory!=NULL){
       while((d=readdir(directory))!=NULL){
           string ap="/";
           string check = argv[2]+ap+d->d_name;
           DIR* directory_1=opendir(check.c_str());
           if (directory_1==NULL){
                cout << d->d_name << endl;
                my_files.push_back(d->d_name);
       }
       }
        closedir(directory);
   }

   int server = socket(PF_INET,SOCK_STREAM,0);
   socklen_t sin_size;
    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(info[1]);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&(my_addr.sin_zero), '\0', 8);
    // bind(server, (struct sockaddr*) &my_addr, sizeof(struct sockaddr));
    int t = bind(server, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
        if(t == -1 )  {perror("bind");exit(1);close(server);server = socket(AF_INET, SOCK_STREAM, 0);t = bind(server, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));};
    
    thread t1(server_side,server,to_string(info[2]),my_files, argv[2]);
    thread t2(client_side,neighbours,no_neighbours,files_names,to_string(info[2]),argv[2]);
    // cout << 222 << endl;
    t1.join();
    t2.join();





}