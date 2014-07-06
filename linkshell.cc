/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "link_queue.hh"
#include "packetshell.cc"

using namespace std;

int main( int argc, char *argv[] )
{
    try {
        /* clear environment while running as root */
        char ** const user_environment = environ;
        environ = nullptr;

        check_requirements( argc, argv );

        if ( argc < 3 ) {
            throw Exception( "Usage", string( argv[ 0 ] ) + " uplink-file downlink-file [command...]" );
        }

        const std::string uplink_filename = argv[ 1 ];
        const std::string downlink_filename = argv[ 2 ];

        vector< string > command;

        if ( argc == 3 ) {
            command.push_back( shell_path() );
        } else {
            for ( int i = 3; i < argc; i++ ) {
                command.push_back( argv[ i ] );
            }
        }

        PacketShell<LinkQueue> link_shell_app( "link" );

        link_shell_app.start_uplink( "[link, up=" + uplink_filename + ", down=" + downlink_filename + "] ",
                                     user_environment,
                                     command,
                                     uplink_filename );
        link_shell_app.start_downlink( downlink_filename );
        return link_shell_app.wait_for_exit();
    } catch ( const Exception & e ) {
        e.perror();
        return EXIT_FAILURE;
    }
}
