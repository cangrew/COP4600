#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <filesystem>

using namespace std;


class Mysh {
	public:	
		// Parser object that takes care of tokenizing
		class Parser {
			public:
				// Takes a string and returns a list of tokens
				vector<string>* tokenize(string str){
					stringstream token_buffer(str);
					vector<string> *tokens = new vector<string>();
					string temp;
					while(!token_buffer.eof()){
						token_buffer >> temp; 
						tokens->push_back(temp);
					}
					return tokens;
				}


		};
		

		Parser p;
		vector<string> his;
		vector<pid_t> pids;

		//Implements history operation
		void history(){
			for(int i = his.size() - 1 ; i >= 0 ; i--){
				cout << his.size() -1 - i << ": " << his[i] << endl;
			}

		}

		void history(string op){
			if(op == "-c")
				his.clear();
			else
				cerr << "Invalid argument!\n";
		}

		void byebye(){
			ofstream fout;
			fout.open("mysh.history");
			for(int i = 0; i < his.size(); i++){
				fout << his[i] << endl;
			}
			fout.close();	
			exit(0);
		}
		
		void replay(string arg){
			if(all_of(arg.begin(), arg.end(), ::isdigit)){	
				int id = stoi(arg);
				runOP(*(p.tokenize(his[his.size() -2 - id])));
			}
			else{
				cerr << "Argument Invalid!\n";
			}
		}

		void start(string arg){
			int status = 0;
			pid_t pid;
			if((pid = fork()) == 0)
				status = execlp(arg.c_str(), arg.c_str() ,NULL);
				if(status == -1){
					cerr << "Program Not Found!\n";
					exit(0);
				}
			else{
				waitpid(pid,NULL,0);
			}
		}
		
		void start(vector<string> tokens){
			char* ar[tokens.size()];
			int status = 0;
			pid_t pid;
			int j = 0;
			for(int i = 1; i < tokens.size(); i++ ){
				ar[j] = (char*)malloc(sizeof(char)*((tokens[i]).length() +1)); 
				strcpy(ar[j], tokens[i].c_str());
				j++;
			}
			ar[j] = NULL;

			if((pid = fork()) == 0)
				status = execvp(tokens[1].c_str(),ar);
				if(status == -1){
					cerr << "Program Not Found!\n";
					exit(0);
				}
			else{
				waitpid(pid,NULL,0);
			}
		}

		void background(string arg){
			int status = 0;
			pid_t pid;
			if((pid = fork()) == 0)
				status = execlp(arg.c_str(), arg.c_str() ,NULL);
				if(status == -1){
					cerr << "Program Not Found!\n";
					exit(0);
				}
			else{
				cout << pid << endl;
				pids.push_back(pid);
			}
		}


		void background(vector<string> tokens){
			char* ar[tokens.size()];
			int status = 0;
			pid_t pid;
			int j = 0;
			for(int i = 1; i < tokens.size(); i++ ){
				ar[j] = (char*)malloc(sizeof(char)*((tokens[i]).length() +1)); 
				strcpy(ar[j], tokens[i].c_str());
				j++;
			}
			ar[j] = NULL;

			if((pid = fork()) == 0)
				status = execvp(tokens[1].c_str(),ar);
				if(status == -1){
					cerr << "Program Not Found!\n";
					exit(0);
				}
			else{
				cout << pid << endl;
				pids.push_back(pid);
			}
		}

		void terminate(string arg){
			if(all_of(arg.begin(), arg.end(), ::isdigit)){	
				pid_t pid = stoi(arg);
				kill(pid, SIGKILL );
				pids.erase(remove(pids.begin(),pids.end(),pid));
			}
			else{
				cerr << "Invalid PID!\n";
			}
		}
		
		void terminate(int arg){
			kill(arg, SIGKILL );
			pids.erase(remove(pids.begin(),pids.end(),arg));
		}

		void repeat(vector<string> tokens){
					
		}
	
		void terminateall(){
			if(!pids.empty()){
				cout << "Terminating " << pids.size() << " processes: ";
				for(int i = 0; i < pids.size(); i++){
					cout << pids[i] << " ";
					terminate(pids[i]);
				}
				pids.clear();
			}
			else{
				cout << "No processes to terminate.\n";
			}
		}

		void loadHistory(){
			ifstream fin;
			fin.open("mysh.history");
			if(!fin.good())
				return;
			string temp;
			while(!fin.eof()){
				getline(fin,temp);
				his.push_back(temp);
			}
			his.pop_back();
		}
		
		/*// permision mask = 110100100
		void maik(string filepath){
			struct stat buffer;
			if(stat(filepath.c_str(), &buffer) != 0){
				int f = open(filepath.c_str(), O_WRONLY | O_CREAT, 0644);
				
				close(f);
			}
			else{
				cout << "File already exist\n";
			}
		}*/

	
		void maik(string filepath){
			ofstream file(filepath.c_str());
			if(file){
				file << "Draft" << endl;
				file.close();
			}
			else{
				cerr << "File already exist!\n";
			}
		}

		void dwelt(string filepath){
			struct stat path;
			int st = stat(filepath.c_str(),&path);
			if(st == 0){
				if(S_ISREG(path.st_mode)){
					cout << "Dwelt indeed" << endl;
				}
				else if(S_ISDIR(path.st_mode)){
					cout << "Abode is" << endl;
				}
			}
			else{
				cout << "Dwelt not" << endl;
			}
		}

		void coppy(string file1, string file2){
			ifstream fi(file1.c_str());
			if(fi){
				ofstream fo(file2.c_str());
				if(fo){
					fo << fi.rdbuf();
				}
				else{
					cerr << "File already exist or directory not found!\n";
				}
			}	
			else{
				cerr << "Source not found!\n" << endl;
			}
		}


		void runOP(vector<string> tokens){
			if(tokens[0] == "history" && tokens.size() <= 2){
				try{
					history(tokens[1]);
				}
				catch(...){
					history();
				}
			}
			else if(tokens[0] == "byebye" && tokens.size() == 1 ){
				byebye();	
			}	
			else if(tokens[0] == "replay" && tokens.size() <= 2 ){
				try{
					replay(tokens[1]);
				}
				catch(...){
					cerr << "Missing Argument!\n";
				}
			}
			else if(tokens[0] == "start" && tokens.size() >= 2){
				if(tokens.size() <= 2)
					start(tokens[1]);
				else{
					start(tokens);
				}
			}
			else if(tokens[0] == "background" && tokens.size() >= 2){
				if(tokens.size() <= 2){
					background(tokens[1]);
				}
				else{
					background(tokens);
				}
			}
			else if(tokens[0] == "terminate" && tokens.size() == 2){
				terminate(tokens[1]);
			}
			else if(tokens[0] == "repeat" && tokens.size() == 3){
				repeat(tokens);
			}
			else if(tokens[0] == "terminateall" && tokens.size() == 1){
				terminateall();
			}
			else if(tokens[0] == "maik" && tokens.size() == 2){
				maik(tokens[1]);
			}
			else if(tokens[0] == "dwelt" && tokens.size() == 2){
				dwelt(tokens[1]);
			}
			else if(tokens[0] == "coppy" && tokens.size() == 3){
				coppy(tokens[1], tokens[2]);
			}
			else{
				cerr << "Invalid Command!\n";
			}
		}

		//runs the prompt
		void run(){
			vector<string> *tokens;
			string instruction;
			loadHistory();
			while(1){
				cout << "#";
				getline(cin, instruction);
				his.push_back(instruction);
				tokens = p.tokenize(instruction);
				runOP(*tokens);
				free(tokens);
			}
			cout << endl;
		}
		
};		

int main(int argc, char** argv) {
	Mysh shell;
	shell.run();
}
