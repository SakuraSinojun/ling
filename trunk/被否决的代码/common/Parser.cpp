//////////////////////////////////////////////////////////////////
//
// name: Parser.h
// func: �ļ��������������
// date: 2010.10.20	
//
///////////////////////////////////////////////////////////////////

#include "parser.h"

///////////////////////////////////////////////////////////////////
//
// ��Ա����
//
///////////////////////////////////////////////////////////////////

Parser::Parser()
{

}

//----------------------------------------/

Parser::~Parser()
{

}

//--------------------------------------------/

void Parser::Reset()
{
	m_ifs.close();
	m_ifs.clear();

	this->strLine = "";
	m_strComment = "";
	m_lineNum = 0;
}

//-----------------------------------------------------------/

bool Parser::Open(const char *fileName)
{
	// ��ֹ��û�йرյ��ļ�

	this->Reset();

	m_ifs.open(fileName, ios_base::in);

	if(!m_ifs)
	{
		m_ifs.setstate(ios_base::badbit);
		return false;
	}

	return true;
}

//--------------------------------------------------/

void Parser::Close()
{
	this->Reset();
}

//--------------------------------------------------------/

const char *Parser::GetLine(int mode)
{
	if(m_ifs)
	{
		this->strLine = "";		// ���ַ����ÿ�

		// ��������

		if(mode & PARSER_STRIP_EMPTY_LINES)
		{
			while(true)
			{
				// û�л�ȡ���ļ���

				if(!getline(m_ifs, this->strLine))
					break;

				string::size_type i = this->strLine.find_first_not_of(' ');

				if(string::npos != i)
				{
					// ������ǰ�ߵĿո�ɾ��

					this->strLine.erase(0, i);

					// ɾ��ע��

					if(mode & PARSER_STRIP_COMMENTS)
					{
						// ��һ��ע�͵�����

						string::size_type pos = this->strLine.find_first_of(m_strComment);

						// nposΪfind���ص�һ������ֵ�������ش�ֵ����˵������û��ƥ��
						// ��ʱ�Ѿ��õ���Ҫ���У�����Ѱ�ҿ���ѭ��

						if(string::npos == pos)
							break;

						// ����һ��ע���У�������һ��

						if(0 == pos)
							continue;

						else
						{
							// ��ʱע���������ߣ���ע�Ϳ�ʼλ����ΪNULL��

							this->strLine.erase(strLine.begin() + pos, strLine.end());

							break;
						}

					} // end if strip comments

					// �Ѿ��õ��������ݣ�����ѭ��

					break;

				} // end if not null string

			} // end while

		} //end if strip empty line
		else
		{
			// ֻ�Ǽ򵥵ĵõ�һ��

			getline(m_ifs, this->strLine);

			if(mode & PARSER_STRIP_COMMENTS)
			{
				// ��һ��ע�͵�����

				string::size_type pos = this->strLine.find_first_of(m_strComment);


				if(string::npos != pos)
				{
					// ��ʱɾ��ע��

					this->strLine.erase(strLine.begin() + pos, strLine.end());
				}

			} // end if strip comments
		}

		// �õ���һ��

		if(!this->strLine.empty())
		{
			m_lineNum++;

			// ɾ����β�ո�

			if(mode & PARSER_STRIP_WS_ENDS)
			{
				// ɾ�����׿ո�

				string::size_type pos = this->strLine.find_first_not_of(' ');
				this->strLine.erase(0, pos);

				// ɾ����β�ո�

				for(string::iterator iter = this->strLine.end() - 1;iter != this->strLine.begin(); iter--)
				{
					if(*iter != ' ')
					{
						// ���ִ���β���һ���ǿո񣬽��˺���ַ���ɾ��

						this->strLine.erase(iter + 1, this->strLine.end());
						break;
					}
				}
			}

			return this->strLine.c_str();
		} // end if empty
		else
		{
			return NULL;
		}

	} // end if ifs

	return NULL;
}

//-----------------------------------------------------------------------------/

void Parser::SetComment(char comment)
{
	m_strComment += comment;
}

//---------------------------------------------------------------------------/

bool Parser::PatternMath(string buffer, string pattern, ...)
{
	// �˺������ڴ�buffer����ȡƥ��pattern�ĸ�ʽ
	// pattern�ַ������԰���������Ϣ
	// ['string']	��ȡ�ַ���string
	// [f]			��ȡһ��������
	// [i]			��ȡһ������
	// [s=ddd]		sΪ�̶���ʶ��dddΪһ��10��������������ȡһ������Ϊddd���ַ���
	// [s<ddd]		��ȡ����С��ddd���ַ���
	// [s>ddd]		��ȡ���ȴ���ddd���ַ���
	// ����Ҫƥ���ַ���"vertex: 34.234 56.34 12.4"
	// ��Ҫ�������ĸ�ʽ ['vertex'] [f] [f] [f]����ȡ
	// ��ȡ������ݰ�˳�򴢴��ڳ�Ա������

	if(buffer.empty() || pattern.empty())
	{
		return false;
	}

	string strType;				// ����pattern���й����͵��ַ�
	string strOperator;			// ����pattern���й���������ַ�
	vector<string> vString;		// ����pattern��ƥ����ַ���
	vector<int> vNum;			// ����pattern��˵�����ַ���������Ϣ

	string::size_type startPos = 0;	// �������̵Ŀ�ʼλ��
	string::size_type endPos;	// �������̵Ľ���λ��

	int tokenNum = 0;				// �ܹ���Ҫ��ȡ�����ݵ�����

	// ����pattern�׶Σ�������pattern��������ݣ���buffer�е�����ַ�����ȡ����

	while(true)
	{
		// �����п�ʼλ�õĿո�

		startPos = pattern.find_first_not_of(' ', startPos);

		if(string::npos == startPos)
			break;

		if(pattern[startPos] == '[')
		{
			switch(pattern[startPos+1])
			{
			case 'f':		// float
				{
					if(pattern[startPos+2] != ']')
						return false;

					startPos += 3;

					// û����ȡ��Ҫ���룬û����ȡ�Ĳ���Ҫ�ÿհ�ռ��
					// string�ÿմ����ַ���'\0'��������0
					strType += 'f';
					vString.push_back("");
					strOperator += '\0';
					vNum.push_back(0);
					tokenNum++;
				}
				break;

			case 'i':       // int
				{
					if(pattern[startPos+2] != ']')
						return false;

					startPos += 3;

					strType += 'i';
					vString.push_back("");
					strOperator += '\0';
					vNum.push_back(0);
					tokenNum++;
				}
				break;

			case '\'':		// string
				{
					// ������ǰ�� '

					startPos += 2;  

					// Ѱ�ҽ�����ʶ

					endPos = pattern.find( '\'', startPos);

					if(pattern[endPos+1] != ']')
						return false;

					vString.push_back(pattern.substr(startPos, endPos - startPos));
					strType += '\'';
					strOperator += '\0';
					vNum.push_back(0);

					startPos = endPos +2;
					tokenNum++;
				}
				break;
				
			case 's':      // ����ƥ��string
				{
					if('=' == pattern[startPos+2] ||
					   '<' == pattern[startPos+2] ||
					   '>' == pattern[startPos+2])
					{
						endPos = startPos + 3;

						while(isdigit(pattern[endPos]))
							endPos++;

						if(pattern[endPos] != ']')
							return false;

						// Խ��s= ��ȡ��ߵ�����

						string temp = pattern.substr(startPos+3, endPos-startPos);

						vString.push_back("");
						strType += 's';
						strOperator += pattern[startPos+2];
						vNum.push_back(atoi(temp.c_str()));
					}
					else
					{
						return false;
					}

					startPos = endPos + 1;
					tokenNum++;
				}
				break;

			default:
				break;
			} // end switch

		} // end if '['

		// ��ʱȫ����ȡ���

		if(startPos >= pattern.length())
			break;

	}// end while

	// ����ȡ�������ַ�������ת���Ľ׶�

	string strWork = buffer;	// �����ַ���
	int patternState = PATTERN_STATE_INIT;	// ��¼��ǰ״̬
	int curToken = 0;				// ��¼��ǰ���ڴ��������еĵڼ���λ��
	string strToken;				// �洢��Ҫ��ʱת�����ַ���	
	bool isFirstPass = true;			// ��NEXT�������ж��Ƿ���Ҫ�����¿�ʼ����㸳ֵ
	string::size_type restartPos = 0;		// ���´�������

	while(true)
	{
		switch(patternState)
		{
		case PATTERN_STATE_INIT:		// ��ʼ���׶�
			{
				strWork = buffer;

				startPos = 0;
				endPos = 0;
				isFirstPass = true;
				restartPos = 0;

				// ����Ա�������ÿ�
				this->vints.clear();
				this->vfloats.clear();
				this->vstrings.clear();

				patternState = PATTERN_STATE_NEXT;
			}
			break;

		case PATTERN_STATE_RESTART:		// ���¿�ʼ�׶Σ���ǰ�Ĵ���������������ǰ
			{
				curToken = 0;
				isFirstPass = true;
				if(endPos > buffer.length())
					return false;
				startPos = endPos = restartPos;

				patternState = PATTERN_STATE_NEXT;
			}
			break;
			
		case PATTERN_STATE_NEXT:          // ������һ���ַ����Ĵ���
			{
				if(curToken >= tokenNum)
					patternState = PATTERN_STATE_MATCH;
				else
				{
					if(endPos > strWork.length())
						return false;

					startPos = endPos;

					while(isspace(strWork[startPos]))
						startPos++;

					endPos = startPos;

					while(!isspace(strWork[endPos]) && endPos < strWork.length())
						endPos++;

					strToken = strWork.substr(startPos, endPos - startPos);

					if(strToken.empty())
						return false;
					
					if(isFirstPass)
					{
						isFirstPass = false;
						restartPos = endPos;
					}

					switch(strType[curToken])
					{
					case 'f':
						{
							patternState = PATTERN_STATE_FLOAT;
						}
						break;

					case 'i':
						{
							patternState = PATTERN_STATE_INT;
						}
						break;

					case 's':
						{
							patternState = PATTERN_STATE_STRING;
						}
						break;

					case '\'':
						{
							patternState = PATTERN_STATE_LITERAL;
						}
						break;

					default:

						break;
					}

				} // end else

			}
			break;
			
		case PATTERN_STATE_FLOAT:
			{
				float f = IsFloat(strToken);

				if(FLT_MIN != f)
				{
					this->vfloats.push_back(f);
					curToken++;
					patternState = PATTERN_STATE_NEXT;
				}
				else
				{
					patternState = PATTERN_STATE_RESTART;
				}
			}
			break;

		case PATTERN_STATE_INT:
			{
				int i = IsInt(strToken);

				if(INT_MIN != i)
				{
					this->vints.push_back(i);
					curToken++;
					patternState = PATTERN_STATE_NEXT;
				}
				else
				{
					patternState = PATTERN_STATE_RESTART;
				}
			}
			break;

		case PATTERN_STATE_LITERAL:
			{
				if(strToken == vString[curToken])
				{
					this->vstrings.push_back(strToken);
					curToken++;
					patternState = PATTERN_STATE_NEXT;
				}
				else
				{
					patternState = PATTERN_STATE_RESTART;
				}
			}
			break;

		case  PATTERN_STATE_STRING:
			{
				switch(strOperator[curToken])
				{
				case '=':
					{
						if(strToken.length() == vNum[curToken])
						{
							this->vstrings.push_back(strToken);
							curToken++;
							patternState = PATTERN_STATE_NEXT;
						}
						else
						{
							patternState = PATTERN_STATE_RESTART;
						}
					}
					break;

				case '>':
					{
						if((int)strToken.length() > vNum[curToken])
						{
							this->vstrings.push_back(strToken);
							curToken++;
							patternState = PATTERN_STATE_NEXT;
						}
						else
						{
							patternState = PATTERN_STATE_RESTART;
						}
					}
					break;

				case '<':
					{
						if((int)strToken.length() < vNum[curToken])
						{
							this->vstrings.push_back(strToken);
							curToken++;
							patternState = PATTERN_STATE_NEXT;
						}
						else
						{
							patternState = PATTERN_STATE_RESTART;
						}
					}
					break;

				default:
					break;
				}
			}
			break;

		case PATTERN_STATE_MATCH:
			{
				return true;
			}
			break;

		case PATTERN_STATE_END:
			{ 
			} 
			break;

		default:

			break;

		} // end switch
	} // end while
	return true;
}

//------------------------------------------------------------------------------/

float Parser::IsFloat(const string fstring)
{
	const char *iter = fstring.c_str();
	const char *start = iter;

	while(isspace(*iter))
		iter++;

	if(*iter == '+' || *iter == '-')
		iter++;

	while(isdigit(*iter))
		iter++;

	if(*iter == '.')
	{
		iter++;
		
		while(isdigit(*iter))
			iter++;
	}

	if(*iter == 'e' || *iter == 'E' || *iter == 'd' || *iter == 'D')
	{
		iter++;

		if(*iter == '+' || *iter == '-')
			iter++;

		while(isdigit(*iter))
			iter++;
	}

	if(fstring.length() == iter - start)
	{
		return ((float)atof(start));
	}
	else
	{
		return (FLT_MIN);
	}
}

//------------------------------------------------------------------------------------/

int Parser::IsInt(const string istring)
{
	const char *iter = istring.c_str();
	const char *start = iter;

	while(isspace(*iter))
		iter++;

	if(*iter == '+' || *iter == '-')
		iter++;

	while(isdigit(*iter))
		iter++;

	if(istring.length() == iter - start)
	{
		return atoi(start);
	}
	else
	{
		return (INT_MIN);
	}
}

 const char *Parser::GetString()
 {
         m_ifs>>strToke;
         return strToke.c_str();
 }
 int Parser::GetInt()
 {
        int a;
        m_ifs>>a;
        return a;
 }


 float Parser::GetFloat()
 {
        float a;
        m_ifs>>a;
        return a;
 }

///////////////////////////////////////////////////////////////////
//
// �ǳ�Ա����
//
///////////////////////////////////////////////////////////////////

int StripChars(const char *stringIn, char *stringOut, const char *stripChars, bool isCase)
{
	if(NULL == stringIn || NULL == stringOut || 0 == strlen(stringIn))
	{
		return 0;
	}

	int stripLen = strlen(stripChars);		// ���������ַ����ַ����ĳ���

	if(NULL == stripChars || 0 == stripLen)
	{
		strcpy(stringOut, stringIn);
		return 0;
	}

	int indexIn = 0;			// �����ַ���������
	int indexOut = 0;			// �����ַ���������
	int numExtracts = 0;		// ɾ�����ַ���
	int indexStrip = 0;

	if(isCase)
	{
		while(stringIn[indexIn])
		{
			for(indexStrip=0; indexStrip<stripLen; indexStrip++)
			{
				if(stringIn[indexIn] == stripChars[indexStrip])
				{
					indexIn++;
					numExtracts++;
					break;
				}	
			}

			if(indexStrip >= stripLen)
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				stringOut[indexOut++] = stringIn[indexIn++];
			}
		}
	}
	else
	{
		while(stringIn[indexIn])
		{
			for(indexStrip=0; indexStrip<stripLen; indexStrip++)
			{
				if(toupper(stringIn[indexIn]) == toupper(stripChars[indexStrip]))
				{
					indexIn++;
					numExtracts++;
					break;
				}	
			}

			if(indexStrip >= stripLen)
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				stringOut[indexOut++] = stringIn[indexIn++];
			}
		}
	}

	stringOut[indexOut] = 0;

	return numExtracts;
}

//-----------------------------------------------------------------------------------------/

int StripChars(const string &stringIn, string &stringOut, const string &stripChars, bool isCase)
{
	if(stringIn.length() == 0)
	{
		return 0;
	}

	int stripLen = stripChars.length();		// ���������ַ����ַ����ĳ���

	if(0 == stripLen)
	{
		stringOut = stringIn;
		return 0;
	}

	int numExtracts = 0;		// ɾ�����ַ���

	string::const_iterator iterIn = stringIn.begin();
	string::iterator iterOut = stringOut.begin();
	string::const_iterator iterStrip = stripChars.begin();

	if(isCase)
	{
		while(iterIn != stringIn.end())
		{
			for(iterStrip=stripChars.begin(); iterStrip!=stripChars.end(); iterStrip++)
			{
				if(*iterIn == *iterStrip)
				{
					iterIn++;
					numExtracts++;
					break;
				}	
			}

			if(iterStrip == stripChars.end())
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				*iterOut++ = *iterIn++;
			}
		}
	}
	else
	{
		while(iterIn != stringIn.end())
		{
			for(iterStrip=stripChars.begin(); iterStrip!=stripChars.end(); iterStrip++)
			{
				if(toupper(*iterIn) == toupper(*iterStrip))
				{
					iterIn++;
					numExtracts++;
					break;
				}	
			}

			if(iterStrip == stripChars.end())
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				*iterOut++ = *iterIn++;
			}
		}

	}

	stringOut.erase(iterOut, stringOut.end());

	return numExtracts;
}

//---------------------------------------------------------------------------/

int ReplaceChars(const char *stringIn, char *stringOut, 
				 const char *replaceChars, char repChar, bool isCase)
{
	if(NULL == stringIn || NULL == stringOut || 0 == strlen(stringIn))
		return 0;

	int replaceLen = strlen(replaceChars);

	if(NULL == replaceChars || 0 == replaceLen)
	{
		strcpy(stringOut, stringIn);
		return 0;
	}

	int indexIn = 0;			// �����ַ���������
	int indexOut = 0;			// �����ַ���������
	int numReplace = 0;			// �滻���ַ���
	int indexReplace = 0;

	if(isCase)
	{
		while(stringIn[indexIn])
		{
			for(indexReplace=0; indexReplace<replaceLen; indexReplace++)
			{
				if(stringIn[indexIn] == replaceChars[indexReplace])
				{
					stringOut[indexOut++] = repChar;
					indexIn++;
					numReplace++;
					break;
				}	
			}

			if(indexReplace >= replaceLen)
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				stringOut[indexOut++] = stringIn[indexIn++];
			}
		}
	}
	else
	{
		while(stringIn[indexIn])
		{
			for(indexReplace=0; indexReplace<replaceLen; indexReplace++)
			{
				if(toupper(stringIn[indexIn]) == toupper(replaceChars[indexReplace]))
				{
					stringOut[indexOut++] = repChar;
					indexIn++;
					numReplace++;
					break;
				}	
			}

			if(indexReplace >= replaceLen)
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				stringOut[indexOut++] = stringIn[indexIn++];
			}
		}
	}

	stringOut[indexOut] = 0;

	return numReplace;
}

//------------------------------------------------------------------------------------------/

int ReplaceChars(const string &stringIn, string &stringOut, 
				 const string &replaceChars, char repChar, bool isCase)
{
	if(stringIn.length() == 0)
	{
		return 0;
	}

	int replaceLen = replaceChars.length();		// ���������ַ����ַ����ĳ���

	if(0 == replaceLen)
	{
		stringOut = stringIn;
		return 0;
	}

	int numReplace = 0;		// ɾ�����ַ���

	string::const_iterator iterIn = stringIn.begin();
	string::iterator iterOut = stringOut.begin();
	string::const_iterator iterReplace = replaceChars.begin();

	if(isCase)
	{
		while(iterIn != stringIn.end())
		{
			for(iterReplace=replaceChars.begin(); iterReplace!=replaceChars.end(); iterReplace++)
			{
				if(*iterIn == *iterReplace)
				{
					*iterOut++ = repChar;
					iterIn++;
					numReplace++;
					break;
				}	
			}

			if(iterReplace == replaceChars.end())
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				*iterOut++ = *iterIn++;
			}
		}
	}
	else
	{
		while(iterIn != stringIn.end())
		{
			for(iterReplace=replaceChars.begin(); iterReplace!=replaceChars.end(); iterReplace++)
			{
				if(toupper(*iterIn) == toupper(*iterReplace))
				{
					*iterOut++ = repChar;
					iterIn++;
					numReplace++;
					break;
				}	
			}

			if(iterReplace == replaceChars.end())
			{
				// ��ǰ�ַ����������ַ����������ֵ������ַ���

				*iterOut++ = *iterIn++;
			}
		}

	}

	stringOut.erase(iterOut, stringOut.end());

	return numReplace;
}