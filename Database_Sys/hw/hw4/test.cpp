#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <pqxx/pqxx>
using namespace pqxx; 
using namespace std;



int main(int argc, char* argv[])
{
	//open databse
	string user = getenv("USER");
	string host = "/home/"+user+"/postgres";
	string connect = "dbname=postgres user="+user +" host="+host+" port=5432";
	connection C(connect.c_str());
	if (C.is_open()) {
		cout << "Opened db successfully: " << C.dbname() << endl;
	} 
	else {
		cout << "Can't open database" << endl;
		return 1;
	}
	cout << "---comment out line 30 to line 45 if loading for the first time---" << endl;
/*	work W1(C);

	string drop1 = "DROP TABLE StudentCourse;";
	string drop2 = "DROP TABLE CourseInfo;";
	string drop3 = "DROP TABLE InstrCourse;";
	string drop4 = "DROP TABLE CourseMeeting;";
	string drop5 = "DROP TABLE StudentInfo;";
	W1.exec(drop1);
	W1.exec(drop2);
	W1.exec(drop3);
	W1.exec(drop4);
	W1.exec(drop5);
	cout << "dropped all tables successfully" << endl; 
	W1.commit();
	C.disconnect();
*/
	//note! if grade is 9.9, dont take into calcualtion
	string sql = "CREATE TABLE StudentCourse(" \
		"SID INT,"\
		"CID INT," \
		"TERM INT," \
		"UNIT INT," \
		"GRADE DECIMAL(2,1),"\
		"LEVEL VARCHAR(5)," \
		"CLASS VARCHAR(5)," \
		"MAJOR VARCHAR(10)," \
		"STATUS VARCHAR(5));"; 
	work W(C); 
	W.exec(sql);
	cout << "Table StudentCourse created" << endl; 

	string sql2 = "CREATE TABLE CourseInfo(" \
		"CID INT," \
		"TERM INT," \
		"SUBJ VARCHAR(10)," \
		"CRSE INT,"\
		"SEC INT," \
		"UNIT VARCHAR(30));"; 
	W.exec(sql2);
	cout << "Table CourseInfo created" << endl; 

	string sql3 = "CREATE TABLE InstrCourse(" \
		"INSTRUCTOR VARCHAR(30)," \
		"CID INT," \
		"TERM INT);"; 
	W.exec(sql3);
	cout << "Table InstrCourse created" << endl; 

	string sql4 = "CREATE TABLE CourseMeeting(" \
		"INSTRUCTOR VARCHAR(30)," \
		"CCID INT," \
		"CTERM INT," \
		"TYPE VARCHAR(30)," \
		"MON BOOLEAN," \
		"TUE BOOLEAN," \
		"WED BOOLEAN," \
		"THU BOOLEAN," \
		"FRI BOOLEAN," \
		"TIMESTART TIME," \
		"TIMEEND TIME," \
		"BUILD VARCHAR(10),"\
		"ROOM INT," \
		"SEATNUM INT);"; 
	W.exec(sql4);
	cout << "Table CourseMeeting created" << endl; 

	string sql5 = "CREATE TABLE StudentInfo(" \
		"SID INT," \
		"SURNAME VARCHAR(30)," \
		"PREFNAME VARCHAR(30)," \
		"TERM INT,"\
		"MAJOR VARCHAR(10));"; 
	W.exec(sql5);
	cout << "Table StudentInfo created" << endl; 


	W.commit(); 
	C.disconnect();

	//get file names
	cout << "Grades directory must be in the same directory!\n" << endl;
	cout << "-----takes about 25 seconds to load------" << endl;

	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	dir = opendir ("Grades");
	while ((ent = readdir (dir)) != NULL) {
		if (strcmp(".", ent->d_name) != 0 && strcmp("..", ent->d_name) ){
			string name("Grades/");
			name = name + ent->d_name;
			filenames.push_back(name);
		}
	}
	closedir (dir);

	for (unsigned int k = 0; k < filenames.size(); k++)
	{
		/*
		if (k == 12 || k == 16){
			cout << filenames[k] << endl;
			cout << filenames[k] << endl;
			continue;
		}
		*/

		//cout << k << endl;
		ifstream inf(filenames[k].c_str());
		string line;

		vector<string> StudentCourse;
		vector<string> CourseInfo;
		vector<string> InstrCourse;
		vector<string> CourseMeeting;
		vector<string> StudentInfo;

		int linenum = 0;
		//get rid of empty line
		getline(inf,line,'\n');
		linenum++;
		while(!inf.eof())
		{
			bool flag = true;
			vector<string> field0;
			vector< vector<string> > field1;
			vector< vector<string> > field2;

			//first field, check header 
			getline(inf,line,'\n');
			linenum++;
			if(inf.eof())
				break;

			if (line.find("CID")){
				//split string
				char* ptr;
				
				getline(inf,line,'\n');
				linenum++;
				//if empty line, no course, continue to next loop
				if(line.length() <= 3)
					continue;
				ptr = strtok(const_cast<char*>(line.c_str()),"\",");
				while (ptr != NULL)
				{
					string temp(ptr);
					field0.push_back(temp);
					ptr =strtok(NULL, "\",");
				}
			}
			//get rid of empty line
			getline(inf,line,'\n');
			linenum++;
			//second field, check header
			getline(inf,line,'\n');
			linenum++;

			if(line.find("INSTRUCTOR")){
				//split string, add field_line's to field1
				vector<string> field1_line;		
				char* ptr;	
				
				getline(inf,line,'\n');
				linenum++;
				//if empty line, no meeting, continue to next loop
				if(line.length() <= 3)
					continue;
				ptr = strtok(const_cast<char*>(line.c_str()),"\"");
				while (ptr != NULL)
				{
					if (strcmp(ptr, ",") == 0){
						ptr =strtok(NULL, "\"");
						continue;
					}
					string temp(ptr);
					field1_line.push_back(temp);
					ptr =strtok(NULL, "\"");
				}
				field1.push_back(field1_line);

				//if next line not empty, iterate
				getline(inf,line,'\n');
				linenum++;
				while(line.length() > 3)
				{
					vector<string> field1_line;		
					char* ptr;	
					ptr = strtok(const_cast<char*>(line.c_str()),"\"");
					while (ptr != NULL)
					{
						if (strcmp(ptr, ",") == 0){
							ptr =strtok(NULL, "\"");
							continue;
						}
						string temp(ptr);
						field1_line.push_back(temp);
						ptr =strtok(NULL, "\"");
					}
					//if meeting info not complete, abandon FIXME
					if (field1_line.size() >= 6)
						field1.push_back(field1_line);	
					getline(inf,line,'\n');
					linenum++;
				}

				//not enough meeting info
				if(field1[0].size() < 7)
					flag = false;

				else if(field1[0].size() == 7){
					//fill blank instructor field with entry above
					for(unsigned int i = 1; i < field1.size(); i++){
						if(field1[i].size() < 7)
							field1[i].insert(field1[i].begin(), field1[i-1][0]);
					}
				}
			}
			
			//third field, check header
			getline(inf,line,'\n');
			linenum++;

			if(line.find("SEAT")){
				//split string, add field_line's to field2
				vector<string> field2_line;		
				char* ptr;	

				//if end of file, break;
				if(inf.eof())
					break;
				getline(inf,line,'\n');
				linenum++;
				//if empty line, no student, continue to next loop
				if(line.length() <= 3){
					//cout << "with no student" << endl;
					flag = false;
				}
				else {
					ptr = strtok(const_cast<char*>(line.c_str()),"\"");
					while (ptr != NULL)
					{
						if (strcmp(ptr, ",") == 0){
							ptr =strtok(NULL, "\"");
							continue;
						}
						string temp(ptr);
						field2_line.push_back(temp);
						ptr =strtok(NULL, "\"");
					}
					field2.push_back(field2_line);

					
					//check if next line is still in field2 or ""
					getline(inf,line,'\n');
					linenum++;
					while(line.length() > 3)//not empty
					{
						vector<string> field2_line;		
						char* ptr;	
						ptr = strtok(const_cast<char*>(line.c_str()),"\"");
						while (ptr != NULL)
						{
							if (strcmp(ptr, ",") == 0){
								ptr =strtok(NULL, "\"");
								continue;
							}
							string temp(ptr);
							field2_line.push_back(temp);
							ptr =strtok(NULL, "\"");
						}
						field2.push_back(field2_line);	
						getline(inf,line,'\n');
						linenum++;
					}		
				}	
			}

			if (!flag)
				continue;

			//process result and store

			for (unsigned int j = 0; j < field2.size(); j++){ 
				for(int i = 0; i < 11;i++){		
					if(i == 8){
						string temp("");
						if (strcmp(field2[j][i].c_str(),"A+") == 0)
							temp = temp + "4.0";
						else if (strcmp(field2[j][i].c_str(),"A") == 0)
							temp = temp + "4.0";
						else if (strcmp(field2[j][i].c_str(),"A-") == 0)
							temp = temp + "3.7";
						else if (strcmp(field2[j][i].c_str(),"B+") == 0)
							temp = temp + "3.3";
						else if (strcmp(field2[j][i].c_str(),"B") == 0)
							temp = temp + "3.0";
						else if (strcmp(field2[j][i].c_str(),"B-") == 0)
							temp = temp + "2.7";
						else if (strcmp(field2[j][i].c_str(),"C+") == 0)
							temp = temp + "2.3";
						else if (strcmp(field2[j][i].c_str(),"C") == 0)
							temp = temp + "2.0";
						else if (strcmp(field2[j][i].c_str(),"C-") == 0)
							temp = temp + "1.7";
						else if (strcmp(field2[j][i].c_str(),"D+") == 0)
							temp = temp + "1.3";
						else if (strcmp(field2[j][i].c_str(),"D") == 0)
							temp = temp + "1.0";
						else if (strcmp(field2[j][i].c_str(),"F") == 0)
							temp = temp + "0.0";
						else//exclude these for calculation
							temp = temp + "9.9";
						field2[j].erase(field2[j].begin()+i);
						field2[j].insert(field2[j].begin()+i, temp);
					}
				}
			}
			
			string temp_CI = "";
			temp_CI = temp_CI + "("+field0[0]+","+field0[1]+","+"'"+field0[2]+"'"+","+field0[3]+","+field0[4]+","+"'"+field0[5]+"'"+")";
			CourseInfo.push_back(temp_CI);
			//cout << temp_CI << endl;

			for (unsigned int i = 0; i < field1.size();i++){
				
				string temp_IC = "";
				//change ' in INSTRUCTOR to ''
				string instr = "";
				for(unsigned int j = 0; j < field1[i][0].size();j++){
					instr.append(1,field1[i][0][j]);
					if(field1[i][0][j] == '\'')
						instr.append(1,field1[i][0][j]);
				}

				temp_IC = temp_IC+"("+"'"+instr+"'"+","+field0[0]+","+field0[1]+")";
				InstrCourse.push_back(temp_IC);
				//cout << temp_IC << endl;
			}

			for(unsigned int i = 0; i < field1.size();i++){
				string temp_CM = "";
				//change ' in INSTRUCTOR to ''
				string instr = "";
				for(unsigned int j = 0; j < field1[i][0].size();j++){
					instr.append(1,field1[i][0][j]);
					if(field1[i][0][j] == '\'')
						instr.append(1,field1[i][0][j]);
				}
				//parse field1[3] format: 10:00 AM - 11:40 AM;
				string timestart;
				string timeend;
				size_t pos = field1[i][3].find("-");
				timestart = field1[i][3].substr(0, pos-1);
				timeend = field1[i][3].substr(pos+2);

				//parse field1[i][2] into days bool:
				string days;
				unsigned int h = 0;
				if (field1[i][2][h] == 'M') {
					h++;
					days += "'1',";
				}
				else days += "'0',";
				if (field1[i][2][h] == 'T') {
					h++;
					days += "'1',";
				}
				else days += "'0',";
				if (field1[i][2][h] == 'W') {
					h++;
					days += "'1',";
				}
				else days += "'0',";
				if (field1[i][2][h] == 'R') {
					h++;
					days += "'1',";
				}
				else days += "'0',";
				if (field1[i][2][h] == 'F') {
					h++;
					days += "'1'";
				}
				else days += "'0'";


				temp_CM = temp_CM+"("+"'"+instr+"'"+","+field0[0]+","+field0[1]+","+"'"+field1[i][1]+"'"+","+days+","+"'"+timestart+"'"+","+"'"+timeend+"'"+","+"'"+field1[i][4]+"'"+","+"'"+field1[i][5]+"'"+","+to_string(field2.size())+")";
				CourseMeeting.push_back(temp_CM);
				//cout << temp_CM << endl;
			}

			for(unsigned int i = 0; i < field2.size();i++){
				string temp_SC = "";
				string temp_SI = "";

				//if student doesnt have status, set null
				string status = field2[i][9];
				size_t pos = status.find("@");
				if (pos != string::npos)
					status = "null";
				temp_SC = temp_SC+"("+field2[i][1]+","+field0[0]+","+field0[1]+","+field2[i][5]+","+field2[i][8]+","+"'"+field2[i][4]+"'"+","+"'"+field2[i][6]+"'"+","+"'"+field2[i][7]+"'"+","+"'"+status+"'"+")";
				
				//if surname or prefname has ' in it, make it double for insertion into db
				//change ' to '' in surname
				string surname = "";
				for(unsigned int j = 0; j < field2[i][2].size();j++){
					surname.append(1,field2[i][2][j]);
					if(field2[i][2][j] == '\'')
						surname.append(1,field2[i][2][j]);
				}
				//change ' to '' in prefname
				string prefname = "";
				for(unsigned int j = 0; j < field2[i][3].size();j++){
					prefname.append(1,field2[i][3][j]);
					if(field2[i][3][j] == '\'')
						prefname.append(1,field2[i][3][j]);
				}

				temp_SI = temp_SI+"("+field2[i][1]+","+"'"+surname+"'"+","+"'"+prefname+"'"+","+field0[1]+","+"'"+field2[i][7]+"'"+")";
				//cout << "surname: " << surname << "prefname: "<< prefname << endl;
				StudentCourse.push_back(temp_SC);
				StudentInfo.push_back(temp_SI);
				//cout << temp_SC << endl;
				//cout << temp_SI << endl;
			}
		}//while !eof
		//cout << "end of file " << endl;
		//end of one file
		work W2(C); 

		if(CourseInfo.size()>0){
			string string_CourseInfo = CourseInfo[0];
			for(unsigned int i = 1; i < CourseInfo.size();i++){
				string_CourseInfo = string_CourseInfo + ","+ CourseInfo[i];
			}
			string sql_2 = "INSERT INTO CourseInfo(CID,TERM,SUBJ,CRSE,SEC,UNIT)" \
					"VALUES"+ string_CourseInfo +";";
			W2.exec(sql_2);
			//cout << "Inserted into CourseInfo" << endl; 
		}
		if(InstrCourse.size()>0){
			string string_InstrCourse = InstrCourse[0];
			for(unsigned int i = 1; i < InstrCourse.size();i++){
				string_InstrCourse = string_InstrCourse + ","+InstrCourse[i];
			}
			string sql_3 = "INSERT INTO InstrCourse(INSTRUCTOR,CID,TERM)" \
					"VALUES"+ string_InstrCourse +";";
			W2.exec(sql_3);
			//cout << "Inserted into InstrCourse" << endl; 
		}
		if(CourseMeeting.size()>0){
			string string_CourseMeeting = CourseMeeting[0];
			for(unsigned int i = 1; i < CourseMeeting.size();i++){
				string_CourseMeeting = string_CourseMeeting + "," + CourseMeeting[i];
			}
			//cout << string_CourseMeeting<< endl;
			string sql_4 = "INSERT INTO CourseMeeting(INSTRUCTOR,CCID,CTERM,TYPE,MON,TUE,WED,THU,FRI,TIMESTART,TIMEEND,BUILD,ROOM,SEATNUM)" \
					"VALUES"+ string_CourseMeeting +";";
			W2.exec(sql_4);
			//cout << "Inserted into CourseMeeting" << endl; 
		}
		if(StudentCourse.size()>0){
			string string_StudentCourse = StudentCourse[0];
			for(unsigned int i = 1; i < StudentCourse.size();i++){
				string_StudentCourse = string_StudentCourse + "," + StudentCourse[i];
			}
			string sql_1 = "INSERT INTO StudentCourse(SID,CID,TERM,UNIT,GRADE,LEVEL,CLASS,MAJOR,STATUS)" \
					"VALUES"+ string_StudentCourse +";";
			W2.exec(sql_1);
			//cout << "Inserted into StudentCourse" << endl; 
		}

		if(StudentInfo.size()>0){
			string string_StudentInfo = StudentInfo[0];
			for(unsigned int i = 1; i < StudentInfo.size();i++){
				string_StudentInfo = string_StudentInfo + "," + StudentInfo[i];
			}
			string sql_5 = "INSERT INTO StudentInfo(SID,SURNAME,PREFNAME,TERM,MAJOR)" \
					"VALUES"+ string_StudentInfo +";";
			W2.exec(sql_5);
			//cout << "Inserted into StudentInfo" << endl;
		}

		
		W2.commit(); //commit once per file
	
 		inf.close();
 		C.disconnect(); 
	}
	
	cout << "-----loaded into database successfully-----" << endl;
	return 0;
}