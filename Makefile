servl:bf_tcp_server.o search.o addSQL.o Sch.o addinfo.o
	g++ -std=c++11 -o servl bf_tcp_server.o search.o addSQL.o Sch.o addinfo.o -lsqlite3
bf_tcp_server.o:bf_tcp_server.cpp search.cpp addSQL.cpp Sch.cpp addinfo.o
	g++ -std=c++11 -c bf_tcp_server.cpp Sch.cpp addinfo.cpp -lsqlite3
search.o:search.cpp
	g++ -std=c++11 -c search.cpp -lsqlite3
addSQL.o:addSQL.cpp Sch.cpp
	g++ -std=c++11 -c addSQL.cpp Sch.cpp -lsqlite3
Sch.o:Sch.cpp
	g++ -std=c++11 -c Sch.cpp -lsqlite3 
addinfo.o:addinfo.cpp
	g++ -std=c++11 -c addinfo.cpp -lsqlite3 

clean:
	rm bf_tcp_server.o addSQL.o search.o Sch.o addinfo.o	

