// Set up a Doxygen group.
/** @addtogroup Main
 *  @{
 */

#include "SDKClient.hpp"

/// @brief The entry point and main loop of the application.
/// @param argc standard main argument parameter
/// @param argv standard main argument parameter
/// @return ClientReturnCode_Success if successful,
/// ClientReturnCode_FailedToResizeWindow if the client was not able to resize the console window,
/// ClientReturnCode_FailedToInitialize if the system was not able to initialize the SDK,
/// ClientReturnCode_FailedToFindHosts if no hosts were found,
/// ClientReturnCode_FailedToConnect if the sdk could not connect to Core,
/// ClientReturnCode_UnrecognizedStateEncountered if un unrecognized state was encountered,
/// ClientReturnCode_FailedToShutDown if it failed to shut down the SDK wrapper,
/// ClientReturnCode_FailedToRestart if the SDK was not able to reconnect to Core.
int main(int argc, char* argv[])
{
	ClientReturnCode t_Result;
	SDKClient t_SDKClient;

	t_Result = t_SDKClient.Initialize();

	if (t_Result != ClientReturnCode::ClientReturnCode_Success)
	{
		t_SDKClient.ShutDown();
		return static_cast<int>(t_Result); // Returning initialise failure state
	}

	t_Result = t_SDKClient.Run();
	if (t_Result != ClientReturnCode::ClientReturnCode_Success)
	{
		t_SDKClient.ShutDown();
		return static_cast<int>(t_Result); // Returning run failure state
	}

	t_Result = t_SDKClient.ShutDown();
	return static_cast<int>(t_Result);
}

// Close the Doxygen group.
/** @} */
