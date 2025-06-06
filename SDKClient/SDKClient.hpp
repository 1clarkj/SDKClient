#ifndef _SDK_CLIENT_HPP_
#define _SDK_CLIENT_HPP_

// Set up a Doxygen group.
/** @addtogroup SDKClient
 *  @{
 */

#include <chrono> // Used for haptic commands.
#include <cmath> // Used for rounding float values.
#include <iomanip> // Used for printing glove data, and converting glove IDs to strings.
#include <memory> // Used for smart pointers.
#include <sstream>
#include <functional>
#include <fstream>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "ClientPlatformSpecific.hpp"
#include "ManusSDK.h"

#include "UDPSender.h"

/// @brief Constant expression: number of hands supported by demo.
constexpr unsigned int NUMBER_OF_HANDS_SUPPORTED = 2;

/// @brief  Constant expression used to define the time between two possible haptics commands sent 
constexpr unsigned long long int MINIMUM_MILLISECONDS_BETWEEN_HAPTICS_COMMANDS = 20;

/// @brief  Constant expression used to define the time between two updates of the temporary skeleton count printing 
constexpr unsigned long long int MILLISECONDS_BETWEEN_TEMPORARY_SKELETONS_UPDATE = 1000;

/// @brief The current state of the client.
enum class ClientState : int
{
	ClientState_PickingConnectionType = 0,
	ClientState_LookingForHosts,
	ClientState_NoHostsFound,
	ClientState_PickingHost,
	ClientState_ConnectingToCore,
	ClientState_DisplayingData,
	ClientState_Disconnected,

	ClientState_MAX_CLIENT_STATE_SIZE
};

/// @brief Values that can be returned by this application.
enum class ClientReturnCode : int
{
	ClientReturnCode_Success = 0,
	ClientReturnCode_FailedPlatformSpecificInitialization,
	ClientReturnCode_FailedToResizeWindow,
	ClientReturnCode_FailedToInitialize,
	ClientReturnCode_FailedToFindHosts,
	ClientReturnCode_FailedToConnect,
	ClientReturnCode_UnrecognizedStateEncountered,
	ClientReturnCode_FailedToShutDownSDK,
	ClientReturnCode_FailedPlatformSpecificShutdown,
	ClientReturnCode_FailedToRestart,
	ClientReturnCode_FailedWrongTimeToGetData,

	ClientReturnCode_MAX_CLIENT_RETURN_CODE_SIZE
};

/// @brief Haptic settings for a single glove.
///
/// This is used to store if a specific motor on the glove should be enabled or not.
struct ClientHapticSettings
{
	bool shouldHapticFinger[NUM_FINGERS_ON_HAND];
};

/// @brief Used to store the information about the final animated skeletons.
class ClientSkeleton
{
public:
	SkeletonInfo info;
	std::vector<SkeletonNode> nodes;
};

/// @brief Used to store all the final animated skeletons received from Core.
class ClientSkeletonCollection
{
public:
	std::vector<ClientSkeleton> skeletons;
};

/// @brief Used to store the information about the skeleton data coming from the estimation system in Core.
class ClientRawSkeleton
{
public:
	RawSkeletonInfo info;
	std::vector<SkeletonNode> nodes;
};

/// @brief Used to store all the skeleton data coming from the estimation system in Core.
class ClientRawSkeletonCollection
{
public:
	std::vector<ClientRawSkeleton> skeletons;
};

/// @brief Used to store all the tracker data coming from Core.
class TrackerDataCollection
{
public:
	std::vector<TrackerData> trackerData;
};

/// @brief Used to store the information about the final animated skeletons.

class ClientGestures
{
public:
	GestureProbabilities info;
	std::vector<GestureProbability> probabilities;
};

class SDKClient : public SDKClientPlatformSpecific
{
private:
	std::ofstream csvFile; // File stream for CSV
	
	bool m_LastToggleState = false;
	std::string m_CurrentIP = "127.0.0.1";

public:
	SDKClient();
	~SDKClient();

	ClientReturnCode Initialize();
	ClientReturnCode Run();
	ClientReturnCode ShutDown();

	//Callbacks
	static void OnConnectedCallback(const ManusHost* const p_Host);
	static void OnDisconnectedCallback(const ManusHost* const p_Host);
	static void OnSkeletonStreamCallback(const SkeletonStreamInfo* const p_Skeleton);
	static void OnLandscapeCallback(const Landscape* const p_Landscape);
	static void OnSystemCallback(const SystemMessage* const p_SystemMessage);
	static void OnErgonomicsCallback(const ErgonomicsStream* const p_Ergo);
	static void OnRawSkeletonStreamCallback(const SkeletonStreamInfo* const p_RawSkeletonStreamInfo);
	static void OnTrackerStreamCallback(const TrackerStreamInfo* const p_TrackerStreamInfo);
	static void OnGestureStreamCallback(const GestureStreamInfo* const p_GestureStream);
	void OpenCSVFile();
	void WriteToCSV(const ErgonomicsData& handData);



	float RoundFloatValue(float p_Value, int p_NumDecimalsToKeep);
	void AdvanceConsolePosition(short int p_Y);

protected:
	virtual ClientReturnCode InitializeSDK();
	virtual ClientReturnCode RestartSDK();
	virtual ClientReturnCode RegisterAllCallbacks();

	virtual ClientReturnCode PickingConnectionType();
	virtual ClientReturnCode LookingForHosts();
	virtual ClientReturnCode NoHostsFound();
	virtual ClientReturnCode PickingHost();
	virtual ClientReturnCode ConnectingToCore();
	
	virtual ClientReturnCode UpdateBeforeDisplayingData();

	virtual ClientReturnCode DisplayingData();
	virtual ClientReturnCode DisplayingDataGlove();
	virtual ClientReturnCode DisplayingDataSkeleton();
	virtual ClientReturnCode DisplayingDataTracker();
	virtual ClientReturnCode DisplayingDataTemporarySkeleton();
	virtual ClientReturnCode DisplayingLandscapeTimeData();
	virtual ClientReturnCode DisplayingDataGestures();

	virtual ClientReturnCode DisconnectedFromCore();
	virtual ClientReturnCode ReconnectingToCore(int32_t p_ReconnectionTime = 0, int32_t p_ReconnectionAttempts = 0);

	void PrintHandErgoData(ErgonomicsData& p_ErgoData, bool p_Left);
	void PrintErgonomicsData();
	void PrintDongleData();
	void PrintSystemMessage();
	void PrintSkeletonData();
	void PrintRawSkeletonData();


	void PrintTrackerData();
	void PrintTrackerDataGlobal();
	void PrintTrackerDataPerUser();

	void PrintLandscapeTimeData();
	
	void PrintGestureData();

	void PrintSkeletonInfo();
	void PrintTemporarySkeletonInfo();
	void GetTemporarySkeletonIfModified();

	void HandleHapticCommands();

	void HandleSkeletonCommands();
	void HandleSkeletonHapticCommands();
	void HandleTemporarySkeletonCommands();
	void HandleTrackerCommands();
	void HandleGesturesCommands();

	static NodeSetup CreateNodeSetup(uint32_t p_Id, uint32_t p_ParentId, float p_PosX, float p_PosY, float p_PosZ, std::string p_Name);
	bool SetupHandNodes(uint32_t p_SklIndex);
	bool SetupHandChains(uint32_t p_SklIndex);

	void LoadTestSkeleton();
	void UnloadTestSkeleton();

	void AllocateChains();

	void BuildTemporarySkeleton();
	void ClearTemporarySkeleton();
	void ClearAllTemporarySkeletons();
	void SaveTemporarySkeletonToFile();
	void GetTemporarySkeletonFromFile();

	void TestTimestamp();

	void RemoveIndexFromTemporarySkeletonList(uint32_t p_Idx);
	static ManusVec3 CreateManusVec3(float p_X, float p_Y, float p_Z);

protected:
	static SDKClient* s_Instance;

	bool m_RequestedExit = false;

	//Console
	uint32_t m_ConsoleClearTickCount = 0;
	const short int m_ConsoleWidth = 220;
	const short int m_ConsoleHeight = 44;
	const short int m_ConsoleScrollback = 500;
	int m_ConsoleCurrentOffset = 0;

	SessionType m_ClientType = SessionType::SessionType_CoreSDK;

	ClientState m_State = ClientState::ClientState_PickingConnectionType;
	ClientState m_PreviousState = m_State;

	std::function<ClientReturnCode()> m_CurrentInteraction = nullptr;

	//Networking
	bool m_ShouldConnectLocally = true;
	bool m_ShouldConnectGRPC = true;

	uint32_t m_HostToConnectTo = 0;
	uint32_t m_NumberOfHostsFound = 0;
	uint32_t m_SecondsToFindHosts = 2;

	int32_t m_SecondsToAttemptReconnecting = 60;
	int32_t m_MaxReconnectionAttempts = 10;
	uint32_t m_SleepBetweenReconnectingAttemptsInMs = 100;

	std::unique_ptr<ManusHost[]> m_AvailableHosts = nullptr;
	std::unique_ptr<ManusHost> m_Host;

	//Data
	uint32_t m_SessionId = 0;

	bool m_RumblingWrist[NUMBER_OF_HANDS_SUPPORTED] = { false, false };

	std::vector<uint32_t> m_LoadedSkeletons;
	std::vector<uint32_t> m_TemporarySkeletons;

	std::mutex m_SkeletonMutex;
	std::mutex m_RawSkeletonMutex;

	ClientSkeletonCollection* m_NextSkeleton = nullptr;
	ClientSkeletonCollection* m_Skeleton = nullptr;

	ClientRawSkeletonCollection* m_NextRawSkeleton = nullptr;
	ClientRawSkeletonCollection* m_RawSkeleton = nullptr;

	std::mutex m_TrackerMutex;

	TrackerDataCollection* m_NextTrackerData = nullptr;
	TrackerDataCollection* m_TrackerData = nullptr;

	std::chrono::time_point<std::chrono::high_resolution_clock> m_TimeSinceLastDisconnect;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTemporarySkeletonUpdate = std::chrono::high_resolution_clock::now();

	std::mutex m_SystemMessageMutex;
	std::string m_SystemMessage = "";
	SystemMessageType m_SystemMessageCode = SystemMessageType::SystemMessageType_Unknown;
	uint32_t m_ModifiedSkeletonIndex = UINT_MAX;

	ManusTimestampInfo m_ErgoTimestampInfo;
	ErgonomicsData m_LeftGloveErgoData;
	ErgonomicsData m_RightGloveErgoData;

	ChainType m_ChainType = ChainType::ChainType_Invalid;

	bool m_TrackerTest = false;
	bool m_TrackerDataDisplayPerUser = false;
	float m_TrackerOffset = 0.0f;

	std::mutex m_LandscapeMutex;
	Landscape* m_NewLandscape = nullptr;
	Landscape* m_Landscape = nullptr;
	std::vector<GestureLandscapeData> m_NewGestureLandscapeData;
	std::vector<GestureLandscapeData> m_GestureLandscapeData;

	uint32_t m_FirstLeftGloveID = 0;
	uint32_t m_FirstRightGloveID = 0;

	std::mutex m_GestureMutex;
	ClientGestures* m_NewFirstLeftGloveGestures = nullptr;
	ClientGestures* m_FirstLeftGloveGestures = nullptr;

	ClientGestures* m_NewFirstRightGloveGestures = nullptr;
	ClientGestures* m_FirstRightGloveGestures = nullptr;

	bool m_ShowLeftGestures = true;
};

// Close the Doxygen group.
/** @} */
#endif
