#include "stdafx.h"
#include "UBJSONFunctions.h"

//////////////////////////////////////////////////////////////////////////
namespace fmt_internal
{
	void WriteUBJsonKey(core::IChannel& Channel, const std::string& refKey)
	{
		char cKeyLen = (char)refKey.length();
		Channel.Access((void*)"i", 1);
		Channel.Access((void*)&cKeyLen, sizeof(cKeyLen));
		WriteUBJsonConst(Channel, refKey);
	}

	void WriteUBJsonKey(core::IChannel& Channel, const std::wstring& refKey)
	{
		char cKeyLen = (char)refKey.length();
		Channel.Access((void*)"i", 1);
		Channel.Access((void*)&cKeyLen, sizeof(cKeyLen));
		WriteUBJsonConst(Channel, refKey);
	}

	void WriteUBJsonValue(core::IChannel& Channel, const char& refValue)
	{
		Channel.Access((void*)"i", 1);
		Channel.Access((void*)& refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const short& refValue)
	{
		Channel.Access((void*)"I", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const int32_t& refValue)
	{
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const int64_t& refValue)
	{
		Channel.Access((void*)"L", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const BYTE& refValue)
	{
		Channel.Access((void*)"U", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const WORD& refValue)
	{
		Channel.Access((void*)"I", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const DWORD& refValue)
	{
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const QWORD& refValue)
	{
		Channel.Access((void*)"L", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const float& refValue)
	{
		Channel.Access((void*)"d", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const double& refValue)
	{
		Channel.Access((void*)"D", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const std::string& refValue)
	{
		std::string strValueU8 = UTF8FromMBS(refValue);
		const DWORD dwLength = (DWORD)strValueU8.length();

		Channel.Access((void*)"S", 1);
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&dwLength, sizeof(dwLength));
		if (dwLength)
			Channel.Access((void*)strValueU8.c_str(), dwLength);
	}

	void WriteUBJsonValue(core::IChannel& Channel, const std::wstring& refValue)
	{
		std::string strValueU8 = UTF8FromWCS(refValue);
		const DWORD dwLength = (DWORD)strValueU8.length();

		Channel.Access((void*)"S", 1);
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&dwLength, sizeof(dwLength));
		if (dwLength)
			Channel.Access((void*)strValueU8.c_str(), dwLength);
	}

	void WriteUBJsonValue(core::IChannel& Channel, const std::vector<BYTE>& refValue)
	{
		const DWORD dwSize = (DWORD)refValue.size();

		Channel.Access((void*)"B", 1);
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&dwSize, sizeof(dwSize));
		if (!refValue.empty())
			Channel.Access((void*)refValue.data(), refValue.size());
	}

	void WriteUBJsonConst(core::IChannel& Channel, std::string strValue)
	{
		std::string strValueU8 = UTF8FromMBS(strValue);
		Channel.Access((void*)strValueU8.c_str(), strValueU8.length());
	}

	void WriteUBJsonConst(core::IChannel& Channel, std::wstring strValue)
	{
		std::string strValueU8 = UTF8FromWCS(strValue);
		Channel.Access((void*)strValueU8.c_str(), strValueU8.length());
	}

	inline void ReadUBJConst(core::IChannel& Channel, void* pDest, size_t tDestSize)
	{
		size_t tReadSize = Channel.Access(pDest, tDestSize);
		if (tReadSize != tDestSize)
			throw exception_format(TEXT("Reading data failure"));
	}

	template<typename T>
	inline T ReadUBJConst(core::IChannel& Channel)
	{
		T ret;
		ReadUBJConst(Channel, &ret, sizeof(ret));
		return ret;
	}

	inline std::string ReadUBJValueWorker(core::IChannel& Channel, char cType)
	{
		std::string strRet;

		switch (cType)
		{
		case 'U':	strRet = StringFromA(ReadUBJConst<BYTE>(Channel));			break;
		case 'i':	strRet = StringFromA(ReadUBJConst<char>(Channel));			break;
		case 'I':	strRet = StringFromA(ReadUBJConst<short>(Channel));			break;
		case 'l':	strRet = StringFromA(ReadUBJConst<int>(Channel));			break;
		case 'L':	strRet = StringFromA(ReadUBJConst<int64_t>(Channel));		break;
		case 'd':	strRet = StringFromA(ReadUBJConst<float>(Channel));			break;
		case 'D':	strRet = StringFromA(ReadUBJConst<double>(Channel));		break;
		case 'S':
		{
			std::string strLength = ReadUBJValue(Channel);
			int nLength = IntFrom(strLength);
			if (nLength < 0)
				throw exception_format(TEXT("Invalid STRING length found, %s(%d)"), strLength.c_str(), nLength);

			strRet.resize(nLength);
			if (nLength)
				ReadUBJConst(Channel, (void*)strRet.c_str(), nLength);
		}	break;

		case 'B':
		{
			std::string strLength = ReadUBJValue(Channel);
			int nLength = IntFrom(strLength);
			if (nLength < 0)
				throw exception_format(TEXT("Invalid BINARY length found, %s(%d)"), strLength.c_str(), nLength);

			strRet.resize(nLength);
			if (nLength)
				ReadUBJConst(Channel, (void*)strRet.c_str(), nLength);
		}	break;

		case '[':
			strRet = "[";
			break;

		case '{':
			strRet = "{";
			break;

		default:
			throw exception_format(TEXT("Unexpected VALUE type found, %c"), cType);
		}

		return strRet;
	}

	std::string ReadUBJValue(core::IChannel& Channel)
	{
		char cType = ReadUBJConst<char>(Channel);
		return ReadUBJValueWorker(Channel, cType);
	}

	std::string ReadUBJValue(core::IChannel& Channel, char cType)
	{
		return ReadUBJValueWorker(Channel, cType);
	}

	ECODE ParseUBJson(core::IChannel& Channel, ST_UBJ_NODE& outRoot, std::tstring& outErrMsg)
	{
		std::stack<ST_UBJ_NODE*> stackNode;
		try
		{
			if ('{' != ReadUBJConst<char>(Channel))
				throw exception_format(TEXT("UBJson start char is not `{`"));

			stackNode.push(&outRoot);

			while (!stackNode.empty())
			{
				ST_UBJ_NODE* pCurNode = stackNode.top();

				const char cType = ReadUBJConst<char>(Channel);
				switch (cType)
				{
				case 'i':
				{
					ST_UBJ_NODE child;
					BYTE len = ReadUBJConst<BYTE>(Channel);
					if (0 == len)
						throw exception_format(TEXT("KEY LEN is Zero"));

					child.strKey.resize(len);
					ReadUBJConst(Channel, (void*)child.strKey.data(), len);

					child.strValue = ReadUBJValue(Channel);
					pCurNode->Children.push_back(child);

					if ("{" == child.strValue)
						stackNode.push(&pCurNode->Children.back());

					if ("[" == child.strValue)
						stackNode.push(&pCurNode->Children.back());
				}	break;

				case '{':
					pCurNode->Children.push_back(ST_UBJ_NODE());
					stackNode.push(&pCurNode->Children.back());
					break;

				case '}':
					stackNode.pop();
					break;

				case ']':
					stackNode.pop();
					break;

				default: {
					ST_UBJ_NODE child;
					child.strValue = ReadUBJValue(Channel, cType);
					pCurNode->Children.push_back(child);
				}	break;
				}
			}
		}
		catch (const std::exception& e)
		{
			Log_Error("%s", e.what());
			outErrMsg = TCSFromMBS(e.what());
			return EC_INVALID_DATA;
		}
		return EC_SUCCESS;
	}
}
