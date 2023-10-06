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
#include <algorithm> 
//SIC_input_get (1).txt
//SICXE_input.txt
//SIC_input.txt
using namespace std;
using std::string; using std::hex;
using std::stringstream;
struct value{
	int tableNum ;
	int num;
};

struct identifier{
	string str ;
	int sub ;
	int type; // 1:array 2:bool 3:char 4:int 5:label 6:real
	int pointer;
};

struct code{
	pair<int,int> num[4];
	string source;
};

struct label_data{
	int type; // 1: id的ptr, 2: gto , 3: call 
	int index;
	string str;
	int ptr;
};

struct data{
	int priority;
	string str;
};

void openFile( ifstream & file, string &fileName ) { 
    // 打開文件
    string temp;
	cout << "Input a file name [0: quit] :\n" ;
	getline( cin, fileName );
	fileName = fileName + ".txt" ;
	if ( fileName.compare( "0" ) == 0 ) return ;
	file.open( fileName.c_str() ) ;
	
 	while ( !file ) {
		cout << "### " << fileName + ".txt" << " does not exist! ###\n" ;
		openFile( file, fileName ) ;
	} // while

}  

void clear( value &temp ) {
	temp.tableNum = 0;
	temp.num = 0 ;
}

bool isSpace ( char c ) {
	if ( c == ' ' || c == '\t' || c == '\n' || c == '\0' ) return true;
	else return false;
}

bool isRelation( string str ) {
	if ( strcasecmp( str.c_str(), "and" ) == 0 ) return true;
	else if ( strcasecmp( str.c_str(), "eq" ) == 0 ) return true;
	else if ( strcasecmp( str.c_str(), "ge" ) == 0 ) return true;
	else if ( strcasecmp( str.c_str(), "gt" ) == 0 ) return true;
	else if ( strcasecmp( str.c_str(), "le" ) == 0 ) return true;
	else if ( strcasecmp( str.c_str(), "lt" ) == 0 ) return true;
	else if ( strcasecmp( str.c_str(), "ne" ) == 0 ) return true;
	else if ( strcasecmp( str.c_str(), "or" ) == 0 ) return true;
	else return false;
}

void value_insert( vector<value> &list, int table_num, int num ) {
	value temp ;
	temp.tableNum = table_num;
	temp.num = num;
	list.push_back( temp ) ;
}

int typeNum( string str ) {
	if ( strcasecmp( str.c_str(), "array" ) == 0 ) return 1;
	else if ( strcasecmp( str.c_str(), "boolean" ) == 0 ) return 2;
	else if ( strcasecmp( str.c_str(), "character" ) == 0 ) return 3;
	else if ( strcasecmp( str.c_str(), "integer" ) == 0 ) return 4;
	else if ( strcasecmp( str.c_str(), "label" ) == 0 ) return 5;
	else if ( strcasecmp( str.c_str(), "real" ) == 0 ) return 6;
	else return 0;
}

class Delimiter{
	vector<string> table1 ;
	public:
	Delimiter(){
		ifstream file ;
		string str;
		file.open( "Table1.table" ) ;
		while ( file >> str ){
			table1.push_back( str ) ;
		}
		
		file.close();
	}
	
	bool isDelimiter ( char c ) { // 切token用 
		for ( int i = 0 ; i < table1.size() ; i++ ) {
			if ( c == table1[i][0] )	return true;
		}
		
		return false;
	}
	
	bool isDelimiter ( string str ) {
		for ( int i = 0 ; i < table1.size() ; i++ ) {
			if ( str.compare( table1[i] ) == 0 ) {
				return true;
			}
		}
		
		return false;
	}
	
	pair<int,int> find_index ( string str ) {
		for ( int i = 0 ; i < table1.size() ; i++ ) {
			if ( str.compare( table1[i] ) == 0 ) {
				return make_pair(1,i);
			}
		}
		
		return make_pair(-1,-1);
	}
};

class Reserved{
	vector<string> table2 ;
	public:
	Reserved(){
		ifstream file ;
		string str;
		file.open( "Table2.table" ) ;
		while ( file >> str ){
			table2.push_back( str ) ;
		}
		
		file.close();
	}
	
	bool isReserved ( string str ) {
		for ( int i = 0 ; i < table2.size() ; i++ ) {
			if ( strcasecmp( str.c_str(), table2[i].c_str() ) == 0 ) {
				return true;
			}
		}
		
		return false;
	}
	
	pair<int, int> find_index( string str ) {
		for ( int i = 0 ; i < table2.size() ; i++ ) {
			if ( strcasecmp( str.c_str(), table2[i].c_str() ) == 0 ) {
				return make_pair( 2, i ) ;
			}
		}
		
		return make_pair( 2, -1 ) ;
	}
};

class Integer{
	
	public:
	vector<string> table3 ;
	
	bool isInteger ( string str ) {
		for ( int i = 0 ; i < str.size() ; i++ ) {
			if ( str[i] >= '0' && str[i] <= '9' ) {
			}
			else return false;
		}
		
		return true;
	}
	
	pair<int,int> hash( string str ){
		table3.push_back( str ) ;
		return make_pair( 3, table3.size()-1 );
	}
};

class Real{
	public:
	vector<string> table4 ;
	bool isReal( string str ) {
		int index = -1; 
		for ( int i = 0 ; i < str.size() ; i++ ) {
			if ( str[i] != '.' && str[i] > '9' && str[i] < '0' ) return false ;
			if ( str[i] == '.' ) index = i ;
		}
		
		if ( index == -1 ) return false ;
		return true;
	}
	
	pair<int,int> hash( string str ){
		table4.push_back( str ) ;
		return make_pair( 4, table4.size()-1 );
	}
};

class Id{
	public:
	identifier table5[100] ;
	pair<int,int> insert( string str, int sub, string type, int ptr ) {
		identifier temp;
		pair<int,int> temp2 ;
		temp.str = str;
		temp.sub = sub;
		temp.type = typeNum( type ) ;
		temp.pointer = ptr;
		int sum = 0;
		for ( int i = 0 ; i < str.length() ; i++ ) {
			sum = sum + str[i] ;
		}
		
		for ( int i = sum %100 ; i < 100 ; i++ ) {
			if ( table5[i].str.empty() ) {
				table5[i] = temp;
				temp2.first = 5 ;
				temp2.second = i ;
				return temp2;
			}
			
			if ( i == 99 ) i = -1;
		}
	}
	
	
	pair<int,int> find_index( string str ) {
		for ( int i = 0 ; i < 100 ; i ++ ) {
			if ( table5[i].str.compare( str ) == 0 ) return make_pair( 5, i ) ;
		}	
		
		return make_pair( 5, -1 ) ;
	}
	
	void change( int index, int num ) {
		if ( index >= 0 ) table5[index].pointer = num;
	}
};

class Quadruple{
	public:
		vector<code> table6;
		int insert( pair<int,int> code0, pair<int,int> code1, pair<int,int> code2, pair<int,int> code3, string str ) {
			code temp;
			temp.num[0] = code0 ;
			temp.num[1] = code1 ;
			temp.num[2] = code2 ;
			temp.num[3] = code3 ;
			temp.source = str;
			table6.push_back( temp ) ;
			return table6.size() ;
		}
		
};

class information{
	public:
		vector<int> table7;
		int insert( int num ) {
			table7.push_back(num) ;
			return table7.size() ;
		}
		
		int insert( string num ) {
			table7.push_back( stoi(num) ) ;
			return table7.size() ;
		}
		
		void change( int index, int num ) {
			if ( index < table7.size() ) {
				table7[index] = num;
			}
		} 
};

class Term{
	public:
		vector<string> table0;
		pair<int,int> insert( string str ) {
			for ( int i = 0 ; i < table0.size() ; i ++ ) {
				if ( table0[i].compare( str ) == 0 ) return make_pair( 0, i );
			}
			
			table0.push_back( str ) ;
			return make_pair( 0, table0.size()-1 );
		} 
		
		pair<int,int> find( string str ) {
			for ( int i = 0 ; i < table0.size() ; i ++ ) {
				if ( table0[i].compare( str ) == 0 ) return make_pair( 0, i );
			}
			
			return make_pair( -1, -1 );
		} 
} ;

Term table0;
Delimiter table1;
Reserved table2;
Integer table3;
Real table4;
Id table5;
Quadruple table6;
information table7;

int dataType( string str ) {
	string str2 = str.substr( 1, str.length()-1 ) ;
	if ( str[0] == 'T' && table3.isInteger(str2) && stoi(str2) < table0.table0.size() ) return 0;
	else if ( str.length() == 1 && table1.isDelimiter( str[0] ) ) return 1; // deli
	else if ( table2.isReserved( str ) ) return 2; // statement
	else if ( table3.isInteger(str) ) return 3; // integer
	else if ( table4.isReal( str ) ) return 4 ; // real
	else return 5; // id
}
 
bool isCondition( vector<string> str, int index ) {
	int open = 0;
	for ( int i = 1 ; i < index ; i = i+2 ) {
		if ( strcasecmp( str[i].c_str(), "(" ) == 0 ) {
			open ++;
			str.erase( str.begin() + i ) ;
		}
		if ( strcasecmp( str[i].c_str(), ")" ) == 0 ) {
			open--;
			str.erase( str.begin() + i ) ;
		}
		
		if ( open < 0 ) return false ;
	}
	
	if ( open != 0 ) return false;
	
	for ( int i = 1 ; i < index ; i = i+2 ) {
		if ( dataType( str[i] ) != 3 && dataType( str[i] ) != 4 && dataType( str[i] ) != 5 ) return false ;
		if ( i != index -1 && !isRelation( str[i+1] )  ) return false;
	}
	
	return true;
}

pair<int,int> condition( vector<string> input, int index ) { // if 有operator 
	vector<data> operand ;
	vector<string>  symbol;
	pair<int, int> symbol1, symbol2, term;
	data temp ;
	string temp2;
	for ( int i = 1 ; i < index ; i++ )  {
		if ( !isRelation( input[i] ) && strcasecmp( input[i].c_str(), "(" ) != 0 && strcasecmp( input[i].c_str(), ")" ) != 0 )  {
			symbol.push_back( input[i] ) ;
		}
		else if ( isRelation( input[i] ) ) { // 優先度為 1 
			if ( !operand.empty() && operand.back().priority >= 1 ) { 
			// 若比堆疊頂端優先權小就彈出堆疊最上層，存進list，直到優先權比堆疊頂端大或是堆疊為空，再存進堆疊 
				while ( !operand.empty() && operand.back().priority >= 1 ) { 
					temp2 = operand.back().str + " " + symbol.back() ;
					if ( dataType( symbol.back() ) == 5 ) { //id
						symbol1 = table5.find_index( symbol.back() ) ;
					}
					else if ( dataType( symbol.back() ) == 4 ) { // real
						symbol1 = table4.hash( symbol.back() ) ;
					}
					else if ( dataType( symbol.back() ) == 3 ) { // int
						symbol1 = table3.hash( symbol.back() ) ;
					}
					else symbol1 = term ;
				
					symbol.pop_back() ;
					temp2 = symbol.back() + " " + temp2;
					if ( dataType( symbol.back() ) == 5 ) { //id
						symbol2 = table5.find_index( symbol.back() ) ;
					}
					else if ( dataType( symbol.back() ) == 4 ) { // real
						symbol2 = table4.hash( symbol.back() ) ;
					}
					else if ( dataType( symbol.back() ) == 3 ) { // int
						symbol2 = table3.hash( symbol.back() ) ;
					}
					else symbol2 = term ;
				
					symbol.pop_back();
					term = table0.insert( temp2 ) ;
					string a = "T" + to_string( term.second ) + " = " + temp2 ;
					table6.insert( table2.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
					symbol.push_back( "T" + to_string( term.second ) ) ;
					operand.pop_back() ;
				} // while
				
			} // else
			
			temp.priority = 1;
			temp.str = input[i] ;
			operand.push_back(temp) ;
		} // if
		
		else if ( strcasecmp( input[i].c_str(), "(" ) == 0 )	{ // 優先度為0 
			temp.priority = 0;
			temp.str = input[i] ;
			operand.push_back(temp) ;
		}
		else if ( strcasecmp( input[i].c_str(), ")" ) == 0 ) {
			//如果是右括號，就把到推出堆疊，存進list，直到左括號後 
			while ( strcasecmp( operand.back().str.c_str(), "(" ) != 0 ) {
				temp2 = operand.back().str + " " + symbol.back() ;
				if ( dataType( symbol.back() ) == 5 ) { //id
					symbol1 = table5.find_index( symbol.back() ) ;
				}
				else if ( dataType( symbol.back() ) == 4 ) { // real
					symbol1 = table4.hash( symbol.back() ) ;
				}
				else if ( dataType( symbol.back() ) == 3 ) { // int
					symbol1 = table3.hash( symbol.back() ) ;
				}
				else symbol1 = term ;
				
				symbol.pop_back() ;
				temp2 = symbol.back() + " " + temp2;
				if ( dataType( symbol.back() ) == 5 ) { //id
					symbol2 = table5.find_index( symbol.back() ) ;
				}
				else if ( dataType( symbol.back() ) == 4 ) { // real
					symbol2 = table4.hash( symbol.back() ) ;
				}
				else if ( dataType( symbol.back() ) == 3 ) { // int
					symbol2 = table3.hash( symbol.back() ) ;
				}
				else symbol2 = term ;
				
				symbol.pop_back();
				term = table0.insert( temp2 ) ;
				string a = "T" + to_string( term.second ) + " = " + temp2 ;
				table6.insert( table2.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
				symbol.push_back( "T" + to_string( term.second ) ) ;
				operand.pop_back() ;
			} // while
			
			operand.pop_back() ; // 移除'(' 
		} // if
	}

	while ( !operand.empty() )  { 
	// 把剩下的運算符號彈出 
		temp2 = operand.back().str + " " + symbol.back() ;
		if ( dataType( symbol.back() ) == 5 ) { //id
			symbol1 = table5.find_index( symbol.back() ) ;
		}
		else if ( dataType( symbol.back() ) == 4 ) { // real
			symbol1 = table4.hash( symbol.back() ) ;
		}
		else if ( dataType( symbol.back() ) == 3 ) { // int
			symbol1 = table3.hash( symbol.back() ) ;
		}
		else symbol1 = term ;
				
		symbol.pop_back() ;
		temp2 = symbol.back() + " " + temp2;
		if ( dataType( symbol.back() ) == 5 ) { //id
			symbol2 = table5.find_index( symbol.back() ) ;
		}
		else if ( dataType( symbol.back() ) == 4 ) { // real
			symbol2 = table4.hash( symbol.back() ) ;
		}
		else if ( dataType( symbol.back() ) == 3 ) { // int
			symbol2 = table3.hash( symbol.back() ) ;
		}
		else symbol2 = term ;
				
		symbol.pop_back();
		term = table0.insert( temp2 ) ;
		string a = "T" + to_string( term.second ) + " = " + temp2 ;
		table6.insert( table2.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
		symbol.push_back( "T" + to_string( term.second ) ) ;
		operand.pop_back() ;
	}
	
	if ( index == 2 ) {
		if ( dataType( symbol.back() ) == 5 ) { //id
			term = table5.find_index( symbol.back() ) ;
		}
		else if ( dataType( symbol.back() ) == 4 ) { // real
			term = table4.hash( symbol.back() ) ;
		}
		else if ( dataType( symbol.back() ) == 3 ) { // int
			term = table3.hash( symbol.back() ) ;
		}
	}
	
	return term;
}

pair<int,int> array( string str ) {
	int index = str.find( "(" ) ;
	vector<string> num; 
	string temp = "";
	pair<int,int> term, symbol1;
	string symbol;
	for ( int i = index+1 ; i < str.length() ; i++ ) {
		if ( str[i]== ',' || str[i] == ')' ) {
			num.push_back( temp );
			temp.clear();
		}
		else temp = temp + str[i];
	}
	
	int dimension = num.size() ;
	string id_name = str.substr( 0, index ) ;
	int ptr = table5.table5[table5.find_index( id_name ).second].pointer;

	for ( int i = num.size()-1 ; i >= 0 ; i-- ){
		if ( i == 0 ) {
			if ( num.size() != 1 ) {
				symbol1 = term;
				term = table0.insert( symbol + "+" + num[0] ) ;
				string a = "T" + to_string( term.second ) + " = " + symbol + "+" + num[0] ;
				if ( dataType( num[i] ) == 5 ) { //id
					table6.insert( table1.find_index( "+" ), table5.find_index( num[0] ), symbol1, term, a ) ;
				}
				else if ( dataType( num[i] ) == 4 ) { // real
					table6.insert( table1.find_index( "+" ), table5.find_index( num[0] ), symbol1, term, a ) ;
				}
				else if ( dataType( num[i] ) == 3 ) { // int
					table6.insert( table1.find_index( "+" ), table5.find_index( num[0] ), symbol1, term, a ) ;
				}
				
				symbol = "T" + to_string( term.second );
			}
			else symbol = num[0];
			
			term = table0.insert( id_name + "(" + symbol + ")" ) ;
			string a = "T" + to_string( term.second ) + " = " + id_name + "(" + symbol + ")" ;
			if ( dataType( symbol ) == 5 ) { //id
				table6.insert( table1.find_index( "=" ), table5.find_index( id_name ), table5.find_index( symbol ), term, a ) ;
			}
			else if ( dataType( symbol ) == 4 ) { // real
				table6.insert( table1.find_index( "=" ), table5.find_index( id_name ), table4.hash( symbol ), term, a ) ;
			}
			else if ( dataType( symbol ) == 3 ) { // int
				table6.insert( table1.find_index( "=" ), table5.find_index( id_name ), table3.hash( symbol ), term, a ) ;
			}
			else {
				table6.insert( table1.find_index( "=" ), table5.find_index( id_name ), make_pair( 0, stoi(symbol.substr(1, symbol.length()-1)) ), term, a ) ;
			}
		
			return term;
		}
		
		term = table0.insert( num[i] + "- 1" ) ; // j-1
		string a = "T" + to_string( term.second ) + " = " + num[i] + "- 1" ;
		if ( dataType( num[i] ) == 5 ) { //id
			table6.insert( table1.find_index( "-" ), table5.find_index( num[i] ), table3.hash( "1" ), term, a ) ;
		}
		else if ( dataType( num[i] ) == 4 ) { // real
			table6.insert( table1.find_index( "-" ), table4.hash( num[i] ), table3.hash( "1" ), term, a ) ;
		}
		else if ( dataType( num[i] ) == 3 ) { // int
			table6.insert( table1.find_index( "-" ), table3.hash( num[i] ), table3.hash( "1" ), term, a ) ;
		}
		
		for ( int j = ptr+2, k = 0; k < dimension-1 ; k++, j++ ) {
			symbol1 = term;
			symbol = "T" + to_string( term.second ) ;
			term = table0.insert( symbol + "*" + to_string( table7.table7[j]) ) ;
			string a = "T" + to_string( term.second ) + " = " + symbol + "*" + to_string( table7.table7[j]) ;
			table6.insert( table1.find_index( "*" ), symbol1, table3.hash( to_string( table7.table7[j]) ), term, a ) ;
		}
	}
}

void assignment ( vector<string> input ) { // if 有operator 
	vector<data> operand ;
	vector<pair<string,bool>>  symbol; // bool = true->array
	
	pair<int, int> symbol1, symbol2, term;
	data temp ;
	string temp2;
	for ( int i = 0 ; i < input.size() -1 ; i++ )  {
		if ( !table1.isDelimiter( input[i] ) && strcasecmp( input[i].c_str(), "(" ) != 0 && strcasecmp( input[i].c_str(), ")" ) != 0 )  {
			if ( dataType( input[i] ) == 5 && table5.table5[table5.find_index( input[i] ).second].type == 1 ) { // array存同一格 
				string array_temp = "";
				for ( int j = i ; input[j].compare( ")" ) != 0 ; j++, i++ ) {
					array_temp = array_temp + input[j] ;
				}
				
				array_temp = array_temp + input[i] ;
				symbol.push_back( make_pair( array_temp, true ) ) ;
				continue;
			}
			
			symbol.push_back( make_pair( input[i], false ) ) ;
		}
		else if ( strcasecmp( input[i].c_str(), "+" ) == 0 || strcasecmp( input[i].c_str(), "-" ) == 0 ) { // 優先度為 1 
			if ( !operand.empty() && operand.back().priority >= 1 ) { 
			// 若比堆疊頂端優先權小就彈出堆疊最上層，存進list，直到優先權比堆疊頂端大或是堆疊為空，再存進堆疊 
				while ( !operand.empty() && operand.back().priority >= 1 ) { 
					temp2 = operand.back().str + " " + symbol.back().first ;
					if ( symbol.back().second == true ) symbol1 = array( symbol.back().first ) ;
					else if ( dataType( symbol.back().first ) == 5 ) { //id
						symbol1 = table5.find_index( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 4 ) { // real
						symbol1 = table4.hash( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 3 ) { // int
						symbol1 = table3.hash( symbol.back().first ) ;
					}
					else symbol1 = term ;
				
					symbol.pop_back() ;
					temp2 = symbol.back().first + " " + temp2;
					if ( symbol.back().second == true ) symbol2 = array( symbol.back().first ) ;
					else if ( dataType( symbol.back().first ) == 5 ) { //id
						symbol2 = table5.find_index( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 4 ) { // real
						symbol2 = table4.hash( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 3 ) { // int
						symbol2 = table3.hash( symbol.back().first ) ;
					}
					else symbol2 = term ;
				
					symbol.pop_back();
					if ( strcasecmp( operand.back().str.c_str(), "=" ) == 0 ) {
						table6.insert( table1.find_index( operand.back().str ), symbol1, make_pair(-1,-1) , symbol2, temp2 ) ; // 產生中間碼 
					}
					else {
						term = table0.insert( temp2 ) ;
						string a = "T" + to_string( term.second ) + " = " + temp2 ;
						table6.insert( table1.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
						symbol.push_back( make_pair( "T" + to_string( term.second ), false ) ) ;
					}
		
					operand.pop_back() ;
				} // while
				
			} // else
			
			temp.priority = 1;
			temp.str = input[i] ;
			operand.push_back(temp) ;
		} // if
		else if ( strcasecmp( input[i].c_str(), "*" ) == 0 || strcasecmp( input[i].c_str(), "/" ) == 0 ) { // 優先度為 1 
			if ( !operand.empty() && operand.back().priority >= 2 ) { 
			// 若比堆疊頂端優先權小就彈出堆疊最上層，存進list，直到優先權比堆疊頂端大或是堆疊為空，再存進堆疊 
				while ( !operand.empty() && operand.back().priority >= 2 ) { 
					temp2 = operand.back().str + " " + symbol.back().first ;
					if ( symbol.back().second == true ) symbol1 = array( symbol.back().first ) ;
					else if ( dataType( symbol.back().first ) == 5 ) { //id
						symbol1 = table5.find_index( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 4 ) { // real
						symbol1 = table4.hash( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 3 ) { // int
						symbol1 = table3.hash( symbol.back().first ) ;
					}
					else symbol1 = term ;
				
					symbol.pop_back() ;
					temp2 = symbol.back().first + " " + temp2;
					if ( symbol.back().second == true ) symbol2 = array( symbol.back().first ) ;
					else if ( dataType( symbol.back().first ) == 5 ) { //id
						symbol2 = table5.find_index( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 4 ) { // real
						symbol2 = table4.hash( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 3 ) { // int
						symbol2 = table3.hash( symbol.back().first ) ;
					}
					else symbol2 = term ;
				
					symbol.pop_back();
					if ( strcasecmp( operand.back().str.c_str(), "=" ) == 0 ) {
						table6.insert( table1.find_index( operand.back().str ), symbol1, make_pair(-1,-1) , symbol2, temp2 ) ; // 產生中間碼 
					}
					else {
						term = table0.insert( temp2 ) ;
						string a = "T" + to_string( term.second ) + " = " + temp2 ;
						table6.insert( table1.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
						symbol.push_back( make_pair( "T" + to_string( term.second ), false ) ) ;
					}
		
					operand.pop_back() ;
				} // while
				
			} // else
			
			temp.priority = 2;
			temp.str = input[i] ;
			operand.push_back(temp) ;
		} // if
		else if ( strcasecmp( input[i].c_str(), "^" ) == 0 ) { // 優先度為 1 
			if ( !operand.empty() && operand.back().priority > 3 ) { 
			// 若比堆疊頂端優先權小就彈出堆疊最上層，存進list，直到優先權比堆疊頂端大或是堆疊為空，再存進堆疊 
				while ( !operand.empty() && operand.back().priority >= 3 ) { 
					temp2 = operand.back().str + " " + symbol.back().first ;
					if ( symbol.back().second == true ) symbol1 = array( symbol.back().first ) ;
					else if ( dataType( symbol.back().first ) == 5 ) { //id
						symbol1 = table5.find_index( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 4 ) { // real
						symbol1 = table4.hash( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 3 ) { // int
						symbol1 = table3.hash( symbol.back().first ) ;
					}
					else symbol1 = term ;
				
					symbol.pop_back() ;
					temp2 = symbol.back().first + " " + temp2;
					if ( symbol.back().second == true ) symbol2 = array( symbol.back().first ) ;
					else if ( dataType( symbol.back().first ) == 5 ) { //id
						symbol2 = table5.find_index( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 4 ) { // real
						symbol2 = table4.hash( symbol.back().first ) ;
					}
					else if ( dataType( symbol.back().first ) == 3 ) { // int
						symbol2 = table3.hash( symbol.back().first ) ;
					}
					else symbol2 = term ;
				
					symbol.pop_back();
					if ( strcasecmp( operand.back().str.c_str(), "=" ) == 0 ) {
						table6.insert( table1.find_index( operand.back().str ), symbol1, make_pair(-1,-1) , symbol2, temp2 ) ; // 產生中間碼 
					}
					else {
						term = table0.insert( temp2 ) ;
						string a = "T" + to_string( term.second ) + " = " + temp2 ;
						table6.insert( table1.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
						symbol.push_back( make_pair( "T" + to_string( term.second ), false ) ) ;
					}
		
					operand.pop_back() ;
				} // while
				
			} // else
			
			temp.priority = 3;
			temp.str = input[i] ;
			operand.push_back(temp) ;
		} // if
		else if ( strcasecmp( input[i].c_str(), "(" ) == 0 || strcasecmp( input[i].c_str(), "=" ) == 0 )	{ // 優先度為0 
			temp.priority = 0;
			temp.str = input[i] ;
			operand.push_back(temp) ;
		}
		else if ( strcasecmp( input[i].c_str(), ")" ) == 0 ) {
			//如果是右括號，就把到推出堆疊，存進list，直到左括號後 
			while ( strcasecmp( operand.back().str.c_str(), "(" ) != 0 ) {
				temp2 = operand.back().str + " " + symbol.back().first ;
				if ( symbol.back().second == true ) symbol1 = symbol1 = array( symbol.back().first ) ;
				else if ( dataType( symbol.back().first ) == 5 ) { //id
					symbol1 = table5.find_index( symbol.back().first ) ;
				}
				else if ( dataType( symbol.back().first ) == 4 ) { // real
					symbol1 = table4.hash( symbol.back().first ) ;
				}
				else if ( dataType( symbol.back().first ) == 3 ) { // int
					symbol1 = table3.hash( symbol.back().first ) ;
				}
				else symbol1 = term ;
				
				symbol.pop_back() ;
				temp2 = symbol.back().first + " " + temp2;
				if ( symbol.back().second == true ) symbol2 = array( symbol.back().first ) ;
				else if ( dataType( symbol.back().first ) == 5 ) { //id
					symbol2 = table5.find_index( symbol.back().first ) ;
				}
				else if ( dataType( symbol.back().first ) == 4 ) { // real
					symbol2 = table4.hash( symbol.back().first ) ;
				}
				else if ( dataType( symbol.back().first ) == 3 ) { // int
					symbol2 = table3.hash( symbol.back().first ) ;
				}
				else symbol2 = term ;
				
				symbol.pop_back();
				if ( strcasecmp( operand.back().str.c_str(), "=" ) == 0 ) {
					table6.insert( table1.find_index( operand.back().str ), symbol1, make_pair(-1,-1) , symbol2, temp2 ) ; // 產生中間碼 
				}
				else {
					term = table0.insert( temp2 ) ;
					string a = "T" + to_string( term.second ) + " = " + temp2 ;
					table6.insert( table1.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
					symbol.push_back( make_pair( "T" + to_string( term.second ), false ) ) ;
				}
		
				operand.pop_back() ;
			} // while
			
			operand.pop_back() ; // 移除'(' 
		} // if
	}

	while ( !operand.empty() )  { 
	// 把剩下的運算符號彈出 
		temp2 = operand.back().str + " " + symbol.back().first ;
		if ( symbol.back().second == true ) symbol1 = array( symbol.back().first ) ;
		else if ( dataType( symbol.back().first ) == 5 ) { //id
			symbol1 = table5.find_index( symbol.back().first ) ;
		}
		else if ( dataType( symbol.back().first ) == 4 ) { // real
			symbol1 = table4.hash( symbol.back().first ) ;
		}
		else if ( dataType( symbol.back().first ) == 3 ) { // int
			symbol1 = table3.hash( symbol.back().first ) ;
		}
		else symbol1 = term ;
				
		symbol.pop_back() ;
		temp2 = symbol.back().first + " " + temp2;
		if ( symbol.back().second == true ) symbol2 = array( symbol.back().first ) ;
		else if ( dataType( symbol.back().first ) == 5 ) { //id
			symbol2 = table5.find_index( symbol.back().first ) ;
		}
		else if ( dataType( symbol.back().first ) == 4 ) { // real
			symbol2 = table4.hash( symbol.back().first ) ;
		}
		else if ( dataType( symbol.back().first ) == 3 ) { // int
			symbol2 = table3.hash( symbol.back().first ) ;
		}
		else symbol2 = term ;
				
		symbol.pop_back();
		if ( strcasecmp( operand.back().str.c_str(), "=" ) == 0 ) {
			table6.insert( table1.find_index( operand.back().str ), symbol1, make_pair(-1,-1) , symbol2, temp2 ) ; // 產生中間碼 
		}
		else { 
			term = table0.insert( temp2 ) ;
			string a = "T" + to_string( term.second ) + " = " + temp2 ;
			table6.insert( table1.find_index( operand.back().str ), symbol2, symbol1, term, a ) ; // 產生中間碼 
			symbol.push_back( make_pair( "T" + to_string( term.second ), false ) ) ;
		}
		
		operand.pop_back() ;
	}
}

void token( string line, vector<string> & token_list ) {
	vector<char> input;
	string str;
	value temp;
	int num = 0;
	for ( int i = 0 ; i < line.length() ; i ++ ) {
		clear( temp ) ;
		num = 0;
		if ( table1.isDelimiter( line[i] ) || isSpace( line[i] ) || i == line.length()-1 ) {
			if ( !input.empty() ) { // 暫存區有東西 
				str.assign( input.begin(), input.end() ) ;
				token_list.push_back( str ) ;
				input.clear();
			}
			
			if ( table1.isDelimiter( line[i] ) && line[i] != '.' ) {
				str.assign( 1, line[i] );
				token_list.push_back( str ) ;
			}
		}
		else input.push_back( line[i] ) ;
	}
	
	
}

int routine = 0;

bool syntax( string line, vector<string> token, vector<label_data> &und_label ) {
	value temp;
	label_data temp2;
	pair<int,int> pair_temp;
	int type = 0, num = 0;
	int ptr = 0, dimension = 0, index = 0;
	if ( dataType( token[0] ) == 5 ) {
		for ( int i = 0 ; i < und_label.size() ; i ++ ) {
			if ( token[0].compare( und_label[i].str ) == 0 ) {
				token.erase( token.begin() ) ;
				break;
			}
		}
		
		
	}
	if ( table2.isReserved( token[0] ) ) { 
		if ( strcasecmp( "program", token[0].c_str() ) == 0 ) {
			if ( token.size() != 3 ) return false;
			else if ( dataType( token[1] ) != 5 )	return false;
			return true;
		}
		else if ( strcasecmp( "variable", token[0].c_str() ) == 0 ) {
			type = typeNum( token[1] ) ;
			if ( type == 0 ) return false; // type不對 
			else if ( token[2].compare( ":" ) != 0 ) return false;  
			else if ( token.size() < 5 ) return false; // 至少一個變數 
			for ( int i = 3 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
				if ( dataType( token[i] ) != 5 ) return false; // id
				if ( token[i+1].compare( "," ) != 0 && i != token.size() -2 ) return false; //， 
				if ( token[i+1].compare( "," ) == 0 && i == token.size() -3 ) return false; 
			}
			
			return true;
		}
		else if ( strcasecmp( "dimension", token[0].c_str() ) == 0 ) {
			type = typeNum( token[1] ) ;
			if ( type == 0 ) return false; // type不對
			else if ( token[2].compare( ":" ) != 0 ) return false; 
			else if ( dataType( token[3] ) != 5 ) return false;
			else {
				for ( int i = 3 ; token[i].compare( ";" ) != 0 ; ) { // 判斷文法 
					if ( dataType( token[i] ) != 5 ) return false;
					else if ( token[i+1].compare( "(" ) != 0 ) return false;
					for ( int j = i+2; token[j].compare( ")" ) != 0 ; ) {
						if ( dataType( token[j] ) != 3 ) return false; // int
						if ( token[j+1].compare( "," ) == 0 ) j = j + 2; // 還有 
						else if ( token[j+1].compare( ")" ) != 0 ) return false;
						else {
							i = j+1; // i 是右括號 
							break;
						} 
					}
					
					if ( token[i+1].compare( "," ) == 0 && i != token.size() - 3 ) i = i+2;
					else if ( token[i+1].compare( "," ) == 0 ) return false;
					else if ( token[i+1].compare( ";" ) != 0 ) return false;
					else i++;
				}
				
				return true;
			}
		}
		else if ( strcasecmp( "label", token[0].c_str() ) == 0 ) {
			if ( token.size() < 3 ) return false;
			for ( int i = 1 ; token[i].compare( ";" ) != 0 ; ) { // syntax 檢查 
				if ( dataType( token[i] ) != 5 ) return false;
				if ( token[i+1].compare( ";" ) == 0 ) i++;
				else if ( token[i+1].compare( "," ) == 0 && i == token.size() - 3 ) return false;
				else if ( token[i+1].compare( "," ) == 0 ) i = i + 2 ;
				else return false;
			}
			
			return true;
		}
		else if ( strcasecmp( "gto", token[0].c_str() ) == 0 ) {
			if ( token.size() != 3 ) return false;
			pair_temp = table5.find_index( token[1] ) ;
			if ( pair_temp.second == -1 ) return false;
			return true;
		}
		else if ( strcasecmp( "subroutine", token[0].c_str() ) == 0 ) {
			if ( token.size() < 8 ) return false;
			else if ( dataType( token[1] ) != 5 ) return false;
			else if ( token[2].compare( "(" ) != 0 ) return false;
			else if ( typeNum( token[3] ) == 0 ) return false;
			else if ( token[4].compare( ":" ) != 0 ) return false;
			else if ( token[token.size()-2].compare( ")" ) != 0 ) return false;
			else {
				for ( int i = 5 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
					if ( dataType( token[i] ) != 5 ) return false; // id
					if ( token[i+1].compare( "," ) != 0 && i != token.size() -3 ) return false; //， 
				}
			}
			
			return true;
		}
		else if ( strcasecmp( "call", token[0].c_str() ) == 0 ) {
			if ( token.size() < 6 ) return false;
			else if ( dataType( token[1] ) != 5 ) return false;
			else if ( token[2].compare( "(" ) != 0 ) return false;
			else if ( token[token.size()-2].compare( ")" ) != 0 ) return false;
			else {
				for ( int i = 3 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
					if ( dataType( token[i] ) != 5 && dataType( token[i] ) != 3 && dataType( token[i] ) != 4 ) return false; // id.real.int
					if ( token[i+1].compare( "," ) != 0 && i != token.size() -3 ) return false; //， 
				}
			}
			
			return true;
		}	
		else if ( strcasecmp( "input", token[0].c_str() ) == 0 ) {
			for ( int i = 1 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
				if ( dataType( token[i] ) != 5 ) return false; // id
				if ( token[i+1].compare( "," ) != 0 && i != token.size() -2 ) return false; //， 
			}
			
			return true;
		}
		else if ( strcasecmp( "output", token[0].c_str() ) == 0 ) {
			for ( int i = 1 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
				if ( dataType( token[i] ) != 5 ) return false; // id
				if ( token[i+1].compare( "," ) != 0 && i != token.size() -2 ) return false; //， 
			}
			
			return true;
		}
		else if ( strcasecmp( "if", token[0].c_str() ) == 0 ) {
			for ( int i = 0 ; i < token.size() ; i ++ ) {
				if ( strcasecmp( "then", token[i].c_str() ) == 0 ) index = i;
			}
			
			if ( index == 0 ) return false;
			if ( !isCondition( token, index ) ) return false;
			int index2 = 0;
			for ( int i = 0 ; i < token.size() ; i ++ ) {
				if ( strcasecmp( "else", token[i].c_str() ) == 0 ) index2 = i;
			}
			
			vector<string> temp;
			if ( index2 != 0 ) { // if ... then...else...
				for ( int i = index +1 ; i != index2 ; i++ ) {
					temp.push_back( token[i] ) ;
				}
				
				temp.push_back( ";" ) ;
				if ( !syntax( line, temp, und_label ) ) return false ; // statement1
				temp.clear() ;
				for ( int i = index2 +1 ; i != token.size() ; i++ ) {
					temp.push_back( token[i] ) ;
				}
				
				if ( !syntax( line, temp, und_label ) ) return false ; // statement2
			}
			else { // if ... then...
				
				for ( int i = index +1 ; i != token.size() ; i++ ) {
					temp.push_back( token[i] ) ;
				}
				
				if ( !syntax( line, temp, und_label ) ) return false ; // statement2
			}
		}
		else if ( strcasecmp( "enp", token[0].c_str() ) == 0 || strcasecmp( "ens", token[0].c_str() ) == 0 ) {
			if ( token.size() != 2 ) return false;
		}
		else return false;
		
	}
	else { //assignment // label
		if ( table5.find_index( token[0] ).second == -1 ) return false;
		int open = 0 ;
		dimension = 1;
		for ( int i = 1 ; i < token.size() -1 ; i = i+2 ) {
			dimension = 1;
			if ( token[i].compare( "=" ) == 0 ) i++;
			if ( dataType( token[i] ) == 5 && table5.table5[table5.find_index( token[i] ).second].type == 1 ) { // array
				if ( token[i+1].compare( "(" ) != 0 ) return false;
				for ( int j = i+2; token[j].compare( ")" ) != 0 ; ) {
					if ( dataType( token[j] ) != 3 && dataType( token[j] ) != 5 ) return false; // int
					if ( token[j+1].compare( "," ) == 0 ) {
						j = j + 2; // 還有 
						dimension++;
					}
					else if ( token[j+1].compare( ")" ) != 0 ) return false;
					else {
						
						int int_temp = table5.table5[table5.find_index( token[i] ).second].pointer;
						if ( table7.table7[int_temp+1] != dimension ) return false;
						if ( !table1.isDelimiter( token[j+2] ) ) return false ;
						else i = j +1;
						break;
					} 
				}
				
				continue;
			}
			
			if ( token[i].compare( "(" ) == 0 || token[i].compare( ")" ) == 0 ) {
				i--;
				continue;
			}
			
			if ( token[i+1].compare( "(" ) == 0 || token[i+1].compare( ")" ) == 0 ) {
				i++;
				continue;
			}
			if ( ( i != token.size() -2 && !table1.isDelimiter( token[i+1] ) ) || token[i+1].compare( ":" ) == 0 ) return false;
			
			if ( ( dataType( token[i] ) != 3 && dataType( token[i] ) != 4 && dataType( token[i] ) != 5 ) || ( dataType( token[i] ) == 5 && table5.find_index( token[i] ).second == -1) ) return false ;
			
		}
		
		for ( int i = 1 ; i < index ; i = i+2 ) {
			if ( strcasecmp( token[i].c_str(), "(" ) == 0 ) {
				open ++;
			}
			if ( strcasecmp( token[i].c_str(), ")" ) == 0 ) {
				open--;
			}
		
			if ( open < 0 ) return false ;
		}	
	
		if ( open != 0 ) return false;
	}
	
	return true;
}

bool translate( string line, vector<string> token, vector<label_data> &und_label ) {
	label_data temp2;
	pair<int,int> pair_empty(-1, -1);
	pair<int,int> pair_temp, term;
	int type = 0, num = 0;
	int ptr = 0, dimension = 0, index = 0;
	if ( !syntax( line, token, und_label ) ) return false ; // error
	if ( dataType( token[0] ) == 5 ) {
		for ( int i = 0 ; i < und_label.size() ; i ++ ) {
			if ( token[0].compare( und_label[i].str ) == 0 ) {
				table5.change( und_label[i].index, table6.table6.size() ) ;
				und_label.erase( und_label.begin() + i ) ;
				token.erase( token.begin() ) ;
				break;
			}
		}
	}
	if ( table2.isReserved( token[0] ) ) { 
		if ( strcasecmp( "program", token[0].c_str() ) == 0 ) {
			routine = table5.insert( token[1], 0, "", table6.table6.size() ).second ;
		}
		else if ( strcasecmp( "variable", token[0].c_str() ) == 0 ) {
			type = typeNum( token[1] ) ;
			pair_temp = table5.insert( token[3], routine, token[1], -1 ) ; // 存進id table 
			table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[3] ) ; // 產生中間碼 
			for ( int i = 4 ; token[i].compare( ";" ) != 0 ; i = i +2 ) {
				pair_temp = table5.insert( token[i+1], routine, token[1], -1 ) ; // 存進id table 
				table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[i+1] ) ; // 產生中間碼 
			}
		}
		else if ( strcasecmp( "dimension", token[0].c_str() ) == 0 ) {
			type = typeNum( token[1] ) ;	
			for ( int i = 3 ; token[i].compare( ";" ) != 0 ; ) { 
				ptr = table7.insert( type ) -1;
				index = table7.insert( 0 ) -1 ;
				dimension = 0;
				for ( int j = i+2; token[j].compare( ")" ) != 0 ; ) {
					table7.insert( token[j] ) ;
					dimension++;
					if ( token[j+1].compare( "," ) == 0 ) j = j + 2; // 還有
					else {
						table7.change( index, dimension ) ;
						pair_temp = table5.insert( token[i], routine, "array", ptr ) ; // 存進id table 
						table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[i] ) ; // 產生中間碼 
						i = j+1; // i 是右括號 
						break;
					} 
				}
					
				if ( token[i+1].compare( "," ) == 0 ) i = i+2;
				else i++;
			}
		}
		else if ( strcasecmp( "label", token[0].c_str() ) == 0 ) {
			for ( int i = 1 ; token[i].compare( ";" ) != 0 ; ) { 
				pair_temp = table5.insert( token[i], routine, "label", -1 ) ; // 存進id table 之後要存ptr 
				temp2.index = pair_temp.second ;
				temp2.str = token[i] ;
				temp2.type = 1;
				und_label.push_back( temp2 ) ;
				table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[i] ) ; // 產生中間碼 
				if ( token[i+1].compare( ";" ) == 0 ) i++;
				else i = i + 2 ;
			}
		}
		else if ( strcasecmp( "gto", token[0].c_str() ) == 0 ) {
			pair_temp = make_pair( 6, table5.table5[table5.find_index( token[1] ).second].pointer);
			table6.insert( table2.find_index("gto"), pair_empty, pair_empty, pair_temp, line ) ;
			if ( pair_temp.second == -1 ) {
				temp2.index = table6.table6.size() -1 ;
				temp2.str = token[1] ;
				temp2.type = 2;
				und_label.push_back( temp2 ) ;
			}
		}
		else if ( strcasecmp( "subroutine", token[0].c_str() ) == 0 ) {
			routine = table5.insert( token[1], 0, "", table6.table6.size() ).second ; // 存sub名 
			pair_temp = table5.insert( token[5], routine, token[3], -1 ) ; // 存進id table 
			table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[5] ) ; // 產生中間碼 
			for ( int i = 6 ; token[i].compare( ")" ) != 0 ; i = i +2 ) {
				pair_temp = table5.insert( token[i+1], routine, token[3], -1 ) ; // 存進id table 
				table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[i+1] ) ; // 產生中間碼 
			}
		}
		else if ( strcasecmp( "call", token[0].c_str() ) == 0 ) {
			pair_temp = table5.find_index( token[1] ) ;
			if ( pair_temp.second == -1 ) {
				temp2.index = table6.table6.size();
				temp2.str = token[1] ;
				temp2.type = 3;
				und_label.push_back( temp2 ) ;
			}
			
			table6.insert( table2.find_index("call"), pair_temp, pair_empty, make_pair(7, table7.table7.size()), line ) ; // 產生中間碼 
			for ( int i = 3 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
				if ( dataType( token[i] ) == 5 ) { // label
					pair_temp = table5.find_index( token[i] ) ;
					table7.insert( pair_temp.first ) ;
					table7.insert( pair_temp.second ) ;
				}
				else if ( dataType( token[i] ) == 3 ) { // int
					pair_temp = table3.hash( token[i] ) ;
					table7.insert( pair_temp.first ) ;
					table7.insert( pair_temp.second ) ;
				}
				else if ( dataType( token[i] ) == 4 ) { // real
					pair_temp = table4.hash( token[i] ) ;
					table7.insert( pair_temp.first ) ;
					table7.insert( pair_temp.second ) ;
				}
			}
		}	
		else if ( strcasecmp( "input", token[0].c_str() ) == 0 ) {
			for ( int i = 1 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
				pair_temp = table5.insert( token[i], routine, "label", -1 ) ; // 存進id table 
				table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[i] ) ; // 產生中間碼 
			}
		}
		else if ( strcasecmp( "output", token[0].c_str() ) == 0 ) {
			for ( int i = 1 ; i < token.size() -1 ; i = i + 2 ) { // 非最後一位 
				pair_temp = table5.insert( token[i], routine, "label", -1 ) ; // 存進id table 
				table6.insert( pair_temp, pair_empty, pair_empty, pair_empty, token[i] ) ; // 產生中間碼 
			}
		}
		else if ( strcasecmp( "if", token[0].c_str() ) == 0 ) {
			for ( int i = 0 ; i < token.size() ; i ++ ) {
				if ( strcasecmp( "then", token[i].c_str() ) == 0 ) index = i;
			}
			
			int index2 = 0;
			for ( int i = 0 ; i < token.size() ; i ++ ) {
				if ( strcasecmp( "else", token[i].c_str() ) == 0 ) index2 = i;
			}
			
			term = condition( token, index ) ; 
			// **********************if 中間碼 
			int if_index = table6.table6.size() ;
			table6.insert( table2.find_index("if"), term, make_pair( 6,if_index +1 ), pair_empty, "" ) ;
			vector<string> temp;
			string temp3="";
			if ( index2 != 0 ) { // if ... then...else...
				for ( int i = index +1 ; i != index2 ; i++ ) {
					temp.push_back( token[i] ) ;
					temp3 = temp3 + token[i] + " ";
				}
				
				int gto_index = -1;
				temp.push_back( ";" ) ;
				translate( temp3, temp, und_label ) ;
				if ( strcasecmp( "gto", token[index +1].c_str() ) != 0 ) {
					gto_index = table6.table6.size() ;
					table6.insert( table2.find_index("gto"), pair_empty, pair_empty, pair_empty, "" ) ;
				}
				table6.table6[if_index].num[3] = make_pair( 6, table6.table6.size() ) ;
				if ( term.first == 0 ) table6.table6[if_index].source = "IF " + table0.table0[term.second] + " GO TO " + to_string( if_index +2 ) + ", ELSE GO TO " + to_string( table6.table6.size()+1);
				else table6.table6[if_index].source = "IF " + token[index -1] + " GO TO " + to_string( if_index +2 ) + ", ELSE GO TO " + to_string( table6.table6.size() +1 );
				temp.clear() ;
				temp3 = "" ;
				for ( int i = index2 +1 ; i != token.size() ; i++ ) {
					temp.push_back( token[i] ) ;
					temp3 = temp3 + token[i] + " ";
				}
				
				translate( temp3, temp, und_label ) ;
				if ( gto_index != -1 ) {
					table6.table6[gto_index].num[3] = make_pair( 6, table6.table6.size() ) ;
					table6.table6[gto_index].source = "GTO " + to_string( table6.table6.size() +1 );
				}
			}
			else { // if ... then...
				for ( int i = index +1 ; i != token.size() ; i++ ) {
					temp.push_back( token[i] ) ;
					temp3 = temp3 + token[i] ;
				}
				
				translate( temp3, temp, und_label ) ;
				table6.table6[if_index].num[3] = make_pair(0, 0) ;
				if ( term.first == 0 ) table6.table6[if_index].source = "IF " + table0.table0[term.second] + " GO TO " + to_string( if_index +2 ) ;
				else table6.table6[if_index].source = "IF " + token[index -1] + " GO TO " + to_string( if_index +2 ) ;
			}
		}
		else if ( strcasecmp( "enp", token[0].c_str() ) == 0 ||  strcasecmp( "ens", token[0].c_str() ) == 0 ) {
			table6.insert( table2.find_index( token[0] ), pair_empty, pair_empty, pair_empty, line ) ;
		}
	}
	else { //assignment // label
		assignment( token ) ;
	}
	
	return true;
}

void input( ifstream & file, vector<vector<string> > &token_list, vector<string> & error ) {
	string line, line2;
	vector<string> temp;
	vector<label_data> und_label ;
	string error_line;
	for ( int i = 0 ; !file.eof() && getline( file, line ) ; i++ ) {
		if ( line == "" || ( line.length() == 1 && isSpace( line[0] ) ) ) {
			//cout << "space" <<endl;
			continue;
		}
		
		while ( line[line.length()-1] != ';' ) {
			getline( file, line2 ) ;
			line = line + line2;
		}
		//cout << "line:" << line<< endl;
		token( line, temp ) ;
		for ( int j = 0 ; j < temp.size() ; j++ ) {
			//cout << temp[j] << "|" ;
		}
		
		if ( temp.back() == ";" ) {
			if ( translate( line, temp, und_label ) ) ;
			else {
				error_line = "line " + to_string( i+1 ) + ":" + line ;
				error.push_back( error_line ) ;
			}
		}
		else {
			error_line = "line" + to_string( i+1 ) + " : 最後字元非結束指令 ';'" ;
			error.push_back( error_line ) ;
		}
		temp.clear();
	}
	
	for( int i = 0 ; i < und_label.size() ; i++ ) {
		if ( und_label[i].type == 1 ) {
			cout << "label undefine:" << und_label[i].str << endl ;
		}
		else if ( und_label[i].type == 2 ){
			if ( table5.table5[table5.find_index( und_label[i].str ).second].pointer == -1 ) cout << "line" << und_label[i].index << "GOTO undefine label !" << endl;
			else table6.table6[und_label[i].index].num[3].second = table5.table5[table5.find_index( und_label[i].str ).second].pointer ;
		}
		else {
			if ( table5.table5[table5.find_index( und_label[i].str ).second].pointer == -1 ) cout << "line" << und_label[i].index << "call undefine subroutine !" << endl;
			else table6.table6[und_label[i].index].num[1].second = table5.find_index( und_label[i].str ).second ;
		}
	}
}

void output( string name, vector<string> & error ) {
	name = name + "_out.txt";
	ofstream out( name );
	for ( int i = 0 ; i < table6.table6.size() ; i ++ ) {
		cout << setw(2) << i+1 << "	(" ;
		out << setw(2) << i+1 << "	(" ;
		for ( int j = 0 ; j < 4 ; j ++ ) {
			if ( table6.table6[i].num[j].second != -1 ) cout << "(" << table6.table6[i].num[j].first << "," << table6.table6[i].num[j].second << ")" ;
			if ( j < 3) cout << ",	" ;
			if ( table6.table6[i].num[j].second != -1 ) out << "(" << table6.table6[i].num[j].first << "," << table6.table6[i].num[j].second << ")" ;
			if ( j < 3) out << ",	" ;
		}
		
		cout << ")	" << table6.table6[i].source << endl ;
		out << ")	" << table6.table6[i].source << endl ;
	}
	
	cout << "syntax error 行數: " <<endl ; 
	out << "syntax error 行數: " <<endl ; 
	for ( int i = 0 ; i < error.size() ; i++ ) {
		cout << error[i] << endl;
		out << error[i] << endl;
	}
}

int main() {
	vector<vector<string> > token;
	vector<string> source, error ;
	string fileName = ""; //檔名數字部分
	ifstream file ; // 讀入的檔案
	openFile( file, fileName ) ;
	if ( fileName != "0" ) {
		input( file, token, error ) ;
		output( fileName, error ) ;
		file.close();
	}
} 
