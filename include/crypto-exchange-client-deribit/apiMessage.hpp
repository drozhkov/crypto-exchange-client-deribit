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

/// apiMessage.hpp
///
/// 0.0 - created (Denis Rozhkov <denis@rozhkoff.com>)
///

#ifndef __CRYPTO_EXCHANGE_CLIENT_DERIBIT__API_MESSAGE__H
#define __CRYPTO_EXCHANGE_CLIENT_DERIBIT__API_MESSAGE__H


#include <unordered_map>
#include <mutex>

#include "boost/json.hpp"

#include "crypto-exchange-client-core/core.hpp"
#include "crypto-exchange-client-core/exception.hpp"
#include "crypto-exchange-client-core/apiMessage.hpp"


namespace as::cryptox::deribit {

	class ApiMessage : public ::as::cryptox::ApiMessage<ApiMessage> {
	public:
		static const as::cryptox::t_api_message_type_id TypeIdAuth = 100;

	protected:
		inline static std::unordered_map<t_request_id,
			as::cryptox::t_api_message_type_id>
			s_requestIdTypeMap;

		inline static std::mutex s_requestIdTypeMapSync;

	protected:
		static boost::json::object create( const as::t_stringview & methodName,
			as::cryptox::t_api_message_type_id typeId )
		{

			auto reqId = RequestId();

			boost::json::object o;
			o["jsonrpc"] = "2.0";
			o["id"] = reqId;
			o["method"] = methodName.data();


			{
				std::lock_guard<std::mutex> l( s_requestIdTypeMapSync );
				s_requestIdTypeMap[reqId] = typeId;
			}

			return o;
		}

	public:
		ApiMessage( t_api_message_type_id typeId )
			: as::cryptox::ApiMessage<ApiMessage>( typeId )
		{
		}

		static as::cryptox::t_api_message_type_id MessageTypeId(
			t_request_id reqId )
		{

			auto it = s_requestIdTypeMap.find( reqId );

			if ( s_requestIdTypeMap.end() == it ) {
				return 0;
			}

			as::cryptox::t_api_message_type_id result = it->second;

			{
				std::lock_guard<std::mutex> l( s_requestIdTypeMapSync );
				s_requestIdTypeMap.erase( reqId );
			}

			return result;
		}

		static as::t_string AuthClientSignature(
			const as::t_stringview & clientId,
			const as::t_stringview & clientSecret,
			t_timespan ts )
		{

			auto ts_s = AS_TOSTRING( ts );
			auto s = ts_s + '\n' + ts_s + '\n';
			auto signature = hmacSha256( clientSecret, s );
			auto signature_s =
				toHex( t_buffer( signature.data(), signature.size() ) );

			auto o = create( AS_T( "public/auth" ), TypeIdAuth );
			o["params"] = { { "grant_type", "client_signature" },
				{ "client_id", clientId },
				{ "timestamp", ts_s },
				{ "nonce", ts_s },
				{ "signature", signature_s } };

			return boost::json::serialize( o );
		}
	};

	class ApiMessageError : public ApiMessage {
	public:
		ApiMessageError()
			: ApiMessage( TypeIdError )
		{
		}
	};

	class ApiMessageAuth : public ApiMessage {
	public:
		ApiMessageAuth( bool isGood )
			: ApiMessage( TypeIdAuth )
		{
			m_isGood = isGood;
		}
	};

}


#endif
