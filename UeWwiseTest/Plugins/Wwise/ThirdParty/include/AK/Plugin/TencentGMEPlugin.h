/** Copyright (c) 2018-2028 Tencent Technology All Right Reserved.
* @file       TencentGMEPlugin.h
* @brief      Game Multimedia Engine(GME) Plugin
* @version    2019-01-07 gaopenggao create this file
*******************************************************************************/

// TencentGMEPlugin.h

#ifndef _TENCENTGMEPLUGIN_API_H_
#define _TENCENTGMEPLUGIN_API_H_

#if defined (__APPLE__) || defined(ANDROID) || defined(__linux__) || defined(AK_PS4) || defined(AK_XBOXONE) || defined(AK_NX)
#define GMEPlugin_API_CALL
#elif defined(_WIN32)
#define GMEPlugin_API_CALL __cdecl
#endif

enum GMEWWisePlugin_TeamMode
{
	TEAMMODE_GLOBAL = 0,
	TEAMMODE_TEAM = 1
};

enum GMEWWisePlugin_LogLevel
{
	GMESDK_LOGLEVEL_NONE = 0,
	GMESDK_LOGLEVEL_ERROR = 1,
	GMESDK_LOGLEVEL_INFO = 2,
	GMESDK_LOGLEVEL_DEBUG = 3,
	GMESDK_LOGLEVEL_VERBOSE = 4
};

/**
* @brief Set the local user ID. Each GME user must have a unique identifier.
*	This function must be called before posting events sending or receiving to GME servers.
* @param[in] userID The identifier of the local GME user.
*	The value is a 64-bit integer data and should be greater than 10000. It needs to be
*	converted into character type.
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SetUserID(const char* userID);

/**
* @brief Set the GME chatting room ID.
*	This function must be called before posting events sending or receiving to GME servers.
*	Setting a new roomID will not affect already playing voices.
* @param[in] roomID Alphanumeric character string of up to 127 characters identifying a 
*	GME chatting room.
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SetRoomID(const char* roomID);

/**
* @brief For receiving the voice chat, set the mapping between game object ID and GME
*	voice ID. The individual voice or the mixed voice can be received according to if 
*	the mapping relationship is set.
* @param[in] gameObjectID The gameObjectID allocated in the game.
* @param[in] userID The voice ID of the GME user.
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_ReceivePlugin_SetReceiveOpenIDWithGameObjectID(AkUInt64 gameObjectID, const char* userID);

/**
* @brief For receiving the voice chat, get the mapping between game object ID and GME
*	voice ID. 
* @param[in] gameObjectID The gameObjectID allocated in the game.
* @param[out] userID The voice ID of the GME user.
*/
extern "C" 
void GMEPlugin_API_CALL GMEWWisePlugin_ReceivePlugin_GetReceiveOpenIDWithGameObjectID(AkUInt64 gameObjectID, char* userID, int maxlen = 128);

/**
* @brief Enable or disable audio loopback. Loopback controls if sound related to the
*	specified game object is routed back to Wwise to be played on the local device.
*	The audio is always sent to the server, but will only be played locally if loopback
*	is enabled.
* @param[in] gameObjectID The gameObjectID allocated in the game.
* @param[in] enableLoopback Loopback flag value.
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SendPlugin_EnableLoopbackWithGameObjectID(AkUInt64 gameObjectID, bool enableLoopback);

/**
* @brief Get the loopback status. Retrieve whether loopback is enabled or disabled for
*	a given game object. Loopback controls if sound posted on the specified game
*	object is routed back to Wwise to be played on the local device.
*	The audio is always sent to the server, but will only be played locally if loopback
*	is enabled.
* @param[in] gameObjectID The gameObjectID allocated in the game.
* @return The loopback status.
*/
extern "C"
bool GMEPlugin_API_CALL GMEWWisePlugin_SendPlugin_GetEnableLoopbackWithGameObjectID(AkUInt64 gameObjectID);

/**
* @brief Set the level of logging for the GME SDK library.
*	For log files, default values are GMESDK_LOGLEVEL_INFO in debug builds and GMESDK_LOGLEVEL_ERROR in release builds.
*	For console print, default values are GMESDK_LOGLEVEL_INFO in debug builds and GMESDK_LOGLEVEL_NONE in release builds.
* @param[in] levelWrite Logging level for saved log file.
* @param[in] levelPrint Logging level for console printing.
* @sa GMEWWisePlugin_LogLevel
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SetLogLevel(GMEWWisePlugin_LogLevel levelWrite, GMEWWisePlugin_LogLevel levelPrint);

/*
	The following APIs are only for the proximity voice. For normal voice chatting use cases,
	these APIs don't need to be called.
*/

/**
* @brief Set the team ID for the current player.
*	Players with the same team ID can always talk to each other,
*	regardless of the distance between them in any team mode.
*	A team is a group of players sharing the same team ID within the same room.
* @param[in] teamID Integer value identifying a team ID
*	A team ID of 0 indicates "No team", and will deactivate any filtering done on the
*	when routing voices to this user.
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SetRangeAudioTeamID(int teamID);

/**
* @brief Set the team mode to use for the current player.
* @param[in] teamMode The communication mode for the current player
*	* TEAMMODE_GLOBAL: Sound from players of any team is received if their position is within the receiving range.
*		Sound from players in the same team is always received.
*	* TEAMMODE_TEAM: Sound is only received from teammates, i.e., players sharing the same team ID as the current player).
*		Filtering based on the distance range is not applied.
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SetRangeAudioTeamMode(GMEWWisePlugin_TeamMode teamMode);

/**
* @brief Set the hearing distance between players in global team mode.
*	In global team mode, sound from players with a different team ID will be received when their position is within range.
*	The range is the 3D distance between player positions as set by GMEWWisePlugin_SetSelfPosition.
*	The filtering applied by this range is on the server.
* @param[in] range Integer value represents the hearing distance.
*/
extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SetRangeAudioRecvRange(int range);

/**
* @brief Set the position of the player. The position values are used to compute if different
*	players falls in the hearing range.
* @param[in] positionX The X-coordinate of the player
* @param[in] positionY The Y-coordinate of the player
* @param[in] positionZ The Z-coordinate of the player
*/

extern "C"
void GMEPlugin_API_CALL GMEWWisePlugin_SetSelfPosition(int positionX, int positionY, int positionZ);

/**
* @brief Get GME specific messages.
* @param[in] localUTCTime Unix time of the message.
* @param[in] messageType Message type, ref to the following detailed description.
* @param[in] code Message code according to the message type, ref to the following detailed description.
* @param[in] message1 Content of Message1
* @param[in] len1 Length of the message1 array.
* @param[in] message2 Content of Message2
* @param[in] len2 Length of the message2 array.
*/
extern "C"
int GMEPlugin_API_CALL GMEWWisePlugin_GetMessage(int* localUTCTime, int* messageType, int* code, char* message1, int len1, char* message2, int len2);

#endif // _TENCENTGMEPLUGIN_API_H_
