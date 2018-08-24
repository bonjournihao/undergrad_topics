#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <pqxx/pqxx>
#include <iomanip>
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
	cout << "-----takes about 20 seconds to display------" << endl;

	string sql1 = "select 100*A.studentnum/sum(B.studentnum) as percent, A.unit"\
					" from (select count(*) as studentnum, quarter.unitnum as unit "\
							" from (select sid, term, sum(unit) as unitnum"\
									" from studentcourse"\
							" group by sid, term)as quarter"\
						" group by quarter.unitnum)as A,"\

							" (select count(*) as studentnum, quarter.unitnum as unit" \
								" from (select sid, term, sum(unit) as unitnum"\
									" from studentcourse"\
									" group by sid, term)as quarter"\
								" group by quarter.unitnum)as B"\
							" where A.unit >0 and A.unit < 21"\
							" group by A.studentnum, A.unit"\
							" order by A.unit;";

	string sql2 = " select B.unitnum, avg(A.gpa)"\
					" from"\
						" (select sid, term, sum(grade*unit)/sum(unit) as gpa"\
						" from studentcourse"\
						" where grade < 9.9"\
						" group by sid,term having sum(unit) > 0)as A,"\
						" (select sid, term, sum(unit) as unitnum"\
						" from studentcourse"\
						" group by sid, term)as B"\
					" where A.sid = B.sid and A.term = B.term and B.unitnum > 0 and B.unitnum < 21"\
					" group by B.unitnum"\
					" order by B.unitnum;";

	string sql3 = " select C.instructor, C.grades"\
					" from"\
						" (select A.instructor as instructor, sum(B.grade*B.unit)/sum(B.unit) as grades"\
						" from"\
							" (select instructor,cid,term"\
							" from InstrCourse)as A,"\
							" (select sid,cid,term,grade,unit"\
							" from studentcourse"\
							" where grade < 9.9)as B"\
						" where A.cid = B.cid and A.term = B.term"\
						" group by instructor)as C,"\
						" (select A.instructor as instructor, sum(B.grade*B.unit)/sum(B.unit) as grades"\
						" from"\
							" (select instructor,cid,term"\
							" from InstrCourse)as A,"\
							" (select sid,cid,term,grade,unit"\
							" from studentcourse"\
							" where grade < 9.9)as B"\
						" where A.cid = B.cid and A.term = B.term"\
						" group by instructor)as D"\
					" group by C.instructor, C.grades having C.grades = MAX(D.grades) OR C.grades = MIN(D.grades)"\
					" order by C.grades;";

	string sql4 = " select instcourse.instructor, gradecourse.maxgrade, gradecourse.crse"\
					" from (select max(d.grades) as maxgrade,crse"\
						" from (select A.instructor as instructor, P.crse as crse, sum(B.grade*B.unit)/sum(B.unit) as grades"\
								" from (select instructor,cid,term"\
									" from InstrCourse)as A,"\
									" (select cid,term,grade,unit"\
									" from studentcourse"\
									" where grade < 9.9) as B,"\
									" (select cid,term,subj,crse"\
									" from courseinfo"\
									" where SUBJ like '%ABC%' AND crse > 99 and crse < 200)as P"\
								" where A.cid = B.cid and A.cid = P.cid and A.term = B.term and A.term = P.term"\
								" group by instructor, P.crse) as d"\
						" group by d.crse) gradecourse,"\

						" (select A.instructor as instructor, P.crse as crse, sum(B.grade*B.unit)/sum(B.unit) as grades"\
							" from (select instructor,cid,term"\
								" from InstrCourse)as A,"\
								" (select cid,term,grade,unit"\
								" from studentcourse"\
								" where grade < 9.9) as B,"\
								" (select cid,term,subj,crse"\
								" from courseinfo"\
								" where SUBJ like '%ABC%' AND crse > 99 and crse < 200)as P"\
							" where A.cid = B.cid and A.cid = P.cid and A.term = B.term and A.term = P.term"\
							" group by instructor, P.crse) as instcourse"\
					" where gradecourse.maxgrade = instcourse.grades"\
					" order by gradecourse.crse;";

	string sql4_2 = " select instcourse.instructor, gradecourse.mingrade, gradecourse.crse"\
					" from (select min(d.grades) as mingrade,crse"\
						" from (select A.instructor as instructor, P.crse as crse, sum(B.grade*B.unit)/sum(B.unit) as grades"\
								" from (select instructor,cid,term"\
									" from InstrCourse)as A,"\
									" (select cid,term,grade,unit"\
									" from studentcourse"\
									" where grade < 9.9) as B,"\
									" (select cid,term,subj,crse"\
									" from courseinfo"\
									" where SUBJ like '%ABC%' AND crse > 99 and crse < 200)as P"\
								" where A.cid = B.cid and A.cid = P.cid and A.term = B.term and A.term = P.term"\
								" group by instructor, P.crse) as d"\
						" group by d.crse) gradecourse,"\

						" (select A.instructor as instructor, P.crse as crse, sum(B.grade*B.unit)/sum(B.unit) as grades"\
							" from (select instructor,cid,term"\
								" from InstrCourse)as A,"\
								" (select cid,term,grade,unit"\
								" from studentcourse"\
								" where grade < 9.9) as B,"\
								" (select cid,term,subj,crse"\
								" from courseinfo"\
								" where SUBJ like '%ABC%' AND crse > 99 and crse < 200)as P"\
							" where A.cid = B.cid and A.cid = P.cid and A.term = B.term and A.term = P.term"\
							" group by instructor, P.crse) as instcourse"\
					" where gradecourse.mingrade = instcourse.grades"\
					" order by gradecourse.crse;";

	string sql5 = " Select ac.subj, ac.crse, bc.subj, bc.crse from"\
					" (Select distinct acid as cid1, bcid as cid2, aterm, bterm from"\
					" (Select acid, bcid, aterm, bterm from"\
					" (select distinct acid, bcid, ats, ate, bts, bte, aterm, bterm"\
					" from (select distinct a.ccid as acid, b.ccid as bcid, a.timestart as ats, a.timeend as ate, b.timestart as bts, b.timeend as bte, a.mon as amon, a.tue as atue, a.wed as awed, a.thu as athu, a.fri as afri, b.mon as bmon, b.tue as btue, b.wed as bwed, b.thu as bthu, b.fri as bfri, a.cterm as aterm, b.cterm as bterm"\
					" from (coursemeeting join studentcourse on coursemeeting.ccid = studentcourse.cid) as a, (coursemeeting join studentcourse on coursemeeting.ccid = studentcourse.cid) as b"\
					" where a.build = b.build and a.room = b.room and a.ccid != b.ccid and a.cterm = b.cterm and not (a.instructor = b.instructor and a.mon = b.mon and a.tue = b.tue and a.wed = b.wed and a.thu = b.thu and a.fri = b.fri and a.timestart = b.timestart and a.timeend = b.timeend and a.build = b.build and a.room = b.room)"\
					" group by acid, bcid, ats, ate, bts, bte, amon, atue, awed, athu, afri, bmon, btue, bwed, bthu, bfri, aterm, bterm)as physicaloverlap"\
					" where (amon = bmon and amon = 't') or"\
					" (atue = btue and atue = 't') or"\
					" (awed = bwed and awed = 't') or"\
					" (athu = bthu and athu = 't') or"\
					" (afri = bfri and afri = 't') ) as dayoverlap"\
					" where (ats, ate) overlaps (bts, bte)"\
					" group by acid, bcid, aterm, bterm) as d"\
					" where acid >= bcid"\
					" order by acid asc) as final,"\
					" courseinfo as ac,"\
					" courseinfo as bc"\
					" where (ac.cid = final.cid1 and aterm = ac.term and ac.term = bterm) and (bc.cid = final.cid2 and bc.term = bterm and bc.term = aterm)"\
					" group by ac.crse, bc.crse, ac.subj, bc.subj"\
					" order by ac.subj asc;";

	string sql6_1 = " Select distinct a.dmajor as majordisplay, a.sumunit/b.sumunit as avgrade"\
					" from (select distinct major as dmajor, sum(a.unit * a.grade) as sumunit from studentcourse as a, courseinfo as b where grade != 9.9 and a.cid = b.cid and b.subj = 'ABC' group by major)  as a,"\
					" (select distinct major as dmajor, sum(a.unit) as sumunit from studentcourse as a, courseinfo as b where grade != 9.9 and a.cid = b.cid and b.subj = 'ABC' group by major) as b"\
					" where a.dmajor = b.dmajor and b.sumunit != 0"\
					" group by majordisplay, a.sumunit, b.sumunit"\
					" order by avgrade desc"\
					" limit 1;";
	string sql6_2 = " Select distinct a.dmajor as majordisplay, a.sumunit/b.sumunit as avgrade"\
					" from (select distinct major as dmajor, sum(a.unit * a.grade) as sumunit from studentcourse as a, courseinfo as b where grade != 9.9 and a.cid = b.cid and b.subj = 'ABC' group by major)  as a,"\
					" (select distinct major as dmajor, sum(a.unit) as sumunit from studentcourse as a, courseinfo as b where grade != 9.9 and a.cid = b.cid and b.subj = 'ABC' group by major) as b"\
					" where a.dmajor = b.dmajor and b.sumunit != 0"\
					" group by majordisplay, a.sumunit, b.sumunit"\
					" order by avgrade asc"\
					" limit 1;";
	string sql7_1 = " select 100.0*count(distinct a.sid)/(select count(distinct sid) from studentinfo) as percentage from studentinfo as a, studentinfo as b"\
					" where b.term > a.term and b.major like 'ABC%' and a.major not like 'ABC%' and a.sid = b.sid;";

	string sql7_2 = " select amajor, 100.0*count(*)/ (select count(*)"\
					" from (select distinct a.sid as studentid, a.major as amajor"\
					" from studentinfo as a, studentinfo as b"\
					" where b.term > a.term and b.major like 'ABC%' and a.major not like 'ABC%' and a.sid = b.sid group by a.sid, a.major)"\
					" as b) as totaltrans"\
					" from (select distinct a.sid as studentid, a.major as amajor"\
					" from studentinfo as a, studentinfo as b"\
					" where b.term > a.term and b.major like 'ABC%' and a.major not like 'ABC%' and a.sid = b.sid group by a.sid, a.major)"\
					" as a"\
					" group by amajor"\
					" order by totaltrans desc"\
					" limit 5;";

	string xtr5a_203_75 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '203') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '203') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 75 or result.percent = 75;";

	string xtr5a_203_80 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '203') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '203') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 80 or result.percent = 80;";

	string xtr5a_203_85 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '203') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '203') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 85 or result.percent = 85;";

	string xtr5a_203_90 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '203') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '203') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 90 or result.percent = 90;";
	string xtr5a_203_95 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '203') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '203') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 95 or result.percent = 95;";

	//ABC210
	string xtr5a_210_75 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '210') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '210') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 75 or result.percent = 75;";

	string xtr5a_210_80 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '210') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '210') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 80 or result.percent = 80;";

	string xtr5a_210_85 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '210') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '210') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 85 or result.percent = 85;";

	string xtr5a_210_90 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '210') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '210') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 90 or result.percent = 90;";
	string xtr5a_210_95 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '210') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '210') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 95 or result.percent = 95;";

	//ABC222
	string xtr5a_222_75 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '222') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '222') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 75 or result.percent = 75;";

	string xtr5a_222_80 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '222') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '222') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 80 or result.percent = 80;";

	string xtr5a_222_85 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '222') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '222') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 85 or result.percent = 85;";

	string xtr5a_222_90 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '222') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '222') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 90 or result.percent = 90;";
	string xtr5a_222_95 = " select result.subj, result.crse"\
							" from (select 100*X.num/stt.studnum as percent, X.subj as subj, X.crse as crse"\
								" from (select count(*) as num, stc.subj as subj, stc.crse as crse"\
									" from (select B.sid as sid, B.term as term"\
										" from (select cid, term"\
												" from courseinfo"\
												" where subj = 'ABC' and crse = '222') as A,"\
												" studentcourse as B"\
										" where B.CID = A.CID AND B.TERM = A.TERM"\
										" group by B.sid, B.term)as stt,"\
										" (select A.sid, A.term, B.subj, B.crse"\
										" from studentcourse as A,"\
											" courseinfo as B"\
										" where A.cid = B.cid AND A.term = B.term) as stc"\
									" where stt.sid = stc.sid and stt.term > stc.term"\
									" group by stc.subj, stc.crse)as X,"\

									" (select count(*) as studnum"\
									" from (select cid, term, subj, crse"\
											" from courseinfo"\
											" where subj = 'ABC' and crse = '222') as A,"\
										" studentcourse as B"\
									" where B.CID = A.CID AND B.TERM = A.TERM"\
									" group by A.subj, A.crse)as stt"\
								" order by percent)as result"\
							" where result.percent > 95 or result.percent = 95;";

	string xtr5b_1 = " select totalroom.build, totalroom.room, max(totalroom.seatnum)"\
					" from ((select build, room, max(seatnum) as seatnum"\
						" from coursemeeting"\
						" group by build, room)"\
						" union"\
						" (select c.build, c.room, sum(c.seatnum) as seatnum"\
						" from (select A.ccid as cid, A.cterm as term, subj, crse, sec, unit, type, mon, tue, wed, thu, fri, sat, timestart, timeend, build, room, seatnum"\
							" from coursemeeting as A, courseinfo as B where A.ccid = B.cid and A.cterm = B.term)as C,"\
							" (select A.ccid as cid, A.cterm as term, subj,crse, sec, unit, type, mon, tue, wed, thu, fri, sat, timestart, timeend, build, room, seatnum"\
							" from coursemeeting as A, courseinfo as B where A.ccid = B.cid and A.cterm = B.term)as D"\
						" where C.cid < D.cid and C.sec < D.sec and C.term = D.term and C.subj = D.subj and C.crse = D.crse and C.unit = D.unit and C.type = D.type and C.mon = D.mon and C.tue = D.tue and C.wed = D.wed and C.thu = D.thu and C.fri = D.fri and C.sat = D.sat and C.timestart = D.timestart and C.timeend = D.timeend and C.build = D.build and C.room = D.room"\
						" group by c.term, c.subj, c.crse, c.unit, c.type, c.build, c.room)"\
						" union"\
						" (select c.build as build, c.room as room, sum(c.seatnum) as seatnum"\
						" from (select A.ccid as cid, A.cterm as term, subj, crse, sec, unit, type, mon, tue, wed, thu, fri, sat, timestart, timeend, build, room, seatnum"\
							" from coursemeeting as A, courseinfo as B where A.ccid = B.cid and A.cterm = B.term)as C,"\
							" (select A.ccid as cid, A.cterm as term, subj,crse, sec, unit, type, mon, tue, wed, thu, fri, sat, timestart, timeend, build, room, seatnum"\
							" from coursemeeting as A, courseinfo as B where A.ccid = B.cid and A.cterm = B.term)as D"\
						" where C.cid < D.cid and C.sec < D.sec and C.subj < D.subj and C.crse < D.crse and C.term = D.term  and C.unit = D.unit and C.type = D.type and C.mon = D.mon and C.tue = D.tue and C.wed = D.wed and C.thu = D.thu and C.fri = D.fri and C.sat = D.sat and C.timestart = D.timestart and C.timeend = D.timeend and C.build = D.build and C.room = D.room"\
						" group by c.term, c.unit, c.type, c.build, c.room)) as totalroom"\
						" group by totalroom.build, totalroom.room;";


	nontransaction N(C);

	result R1(N.exec(sql1));
	result R2(N.exec(sql2));
	result R3(N.exec(sql3));

	result R4(N.exec(sql4));
	result R4_2(N.exec(sql4_2));

	result R5(N.exec(sql5));
	result R6_1(N.exec(sql6_1));
	result R6_2(N.exec(sql6_2));
	result R7_1(N.exec(sql7_1));
	result R7_2(N.exec(sql7_2));

	//extra credit
	result R_203_75(N.exec(xtr5a_203_75));
	result R_203_80(N.exec(xtr5a_203_80));
	result R_203_85(N.exec(xtr5a_203_85));
	result R_203_90(N.exec(xtr5a_203_90));
	result R_203_95(N.exec(xtr5a_203_95));

	result R_210_75(N.exec(xtr5a_210_75));
	result R_210_80(N.exec(xtr5a_210_80));
	result R_210_85(N.exec(xtr5a_210_85));
	result R_210_90(N.exec(xtr5a_210_90));
	result R_210_95(N.exec(xtr5a_210_95));

	result R_222_75(N.exec(xtr5a_222_75));
	result R_222_80(N.exec(xtr5a_222_80));
	result R_222_85(N.exec(xtr5a_222_85));
	result R_222_90(N.exec(xtr5a_222_90));
	result R_222_95(N.exec(xtr5a_222_95));

	result R_5b_1(N.exec(xtr5b_1));
	

	cout << setw(10) << left <<"Unit" <<  "Percentage" << endl;
	for (result::const_iterator c = R1.begin(); c != R1.end(); ++c) {
		cout << setw(10) << left <<c[1].as<int>() << c[0].as<string>() << endl;
	}
	cout << endl<<"3a done successfully" << endl << endl;
	cout << "____________________________________" << endl;

	cout << setw(10) << left <<"Unit" <<  "Average GPA" << endl;
	for (result::const_iterator c = R2.begin(); c != R2.end(); ++c) {
		cout << setw(10) << left << c[0].as<int>() << c[1].as<float>() << endl;
	}
	cout << endl<<"3b done successfully" << endl << endl;
	cout << "____________________________________" << endl;

	cout << setw(25) << left <<"Instructor" <<  "Grades" << endl;
	for (result::const_iterator c = R3.begin(); c != R3.end(); ++c) {
		cout << setw(25) << left << c[0].as<string>() <<c[1].as<float>() << endl;
	}
	cout << endl<<"easiet and hardest instructors listed together (above)" << endl;
	cout << "3c done successfully" << endl << endl;
	cout << "____________________________________" << endl;

	cout << setw(25) << left <<"Hardest Instructor"<< setw(25) << left <<  "Average Grade"  << "Course"<< endl;
	for (result::const_iterator c = R4.begin(); c != R4.end(); ++c) {
		cout << setw(25) << left << c[0].as<string>()<< setw(25) << left << c[1].as<float>() << c[2].as<int>()<< endl;
	}
	cout << endl << setw(25) << left <<"Easiet Instructor"<< setw(25) << left <<  "Average Grade"  << "Course"<< endl;
	for (result::const_iterator c = R4_2.begin(); c != R4_2.end(); ++c) {
		cout << setw(25) << left << c[0].as<string>()<< setw(25) << left << c[1].as<float>() << c[2].as<int>()<< endl;
	}

	cout <<endl<< "3d done successfully" << endl << endl;
	cout << "____________________________________" << endl;

	cout << setw(10) << left << "subj 1"<< setw(10) << left << "crse 1"<< setw(10) << left << "subj 2"<< setw(10) << left <<  "crse 2" << endl;
	for (result::const_iterator c = R5.begin(); c != R5.end(); ++c) {
		cout << setw(10) << left << c[0].as<string>()<< setw(10) << left << c[1].as<int>()<< setw(10) << left<< c[2].as<string>()<< setw(10) << left<< c[3].as<int>() << endl;
	}
	cout << endl<<"3e done successfully" << endl << endl;
	cout << "____________________________________" << endl;

	cout << setw(20) << left <<"Best Major" <<  "Average GPA" << endl;
	for (result::const_iterator c = R6_1.begin(); c != R6_1.end(); ++c) {
		cout << setw(20) << left << c[0].as<string>() << c[1].as<float>() << endl;
	}
	cout << setw(20) << left <<"Worst Major" <<  "Average GPA" << endl;
	for (result::const_iterator c = R6_2.begin(); c != R6_2.end(); ++c) {
		cout << setw(20) << left << c[0].as<string>() << c[1].as<float>() << endl;
	}

	cout << endl <<"3f done successfully" << endl << endl;
	cout << "____________________________________" << endl;


	cout <<"Percentage Transfer" << endl;
	for (result::const_iterator c = R7_1.begin(); c != R7_1.end(); ++c) {
		cout << c[0].as<float>() << endl << endl;
	}

	cout << setw(20) << left <<"Top Major" <<  "Major Percentage" << endl;
	for (result::const_iterator c = R7_2.begin(); c != R7_2.end(); ++c) {
		cout << setw(20) << left << c[0].as<string>() << c[1].as<float>() << endl;
	}
	cout << endl <<  "3g done successfully" << endl << endl;
	cout << "____________________________________" << endl;

	cout << "EXTRA CREDIT!" << endl;

	//ABC2O3
	cout << endl << "Before ABC203, 75% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_203_75.begin(); c != R_203_75.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC203, 80% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_203_80.begin(); c != R_203_80.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC203, 85% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_203_85.begin(); c != R_203_85.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC203, 90% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_203_90.begin(); c != R_203_90.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC203, 95% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_203_95.begin(); c != R_203_95.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	//ABC210
	cout << endl << "Before ABC210, 75% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_210_75.begin(); c != R_210_75.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC210, 80% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_210_80.begin(); c != R_210_80.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC210, 85% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_210_85.begin(); c != R_210_85.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC210, 90% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_210_90.begin(); c != R_210_90.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC210, 95% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_210_95.begin(); c != R_210_95.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	//ABC222
	cout << endl << "Before ABC222, 75% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_222_75.begin(); c != R_222_75.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC222, 80% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_222_80.begin(); c != R_222_80.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC222, 85% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_222_85.begin(); c != R_222_85.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC222, 90% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_222_90.begin(); c != R_222_90.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl << "Before ABC222, 95% or more have taken courses: " << endl;
	cout << setw(10) << left <<"subj" <<  "crse" << endl;
	for (result::const_iterator c = R_222_95.begin(); c != R_222_95.end(); ++c) {
		cout << setw(10) << left <<c[0].as<string>() << c[1].as<int>() << endl;
	}

	cout << endl<<"5a done successfully" << endl << endl;
	cout << "____________________________________" << endl;



	cout << setw(15) << left <<"Building" << setw(15) << left <<  "Room"  << "SeatNum"<< endl;
	for (result::const_iterator c = R_5b_1.begin(); c != R_5b_1.end(); ++c) {
		cout << setw(15) << left <<c[0].as<string>()<< setw(15) << left << c[1].as<int>() << c[2].as<int>()<< endl;
	}

	cout << endl<<"5b room capacities" << endl << endl;
	cout << "____________________________________" << endl;


	C.disconnect();
}



