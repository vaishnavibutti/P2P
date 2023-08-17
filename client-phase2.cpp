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
using namespace std;

bool sortcol(const vector<string>& v1, const vector<string>& v2)
{
    return v1[0] < v2[0];
}

vector<vector<string>> file_Search(vector<vector<string>> re,vector<string> my_fi,vector<string> my){
    sort(re.begin(),re.end(),sortcol);
    re.insert(re.begin(),my);
    vector<vector<string>> ans;
    int m=re.size();
    int n=re[0].size();
    for(int i=1;i<n;i++){
        vector<string> anss;
        for(int j=0;j<m;j++){
            if(re[j][i]=="1"){
                anss.push_back(my_fi[i-1]);
                anss.push_back(re[j][0]);
                if(j==0) anss.push_back("0");
                else anss.push_back("1");
                ans.push_back(anss);
                break;
            }
            else if(j==m-1){
                anss.push_back(my_fi[i-1]);
                anss.push_back("0");
                anss.push_back("0");
                ans.push_back(anss);

            }
        }
    }
    sort(ans.begin(),ans.end(),sortcol);
    for(int i=0;i<ans.size();i++){
       cout << "Found "<<ans[i][0] << " at "<<  ans[i][1] << " with MD5 0" << " at depth "<< ans[i][2] << endl;
    }
    return ans;
}


void server_side(int server , string unique_id,vector<string> my_files){
    if(listen(server,20) == 0){
        for(int i=0;i < 2000; i++){
            struct sockaddr_in their_addr;
            socklen_t sin_size = sizeof(struct sockaddr_in);
            int a = accept(server,(struct sockaddr *)&their_addr, &sin_size);
           char* buffer;
             char idk[256];
             int m=recv(a,idk,256,0);
            stringstream ss(idk);
                string s;
                char del=' ';
                string ch=unique_id+" ";
                while(getline(ss,s,del)){
                    bool found = find(my_files.begin(),my_files.end(),s)!=my_files.end();
                    ch=ch+to_string(found)+" ";
                }
                buffer=&ch[0];
            send(a,buffer,256,0);

        }

    }
    return;
}

void client_side(vector<int> ports,int neighbours,string files_names,string unique_id,vector<string> fff){
    int cli_soc[neighbours];
    vector<string> un;
    vector<vector<string>> ree;
    stringstream fi(files_names);
        vector<string> fil;
        string bu;
        while(getline(fi,bu,' ')){
            fil.push_back(bu);
        }
        vector<string> ff;
        ff.push_back(unique_id);
        for(int i=0;i<fil.size();i++){
            if(find(fff.begin(),fff.end(),fil[i]) !=fff.end()) ff.push_back("1");
            else ff.push_back("0");
        }
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
        char* msg;
        vector<string> res;
        
        msg=&files_names[0];
        send(cli_soc[i],msg,256,0);
        recv(cli_soc[i],buffer,256,0);
        stringstream st(buffer);
        string buff;
        while (getline(st,buff,' '))
        {
            res.push_back(buff);
        }
        ree.push_back(res);
        
        cout << "connected to " << ports[2*i] << " with unique id " << res[0] << " on port " << ports[2*i+1] << endl;
        un.push_back(res[0]);
    }
    vector<vector<string>> faa=file_Search(ree,fil,ff);

    

    return;

}

int main(int argc , char* argv[]){
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
    bind(server, (struct sockaddr*) &my_addr, sizeof(struct sockaddr));
    
    thread t1(server_side,server,to_string(info[2]),my_files);
    thread t2(client_side,neighbours,no_neighbours,files_names,to_string(info[2]),my_files);

    t1.join();
    t2.join();





}