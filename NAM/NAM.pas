var s, st: string;
	pr: array[1..100, 1..2] of string;
	fin: text;
	i, temp, n: longint;
	flag: boolean;

function pst(s1:string; var s2:string):longint;
	begin
		if s1='' then exit(1)
					else exit(pos(s1, s2));
	end;

begin
	assign(fin, 'NAM.prog');
	reset(fin);
	readln(fin, n);
	for i:=1 to n do begin
		readln(fin, st);
		temp:=pst('->', st);
		pr[i, 1]:=copy(st, 1, temp-1);
		pr[i, 2]:=copy(st, temp+2, length(st)-temp-1);
		end;
	close(fin);
{	writeln('Program:');
	for i:=1 to n do
		writeln('"', pr[i, 1], '" -> "', pr[i, 2], '"');}
	readln(s);
	while true do begin
		flag:=true;
		for i:=1 to n do begin
			temp:=pst(pr[i, 1], s);
			if temp>0 then begin
				delete(s, temp, length(pr[i, 1]));
				insert(pr[i, 2], s, temp);
				flag:=false;
				break;
				end;
			end;
		writeln(s);
		if flag then break;
		end;
end.