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

/// wsMessage.cpp
///
/// 0.0 - created (Denis Rozhkov <denis@rozhkoff.com>)
///

#include "crypto-exchange-client-core/logger.hpp"

#include "crypto-exchange-client-deribit/wsMessage.hpp"


namespace as::cryptox::deribit {

	std::shared_ptr<as::cryptox::ApiMessageBase> WsMessage::deserialize(
		const char * data, size_t size )
	{

		auto v = boost::json::parse( { data, size } );
		auto & o = v.get_object();

		if ( !o.contains( "id" ) ) {
			return s_unknown;
		}

		auto isError = o.contains( "error" );

		t_request_id reqId = static_cast<t_request_id>( o["id"].get_int64() );
		auto messageTypeId = ApiMessage::MessageTypeId( reqId );


		if ( 0 == messageTypeId ) {
			return (
				isError ? std::make_shared<ApiMessageError>() : s_unknown );
		}

		switch ( messageTypeId ) {
			case ApiMessage::TypeIdAuth:
				return std::make_shared<ApiMessageAuth>( !isError );
				break;

			default:
				break;
		}

		// WsMessage * r = nullptr;

		// auto & typeName = o["type"].get_string();

		// if ( "welcome" == typeName ) {
		//	r = new WsMessageWelcome;
		//}
		// if ( "message" == typeName ) {
		//	auto & topicName = o["topic"].get_string();

		//	if ( topicName.starts_with( "/market/ticker:" ) ) {
		//		r = new WsMessagePriceBookTicker;
		//	}
		//}

		// if ( nullptr == r ) {
		//	return s_unknown;
		//}

		// r->deserialize( o );

		// return std::shared_ptr<as::cryptox::WsMessage>( r );

		return s_unknown;
	}

	//

}
