///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include "gscommon.h"
#include "gscore.h"
#include "gsavailable.h"
#include "webservices/authservice.h"
#include "sc/sc.h"

#include <float.h>

#ifdef UNDER_CE
void RetailOutputA(CHAR* tszErr, ...);
#define printf RetailOutputA
#elif defined(_NITRO)
#include "nitro/screen.h"
#define printf  Printf
#define vprintf VPrintf
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This sample simulates a stats implementation for a 1v1 game.
//
// For simplicity, all 2 players are run from one machine.
// 2 people use the auth service functions to login and get their certificates
// The SDK is initialized and the game creates a session using one of the
// player's certificate
//
//

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Player account information.  (see GP documentation for account creation.)
#define SCTEST_LOGIN_PARTNERCODE 0 // gamespy account space
#define SCTEST_LOGIN_NAMESPACE   1 // shared namespace with unique nicknames

#define SCTEST_REMOTE_PARTNERCODE 1
#define SCTEST_REMOTE_NAMESPACEID 0

// Hardcoded information for four sample players
#define SCTEST_NICK_1   _T("sctest01")
#define SCTEST_NICK_2   _T("sctest02")
#define SCTEST_EMAIL    _T("sctest@gamespy.com")
#define SCTEST_PASSWORD _T("gspy")

#define SCTEST_TOKEN_1                                                                                                 \
    _T("GMTy13lsJmiY7L19ojyN3XTM08ll0C4EWWijwmJyq3ttiZmoDUQJ0OSnar9nQCu5MpOGvi4Z0EcC2uNaS4yKrUA+h+")                   \
    _T("tTDDoJHF7ZjoWKOTj00yNOEdzWyG08cKdVQwFRkF+h8oG/Jd+Ik3sWviXq/+5bhZQ7iXxTbbDwNL6Lagp/pLZ9czLnYPhY7VEcoQlx9oO")
#define SCTEST_CHALLENGE_1 _T("LH8c.DLe")
#define SCTEST_TOKEN_2                                                                                                 \
    _T("GMTSc9Hl0VvG1FjqkCLkanABNB/")                                                                                  \
    _T("u9jhNrQnXng6cQva0JtsJgEkwt6znjLDABAm3MmyEHDEcj6yZWwZmvPl1R8uEcJ0RW1OplewifmRhfU3dglMq3OP1VOgX6+")              \
    _T("lG0SoPwp9BLmka8O4JGiSTSq8jGuDyHnCBLVozMMpeYPJ3wGAuxrx2OlWdUe3PTNhFK/LeNBl")
#define SCTEST_CHALLENGE_2 _T("@WWm5DWI")

#define SCTEST_LOGIN_PROFILE    0
#define SCTEST_LOGIN_UNIQUE     1
#define SCTEST_LOGIN_REMOTEAUTH 2

#define SCTEST_GAMETYPE_CTF        2
#define SCTEST_GAMETYPE_DEATHMATCH 3
#define SCTEST_TEAMRANK_HEROES     26
#define SCTEST_TEAMRANK_VILLAINS   35

#define SCTEST_KEY_TEAM_TYPE_HEROES   0
#define SCTEST_KEY_TEAM_TYPE_VILLAINS 1

#define SCTEST_KEY_TEAM_ARENAID_HEROES   7066
#define SCTEST_KEY_TEAM_ARENAID_VILLAINS 7067
#define SCTEST_KEY_TEAM_NAME_HEROES      _T("Heroes")
#define SCTEST_KEY_TEAM_NAME_VILLAINS    _T("Villains")

// StatsAdmin generated header file
// NOTE: not all keys are used
#define SCTEST_KEY_HEADER_VERSION 20001

#define FIRAXIS_FFA_PLACE        1  // [TYPE: byte]
#define FIRAXIS_GAMETYPE_1V1     2  // [TYPE: byte]
#define FIRAXIS_GAMETYPE_2V2     3  // [TYPE: byte]
#define FIRAXIS_GAMETYPE_FFA     4  // [TYPE: byte]
#define SCTEST_KEY_CUSTOM_MAP    5  // [TYPE: byte]
#define SCTEST_KEY_GAMETYPE      6  // [TYPE: byte]
#define SCTEST_KEY_GAMEVER       7  // [TYPE: string]
#define SCTEST_KEY_HOSTNAME      8  // [TYPE: string]
#define SCTEST_KEY_MAPNAME       9  // [TYPE: string]
#define SCTEST_KEY_PLAYER_DEATHS 10 // [TYPE: short]
#define SCTEST_KEY_PLAYER_FRAGS  11 // [TYPE: int]
#define SCTEST_KEY_PLAYER_NICK   12 // [TYPE: string]
#define SCTEST_KEY_PLAYER_SCORE  13 // [TYPE: int]
#define SCTEST_KEY_PLAYER_SHOTS  14 // [TYPE: int]
#define SCTEST_KEY_PLAYER_TEAM   15 // [TYPE: int]
#define SCTEST_KEY_PLAYER_TIME   16 // [TYPE: float]
#define SCTEST_KEY_ROUND_TIME    17 // [TYPE: int] [DESC: The amount of time spent in one session]
#define SCTEST_KEY_TEAM_ID       18 // [TYPE: int]
#define SCTEST_KEY_TEAM_NAME     19 // [TYPE: string]
#define SCTEST_KEY_TEAM_RANK     20 // [TYPE: byte]
#define SCTEST_KEY_WINNINGTEAM   21 // [TYPE: int] [DESC: the team that won the round]
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Some application settings, #defined for clarity
#define SCTEST_GAMENAME     _T("gmtest")
#define SCTEST_GAME_ID      0 // assigned by GameSpy
#define SCTEST_GAME_VERSION _T("v1.0")
#define SLEEP_MS            100

#define TIMEOUT_MS 0 // use default

#define INVALID_PTR ((void*)(uintptr_t)0xdeadc0de)

#define SCTEST_NUM_TEAMS   2
#define SCTEST_NUM_PLAYERS 2

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Report keys, generated by the admin webpage at http://net.gamespy.com/
/*
//    - server keys
#define SC_KEY_HOSTNAME     0
#define SC_KEY_MAPNAME      1
//    - player keys (one value for each player)
#define SC_KEY_FRAGS        2
#define SC_KEY_DEATHS       3
#define SC_KEY_SHOTS        4
#define SC_KEY_SCORE        5
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This represents the player data structure in your game.
typedef struct SamplePlayerData
{
    // "Normal" game data
    gsi_u32 mProfileId;
    GSLoginCertificate mCertificate;
    GSLoginPrivateData mPrivateData;
    SCPeerCipher mPeerSendCipher[SCTEST_NUM_PLAYERS]; // for fast encryption
    SCPeerCipher mPeerRecvCipher[SCTEST_NUM_PLAYERS]; // for fast decryption

    // Stats interface
    SCInterfacePtr mStatsInterface;

    // Stats Report
    SCReportPtr mReport;
    // Stats related data
    gsi_u8 mConnectionId[SC_CONNECTION_GUID_SIZE];
    gsi_u8 mStatsAuthdata[16];
    gsi_i32 mFrags;
    gsi_i32 mScore;
    gsi_i16 mDeaths;
    gsi_i16 mShots;
    gsi_u32 mTeam;

    // Obfuscated versions
    SCHiddenData mHiddenFrags;
    SCHiddenData mHiddenDeaths;
    SCHiddenData mHiddenShots;
    SCHiddenData mHiddenScore;
} SamplePlayerData;
static SamplePlayerData gPlayerData[SCTEST_NUM_PLAYERS];

// This represents the server (session) data in your game.
typedef struct SampleServerData
{
    // "Normal" server data
    gsi_u32 mNumPlayers;
    SCHiddenData mObfuscationSeed; // Starting value for SCHiddenData

    // Reported session data (You will likely report this to qr2 as well)
    const gsi_char* mHostName;
    const gsi_char* mMapName;
    const gsi_char* mVersion;
    int mWinningTeam;
    int mGameType;
    gsi_i8 mCustomMap;
    float mRoundTime;

    // A simple way to block the sample's progress
    gsi_u32 mWaitCount;

} SampleServerData;
static SampleServerData gServerData;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Text equivalent of SCResult values, for debugging
char* SCResultStr[SCResultMax + 1] = {"SCResult_NO_ERROR",
                                      "SCResult_NO_AVAILABILITY_CHECK",
                                      "SCResult_INVALID_PARAMETERS",
                                      "SCResult_NOT_INITIALIZED",
                                      "SCResult_CORE_NOT_INITIALIZED",
                                      "SCResult_OUT_OF_MEMORY",
                                      "SCResult_CALLBACK_PENDING",
                                      "SCResult_HTTP_ERROR",
                                      "SCResult_SERVER_ERROR",
                                      "SCResult_RESPONSE_INVALID",
                                      "SCResult_REPORT_INCOMPLETE",
                                      "SCResult_REPORT_INVALID",
                                      "SCResult_SUBMISSION_FAILED",
                                      (char*)INVALID_PTR};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static gsi_bool isBackendAvailable()
{
    // Do Availability Check - Make sure backend is available
    /////////////////////////////////////////////////////////
    GSIACResult aResult = GSIACWaiting;
    GSIStartAvailableCheck(SCTEST_GAMENAME);
    while (aResult == GSIACWaiting) {
        aResult = GSIAvailableCheckThink();
        msleep(5);
    }

    if (aResult == GSIACUnavailable) {
        printf("Online Services for sctest are no longer available\r\n");
        return gsi_false;
    }

    if (aResult == GSIACTemporarilyUnavailable) {
        printf("Online Services for sctest are temporarily down for maintenance\r\n");
        return gsi_false;
    }

    return gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void myLoginCallback(GHTTPResult httpResult, WSLoginResponse* theResponse, void* theUserData)
{
    if (httpResult != GHTTPSuccess) {
        gsDebugFormat(GSIDebugCat_App,
                      GSIDebugType_Misc,
                      GSIDebugLevel_HotError,
                      "Failed on player login, HTTP error: %d\r\n",
                      httpResult);
        getc(stdin);
        exit(0);
    } else if (theResponse->mLoginResult != WSLogin_Success) {

        gsDebugFormat(GSIDebugCat_App,
                      GSIDebugType_Misc,
                      GSIDebugLevel_HotError,
                      "Failed on player login, Login result: %d\r\n",
                      theResponse->mLoginResult);
        getc(stdin);
        exit(0);
    } else {
        SamplePlayerData* newPlayer = NULL;
        int playerIndex = (int)(uintptr_t)theUserData;
        char playerNick[WS_LOGIN_NICK_LEN];

        newPlayer = &gPlayerData[playerIndex];

        // copy certificate and private key
        newPlayer->mProfileId = theResponse->mCertificate.mProfileId;
        memcpy(&newPlayer->mCertificate, &theResponse->mCertificate, sizeof(GSLoginCertificate));
        memcpy(&newPlayer->mPrivateData, &theResponse->mPrivateData, sizeof(GSLoginPrivateData));

#ifdef GSI_UNICODE
        UCS2ToAsciiString(theResponse->mCertificate.mUniqueNick, playerNick);
        printf("Player '%s' logged in.\r\n", playerNick);
#else
        printf("Player '%s' logged in.\r\n", theResponse->mCertificate.mUniqueNick);
        GSI_UNUSED(playerNick);
#endif
    }
    gServerData.mWaitCount--;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*static void myPs3LoginCallback(GHTTPResult httpResult, WSLoginPs3CertResponse * theResponse, void * theUserData)
{
	if (httpResult != GHTTPSuccess)
	{
		gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_HotError, 
			"Failed on player login, HTTP error: %d\r\n", httpResult);
		getc(stdin);
		exit(0);
	}
	else if (theResponse->mLoginResult != WSLogin_Success)
	{
		
		gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_HotError, 
			"Failed on player login, Login result: %d\r\n", theResponse->mLoginResult);
		getc(stdin);
		exit(0);
	}
	else
	{
		SamplePlayerData * newPlayer = NULL;
		int playerIndex = (int)theUserData;
		char playerNick[WS_LOGIN_NICK_LEN];

		newPlayer = &gPlayerData[playerIndex];
	}
	gServerData.mWaitCount--;
}*/

static void myPlayerLogin(gsi_u8 logintype, const gsi_char* nick, const gsi_char* password, int localPlayerNumber)
{
    gsi_u32 result;

    if (logintype == SCTEST_LOGIN_PROFILE) {
        if (0
            != wsLoginProfile(SCTEST_LOGIN_PARTNERCODE,
                              SCTEST_LOGIN_NAMESPACE,
                              nick,
                              SCTEST_EMAIL,
                              password,
                              _T(""),
                              myLoginCallback,
                              (void*)(uintptr_t)localPlayerNumber)) {
            gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_HotError, "Failed on wsLoginProfile\r\n");
            getc(stdin);
            exit(0);
        }
    } else if (logintype == SCTEST_LOGIN_UNIQUE) {
        result = wsLoginUnique(SCTEST_LOGIN_PARTNERCODE,
                               SCTEST_LOGIN_NAMESPACE,
                               nick,
                               password,
                               _T(""),
                               myLoginCallback,
                               (void*)(uintptr_t)localPlayerNumber);
        if (result != WSLogin_Success) {
            gsDebugFormat(GSIDebugCat_App,
                          GSIDebugType_Misc,
                          GSIDebugLevel_HotError,
                          "Failed on wsLoginUnique. Result: %d\r\n",
                          result);
            getc(stdin);
            exit(0);
        }
    } else if (logintype == SCTEST_LOGIN_REMOTEAUTH) {
        result = wsLoginRemoteAuth(SCTEST_REMOTE_PARTNERCODE,
                                   SCTEST_REMOTE_NAMESPACEID,
                                   nick,
                                   password,
                                   myLoginCallback,
                                   (void*)(uintptr_t)localPlayerNumber);
        if (result != WSLogin_Success) {
            gsDebugFormat(GSIDebugCat_App,
                          GSIDebugType_Misc,
                          GSIDebugLevel_HotError,
                          "Failed on wsLoginUnique. Result: %d\r\n",
                          result);
            getc(stdin);
            exit(0);
        }
    }

    /*result = wsLoginPs3Cert(0, 19, 28, 
		"IQAAAAAAAPAwAACkAAgAFJM+TMa5EdVMnN4uVUeVHCBFZWMTAAEABAAAAAEABwAIAAABDNXa36MABwAIAAABDNXtLXAAAgAIMkQh9jjX3f0ABAAgZ3NpLXNuYWRlcgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAEdXMAAQAEAARhMAAAAAgAGFNDRUktWFgtWFRDTTAwMDAzLTAwAAAAAAABAARqAAAAAAAAAAAAAAAwAgBEAAgABFoOxO8ACAA4MDUCGQDoMjq/8ZeDFo0Bdo7FPBmAFoWLEzJHbRgCGGX88UQim5OJqDzp7N048ZBmjZcS7xP0dQA=", 
		strlen("IQAAAAAAAPAwAACkAAgAFJM+TMa5EdVMnN4uVUeVHCBFZWMTAAEABAAAAAEABwAIAAABDNXa36MABwAIAAABDNXtLXAAAgAIMkQh9jjX3f0ABAAgZ3NpLXNuYWRlcgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAEdXMAAQAEAARhMAAAAAgAGFNDRUktWFgtWFRDTTAwMDAzLTAwAAAAAAABAARqAAAAAAAAAAAAAAAwAgBEAAgABFoOxO8ACAA4MDUCGQDoMjq/8ZeDFo0Bdo7FPBmAFoWLEzJHbRgCGGX88UQim5OJqDzp7N048ZBmjZcS7xP0dQA="), 
		myPs3LoginCallback, (void*)localPlayerNumber);

	if (result != WSLogin_Success)
	{
		gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_HotError, 
			"Failed on wsLoginPs3Cert. Result: %d\r\n", result);
		getc(stdin);
		exit(0);
	}*/

    // wait for it to complete
    gServerData.mWaitCount++;
    while (gServerData.mWaitCount > 0) {
        msleep(SLEEP_MS);
        gsCoreThink(0);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Update obfuscated verification data
static void myUpdateHiddenData(SCHiddenData theDataField, gsi_u32 theAmount)
{
    // TODO: Choice of algorithm for obfuscation
    double data;

    memcpy(&data, theDataField, sizeof(double));
    data += (double)theAmount;
    memcpy(theDataField, &data, sizeof(double));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Setup player structure, initial obfuscated verification data
static void myRecordPlayerJoined(gsi_u32 theProfileId, gsi_u32 theTeamId)
{
    SamplePlayerData* newPlayer = &gPlayerData[gServerData.mNumPlayers];
    newPlayer->mProfileId = theProfileId;
    newPlayer->mTeam = theTeamId;

    newPlayer->mFrags = 0;
    newPlayer->mDeaths = 0;
    newPlayer->mScore = 0;
    newPlayer->mShots = 0;

    // Initialize obfuscation data
    //   TODO: dervice obfuscation seed in a way that prevents copying between variables
    memcpy(newPlayer->mHiddenFrags, gServerData.mObfuscationSeed, sizeof(newPlayer->mHiddenDeaths));
    memcpy(newPlayer->mHiddenDeaths, gServerData.mObfuscationSeed, sizeof(newPlayer->mHiddenDeaths));
    memcpy(newPlayer->mHiddenScore, gServerData.mObfuscationSeed, sizeof(newPlayer->mHiddenDeaths));
    memcpy(newPlayer->mHiddenShots, gServerData.mObfuscationSeed, sizeof(newPlayer->mHiddenDeaths));

    gServerData.mNumPlayers++;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Increment player kills/deaths and transform obfuscated verification data
//    For obfuscation purposes, it's best if this happens as part of
//    a larger function which updates game state.
static void myRecordPlayerFrag(gsi_u32 theFraggerId, gsi_u32 theFraggedId)
{
    unsigned int i = 0;
    for (i = 0; i < gServerData.mNumPlayers; i++) {
        // Score a kill for the fragger
        if (gPlayerData[i].mProfileId == theFraggerId) {
            gPlayerData[i].mFrags++;
            myUpdateHiddenData(gPlayerData[i].mHiddenFrags, 1);
        }
        // Scor a death for the fragged
        if (gPlayerData[i].mProfileId == theFraggedId) {
            gPlayerData[i].mDeaths++;
            myUpdateHiddenData(gPlayerData[i].mHiddenFrags, 1);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Increment player score and transform obfuscated verification data
//    For obfuscation purposes, it's best if this happens as part of
//    a larger function which updates game state.
static void myRecordPlayerScored(gsi_u32 thePid, gsi_u32 theScore)
{
    unsigned int i = 0;
    for (i = 0; i < gServerData.mNumPlayers; i++) {
        // Add to player's score
        if (gPlayerData[i].mProfileId == thePid) {
            gPlayerData[i].mScore += theScore;
            myUpdateHiddenData(gPlayerData[i].mHiddenScore, theScore);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Increment player shots and transform obfuscated verification data
//    For obfuscation purposes, it's best if this happens as part of
//    a larger function which updates game state.
static void myRecordPlayerFiredGun(gsi_u32 thePid)
{
    unsigned int i = 0;
    for (i = 0; i < gServerData.mNumPlayers; i++) {
        // Add to player's shots
        if (gPlayerData[i].mProfileId == thePid) {
            gPlayerData[i].mShots++;
            myUpdateHiddenData(gPlayerData[i].mHiddenShots, 1);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// A utility to get a random integer
static int myGetRandomInt(int theMax)
{
    return (int)((float)rand() / RAND_MAX * theMax);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// A utility to get a random player
//   Used by the sample to introduce some variation in results
static gsi_u32 myGetRandomPlayerId(gsi_u32 excludedId)
{
    int aPlayerIndex = 0;

    do {
        float randomNum = (float)rand() / RAND_MAX;
        aPlayerIndex = (int)(randomNum * SCTEST_NUM_PLAYERS);
    } while (gPlayerData[aPlayerIndex].mProfileId == excludedId);

    return gPlayerData[aPlayerIndex].mProfileId;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// A utility to block until a request completes
//     Used to simplify the logic flow of the sample
static void myWaitForCallbacks(SCInterfacePtr theInterface, int howMany)
{
    // wait for the request to complete
    gServerData.mWaitCount = (gsi_u32)howMany;
    while (gServerData.mWaitCount > 0) {
        msleep(SLEEP_MS);
        scThink(theInterface);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This will be triggered when scCreateSession() has completed.
//    Expected to occur once per reported game session.
void createSessionCallback(SCInterfacePtr theInterface,
                           GHTTPResult theHttpResult,
                           SCResult theResult,
                           void* theUserData)
{
    gsDebugFormat(GSIDebugCat_App,
                  GSIDebugType_Misc,
                  GSIDebugLevel_Notice,
                  "CreateSessionCallback: theHttpResult = %d, theResult = %s\r\n",
                  theHttpResult,
                  SCResultStr[theResult]);

    if (theHttpResult == GHTTPSuccess && theResult == SCResult_NO_ERROR) {
        const char* sessionId = scGetSessionId(theInterface);
        const char* connectionId = scGetConnectionId(theInterface);

        gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_Debug, "Session ID: %s\r\n", sessionId);
        gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_Debug, "Connection ID: %s\r\n", connectionId);
        GSI_UNUSED(sessionId);
        GSI_UNUSED(connectionId);
    }

    gServerData.mWaitCount--; // one less request to wait for

    GSI_UNUSED(theInterface);
    GSI_UNUSED(theUserData);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This will be triggered when scSetReportIntention() has completed.
//    Expected to occur once per reported game session.
void setReportIntentionCallback(SCInterfacePtr theInterface,
                                GHTTPResult theHttpResult,
                                SCResult theResult,
                                void* theUserData)
{
    SamplePlayerData* thePlayer = (SamplePlayerData*)theUserData;

    gsDebugFormat(GSIDebugCat_App,
                  GSIDebugType_Misc,
                  GSIDebugLevel_Notice,
                  "SetReportIntentionCallback: theHttpResult = %d, theResult = %s\r\n",
                  theHttpResult,
                  SCResultStr[theResult]);

    if (theHttpResult == GHTTPSuccess && theResult == SCResult_NO_ERROR) {
        const char* connectionId = scGetConnectionId(theInterface);
        memcpy(thePlayer->mConnectionId, connectionId, SC_CONNECTION_GUID_SIZE);
        gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_Debug, "Connection ID: %s\r\n", connectionId);
    }

    gServerData.mWaitCount--; // one less request to wait for

    GSI_UNUSED(theInterface);
    GSI_UNUSED(theUserData);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This will be triggered when scSubmitReport() has completed
//    Expected to occur once per call to scSetReportIntention
void submitReportCallback(SCInterfacePtr theInterface, GHTTPResult theHttpResult, SCResult theResult, void* theUserData)
{
    gsDebugFormat(GSIDebugCat_App,
                  GSIDebugType_Misc,
                  GSIDebugLevel_Notice,
                  "SubmitReportCallback: theHttpResult = %d, theResult = %s\r\n",
                  theHttpResult,
                  SCResultStr[theResult]);

    gServerData.mWaitCount--; // one less request to wait for

    GSI_UNUSED(theHttpResult);
    GSI_UNUSED(theResult);
    GSI_UNUSED(theInterface);
    GSI_UNUSED(theUserData);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// CreateReportAndSubmit

static gsi_bool CreateReportAndSubmit()
{
    int i;
    SCResult aResult;
    ////////////////////////////////////////
    ////////////////////////////////////////
    // Submit report
    //    This is a little hard to demonstrate since we are simulating four players.
    //    In a live application you would need to determine which players would
    //    submit data and when, depending on game specifics.

    //    It's not as complicated as it may sound.
    //    - For late entry games, this will likely occur after each map rotation
    //    - For coordinated entry, this will likely occur once for each player eliminated
    //
    // Build the report
    // Note that the order for global, player, and team data matters
    for (i = 0; i < SCTEST_NUM_PLAYERS; i++) {
        SCGameResult playersGameResult;
        // Player reports results only for itself.
        aResult =
            scCreateReport(gPlayerData[i].mStatsInterface, SCTEST_KEY_HEADER_VERSION, 1, 1, &gPlayerData[i].mReport);
        if (aResult != SCResult_NO_ERROR) {
            //need to cleanup the SDK after failures
            return gsi_false;
        }

        // REQUIRED FOR MATCHLESS GAMES
        // player joined a matchless game.
        scReportSetAsMatchless(gPlayerData[i].mReport);

        // Non-player data
        scReportBeginGlobalData(gPlayerData[i].mReport);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_MAPNAME, gServerData.mMapName);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_GAMETYPE, gServerData.mGameType);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_GAMEVER, gServerData.mVersion);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_HOSTNAME, gServerData.mHostName);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_WINNINGTEAM, SCTEST_KEY_TEAM_ARENAID_HEROES);

        // test float and byte values
        scReportAddFloatValue(gPlayerData[i].mReport, SCTEST_KEY_ROUND_TIME, gServerData.mRoundTime);
        scReportAddByteValue(gPlayerData[i].mReport, SCTEST_KEY_CUSTOM_MAP, gServerData.mCustomMap);

        // Player data
        scReportBeginPlayerData(gPlayerData[i].mReport);

        if (gPlayerData[i].mTeam == SCTEST_KEY_TEAM_ARENAID_HEROES)
            playersGameResult = SCGameResult_WIN; // team 0 always wins
        else
            playersGameResult = SCGameResult_LOSS; // team 1 loses (so sorry!)
        scReportBeginNewPlayer(gPlayerData[i].mReport);

        // Remember to report the player's data in the 0th position
        // otherwise the Competition System will treat the report as if there
        // were no players in the report.
        scReportSetPlayerData(gPlayerData[i].mReport,
                              (gsi_u32)0,
                              gPlayerData[i].mConnectionId,
                              gPlayerData[i].mTeam,
                              playersGameResult,
                              gPlayerData[i].mProfileId,
                              &gPlayerData[i].mCertificate,
                              gPlayerData[i].mStatsAuthdata);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_NICK, gPlayerData[i].mCertificate.mUniqueNick);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_FRAGS, gPlayerData[i].mFrags);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_SCORE, gPlayerData[i].mScore);
        scReportAddShortValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_SHOTS, gPlayerData[i].mShots);
        scReportAddShortValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_DEATHS, gPlayerData[i].mDeaths);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_TEAM, (gsi_i32)gPlayerData[i].mTeam);

        // Team data
        scReportBeginTeamData(gPlayerData[i].mReport);

        // If player is on the winners team, report the winner team
        // or report loosing team.
        if ((i & 1) == 0) {
            scReportBeginNewTeam(gPlayerData[i].mReport);
            scReportSetTeamData(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ARENAID_HEROES, SCGameResult_WIN);
            scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ID, SCTEST_KEY_TEAM_ARENAID_HEROES);
            scReportAddByteValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_RANK, SCTEST_TEAMRANK_HEROES);
            scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_NAME, SCTEST_KEY_TEAM_NAME_HEROES);
        } else {
            scReportBeginNewTeam(gPlayerData[i].mReport);
            scReportSetTeamData(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ARENAID_VILLAINS, SCGameResult_LOSS);
            scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ID, SCTEST_KEY_TEAM_ARENAID_VILLAINS);
            scReportAddByteValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_RANK, SCTEST_TEAMRANK_VILLAINS);
            scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_NAME, SCTEST_KEY_TEAM_NAME_VILLAINS);
        }

        // The setting of an authoritative report in matchless sessions should
        // be ignored.  It can be set to either gsi_true or gsi_false
        scReportEnd(gPlayerData[i].mReport, gsi_false, SCGameStatus_COMPLETE);

        // Submit the report
        // The setting of an authoritative report in matchless sessions should
        // be ignored.  It can be set to either gsi_true or gsi_false
        aResult = scSubmitReport(gPlayerData[i].mStatsInterface,
                                 gPlayerData[i].mReport,
                                 gsi_false,
                                 &gPlayerData[i].mCertificate,
                                 &gPlayerData[i].mPrivateData,
                                 submitReportCallback,
                                 TIMEOUT_MS,
                                 NULL);
        if (SCResult_NO_ERROR != aResult) {
            // error, failed to submit report
            return gsi_false;
        }
        myWaitForCallbacks(gPlayerData[i].mStatsInterface, 1);
        // End of Submitting the report
    }

    // The host would then send all other player's the Session ID generated when the session was created
    // so that they can set their intentions, and retrieve the connection ID for report submissions
    //scSetSessionId(gPlayerData[1].mStatsInterface, (const unsigned char *)scGetSessionId(gPlayerData[0].mStatsInterface));

    scSetReportIntention(gPlayerData[1].mStatsInterface,
                         (const gsi_u8*)scGetConnectionId(gPlayerData[1].mStatsInterface),
                         gsi_false,
                         &gPlayerData[1].mCertificate,
                         &gPlayerData[1].mPrivateData,
                         setReportIntentionCallback,
                         TIMEOUT_MS,
                         &gPlayerData[1]);

    // store each connection id for the other players
    //strcpy((char *)gPlayerData[j].mConnectionId, scGetConnectionId(gPlayerData[j].mStatsInterface));

    // *NOTE* this is simply meant to show HOW this process works, in truth the connection id
    // here is overwritten because it's using the same interface so it will report 4 of the same
    // player information. We need to upgrade the sample to communicate with other instances
    // in order to report individual player data

    myWaitForCallbacks(gPlayerData[1].mStatsInterface, 1);

    for (i = 1; i < SCTEST_NUM_PLAYERS; i++) {
        SCGameResult playersGameResult;
        // Player reports results only for itself.
        aResult =
            scCreateReport(gPlayerData[i].mStatsInterface, SCTEST_KEY_HEADER_VERSION, 1, 1, &gPlayerData[i].mReport);
        if (aResult != SCResult_NO_ERROR) {
            //need to cleanup the SDK after failures
            return gsi_false;
        }

        // REQUIRED FOR MATCHLESS GAMES
        // player joined a matchless game.
        scReportSetAsMatchless(gPlayerData[i].mReport);

        // Non-player data
        scReportBeginGlobalData(gPlayerData[i].mReport);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_MAPNAME, gServerData.mMapName);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_GAMETYPE, gServerData.mGameType);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_GAMEVER, gServerData.mVersion);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_HOSTNAME, gServerData.mHostName);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_WINNINGTEAM, SCTEST_KEY_TEAM_ARENAID_HEROES);

        // test float and byte values
        scReportAddFloatValue(gPlayerData[i].mReport, SCTEST_KEY_ROUND_TIME, gServerData.mRoundTime);
        scReportAddByteValue(gPlayerData[i].mReport, SCTEST_KEY_CUSTOM_MAP, gServerData.mCustomMap);

        // Player data
        scReportBeginPlayerData(gPlayerData[i].mReport);

        if (gPlayerData[i].mTeam == SCTEST_KEY_TEAM_ARENAID_HEROES)
            playersGameResult = SCGameResult_WIN; // team 0 always wins
        else
            playersGameResult = SCGameResult_LOSS; // team 1 loses (so sorry!)
        scReportBeginNewPlayer(gPlayerData[i].mReport);

        // Remember to report the player's data in the 0th position
        // otherwise the Competition System will treat the report as if there
        // were no players in the report.
        scReportSetPlayerData(gPlayerData[i].mReport,
                              (gsi_u32)0,
                              gPlayerData[i].mConnectionId,
                              gPlayerData[i].mTeam,
                              playersGameResult,
                              gPlayerData[i].mProfileId,
                              &gPlayerData[i].mCertificate,
                              gPlayerData[i].mStatsAuthdata);
        scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_NICK, gPlayerData[i].mCertificate.mUniqueNick);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_FRAGS, gPlayerData[i].mFrags);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_SCORE, gPlayerData[i].mScore);
        scReportAddShortValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_SHOTS, gPlayerData[i].mShots);
        scReportAddShortValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_DEATHS, gPlayerData[i].mDeaths);
        scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_PLAYER_TEAM, (gsi_i32)gPlayerData[i].mTeam);

        // Team data
        scReportBeginTeamData(gPlayerData[i].mReport);

        // If player is on the winners team, report the winner team
        // or report loosing team.
        if ((i & 1) == 0) {
            scReportBeginNewTeam(gPlayerData[i].mReport);
            scReportSetTeamData(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ARENAID_HEROES, SCGameResult_WIN);
            scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ID, SCTEST_KEY_TEAM_ARENAID_HEROES);
            scReportAddByteValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_RANK, SCTEST_TEAMRANK_HEROES);
            scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_NAME, SCTEST_KEY_TEAM_NAME_HEROES);
        } else {
            scReportBeginNewTeam(gPlayerData[i].mReport);
            scReportSetTeamData(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ARENAID_VILLAINS, SCGameResult_LOSS);
            scReportAddIntValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_ID, SCTEST_KEY_TEAM_ARENAID_VILLAINS);
            scReportAddByteValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_RANK, SCTEST_TEAMRANK_VILLAINS);
            scReportAddStringValue(gPlayerData[i].mReport, SCTEST_KEY_TEAM_NAME, SCTEST_KEY_TEAM_NAME_VILLAINS);
        }

        // The setting of an authoritative report in matchless sessions should
        // be ignored.  It can be set to either gsi_true or gsi_false
        scReportEnd(gPlayerData[i].mReport, gsi_false, SCGameStatus_COMPLETE);

        // Submit the report
        // The setting of an authoritative report in matchless sessions should
        // be ignored.  It can be set to either gsi_true or gsi_false
        aResult = scSubmitReport(gPlayerData[i].mStatsInterface,
                                 gPlayerData[i].mReport,
                                 gsi_false,
                                 &gPlayerData[i].mCertificate,
                                 &gPlayerData[i].mPrivateData,
                                 submitReportCallback,
                                 TIMEOUT_MS,
                                 NULL);
        if (SCResult_NO_ERROR != aResult) {
            // error, failed to submit report
            return gsi_false;
        }
        myWaitForCallbacks(gPlayerData[i].mStatsInterface, 1);
        // End of Submitting the report
    }
    return gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// This runs all the tests
static gsi_bool RunTests(gsi_u8 loginMethod, gsi_bool isLoginOnly)
{
    //SCReportPtr     aReport = NULL;
    SCResult aResult = SCResult_NO_ERROR;
    int i = 0, k = 0, j = 0;

    // Clear sample data
    memset(&gServerData, 0, sizeof(gServerData));
    memset(gPlayerData, 0, sizeof(gPlayerData));

    // Initialize SDK core/common objects for both the auth service and
    // the Competition SDK
    gsCoreInitialize();

    // Override service URLs for debugging?
    //strcpy(wsAuthServiceURL, "http://mwstage.gamespy.com/AuthService/AuthService.asmx");
    //strcpy(scServiceURL, "http://mwstage.gamespy.com/CompetitionService/CompetitionService.asmx");

    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    // Obtain a certificate for each local player.
    //    - For the sample, all player's are local.

    // Switch up login modes as necessary for testing the authservice
    if (loginMethod == SCTEST_LOGIN_REMOTEAUTH) {
        myPlayerLogin(SCTEST_LOGIN_REMOTEAUTH, SCTEST_TOKEN_1, SCTEST_CHALLENGE_1, 0);
        myPlayerLogin(SCTEST_LOGIN_REMOTEAUTH, SCTEST_TOKEN_2, SCTEST_CHALLENGE_2, 1);
    } else if (loginMethod == SCTEST_LOGIN_UNIQUE) {
        myPlayerLogin(SCTEST_LOGIN_UNIQUE, SCTEST_NICK_1, SCTEST_PASSWORD, 0);
        myPlayerLogin(SCTEST_LOGIN_UNIQUE, SCTEST_NICK_2, SCTEST_PASSWORD, 1);
        //myPlayerLogin(SCTEST_LOGIN_UNIQUE, SCTEST_NICK_3, SCTEST_PASSWORD, 2);
        //myPlayerLogin(SCTEST_LOGIN_UNIQUE, SCTEST_NICK_4, SCTEST_PASSWORD, 3);
        //myPlayerLogin(SCTEST_LOGIN_UNIQUE, SCTEST_NICK_5, SCTEST_PASSWORD, 4);
        //myPlayerLogin(SCTEST_LOGIN_UNIQUE, SCTEST_NICK_6, SCTEST_PASSWORD, 5);
    } else if (loginMethod == SCTEST_LOGIN_PROFILE) {
        myPlayerLogin(SCTEST_LOGIN_PROFILE, SCTEST_NICK_1, SCTEST_PASSWORD, 0);
        myPlayerLogin(SCTEST_LOGIN_PROFILE, SCTEST_NICK_2, SCTEST_PASSWORD, 1);
    }

    if (!isLoginOnly) {
        // Initialize the SDK
        for (i = 0; i < SCTEST_NUM_PLAYERS; i++) {
            aResult = scInitialize(SCTEST_GAME_ID, &gPlayerData[i].mStatsInterface);
            if (aResult != SCResult_NO_ERROR) {
                gsDebugFormat(GSIDebugCat_App,
                              GSIDebugType_Misc,
                              GSIDebugLevel_HotError,
                              "scInitialize returned %s\r\n",
                              SCResultStr[aResult]);

                return gsi_false;
            }
        }

        ////////////////////////////////////////
        ////////////////////////////////////////
        // Simulate peer-to-peer authentication
        //    You can have the server validate each client.
        //    You may have each client validate every other.
        //    Certificates may also be used to exchange keys for encrypting game traffic.
        //
        //    Step1 - Trade certificates using unsecured game socket. (The sample doesn't do this)
        //    Step2 - Verify certificate is authentic using wsLoginCertIsValid
        //    Step3 - Verify that the guy who gave you the certificate actually owns it

        // Step1 - skipped.
        //       - The usual scenario is to have the host verify the client certificate when the player joins.

        // Step2 - validate certificates
        for (i = 0; i < SCTEST_NUM_PLAYERS; i++) {
            if (gsi_is_false(wsLoginCertIsValid(&gPlayerData[i].mCertificate))) {
                gsDebugFormat(GSIDebugCat_App,
                              GSIDebugType_Misc,
                              GSIDebugLevel_HotError,
                              "Error validating certificate for player %d!\r\n",
                              i);
                return gsi_false;
            }
        }

        // Step3 - exchange keys
        gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_Debug, "Generating encryption keys\r\n");
        for (i = 0; i < SCTEST_NUM_PLAYERS; i++) {
            for (k = i + 1; k < SCTEST_NUM_PLAYERS; k++) {
                SCPeerKeyExchangeMsg exchangeMsg1;
                SCPeerKeyExchangeMsg exchangeMsg2;

                const char* plainTextMsg = "Hello Secure!";
                char cipherMsg[32] = {'\0'};
                int msgLen = (int)strlen(plainTextMsg);

                gsDebugFormat(GSIDebugCat_App,
                              GSIDebugType_Misc,
                              GSIDebugLevel_Notice,
                              "Creating session keys for player pair %d <-> %d\r\n",
                              i,
                              k);

                // Each player should create a key for receiving data from the remote player
                //     For extra security, we use a different encryption key for each channel
                scPeerCipherInit(&gPlayerData[i].mCertificate,
                                 &gPlayerData[i].mPeerRecvCipher[k]); // 'i' to receive data from 'k'
                scPeerCipherInit(&gPlayerData[k].mCertificate,
                                 &gPlayerData[k].mPeerRecvCipher[i]); // 'k' to receive data from 'i'

                // Create a key exchange message for transmitting the key to the other player
                scPeerCipherCreateKeyExchangeMsg(&gPlayerData[k].mCertificate,
                                                 &gPlayerData[i].mPeerRecvCipher[k],
                                                 exchangeMsg1); // from 'i', encrypted with 'k' public key
                scPeerCipherCreateKeyExchangeMsg(&gPlayerData[i].mCertificate,
                                                 &gPlayerData[k].mPeerRecvCipher[i],
                                                 exchangeMsg2); // from 'k', encrypted with 'i' public key

                // Send it (using game network layer)
                //   - sample doesn't need to send b/c all player's are local

                // Receiving player should parse the key out of it.
                //   - decrypting the msg requires the local player's private data
                scPeerCipherParseKeyExchangeMsg(&gPlayerData[k].mCertificate,
                                                &gPlayerData[k].mPrivateData,
                                                exchangeMsg1,
                                                &gPlayerData[k].mPeerSendCipher[i]); // 'k' to send data to 'i'
                scPeerCipherParseKeyExchangeMsg(&gPlayerData[i].mCertificate,
                                                &gPlayerData[i].mPrivateData,
                                                exchangeMsg2,
                                                &gPlayerData[i].mPeerSendCipher[k]); // 'i' to send data to 'k'

                // Now we can send secure data by using the (fast) encryption and decryption functions
                //   - Encrypts and Decrypts in place
                strcpy(cipherMsg, plainTextMsg);
                scPeerCipherEncryptBufferIV(
                    &gPlayerData[i].mPeerSendCipher[k], 1, (gsi_u8*)cipherMsg, (gsi_u32)msgLen); // 'i' sending to 'k'
                scPeerCipherDecryptBufferIV(&gPlayerData[k].mPeerRecvCipher[i],
                                            1,
                                            (gsi_u8*)cipherMsg,
                                            (gsi_u32)msgLen); // 'k' receiving from 'i'
            }
        }

        ////////////////////////////////////////
        ////////////////////////////////////////
        // Host creates session
        // Remember that everyone is on the same machine, so the
        // first player's login certificate and private data are
        // used to create a session

        // REQUIRED FOR MATCHLESS GAMES
        // For matchless games this function needs to be called so that the Competition
        // System is aware of the type of reports coming in.
        scCreateMatchlessSession(gPlayerData[0].mStatsInterface,
                                 &gPlayerData[0].mCertificate,
                                 &gPlayerData[0].mPrivateData,
                                 createSessionCallback,
                                 TIMEOUT_MS,
                                 NULL);

        myWaitForCallbacks(gPlayerData[0].mStatsInterface, 1);
        // End creating the session

        // The host can now set his connection ID
        strcpy((char*)gPlayerData[0].mConnectionId, scGetConnectionId(gPlayerData[0].mStatsInterface));

        // In most cases the host also has the "authoritative" game view
        // For this game
        // Remember that in matchless sessions, the setting for authoritative should
        // not matter as each player's stats get updated soon after they leave.  The
        // system will not need to wait for all players to submit before processing stats.
        scSetReportIntention(gPlayerData[0].mStatsInterface,
                             NULL,
                             gsi_false,
                             &gPlayerData[0].mCertificate,
                             &gPlayerData[0].mPrivateData,
                             setReportIntentionCallback,
                             TIMEOUT_MS,
                             &gPlayerData[0]);

        myWaitForCallbacks(gPlayerData[0].mStatsInterface, 1);

        for (j = 1; j < SCTEST_NUM_PLAYERS; j++) {
            // The host would then send all other player's the Session ID generated when the session was created
            // so that they can set their intentions, and retrieve the connection ID for report submissions
            scSetSessionId(gPlayerData[j].mStatsInterface,
                           (const unsigned char*)scGetSessionId(gPlayerData[0].mStatsInterface));

            scSetReportIntention(gPlayerData[j].mStatsInterface,
                                 NULL,
                                 gsi_false,
                                 &gPlayerData[j].mCertificate,
                                 &gPlayerData[j].mPrivateData,
                                 setReportIntentionCallback,
                                 TIMEOUT_MS,
                                 &gPlayerData[j]);

            // store each connection id for the other players
            //strcpy((char *)gPlayerData[j].mConnectionId, scGetConnectionId(gPlayerData[j].mStatsInterface));

            // *NOTE* this is simply meant to show HOW this process works, in truth the connection id
            // here is overwritten because it's using the same interface so it will report 4 of the same
            // player information. We need to upgrade the sample to communicate with other instances
            // in order to report individual player data

            myWaitForCallbacks(gPlayerData[j].mStatsInterface, 1);
        }

        ////////////////////////////////////////
        ////////////////////////////////////////
        // Game start
        //    - Record some fake stats
        //
        srand(current_time());

        // Server settings
        gServerData.mHostName = _T("scTest");
        gServerData.mMapName = _T("scBigMap v2");
        gServerData.mGameType = SCTEST_GAMETYPE_CTF;
        gServerData.mCustomMap = 1;
        gServerData.mRoundTime = 81.257f;
        gServerData.mVersion = _T("1.0en");
        memset(gServerData.mObfuscationSeed, 0, sizeof(gServerData.mObfuscationSeed));

        // Pretend everyone is joining now
        for (i = 0; i < SCTEST_NUM_PLAYERS; i++) {
            if ((i & 1) == 0) // even players on hero team, odd players on villans
            {
                myRecordPlayerJoined(
                    gPlayerData[i].mProfileId,
                    (gsi_u32)((i & 1) == 0 ? SCTEST_KEY_TEAM_ARENAID_HEROES : SCTEST_KEY_TEAM_ARENAID_VILLAINS));
            } else {
                myRecordPlayerJoined(
                    gPlayerData[i].mProfileId,
                    (gsi_u32)((i & 1) == 0 ? SCTEST_KEY_TEAM_ARENAID_HEROES : SCTEST_KEY_TEAM_ARENAID_VILLAINS));
            }
        }

        // Record some events
        for (i = 0; i < 100; i++) {
            // Who did the shooting and who got shot?
            gsi_u32 aShooterId = myGetRandomPlayerId(0);
            gsi_u32 aShooteeId = myGetRandomPlayerId(aShooterId); // prevent shooter from suiciding

            // How many bullets were fired?
            int aRandomNumberOfShots = (int)((float)rand() / RAND_MAX) * 20;
            for (k = 0; k < aRandomNumberOfShots; k++)
                myRecordPlayerFiredGun(aShooterId); // (could do a bulk add also)

            // 10 shots for the kill!
            if (aRandomNumberOfShots >= 10)
                myRecordPlayerFrag(aShooterId, aShooteeId);
        }

        // some random point values
        for (i = 0; i < SCTEST_NUM_PLAYERS; i++)
            myRecordPlayerScored(gPlayerData[i].mProfileId, (gsi_u32)myGetRandomInt(100));

        if (!CreateReportAndSubmit()) {
            gsDebugFormat(GSIDebugCat_App,
                          GSIDebugType_Misc,
                          GSIDebugLevel_Debug,
                          "Failed to Create or Submit one of the Reports\r\n");
        }
        ////////////////////////////////////////
        ////////////////////////////////////////
        // Game Over
        //   exit
        //     - or -
        //   clear user data
        //   goto start (for new map, round etc)

        // Cleanup
        //scDestroyReport(aReport);
        //aReport = NULL;

        for (i = 0; i < SCTEST_NUM_PLAYERS; i++) {
            if (gPlayerData[i].mReport) {
                scDestroyReport(gPlayerData[i].mReport);
                gPlayerData[i].mReport = NULL;
            }
            if (gPlayerData[i].mStatsInterface) {
                scShutdown(gPlayerData[i].mStatsInterface);
                gPlayerData[i].mStatsInterface = NULL;
            }
        }
    }

    gsCoreShutdown();

    // Wait for core shutdown
    //   (should be instantaneous unless you have multiple cores)
    while (gsCoreIsShutdown() == GSCore_SHUTDOWN_PENDING) {
        gsCoreThink(0);
        msleep(5);
    }

    GSI_UNUSED(SCResultStr);
    return gsi_true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG)
#include <crtdbg.h>
#endif

int test_main(int argc, char* argv[])
{
    gsi_bool result;

    // Set debug output options
    //gsSetDebugFile(stdout);
    gsSetDebugLevel(GSIDebugCat_All, GSIDebugType_All, GSIDebugLevel_Debug);

// enable Win32 C Runtime debugging
#if defined(_WIN32) && !defined(_XBOX) && defined(_DEBUG)
    {
        int tempFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        _CrtSetDbgFlag(tempFlag | _CRTDBG_LEAK_CHECK_DF);
    }
#endif

    if (!isBackendAvailable())
        return -1;

    result = RunTests(SCTEST_LOGIN_UNIQUE, gsi_false);

    // For Testing AuthService logins w/o submitting snapshots
    //////////////////////////////////////////////////////////
    /*gsSetDebugLevel(GSIDebugCat_All, GSIDebugType_All, GSIDebugLevel_None);
	printf("=== Testing wsLoginProfile === \n");
	result = RunTests(SCTEST_LOGIN_PROFILE, gsi_true);
	printf("=== Testing wsLoginRemoteAuth === \n");
	result = RunTests(SCTEST_LOGIN_REMOTEAUTH, gsi_true);*/

#if defined(_WIN32) && !defined(_XBOX) && defined(_UNIX)
    fflush(stderr);
    printf("Done - Press Enter\r\n");
    fflush(stdout);
    getc(stdin);
#else
    printf("Done.\r\n");
#endif

    GSI_UNUSED(argc);
    GSI_UNUSED(argv);

    return gsi_is_true(result) ? 0 : -1;
}
