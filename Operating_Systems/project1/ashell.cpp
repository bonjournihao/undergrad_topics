//
//  ashell.cpp
//  ECS 150 P1
//
//  Created by  on 04/01/16.
//  Copyright © 2016 . All rights reserved.
//


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>// file info
#include <dirent.h> // ls
#include <cstdlib>
#include <termios.h>
#include <cctype>
#include <string>
#include <vector>
#include <cstring>
#include <fcntl.h>
#include <queue>
#include <iostream>


using namespace std;

void ls(char* address);
void ff(char* command);
void pwd();
//void recursive_ff(char* filename, char* root);




void run(char* command1)
{
    //first scan how many pipe
    int counter=0;
    int i=0;
    char record[100];
    while(true)
    {
        if(command1[i]=='|' or command1[i]=='>' or command1[i]=='<')
        {
            record[counter]=command1[i];
            counter++;
        }
        i++;
        if(command1[i]=='\0')
            break;
    }
    
    //get token
    char * pch;
    vector<char*> store(10);
    pch = strtok (command1,"><|\n");
    store[0]=pch;
//cout<<"0 is " <<store[0]<<endl;
    int j=1;
    while (pch != NULL)
    {
        pch = strtok (NULL, "><|\n");
        store[j]=pch;
       // cout<<j<<" is "<<store[j]<<endl;
        j++;
    }
   // cout<<"counter is "<<counter<<endl;
    
    char* c1[100][100];
    for(int k=0;k<=counter;k++)
    {
        pch=strtok(store[k]," \n");
        c1[k][0]=pch;
        int counter1=1;
        while(pch != NULL)
        {
            pch=strtok(NULL," \n");
            c1[k][counter1]=pch;
        //    cout<<c1[k][counter1]<<endl;
            counter1++;
        }
        c1[k][counter1]=NULL;
    //    cout<<"hahha"<<endl;
    }

    //record start from 0 to counter-1
    //c1 statr from 0 to counter
    
    //swap
    for(int i=0;i<=counter-1;i++)
    {
        if(record[i]=='<')
        {
            char *tempt[100][100];
            int counter2=0;
            while(c1[i][counter2]!=NULL)
            {
                tempt[0][counter2]=c1[i][counter2];
               // cout<<tempt[counter];
                counter2++;
            }
            tempt[0][counter2]=NULL;
            c1[i][0]=c1[i+1][0];
            counter2=0;
            while(tempt[0][counter2]!=NULL)
            {
                c1[i+1][counter2]=tempt[0][counter2];
                counter2++;
            }
            
        }
    }

     int pp[100][2];
   // int p[2];
    // pipe(p);
    for(int k = 1; k <= counter; k++)
    pipe(pp[k]);

    for(int k=1;k<=counter+1;k++)
    {
       // cout<<"k is "<<k<<endl;
       // cout<<c1[k][0]<<endl;
        if(fork()==0)
        {
            if(k!=counter+1)//redirect writing end;
            {
                dup2(pp[k][1],1);
                close(pp[k][1]);
                close(pp[k][0]);
            }
            if(k!=1) // redirect reading end;
            {
                dup2(pp[k-1][0],0);
                for(int j=1;j<=k-1;j++)
                {
                    close(pp[j][1]);
                    close(pp[j][1]);
                }

            }
            if(k==1)
            {
                if(!strcmp(c1[k-1][0],"ls"))
                   {
                       ls(c1[k-1][1]);
                       exit(0);
                   }
                else if(!strcmp(c1[k-1][0],"pwd"))
                   {
                       pwd();
                       exit(0);
                   }
                else if(!strcmp(c1[k-1][0],"ff"))
                   {
                       ff(command1);
                       exit(0);
                   }
                else
                {
                        execvp(c1[k-1][0],c1[k-1]);
                        exit(0);
                }      
            }

            if(record[k-2]=='>')
            {
               int fd1=creat(c1[k-1][0],S_IRWXU);
                char cbuff[1];
                while(read(0,cbuff,1))
                {
                    write(fd1,cbuff,1);
                }
                exit(0);
            }
            else if(record[k-2]=='<')
            {
                int fd2=open(c1[k-2][0],O_RDWR);
                char cbuff[1];
                while(read(fd2,cbuff,1))
                {
                    write(1,cbuff,1);
                }
                 exit(0);
            }


            if(!strcmp(c1[k-1][0],"ls"))
            {
                ls(c1[k-1][1]);
                exit(0);
            }
            else if(!strcmp(c1[k-1][0],"pwd"))
            {
               pwd();
               exit(0);
            }
            else if(!strcmp(c1[k-1][0],"ff"))
            {
               ff(command1);
               exit(0);
            }
            else
            {
                execvp(c1[k-1][0],c1[k-1]);
                exit(0);
            }
        }
    }
    
   // close(pp[0]);
  // close(pp[1]);
    for(int i=1;i<=counter;i++)
   {
      close(pp[i][0]);
      close(pp[i][1]);
   }
    
}


/*
//recursively open .
void recursive_ff(char* filename, char* root, char* open_dir)
{
    DIR* dp; //directory reader
    struct dirent *d;//directory
    dp = opendir(open_dir);
    if (dp == NULL)
    {
        string dir;
        dir.assign(open_dir);
        write(STDOUT_FILENO, "cannot open: ",13); 
        write(STDOUT_FILENO, open_dir, dir.size());
        return;
    }
    
    while (1)
    {
        d = readdir(dp);
        if (d == NULL)
        {
            cout << "end of directory " << endl;
            break;
        }

        //cout << "\nfile/dir name is: " << d->d_name << endl;
        if (strcmp(d->d_name, ".") == 0)
            continue;
        if (strcmp(d->d_name, "..") == 0)
            continue;


        //if find a directory
        //chdir and call recursively

        if (d->d_type == DT_DIR) 
        {
            cout << d->d_name << " is a directory" << endl;
            char next_dir[1000];
            getcwd(next_dir, 1000);
            strcat(next_dir, "/");
            strcat(next_dir, d->d_name);
            //cout << "\n going to search : " << next_dir << endl;
            recursive_ff(filename, root, next_dir);
        }
        //if find a file
        //check if contains filename
        if(d->d_type == DT_REG) 
        {
            cout << "\n" << d->d_name<<" is a file" << endl;
            //if filenames match
            if (strcmp(d->d_name, filename) == 0)
            {
                string root_directory;
                string cwd;
                string sub;
                root_directory.assign(root);
                write(1, "\nroot is: ", 10);
                write(1, root, root_directory.size());

                char *buff;
                getcwd(buff, 1000);
                cwd.assign(buff);
                write(1,"\ncwd is: ", 9);
                write(1, buff, cwd.size());
                sub = root_directory.substr(root_directory.find_last_of("/\\")+1);
                //print to screen: sub + (cwd - root.cwd)
            }
        }
        d = readdir(dp);
    }
}



void ff(char* command)
{
    char root[1000];
    char filename[100];
    char path[200];
    
    //path = .
    strcpy(path, "."); 

    //path = pathname
    char *pch;
    pch = strtok(command," \n");
    pch = strtok(NULL, " \n");
    strcpy(filename, pch);
    if (pch = strtok(NULL, " \n")) 
        strcpy(path, pch);

    //cout << "\nfilenames is: " << filename << endl;
    //getcwd(root, 1000);
    //cout << "\ncurrent working directory: "  << root << endl;

    //set cwd to path
    //cout<<"chdir (0 = success) : "<<chdir(path)<<endl;
    //cout << "current working directory: ";
    chdir(path);
    getcwd(root, 1000);
    //cout << root << endl;
    //cout << "filename is : " << filename << endl;

    //pass in root (absolute path)
    recursive_ff(filename,root, ".");
}
*/


void ff (char *command)
{
    char root[1000];
    //line[0] is filename
    //line[1] is root

    struct dirent *d;
    DIR* dp;


    char * pch;
    vector<char*> line(10);
    pch=strtok(command," \n");
    int counter1=0;

    while(1)
    {
        pch=strtok(NULL," \n");
        if(pch == NULL)
            break;
        line[counter1]=pch;
        counter1++;
    }
    
    if (counter1 == 1)
    {  //cout<<"counter is "<<counter1<<endl;
        //strcpy(line[1], ".");
        line[1]=".";
    }

      strcpy(root, line[1]) ;
      cout<<endl;
      cout<<root<<endl;

      if (!opendir(root))
    {
        cout << "cannot open: " << root << endl;
        return;
    }
   
     dp=opendir(root); // dp point to fisrt file
    // cout<<"open"<<endl;
    queue<char*> Q;
   
  
     while(1)
    {
        d=readdir(dp);
       //  cout<<"here"<<endl;
        if(!d)// dp point to the next file
        {
            //cout << "d is NULL" << endl;
            //return;
            if(Q.empty())
            {
              //  cout<<"return"<<endl;
                return;
            }
            strcpy(root, Q.front());
            Q.pop();
           // cout<<"front is "<<Q.front()<<endl;
             //printf("dequque file is %s\n",root);
            // cout<<"dequque file is "<<root<<endl;
            dp=opendir(root);
           // printf("%d\n",dp);
            d=readdir(dp);
           // printf("%s\n",d->d_name);
        }
        //return;
        if (strcmp(d->d_name, ".") != 0 &&  strcmp(d->d_name,"..")!= 0)
        {
        //cout << "root is : "<< root << endl;
        //  return;
        char tempt[1000];
        strcpy(tempt, root);
        char* path=new char[2+strlen(tempt)+ strlen(d->d_name)];
        strcpy(path,tempt);
        strcat(path,"/");
        strcat(path,d->d_name);
      //  printf("%s\n",d->d_name);
       //   cout<<"push path is "<<path<<endl;
        if(strcmp(d->d_name,line[0])==0)
        {cout<<path<<endl;}
          //  cout<<"d_dname is "<<d->d_name<<endl;
            
        if (d->d_type == DT_DIR)
            Q.push(path);
    
       //  cout<<"front is "<<Q.front()<<endl;
        }
      //  printf("%s\n",root);
    }
    
}



void ls(char* address)
{
    DIR* dp;
    struct stat status;
    //stat(const char *pathname, struct stat *buf);
    struct dirent *d;

    write(1, "\n", 1);
    if (address == NULL)
    {
        dp = opendir(".");
    }
    else
        dp=opendir(address);

    while(d=readdir(dp))
    {
        stat(d->d_name, &status);
        if (d->d_type == DT_REG)
            write(1, "-", 1);
        else
            write(1, "d", 1);
        if(status.st_mode & S_IRUSR)
            write(1,"r",1);
        else
            write(1,"-",1);
        if(status.st_mode & S_IWUSR)
            write(1,"w",1);
        else
            write(1,"-",1);
        if(status.st_mode & S_IXUSR)
            write(1,"x",1);
        else
            write(1,"-",1);
        
        //
        if(status.st_mode & S_IRGRP)
            write(1,"r",1);
        else
            write(1,"-",1);
        if(status.st_mode & S_IWGRP)
            write(1,"w",1);
        else
            write(1,"-",1);
        if(status.st_mode & S_IXGRP)
            write(1,"x",1);
        else
            write(1,"-",1);
        
        if(status.st_mode & S_IROTH)
            write(1,"r",1);
        else
            write(1,"-",1);
        if(status.st_mode & S_IWOTH)
            write(1,"w",1);
        else
            write(1,"-",1);
        if(status.st_mode & S_IXOTH)
            write(1,"x",1);
        else
            write(1,"-",1);

        //cout << " " << d->d_name << endl;
        //write(STDOUT_FILENO, d->d_name,sizeof(d->d_name));
        int counter3=0;
        write(1, " ", 1);

        while(d->d_name[counter3]!='\0')
        {
            write(1,&d->d_name[counter3],1);
            counter3++;
        }
        write(1, "\n", 1);
    }
}


//from Prof. Nitta's example
void ResetCanonicalMode(int fd, struct termios *savedattributes){
    tcsetattr(fd, TCSANOW, savedattributes);
}

//from Prof. Nitta's example
void SetNonCanonicalMode(int fd, struct termios *savedattributes){
    struct termios TermAttributes;
    
    // Save the terminal attributes so we can restore them later. 
    tcgetattr(fd, savedattributes);
    
    // Set the funny terminal modes. 
    tcgetattr (fd, &TermAttributes);
    TermAttributes.c_lflag &= ~(ICANON | ECHO); // Clear ICANON and ECHO. 
    TermAttributes.c_cc[VMIN] = 1;
    TermAttributes.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSAFLUSH, &TermAttributes);
}

//clears white spaces for a string
string clearSpace(string s1)
{
    string s2;
    for (int i = 0; i < s1.size(); i++)
        {
            if (!isspace(s1[i]))//clear whitespaces
                s2.append(1,s1[i]);
        }
    return s2;
}

void pwd()
{
    char newbuff1[1000];
    getcwd(newbuff1, 10000);

    string string1;
    string1.assign(newbuff1);
    write(STDOUT_FILENO, "\n", 1);
    write(STDOUT_FILENO, newbuff1, string1.size());
    write(STDOUT_FILENO, "\n", 1);
}


int main(int argc, char *argv[]){
    struct termios SavedTermAttributes;
    char RXChar;
    string prompt;
    string input;       
    vector<string> history;

    history.insert(history.begin(), "yo 1");
    history.insert(history.begin(), "ya 2");
    history.insert(history.begin(), "3");
    history.insert(history.begin(), "4");
    history.insert(history.begin(), "5");
    history.insert(history.begin(), "ya 6");
    history.insert(history.begin(), "ya 7");
    history.insert(history.begin(), "ya 8");
    history.insert(history.begin(), "ya 9");
    history.insert(history.begin(), "ya 10");
    history.insert(history.begin(), "ya 11");
    history.insert(history.begin(), "ya 12");


    //while in shell
    while(1)
    {
        //format prompt
        char cwd[100];
        getcwd(cwd, 100);
        prompt.assign(cwd);
        if (prompt.size() > 16)
        {
            // position of last directory
            size_t found = prompt.find_last_of("/");
            // get directory
            string last_dir = prompt.substr(found+1); 
            prompt.assign("/.../");
            prompt.append(last_dir);
        }
        prompt.append("% ");
        
        //print prompt
        for (int i = 0; i < (prompt.size()+input.size()); i++)
            write(STDOUT_FILENO, "\b \b", 3);
        write(STDOUT_FILENO,prompt.c_str(), prompt.size());

        //reset
        int position_his = -1; //position in history reset for every cycle
        input.assign(""); // input reset for every cycle

        //command line: read in and print to screen 
        while(1)
        {
            SetNonCanonicalMode(STDIN_FILENO, &SavedTermAttributes);
            read(STDIN_FILENO, &RXChar, 1);

            //enter, store input into history
            if (RXChar == 10) 
            {
                history.insert(history.begin(), input);
                break;//command line finished
            }
            //exit
            else if(0x04 == RXChar){ // C-d
                break;
            }

            //arrows
            else if (RXChar == 27) //esc seq. for direction arrows, store result in input
            {
                //backspace to erase the original string
                for (int i = 0; i < input.size(); i++)
                    write(STDOUT_FILENO, "\b \b", 3);

                read(STDIN_FILENO, &RXChar, 1);
                read(STDIN_FILENO, &RXChar, 1);
                int size = history.size();
                if (RXChar == 65) //up arrow
                {
                    position_his++; //pre increment pos
                    if (size == 0) //no his
                        break;
                    if (size - position_his < 1) //no more his
                        break;
                    if (position_his > 9) //no more than 10 entries
                    {
                        write(STDOUT_FILENO, "\a", 1);
                        position_his--;
                    }

                    input.assign(history[position_his]);// change input
                    
                    //write result to screen
                    write(STDOUT_FILENO, input.c_str(), input.size());
                }
                else if (RXChar == 66) //down arrow
                {
                    position_his--; //decrement pos
                    if (position_his < -1) //avoid cycling in history vector
                        position_his = -1;
                    if (size == 0) //no his
                        break;
                    if (position_his < 0) //no more history
                    {
                        //erase the original input, show blank
                        //input.erase(input.begin(), input.end());
                        input.assign("");
                        write(STDOUT_FILENO, "\a", 1);
                    }

                    else
                    {
                        input.assign(history[position_his]); //change input
                        //write result to screen
                        write(STDOUT_FILENO, input.c_str(), input.size());
                    }
                }
                else //left and right arrows do nothing to the input
                    write(STDOUT_FILENO, input.c_str(), input.size());
            }

            //backspace
            else if (RXChar == 127)
            {
                if (input.size() == 0)
                    write(STDOUT_FILENO, "\a", 1);
                else
                {
                    //input.pop_back(); //delete char in input
                    input.erase(input.size()-1);
                    write(STDOUT_FILENO, "\b \b", 3); //show on screen
                }
            }
            //symbols
            else if (RXChar >= 32 && RXChar <= 126) //symbols in ASCII
            {
                //delete the whole string first
                for (int i = 0; i < input.size(); i++)
                        write(STDOUT_FILENO, "\b \b", 3);
                input.append(1, RXChar); //append new symbol to string
                //write to screen
                write(STDOUT_FILENO, input.c_str(), input.size());              
            }
            else//do nothing to input string (could be unnecessary condition)
            {
                for (int i = 0; i < input.size(); i++)
                        write(STDOUT_FILENO, "\b \b", 3);
                write(STDOUT_FILENO, input.c_str(), input.size());
            }
        }
        ResetCanonicalMode(STDIN_FILENO, &SavedTermAttributes);

        //empty
        if (input.empty())
        {
            write(STDOUT_FILENO,"\n", 1);
            write(STDOUT_FILENO,prompt.c_str(), prompt.size());
            continue;
        }

        //exit
        int pos_exit = input.find("exit");
        if (pos_exit != string::npos) // if exit entered
        {
            write(1, "\n", 1);
            break;
        }

        //cd
        int pos_cd = input.find("cd");
        if (pos_cd != string::npos) // if cd entered
        {
            //get arg
            string arg = input.substr(pos_cd+2);
            //clear white spaces in arguments
            arg = clearSpace(arg);

            //if cd only
            if(arg.empty())
            {
                chdir(getenv("HOME"));
                write(STDOUT_FILENO, "\n", 1);
            }

            //if cd + arg
            else
            {
                string error;
                error.assign("\nError changing directory.\n");
                int status;
                //
                status = chdir(arg.c_str());
                if (status == -1) //unsuccessful cd
                    write(STDOUT_FILENO, error.c_str(), error.size());
                char cwd[100];
                getcwd(cwd, 100);
                prompt.assign(cwd);
                write(STDOUT_FILENO, "\n", 1);
            }
            continue;
        }

        //pwd
        int pos_pwd = input.find("pwd");
        if (pos_pwd != string::npos) //pwd entered
        {
            //get arg
            string arg = input.substr(pos_pwd+3);
            //clear white spaces in arguments
            arg = clearSpace(arg);

            //pwd + no pipe
            if(arg.empty())
                pwd();
            //pwd + |*
            else
            {
                char *command = new char[100];
                strcpy(command, input.c_str());
                run(command);
            }   
            continue;
        }

        //ls
        int pos_ls = input.find("ls");
        if (pos_ls != string::npos) //ls entered
        {
            //get arg
            string arg = input.substr(pos_ls+2);
            //clear white spaces in arguments
            arg = clearSpace(arg);
            //ls + nothing
            if(arg.empty())
                ls(NULL);

            //ls + none-pipe
            else if(arg.find("|") == string::npos)
            {
                char *arg_c = new char[100];
                strcpy(arg_c, arg.c_str());
                ls(arg_c);
            }
            //ls + pipe
            else
            {
                char *command = new char[100];
                strcpy(command, input.c_str());
                run(command);
            }   
            continue;
        }

        int pos_ff = input.find("ff");
        if (pos_ff != string::npos) //found ff
        {
            char *command = new char[100];
            strcpy(command, input.c_str());
            //ff + no pipe
            //write(1, "\nff without pipe", 15);
            if(input.find('|') == string::npos)
            {
        
                ff(command);
            }

            //ff + |*
            else
            {
                write(1, "\nff with pipe", 15); 
                run(command);
            }   
            continue;

        }
            //write(1,"Here",4);


            //write(1,"Here",4);
            char* line[10];
            char * pch;
            char *command = new char[100];
            strcpy(command, input.c_str());

            if(input.find('|') == string::npos || input.find('<') == string::npos || input.find('>') == string::npos)
            {
                run(command);
                continue;
            }

            pch=strtok(command," \n");
            line[0]=pch;
        //cout<<"c"<<k<<"0 is "<<c1[k][0]<<endl;
            int counter1=1;
            while(pch != NULL)
           {
              pch=strtok(NULL," \n");
              line[counter1]=pch;
        //    cout<<c1[k][counter1]<<endl;
              counter1++;
           }
           line[counter1]=NULL;
           execvp(line[0],line);
    }     
    return 0;
}
