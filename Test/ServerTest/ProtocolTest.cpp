#include "stdafx.h"

struct ST_TEST_PACKET : public IFormatterObject
{
	static const int ID = 1;

	int a;
	int b;
	std::tstring strContext;

	void OnSync(IFormatter& formatter)
	{
		formatter
			+ sPair(TEXT("a"), a)
			+ sPair(TEXT("b"), b)
			+ sPair(TEXT("Context"), strContext)
			;
	}
};

TEST(ServerTest, BinProtocolTest)
{
	ST_TEST_PACKET original;
	original.a = 1;
	original.b = 2;
	original.strContext = TEXT("hello?");

	std::vector<BYTE> vecBody;
	EXPECT_EQ(EC_SUCCESS, Packetize(ST_TEST_PACKET::ID, &original, vecBody));

	ST_TEST_PACKET restored;
	EXPECT_EQ(EC_SUCCESS, Unpacketize(vecBody, ST_TEST_PACKET::ID, &restored));

	EXPECT_EQ(original.a, restored.a);
	EXPECT_EQ(original.b, restored.b);
	EXPECT_EQ(original.strContext, restored.strContext);
}

class CTestConenction : public CSyncConnection
{
	CSyncTCPSocket m_Socket;
	CJsonProtocol m_Protocol;

public:
	CTestConenction(void)
	: CSyncConnection(&m_Socket)
	, m_Socket()
	, m_Protocol(&m_Socket)
	{}
	~CTestConenction(void)	{}

	void	OnConnect(void)
	{
	}

	void	OnClose(void)
	{
	}

	void	OnRecv(void)
	{
		ST_TEST_PACKET packet;
		EXPECT_EQ(EC_SUCCESS, m_Protocol.RecvPacket(&packet));
		EXPECT_EQ(EC_SUCCESS, m_Protocol.SendPacket(&packet));
	}
};

TEST(ServerTest, JsonProtocolSocketTest)
{
	ST_SYNCSERVER_INIT stInit;
	stInit.wPort = 61503;
	stInit.Connections.push_back(new CTestConenction());
	stInit.Connections.push_back(new CTestConenction());
	stInit.Connections.push_back(new CTestConenction());

	CSyncServer server;
	ASSERT_EQ(EC_SUCCESS, server.StartUp(stInit));
	Sleep(100);
	{
		ST_TEST_PACKET original;
		original.a = 1;
		original.b = 2;
		original.strContext = TEXT("hello?");

		CSyncTCPSocket client;
		CJsonProtocol protocol(&client);
		EXPECT_EQ(EC_SUCCESS, protocol.Connect("127.0.0.1", stInit.wPort, 1000));
		EXPECT_EQ(EC_SUCCESS, protocol.SendPacket(&original));

		ST_TEST_PACKET restored;
		EXPECT_EQ(EC_SUCCESS, protocol.RecvPacket(&restored));

		EXPECT_EQ(original.a, restored.a);
		EXPECT_EQ(original.b, restored.b);
		EXPECT_EQ(original.strContext, restored.strContext);

		protocol.Close();
	}
	server.ShutDown();
}

TEST(ServerTest, JsonProtocolSocketPeekTest)
{
	ST_SYNCSERVER_INIT stInit;
	stInit.wPort = 61503;
	stInit.Connections.push_back(new CTestConenction());
	stInit.Connections.push_back(new CTestConenction());
	stInit.Connections.push_back(new CTestConenction());

	CSyncServer server;
	ASSERT_EQ(EC_SUCCESS, server.StartUp(stInit));
	Sleep(100);
	{
		ST_TEST_PACKET original;
		original.a = 1;
		original.b = 2;
		original.strContext = TEXT("hello?");

		CSyncTCPSocket client;
		CJsonProtocol protocol(&client);
		EXPECT_EQ(EC_SUCCESS, protocol.Connect("127.0.0.1", stInit.wPort, 1000));
		EXPECT_EQ(EC_SUCCESS, protocol.SendPacket(&original));

		ST_PACKET_HEADER header;
		EXPECT_EQ(EC_SUCCESS, protocol.PeekHeader(header));
		EXPECT_EQ(header.dwID, original.ID);

		ST_TEST_PACKET restored;
		EXPECT_EQ(EC_SUCCESS, protocol.RecvPacket(&restored));

		EXPECT_EQ(original.a, restored.a);
		EXPECT_EQ(original.b, restored.b);
		EXPECT_EQ(original.strContext, restored.strContext);

		protocol.Close();
	}
	server.ShutDown();
}