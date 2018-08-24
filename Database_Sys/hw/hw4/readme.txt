-----This writing up is for running "load.out" and "query.out"-----

1) make

2) "load.out" assumes "Grade" folder is in the same directory, the reads filenames from the directory.

3) (should work fine) "load.out" and "query.out"'s conection to server is set as: 
	-- host = "/home/$USER/postgres"
	-- database name = postgres
	-- port=5432

Note: For testing purpose, line 30 though line 45 has been commented out. (If need to load for multiple times, delete "/*" and "*/" to enable the program to drop all existing tables.)


