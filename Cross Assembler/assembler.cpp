# include <string.h>
# include <cstring>
# include <iostream>
# include <fstream> 
# include <stdio.h>
# include <vector> 
# include <sstream>
# include<cstdio>
# include<cmath>
#include <iomanip>
//SIC_input_get (1).txt
//SICXE_input.txt
//SIC_input.txt
using namespace std;
using std::string; using std::hex;
using std::stringstream;
int ver = 1;
struct value{
	string line ;
	int tableNum ;
	int num;
};

struct data{
	string symbol;
	string addr;
};

struct token_group{
	string label;
	string instr;
	string op1;
	string op2;
	int format ;
	string comment;
	bool hascomment;
};

struct instr_set{
	string name;
	int format;
	string opcode;
	bool sicxe ;
};

void openFile( ifstream & file, string &fileName ) { 
    // 打開文件
    string temp;
	cout << "Input a file number [0: quit] :\n" ;
	getline( cin, fileName );
	if ( fileName.compare( "0" ) == 0 ) return ;
	file.open( fileName.c_str() ) ;
	
 	while ( !file ) {
		cout << "### " << fileName.c_str() << " does not exist! ###\n" ;
		openFile( file, fileName ) ;
	} // while
	
	cout << "1:SICXE 2:SIC" << endl;
	cin >> temp;
	if ( temp.compare("1") == 0 ) ver = 1;
	else if ( temp.compare( "2" ) == 0 ) ver = 2;
	else {
		while ( temp.compare("1") != 0 && temp.compare( "2" ) != 0 ) {
			cout << "1:SICXE 2:SIC" << endl;
			cin >> temp;
		}
	}
}  

void clear( value &temp ) {
	temp.line = "";
	temp.tableNum = 0;
	temp.num = 0 ;
}

void clear( token_group &temp ) {
	temp.instr = "";
	temp.label = "";
	temp.op1 = "" ;
	temp.op2 = "" ;
	temp.comment = "" ;
	temp.format = 0;
	temp.hascomment = false;
}

bool isSpace ( char c ) {
	if ( c == ' ' || c == '\t' || c == '\n' || c == '\0' ) return true;
	else return false;
}

void value_insert( vector<value> &list, string line, int table_num, int num ) {
	value temp ;
	temp.line = line;
	temp.tableNum = table_num;
	temp.num = num;
	list.push_back( temp ) ;
}

class Instruction{
	vector<instr_set> table1 ;
	public:
	Instruction(){
		instr_set str ;
		ifstream file ;
		file.open( "Table1.table" ) ;
		while ( file >> str.name ){
			instruction_set( str.name, str ) ;
			table1.push_back( str ) ;
		}
		
		file.close();
	}
	
	void instruction_set( string str, instr_set & list ) {
		if ( strcasecmp( str.c_str(), "add" ) == 0 ) {
			list.format = 3;
			list.opcode = "18" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "addf" ) == 0 ) {
			list.format = 3;
			list.opcode = "58" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "addr" ) == 0 ) {
			list.format = 2;
			list.opcode = "90" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "and" ) == 0 ) {
			list.format = 3;
			list.opcode = "40" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "clear" ) == 0 ) {
			list.format = 2;
			list.opcode = "B4" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "comp" ) == 0 ) {
			list.format = 3;
			list.opcode = "28" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "compf" ) == 0 ) {
			list.format = 3;
			list.opcode = "88" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "compr" ) == 0 ) {
			list.format = 2;
			list.opcode = "A0" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "div" ) == 0 ) {
			list.format = 3;
			list.opcode = "24" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "divf" ) == 0 ) {
			list.format = 3;
			list.opcode = "64" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "divr" ) == 0 ) {
			list.format = 2;
			list.opcode = "9C" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "fix" ) == 0 ) {
			list.format = 1;
			list.opcode = "C4" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "float" ) == 0 ) {
			list.format = 1;
			list.opcode = "C0" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "hio" ) == 0 ) {
			list.format = 1;
			list.opcode = "F4" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "j" ) == 0 ) {
			list.format = 3;
			list.opcode = "3C" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "jeq" ) == 0 ) {
			list.format = 3;
			list.opcode = "30" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "jgt" ) == 0 ) {
			list.format = 3;
			list.opcode = "34" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "jlt" ) == 0 ) {
			list.format = 3;
			list.opcode = "38" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "jsub" ) == 0 ) {
			list.format = 3;
			list.opcode = "48" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "lda" ) == 0 ) {
			list.format = 3;
			list.opcode = "00" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "ldb" ) == 0 ) {
			list.format = 3;
			list.opcode = "68" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "ldch" ) == 0 ) {
			list.format = 3;
			list.opcode = "50" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "ldf" ) == 0 ) {
			list.format = 3;
			list.opcode = "70" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "ldl" ) == 0 ) {
			list.format = 3;
			list.opcode = "08" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "lds" ) == 0 ) {
			list.format = 3;
			list.opcode = "6C" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "ldt" ) == 0 ) {
			list.format = 3;
			list.opcode = "74" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "ldx" ) == 0 ) {
			list.format = 3;
			list.opcode = "04" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "lps" ) == 0 ) {
			list.format = 3;
			list.opcode = "D0" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "mul" ) == 0 ) {
			list.format = 3;
			list.opcode = "20" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "mulf" ) == 0 ) {
			list.format = 3;
			list.opcode = "60" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "mulr" ) == 0 ) {
			list.format = 2;
			list.opcode = "98" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "norm" ) == 0 ) {
			list.format = 1;
			list.opcode = "C8" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "or" ) == 0 ) {
			list.format = 3;
			list.opcode = "44" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "rd" ) == 0 ) {
			list.format = 3;
			list.opcode = "D8" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "rmo" ) == 0 ) {
			list.format = 2;
			list.opcode = "AC" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "rsub" ) == 0 ) {
			list.format = 3;
			list.opcode = "4C" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "shiftl" ) == 0 ) {
			list.format = 2;
			list.opcode = "A4" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "shiftr" ) == 0 ) {
			list.format = 2;
			list.opcode = "A8" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "sio" ) == 0 ) {
			list.format = 1;
			list.opcode = "F0" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "ssk" ) == 0 ) {
			list.format = 3;
			list.opcode = "EC" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "sta" ) == 0 ) {
			list.format = 3;
			list.opcode = "0C" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "stb" ) == 0 ) {
			list.format = 3;
			list.opcode = "78" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "stch" ) == 0 ) {
			list.format = 3;
			list.opcode = "54" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "stf" ) == 0 ) {
			list.format = 3;
			list.opcode = "80" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "sti" ) == 0 ) {
			list.format = 3;
			list.opcode = "D4" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "stl" ) == 0 ) {
			list.format = 3;
			list.opcode = "14" ;
			list.sicxe = 0;
		}
		
		else if ( strcasecmp( str.c_str(), "sts" ) == 0 ) {
			list.format = 3;
			list.opcode = "7C" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "stsw" ) == 0 ) {
			list.format = 3;
			list.opcode = "E8" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "stt" ) == 0 ) {
			list.format = 3;
			list.opcode = "84" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "stx" ) == 0 ) {
			list.format = 3;
			list.opcode = "10" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "sub" ) == 0 ) {
			list.format = 3;
			list.opcode = "1C" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "subf" ) == 0 ) {
			list.format = 3;
			list.opcode = "5C" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "subr" ) == 0 ) {
			list.format = 2;
			list.opcode = "94" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "svc" ) == 0 ) {
			list.format = 2;
			list.opcode = "B0" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "td" ) == 0 ) {
			list.format = 3;
			list.opcode = "E0" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "tio" ) == 0 ) {
			list.format = 1;
			list.opcode = "F8" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "tix" ) == 0 ) {
			list.format = 3;
			list.opcode = "2C" ;
			list.sicxe = 0;
		}
		else if ( strcasecmp( str.c_str(), "tixr" ) == 0 ) {
			list.format = 2;
			list.opcode = "B8" ;
			list.sicxe = 1;
		}
		else if ( strcasecmp( str.c_str(), "wd" ) == 0 ) {
			list.format = 3;
			list.opcode = "DC" ;
			list.sicxe = 0;
		}
	}

	bool isInstr ( string str, string line, value & temp, int & format ) {
		for ( int i = 0 ; i < table1.size() ; i++ ) {
			if ( strcasecmp( str.c_str(), table1[i].name.c_str() ) == 0 ) {
				temp.line = line;
				temp.tableNum = 1;
				temp.num = i+1;
				format = table1[i].format;
				return true;
			}
		}
		
		return false;
	}
};

class Pseudo{
	vector<string> table2 ;
	public:
	Pseudo(){
		string str ;
		ifstream file ;
		file.open( "Table2.table" ) ;
		while ( file >> str ){
			table2.push_back( str ) ;
		}
		
		file.close();
	}
	
	bool isPseudo ( string str, string line, value & temp ) {
		for ( int i = 0 ; i < table2.size() ; i++ ) {
			if ( strcasecmp( str.c_str(), table2[i].c_str() ) == 0 ) { 
				temp.line = line;
				temp.tableNum = 2;
				temp.num = i+1;
				return true;
			}
		}
		
		return false;
	}
};

class Register{
	vector<string> table3 ;
	public:
	Register(){
		string str ;
		ifstream file ;
		file.open( "Table3.table" ) ;
		while ( file >> str ){
			table3.push_back( str ) ;
		}
		
		file.close();
	}
	
	bool isReg ( string str, string line, value & temp ) {
		for ( int i = 0 ; i < table3.size() ; i++ ) {
			if ( strcasecmp( str.c_str(), table3[i].c_str() ) == 0 ) {
				temp.line = line;
				temp.tableNum = 3;
				temp.num = i;
				return true;
			}
		}
		
		return false;
	}
};

class Delimiter{
	vector<string> table4 ;
	public:
	Delimiter(){
		ifstream file ;
		string str;
		file.open( "Table4.table" ) ;
		while ( file >> str ){
			table4.push_back( str ) ;
		}
		
		file.close();
	}
	
	bool isDelimiter ( char c ) {
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( c == table4[i][0] )	return true;
		}
		
		return false;
	}
	
	bool isDelimiter ( string str, string line, value & temp ) {
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( str.compare( table4[i] ) == 0 ) {
				temp.line = line;
				temp.tableNum = 4;
				temp.num = i+1;
				return true;
			}
		}
		
		return false;
	}
};

class Symbol{
	public:
	data table5[100] ;
	
	int calculate( string str ) {
		int sum = 0 ;
		for ( int i = 0 ; i < str.length() ; i++ ) {
			sum = sum + str[i];
		}
		
		return sum ;
	}
	
	void hash( int num, string str, string line, vector<value> &list, bool & dup ){
		for ( int i = num%100 ; i < 100 ; i ++ ) {
			if ( table5[i].symbol.empty() || table5[i].symbol.compare( str ) == 0 ) {
				table5[i].symbol = str ;
				if ( table5[i].addr != "" && line != "" ) {
					dup = true;
					return;
				}
				else dup = false; 
				if ( line != "" )  table5[i].addr = line ;
				value_insert( list, line, 5, i );
				return ;
			}
			else if ( i == 99 ) i = -1 ; 
		}
	}
	
	void check( vector<string> &undefine ) {
		for ( int i = 0 ; i < 100 ; i ++ ) {
			if ( table5[i].symbol != "" && table5[i].addr == "" ) undefine.push_back( table5[i].symbol ) ;
		}
	}
	
	string address( string str ) {
		int index = calculate( str ) ;
		while ( table5[index].symbol.compare( str ) != 0 ) index ++ ;
		return table5[index].addr;
	}
};

class Integer{
	
	public:
	string table6[100] ;
	bool isInteger ( string str, string line, value & temp, int & sum ) {
		sum = 0;
		for ( int i = 0 ; i < str.size() ; i++ ) {
			if ( ( str[i] >= '0' && str[i] <= '9' ) || ( str[i] >= 'A' && str[i] <= 'F' ) ) {
				temp.line = line;
				temp.tableNum = 6;
				temp.num = i+1;
				sum = sum + str[i];
			}
			else return false;
		}
		
		return true;
	}
	
	void hash( int num, string str, string line, vector<value> &list ){
		for ( int i = num%100 ; i < 100 ; i ++ ) {
			if ( table6[i].empty() ) {
				table6[i] = str ;
				value_insert( list, line, 6, i );
				return ;
			}
			if ( i == 99 ) i = -1 ; 
		}
	}
};

class StringTable{
	
	public:
	string table7[100] ;
	int calculate( string str ) {
		int sum = 0 ;
		for ( int i = 0 ; i < str.length() ; i++ ) {
			if ( str[i] != '\'' && str[i] != 'C' && str[i] != 'c' && str[i] != 'X' && str[i] != 'x' )sum = sum + str[i];
		}
		
		return sum ;
	}
	
	void hash( int num, string str, string line, vector<value> &list ){
		for ( int i = num%100 ; i < 100 ; i ++ ) {
			if ( table7[i].empty() ) {
				table7[i] = str ;
				value_insert( list, line, 7, i );
				return ;
			}
			if ( i == 99 ) i = -1 ; 
		}
	}
};

class literal{
	public:
	vector <int> table8;
};

Instruction table1;
Pseudo table2;
Register table3;
Delimiter table4;
Symbol table5;
Integer table6;
StringTable table7;
literal table8;
data base;
int baseIndex;

string hex_add( string str, int num ) {
	string result ;
	int str_dec = 0;
	for ( int i = 0 ; i < str.length() ; i++ ) {
		if ( str[i] >= '0' && str[i] <= '9' ) str_dec = ( str[i] - '0' ) + str_dec * 16 ;
		else str_dec = ( str[i] - 'A' + 10 ) + str_dec * 16 ;
	}
	
	str_dec = str_dec + num;
	int temp = 0;
	while (str_dec != 0) {
		temp = str_dec % 16 ;
		if ( temp >= 10 ) result.insert( result.begin(), temp - 10 + 'A' ) ;
		else result.insert( result.begin(), temp + '0' ) ;
		str_dec /= 16;
	}
	
	return result;
}

bool hex_com( string str1, string str2 ){
	int int1 = 0, int2 = 0;
	for ( int i = 0 ; i < str1.length() ; i++ ) {
		if ( str1[i] >= '0' && str1[i] <= '9' ) int1 = ( str1[i] - '0' ) + int1 * 16 ;
		else int1 = ( str1[i] - 'A' + 10 ) + int1 * 16 ;
	}
	
	for ( int i = 0 ; i < str2.length() ; i++ ) {
		if ( str2[i] >= '0' && str2[i] <= '9' ) int2 = ( str2[i] - '0' ) + int2 * 16 ;
		else int2 = ( str2[i] - 'A' + 10 ) + int2 * 16 ;
	}
	
	if ( int1 >= int2 ) return 1;
	else return false;
}

string hex_sub( string a, string b ) {
	string result;
	vector<int> binary__a ;
	vector<int> binary__b ;
	int dec_b = 0, dec_a = 0, temp = 0;
	if ( hex_com( a, b ) == 1 ) { // a>b
		for ( int i = 0 ; i < b.length() ; i++ ) {
			if ( b[i] >= '0' && b[i] <= '9' ) dec_b = ( b[i] - '0' ) + dec_b * 16 ;
			else dec_b = ( b[i] - 'A' + 10 ) + dec_b * 16 ;
		}
		
		result = hex_add( a, 0-dec_b ) ;
	}
	else {
		for ( int i = 0 ; i < a.length() ; i++ ) {
			if ( a[i] >= '0' && a[i] <= '9' ) dec_a = ( a[i] - '0' ) + dec_a * 16 ;
			else dec_a = ( a[i] - 'A' + 10 ) + dec_a * 16 ;
		}
		
		for ( int i = 0 ; i < b.length() ; i++ ) {
			if ( b[i] >= '0' && b[i] <= '9' ) dec_b = ( b[i] - '0' ) + dec_b * 16 ;
			else dec_b = ( b[i] - 'A' + 10 ) + dec_b * 16 ;
		}
		
		temp = dec_a - dec_b;
		//cout << dec_a <<" " <<dec_b <<endl;
 		if ( temp < -4096 ) {
 			result = "" ;
 			return result;
		 }
		 
		temp = abs(temp) ;
		result = hex_add( "0", temp-1 ) ;
		for ( int i = 0 ; i < result.length() ; i++ ) {
			if ( result[i] >= 'A' && result[i] <= 'F' ) temp = result[i] - 'A' + 10 ;
			else temp = result[i] - '0' ;
			temp = 15 - temp ;
			if ( temp >= 10 ) result[i] = temp - 10 + 'A' ;
			else result[i] = temp + '0';
		}
		
		return result;
	}
}

void token( string line, vector<string> & token_list, int &comment_num, string &comment ) {
	vector<char> input;
	string str;
	value temp;
	int num = 0;
	for ( int i = 0 ; i < line.length() ; i ++ ) {
		clear( temp ) ;
		num = 0;
		if ( table4.isDelimiter( line[i] ) || isSpace( line[i] ) || i == line.length()-1 ) {
			
			if ( i == line.length()-1 && !table4.isDelimiter( line[i] ) && !isSpace( line[i] ) ) input.push_back( line[i] ) ;
			if ( line[i] == '.' ) { // comment
				comment_num = i;
				if ( i != line.length() -1 ) {
					str = line.substr( i+1, line.length() ) ;
					comment = str;
				}
				
				return;
			}
			
			if ( line[i] == '=' ) {
				int j = i;
				for ( j = i; j < line.length() ; j ++ ) {
					if ( j == i || line[j] == '\'' || ( !table4.isDelimiter( line[j] ) && !isSpace( line[j] ) ) ) input.push_back( line[j] ) ;
					else break ;
				}
				
				str.assign( input.begin(), input.end() ) ;
				token_list.push_back( str ) ;
				input.clear();
				i = j ;
				continue;
			}
			
			if ( line[i] == '\'' ) { // literal
				if ( input.size() != 1 ) return;
				
				//str.assign( 1, line[i] );
				//token_list.push_back( str ) ;
				int j = i+1;
				for ( ; j < line.length() && line[j] != '\'' ; j++ ) ;
				if ( j < line.length() ) {
					str = line.substr( i, j-i+1 ) ;
					str = input[0] + str ;
					
					token_list.push_back( str ) ;
					//cout << "literal:" << str << "end" <<endl;
					i = j;
					input.clear();
					continue;
				}
				else return;
			}
			
			if ( !input.empty() ) { // 暫存區有東西 
				str.assign( input.begin(), input.end() ) ;
				token_list.push_back( str ) ;
				input.clear();
			}
			
			
			
			if ( table4.isDelimiter( line[i] ) ) {
				str.assign( 1, line[i] );
				token_list.push_back( str ) ;
			}
		}
		else input.push_back( line[i] ) ;
	}
	
}

void group( string &line_num, int &length, vector<string> token_list, vector<value> & token_value, token_group & groupList, string & dupSym ) { 
	string str;
	value temp;
	bool dup = false;
	int num = 0;
	int format = 0;
	length = 0;
	//cout << "line:" << line_num <<endl;
	vector<string> addTemp;
	for ( int i = 0 ; i < token_list.size() ; i ++ ) {
		clear( temp ) ;
		num = 0;
		if ( table1.isInstr( token_list[i], line_num, temp, format ) ) {
			if ( i != token_list.size() - 1 ) { // not format1
				if ( table3.isReg( token_list[i+1], line_num, temp ) ) { // format2
					length = 2;
					if ( token_list.size() - i == 4 && !token_list[i+2].compare( "," ) == 0 ) return;
					else if ( token_list.size() - i == 4 && !table3.isReg( token_list[i+3], line_num, temp ) )	return;
					else if ( token_list.size() - i != 4 && token_list.size() - i !=  2 ) return;
					else {
						if ( i != 0 ) { // label
							num = table5.calculate( token_list[0] ) ;
							table5.hash( num, token_list[0], line_num, token_value, dup );
							groupList.label = token_list[0];
							if ( dup == true ) dupSym = groupList.label;
						}
						if ( table1.isInstr( token_list[i], line_num, temp, format ) ) token_value.push_back( temp ) ;
						groupList.instr = token_list[i] ;
						if ( table3.isReg( token_list[i+1], line_num, temp ) ) token_value.push_back( temp ) ;
						groupList.op1 = token_list[i+1];
						if ( token_list.size() == 4 ) {
							if ( table4.isDelimiter( token_list[i+2], line_num, temp ) ) token_value.push_back( temp ) ;
							if ( table3.isReg( token_list[i+3], line_num, temp ) ) token_value.push_back( temp ) ;
							groupList.op2 = token_list[i+3];
						}
						
					}
				}
				else if ( token_list[i+1].compare( "#" ) == 0 || token_list[i+1].compare( "@" ) == 0 ) { 
					length = 3 ;
					if ( token_list.size() - i != 3 ) return;
					if ( token_list[0].compare( "+" ) == 0 || token_list[1].compare( "+" ) == 0 ) { // lebel+
						length = 4;
						if ( token_list[0].compare( "+" ) == 0 ) {
							table4.isDelimiter( token_list[0], line_num, temp ) ;
							token_value.push_back( temp ) ;
						}
						else {
							num = table5.calculate( token_list[0] ) ;
							table5.hash( num, token_list[0], line_num, token_value, dup );
							groupList.label = token_list[0];
							if ( dup == true ) dupSym = groupList.label;
							table4.isDelimiter( token_list[1], line_num, temp ) ;
							token_value.push_back( temp ) ;
						}
					}
					else if ( i != 0 ) { // label
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						groupList.label = token_list[0];
						if ( dup == true ) dupSym = groupList.label;
					}
					table1.isInstr( token_list[i], line_num, temp, format ) ;
					token_value.push_back( temp ) ;
					groupList.instr = token_list[i];
					if ( table4.isDelimiter( token_list[i+1], line_num, temp ) ) token_value.push_back( temp ) ;
					if ( table6.isInteger( token_list[i+2], line_num, temp, num )  ) {
						table6.hash( num, token_list[i+2], line_num, token_value );
						if ( token_list[i+1].compare( "#" ) == 0 ) groupList.op1 = "#" + token_list[i+2];
						else groupList.op1 = "@"  + token_list[i+2];
					}
					else {
						num = table5.calculate( token_list[i+2] ) ;
						table5.hash( num, token_list[i+2], "", token_value, dup );
						if ( token_list[i+1].compare( "#" ) == 0 ) groupList.op1 = "#"  + token_list[i+2];
						else groupList.op1 = "@"  + token_list[i+2];
					}
				}
				else if ( token_list.size() - i == 4 ) { // <symbol>, X
					if ( !token_list[i+2].compare( "," ) == 0 || strcasecmp( token_list[i+3].c_str(), "x" ) != 0 ) return;
					else {
						if ( i != 0 && !token_list[i-1].compare( "+" ) == 0 ) length = 4;
						else length = 3;
						for ( int j = 0 ; j < token_list.size() ; j ++ ) {
							if ( table1.isInstr( token_list[j], line_num, temp, format ) ) {
								token_value.push_back( temp ) ;
								groupList.instr = token_list[j];
							}
							else if ( table3.isReg( token_list[j], line_num, temp ) ) token_value.push_back( temp ) ;
							else if ( table4.isDelimiter( token_list[j], line_num, temp ) ) token_value.push_back( temp ) ;
							else { // symbol
								num = table5.calculate( token_list[j] ) ;
								if ( j != 0 ) {
									table5.hash( num, token_list[j], "", token_value, dup );
									groupList.op1 = token_list[j] +  token_list[j+1] +  token_list[j+2];
								}
								else {
									table5.hash( num, token_list[j], line_num, token_value, dup );
									groupList.label = token_list[0];
								}
							
								if ( dup == true ) dupSym = token_list[j];
							}	
						}
						
					}
				}
				else if ( token_list[i+1][0] == '=' ) { // = X''… | C''…
					format = 3 ;
					if ( i != 0 && token_list[0].compare( "+" ) != 0 ) { // label
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						groupList.label = token_list[0];
						if ( dup == true ) dupSym = groupList.label;
					}
					
					if ( i != 0 && token_list[i-1].compare( "+" ) == 0 ) {
						table4.isDelimiter( token_list[0], line_num, temp ) ;
						token_value.push_back( temp ) ;
						format = 4 ;
					}
					
					table1.isInstr( token_list[i], line_num, temp, format ) ;
					groupList.instr = token_list[i] ;
					token_value.push_back( temp ) ;
					num = table5.calculate( token_list[i+1] ) ;
					table5.hash( num, token_list[i+1], "", token_value, dup ) ;
					table8.table8.push_back( token_value.back().num ) ;
					groupList.op1 = token_list[i+1];
				}
				else if ( token_list.size() - i == 2 ) { // <direct>
					if ( i != 0 && token_list[0].compare( "+" ) != 0 ) { // label
						format = 3;
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						groupList.label = token_list[0];
						if ( dup == true ) dupSym = groupList.label;
					}
					
					if ( i != 0 && token_list[i-1].compare( "+" ) == 0 ) {
						format = 4;
						table4.isDelimiter( token_list[i-1], line_num, temp ) ;
						token_value.push_back( temp ) ;
					}
					
					table1.isInstr( token_list[i], line_num, temp, format ) ;
					token_value.push_back( temp ) ;
					groupList.instr = token_list[i];
					if ( table6.isInteger( token_list[i+1], line_num, temp, num )  ) {
						table6.hash( num, token_list[i+1], line_num, token_value ) ;
						groupList.op1 = token_list[i+1] ;
					}//addr
					else {
						num = table5.calculate( token_list[i+1] ) ;
						table5.hash( num, token_list[i+1], "", token_value, dup );
						groupList.op1 = token_list[i+1] ;
					}
				}
				else return;
			}
			else { // format1
				length = 1;
				if ( i != 0 ) {
					num = table5.calculate( token_list[0] ) ;
					table5.hash( num, token_list[0], line_num, token_value, dup );
					groupList.label = token_list[0];
					if ( dup == true ) dupSym = groupList.label;
				}
				
				token_value.push_back( temp ) ; // format1
				groupList.instr = token_list[i] ;
			}
			
			if ( token_list[0].compare( "+" ) == 0 || ( token_list.size() > 1 && token_list[1].compare( "+" ) == 0 ) ) format = 4;
			length = format ;
			groupList.format = format ;
		}
		else if ( table2.isPseudo( token_list[i], line_num, temp ) && ( i == 0 || i == 1 ) ) {
			groupList.format = 0 ;
			if ( strcasecmp( token_list[i].c_str(), "start" ) == 0 || strcasecmp( token_list[i].c_str(), "resb" ) == 0 || strcasecmp( token_list[i].c_str(), "resw" ) == 0 ) {
				if ( token_list.size() - i == 2 && table6.isInteger( token_list[i+1], line_num, temp, num ) ){
					if ( strcasecmp( token_list[i].c_str(), "start" ) == 0 ) line_num = token_list[i+1];
					else if ( strcasecmp( token_list[i].c_str(), "resb" ) == 0 ) length = stoi(token_list[i+1]);
					else length = stoi(token_list[i+1])*3 ;
					
					if ( i == 1 ) { // label
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num , token_value, dup );
						if ( dup == true ) dupSym = groupList.label;
						groupList.label = token_list[0];
					}
					
					table2.isPseudo( token_list[i], line_num, temp ) ;
					token_value.push_back( temp ) ;
					table6.isInteger( token_list[i+1], line_num, temp, num );
					groupList.op1 = token_list[i+1] ;
					table6.hash( num, token_list[i+1], line_num, token_value ) ;
				}
				else return ;
			}
			else if ( strcasecmp( token_list[i].c_str(), "end" ) == 0 ) {
				if ( token_list.size() > 3 ) return;
				else if ( token_list.size() == 3 && strcasecmp( token_list[0].c_str(), "end" ) == 0 ) return;
				else if ( token_list.size() == 3 && strcasecmp( token_list[2].c_str(), "end" ) == 0 ) return;
				else {
					if ( i == 1 ) { // label
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						if ( dup == true ) dupSym = groupList.label;
						groupList.label = token_list[0];
					}
					
					for ( int j = i ; j < token_list.size() ; j++ ) {
						if ( table2.isPseudo( token_list[j], line_num, temp ) ) token_value.push_back( temp ) ;
						else {
							num = table5.calculate( token_list[j] ) ;
							table5.hash( num, token_list[j], "0000", token_value, dup );
							groupList.op1 = token_list[j] ;
						}
					}
					
					for ( int j = 0 ; table8.table8.size() > 0 ;  ) {
						if ( table5.table5[table8.table8[0]].addr == "" && table5.table5[table8.table8[0]].symbol != "" && table5.table5[table8.table8[0]].symbol[0] == '=' ) {
							if ( table5.table5[table8.table8[0]].symbol[1] == 'c' || table5.table5[table8.table8[0]].symbol[1] == 'C' ) {
								table5.table5[table8.table8[0]].addr = line_num;
								int line_temp = table5.table5[table8.table8[0]].symbol.length() ;
								line_num = hex_add( line_num, line_temp ) ; 
								table8.table8.erase( table8.table8.begin() ) ;
							}
							else if ( table5.table5[table8.table8[0]].symbol[1] == 'X' || table5.table5[table8.table8[0]].symbol[1] == 'x' ) {
								table5.table5[table8.table8[0]].addr = line_num;
								int line_temp = table5.table5[table8.table8[0]].symbol.length() ;
								line_num = hex_add( line_num, line_temp/2 ) ; 
								table8.table8.erase( table8.table8.begin() ) ;
							}
							else {
								table5.table5[table8.table8[0]].addr = line_num;
								line_num = hex_add( line_num, 3 ) ; 
								table8.table8.erase( table8.table8.begin() ) ;
							}
						}
						else table8.table8.erase( table8.table8.begin() ) ;
					}
				}
			}
			else if ( strcasecmp( token_list[i].c_str(), "EQU" ) == 0 ) {
				if ( i != 1 || token_list.size() != 3 ) {
					return;
				}
				else if ( strcasecmp( token_list[0].c_str(), "equ" ) == 0 || strcasecmp( token_list[2].c_str(), "equ" ) == 0 ) {
					return ;
				}
				else {
					for ( int j = 0 ; j < token_list.size() ; j++ ) {
						if ( table2.isPseudo( token_list[j], line_num, temp ) ) token_value.push_back( temp ) ;
						else if ( table6.isInteger( token_list[j], line_num, temp, num ) ) {
							table6.hash( num, token_list[j], line_num, token_value );
							groupList.op1 = token_list[j] ;
							num = table5.calculate( token_list[0] ) ;
							table5.hash( num, token_list[0], hex_add( "0", stoi( token_list[j] ) ), token_value, dup );
							if ( hex_com( line_num, hex_add( "0", stoi(token_list[j]) ) ) == 1 ) {
								string hex_temp = hex_sub( line_num, hex_add( "0", stoi(token_list[j]) ) ) ;
								for ( int k = 0 ; k < hex_temp.length() ; k ++ ) {
									int temp1 = 0;
									if ( hex_temp[k] >= 'A' && hex_temp[k] <= 'F' ) temp1 = hex_temp[k] - 'A' + 10 ;
									else temp1 = hex_temp[k] - '0' ;
									length = temp1*pow( 16, hex_temp.length()- 1 - k ) + length;
								}
							}
							
							line_num = hex_add( "0", stoi( token_list[j] ) ) ;
							
							token_value.pop_back() ;
						}
						else {
							num = table5.calculate( token_list[j] ) ;
							if ( j != 0 ) {
								table5.hash( num, token_list[0], table5.address( token_list[j] ), token_value, dup );
								line_num = table5.address( token_list[j] ) ;
								table5.hash( num, token_list[j], "", token_value, dup );
								groupList.op1 = token_list[j] ;
								
							}
							else {
								table5.hash( num, token_list[j], "", token_value, dup );
								groupList.label = token_list[j];
							}
							if ( dup == true ) dupSym = token_list[j];
						}
					}
				}
			}
			else if ( strcasecmp( token_list[i].c_str(), "byte" ) == 0 || strcasecmp( token_list[i].c_str(), "word" ) == 0 ) {
				groupList.format = 5;
				//cout << "size:" << token_list.size() << endl;
				if ( i != 0 && i!=1 ) return;
				else if ( table6.isInteger( token_list[i+1], line_num, temp, num ) ) {
					if ( token_list.size() != i+2 ) return ;
					if ( i == 1 ) {
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						if ( dup == true ) dupSym = token_list[0];
						groupList.label = token_list[0];
					}
					
					length = 3 ;
					table2.isPseudo( token_list[i], line_num, temp ) ;
					token_value.push_back( temp ) ;
					table6.isInteger( token_list[i+1], line_num, temp, num );
					if ( strcasecmp( token_list[i].c_str(), "byte" ) == 0 ) length = token_list[i+1].size()/2 ;
					table6.hash( num, token_list[i+1], line_num, token_value );
					groupList.op1 = token_list[i+1] ;
					groupList.format = 5;
				}
				else if ( ( token_list[i+1][0] == 'c' || token_list[i+1][0] == 'C' || token_list[i+1][0] == 'x' || token_list[i+1][0] == 'X' ) && token_list.size() == i+2 ) {
					if ( i == 1 ) {
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						if ( dup == true ) dupSym = token_list[0];
						groupList.label = token_list[0];
					}
					
					length = 3 ;
					table2.isPseudo( token_list[i], line_num, temp ) ;
					token_value.push_back( temp ) ;
					num = table7.calculate( token_list[i+1] ) ;
					table7.hash( num, token_list[i+1], line_num, token_value ) ;
					if ( strcasecmp( token_list[i].c_str(), "byte" ) == 0 && token_list[i+1][0] == 'C' ) length = (token_list[i+1].size()-3) ;
					else if ( strcasecmp( token_list[i].c_str(), "byte" ) == 0 && token_list[i+1][0] == 'X' ) length = (token_list[i+1].size()-3)/2 ;
					groupList.op1 = token_list[i+1] ;
					groupList.format = 5;
				}
				else return;
			}
			else if ( strcasecmp( token_list[i].c_str(), "ltorg" ) == 0 ) {
				if ( i != 0 && i != 1 ) return ;
				else if ( token_list.size() > 2 ) return ;
				else if ( i == 0 && token_list.size() != 1 ) return;
				else {
					if ( i == 1 ) {
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						if ( dup == true ) dupSym = token_list[0];
						groupList.label = token_list[0];
					}
					
					table2.isPseudo( token_list[i], line_num, temp ) ;
					token_value.push_back( temp ) ;
					for ( int j = 0 ; table8.table8.size() > 0 ;  ) {
						if ( table5.table5[table8.table8[0]].addr == "" && table5.table5[table8.table8[0]].symbol != "" && table5.table5[table8.table8[0]].symbol[0] == '=' ) {
							if ( table5.table5[table8.table8[0]].symbol[1] == 'c' || table5.table5[table8.table8[0]].symbol[1] == 'C' ) {
								table5.table5[table8.table8[0]].addr = line_num;
								int line_temp = table5.table5[table8.table8[0]].symbol.length() ;
								line_num = hex_add( line_num, line_temp ) ; 
								table8.table8.erase( table8.table8.begin() ) ;
							}
							else if ( table5.table5[table8.table8[0]].symbol[1] == 'X' || table5.table5[table8.table8[0]].symbol[1] == 'x' ) {
								table5.table5[table8.table8[0]].addr = line_num;
								int line_temp = table5.table5[table8.table8[0]].symbol.length() ;
								line_num = hex_add( line_num, line_temp/2 ) ; 
								table8.table8.erase( table8.table8.begin() ) ;
							}
							else {
								table5.table5[table8.table8[0]].addr = line_num;
								line_num = hex_add( line_num, 3 ) ; 
								table8.table8.erase( table8.table8.begin() ) ;
							}
						}
						else table8.table8.erase( table8.table8.begin() ) ;
					}
					
				}
			}
			else if ( strcasecmp( token_list[i].c_str(), "base" ) == 0 ) {
				if ( token_list.size() != 2 && token_list.size() != 3 ) return;
				else if ( token_list.size() == 3 && ( strcasecmp( token_list[0].c_str(), "base" ) == 0 || strcasecmp( token_list[2].c_str(), "base" ) == 0 ) ) return ;
				else if ( token_list.size() == 2 && strcasecmp( token_list[1].c_str(), "base" ) == 0  ) return ;
				else if ( table6.isInteger( token_list[i+1], line_num, temp, num ) ) { 
					if ( i == 1 ) { // label
						num = table5.calculate( token_list[0] ) ;
						table5.hash( num, token_list[0], line_num, token_value, dup );
						if ( dup == true ) dupSym = token_list[0];
						groupList.label = token_list[0];
					}
					
					table2.isPseudo( token_list[i], line_num, temp ) ;
					token_value.push_back( temp ) ;
					table6.isInteger( token_list[i+1], line_num, temp, num );
					table6.hash( num, token_list[i+1], line_num, token_value );
					groupList.op1 = token_list[i+1] ;
					num = table5.calculate( "base" ) ;
					table5.hash( num, "base", hex_add( "0", stoi(token_list[i+1]) ), token_value, dup );
					base = table5.table5[token_value.back().num] ;
					baseIndex = token_value.back().num;
					token_value.pop_back() ;
				}
				else {
					for ( int j = 0 ; j < token_list.size() ; j++ ) {
						if ( table2.isPseudo( token_list[j], line_num, temp ) ) token_value.push_back( temp ) ;
						else {
							num = table5.calculate( token_list[j] ) ;
							if ( j == 0 ) {
								table5.hash( num, token_list[j], line_num, token_value, dup );
								groupList.label = token_list[0];
							}
							else {
								table5.hash( num, token_list[j], "", token_value, dup );
								groupList.op1 = token_list[j] ;
								base = table5.table5[token_value.back().num] ;
								baseIndex = token_value.back().num;
								groupList.op1 = table5.table5[token_value.back().num].symbol;
							}
							if ( dup == true ) dupSym = token_list[0];
						}
					}
					
					
				}
				
				
			}
			else	return;
			groupList.instr = token_list[i] ;
		}			
	}
	
}

void translate( string line, token_group groupList, string &obj, vector<value> & token_value ) {
	instr_set set;
	string addr ;
	string ta;
	string pc;
	if ( groupList.format != 5 ) table1.instruction_set( groupList.instr, set ) ;
	else if ( groupList.format == 5 ) set.format = 5;
	char nixbpe[6] = { '1', '1', '0', '0', '0', '0' } ;
	if ( ver == 2 ) {
		nixbpe[0] = '0' ;
		nixbpe[1] = '0' ;
	}
	int index = 0, temp = 0;
	//cout << groupList.instr << " format:" << set.format << endl;
	//cout << line << endl;
	for ( int i = 0 ; i < token_value.size() ; i ++ ) if ( token_value[i].tableNum == 4 && token_value[i].num == 2 ) set.format = 4;
	//cout << set.format <<endl; 
	if ( set.format ==  1 ) obj = set.opcode;
	else if ( set.format == 2 ) { 
		if ( strcasecmp( groupList.instr.c_str(), "clear" ) == 0 || strcasecmp( groupList.instr.c_str(), "svc" ) == 0 || strcasecmp( groupList.instr.c_str(), "tixr" ) == 0 ) {
			for ( ; index < token_value.size() ; index++ ) if ( token_value[index].tableNum == 1 ) break;
			if ( index == 1 && token_value.size() > 3 ) {
				obj = "syntex error." ;
				return;
			}
			if ( index == 0 && token_value.size() > 2 ) {
				obj = "syntex error." ;
				return;
			}
			obj = set.opcode + to_string( token_value[index+1].num ) + "0";
		}
		else {
			for ( ; index < token_value.size() ; index++ ) if ( token_value[index].tableNum == 1 ) break; // index 為 instr 
			if ( token_value.size() - index != 4 ) {
				obj = "syntex error." ;
				return;
			}
			
			obj = set.opcode + to_string( token_value[index+1].num ) + to_string( token_value[index+3].num );
		}
	}
	else if ( set.format == 3 ) { // e = 0
		for ( ; index < token_value.size() ; index++ ) {
			if ( token_value[index].tableNum == 4 && token_value[index].num == 12 ) nixbpe[0] = '0' ; // imme
			if ( token_value[index].tableNum == 4 && token_value[index].num == 13 ) nixbpe[1] = '0' ; // indirect
			if ( token_value[index].tableNum == 3 && token_value[index].num == 1 ) nixbpe[2] = '1' ; // x
		}
		if ( ver == 2 ) {
			nixbpe[0] = '0' ;
			nixbpe[1] = '0' ;
		}
		if ( nixbpe[0] == '1' && nixbpe[1] == '0' ) set.opcode[1] = set.opcode[1] + 2 ;
		else if ( nixbpe[0] == '1' && nixbpe[1] == '1' ) set.opcode[1] = set.opcode[1] + 3 ;
		else if ( nixbpe[0] == '0' && nixbpe[1] == '1' ) set.opcode[1] = set.opcode[1] + 1 ;
		if ( set.opcode[1] > '9' && ( set.opcode[1] < 'A' || set.opcode[1] > 'F' ) ) set.opcode[1] = set.opcode[1] -'9' - 1 + 'A' ;
		for ( index = 0; index < token_value.size() ; index++ ) if ( token_value[index].tableNum == 1 ) break; // index 為 instr 
		if ( token_value[index+1].tableNum == 5 ) { // symbol or symbol,x
			ta = table5.table5[token_value[index+1].num].addr ;
			if ( ta == "" ) {
				obj = "undefine symbol" ;
				return; 
			} 
			pc = hex_add( line, 3 ) ;
			addr = hex_sub( ta, pc ) ;
			//cout << "ta:" <<ta << "pc:" << pc << endl;
			if ( token_value.size() > index + 2 && token_value[index+3].tableNum == 3 ) nixbpe[2] = '1' ;
			if ( ver == 2 ) addr = ta ;
			else if ( addr != "" && addr.length() < 4 ) { // 可用PC 
				while ( addr.length() < 3 ) {
					if ( hex_com( ta, pc ) == 1 ) addr.insert( addr.begin(), '0' ) ;
					else addr.insert( addr.begin(), 'F' ) ;
				}
				
				nixbpe[4] = '1' ;
			}
			else { // 用base 
				if ( base.addr == "" ) {
					obj = "沒有設定base" ;
					return;
				}
				
				//cout << "用base" <<endl;
				addr = hex_sub( ta, base.addr ) ;
				while ( addr.length() < 3 ) {
					if ( hex_com( ta, pc ) == 1 ) addr.insert( addr.begin(), '0' ) ;
					else addr.insert( addr.begin(), 'F' ) ;
				}
				
				nixbpe[3] = '1' ;
			}
			
		}
		else if ( token_value[index+1].tableNum == 6 ) { // direct addr
			addr = table6.table6[token_value[index+1].num] ;
			while ( addr.length() < 3 ) addr.insert( addr.begin(), '0' ) ;
		}
		else if ( token_value[index+1].tableNum == 7 ) { // literal
		//	ta = table7.table7
		}
		else if ( token_value[index+1].tableNum == 4 && token_value[index+1].num == 12 ) { // #(imme)
			if ( token_value[index+2].tableNum == 5 ) {
				ta = table5.table5[token_value[index+2].num].addr ;
				if ( ta == "" ) {
					obj = "undefine symbol" ;
					return; 
				} 
			
				pc = hex_add( line, 3 ) ;
				addr = hex_sub( ta, pc ) ;
				if ( addr != "" && addr.length() < 4 ) { // 可用PC 
					while ( addr.length() < 3 ) {
						if ( hex_com( ta, pc ) == 1 ) addr.insert( addr.begin(), '0' ) ;
						else addr.insert( addr.begin(), 'F' ) ;
					}
					
					nixbpe[4] = '1' ;
				}
				else { // 用base 
					if ( base.addr == "" ) {
						obj = "沒有設定base" ;
						return;
					}
					addr = hex_sub( ta, base.addr ) ;
					while ( addr.length() < 3 ) {
						if ( hex_com( ta, pc ) == 1 ) addr.insert( addr.begin(), '0' ) ;
						else addr.insert( addr.begin(), 'F' ) ;
					}
				
					nixbpe[3] = '1' ;
				}
			}
			else {
				addr = table6.table6[token_value[index+2].num] ;
			}
			
		}
		else if ( token_value[index+1].tableNum == 4 && token_value[index+1].num == 13 ) {// @ ( indirect)
			ta = table5.table5[token_value[index+2].num].addr ;
			if ( ta == "" ) {
				obj = "undefine symbol" ;
				return; 
			} 
			
			pc = hex_add( line, 3 ) ;
			addr = hex_sub( ta, pc ) ;
			if ( addr != "" && addr.length() < 4 ) { // 可用PC 
				while ( addr.length() < 3 ) {
					if ( hex_com( ta, pc ) == 1 ) addr.insert( addr.begin(), '0' ) ;
					else addr.insert( addr.begin(), 'F' ) ;
				}
				
				nixbpe[4] = '1' ;
			}
			else { // 用base 
				if ( base.addr == "" ) {
					obj = "沒有設定base" ;
					return;
				}
				addr = hex_sub( ta, base.addr ) ;
				while ( addr.length() < 3 ) {
					if ( hex_com( ta, pc ) == 1 ) addr.insert( addr.begin(), '0' ) ;
					else addr.insert( addr.begin(), 'F' ) ;
				}
				
				nixbpe[3] = '1' ;
			}
		}
		
		for ( int i = 2; i < 6 ; i ++ ) {
			temp = temp + (nixbpe[i]-'0')*pow( 2, 5-i) ;
		}
		
		if ( temp < 10 ) set.opcode.push_back( temp + '0' ) ;
		else set.opcode.push_back( temp -10 + 'A' ) ;
		if ( ver == 2 ) {
			set.opcode.pop_back() ;
			if ( nixbpe[2] == '1' ) {
				addr[0] = addr[0] + 8;
				if( addr[0] > '9' ) addr[0] = addr[0] - '9' - 1 + 'A' ;
			}
			
			while ( addr.length() < 4 ) addr.insert( addr.begin(), '0' ) ;
		}
		else {
			while ( addr.length() < 3 ) addr.insert( addr.begin(), '0' ) ;
		}
		obj = set.opcode + addr;
	}
	else if ( set.format == 4 ) { // b =0, p =0, e = 1
		nixbpe[5] = '1';
		for ( ; index < token_value.size() ; index++ ) {
			if ( token_value[index].tableNum == 4 && token_value[index].num == 12 ) nixbpe[0] = '0' ; // imme
			if ( token_value[index].tableNum == 4 && token_value[index].num == 13 ) nixbpe[1] = '0' ; // indirect
			if ( token_value[index].tableNum == 3 && token_value[index].num == 1 ) nixbpe[2] = '1' ; // x
		}
		
		if ( nixbpe[0] == '1' && nixbpe[1] == '0' ) set.opcode[1] = set.opcode[1] + 2 ;
		else if ( nixbpe[0] == '1' && nixbpe[1] == '1' ) set.opcode[1] = set.opcode[1] + 3 ;
		else if ( nixbpe[0] == '0' && nixbpe[1] == '1' ) set.opcode[1] = set.opcode[1] + 1 ;
		if ( set.opcode[1] > '9' ) set.opcode[1] = set.opcode[1] -'9' - 1 + 'A' ;
		if ( nixbpe[2] == '1' ) set.opcode.push_back( '9' ) ;
		else set.opcode.push_back( '1' ) ;
		
		for ( index = 0 ; index < token_value.size() ; index++ ) if ( token_value[index].tableNum == 1 ) break; // index 為 instr 
		if ( token_value[index+1].tableNum == 5 ) addr = table5.table5[token_value[index+1].num].addr ;
		while ( addr.length() < 5 ) addr.insert( addr.begin(), '0' ) ;
		obj = set.opcode + addr ;
	}
	else if ( set.format == 5 ) { // word.byte
		for ( ; index < token_value.size() ; index++ ) {
			if ( token_value[index].tableNum == 2 ) break ; 
		}
		
		if ( token_value[index].num == 3 ) { // byte
			if ( token_value[index+1].tableNum == 7 ) { // c'' .x''
				string temp = table7.table7[token_value[index+1].num] ;
				if ( temp[0] == 'X' || temp[0] == 'x' ) {
					obj = temp.substr( 2, line.length()-2 ) ;
				}
				else { // c
					for ( int j = 2 ; j < temp.length()-1 ; j ++ ) obj = obj + hex_add( "0", temp[j] ) ;
				}
			}
		}
		else if ( token_value[index].num == 4 ) { // word
			if ( token_value[index+1].tableNum == 6 ) { // decNum
				obj = hex_add( "0", stoi( table6.table6[token_value[index+1].num] ) ) ;
				while ( obj.length() < 6 ) obj.insert( obj.begin(), '0' ) ;
			}
		}
		
		//if ( table7.table7[token_value[index+1].num][0] ==  )obj =  ;
	}
}

void pass2( vector<vector<value>> token_value, vector<token_group> group_list, vector<string> line, vector<string> & obj ) {
	string temp ;
	for ( int i = 0 ; i < token_value.size() ; i++ ) {
		if ( group_list[i].format != 0 ) translate( line[i], group_list[i], temp, token_value[i] ) ;
		//cout << group_list[i].instr << ":" << temp << endl;
		obj.push_back( temp ) ;
		temp.clear() ;
	}
}

void input( ifstream & file, vector<vector<value>> & token_value, vector<token_group> &group_list, vector<string> &lineNum, vector<string> &source ) {
	string line = "";
	string sourceline = "" ;
	string dupSym ;
	int length = 0;
	vector<value> temp ;
	vector<string> list;
	vector<string> temp2;
	int comment_index = -1;
	token_group temp3;
	string comment;
	string location = "0000";
	while ( getline( file, line ) ) {
		//cout << line << endl ;
		if ( line == "" ) continue;
		source.push_back( line ) ;
		token( line, temp2, comment_index, comment );
		
		if ( comment_index != -1 ) 	{
			temp3.hascomment = true;
			temp3.comment = comment;
		}
		//if ( temp2.size() == 4) cout << temp2.size() << " " << temp2[1] << " " << temp2[3] <<"%%%" ;
		list.insert( list.end(), temp2.begin(), temp2.end() ) ;
		group( location, length, temp2, temp, temp3, dupSym );
		if ( temp2.empty() ) lineNum.push_back( "" ) ; // comment
		else lineNum.push_back( location ) ;
		//cout << "location:" << temp3.format << endl;
		if ( length > 0 ) location = hex_add( location, length ) ;
		if ( dupSym != "" ) cout << "dup:" << dupSym << endl;
		if ( temp.empty() && comment_index == -1 && line != "" )  cout << "syntex error!" << endl ;
		group_list.push_back( temp3 ) ;
		//cout << temp3.label << " " << temp3.instr << " " << temp3.op1 << " "<< temp3.op2 << endl;
		
		
		token_value.push_back( temp ) ;
		if ( strcasecmp( temp3.instr.c_str(), "end" ) == 0 ) return;
		line = "" ;
		//getline( file, line ) ;
		for ( int j = 0 ; j < temp.size() ; j++ ){
			//cout << "(" << temp[j].tableNum << ", " << temp[j].num << ") " ;
		}
		//cout << "lineend" << endl;
		temp.clear();
		temp2.clear();
		clear( temp3 ) ;
		comment_index = -1 ;
		comment = "" ;
		length = 0;
	}
	
}

void output( vector<string> location, vector<token_group> source, vector<string> obj ) {
	string name;
	if (ver == 1 ) name = "SICXEout.txt";
	if (ver == 2 ) name = "SICout.txt";
	ofstream out( name );
	out << "Line  Location  Source code                              Object code" <<endl;
	out << "----  -------- -------------------------                 -----------" <<endl;
	cout << "Line  Location  Source code                              Object code" <<endl;
	cout << "----  -------- -------------------------                 -----------" <<endl;
	for ( int i = 0 ; i < source.size() ; i ++ ) {
		while ( location[i].length() < 4 && location[i].length() != 0 ) location[i].insert( location[i].begin(), '0' ) ;
		out << right << setw(4) << (i+1)*5 ;
		if ( source[i].instr != "" && ( strcasecmp( source[i].instr.c_str(), "ltorg" ) != 0 && strcasecmp( source[i].instr.c_str(), "end" ) != 0 ) ) out << setw(6) << location[i] ;
		out << setw(10) << source[i].label << "       " ;
		cout << right << setw(4) << (i+1)*5  ;
		if ( source[i].instr != "" && ( strcasecmp( source[i].instr.c_str(), "ltorg" ) != 0 && strcasecmp( source[i].instr.c_str(), "end" ) != 0 ) ) cout << setw(6) << location[i] ;
		else cout << "      ";
		cout << setw(10) << source[i].label << "       " ;
		
		if ( source[i].format == 4 ) out << left << setw(6) << "+" + source[i].instr ; 
		else out << left << setw(8) << source[i].instr <<" "; 
		if ( source[i].format == 4 ) cout << left << setw(6) << "+" + source[i].instr ; 
		else cout << left << setw(8) << source[i].instr <<" "; 
		
		if ( source[i].op2 == "" && source[i].op1 != "" ) out << setw(15) << source[i].op1  ;
		else if ( source[i].op1 != "" ) out << source[i].op1  ;
		else if ( source[i].hascomment == 0 ) out << "                 " ;
		else if ( source[i].hascomment != 0 )  out << "." << source[i].comment ;
		if ( source[i].op2 == "" && source[i].op1 != "" ) cout << setw(15) << source[i].op1  ;
		else if ( source[i].op1 != "" ) cout << source[i].op1  ;
		else if ( source[i].hascomment == 0 ) cout << "                 " ;
		else if ( source[i].hascomment != 0 )  cout << "." << source[i].comment ;
		
		if ( source[i].op2 != "" ) out << right << ","+source[i].op2 << "            ";
		out << "             " << left << setw(20) << obj[i] ;
		out << endl;
		if ( source[i].op2 != "" ) cout << right << ","+source[i].op2 << "            ";
		cout << "             " << left << setw(20) << obj[i] ;
		cout << endl;
	}
}

int main() {
	vector<vector<value>> token_value ;
	vector<token_group> group_list;
	vector<string> undefine;
	vector<string> location;
	vector<string> source;
	vector<string> obj;
	string fileName = ""; //檔名數字部分
	ifstream file ; // 讀入的檔案
	baseIndex = -1;
	openFile( file, fileName ) ;
	if ( fileName != "0" ) {
		input( file, token_value, group_list, location, source );
		if ( baseIndex != -1 ) base = table5.table5[baseIndex] ;
		pass2( token_value, group_list, location, obj ) ;
		output( location, group_list, obj ) ;
		table5.check( undefine ) ;
		if ( !undefine.empty() ) {
			cout << "=====================" << endl ;
			cout << "undefine:" << endl;
			for ( int i = 0 ; i < undefine.size() ; i++ ) {
				cout << undefine[i] << endl;
			}
		}
		file.close();
	}
} 
