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

/// client.hpp
///
/// 0.0 - created (Denis Rozhkov <denis@rozhkoff.com>)
///

#ifndef __CRYPTO_EXCHANGE_CLIENT_DERIBIT__CLIENT__H
#define __CRYPTO_EXCHANGE_CLIENT_DERIBIT__CLIENT__H


#include "crypto-exchange-client-core/httpClient.hpp"
#include "crypto-exchange-client-core/client.hpp"


namespace as::cryptox::deribit {

	class Client : public as::cryptox::Client {
	protected:
		as::t_string m_clientId;
		as::t_string m_clientSecret;

	protected:
		void wsErrorHandler(
			as::WsClient &, int, const as::t_string & ) override;

		void wsHandshakeHandler( as::WsClient & ) override;
		bool wsReadHandler( as::WsClient &, const char *, size_t ) override;

		void initSymbolMap() override
		{
		}

		void initWsClient() override;

	public:
		Client( const as::t_string & clientId = AS_T( "" ),
			const as::t_string & clientSecret = AS_T( "" ),
			const as::t_string & apiUrl = AS_T(
				"https://www.deribit.com/ws/api/v2" ) )
			: as::cryptox::Client( apiUrl, apiUrl )
			, m_clientId( clientId )
			, m_clientSecret( clientSecret )
		{
		}

		void run( const t_exchangeClientReadyHandler & handler ) override;

		void subscribePriceBookTicker( as::cryptox::Symbol symbol,
			const t_priceBookTickerHandler & handler ) override;
	};

}


#endif
