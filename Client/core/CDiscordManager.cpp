/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CDiscordRichPresence.cpp
 *  PURPOSE:     Discord rich presence implementation
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "discord_rpc.h"
#include "CDiscordManager.h"

#define DEFAULT_APP_ID               ("1242520441684361439")
#define DEFAULT_APP_ASSET            ("logo")
#define DEFAULT_APP_ASSET_TEXT       ("BY OGG SHOP")
#define DEFAULT_APP_ASSET_SMALL      ("")
#define DEFAULT_APP_ASSET_SMALL_TEXT ("")

CDiscordManager::CDiscordManager() : m_uiDiscordAppStart(0), m_uiDiscordAppEnd(0)
{
    SetDefaultData();

    m_strDiscordAppState.clear();
    m_strDiscordCustomResourceName.clear();
    m_bConnected = false;
}

CDiscordManager::~CDiscordManager()
{
    if (m_bDiscordRPCEnabled)
        ShutdownDiscord();
}

void CDiscordManager::InitializeDiscord()
{
    std::lock_guard<std::mutex> lock(m_threadSafetyMutex);
    DiscordEventHandlers        handlers;
    memset(&handlers, 0, sizeof(handlers));

    handlers.ready = HandleDiscordReady;
    handlers.errored = HandleDiscordError;
    handlers.disconnected = HandleDiscordDisconnected;

    Discord_Initialize((m_strDiscordAppCurrentId.empty()) ? DEFAULT_APP_ID : m_strDiscordAppCurrentId.c_str(), &handlers, 1, nullptr);

    m_bConnected = true;
}

void CDiscordManager::ShutdownDiscord()
{
    Discord_ClearPresence();
    Discord_Shutdown();

    m_bConnected = false;
}

void CDiscordManager::RestartDiscord()
{
    ShutdownDiscord();
    InitializeDiscord();
}

void CDiscordManager::SetDefaultData()
{
    m_strDiscordAppId = DEFAULT_APP_ID;
    m_strDiscordAppAsset = DEFAULT_APP_ASSET;
    m_strDiscordAppAssetText = DEFAULT_APP_ASSET_TEXT;

    m_strDiscordAppAssetSmall = DEFAULT_APP_ASSET_SMALL;
    m_strDiscordAppAssetSmallText = DEFAULT_APP_ASSET_SMALL_TEXT;

    m_strDiscordAppCurrentId = DEFAULT_APP_ID;
    m_strDiscordCustomResourceName.clear();
    m_strDiscordAppCustomDetails.clear();
    m_strDiscordAppCustomState.clear();

    m_aButtons = {};
    m_bUpdateRichPresence = true;

    m_uiDiscordAppStart = 0;
    m_uiDiscordAppEnd = 0;

    m_iPartySize = 0;
    m_iPartyMax = 0;

    m_iPlayersCount = 0;
    m_iMaxPlayers = 0;
}

void CDiscordManager::UpdatePresence()
{
    if (!m_bDiscordRPCEnabled)
        return;

    // run callbacks
    Discord_RunCallbacks();

    if (!m_bConnected)
        return;

    if (!m_bUpdateRichPresence)
        return;

    std::lock_guard<std::mutex> lock(m_threadSafetyMutex);
    DiscordRichPresence         discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    discordPresence.largeImageKey = m_strDiscordAppAsset.c_str();
    discordPresence.largeImageText = m_strDiscordAppAssetText.c_str();
    discordPresence.smallImageKey = m_strDiscordAppAssetSmall.c_str();
    discordPresence.smallImageText = m_strDiscordAppAssetSmallText.c_str();

    discordPresence.state = (!m_strDiscordAppCustomState.empty() || true) ? m_strDiscordAppCustomState.c_str() : m_strDiscordAppState.c_str();

    discordPresence.details = (!m_strDiscordAppCustomDetails.empty() || true) ? m_strDiscordAppCustomDetails.c_str() : m_strDiscordAppDetails.c_str();
    discordPresence.startTimestamp = m_uiDiscordAppStart;
    discordPresence.endTimestamp = m_uiDiscordAppEnd;
    discordPresence.instance = 0;

    DiscordButton buttons[2]{0};
    if (m_aButtons)
    {
        buttons[0].label = std::get<0>(*m_aButtons).first.c_str();
        buttons[0].url = std::get<0>(*m_aButtons).second.c_str();
        buttons[1].label = std::get<1>(*m_aButtons).first.c_str();
        buttons[1].url = std::get<1>(*m_aButtons).second.c_str();

        discordPresence.buttons = buttons;
    }

    discordPresence.partySize = (true) ? m_iPlayersCount : m_iPartySize;
    discordPresence.partyMax = (true) ? m_iMaxPlayers : m_iPartyMax;
    Discord_UpdatePresence(&discordPresence);
    m_bUpdateRichPresence = false;
}

void CDiscordManager::SetPresenceStartTimestamp(const unsigned long ulStart)
{
    m_uiDiscordAppStart = ulStart;
    m_bUpdateRichPresence = true;
}

void CDiscordManager::SetPresenceEndTimestamp(const unsigned long ulEnd)
{
    m_uiDiscordAppEnd = ulEnd;
    m_bUpdateRichPresence = true;
}

void CDiscordManager::SetAssetLargeData(const char* szAsset, const char* szAssetText)
{
    SetAsset(szAsset, szAssetText, true);
}

void CDiscordManager::SetAssetSmallData(const char* szAsset, const char* szAssetText)
{
    SetAsset(szAsset, szAssetText, false);
}

void CDiscordManager::SetAsset(const char* szAsset, const char* szAssetText, bool isLarge)
{
    if (isLarge)
    {
        m_strDiscordAppAsset = (std::strlen(szAsset) > 0 && szAsset && *szAsset) ? szAsset : DEFAULT_APP_ASSET;
        m_strDiscordAppAssetText = (std::strlen(szAssetText) > 0 && szAssetText && *szAssetText) ? szAssetText : DEFAULT_APP_ASSET_TEXT;
    }
    else
    {
        m_strDiscordAppAssetSmall = (std::strlen(szAsset) > 0 && szAsset && *szAsset) ? szAsset : DEFAULT_APP_ASSET_SMALL;
        m_strDiscordAppAssetSmallText = (std::strlen(szAssetText) > 0 && szAssetText && *szAssetText) ? szAssetText : DEFAULT_APP_ASSET_SMALL_TEXT;
    }
    m_bUpdateRichPresence = true;
}

bool CDiscordManager::SetPresenceState(const char* szState, bool bCustom)
{
    if (bCustom)
        m_strDiscordAppCustomState = szState;
    else
        m_strDiscordAppState = szState;

    m_bUpdateRichPresence = true;
    return true;
}

bool CDiscordManager::SetPresenceButtons(unsigned short int iIndex, const char* szName, const char* szUrl)
{
    // Should it always return true?
    if (iIndex <= 2)
    {
        std::decay_t<decltype(*m_aButtons)> buttons;
        if (m_aButtons)
            buttons = *m_aButtons;

        if (iIndex == 1)
            std::get<0>(buttons) = {szName, szUrl};
        else if (iIndex == 2)
            std::get<1>(buttons) = {szName, szUrl};

        m_aButtons = buttons;
        m_bUpdateRichPresence = true;

        return true;
    }

    return false;
}

bool CDiscordManager::SetPresenceDetails(const char* szDetails, bool bCustom)
{
    if (bCustom)
        m_strDiscordAppCustomDetails = szDetails;
    else
        m_strDiscordAppDetails = szDetails;

    m_bUpdateRichPresence = true;
    return true;
}

bool CDiscordManager::ResetDiscordData()
{
    SetDefaultData();

    if (m_bDiscordRPCEnabled)
    {
        RestartDiscord();
        m_bUpdateRichPresence = true;
    }
    return true;
}

bool CDiscordManager::SetApplicationID(const char* szResourceName, const char* szAppID)
{
    m_strDiscordAppCurrentId = (szAppID && *szAppID) ? szAppID : DEFAULT_APP_ID;

    if (m_bDiscordRPCEnabled)
    {
        RestartDiscord();
        m_bUpdateRichPresence = true;
    }

    if (*szResourceName)
        m_strDiscordCustomResourceName = szResourceName;

    return true;
}

bool CDiscordManager::SetDiscordRPCEnabled(bool bEnabled)
{
    m_bDiscordRPCEnabled = bEnabled;

    if (!bEnabled)
    {
        ShutdownDiscord();
        return true;
    }

    InitializeDiscord();
    m_bUpdateRichPresence = true;
    return true;
}

void CDiscordManager::SetPresencePartySize(int iSize, int iMax, bool bCustom)
{
    if (bCustom)
    {
        m_iPartySize = iSize;
        m_iPartyMax = iMax;
    }
    else
    {
        m_iPlayersCount = iSize;
        m_iMaxPlayers = iMax;
    }
}

void CDiscordManager::SetDiscordUserID(const std::string& strUserID)
{
    m_strDiscordUserID = strUserID;
}

std::string CDiscordManager::GetDiscordUserID() const
{
    return m_strDiscordUserID;
};

#ifdef DISCORD_DISABLE_IO_THREAD
void CDiscordManager::UpdatePresenceConnection()
{
    Discord_UpdateConnection();
}
#endif

void CDiscordManager::HandleDiscordReady(const DiscordUser* pDiscordUser)
{
    if (const auto discord = g_pCore->GetDiscord(); discord && discord->IsDiscordRPCEnabled())
    {
        discord->SetDiscordClientConnected(true);
        discord->SetDiscordUserID(pDiscordUser->userId);
    }
}

void CDiscordManager::HandleDiscordDisconnected(int iErrorCode, const char* szMessage)
{
    WriteDebugEvent(SString(("[DISCORD] Disconnected %s (error #%d)"), szMessage, iErrorCode));

    if (const auto discord = g_pCore->GetDiscord(); discord)
    {
        discord->SetDiscordUserID((""));
        discord->SetDiscordClientConnected(false);
    }
}

void CDiscordManager::HandleDiscordError(int iErrorCode, const char* szMessage)
{
    WriteDebugEvent(SString(("[DISCORD] Error: %s (error #%d)"), szMessage, iErrorCode));
}
