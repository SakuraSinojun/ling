//////////////////////////////////////////////////////////////////
//
// name: Parser.h
// func: 文件分析器类的声明
// date: 2010.10.20	
//
///////////////////////////////////////////////////////////////////

#include "parser.h"

///////////////////////////////////////////////////////////////////
//
// 成员函数
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
	// 防止有没有关闭的文件

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
		this->strLine = "";		// 将字符串置空

		// 跳过空行

		if(mode & PARSER_STRIP_EMPTY_LINES)
		{
			while(true)
			{
				// 没有获取到文件行

				if(!getline(m_ifs, this->strLine))
					break;

				string::size_type i = this->strLine.find_first_not_of(' ');

				if(string::npos != i)
				{
					// 将此行前边的空格删除

					this->strLine.erase(0, i);

					// 删除注释

					if(mode & PARSER_STRIP_COMMENTS)
					{
						// 第一个注释的索引

						string::size_type pos = this->strLine.find_first_of(m_strComment);

						// npos为find返回的一个特殊值，若返回此值，则说明查找没有匹配
						// 此时已经得到所要的行，跳出寻找空行循环

						if(string::npos == pos)
							break;

						// 这是一个注释行，继续下一行

						if(0 == pos)
							continue;

						else
						{
							// 此时注释行在最后边，将注释开始位置置为NULL；

							this->strLine.erase(strLine.begin() + pos, strLine.end());

							break;
						}

					} // end if strip comments

					// 已经得到了行内容，跳出循环

					break;

				} // end if not null string

			} // end while

		} //end if strip empty line
		else
		{
			// 只是简单的得到一行

			getline(m_ifs, this->strLine);

			if(mode & PARSER_STRIP_COMMENTS)
			{
				// 第一个注释的索引

				string::size_type pos = this->strLine.find_first_of(m_strComment);


				if(string::npos != pos)
				{
					// 此时删除注释

					this->strLine.erase(strLine.begin() + pos, strLine.end());
				}

			} // end if strip comments
		}

		// 得到了一行

		if(!this->strLine.empty())
		{
			m_lineNum++;

			// 删除行尾空格

			if(mode & PARSER_STRIP_WS_ENDS)
			{
				// 删除行首空格

				string::size_type pos = this->strLine.find_first_not_of(' ');
				this->strLine.erase(0, pos);

				// 删除行尾空格

				for(string::iterator iter = this->strLine.end() - 1;iter != this->strLine.begin(); iter--)
				{
					if(*iter != ' ')
					{
						// 发现从行尾起第一个非空格，将此后的字符串删除

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
	// 此函数用于从buffer中提取匹配pattern的格式
	// pattern字符串可以包含以下信息
	// ['string']	提取字符串string
	// [f]			提取一个浮点数
	// [i]			提取一个整数
	// [s=ddd]		s为固定标识，ddd为一个10进制数，用于提取一个长度为ddd的字符串
	// [s<ddd]		提取长度小于ddd的字符串
	// [s>ddd]		提取长度大于ddd的字符串
	// 例如要匹配字符串"vertex: 34.234 56.34 12.4"
	// 需要用这样的格式 ['vertex'] [f] [f] [f]来提取
	// 提取后的数据按顺序储存在成员变量中

	if(buffer.empty() || pattern.empty())
	{
		return false;
	}

	string strType;				// 储存pattern中有关类型的字符
	string strOperator;			// 储存pattern中有关运算符的字符
	vector<string> vString;		// 储存pattern中匹配的字符串
	vector<int> vNum;			// 储存pattern中说明的字符串长度信息

	string::size_type startPos = 0;	// 迭代过程的开始位置
	string::size_type endPos;	// 迭代过程的结束位置

	int tokenNum = 0;				// 总共需要提取的数据的数量

	// 分析pattern阶段，并根据pattern的相关数据，把buffer中的相关字符串提取出来

	while(true)
	{
		// 跳过行开始位置的空格

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

					// 没次提取都要对齐，没有提取的部分要用空白占据
					// string用空串，字符用'\0'，数字用0
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
					// 跳过当前的 '

					startPos += 2;  

					// 寻找结束标识

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
				
			case 's':      // 长度匹配string
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

						// 越过s= 提取后边的数字

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

		// 此时全部提取完毕

		if(startPos >= pattern.length())
			break;

	}// end while

	// 将提取出来的字符串进行转化的阶段

	string strWork = buffer;	// 工作字符串
	int patternState = PATTERN_STATE_INIT;	// 记录向前状态
	int curToken = 0;				// 记录当前正在处理序列中的第几个位置
	string strToken;				// 存储需要临时转化的字符串	
	bool isFirstPass = true;			// 在NEXT过程中判断是否需要将重新开始的起点赋值
	string::size_type restartPos = 0;		// 重新处理的起点

	while(true)
	{
		switch(patternState)
		{
		case PATTERN_STATE_INIT:		// 初始化阶段
			{
				strWork = buffer;

				startPos = 0;
				endPos = 0;
				isFirstPass = true;
				restartPos = 0;

				// 将成员变量都置空
				this->vints.clear();
				this->vfloats.clear();
				this->vstrings.clear();

				patternState = PATTERN_STATE_NEXT;
			}
			break;

		case PATTERN_STATE_RESTART:		// 重新开始阶段，当前的处理发生错误，跳过当前
			{
				curToken = 0;
				isFirstPass = true;
				if(endPos > buffer.length())
					return false;
				startPos = endPos = restartPos;

				patternState = PATTERN_STATE_NEXT;
			}
			break;
			
		case PATTERN_STATE_NEXT:          // 进行下一个字符串的处理
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
// 非成员函数
//
///////////////////////////////////////////////////////////////////

int StripChars(const char *stringIn, char *stringOut, const char *stripChars, bool isCase)
{
	if(NULL == stringIn || NULL == stringOut || 0 == strlen(stringIn))
	{
		return 0;
	}

	int stripLen = strlen(stripChars);		// 储存跳过字符的字符串的长度

	if(NULL == stripChars || 0 == stripLen)
	{
		strcpy(stringOut, stringIn);
		return 0;
	}

	int indexIn = 0;			// 传入字符串的索引
	int indexOut = 0;			// 传出字符串的索引
	int numExtracts = 0;		// 删除的字符数
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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

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

	int stripLen = stripChars.length();		// 储存跳过字符的字符串的长度

	if(0 == stripLen)
	{
		stringOut = stringIn;
		return 0;
	}

	int numExtracts = 0;		// 删除的字符数

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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

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

	int indexIn = 0;			// 传入字符串的索引
	int indexOut = 0;			// 传出字符串的索引
	int numReplace = 0;			// 替换的字符数
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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

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

	int replaceLen = replaceChars.length();		// 储存跳过字符的字符串的长度

	if(0 == replaceLen)
	{
		stringOut = stringIn;
		return 0;
	}

	int numReplace = 0;		// 删除的字符数

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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

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
				// 当前字符不在跳过字符的序列里，赋值给输出字符串

				*iterOut++ = *iterIn++;
			}
		}

	}

	stringOut.erase(iterOut, stringOut.end());

	return numReplace;
}