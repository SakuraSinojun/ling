//////////////////////////////////////////////////////////////////
//
// name: Parser.h
// func: �ļ��������������
// date: 2010.10.20	
//
///////////////////////////////////////////////////////////////////

#ifndef PASER_H
#define PASER_H

// �ر�CRT����
#define _CRT_SECURE_NO_WARNINGS


#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cfloat>		// ����float������
#include <climits>		// Ӧ��int������

using namespace std;

const int PARSER_STRIP_EMPTY_LINES = 1;   // ɾ������
const int PARSER_LEAVE_EMPTY_LINES = 2;   // ��������
const int PARSER_STRIP_WS_ENDS     = 4;   // ɾ����β�ո�
const int PARSER_LEAVE_WS_ENDS     = 8;   // ������β�ո�
const int PARSER_STRIP_COMMENTS    = 16;  // ɾ��ע��
const int PARSER_LEAVE_COMMENTS    = 32;  // ����ע��

const int PATTERN_STATE_INIT       = 0;	  // ��ʼ���׶�

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
	
	// ���ļ�

	bool Open(const char *fileName);

	void Close();

	// �õ��ļ��е�һ��

	const char *GetLine(int mode);

	// ��ע���ַ��������ע���ַ�

	void SetComment(char comment);

	bool PatternMath(string buffer, string pattern, ...);

	// ����һ���ַ����Ƿ���float��
	// ����ǣ����ظ�ֵ��������ǣ�����FLT_MIN
	float IsFloat(const string fstring);

	int IsInt(const string istring);

        const char *GetString();
        int GetInt();
        float GetFloat();

public:
	
	ifstream m_ifs;

	string strLine;		// ������ȡ���ļ���
	string m_strComment;	// ����ע�͵��ַ�

        string strToke;

	int m_lineNum;

	vector<int> vints;
	vector<float> vfloats;
	vector<string> vstrings;
	

};

// �˺���ɾ��stringIn��stripChars�������ַ������isCaseΪtrue������д�Сд���жϣ������ж�
// �������ر�ɾ�����ַ���

extern int StripChars(const char *stringIn, char *stringOut, const char *stripChars, bool isCase = true);

// ʹ�ñ�׼������ذ汾

extern int StripChars(const string &stringIn, string &stringOut, const string &stripChars, bool isCase = true);

// �˺�����stringIn��Ѱ����replaceChars�г��ֵ��ַ� ���滻ΪrepChar

extern int ReplaceChars(const char *stringIn, char *stringOut, 
						const char *replaceChars, char repChar, bool isCase = true);

// ��׼��汾

extern int ReplaceChars(const string &stringIn, string &stringOut, 
					    const string &replaceChars, char repChar, bool isCase = true);

#endif