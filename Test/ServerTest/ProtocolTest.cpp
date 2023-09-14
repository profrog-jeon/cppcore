#include "stdafx.h"

struct ST_TEST_PACKET : public IFormatterObject
{
	static const int ID = 1;

	int a;
	int b;
	std::tstring strContext;
	std::vector<BYTE> vecBinary;

	void OnSync(IFormatter& formatter)
	{
		formatter
			+ sPair(TEXT("a"), a)
			+ sPair(TEXT("b"), b)
			+ sPair(TEXT("Context"), strContext)
			+ sPair(TEXT("Binary"), vecBinary)
			;
	}
};

TEST(ServerTest, BinProtocolTest)
{
	ST_TEST_PACKET original;
	original.a = 1;
	original.b = 2;
	original.strContext = TEXT("hello?");
	original.vecBinary.resize(100, 5);

	std::vector<BYTE> vecBody;
	EXPECT_EQ(EC_SUCCESS, Packetize(ST_TEST_PACKET::ID, &original, vecBody));

	ST_TEST_PACKET restored;
	EXPECT_EQ(EC_SUCCESS, Unpacketize(vecBody, ST_TEST_PACKET::ID, &restored));

	EXPECT_EQ(original.a, restored.a);
	EXPECT_EQ(original.b, restored.b);
	EXPECT_EQ(original.strContext, restored.strContext);
	ASSERT_EQ(original.vecBinary.size(), restored.vecBinary.size());
	EXPECT_EQ(0, memcmp(original.vecBinary.data(), restored.vecBinary.data(), original.vecBinary.size()));
}

class CJsonProtocolConnection : public CSyncConnection
{
	CSyncTCPSocket m_Socket;
	CJsonProtocol m_Protocol;

public:
	CJsonProtocolConnection(void)
	: CSyncConnection(&m_Socket)
	, m_Socket()
	, m_Protocol(&m_Socket)
	{}
	~CJsonProtocolConnection(void)	{}

	void OnConnect(void)
	{
	}

	void OnClose(void)
	{
	}

	void OnRecv(void)
	{
		ST_TEST_PACKET packet;
		EXPECT_EQ(EC_SUCCESS, m_Protocol.RecvPacket(&packet));
		EXPECT_EQ(EC_SUCCESS, m_Protocol.SendPacket(&packet));
	}
};

TEST(ServerTest, JsonProtocolTest)
{
	ST_SYNCSERVER_INIT stInit;
	stInit.wPort = 61503;
	stInit.Connections.push_back(new CJsonProtocolConnection());
	stInit.Connections.push_back(new CJsonProtocolConnection());
	stInit.Connections.push_back(new CJsonProtocolConnection());

	CSyncServer server;
	ASSERT_EQ(EC_SUCCESS, server.StartUp(stInit));
	Sleep(100);
	{
		ST_TEST_PACKET original;
		original.a = 1;
		original.b = 2;
		original.strContext = TEXT("hello?");
		original.vecBinary.resize(100, 5);

		CSyncTCPSocket client;
		CJsonProtocol protocol(&client);
		EXPECT_EQ(EC_SUCCESS, protocol.Connect("127.0.0.1", stInit.wPort, 1000));
		EXPECT_EQ(EC_SUCCESS, protocol.SendPacket(&original));

		ST_TEST_PACKET restored;
		EXPECT_EQ(EC_SUCCESS, protocol.RecvPacket(&restored));

		EXPECT_EQ(original.a, restored.a);
		EXPECT_EQ(original.b, restored.b);
		EXPECT_EQ(original.strContext, restored.strContext);

		// JSON 프로토콜에는 바이너리 첨부가 안됨!
		//ASSERT_EQ(original.vecBinary.size(), restored.vecBinary.size());
		//EXPECT_EQ(0, memcmp(original.vecBinary.data(), restored.vecBinary.data(), original.vecBinary.size()));

		protocol.Close();
	}
	server.ShutDown();
}


class CUBJProtocolConenction : public CSyncConnection
{
	CSyncTCPSocket m_Socket;
	CUBJsonProtocol m_Protocol;

public:
	CUBJProtocolConenction(void)
		: CSyncConnection(&m_Socket)
		, m_Socket()
		, m_Protocol(&m_Socket)
	{}
	~CUBJProtocolConenction(void) {}

	void OnConnect(void)
	{
	}

	void OnClose(void)
	{
	}

	void OnRecv(void)
	{
		ST_TEST_PACKET packet;
		EXPECT_EQ(EC_SUCCESS, m_Protocol.RecvPacket(&packet));
		EXPECT_EQ(EC_SUCCESS, m_Protocol.SendPacket(&packet));
	}
};

TEST(ServerTest, UBJsonProtocolTest)
{
	ST_SYNCSERVER_INIT stInit;
	stInit.wPort = 61503;
	stInit.Connections.push_back(new CUBJProtocolConenction());
	stInit.Connections.push_back(new CUBJProtocolConenction());
	stInit.Connections.push_back(new CUBJProtocolConenction());

	CSyncServer server;
	ASSERT_EQ(EC_SUCCESS, server.StartUp(stInit));
	Sleep(100);
	{
		ST_TEST_PACKET original;
		original.a = 1;
		original.b = 2;
		original.strContext = TEXT("hello?");
		original.vecBinary.resize(100, 5);

		CSyncTCPSocket client;
		CUBJsonProtocol protocol(&client);
		EXPECT_EQ(EC_SUCCESS, protocol.Connect("127.0.0.1", stInit.wPort, 1000));
		EXPECT_EQ(EC_SUCCESS, protocol.SendPacket(&original));

		ST_TEST_PACKET restored;
		EXPECT_EQ(EC_SUCCESS, protocol.RecvPacket(&restored));

		EXPECT_EQ(original.a, restored.a);
		EXPECT_EQ(original.b, restored.b);
		EXPECT_EQ(original.strContext, restored.strContext);
		ASSERT_EQ(original.vecBinary.size(), restored.vecBinary.size());
		EXPECT_EQ(0, memcmp(original.vecBinary.data(), restored.vecBinary.data(), original.vecBinary.size()));

		protocol.Close();
	}
	server.ShutDown();
}