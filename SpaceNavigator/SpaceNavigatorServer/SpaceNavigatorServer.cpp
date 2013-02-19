// SpaceNavigatorServer.cpp
// A small example server for the 3D-Connexion Space Navigator
//
// Author: Lars Bilke

#include <vrpn_3DConnexion.h>

vrpn_Connection *connection;
vrpn_3DConnexion_Navigator *nav;

void shutdown()
{
	if(connection)	{connection->removeReference(); connection = NULL; }
}

int main(int argc, char **argv)
{
	// get connection string from command line argument
	char *computerName = "";
	if(argc > 1)
	{
		computerName = argv[1];
	}

	// values of 1.0 or higher lead to wrong outputs
	double milli_sleep_time = 0.9;
	int port = vrpn_DEFAULT_LISTEN_PORT_NO;

	connection = new vrpn_Connection(port);

	// Create new Navigator device object
	// The name has to be used by the client application
	if((nav = new vrpn_3DConnexion_Navigator(computerName, connection)) == NULL)
	{
		printf("Can´t create new device\n");
		return -1;
	}

	// run the mainloops
	while (true)
	{
		nav->mainloop();
		connection->mainloop();
		if(!connection->doing_okay())
			shutdown();

		// Sleep so we don't eat the CPU
		if (milli_sleep_time >= 0.0) 
			vrpn_SleepMsecs(milli_sleep_time);
	}

	shutdown();

	return 0;
}