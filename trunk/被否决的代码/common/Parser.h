//////////////////////////////////////////////////////////////////
//
// name: Parser.h
// func: 文件分析器类的声明
// date: 2010.10.20	
//
///////////////////////////////////////////////////////////////////

#ifndef PASER_H
#define PASER_H

// 关闭CRT警告
#define _CRT_SECURE_NO_WARNINGS


#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cfloat>		// 引用float上下限
#include <climits>		// 应用int上下限

using namespace std;

const int PARSER_STRIP_EMPTY_LINES = 1;   // 删除空行
const int PARSER_LEAVE_EMPTY_LINES = 2;   // 保留空行
const int PARSER_STRIP_WS_ENDS     = 4;   // 删除行尾空格
const int PARSER_LEAVE_WS_ENDS     = 8;   // 保留行尾空格
const int PARSER_STRIP_COMMENTS    = 16;  // 删除注释
const int PARSER_LEAVE_COMMENTS    = 32;  // 保留注释

const int PATTERN_STATE_INIT       = 0;	  // 初始化阶段

const int PATTERN_STATE_RESTART    = 1;
const int PATTERN_STATE_FLOAT      = 2;
const int PATTERN_STATE_INT        = 3;
const int PATTERN_STATE_LITERAL    = 4;
const int PATTERN_STATE_STRING     = 5;
const int PATTERN_STATE_NEXT       = 6;

const int PATTERN_STATE_MATCH      = 7;
const int PATTERN_STATE_END        = 8;

class Parser
{
public:
	Parser();
	~Parser();

	void Reset();
	
	// 打开文件

	bool Open(const char *fileName);

	void Close();

	// 得到文件中的一行

	const char *GetLine(int mode);

	// 向注释字符串中添加注释字符

	void SetComment(char comment);

	bool PatternMath(string buffer, string pattern, ...);

	// 测试一个字符串是否是float，
	// 如果是，返回该值，如果不是，返回FLT_MIN
	float IsFloat(const string fstring);

	int IsInt(const string istring);

        const char *GetString();
        int GetInt();
        float GetFloat();

public:
	
	ifstream m_ifs;

	string strLine;		// 包含读取的文件行
	string m_strComment;	// 包含注释的字符

        string strToke;

	int m_lineNum;

	vector<int> vints;
	vector<float> vfloats;
	vector<string> vstrings;
	

};

// 此函数删除stringIn中stripChars包含的字符，如果isCase为true，则进行大小写的判断，否则不判断
// 函数返回被删除的字符数

extern int StripChars(const char *stringIn, char *stringOut, const char *stripChars, bool isCase = true);

// 使用标准库的重载版本

extern int StripChars(const string &stringIn, string &stringOut, const string &stripChars, bool isCase = true);

// 此函数在stringIn中寻找在replaceChars中出现的字符 并替换为repChar

extern int ReplaceChars(const char *stringIn, char *stringOut, 
						const char *replaceChars, char repChar, bool isCase = true);

// 标准库版本

extern int ReplaceChars(const string &stringIn, string &stringOut, 
					    const string &replaceChars, char repChar, bool isCase = true);

#endif