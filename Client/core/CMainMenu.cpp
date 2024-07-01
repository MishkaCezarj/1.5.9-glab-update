/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CMainMenu.cpp
 *  PURPOSE:     2D Main menu graphical user interface
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include <game/CGame.h>
#include "CLanguageSelector.h"

#define IP_TO_AUTOCONNECT   ("localhost")
#define PORT_TO_AUTOCONNECT 22003

#define NATIVE_RES_X 1280.0f
#define NATIVE_RES_Y 1024.0f
#define NATIVE_BG_Y 1080.0f
#define CORE_MTA_BG_MAX_ALPHA 1.00f
#define CORE_MTA_BG_INGAME_ALPHA 0.90f
#define CORE_MTA_FADER           0.05f
#define CORE_MTA_HOVER_SCALE  0.8f
#define CORE_MTA_NORMAL_SCALE 0.8f
#define CORE_MTA_HOVER_ALPHA  1.0f
#define CORE_MTA_NORMAL_ALPHA 0.6f

static int WaitForMenu = 0;

CMainMenu::CMainMenu(CGUI* pManager)
{
    ulPreviousTick = GetTickCount32();
    m_pHoveredItem = NULL;
    m_iMoveStartPos = 0;

    m_pManager = pManager;
    m_bIsVisible = false;
    m_bIsFullyVisible = false;
    m_bIsIngame = true;
    m_bStarted = false;
    m_fFader = 0;
    m_ucFade = FADE_INVISIBLE;
    m_bCursorAlphaReset = false;

    CVector2D ScreenSize = m_pManager->GetResolution();
    m_ScreenSize = ScreenSize;

    int iBackgroundX = 0;
    int iBackgroundY = 0;
    int iBackgroundSizeX = ScreenSize.fX;
    int iBackgroundSizeY;

    if (ScreenSize.fX > ScreenSize.fY)
    {
        float iRatioSizeY = ScreenSize.fY / NATIVE_RES_Y;
        m_iMenuSizeX = NATIVE_RES_X * iRatioSizeY;
        m_iMenuSizeY = ScreenSize.fY;
        m_iXOff = (ScreenSize.fX - m_iMenuSizeX) * 0.5f;
        m_iYOff = 0;

        float iRatioSizeX = ScreenSize.fX / NATIVE_RES_X;
        iBackgroundSizeX = ScreenSize.fX;
        iBackgroundSizeY = NATIVE_BG_Y * iRatioSizeX;
    }
    else
    {
        float iRatioSizeX = ScreenSize.fX / NATIVE_RES_X;
        m_iMenuSizeY = NATIVE_RES_Y * iRatioSizeX;
        m_iMenuSizeX = ScreenSize.fX;
        m_iXOff = 0;
        m_iYOff = (ScreenSize.fY - m_iMenuSizeY) * 0.5f;

        iBackgroundY = m_iYOff;
        iBackgroundSizeX = m_iMenuSizeX;
        iBackgroundSizeY = NATIVE_BG_Y * iRatioSizeX;
    }

    srand(time(0));
    int random = rand();

    m_pCanvas = reinterpret_cast<CGUIStaticImage*>(pManager->CreateStaticImage());
    m_pCanvas->LoadFromFile(SString(("%s\\%d.png"), ("cgui\\images\\main"), random % 6));
    m_pCanvas->SetProperty(("InheritsAlpha"), ("False"));
    m_pCanvas->SetPosition(CVector2D(iBackgroundX, iBackgroundY), false);
    m_pCanvas->SetSize(CVector2D(iBackgroundSizeX, iBackgroundSizeY), false);
    m_pCanvas->SetZOrderingEnabled(false);
    m_pCanvas->SetAlwaysOnTop(false);
    m_pCanvas->MoveToBack();
    // Loading Text
    m_pCanvasLabel = m_pManager->CreateLabel(m_pCanvas, _("Loading game..."));
    m_pCanvasLabel->SetSize(CVector2D(m_ScreenSize.fX / 1.8, 37), false);
    m_pCanvasLabel->SetPosition(CVector2D(40, m_ScreenSize.fY - 73), false);
    m_pCanvasLabel->SetFont(("open-sans"));
    m_pCanvasLabel->SetHorizontalAlign(CGUI_ALIGN_LEFT);
    m_pCanvasLabel->SetVerticalAlign(CGUI_ALIGN_VERTICALCENTER);
    // Background
    m_pBackground = reinterpret_cast<CGUIStaticImage*>(pManager->CreateStaticImage());
    m_pBackground->LoadFromFile(SString(("%s\\%d.png"), ("cgui\\images\\main"), random % 6));
    m_pBackground->SetProperty(("InheritsAlpha"), ("False"));
    m_pBackground->SetPosition(CVector2D(iBackgroundX, iBackgroundY), false);
    m_pBackground->SetSize(CVector2D(iBackgroundSizeX, iBackgroundSizeY), false);
    m_pBackground->SetZOrderingEnabled(false);
    m_pBackground->SetAlwaysOnTop(false);
    m_pBackground->SetVisible(false);

    // Settings
    m_pCustomSettings = reinterpret_cast<CGUIStaticImage*>(pManager->CreateStaticImage(m_pBackground));
    m_pCustomSettings->LoadFromFile(("cgui\\images\\buttons\\btn_settings_i.png"));
    m_pCustomSettings->SetProperty(("InheritsAlpha"), ("False"));
    m_pCustomSettings->SetSize(CVector2D(143, 40), false);
    m_pCustomSettings->SetPosition(CVector2D(40, m_ScreenSize.fY - 125), false);
    m_pCustomSettings->SetZOrderingEnabled(false);
    m_pCustomSettings->SetAlwaysOnTop(true);
    m_pCustomSettings->SetMouseButtonDownHandler(GUI_CALLBACK(&CMainMenu::ClickSettings, this));
    m_pCustomSettings->SetClickHandler(GUI_CALLBACK(&CMainMenu::OnSettingsButtonClick, this));
    m_pCustomSettings->SetMouseEnterHandler(GUI_CALLBACK(&CMainMenu::EnterSettings, this));
    m_pCustomSettings->SetMouseLeaveHandler(GUI_CALLBACK(&CMainMenu::LeaveSettings, this));
    // Quit
    m_pCustomQuit = reinterpret_cast<CGUIStaticImage*>(pManager->CreateStaticImage(m_pBackground));
    m_pCustomQuit->LoadFromFile(("cgui\\images\\buttons\\btn_quit_i.png"));
    m_pCustomQuit->SetProperty(("InheritsAlpha"), ("False"));
    m_pCustomQuit->SetSize(CVector2D(194, 62), false);
    m_pCustomQuit->SetPosition(CVector2D(m_ScreenSize.fX - 235, m_ScreenSize.fY - 92), false);
    m_pCustomQuit->SetZOrderingEnabled(false);
    m_pCustomQuit->SetAlwaysOnTop(true);
    m_pCustomQuit->SetMouseButtonDownHandler(GUI_CALLBACK(&CMainMenu::ClickQuit, this));
    m_pCustomQuit->SetClickHandler(GUI_CALLBACK(&CMainMenu::OnQuitButtonClick, this));
    m_pCustomQuit->SetMouseEnterHandler(GUI_CALLBACK(&CMainMenu::EnterQuit, this));
    m_pCustomQuit->SetMouseLeaveHandler(GUI_CALLBACK(&CMainMenu::LeaveQuit, this));
    // Reconnect
    m_pCustomReconnect = reinterpret_cast<CGUIStaticImage*>(pManager->CreateStaticImage(m_pBackground));
    m_pCustomReconnect->LoadFromFile(("cgui\\images\\buttons\\btn_connect_i.png"));
    m_pCustomReconnect->SetProperty(("InheritsAlpha"), ("False"));
    m_pCustomReconnect->SetSize(CVector2D(244, 62), false);
    m_pCustomReconnect->SetPosition(CVector2D(m_ScreenSize.fX - 495, m_ScreenSize.fY - 92), false);
    m_pCustomReconnect->SetZOrderingEnabled(false);
    m_pCustomReconnect->SetAlwaysOnTop(true);
    m_pCustomReconnect->SetMouseButtonDownHandler(GUI_CALLBACK(&CMainMenu::ClickReconnect, this));
    m_pCustomReconnect->SetClickHandler(GUI_CALLBACK(&CMainMenu::OnQuickConnectButtonClick, this));
    m_pCustomReconnect->SetMouseEnterHandler(GUI_CALLBACK(&CMainMenu::EnterReconnect, this));
    m_pCustomReconnect->SetMouseLeaveHandler(GUI_CALLBACK(&CMainMenu::LeaveReconnect, this));

    // Submenus
    m_ServerBrowser.SetVisible(false);
    m_ServerInfo.Hide();
    m_Settings.SetVisible(false);

    // We're not ingame
    SetIsIngame(false);

    auto discord = g_pCore->GetDiscord();
    if (!discord->IsDiscordRPCEnabled())
    {
        discord->SetDiscordRPCEnabled(true);

        discord->SetPresenceStartTimestamp(0);
    }

    m_pGraphics = g_pGraphics;

    CXMLNode* pConfig = g_pCore->GetConfig();
    m_ServerBrowser.LoadServerList(pConfig->FindSubNode(CONFIG_NODE_SERVER_FAV), CONFIG_FAVOURITE_LIST_TAG, m_ServerBrowser.GetFavouritesList());
    m_ServerBrowser.LoadServerList(pConfig->FindSubNode(CONFIG_NODE_SERVER_REC), CONFIG_RECENT_LIST_TAG, m_ServerBrowser.GetRecentList());
    m_ServerBrowser.LoadServerList(pConfig->FindSubNode(CONFIG_NODE_SERVER_HISTORY), CONFIG_HISTORY_LIST_TAG, m_ServerBrowser.GetHistoryList());

    if (CXMLNode* pOldNode = pConfig->FindSubNode(CONFIG_NODE_SERVER_INT))
        pConfig->DeleteSubNode(pOldNode);
}

void CMainMenu::SetVisibleCustomButtons(bool bVisible)
{
    if (m_pCustomReconnect)
        m_pCustomReconnect->SetVisible(bVisible);

    if (m_pCustomSettings)
        m_pCustomSettings->SetVisible(bVisible);

    if (m_pCustomQuit)
        m_pCustomQuit->SetVisible(bVisible);
}

bool CMainMenu::EnterSettings(CGUIElement* pElement)
{
    m_pCustomSettings->LoadFromFile(("cgui\\images\\buttons\\btn_settings_h.png"));
    return true;
}

bool CMainMenu::LeaveSettings(CGUIElement* pElement)
{
    m_pCustomSettings->LoadFromFile(("cgui\\images\\buttons\\btn_settings_i.png"));
    return true;
}

bool CMainMenu::ClickSettings(CGUIElement* pElement)
{
    m_pCustomSettings->LoadFromFile(("cgui\\images\\buttons\\btn_settings_c.png"));
    return true;
}

bool CMainMenu::EnterQuit(CGUIElement* pElement)
{
    m_pCustomQuit->LoadFromFile(("cgui\\images\\buttons\\btn_quit_h.png"));
    return true;
}

bool CMainMenu::LeaveQuit(CGUIElement* pElement)
{
    m_pCustomQuit->LoadFromFile(("cgui\\images\\buttons\\btn_quit_i.png"));
    return true;
}

bool CMainMenu::ClickQuit(CGUIElement* pElement)
{
    m_pCustomQuit->LoadFromFile(("cgui\\images\\buttons\\btn_quit_c.png"));
    return true;
}

bool CMainMenu::EnterReconnect(CGUIElement* pElement)
{
    m_pCustomReconnect->LoadFromFile(("cgui\\images\\buttons\\btn_connect_h.png"));
    return true;
}

bool CMainMenu::LeaveReconnect(CGUIElement* pElement)
{
    m_pCustomReconnect->LoadFromFile(("cgui\\images\\buttons\\btn_connect_i.png"));
    return true;
}
bool CMainMenu::ClickReconnect(CGUIElement* pElement)
{
    m_pCustomReconnect->LoadFromFile(("cgui\\images\\buttons\\btn_connect_c.png"));
    return true;
}

CMainMenu::~CMainMenu()
{
    delete m_pBackground;
    delete m_pCanvas;
}

void CMainMenu::SetMenuVerticalPosition(int iPosY)
{
    if (m_pHoveredItem)
    {
        m_unhoveredItems.insert(m_pHoveredItem);
        m_pHoveredItem = NULL;
    }
}

void CMainMenu::SetMenuUnhovered()
{
    m_pHoveredItem = NULL;
}

void CMainMenu::Update()
{
    if (GetKeyState(VK_LMENU) & 0x8000 && GetAsyncKeyState(VK_F4) & 0x8000)
    {
        return g_pCore->Quit();
    }

    if (g_pCore->GetDiagnosticDebug() == EDiagnosticDebug::JOYSTICK_0000)
    {
        m_pBackground->SetVisible(false);
        m_bHideGame = false;
    }

    if (g_pCore->m_pMessageBox && !m_bIsIngame && m_pCustomSettings->IsVisible())
    {
        m_pCustomSettings->SetPosition(CVector2D(40, m_ScreenSize.fY - 125), false);
    }
    else
    {
        m_pCustomSettings->SetPosition(CVector2D(40, m_ScreenSize.fY - 73), false);
    }

    if (m_bFrameDelay)
    {
        m_bFrameDelay = false;
        return;
    }

    CGame* pGame = g_pCore->GetGame();
    eSystemState SystemState = pGame->GetSystemState();

    m_Settings.Update();

    unsigned long ulCurrentTick = GetTickCount32();
    unsigned long ulTimePassed = ulCurrentTick - ulPreviousTick;

    if (m_bHideGame)
        m_pGraphics->DrawRectangle(0, 0, m_ScreenSize.fX, m_ScreenSize.fY, 0xFF000000);

    if (m_bIsFullyVisible)
    {
        tagPOINT cursor;
        GetCursorPos(&cursor);

        HWND hookedWindow = g_pCore->GetHookedWindow();

        tagPOINT windowPos = { 0 };
        ClientToScreen(hookedWindow, &windowPos);

        CVector2D vecResolution = g_pCore->GetGUI()->GetResolution();
        cursor.x -= windowPos.x;
        cursor.y -= windowPos.y;
        if (cursor.x < 0)
            cursor.x = 0;
        else if (cursor.x > (long)vecResolution.fX)
            cursor.x = (long)vecResolution.fX;
        if (cursor.y < 0)
            cursor.y = 0;
        else if (cursor.y > (long)vecResolution.fY)
            cursor.y = (long)vecResolution.fY;
    }

    if (m_ucFade == FADE_IN)
    {
        m_fFader += CORE_MTA_FADER;

        float fFadeTarget = m_bIsIngame ? CORE_MTA_BG_INGAME_ALPHA : CORE_MTA_BG_MAX_ALPHA;

        if (m_fFader > 0.0f)
        {
            m_bIsVisible = true;

            if (!m_bCursorAlphaReset)
            {
                CGUI* pGUI = g_pCore->GetGUI();

                if (pGUI)
                {
                    pGUI->SetCursorAlpha(1.0f);
                    m_bCursorAlphaReset = true;
                }
            }
        }

        if (m_fFader >= fFadeTarget)
        {
            m_ucFade = FADE_VISIBLE;
            m_bIsVisible = true;
            m_bIsFullyVisible = true;
        }
    }
    else if (m_ucFade == FADE_OUT)
    {
        m_fFader -= CORE_MTA_FADER;

        if (m_fFader < 1.0f)
            m_bIsVisible = false;
        m_bCursorAlphaReset = false;

        if (m_fFader <= 0)
        {
            m_bIsFullyVisible = false;
            m_ucFade = FADE_INVISIBLE;
            m_bIsVisible = false;

            m_pCanvas->SetVisible(false);
            m_pBackground->SetVisible(false);
        }
    }

    if ((SystemState == 7 || SystemState == 9) && !m_bIsIngame)
    {
        if (pGame->HasCreditScreenFadedOut())
            WaitForMenu = std::max(WaitForMenu, 250);

        if (WaitForMenu >= 250)
        {
            m_bIsVisible = true;
            m_bStarted = true;
        }

        if (WaitForMenu < 300)
            WaitForMenu++;
    }

    if (m_bIsVisible && SystemState != 8)
    {
        if (SystemState == 7 || SystemState == 9 && !m_bIsIngame)
        {
            if (m_ucFade == FADE_INVISIBLE)
                Show(false);
        }
        else
        {
            if (m_ucFade == FADE_INVISIBLE)
                Show(true);
        }
    }

    ulPreviousTick = GetTickCount32();

    m_ServerBrowser.Update();
    m_ServerInfo.DoPulse();
}

void CMainMenu::Show(bool bOverlay)
{
    SetVisible(true, bOverlay);
}

void CMainMenu::Hide()
{
    SetVisible(false);
}

void CMainMenu::SetVisible(bool bVisible, bool bOverlay, bool bFrameDelay)
{
    CMultiplayer* pMultiplayer = g_pCore->GetMultiplayer();
    pMultiplayer->DisablePadHandler(bVisible);

    if ((m_ucFade == FADE_VISIBLE || m_ucFade == FADE_IN) && bVisible == false)
    {
        m_ucFade = FADE_OUT;
    }
    else if ((m_ucFade == FADE_INVISIBLE || m_ucFade == FADE_OUT) && bVisible == true)
    {
        m_ucFade = FADE_IN;
    }

    if (!bVisible)
    {
        m_bFrameDelay = bFrameDelay;
        SetMenuUnhovered();
        m_ServerBrowser.SetVisible(false);
        m_Settings.SetVisible(false);
    }
    else
    {
        m_bFrameDelay = bFrameDelay;
        SetMenuUnhovered();
        m_pCanvas->SetVisible(true);
        m_pBackground->SetVisible(true);

        if (m_pBackground)
        {
            m_pCanvas->SetVisible(false);
        }
    }

    m_bHideGame = !bOverlay;
}

bool CMainMenu::IsVisible()
{
    return m_bIsVisible;
}

void CMainMenu::SetIsIngame(bool bIsIngame)
{
    if (m_bIsIngame != bIsIngame)
    {
        m_bIsIngame = bIsIngame;
        m_Settings.SetIsModLoaded(bIsIngame);

        m_ulMoveStartTick = GetTickCount32();
        if (bIsIngame)
        {
            m_iMoveTargetPos = m_iSecondItemCentre;
        }
        m_iMoveStartPos = m_menuAY;
    }
}

bool CMainMenu::GetIsIngame()
{
    return m_bIsIngame;
}

bool CMainMenu::OnQuickConnectButtonClick(CGUIElement* pElement)
{
    if (m_ucFade != FADE_VISIBLE)
        return false;

    std::string strNick;
    CVARS_GET(("nick"), strNick);
    g_pCore->GetConnectManager()->Connect(IP_TO_AUTOCONNECT, PORT_TO_AUTOCONNECT, strNick.c_str(), (""));
    return true;
}

void CMainMenu::HideServerInfo()
{
    m_ServerInfo.Hide();
}

bool CMainMenu::OnSettingsButtonClick(CGUIElement* pElement)
{
    if (m_ucFade != FADE_VISIBLE)
        return false;
    {
        m_Settings.SetVisible(true);
    }

    return true;
}

bool CMainMenu::OnQuitButtonClick(CGUIElement* pElement)
{
    if (m_ucFade != FADE_VISIBLE)
        return false;

    g_pCore->Quit();

    return true;
}
