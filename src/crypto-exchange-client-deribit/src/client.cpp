/*
MIT License
Copyright (c) 2022 Denis Rozhkov <denis@rozhkoff.com>
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/// client.cpp
///
/// 0.0 - created (Denis Rozhkov <denis@rozhkoff.com>)
///

#include "boost/json.hpp"

#include "crypto-exchange-client-core/logger.hpp"
#include "crypto-exchange-client-core/exception.hpp"

#include "crypto-exchange-client-deribit/client.hpp"
#include "crypto-exchange-client-deribit/apiMessage.hpp"
#include "crypto-exchange-client-deribit/wsMessage.hpp"


namespace as::cryptox::deribit {

	void Client::wsErrorHandler(
		WsClient & client, int code, const as::t_string & message )
	{
	}

	void Client::wsHandshakeHandler( WsClient & client )
	{
		if ( m_clientId.empty() ) {
			AS_CALL( m_clientReadyHandler, *this );
		}
		else {
			auto buffer = ApiMessage::AuthClientSignature( m_clientId,
				m_clientSecret,
				UnixTs<std::chrono::milliseconds>() );

			client.writeAsync( buffer.c_str(), buffer.length() );
		}

		client.readAsync();
	}

	bool Client::wsReadHandler(
		WsClient & client, const char * data, size_t size )
	{

		try {
			std::string s( data, size );
			std::cout << s << std::endl;

			auto message = WsMessage::deserialize( data, size );

			switch ( message->TypeId() ) {
				case ApiMessage::TypeIdAuth: {
					auto m = static_cast<ApiMessageAuth *>( message.get() );

					if ( m->IsGood() ) {
						AS_CALL( m_clientReadyHandler, *this );
					}
					else {
						AS_CALL( m_clientErrorHandler, *this );
						return false;
					}
				}

				break;
			}
		}
		catch ( ... ) {
		}

		return true;
	}

	void Client::initWsClient()
	{
		as::cryptox::Client::initWsClient();
	}

	void Client::run( const t_exchangeClientReadyHandler & handler )
	{
		m_clientReadyHandler = handler;

		while ( true ) {
			initWsClient();
			m_wsClient->run();
		}
	}

	void Client::subscribePriceBookTicker(
		as::cryptox::Symbol symbol, const t_priceBookTickerHandler & handler )
	{

		as::cryptox::Client::subscribePriceBookTicker( symbol, handler );
		// auto buffer = WsMessage::Subscribe(
		//	std::string( "/market/ticker:" ) + SymbolName( symbol ) );

		// m_wsClient->writeAsync( buffer.c_str(), buffer.length() );
	}
}
