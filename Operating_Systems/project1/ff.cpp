
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

	chdir(path);
	getcwd(root, 1000);

	//pass in root -> absolute directory
	recursive_ff(filename,root);
}


//recursively open .
void recursive_ff(char* filename, char* root)
{
	DIR* dp; //directory reader
	struct dirent *d;//directory

	dp = opendir(".");
	while (d = readdir(dp))
	{
		//if find a directory
		//chdir and call recursively
		if (d->d_type == DT_DIR) 
		{
			chdir(d->d_name);
			recursive_ff(root);
		}
		//if find a file
		//check if contains filename
		if (d->d_type == DT_REG) 
		{
			//if filenames match
			if (strcmp(d->d_name, filename) == 0)
			{
				string root_directory;
				string cwd;
				string sub;
				root_directory.assign(root);
				write(1, "\nroot is: ", 10);
				write(1, root, root_directory.size());

				cwd.assign(getcwd());
				write(1,"\ncwd is: ", 9);
				write(1, getcwd(), cwd.size());


				sub = root_directory.substr(root_directory.find_last_of("/\\")+1);


				//print to screen: sub + (cwd - root.cwd)
			}
		}
	}

}
