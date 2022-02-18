#include <iostream>
#include <exception>

#include "crypto-exchange-client-deribit/client.hpp"
#include "crypto-exchange-client-deribit/apiMessage.hpp"

#include "api-secret.hpp"


int main()
{
	try {
		as::cryptox::deribit::Client client(
			as::cryptox::deribit::ApiClientId(),
			as::cryptox::deribit::ApiClientSecret(),
			AS_T( "https://test.deribit.com/ws/api/v2" ) );

		client.run( []( as::cryptox::Client & c ) {
			std::cout << "ready" << std::endl;
		} );
	}
	catch ( const std::exception & x ) {
		std::cerr << "error: " << x.what() << std::endl;
	}
	catch ( ... ) {
		std::cerr << "error" << std::endl;
	}

	return 0;
}
