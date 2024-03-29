#pragma once
#include "tankett_shared.h"

#include <vector>
#include <map>
#include <memory>

using namespace alpha;
namespace client
{

class NetworkManager
{
public:
	enum class ConnectionState
	{
		None,
		Discovering,
		Challenging,
		Connected,
	};

public:
	NetworkManager();
	~NetworkManager();

	void send();
	void receive();

	void setState(const ConnectionState& state) { mState = state; }
	ConnectionState getState() { return mState; }
	void resetNetworkManager();

	std::vector<std::unique_ptr<::tankett::network_message_header>>& getReceivedMessages() { return mReceivedMessages; }
	std::vector<std::unique_ptr<::tankett::network_message_header>>& getSendMessageQueue() { return mSendMessageQueue; }

	void pushMessage(::std::unique_ptr<::tankett::network_message_header> message);

	void clearReceivedMessages();

	const std::map<ip_address, ::tankett::protocol_connection_challenge>& getChallenges() const { return mChallenges; }
	void acceptChallenge(const ip_address& addr);
private:
	void processMessages();

private:
	ConnectionState mState;
	uint64_t mClientKey;
	uint64_t mServerKey;
	ip_address mServerAddr;
	udp_socket mSocket;
	ip_address mLocalAddr;
	std::vector<std::unique_ptr<::tankett::network_message_header>> mSendMessageQueue;
	std::vector<std::unique_ptr<::tankett::network_message_header>> mReceivedMessages;
	crypt::xorinator mXorinator;
	uint32_t mClientSequence;
	uint32_t mServerSequence;
	std::map<ip_address, ::tankett::protocol_connection_challenge> mChallenges;
};
}