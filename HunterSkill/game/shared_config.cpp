#include "shared_config.h"
#include "../utils/mem.h"


options::config* current_config = nullptr;
options::config::config( )
{
	current_config			= this;
}

options::config* options::config::i( )
{
	if ( !current_config )
		current_config = new options::config( );
	return current_config;
}

options::reversed* current_reversed = nullptr;
options::reversed::reversed( )
{
	current_reversed	= this;

}

options::reversed* options::reversed::i( )
{
	if ( !current_reversed ) new reversed( );
	return current_reversed;
}

void options::config::save_config( const uint64_t time_now ) const
{

}


void options::config::load_config( )
{

}