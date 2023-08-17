
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include<thread>
#include <vector>
#include <string>
#include <filesystem>
using namespace std;
#define LOOPBACK_IP "127.0.0.1"
int sendall(int s, char *buf, int len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = len; // how many we have left to send
    int n;

    while(total < len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 onm failure, 0 on success
} 
void server(int soc,sockaddr_in my_addr,string input, vector<string> info,vector<string> files,vector<string> files_req){
	struct sockaddr_in their_addr;
	int newSocket;
	 socklen_t  addr_size = sizeof(their_addr);		
   bind(soc,(struct sockaddr*) &my_addr, sizeof(struct sockaddr));
   listen(soc,stoi(input));
   //if(listen(soc,stoi(input))==0){
   //cout << "Listening server done\n";}
    while (1) {
 	 listen(soc,stoi(input));

        newSocket = accept(soc, (struct sockaddr*)&their_addr, &addr_size);
        char buff[256];
	strncpy(buff,info[2].c_str(),sizeof(buff));
	send(newSocket,buff,256,0);
        char diff[256];
        char choice[65530];
        //convert info[2] to char and send as 2 and 3rd input to send
	//strncpy(choice,info[2].c_str(),sizeof(choice));
    
  
	int b = recv(newSocket,choice,65530,0);
    
    cout<<choice<<endl;
	string a(choice);
    cout<<sizeof(a)<<endl;
	vector<string> set;
	string T;
	  stringstream X(a);
	  string id;
	  int m=0;
	  std::set<std::string> s;
	  for(int i=0;i<files.size();i++){
	  	s.insert(files[i]);
	  	//cout<<files[i];
	  	}
	while (getline(X, T, ',')) {  
          
        set.push_back(T);
       
	int x=0;
	cout<<set[m]<<files.size()<<endl;
	for(int j=0;j<files.size();j++){
	//cout<<set[m]<<endl;
                            if((set[m].compare(files[j])) == 0) {
                            //cout<<set[m]<<"if loop working"<<endl;
			    id=id+set[m]+"$1,";
			    x=1;
                            break;}
             
                        }
	if(x==0){
	//cout<<set[m]<<"if loop not working"<<endl;
	id=id+set[m]+"$0,";
	x=0;
	}
	m=m+1; // print split string 
        //cout<<m<<endl;
    }
    	/*cout<<set.size()<<"files size"<<endl;
    	cout<<sizeof(id)<<endl;*/
    	
	int y=sendall(newSocket,&id[0],id.length());
	//cout<<y<<"id sent";
	
	close(newSocket);
	//cout << "choice" << choice<<endl;}
	}
	};
void client(vector<string> input,vector<string> words,vector<string> info,vector<string> files,vector<string> files_req,vector<string> minid){
	for(int i=0;i<stoi(input[1]);i++){
	int zero=0;
	socklen_t sin_size;
       int con_soc[stoi(input[1])];
       con_soc[i]=socket(PF_INET,SOCK_STREAM,0);
       struct sockaddr_in their_addr;
       their_addr.sin_family=AF_INET;
       their_addr.sin_port=htons(stoi(words[(2*i)+1]));
       their_addr.sin_addr.s_addr=inet_addr(LOOPBACK_IP);
       memset(&(their_addr.sin_zero),'\0',8);
       sin_size=sizeof(struct sockaddr_in);
       //cout << "Before accepting " << i << "\n";
       char choice[65530];

       int connection=connect(con_soc[i],(struct sockaddr *)&their_addr,sin_size);
       while(connection<0)
       		connection=connect(con_soc[i],(struct sockaddr *)&their_addr,sin_size);
       char buff[256];
       int x=recv(con_soc[i],buff,256,0);
       cout << "Connected to " << words[2*i] << " with unique-ID " << buff << " on port " << words[(2*(i))+1] << endl;
       char c=',';
	string a;
	for(int i=0;i<files_req.size();i++){
	a=a+files_req[i]+",";}
	//cout<<a<<"filesrequired"<<endl;
        cout<<a<<endl;
        cout<<a.length();
        sendall(con_soc[i],&a[0],a.length());
        char choice1[65530];
        int b = recv(con_soc[i],choice1,65530,0);
        //cout<<b<<endl;
        //cout<<choice1;
        string T;
	stringstream X(choice1);
	string id;
	int j=0;
	while (getline(X, T, ',')) {
		char f='$';
		T=T+"$";
		stringstream id(T);
		string divvi;
		int z=0;
		while(getline(id,divvi,f)){
		if(z==1){
			if(divvi == "1"){
			if(minid[j] >buff  or stoi(minid[j])==0){
				minid[j]=buff;}}}
		
		z=z+1;
		}
		j=j+1;
		}
	if(i==stoi(input[1])-1){
	//cout<<i<<endl;
	vector<string> output;
	for(int j=0;j<files_req.size();j++){
		int depth=1;
		int z=0;
		if(stoi(minid[j])==0){depth=0;}
		cout<<files_req[j]<<minid[j]<<endl;
		output.push_back("Found "+files_req[j]+" at "+ minid[j] +" with MD5 0 at depth "+to_string(depth));
		z=0;

		}
	sort(output.begin(),output.end());
	for(int j=0;j<files_req.size();j++){
	cout<<output[j]<<endl;}
	
	}
	
	close(con_soc[i]);}}
int main(int argc, char *argv[]) {
    vector<string> input;
    string no_of_neighbours;
   vector<string> words;
   vector<string> info;
vector<string> files;
vector<string> files_req;
int a=0;
   size_t po=0;
   if (auto dir = opendir(argv[2])) {
    while (auto f = readdir(dir)) {
    	DIR *dr;
    	string k = "/";
    	 string name = argv[2] + k + f->d_name;
            dr = opendir(name.c_str());
        if (!f->d_name || f->d_name[0] == '.' ||dr != NULL )
            continue; // Skip everything that starts with a dot
	files.push_back(f->d_name);
        cout<< f->d_name<<"\n";
    }
    closedir(dir);
}
   fstream file;
    file.open(argv[1],ios::in);
    if (file.is_open()){
      string line;
      while(getline(file, line)){
        input.push_back(line);
      }
      file.close();
   }
  vector<string> minid;
  for(int i=4;i<input.size();i++){
	files_req.push_back(input[i]);
	//minid[i-4]="0";
	minid.push_back("0");
	}

	//for(int i=0; i < input. size(); i++) std::cout << input.at(i) << endl;
    while ((po = input[0].find(" ")) != string::npos) {
        info.push_back(input[0].substr(0, po));
        input[0].erase(0, po + 1);
   }
   info.push_back(input[0]);
   size_t pos = 0;
   while ((pos = input[2].find(" ")) != string::npos) {
        words.push_back(input[2].substr(0, pos));
        input[2].erase(0, pos + 1);
   }
   words.push_back(input[2]);
   vector<string> clients;
   for (int i=0; i<2*stoi(input[1]); i=i+2) {
       string cl;
       cl = words[i] + "-" + words[i+1];
       clients.push_back(cl);
   }

   for (int i = 0; i<clients.size(); i++) {
       //cout << clients[i] << "\n";
   }
	int soc,newSocket;
	
   socklen_t sin_size;
   int soc_cli[stoi(input[1])];
   int con_soc[stoi(input[1])];
   struct sockaddr_in my_addr;
  
   soc=socket(PF_INET,SOCK_STREAM,0);
   my_addr.sin_family=AF_INET;
   my_addr.sin_port=htons(stoi(info[1]));
   my_addr.sin_addr.s_addr=inet_addr(LOOPBACK_IP);
   memset(&(my_addr.sin_zero),'\0',8);
//for(int i=0; i < files_req. size(); i++) std::cout << files_req.at(i) << endl;
   thread th2(server,soc,my_addr,input[1], info,files,files_req);
   thread th1(client,input,words,info,files,files_req,minid);
   th1.join();
   th2.join();


  
    
   
}
