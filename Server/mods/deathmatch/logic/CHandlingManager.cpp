/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        mods/deathmatch/logic/CHandlingManager.cpp
 *  PURPOSE:     Vehicle handling manager
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

SFixedArray<tHandlingData, HT_MAX> CHandlingManager::m_OriginalHandlingData;

SFixedArray<CHandlingEntry*, HT_MAX> CHandlingManager::m_pOriginalEntries;
SFixedArray<CHandlingEntry*, HT_MAX> CHandlingManager::m_pModelEntries;

CHandlingManager::CHandlingManager()
{
    // Initialize all default handlings
    InitializeDefaultHandlings();

    // Create a handling entry for every original handling data
    for (int i = 0; i < HT_MAX; i++)
    {

            m_pOriginalEntries[i] = new CHandlingEntry(&m_OriginalHandlingData[i]);
 
    }

    // Create a handling entry for every model
    for (int i = 0; i < HT_MAX; i++)
    {
        m_pModelEntries[i] = new CHandlingEntry(&m_OriginalHandlingData[i]);
        m_bModelHandlingChanged[i] = false;
    }

    // http://www.gtamodding.com/index.php?title=Handling.cfg#GTA_San_Andreas
    // http://projectcerbera.com/gta/sa/tutorials/handling

    m_HandlingNames["mass"] = HANDLING_MASS;                                                             // works (mass > 0)
    m_HandlingNames["turnMass"] = HANDLING_TURNMASS;                                                     // works
    m_HandlingNames["dragCoeff"] = HANDLING_DRAGCOEFF;                                                   // works
    m_HandlingNames["centerOfMass"] = HANDLING_CENTEROFMASS;                                             // works
    m_HandlingNames["percentSubmerged"] = HANDLING_PERCENTSUBMERGED;                                     // works
    m_HandlingNames["tractionMultiplier"] = HANDLING_TRACTIONMULTIPLIER;                                 // works
    m_HandlingNames["driveType"] = HANDLING_DRIVETYPE;                                                   // works
    m_HandlingNames["engineType"] = HANDLING_ENGINETYPE;                                                 // works
    m_HandlingNames["numberOfGears"] = HANDLING_NUMOFGEARS;                                              // works
    m_HandlingNames["engineAcceleration"] = HANDLING_ENGINEACCELERATION;                                 // works
    m_HandlingNames["engineInertia"] = HANDLING_ENGINEINERTIA;                                           // works
    m_HandlingNames["maxVelocity"] = HANDLING_MAXVELOCITY;                                               // works
    m_HandlingNames["brakeDeceleration"] = HANDLING_BRAKEDECELERATION;                                   // works
    m_HandlingNames["brakeBias"] = HANDLING_BRAKEBIAS;                                                   // works
    m_HandlingNames["ABS"] = HANDLING_ABS;                                                               // has no effect in vanilla gta either
    m_HandlingNames["steeringLock"] = HANDLING_STEERINGLOCK;                                             // works
    m_HandlingNames["tractionLoss"] = HANDLING_TRACTIONLOSS;                                             // works
    m_HandlingNames["tractionBias"] = HANDLING_TRACTIONBIAS;                                             // works
    m_HandlingNames["suspensionForceLevel"] = HANDLING_SUSPENSION_FORCELEVEL;                            // works
    m_HandlingNames["suspensionDamping"] = HANDLING_SUSPENSION_DAMPING;                                  // works
    m_HandlingNames["suspensionHighSpeedDamping"] = HANDLING_SUSPENSION_HIGHSPEEDDAMPING;                // works
    m_HandlingNames["suspensionUpperLimit"] = HANDLING_SUSPENSION_UPPER_LIMIT;                           // works
    m_HandlingNames["suspensionLowerLimit"] = HANDLING_SUSPENSION_LOWER_LIMIT;                           // works
    m_HandlingNames["suspensionFrontRearBias"] = HANDLING_SUSPENSION_FRONTREARBIAS;                      // works
    m_HandlingNames["suspensionAntiDiveMultiplier"] = HANDLING_SUSPENSION_ANTIDIVEMULTIPLIER;            // works
    m_HandlingNames["collisionDamageMultiplier"] = HANDLING_COLLISIONDAMAGEMULTIPLIER;                   // works
    m_HandlingNames["seatOffsetDistance"] = HANDLING_SEATOFFSETDISTANCE;                                 // works
    m_HandlingNames["monetary"] = HANDLING_MONETARY;                      // useless as it only influences SP stats (value of damaged property)
    m_HandlingNames["handlingFlags"] = HANDLING_HANDLINGFLAGS;            // works
    m_HandlingNames["modelFlags"] = HANDLING_MODELFLAGS;                  // works
    m_HandlingNames["headLight"] = HANDLING_HEADLIGHT;                    // doesn't work
    m_HandlingNames["tailLight"] = HANDLING_TAILLIGHT;                    // doesn't seem to work*
    m_HandlingNames["animGroup"] = HANDLING_ANIMGROUP;                    // works model based
}
//* needs testing by someone who knows more about handling

CHandlingManager::~CHandlingManager()
{
    // Destroy all original handling entries
    for (int i = 0; i < HT_MAX; i++)
    {
        delete m_pOriginalEntries[i];
    }

    // Destroy all model handling entries
    for (int i = 0; i < HT_MAX; i++)
    {
        delete m_pModelEntries[i];
    }
}

CHandlingEntry* CHandlingManager::CreateHandlingData()
{
    CHandlingEntry* pHandlingEntry = new CHandlingEntry();
    return pHandlingEntry;
}

bool CHandlingManager::ApplyHandlingData(eVehicleTypes eModel, CHandlingEntry* pEntry)
{
    // Within range?
    if (eModel >= 400 && eModel < 612)
    {
        // Get our Handling ID
        eHandlingTypes eHandling = GetHandlingID(eModel);
        // Apply the data and return success
        m_pModelEntries[eHandling]->ApplyHandlingData(pEntry);
        return true;
    }

    if (eModel >= 6526 && eModel <= 6711)
    {
        eHandlingTypes eHandling = GetHandlingID((static_cast<eVehicleTypes>(eModel)));
        // Apply the data and return success
        m_pModelEntries[eHandling]->ApplyHandlingData(pEntry);
        return true;
    }

    // Failed
    return false;
}

const CHandlingEntry* CHandlingManager::GetOriginalHandlingData(eVehicleTypes eModel)
{
    // Within range?
    if (eModel >= 400 && eModel < 612)
    {
        // Get our Handling ID
        eHandlingTypes eHandling = GetHandlingID(eModel);
        // Return it
        return m_pOriginalEntries[eHandling];
    }

    if (eModel >= 6526 && eModel <= 6711)
    {
        eHandlingTypes eHandling = GetHandlingID((static_cast<eVehicleTypes>(eModel)));
        // Return it
        return m_pOriginalEntries[eHandling];
    }

    return NULL;
}

const CHandlingEntry* CHandlingManager::GetModelHandlingData(eVehicleTypes eModel)
{
    // Within range?
    if ((eModel >= 400 && eModel < 612) || (eModel >= 6526 && eModel <= 6711))
    {
        // Get our Handling ID
        eHandlingTypes eHandling = GetHandlingID(eModel);
        // Return it
        return m_pModelEntries[eHandling];
    }



    return NULL;
}

eHandlingProperty CHandlingManager::GetPropertyEnumFromName(std::string strName)
{
    std::map<std::string, eHandlingProperty>::iterator it;
    it = m_HandlingNames.find(strName);

    if (it != m_HandlingNames.end())
    {
        return it->second;
    }

    return HANDLING_MAX;
}

bool CHandlingManager::HasModelHandlingChanged(eVehicleTypes eModel)
{
    // Within range?
    if (eModel >= 6526 && eModel <= 6711)
    {
        // Get our Handling ID
        eHandlingTypes eHandling = GetHandlingID(static_cast<eVehicleTypes>(eModel));
        // Return if we have changed
        return m_bModelHandlingChanged[eHandling];
    }
    else
    {
        eHandlingTypes eHandling = GetHandlingID(static_cast<eVehicleTypes>(eModel));
        // Return if we have changed
        return m_bModelHandlingChanged[eHandling];
    }

    return false;
}

void CHandlingManager::SetModelHandlingHasChanged(eVehicleTypes eModel, bool bChanged)
{


    eHandlingTypes eHandling = GetHandlingID(eModel);

    // Get our Handling ID
    
    // Return if we have changed.
    m_bModelHandlingChanged[eHandling] = bChanged;
}

// Return the handling manager id
eHandlingTypes CHandlingManager::GetHandlingID(eVehicleTypes eModel)
{
    switch (eModel)
    {
        case VT_LANDSTAL:
            return HT_LANDSTAL;
        case VT_BRAVURA:
            return HT_BRAVURA;
        case VT_BUFFALO:
            return HT_BUFFALO;
        case VT_LINERUN:
            return HT_LINERUN;
        case VT_PEREN:
            return HT_PEREN;
        case VT_SENTINEL:
            return HT_SENTINEL;
        case VT_DUMPER:
            return HT_DUMPER;
        case VT_FIRETRUK:
            return HT_FIRETRUK;
        case VT_TRASH:
            return HT_TRASH;
        case VT_STRETCH:
            return HT_STRETCH;
        case VT_MANANA:
            return HT_MANANA;
        case VT_INFERNUS:
            return HT_INFERNUS;
        case VT_VOODOO:
            return HT_VOODOO;
        case VT_PONY:
            return HT_PONY;
        case VT_MULE:
            return HT_MULE;
        case VT_CHEETAH:
            return HT_CHEETAH;
        case VT_AMBULAN:
            return HT_AMBULAN;
        case VT_LEVIATHN:
            return HT_LEVIATHN;
        case VT_MOONBEAM:
            return HT_MOONBEAM;
        case VT_ESPERANT:
            return HT_ESPERANT;
        case VT_TAXI:
            return HT_TAXI;
        case VT_WASHING:
            return HT_WASHING;
        case VT_BOBCAT:
            return HT_BOBCAT;
        case VT_MRWHOOP:
            return HT_MRWHOOP;
        case VT_BFINJECT:
            return HT_BFINJECT;
        case VT_HUNTER:
            return HT_HUNTER;
        case VT_PREMIER:
            return HT_PREMIER;
        case VT_ENFORCER:
            return HT_ENFORCER;
        case VT_SECURICA:
            return HT_SECURICA;
        case VT_BANSHEE:
            return HT_BANSHEE;
        case VT_PREDATOR:
            return HT_PREDATOR;
        case VT_BUS:
            return HT_BUS;
        case VT_RHINO:
            return HT_RHINO;
        case VT_BARRACKS:
            return HT_BARRACKS;
        case VT_HOTKNIFE:
            return HT_HOTKNIFE;
        case VT_ARTICT1:
            return HT_ARTICT1;
        case VT_PREVION:
            return HT_PREVION;
        case VT_COACH:
            return HT_COACH;
        case VT_CABBIE:
            return HT_CABBIE;
        case VT_STALLION:
            return HT_STALLION;
        case VT_RUMPO:
            return HT_RUMPO;
        case VT_RCBANDIT:
            return HT_RCBANDIT;
        case VT_ROMERO:
            return HT_ROMERO;
        case VT_PACKER:
            return HT_PACKER;
        case VT_MONSTER:
            return HT_MONSTER;
        case VT_ADMIRAL:
            return HT_ADMIRAL;
        case VT_SQUALO:
            return HT_SQUALO;
        case VT_SEASPAR:
            return HT_SEASPAR;
        case VT_PIZZABOY:
            return HT_PIZZABOY;
        case VT_TRAM:
            return HT_TRAM;
        case VT_ARTICT2:
            return HT_ARTICT2;
        case VT_TURISMO:
            return HT_TURISMO;
        case VT_SPEEDER:
            return HT_SPEEDER;
        case VT_REEFER:
            return HT_REEFER;
        case VT_TROPIC:
            return HT_TROPIC;
        case VT_FLATBED:
            return HT_FLATBED;
        case VT_YANKEE:
            return HT_YANKEE;
        case VT_CADDY:
            return HT_GOLFCART;
        case VT_SOLAIR:
            return HT_SOLAIR;
        case VT_TOPFUN:
            return HT_TOPFUN;
        case VT_SKIMMER:
            return HT_SEAPLANE;
        case VT_PCJ600:
            return HT_BIKE;
        case VT_FAGGIO:
            return HT_FAGGIO;
        case VT_FREEWAY:
            return HT_FREEWAY;
        case VT_RCBARON:
            return HT_RCBARON;
        case VT_RCRAIDER:
            return HT_RCRAIDER;
        case VT_GLENDALE:
            return HT_GLENDALE;
        case VT_OCEANIC:
            return HT_OCEANIC;
        case VT_SANCHEZ:
            return HT_DIRTBIKE;
        case VT_SPARROW:
            return HT_SPARROW;
        case VT_PATRIOT:
            return HT_PATRIOT;
        case VT_QUAD:
            return HT_QUADBIKE;
        case VT_COASTG:
            return HT_COASTGRD;
        case VT_DINGHY:
            return HT_DINGHY;
        case VT_HERMES:
            return HT_HERMES;
        case VT_SABRE:
            return HT_SABRE;
        case VT_RUSTLER:
            return HT_RUSTLER;
        case VT_ZR350:
            return HT_ZR350;
        case VT_WALTON:
            return HT_WALTON;
        case VT_REGINA:
            return HT_REGINA;
        case VT_COMET:
            return HT_COMET;
        case VT_BMX:
            return HT_BMX;
        case VT_BURRITO:
            return HT_BURRITO;
        case VT_CAMPER:
            return HT_CAMPER;
        case VT_MARQUIS:
            return HT_MARQUIS;
        case VT_BAGGAGE:
            return HT_BAGGAGE;
        case VT_DOZER:
            return HT_DOZER;
        case VT_MAVERICK:
            return HT_MAVERICK;
        case VT_VCNMAV:
            return HT_COASTMAV;
        case VT_RANCHER:
            return HT_RANCHER;
        case VT_FBIRANCH:
            return HT_FBIRANCH;
        case VT_VIRGO:
            return HT_VIRGO;
        case VT_GREENWOO:
            return HT_GREENWOO;
        case VT_JETMAX:
            return HT_CUPBOAT;
        case VT_HOTRING:
            return HT_HOTRING;
        case VT_SANDKING:
            return HT_SANDKING;
        case VT_BLISTAC:
            return HT_BLISTAC;
        case VT_POLMAV:
            return HT_POLMAV;
        case VT_BOXVILLE:
            return HT_BOXVILLE;
        case VT_BENSON:
            return HT_BENSON;
        case VT_MESA:
            return HT_MESA;
        case VT_RCGOBLIN:
            return HT_RCGOBLIN;
        case VT_HOTRINA:
            return HT_HOTRINA;
        case VT_HOTRINB:
            return HT_HOTRINB;
        case VT_BLOODRA:
            return HT_BLOODRA;
        case VT_RNCHLURE:
            return HT_RNCHLURE;
        case VT_SUPERGT:
            return HT_SUPERGT;
        case VT_ELEGANT:
            return HT_ELEGANT;
        case VT_JOURNEY:
            return HT_JOURNEY;
        case VT_BIKE:
            return HT_CHOPPERB;
        case VT_MTBIKE:
            return HT_MTB;
        case VT_BEAGLE:
            return HT_BEAGLE;
        case VT_CROPDUST:
            return HT_CROPDUST;
        case VT_STUNT:
            return HT_STUNT;
        case VT_PETRO:
            return HT_PETROL;
        case VT_RDTRAIN:
            return HT_RDTRAIN;
        case VT_NEBULA:
            return HT_NEBULA;
        case VT_MAJESTIC:
            return HT_MAJESTIC;
        case VT_BUCCANEE:
            return HT_BUCCANEE;
        case VT_SHAMAL:
            return HT_SHAMAL;
        case VT_HYDRA:
            return HT_HYDRA;
        case VT_FCR900:
            return HT_FCR900;
        case VT_NRG500:
            return HT_NRG500;
        case VT_COPBIKE:
            return HT_HPV1000;
        case VT_CEMENT:
            return HT_CEMENT;
        case VT_TOWTRUCK:
            return HT_TOWTRUCK;
        case VT_FORTUNE:
            return HT_FORTUNE;
        case VT_CADRONA:
            return HT_CADRONA;
        case VT_FBITRUCK:
            return HT_FBITRUCK;
        case VT_WILLARD:
            return HT_WILLARD;
        case VT_FORKLIFT:
            return HT_FORKLIFT;
        case VT_TRACTOR:
            return HT_TRACTOR;
        case VT_COMBINE:
            return HT_COMBINE;
        case VT_FELTZER:
            return HT_FELTZER;
        case VT_REMINGTN:
            return HT_REMINGTN;
        case VT_SLAMVAN:
            return HT_SLAMVAN;
        case VT_BLADE:
            return HT_BLADE;
        case VT_FREIGHT:
            return HT_FREIGHT;
        case VT_STREAK:
            return HT_STREAK;
        case VT_VORTEX:
            return HT_VORTEX;
        case VT_VINCENT:
            return HT_VINCENT;
        case VT_BULLET:
            return HT_BULLET;
        case VT_CLOVER:
            return HT_CLOVER;
        case VT_SADLER:
            return HT_SADLER;
        case VT_FIRELA:
            return HT_FIRELA;
        case VT_HUSTLER:
            return HT_HUSTLER;
        case VT_INTRUDER:
            return HT_INTRUDER;
        case VT_PRIMO:
            return HT_PRIMO;
        case VT_CARGOBOB:
            return HT_CARGOBOB;
        case VT_TAMPA:
            return HT_TAMPA;
        case VT_SUNRISE:
            return HT_SUNRISE;
        case VT_MERIT:
            return HT_MERIT;
        case VT_UTILITY:
            return HT_UTILITY;
        case VT_NEVADA:
            return HT_NEVADA;
        case VT_YOSEMITE:
            return HT_YOSEMITE;
        case VT_WINDSOR:
            return HT_WINDSOR;
        case VT_MONSTERA:
            return HT_MTRUCK_A;
        case VT_MONSTERB:
            return HT_MTRUCK_B;
        case VT_URANUS:
            return HT_URANUS;
        case VT_JESTER:
            return HT_JESTER;
        case VT_SULTAN:
            return HT_SULTAN;
        case VT_STRATUM:
            return HT_STRATUM;
        case VT_ELEGY:
            return HT_ELEGY;
        case VT_RAINDANC:
            return HT_RAINDANC;
        case VT_RCTIGER:
            return HT_RCTIGER;
        case VT_FLASH:
            return HT_FLASH;
        case VT_TAHOMA:
            return HT_TAHOMA;
        case VT_SAVANNA:
            return HT_SAVANNA;
        case VT_BANDITO:
            return HT_BANDITO;
        case VT_FREIFLAT:
            return HT_FREIFLAT;
        case VT_STREAKC:
            return HT_CSTREAK;
        case VT_KART:
            return HT_KART;
        case VT_MOWER:
            return HT_MOWER;
        case VT_DUNERIDE:
            return HT_DUNE;
        case VT_SWEEPER:
            return HT_SWEEPER;
        case VT_BROADWAY:
            return HT_BROADWAY;
        case VT_TORNADO:
            return HT_TORNADO;
        case VT_AT400:
            return HT_AT400;
        case VT_DFT30:
            return HT_DFT30;
        case VT_HUNTLEY:
            return HT_HUNTLEY;
        case VT_STAFFORD:
            return HT_STAFFORD;
        case VT_BF400:
            return HT_BF400;
        case VT_NEWSVAN:
            return HT_NEWSVAN;
        case VT_TUG:
            return HT_TUG;
        case VT_PETROTR:
            return HT_PETROTR;
        case VT_EMPEROR:
            return HT_EMPEROR;
        case VT_WAYFARER:
            return HT_WAYFARER;
        case VT_EUROS:
            return HT_EUROS;
        case VT_HOTDOG:
            return HT_HOTDOG;
        case VT_CLUB:
            return HT_CLUB;
        case VT_FREIBOX:
            return HT_FREIBOX;
        case VT_ARTICT3:
            return HT_ARTICT3;
        case VT_ANDROM:
            return HT_ANDROM;
        case VT_DODO:
            return HT_DODO;
        case VT_RCCAM:
            return HT_RCCAM;
        case VT_LAUNCH:
            return HT_LAUNCH;
        case VT_COPCARLA:
            return HT_POLICE_LA;
        case VT_COPCARSF:
            return HT_POLICE_SF;
        case VT_COPCARVG:
            return HT_POLICE_VG;
        case VT_COPCARRU:
            return HT_POLRANGER;
        case VT_PICADOR:
            return HT_PICADOR;
        case VT_SWATVAN:
            return HT_SWATVAN;
        case VT_ALPHA:
            return HT_ALPHA;
        case VT_PHOENIX:
            return HT_PHOENIX;
        case VT_GLENSHIT:
            return HT_GLENSHIT;
        case VT_SADLSHIT:
            return HT_SADLSHIT;
        case VT_BAGBOXA:
            return HT_BAGBOXA;
        case VT_BAGBOXB:
            return HT_BAGBOXB;
        case VT_TUGSTAIR:
            return HT_STAIRS;
        case VT_BOXBURG:
            return HT_BOXBURG;
        case VT_FARMTR1:
            return HT_FARM_TR1;
        case VT_UTILTR1:
            return HT_UTIL_TR1;
        case VT_ID6526:
            return HT_ID6526;
        case VT_ID6527:
            return HT_ID6527;
        case VT_ID6528:
            return HT_ID6528;
        case VT_ID6529:
            return HT_ID6529;
        case VT_ID6530:
            return HT_ID6530;
        case VT_ID6531:
            return HT_ID6531;
        case VT_ID6532:
            return HT_ID6532;
        case VT_ID6533:
            return HT_ID6533;
        case VT_ID6534:
            return HT_ID6534;
        case VT_ID6535:
            return HT_ID6535;
        case VT_ID6536:
            return HT_ID6536;
        case VT_ID6537:
            return HT_ID6537;
        case VT_ID6538:
            return HT_ID6538;
        case VT_ID6539:
            return HT_ID6539;
        case VT_ID6540:
            return HT_ID6540;
        case VT_ID6541:
            return HT_ID6541;
        case VT_ID6542:
            return HT_ID6542;
        case VT_ID6543:
            return HT_ID6543;
        case VT_ID6544:
            return HT_ID6544;
        case VT_ID6545:
            return HT_ID6545;
        case VT_ID6546:
            return HT_ID6546;
        case VT_ID6547:
            return HT_ID6547;
        case VT_ID6548:
            return HT_ID6548;
        case VT_ID6549:
            return HT_ID6549;
        case VT_ID6550:
            return HT_ID6550;
        case VT_ID6551:
            return HT_ID6551;
        case VT_ID6552:
            return HT_ID6552;
        case VT_ID6553:
            return HT_ID6553;
        case VT_ID6554:
            return HT_ID6554;
        case VT_ID6555:
            return HT_ID6555;
        case VT_ID6556:
            return HT_ID6556;
        case VT_ID6557:
            return HT_ID6557;
        case VT_ID6558:
            return HT_ID6558;
        case VT_ID6559:
            return HT_ID6559;
        case VT_ID6560:
            return HT_ID6560;
        case VT_ID6561:
            return HT_ID6561;
        case VT_ID6562:
            return HT_ID6562;
        case VT_ID6563:
            return HT_ID6563;
        case VT_ID6564:
            return HT_ID6564;
        case VT_ID6565:
            return HT_ID6565;
        case VT_ID6566:
            return HT_ID6566;
        case VT_ID6567:
            return HT_ID6567;
        case VT_ID6568:
            return HT_ID6568;
        case VT_ID6569:
            return HT_ID6569;
        case VT_ID6570:
            return HT_ID6570;
        case VT_ID6571:
            return HT_ID6571;
        case VT_ID6572:
            return HT_ID6572;
        case VT_ID6573:
            return HT_ID6573;
        case VT_ID6574:
            return HT_ID6574;
        case VT_ID6575:
            return HT_ID6575;
        case VT_ID6576:
            return HT_ID6576;
        case VT_ID6577:
            return HT_ID6577;
        case VT_ID6578:
            return HT_ID6578;
        case VT_ID6579:
            return HT_ID6579;
        case VT_ID6580:
            return HT_ID6580;
        case VT_ID6581:
            return HT_ID6581;
        case VT_ID6582:
            return HT_ID6582;
        case VT_ID6583:
            return HT_ID6583;
        case VT_ID6584:
            return HT_ID6584;
        case VT_ID6585:
            return HT_ID6585;
        case VT_ID6586:
            return HT_ID6586;
        case VT_ID6587:
            return HT_ID6587;
        case VT_ID6588:
            return HT_ID6588;
        case VT_ID6589:
            return HT_ID6589;
        case VT_ID6590:
            return HT_ID6590;
        case VT_ID6591:
            return HT_ID6591;
        case VT_ID6592:
            return HT_ID6592;
        case VT_ID6593:
            return HT_ID6593;
        case VT_ID6594:
            return HT_ID6594;
        case VT_ID6595:
            return HT_ID6595;
        case VT_ID6596:
            return HT_ID6596;
        case VT_ID6597:
            return HT_ID6597;
        case VT_ID6598:
            return HT_ID6598;
        case VT_ID6599:
            return HT_ID6599;
        case VT_ID6600:
            return HT_ID6600;
        case VT_ID6601:
            return HT_ID6601;
        case VT_ID6602:
            return HT_ID6602;
        case VT_ID6603:
            return HT_ID6603;
        case VT_ID6604:
            return HT_ID6604;
        case VT_ID6605:
            return HT_ID6605;
        case VT_ID6606:
            return HT_ID6606;
        case VT_ID6607:
            return HT_ID6607;
        case VT_ID6608:
            return HT_ID6608;
        case VT_ID6609:
            return HT_ID6609;
        case VT_ID6610:
            return HT_ID6610;
        case VT_ID6611:
            return HT_ID6611;
        case VT_ID6612:
            return HT_ID6612;
        case VT_ID6613:
            return HT_ID6613;
        case VT_ID6614:
            return HT_ID6614;
        case VT_ID6615:
            return HT_ID6615;
        case VT_ID6616:
            return HT_ID6616;
        case VT_ID6617:
            return HT_ID6617;
        case VT_ID6618:
            return HT_ID6618;
        case VT_ID6619:
            return HT_ID6619;
        case VT_ID6620:
            return HT_ID6620;
        case VT_ID6621:
            return HT_ID6621;
        case VT_ID6622:
            return HT_ID6622;
        case VT_ID6623:
            return HT_ID6623;
        case VT_ID6624:
            return HT_ID6624;
        case VT_ID6625:
            return HT_ID6625;
        case VT_ID6626:
            return HT_ID6626;
        case VT_ID6627:
            return HT_ID6627;
        case VT_ID6628:
            return HT_ID6628;
        case VT_ID6629:
            return HT_ID6629;
        case VT_ID6630:
            return HT_ID6630;
        case VT_ID6631:
            return HT_ID6631;
        case VT_ID6632:
            return HT_ID6632;
        case VT_ID6633:
            return HT_ID6633;
        case VT_ID6634:
            return HT_ID6634;
        case VT_ID6635:
            return HT_ID6635;
        case VT_ID6636:
            return HT_ID6636;
        case VT_ID6637:
            return HT_ID6637;
        case VT_ID6638:
            return HT_ID6638;
        case VT_ID6639:
            return HT_ID6639;
        case VT_ID6640:
            return HT_ID6640;
        case VT_ID6641:
            return HT_ID6641;
        case VT_ID6642:
            return HT_ID6642;
        case VT_ID6643:
            return HT_ID6643;
        case VT_ID6644:
            return HT_ID6644;
        case VT_ID6645:
            return HT_ID6645;
        case VT_ID6646:
            return HT_ID6646;
        case VT_ID6647:
            return HT_ID6647;
        case VT_ID6648:
            return HT_ID6648;
        case VT_ID6649:
            return HT_ID6649;
        case VT_ID6650:
            return HT_ID6650;
        case VT_ID6651:
            return HT_ID6651;
        case VT_ID6652:
            return HT_ID6652;
        case VT_ID6653:
            return HT_ID6653;
        case VT_ID6654:
            return HT_ID6654;
        case VT_ID6655:
            return HT_ID6655;
        case VT_ID6656:
            return HT_ID6656;
        case VT_ID6657:
            return HT_ID6657;
        case VT_ID6658:
            return HT_ID6658;
        case VT_ID6659:
            return HT_ID6659;
        case VT_ID6660:
            return HT_ID6660;
        case VT_ID6661:
            return HT_ID6661;
        case VT_ID6662:
            return HT_ID6662;
        case VT_ID6663:
            return HT_ID6663;
        case VT_ID6664:
            return HT_ID6664;
        case VT_ID6665:
            return HT_ID6665;
        case VT_ID6666:
            return HT_ID6666;
        case VT_ID6667:
            return HT_ID6667;
        case VT_ID6668:
            return HT_ID6668;
        case VT_ID6669:
            return HT_ID6669;
        case VT_ID6670:
            return HT_ID6670;
        case VT_ID6671:
            return HT_ID6671;
        case VT_ID6672:
            return HT_ID6672;
        case VT_ID6673:
            return HT_ID6673;
        case VT_ID6674:
            return HT_ID6674;
        case VT_ID6675:
            return HT_ID6675;
        case VT_ID6676:
            return HT_ID6676;
        case VT_ID6677:
            return HT_ID6677;
        case VT_ID6678:
            return HT_ID6678;
        case VT_ID6679:
            return HT_ID6679;
        case VT_ID6680:
            return HT_ID6680;
        case VT_ID6681:
            return HT_ID6681;
        case VT_ID6682:
            return HT_ID6682;
        case VT_ID6683:
            return HT_ID6683;
        case VT_ID6684:
            return HT_ID6684;
        case VT_ID6685:
            return HT_ID6685;
        case VT_ID6686:
            return HT_ID6686;
        case VT_ID6687:
            return HT_ID6687;
        case VT_ID6688:
            return HT_ID6688;
        case VT_ID6689:
            return HT_ID6689;
        case VT_ID6690:
            return HT_ID6690;
        case VT_ID6691:
            return HT_ID6691;
        case VT_ID6692:
            return HT_ID6692;
        case VT_ID6693:
            return HT_ID6693;
        case VT_ID6694:
            return HT_ID6694;
        case VT_ID6695:
            return HT_ID6695;
        case VT_ID6696:
            return HT_ID6696;
        case VT_ID6697:
            return HT_ID6697;
        case VT_ID6698:
            return HT_ID6698;
        case VT_ID6699:
            return HT_ID6699;
        case VT_ID6700:
            return HT_ID6700;
        case VT_ID6701:
            return HT_ID6701;
        case VT_ID6702:
            return HT_ID6702;
        case VT_ID6703:
            return HT_ID6703;
        case VT_ID6704:
            return HT_ID6704;
        case VT_ID6705:
            return HT_ID6705;
        case VT_ID6706:
            return HT_ID6706;
        case VT_ID6707:
            return HT_ID6707;
        case VT_ID6708:
            return HT_ID6708;
        case VT_ID6709:
            return HT_ID6709;
        case VT_ID6710:
            return HT_ID6710;
        case VT_ID6711:
            return HT_ID6711;
        case VT_ID6712:
            return HT_ID6712;
        case VT_ID6713:
            return HT_ID6713;
        case VT_ID6714:
            return HT_ID6714;
        case VT_ID6715:
            return HT_ID6715;
        case VT_ID6716:
            return HT_ID6716;
        case VT_ID6717:
            return HT_ID6717;
        case VT_ID6718:
            return HT_ID6718;
        case VT_ID6719:
            return HT_ID6719;
        case VT_ID6720:
            return HT_ID6720;
        case VT_ID6721:
            return HT_ID6721;
        case VT_ID6722:
            return HT_ID6722;
        case VT_ID6723:
            return HT_ID6723;
        case VT_ID6724:
            return HT_ID6724;
        case VT_ID6725:
            return HT_ID6725;
        case VT_ID6726:
            return HT_ID6726;
        case VT_ID6727:
            return HT_ID6727;
        case VT_ID6728:
            return HT_ID6728;
        case VT_ID6729:
            return HT_ID6729;
        case VT_ID6730:
            return HT_ID6730;
        case VT_ID6731:
            return HT_ID6731;
        case VT_ID6732:
            return HT_ID6732;
        case VT_ID6733:
            return HT_ID6733;
        case VT_ID6734:
            return HT_ID6734;
        case VT_ID6735:
            return HT_ID6735;
        case VT_ID6736:
            return HT_ID6736;
        case VT_ID6737:
            return HT_ID6737;
        case VT_ID6738:
            return HT_ID6738;
        case VT_ID6739:
            return HT_ID6739;
        case VT_ID6740:
            return HT_ID6740;
        case VT_ID6741:
            return HT_ID6741;
        case VT_ID6742:
            return HT_ID6742;
        case VT_ID6743:
            return HT_ID6743;
        case VT_ID6744:
            return HT_ID6744;
        case VT_ID6745:
            return HT_ID6745;
        case VT_ID6746:
            return HT_ID6746;
        case VT_ID6747:
            return HT_ID6747;
        case VT_ID6748:
            return HT_ID6748;
        case VT_ID6749:
            return HT_ID6749;
        case VT_ID6750:
            return HT_ID6750;
        case VT_ID6751:
            return HT_ID6751;
        case VT_ID6752:
            return HT_ID6752;
        case VT_ID6753:
            return HT_ID6753;
        case VT_ID6754:
            return HT_ID6754;
        case VT_ID6755:
            return HT_ID6755;
        case VT_ID6756:
            return HT_ID6756;
        case VT_ID6757:
            return HT_ID6757;
        case VT_ID6758:
            return HT_ID6758;
        case VT_ID6759:
            return HT_ID6759;
        case VT_ID6760:
            return HT_ID6760;
        case VT_ID6761:
            return HT_ID6761;
        case VT_ID6762:
            return HT_ID6762;
        case VT_ID6763:
            return HT_ID6763;
        case VT_ID6764:
            return HT_ID6764;
        case VT_ID6765:
            return HT_ID6765;
        case VT_ID6766:
            return HT_ID6766;
        case VT_ID6767:
            return HT_ID6767;
        case VT_ID6768:
            return HT_ID6768;
        case VT_ID6769:
            return HT_ID6769;
        case VT_ID6770:
            return HT_ID6770;
        case VT_ID6771:
            return HT_ID6771;
        case VT_ID6772:
            return HT_ID6772;
        case VT_ID6773:
            return HT_ID6773;
        case VT_ID6774:
            return HT_ID6774;
        case VT_ID6775:
            return HT_ID6775;
        case VT_ID6776:
            return HT_ID6776;
        case VT_ID6777:
            return HT_ID6777;
        case VT_ID6778:
            return HT_ID6778;
        case VT_ID6779:
            return HT_ID6779;
        case VT_ID6780:
            return HT_ID6780;
        case VT_ID6781:
            return HT_ID6781;
        case VT_ID6782:
            return HT_ID6782;
        case VT_ID6783:
            return HT_ID6783;
        case VT_ID6784:
            return HT_ID6784;
        case VT_ID6785:
            return HT_ID6785;
        case VT_ID6786:
            return HT_ID6786;
        case VT_ID6787:
            return HT_ID6787;
        case VT_ID6788:
            return HT_ID6788;
        case VT_ID6789:
            return HT_ID6789;
        case VT_ID6790:
            return HT_ID6790;
        case VT_ID6791:
            return HT_ID6791;
        case VT_ID6792:
            return HT_ID6792;
        case VT_ID6793:
            return HT_ID6793;
        case VT_ID6794:
            return HT_ID6794;
        case VT_ID6795:
            return HT_ID6795;
        case VT_ID6796:
            return HT_ID6796;
        case VT_ID6797:
            return HT_ID6797;
        case VT_ID6798:
            return HT_ID6798;
        case VT_ID6799:
            return HT_ID6799;
        case VT_ID6800:
            return HT_ID6800;
        case VT_ID6801:
            return HT_ID6801;
        case VT_ID6802:
            return HT_ID6802;
        case VT_ID6803:
            return HT_ID6803;
        case VT_ID6804:
            return HT_ID6804;
        case VT_ID6805:
            return HT_ID6805;
        case VT_ID6806:
            return HT_ID6806;
        case VT_ID6807:
            return HT_ID6807;
        case VT_ID6808:
            return HT_ID6808;
        case VT_ID6809:
            return HT_ID6809;
        case VT_ID6810:
            return HT_ID6810;
        case VT_ID6811:
            return HT_ID6811;
        case VT_ID6812:
            return HT_ID6812;
        case VT_ID6813:
            return HT_ID6813;
        case VT_ID6814:
            return HT_ID6814;
        case VT_ID6815:
            return HT_ID6815;
        case VT_ID6816:
            return HT_ID6816;
        case VT_ID6817:
            return HT_ID6817;
        case VT_ID6818:
            return HT_ID6818;
        case VT_ID6819:
            return HT_ID6819;
        case VT_ID6820:
            return HT_ID6820;
        case VT_ID6821:
            return HT_ID6821;
        case VT_ID6822:
            return HT_ID6822;
        case VT_ID6823:
            return HT_ID6823;
        case VT_ID6824:
            return HT_ID6824;
        case VT_ID6825:
            return HT_ID6825;
        default:
            break;
    }
    return HT_LANDSTAL;
}

void CHandlingManager::InitializeDefaultHandlings()
{
    // Reset
    memset(&m_OriginalHandlingData[0], 0, sizeof(m_OriginalHandlingData));

    // NB: Don't waste your time changing this manually. Use the dumping code
    //     commented out at the bottom :)
    m_OriginalHandlingData[0].iVehicleID = 0;
    m_OriginalHandlingData[0].fMass = 1700.000000f;
    m_OriginalHandlingData[0].fTurnMass = 5008.299805f;
    m_OriginalHandlingData[0].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[0].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.300000f);
    m_OriginalHandlingData[0].uiPercentSubmerged = 85;
    m_OriginalHandlingData[0].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[0].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[0].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[0].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[0].Transmission.uiHandlingFlags = 5242882;
    m_OriginalHandlingData[0].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[0].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[0].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[0].fBrakeDeceleration = 6.200000f;
    m_OriginalHandlingData[0].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[0].bABS = false;
    m_OriginalHandlingData[0].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[0].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[0].fTractionBias = 0.500000f;
    m_OriginalHandlingData[0].fSuspensionForceLevel = 2.400000f;
    m_OriginalHandlingData[0].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[0].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[0].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[0].fSuspensionLowerLimit = -0.140000f;
    m_OriginalHandlingData[0].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[0].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[0].fCollisionDamageMultiplier = 0.230000f;
    m_OriginalHandlingData[0].uiModelFlags = 0x20;
    m_OriginalHandlingData[0].uiHandlingFlags = 0x500002;
    m_OriginalHandlingData[0].fSeatOffsetDistance = 0.270000f;
    m_OriginalHandlingData[0].uiMonetary = 25000;
    m_OriginalHandlingData[0].ucHeadLight = 0;
    m_OriginalHandlingData[0].ucTailLight = 1;
    m_OriginalHandlingData[0].ucAnimGroup = 0;

    m_OriginalHandlingData[1].iVehicleID = 1;
    m_OriginalHandlingData[1].fMass = 1300.000000f;
    m_OriginalHandlingData[1].fTurnMass = 2200.000000f;
    m_OriginalHandlingData[1].fDragCoeff = 1.700000f;
    m_OriginalHandlingData[1].vecCenterOfMass = CVector(0.000000f, 0.300000f, 0.000000f);
    m_OriginalHandlingData[1].uiPercentSubmerged = 70;
    m_OriginalHandlingData[1].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[1].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[1].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[1].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[1].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[1].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[1].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[1].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[1].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[1].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[1].bABS = false;
    m_OriginalHandlingData[1].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[1].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[1].fTractionBias = 0.520000f;
    m_OriginalHandlingData[1].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[1].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[1].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[1].fSuspensionUpperLimit = 0.310000f;
    m_OriginalHandlingData[1].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[1].fSuspensionFrontRearBias = 0.570000f;
    m_OriginalHandlingData[1].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[1].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[1].uiModelFlags = 0x1;
    m_OriginalHandlingData[1].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[1].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[1].uiMonetary = 9000;
    m_OriginalHandlingData[1].ucHeadLight = 0;
    m_OriginalHandlingData[1].ucTailLight = 0;
    m_OriginalHandlingData[1].ucAnimGroup = 0;

    m_OriginalHandlingData[2].iVehicleID = 2;
    m_OriginalHandlingData[2].fMass = 1500.000000f;
    m_OriginalHandlingData[2].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[2].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[2].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[2].uiPercentSubmerged = 85;
    m_OriginalHandlingData[2].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[2].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[2].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[2].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[2].Transmission.uiHandlingFlags = 270532608;
    m_OriginalHandlingData[2].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[2].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[2].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[2].fBrakeDeceleration = 11.000000f;
    m_OriginalHandlingData[2].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[2].bABS = false;
    m_OriginalHandlingData[2].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[2].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[2].fTractionBias = 0.500000f;
    m_OriginalHandlingData[2].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[2].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[2].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[2].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[2].fSuspensionLowerLimit = -0.240000f;
    m_OriginalHandlingData[2].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[2].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[2].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[2].uiModelFlags = 0x2800;
    m_OriginalHandlingData[2].uiHandlingFlags = 0x10200000;
    m_OriginalHandlingData[2].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[2].uiMonetary = 35000;
    m_OriginalHandlingData[2].ucHeadLight = 1;
    m_OriginalHandlingData[2].ucTailLight = 1;
    m_OriginalHandlingData[2].ucAnimGroup = 0;

    m_OriginalHandlingData[3].iVehicleID = 3;
    m_OriginalHandlingData[3].fMass = 3800.000000f;
    m_OriginalHandlingData[3].fTurnMass = 19953.199219f;
    m_OriginalHandlingData[3].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[3].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[3].uiPercentSubmerged = 90;
    m_OriginalHandlingData[3].fTractionMultiplier = 0.950000f;
    m_OriginalHandlingData[3].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[3].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[3].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[3].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[3].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[3].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[3].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[3].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[3].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[3].bABS = false;
    m_OriginalHandlingData[3].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[3].fTractionLoss = 0.650000f;
    m_OriginalHandlingData[3].fTractionBias = 0.400000f;
    m_OriginalHandlingData[3].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[3].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[3].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[3].fSuspensionUpperLimit = 0.400000f;
    m_OriginalHandlingData[3].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[3].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[3].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[3].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[3].uiModelFlags = 0x6000;
    m_OriginalHandlingData[3].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[3].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[3].uiMonetary = 35000;
    m_OriginalHandlingData[3].ucHeadLight = 0;
    m_OriginalHandlingData[3].ucTailLight = 1;
    m_OriginalHandlingData[3].ucAnimGroup = 2;

    m_OriginalHandlingData[4].iVehicleID = 4;
    m_OriginalHandlingData[4].fMass = 1200.000000f;
    m_OriginalHandlingData[4].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[4].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[4].vecCenterOfMass = CVector(0.000000f, 0.100000f, 0.000000f);
    m_OriginalHandlingData[4].uiPercentSubmerged = 70;
    m_OriginalHandlingData[4].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[4].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[4].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[4].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[4].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[4].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[4].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[4].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[4].fBrakeDeceleration = 4.000000f;
    m_OriginalHandlingData[4].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[4].bABS = false;
    m_OriginalHandlingData[4].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[4].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[4].fTractionBias = 0.480000f;
    m_OriginalHandlingData[4].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[4].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[4].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[4].fSuspensionUpperLimit = 0.370000f;
    m_OriginalHandlingData[4].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[4].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[4].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[4].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[4].uiModelFlags = 0x20;
    m_OriginalHandlingData[4].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[4].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[4].uiMonetary = 10000;
    m_OriginalHandlingData[4].ucHeadLight = 1;
    m_OriginalHandlingData[4].ucTailLight = 1;
    m_OriginalHandlingData[4].ucAnimGroup = 0;

    m_OriginalHandlingData[5].iVehicleID = 5;
    m_OriginalHandlingData[5].fMass = 1600.000000f;
    m_OriginalHandlingData[5].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[5].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[5].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[5].uiPercentSubmerged = 75;
    m_OriginalHandlingData[5].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[5].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[5].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[5].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[5].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[5].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[5].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[5].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[5].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[5].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[5].bABS = false;
    m_OriginalHandlingData[5].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[5].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[5].fTractionBias = 0.500000f;
    m_OriginalHandlingData[5].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[5].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[5].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[5].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[5].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[5].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[5].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[5].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[5].uiModelFlags = 0x0;
    m_OriginalHandlingData[5].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[5].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[5].uiMonetary = 35000;
    m_OriginalHandlingData[5].ucHeadLight = 0;
    m_OriginalHandlingData[5].ucTailLight = 1;
    m_OriginalHandlingData[5].ucAnimGroup = 0;

    m_OriginalHandlingData[6].iVehicleID = 6;
    m_OriginalHandlingData[6].fMass = 20000.000000f;
    m_OriginalHandlingData[6].fTurnMass = 200000.000000f;
    m_OriginalHandlingData[6].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[6].vecCenterOfMass = CVector(0.000000f, 0.500000f, -0.400000f);
    m_OriginalHandlingData[6].uiPercentSubmerged = 90;
    m_OriginalHandlingData[6].fTractionMultiplier = 0.780000f;
    m_OriginalHandlingData[6].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[6].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[6].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[6].Transmission.uiHandlingFlags = 20185601;
    m_OriginalHandlingData[6].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[6].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[6].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[6].fBrakeDeceleration = 3.170000f;
    m_OriginalHandlingData[6].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[6].bABS = false;
    m_OriginalHandlingData[6].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[6].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[6].fTractionBias = 0.550000f;
    m_OriginalHandlingData[6].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[6].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[6].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[6].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[6].fSuspensionLowerLimit = -0.300000f;
    m_OriginalHandlingData[6].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[6].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[6].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[6].uiModelFlags = 0xC008;
    m_OriginalHandlingData[6].uiHandlingFlags = 0x1340201;
    m_OriginalHandlingData[6].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[6].uiMonetary = 5000;
    m_OriginalHandlingData[6].ucHeadLight = 0;
    m_OriginalHandlingData[6].ucTailLight = 1;
    m_OriginalHandlingData[6].ucAnimGroup = 20;

    m_OriginalHandlingData[7].iVehicleID = 7;
    m_OriginalHandlingData[7].fMass = 6500.000000f;
    m_OriginalHandlingData[7].fTurnMass = 36670.800781f;
    m_OriginalHandlingData[7].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[7].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[7].uiPercentSubmerged = 90;
    m_OriginalHandlingData[7].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[7].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[7].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[7].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[7].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[7].Transmission.fEngineAcceleration = 10.800000f;
    m_OriginalHandlingData[7].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[7].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[7].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[7].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[7].bABS = false;
    m_OriginalHandlingData[7].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[7].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[7].fTractionBias = 0.500000f;
    m_OriginalHandlingData[7].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[7].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[7].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[7].fSuspensionUpperLimit = 0.470000f;
    m_OriginalHandlingData[7].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[7].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[7].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[7].fCollisionDamageMultiplier = 0.260000f;
    m_OriginalHandlingData[7].uiModelFlags = 0x4098;
    m_OriginalHandlingData[7].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[7].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[7].uiMonetary = 15000;
    m_OriginalHandlingData[7].ucHeadLight = 0;
    m_OriginalHandlingData[7].ucTailLight = 1;
    m_OriginalHandlingData[7].ucAnimGroup = 2;

    m_OriginalHandlingData[8].iVehicleID = 8;
    m_OriginalHandlingData[8].fMass = 5500.000000f;
    m_OriginalHandlingData[8].fTurnMass = 33187.898438f;
    m_OriginalHandlingData[8].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[8].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[8].uiPercentSubmerged = 90;
    m_OriginalHandlingData[8].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[8].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[8].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[8].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[8].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[8].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[8].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[8].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[8].fBrakeDeceleration = 3.500000f;
    m_OriginalHandlingData[8].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[8].bABS = false;
    m_OriginalHandlingData[8].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[8].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[8].fTractionBias = 0.500000f;
    m_OriginalHandlingData[8].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[8].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[8].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[8].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[8].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[8].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[8].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[8].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[8].uiModelFlags = 0x4008;
    m_OriginalHandlingData[8].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[8].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[8].uiMonetary = 5000;
    m_OriginalHandlingData[8].ucHeadLight = 0;
    m_OriginalHandlingData[8].ucTailLight = 1;
    m_OriginalHandlingData[8].ucAnimGroup = 0;

    m_OriginalHandlingData[9].iVehicleID = 9;
    m_OriginalHandlingData[9].fMass = 2200.000000f;
    m_OriginalHandlingData[9].fTurnMass = 10000.000000f;
    m_OriginalHandlingData[9].fDragCoeff = 1.800000f;
    m_OriginalHandlingData[9].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[9].uiPercentSubmerged = 75;
    m_OriginalHandlingData[9].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[9].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[9].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[9].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[9].Transmission.uiHandlingFlags = 272629761;
    m_OriginalHandlingData[9].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[9].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[9].Transmission.fMaxVelocity = 180.000000f;
    m_OriginalHandlingData[9].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[9].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[9].bABS = false;
    m_OriginalHandlingData[9].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[9].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[9].fTractionBias = 0.500000f;
    m_OriginalHandlingData[9].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[9].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[9].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[9].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[9].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[9].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[9].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[9].fCollisionDamageMultiplier = 0.720000f;
    m_OriginalHandlingData[9].uiModelFlags = 0x282000;
    m_OriginalHandlingData[9].uiHandlingFlags = 0x10400001;
    m_OriginalHandlingData[9].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[9].uiMonetary = 40000;
    m_OriginalHandlingData[9].ucHeadLight = 1;
    m_OriginalHandlingData[9].ucTailLight = 1;
    m_OriginalHandlingData[9].ucAnimGroup = 0;

    m_OriginalHandlingData[10].iVehicleID = 10;
    m_OriginalHandlingData[10].fMass = 1000.000000f;
    m_OriginalHandlingData[10].fTurnMass = 1400.000000f;
    m_OriginalHandlingData[10].fDragCoeff = 2.800000f;
    m_OriginalHandlingData[10].vecCenterOfMass = CVector(0.000000f, 0.200000f, 0.000000f);
    m_OriginalHandlingData[10].uiPercentSubmerged = 70;
    m_OriginalHandlingData[10].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[10].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[10].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[10].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[10].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[10].Transmission.fEngineAcceleration = 7.600000f;
    m_OriginalHandlingData[10].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[10].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[10].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[10].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[10].bABS = false;
    m_OriginalHandlingData[10].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[10].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[10].fTractionBias = 0.500000f;
    m_OriginalHandlingData[10].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[10].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[10].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[10].fSuspensionUpperLimit = 0.310000f;
    m_OriginalHandlingData[10].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[10].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[10].fSuspensionAntiDiveMultiplier = 0.200000f;
    m_OriginalHandlingData[10].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[10].uiModelFlags = 0x0;
    m_OriginalHandlingData[10].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[10].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[10].uiMonetary = 9000;
    m_OriginalHandlingData[10].ucHeadLight = 0;
    m_OriginalHandlingData[10].ucTailLight = 0;
    m_OriginalHandlingData[10].ucAnimGroup = 0;

    m_OriginalHandlingData[11].iVehicleID = 11;
    m_OriginalHandlingData[11].fMass = 1400.000000f;
    m_OriginalHandlingData[11].fTurnMass = 2725.300049f;
    m_OriginalHandlingData[11].fDragCoeff = 1.500000f;
    m_OriginalHandlingData[11].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.250000f);
    m_OriginalHandlingData[11].uiPercentSubmerged = 70;
    m_OriginalHandlingData[11].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[11].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[11].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[11].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[11].Transmission.uiHandlingFlags = 12599296;
    m_OriginalHandlingData[11].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[11].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[11].Transmission.fMaxVelocity = 240.000000f;
    m_OriginalHandlingData[11].fBrakeDeceleration = 11.000000f;
    m_OriginalHandlingData[11].fBrakeBias = 0.510000f;
    m_OriginalHandlingData[11].bABS = false;
    m_OriginalHandlingData[11].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[11].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[11].fTractionBias = 0.500000f;
    m_OriginalHandlingData[11].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[11].fSuspensionDamping = 0.190000f;
    m_OriginalHandlingData[11].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[11].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[11].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[11].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[11].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[11].fCollisionDamageMultiplier = 0.720000f;
    m_OriginalHandlingData[11].uiModelFlags = 0x40002004;
    m_OriginalHandlingData[11].uiHandlingFlags = 0xC04000;
    m_OriginalHandlingData[11].fSeatOffsetDistance = 0.370000f;
    m_OriginalHandlingData[11].uiMonetary = 95000;
    m_OriginalHandlingData[11].ucHeadLight = 1;
    m_OriginalHandlingData[11].ucTailLight = 1;
    m_OriginalHandlingData[11].ucAnimGroup = 1;

    m_OriginalHandlingData[12].iVehicleID = 12;
    m_OriginalHandlingData[12].fMass = 1800.000000f;
    m_OriginalHandlingData[12].fTurnMass = 4411.500000f;
    m_OriginalHandlingData[12].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[12].vecCenterOfMass = CVector(0.000000f, -0.100000f, -0.200000f);
    m_OriginalHandlingData[12].uiPercentSubmerged = 70;
    m_OriginalHandlingData[12].fTractionMultiplier = 0.950000f;
    m_OriginalHandlingData[12].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[12].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[12].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[12].Transmission.uiHandlingFlags = 37814280;
    m_OriginalHandlingData[12].Transmission.fEngineAcceleration = 9.200000f;
    m_OriginalHandlingData[12].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[12].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[12].fBrakeDeceleration = 6.500000f;
    m_OriginalHandlingData[12].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[12].bABS = false;
    m_OriginalHandlingData[12].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[12].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[12].fTractionBias = 0.450000f;
    m_OriginalHandlingData[12].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[12].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[12].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[12].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[12].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[12].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[12].fSuspensionAntiDiveMultiplier = 0.600000f;
    m_OriginalHandlingData[12].fCollisionDamageMultiplier = 0.410000f;
    m_OriginalHandlingData[12].uiModelFlags = 0x0;
    m_OriginalHandlingData[12].uiHandlingFlags = 0x2410008;
    m_OriginalHandlingData[12].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[12].uiMonetary = 30000;
    m_OriginalHandlingData[12].ucHeadLight = 1;
    m_OriginalHandlingData[12].ucTailLight = 1;
    m_OriginalHandlingData[12].ucAnimGroup = 0;

    m_OriginalHandlingData[13].iVehicleID = 13;
    m_OriginalHandlingData[13].fMass = 2600.000000f;
    m_OriginalHandlingData[13].fTurnMass = 8666.700195f;
    m_OriginalHandlingData[13].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[13].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.250000f);
    m_OriginalHandlingData[13].uiPercentSubmerged = 80;
    m_OriginalHandlingData[13].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[13].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[13].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[13].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[13].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[13].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[13].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[13].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[13].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[13].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[13].bABS = false;
    m_OriginalHandlingData[13].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[13].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[13].fTractionBias = 0.500000f;
    m_OriginalHandlingData[13].fSuspensionForceLevel = 2.600000f;
    m_OriginalHandlingData[13].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[13].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[13].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[13].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[13].fSuspensionFrontRearBias = 0.250000f;
    m_OriginalHandlingData[13].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[13].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[13].uiModelFlags = 0x4001;
    m_OriginalHandlingData[13].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[13].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[13].uiMonetary = 20000;
    m_OriginalHandlingData[13].ucHeadLight = 0;
    m_OriginalHandlingData[13].ucTailLight = 3;
    m_OriginalHandlingData[13].ucAnimGroup = 13;

    m_OriginalHandlingData[14].iVehicleID = 14;
    m_OriginalHandlingData[14].fMass = 3500.000000f;
    m_OriginalHandlingData[14].fTurnMass = 14000.000000f;
    m_OriginalHandlingData[14].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[14].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.100000f);
    m_OriginalHandlingData[14].uiPercentSubmerged = 80;
    m_OriginalHandlingData[14].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[14].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[14].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[14].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[14].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[14].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[14].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[14].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[14].fBrakeDeceleration = 4.500000f;
    m_OriginalHandlingData[14].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[14].bABS = false;
    m_OriginalHandlingData[14].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[14].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[14].fTractionBias = 0.460000f;
    m_OriginalHandlingData[14].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[14].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[14].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[14].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[14].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[14].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[14].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[14].fCollisionDamageMultiplier = 0.530000f;
    m_OriginalHandlingData[14].uiModelFlags = 0x4088;
    m_OriginalHandlingData[14].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[14].fSeatOffsetDistance = 0.460000f;
    m_OriginalHandlingData[14].uiMonetary = 22000;
    m_OriginalHandlingData[14].ucHeadLight = 0;
    m_OriginalHandlingData[14].ucTailLight = 3;
    m_OriginalHandlingData[14].ucAnimGroup = 0;

    m_OriginalHandlingData[15].iVehicleID = 15;
    m_OriginalHandlingData[15].fMass = 1200.000000f;
    m_OriginalHandlingData[15].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[15].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[15].vecCenterOfMass = CVector(0.000000f, -0.200000f, -0.200000f);
    m_OriginalHandlingData[15].uiPercentSubmerged = 70;
    m_OriginalHandlingData[15].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[15].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[15].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[15].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[15].Transmission.uiHandlingFlags = 2129920;
    m_OriginalHandlingData[15].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[15].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[15].Transmission.fMaxVelocity = 230.000000f;
    m_OriginalHandlingData[15].fBrakeDeceleration = 11.100000f;
    m_OriginalHandlingData[15].fBrakeBias = 0.480000f;
    m_OriginalHandlingData[15].bABS = false;
    m_OriginalHandlingData[15].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[15].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[15].fTractionBias = 0.500000f;
    m_OriginalHandlingData[15].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[15].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[15].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[15].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[15].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[15].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[15].fSuspensionAntiDiveMultiplier = 0.600000f;
    m_OriginalHandlingData[15].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[15].uiModelFlags = 0xC0002004;
    m_OriginalHandlingData[15].uiHandlingFlags = 0x208000;
    m_OriginalHandlingData[15].fSeatOffsetDistance = 0.400000f;
    m_OriginalHandlingData[15].uiMonetary = 105000;
    m_OriginalHandlingData[15].ucHeadLight = 0;
    m_OriginalHandlingData[15].ucTailLight = 0;
    m_OriginalHandlingData[15].ucAnimGroup = 1;

    m_OriginalHandlingData[16].iVehicleID = 16;
    m_OriginalHandlingData[16].fMass = 2600.000000f;
    m_OriginalHandlingData[16].fTurnMass = 10202.799805f;
    m_OriginalHandlingData[16].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[16].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[16].uiPercentSubmerged = 90;
    m_OriginalHandlingData[16].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[16].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[16].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[16].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[16].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[16].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[16].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[16].Transmission.fMaxVelocity = 155.000000f;
    m_OriginalHandlingData[16].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[16].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[16].bABS = false;
    m_OriginalHandlingData[16].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[16].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[16].fTractionBias = 0.470000f;
    m_OriginalHandlingData[16].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[16].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[16].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[16].fSuspensionUpperLimit = 0.400000f;
    m_OriginalHandlingData[16].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[16].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[16].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[16].fCollisionDamageMultiplier = 0.330000f;
    m_OriginalHandlingData[16].uiModelFlags = 0x4001;
    m_OriginalHandlingData[16].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[16].fSeatOffsetDistance = 0.580000f;
    m_OriginalHandlingData[16].uiMonetary = 10000;
    m_OriginalHandlingData[16].ucHeadLight = 0;
    m_OriginalHandlingData[16].ucTailLight = 1;
    m_OriginalHandlingData[16].ucAnimGroup = 13;

    m_OriginalHandlingData[17].iVehicleID = 17;
    m_OriginalHandlingData[17].fMass = 2000.000000f;
    m_OriginalHandlingData[17].fTurnMass = 5848.299805f;
    m_OriginalHandlingData[17].fDragCoeff = 2.800000f;
    m_OriginalHandlingData[17].vecCenterOfMass = CVector(0.000000f, 0.200000f, -0.100000f);
    m_OriginalHandlingData[17].uiPercentSubmerged = 85;
    m_OriginalHandlingData[17].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[17].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[17].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[17].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[17].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[17].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[17].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[17].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[17].fBrakeDeceleration = 5.500000f;
    m_OriginalHandlingData[17].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[17].bABS = false;
    m_OriginalHandlingData[17].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[17].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[17].fTractionBias = 0.500000f;
    m_OriginalHandlingData[17].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[17].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[17].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[17].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[17].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[17].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[17].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[17].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[17].uiModelFlags = 0x20;
    m_OriginalHandlingData[17].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[17].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[17].uiMonetary = 16000;
    m_OriginalHandlingData[17].ucHeadLight = 1;
    m_OriginalHandlingData[17].ucTailLight = 3;
    m_OriginalHandlingData[17].ucAnimGroup = 0;

    m_OriginalHandlingData[18].iVehicleID = 18;
    m_OriginalHandlingData[18].fMass = 1800.000000f;
    m_OriginalHandlingData[18].fTurnMass = 4350.000000f;
    m_OriginalHandlingData[18].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[18].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[18].uiPercentSubmerged = 70;
    m_OriginalHandlingData[18].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[18].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[18].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[18].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[18].Transmission.uiHandlingFlags = 268435456;
    m_OriginalHandlingData[18].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[18].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[18].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[18].fBrakeDeceleration = 4.000000f;
    m_OriginalHandlingData[18].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[18].bABS = false;
    m_OriginalHandlingData[18].fSteeringLock = 28.000000f;
    m_OriginalHandlingData[18].fTractionLoss = 0.880000f;
    m_OriginalHandlingData[18].fTractionBias = 0.520000f;
    m_OriginalHandlingData[18].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[18].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[18].fSuspensionHighSpdDamping = 1.000000f;
    m_OriginalHandlingData[18].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[18].fSuspensionLowerLimit = -0.180000f;
    m_OriginalHandlingData[18].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[18].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[18].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[18].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[18].uiHandlingFlags = 0x10000000;
    m_OriginalHandlingData[18].fSeatOffsetDistance = 0.360000f;
    m_OriginalHandlingData[18].uiMonetary = 19000;
    m_OriginalHandlingData[18].ucHeadLight = 0;
    m_OriginalHandlingData[18].ucTailLight = 3;
    m_OriginalHandlingData[18].ucAnimGroup = 0;

    m_OriginalHandlingData[19].iVehicleID = 19;
    m_OriginalHandlingData[19].fMass = 1450.000000f;
    m_OriginalHandlingData[19].fTurnMass = 4056.399902f;
    m_OriginalHandlingData[19].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[19].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.250000f);
    m_OriginalHandlingData[19].uiPercentSubmerged = 75;
    m_OriginalHandlingData[19].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[19].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[19].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[19].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[19].Transmission.uiHandlingFlags = 2097152;
    m_OriginalHandlingData[19].Transmission.fEngineAcceleration = 7.600000f;
    m_OriginalHandlingData[19].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[19].Transmission.fMaxVelocity = 180.000000f;
    m_OriginalHandlingData[19].fBrakeDeceleration = 9.100000f;
    m_OriginalHandlingData[19].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[19].bABS = false;
    m_OriginalHandlingData[19].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[19].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[19].fTractionBias = 0.450000f;
    m_OriginalHandlingData[19].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[19].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[19].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[19].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[19].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[19].fSuspensionFrontRearBias = 0.540000f;
    m_OriginalHandlingData[19].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[19].fCollisionDamageMultiplier = 0.510000f;
    m_OriginalHandlingData[19].uiModelFlags = 0x0;
    m_OriginalHandlingData[19].uiHandlingFlags = 0x200000;
    m_OriginalHandlingData[19].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[19].uiMonetary = 20000;
    m_OriginalHandlingData[19].ucHeadLight = 0;
    m_OriginalHandlingData[19].ucTailLight = 1;
    m_OriginalHandlingData[19].ucAnimGroup = 0;

    m_OriginalHandlingData[20].iVehicleID = 20;
    m_OriginalHandlingData[20].fMass = 1850.000000f;
    m_OriginalHandlingData[20].fTurnMass = 5000.000000f;
    m_OriginalHandlingData[20].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[20].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[20].uiPercentSubmerged = 75;
    m_OriginalHandlingData[20].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[20].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[20].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[20].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[20].Transmission.uiHandlingFlags = 272629760;
    m_OriginalHandlingData[20].Transmission.fEngineAcceleration = 8.400000f;
    m_OriginalHandlingData[20].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[20].Transmission.fMaxVelocity = 180.000000f;
    m_OriginalHandlingData[20].fBrakeDeceleration = 7.500000f;
    m_OriginalHandlingData[20].fBrakeBias = 0.650000f;
    m_OriginalHandlingData[20].bABS = false;
    m_OriginalHandlingData[20].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[20].fTractionLoss = 0.650000f;
    m_OriginalHandlingData[20].fTractionBias = 0.520000f;
    m_OriginalHandlingData[20].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[20].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[20].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[20].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[20].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[20].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[20].fSuspensionAntiDiveMultiplier = 0.350000f;
    m_OriginalHandlingData[20].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[20].uiModelFlags = 0x0;
    m_OriginalHandlingData[20].uiHandlingFlags = 0x10400000;
    m_OriginalHandlingData[20].fSeatOffsetDistance = 0.240000f;
    m_OriginalHandlingData[20].uiMonetary = 18000;
    m_OriginalHandlingData[20].ucHeadLight = 1;
    m_OriginalHandlingData[20].ucTailLight = 1;
    m_OriginalHandlingData[20].ucAnimGroup = 0;

    m_OriginalHandlingData[21].iVehicleID = 21;
    m_OriginalHandlingData[21].fMass = 1700.000000f;
    m_OriginalHandlingData[21].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[21].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[21].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.200000f);
    m_OriginalHandlingData[21].uiPercentSubmerged = 75;
    m_OriginalHandlingData[21].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[21].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[21].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[21].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[21].Transmission.uiHandlingFlags = 1064964;
    m_OriginalHandlingData[21].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[21].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[21].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[21].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[21].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[21].bABS = false;
    m_OriginalHandlingData[21].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[21].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[21].fTractionBias = 0.570000f;
    m_OriginalHandlingData[21].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[21].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[21].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[21].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[21].fSuspensionLowerLimit = -0.180000f;
    m_OriginalHandlingData[21].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[21].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[21].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[21].uiModelFlags = 0x40;
    m_OriginalHandlingData[21].uiHandlingFlags = 0x104004;
    m_OriginalHandlingData[21].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[21].uiMonetary = 26000;
    m_OriginalHandlingData[21].ucHeadLight = 0;
    m_OriginalHandlingData[21].ucTailLight = 1;
    m_OriginalHandlingData[21].ucAnimGroup = 0;

    m_OriginalHandlingData[22].iVehicleID = 22;
    m_OriginalHandlingData[22].fMass = 1700.000000f;
    m_OriginalHandlingData[22].fTurnMass = 4108.299805f;
    m_OriginalHandlingData[22].fDragCoeff = 3.500000f;
    m_OriginalHandlingData[22].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[22].uiPercentSubmerged = 85;
    m_OriginalHandlingData[22].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[22].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[22].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[22].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[22].Transmission.uiHandlingFlags = 2;
    m_OriginalHandlingData[22].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[22].Transmission.fEngineInertia = 50.000000f;
    m_OriginalHandlingData[22].Transmission.fMaxVelocity = 145.000000f;
    m_OriginalHandlingData[22].fBrakeDeceleration = 4.170000f;
    m_OriginalHandlingData[22].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[22].bABS = false;
    m_OriginalHandlingData[22].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[22].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[22].fTractionBias = 0.500000f;
    m_OriginalHandlingData[22].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[22].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[22].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[22].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[22].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[22].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[22].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[22].fCollisionDamageMultiplier = 0.770000f;
    m_OriginalHandlingData[22].uiModelFlags = 0x88;
    m_OriginalHandlingData[22].uiHandlingFlags = 0x2;
    m_OriginalHandlingData[22].fSeatOffsetDistance = 0.240000f;
    m_OriginalHandlingData[22].uiMonetary = 29000;
    m_OriginalHandlingData[22].ucHeadLight = 0;
    m_OriginalHandlingData[22].ucTailLight = 1;
    m_OriginalHandlingData[22].ucAnimGroup = 0;

    m_OriginalHandlingData[23].iVehicleID = 23;
    m_OriginalHandlingData[23].fMass = 1200.000000f;
    m_OriginalHandlingData[23].fTurnMass = 2000.000000f;
    m_OriginalHandlingData[23].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[23].vecCenterOfMass = CVector(0.000000f, -0.100000f, -0.100000f);
    m_OriginalHandlingData[23].uiPercentSubmerged = 80;
    m_OriginalHandlingData[23].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[23].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[23].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[23].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[23].Transmission.uiHandlingFlags = 3179008;
    m_OriginalHandlingData[23].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[23].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[23].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[23].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[23].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[23].bABS = false;
    m_OriginalHandlingData[23].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[23].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[23].fTractionBias = 0.500000f;
    m_OriginalHandlingData[23].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[23].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[23].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[23].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[23].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[23].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[23].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[23].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[23].uiModelFlags = 0x201904;
    m_OriginalHandlingData[23].uiHandlingFlags = 0x308200;
    m_OriginalHandlingData[23].fSeatOffsetDistance = 0.380000f;
    m_OriginalHandlingData[23].uiMonetary = 15000;
    m_OriginalHandlingData[23].ucHeadLight = 1;
    m_OriginalHandlingData[23].ucTailLight = 2;
    m_OriginalHandlingData[23].ucAnimGroup = 25;

    m_OriginalHandlingData[24].iVehicleID = 24;
    m_OriginalHandlingData[24].fMass = 1600.000000f;
    m_OriginalHandlingData[24].fTurnMass = 3921.300049f;
    m_OriginalHandlingData[24].fDragCoeff = 1.800000f;
    m_OriginalHandlingData[24].vecCenterOfMass = CVector(0.000000f, -0.400000f, 0.000000f);
    m_OriginalHandlingData[24].uiPercentSubmerged = 75;
    m_OriginalHandlingData[24].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[24].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[24].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[24].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[24].Transmission.uiHandlingFlags = 270532616;
    m_OriginalHandlingData[24].Transmission.fEngineAcceleration = 8.800000f;
    m_OriginalHandlingData[24].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[24].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[24].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[24].fBrakeBias = 0.530000f;
    m_OriginalHandlingData[24].bABS = false;
    m_OriginalHandlingData[24].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[24].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[24].fTractionBias = 0.520000f;
    m_OriginalHandlingData[24].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[24].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[24].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[24].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[24].fSuspensionLowerLimit = -0.120000f;
    m_OriginalHandlingData[24].fSuspensionFrontRearBias = 0.380000f;
    m_OriginalHandlingData[24].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[24].fCollisionDamageMultiplier = 0.240000f;
    m_OriginalHandlingData[24].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[24].uiHandlingFlags = 0x10200008;
    m_OriginalHandlingData[24].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[24].uiMonetary = 25000;
    m_OriginalHandlingData[24].ucHeadLight = 0;
    m_OriginalHandlingData[24].ucTailLight = 1;
    m_OriginalHandlingData[24].ucAnimGroup = 0;

    m_OriginalHandlingData[25].iVehicleID = 25;
    m_OriginalHandlingData[25].fMass = 4000.000000f;
    m_OriginalHandlingData[25].fTurnMass = 17333.300781f;
    m_OriginalHandlingData[25].fDragCoeff = 1.800000f;
    m_OriginalHandlingData[25].vecCenterOfMass = CVector(0.000000f, 0.100000f, 0.000000f);
    m_OriginalHandlingData[25].uiPercentSubmerged = 85;
    m_OriginalHandlingData[25].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[25].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[25].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[25].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[25].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[25].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[25].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[25].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[25].fBrakeDeceleration = 5.400000f;
    m_OriginalHandlingData[25].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[25].bABS = false;
    m_OriginalHandlingData[25].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[25].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[25].fTractionBias = 0.480000f;
    m_OriginalHandlingData[25].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[25].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[25].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[25].fSuspensionUpperLimit = 0.400000f;
    m_OriginalHandlingData[25].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[25].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[25].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[25].fCollisionDamageMultiplier = 0.160000f;
    m_OriginalHandlingData[25].uiModelFlags = 0x4011;
    m_OriginalHandlingData[25].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[25].fSeatOffsetDistance = 0.320000f;
    m_OriginalHandlingData[25].uiMonetary = 40000;
    m_OriginalHandlingData[25].ucHeadLight = 0;
    m_OriginalHandlingData[25].ucTailLight = 1;
    m_OriginalHandlingData[25].ucAnimGroup = 13;

    m_OriginalHandlingData[26].iVehicleID = 26;
    m_OriginalHandlingData[26].fMass = 7000.000000f;
    m_OriginalHandlingData[26].fTurnMass = 30916.699219f;
    m_OriginalHandlingData[26].fDragCoeff = 1.500000f;
    m_OriginalHandlingData[26].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[26].uiPercentSubmerged = 90;
    m_OriginalHandlingData[26].fTractionMultiplier = 0.500000f;
    m_OriginalHandlingData[26].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[26].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[26].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[26].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[26].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[26].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[26].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[26].fBrakeDeceleration = 8.400000f;
    m_OriginalHandlingData[26].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[26].bABS = false;
    m_OriginalHandlingData[26].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[26].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[26].fTractionBias = 0.460000f;
    m_OriginalHandlingData[26].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[26].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[26].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[26].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[26].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[26].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[26].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[26].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[26].uiModelFlags = 0x4001;
    m_OriginalHandlingData[26].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[26].fSeatOffsetDistance = 0.270000f;
    m_OriginalHandlingData[26].uiMonetary = 40000;
    m_OriginalHandlingData[26].ucHeadLight = 1;
    m_OriginalHandlingData[26].ucTailLight = 1;
    m_OriginalHandlingData[26].ucAnimGroup = 13;

    m_OriginalHandlingData[27].iVehicleID = 27;
    m_OriginalHandlingData[27].fMass = 1400.000000f;
    m_OriginalHandlingData[27].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[27].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[27].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[27].uiPercentSubmerged = 70;
    m_OriginalHandlingData[27].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[27].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[27].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[27].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[27].Transmission.uiHandlingFlags = 2097152;
    m_OriginalHandlingData[27].Transmission.fEngineAcceleration = 13.200000f;
    m_OriginalHandlingData[27].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[27].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[27].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[27].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[27].bABS = false;
    m_OriginalHandlingData[27].fSteeringLock = 34.000000f;
    m_OriginalHandlingData[27].fTractionLoss = 0.890000f;
    m_OriginalHandlingData[27].fTractionBias = 0.500000f;
    m_OriginalHandlingData[27].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[27].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[27].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[27].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[27].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[27].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[27].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[27].fCollisionDamageMultiplier = 0.490000f;
    m_OriginalHandlingData[27].uiModelFlags = 0x2004;
    m_OriginalHandlingData[27].uiHandlingFlags = 0x200000;
    m_OriginalHandlingData[27].fSeatOffsetDistance = 0.150000f;
    m_OriginalHandlingData[27].uiMonetary = 45000;
    m_OriginalHandlingData[27].ucHeadLight = 1;
    m_OriginalHandlingData[27].ucTailLight = 1;
    m_OriginalHandlingData[27].ucAnimGroup = 1;

    m_OriginalHandlingData[28].iVehicleID = 28;
    m_OriginalHandlingData[28].fMass = 5500.000000f;
    m_OriginalHandlingData[28].fTurnMass = 33187.898438f;
    m_OriginalHandlingData[28].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[28].vecCenterOfMass = CVector(0.000000f, 0.500000f, 0.000000f);
    m_OriginalHandlingData[28].uiPercentSubmerged = 90;
    m_OriginalHandlingData[28].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[28].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[28].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[28].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[28].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[28].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[28].Transmission.fEngineInertia = 50.000000f;
    m_OriginalHandlingData[28].Transmission.fMaxVelocity = 130.000000f;
    m_OriginalHandlingData[28].fBrakeDeceleration = 4.170000f;
    m_OriginalHandlingData[28].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[28].bABS = false;
    m_OriginalHandlingData[28].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[28].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[28].fTractionBias = 0.400000f;
    m_OriginalHandlingData[28].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[28].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[28].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[28].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[28].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[28].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[28].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[28].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[28].uiModelFlags = 0x4002;
    m_OriginalHandlingData[28].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[28].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[28].uiMonetary = 15000;
    m_OriginalHandlingData[28].ucHeadLight = 1;
    m_OriginalHandlingData[28].ucTailLight = 1;
    m_OriginalHandlingData[28].ucAnimGroup = 16;

    m_OriginalHandlingData[29].iVehicleID = 29;
    m_OriginalHandlingData[29].fMass = 25000.000000f;
    m_OriginalHandlingData[29].fTurnMass = 250000.000000f;
    m_OriginalHandlingData[29].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[29].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[29].uiPercentSubmerged = 90;
    m_OriginalHandlingData[29].fTractionMultiplier = 2.500000f;
    m_OriginalHandlingData[29].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[29].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[29].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[29].Transmission.uiHandlingFlags = 3180608;
    m_OriginalHandlingData[29].Transmission.fEngineAcceleration = 16.000000f;
    m_OriginalHandlingData[29].Transmission.fEngineInertia = 150.000000f;
    m_OriginalHandlingData[29].Transmission.fMaxVelocity = 80.000000f;
    m_OriginalHandlingData[29].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[29].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[29].bABS = false;
    m_OriginalHandlingData[29].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[29].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[29].fTractionBias = 0.500000f;
    m_OriginalHandlingData[29].fSuspensionForceLevel = 0.400000f;
    m_OriginalHandlingData[29].fSuspensionDamping = 0.020000f;
    m_OriginalHandlingData[29].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[29].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[29].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[29].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[29].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[29].fCollisionDamageMultiplier = 0.090000f;
    m_OriginalHandlingData[29].uiModelFlags = 0x40008008;
    m_OriginalHandlingData[29].uiHandlingFlags = 0x308840;
    m_OriginalHandlingData[29].fSeatOffsetDistance = 0.220000f;
    m_OriginalHandlingData[29].uiMonetary = 110000;
    m_OriginalHandlingData[29].ucHeadLight = 0;
    m_OriginalHandlingData[29].ucTailLight = 1;
    m_OriginalHandlingData[29].ucAnimGroup = 24;

    m_OriginalHandlingData[30].iVehicleID = 30;
    m_OriginalHandlingData[30].fMass = 10500.000000f;
    m_OriginalHandlingData[30].fTurnMass = 61407.500000f;
    m_OriginalHandlingData[30].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[30].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[30].uiPercentSubmerged = 90;
    m_OriginalHandlingData[30].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[30].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[30].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[30].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[30].Transmission.uiHandlingFlags = 1048576;
    m_OriginalHandlingData[30].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[30].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[30].Transmission.fMaxVelocity = 180.000000f;
    m_OriginalHandlingData[30].fBrakeDeceleration = 4.000000f;
    m_OriginalHandlingData[30].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[30].bABS = false;
    m_OriginalHandlingData[30].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[30].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[30].fTractionBias = 0.470000f;
    m_OriginalHandlingData[30].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[30].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[30].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[30].fSuspensionUpperLimit = 0.470000f;
    m_OriginalHandlingData[30].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[30].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[30].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[30].fCollisionDamageMultiplier = 0.260000f;
    m_OriginalHandlingData[30].uiModelFlags = 0x200809;
    m_OriginalHandlingData[30].uiHandlingFlags = 0x100000;
    m_OriginalHandlingData[30].fSeatOffsetDistance = 0.620000f;
    m_OriginalHandlingData[30].uiMonetary = 10000;
    m_OriginalHandlingData[30].ucHeadLight = 0;
    m_OriginalHandlingData[30].ucTailLight = 1;
    m_OriginalHandlingData[30].ucAnimGroup = 2;

    m_OriginalHandlingData[31].iVehicleID = 31;
    m_OriginalHandlingData[31].fMass = 1400.000000f;
    m_OriginalHandlingData[31].fTurnMass = 3400.000000f;
    m_OriginalHandlingData[31].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[31].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.300000f);
    m_OriginalHandlingData[31].uiPercentSubmerged = 75;
    m_OriginalHandlingData[31].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[31].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[31].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[31].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[31].Transmission.uiHandlingFlags = 32768;
    m_OriginalHandlingData[31].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[31].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[31].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[31].fBrakeDeceleration = 11.000000f;
    m_OriginalHandlingData[31].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[31].bABS = false;
    m_OriginalHandlingData[31].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[31].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[31].fTractionBias = 0.500000f;
    m_OriginalHandlingData[31].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[31].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[31].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[31].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[31].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[31].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[31].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[31].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[31].uiModelFlags = 0x40006800;
    m_OriginalHandlingData[31].uiHandlingFlags = 0x8000;
    m_OriginalHandlingData[31].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[31].uiMonetary = 35000;
    m_OriginalHandlingData[31].ucHeadLight = 1;
    m_OriginalHandlingData[31].ucTailLight = 1;
    m_OriginalHandlingData[31].ucAnimGroup = 0;

    m_OriginalHandlingData[32].iVehicleID = 32;
    m_OriginalHandlingData[32].fMass = 3800.000000f;
    m_OriginalHandlingData[32].fTurnMass = 30000.000000f;
    m_OriginalHandlingData[32].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[32].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.500000f);
    m_OriginalHandlingData[32].uiPercentSubmerged = 90;
    m_OriginalHandlingData[32].fTractionMultiplier = 0.450000f;
    m_OriginalHandlingData[32].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[32].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[32].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[32].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[32].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[32].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[32].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[32].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[32].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[32].bABS = false;
    m_OriginalHandlingData[32].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[32].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[32].fTractionBias = 0.500000f;
    m_OriginalHandlingData[32].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[32].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[32].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[32].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[32].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[32].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[32].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[32].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[32].uiModelFlags = 0x20002000;
    m_OriginalHandlingData[32].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[32].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[32].uiMonetary = 35000;
    m_OriginalHandlingData[32].ucHeadLight = 0;
    m_OriginalHandlingData[32].ucTailLight = 1;
    m_OriginalHandlingData[32].ucAnimGroup = 0;

    m_OriginalHandlingData[33].iVehicleID = 33;
    m_OriginalHandlingData[33].fMass = 1400.000000f;
    m_OriginalHandlingData[33].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[33].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[33].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.100000f);
    m_OriginalHandlingData[33].uiPercentSubmerged = 70;
    m_OriginalHandlingData[33].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[33].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[33].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[33].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[33].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[33].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[33].Transmission.fEngineInertia = 7.000000f;
    m_OriginalHandlingData[33].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[33].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[33].fBrakeBias = 0.650000f;
    m_OriginalHandlingData[33].bABS = false;
    m_OriginalHandlingData[33].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[33].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[33].fTractionBias = 0.450000f;
    m_OriginalHandlingData[33].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[33].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[33].fSuspensionHighSpdDamping = 2.000000f;
    m_OriginalHandlingData[33].fSuspensionUpperLimit = 0.310000f;
    m_OriginalHandlingData[33].fSuspensionLowerLimit = -0.180000f;
    m_OriginalHandlingData[33].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[33].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[33].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[33].uiModelFlags = 0x0;
    m_OriginalHandlingData[33].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[33].fSeatOffsetDistance = 0.210000f;
    m_OriginalHandlingData[33].uiMonetary = 9000;
    m_OriginalHandlingData[33].ucHeadLight = 0;
    m_OriginalHandlingData[33].ucTailLight = 0;
    m_OriginalHandlingData[33].ucAnimGroup = 0;

    m_OriginalHandlingData[34].iVehicleID = 34;
    m_OriginalHandlingData[34].fMass = 9500.000000f;
    m_OriginalHandlingData[34].fTurnMass = 57324.601563f;
    m_OriginalHandlingData[34].fDragCoeff = 1.800000f;
    m_OriginalHandlingData[34].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[34].uiPercentSubmerged = 90;
    m_OriginalHandlingData[34].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[34].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[34].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[34].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[34].Transmission.uiHandlingFlags = 2097152;
    m_OriginalHandlingData[34].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[34].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[34].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[34].fBrakeDeceleration = 5.700000f;
    m_OriginalHandlingData[34].fBrakeBias = 0.350000f;
    m_OriginalHandlingData[34].bABS = false;
    m_OriginalHandlingData[34].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[34].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[34].fTractionBias = 0.350000f;
    m_OriginalHandlingData[34].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[34].fSuspensionDamping = 0.040000f;
    m_OriginalHandlingData[34].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[34].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[34].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[34].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[34].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[34].fCollisionDamageMultiplier = 0.480000f;
    m_OriginalHandlingData[34].uiModelFlags = 0x2;
    m_OriginalHandlingData[34].uiHandlingFlags = 0x200000;
    m_OriginalHandlingData[34].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[34].uiMonetary = 20000;
    m_OriginalHandlingData[34].ucHeadLight = 0;
    m_OriginalHandlingData[34].ucTailLight = 1;
    m_OriginalHandlingData[34].ucAnimGroup = 15;

    m_OriginalHandlingData[35].iVehicleID = 35;
    m_OriginalHandlingData[35].fMass = 1750.000000f;
    m_OriginalHandlingData[35].fTurnMass = 4351.700195f;
    m_OriginalHandlingData[35].fDragCoeff = 2.900000f;
    m_OriginalHandlingData[35].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.150000f);
    m_OriginalHandlingData[35].uiPercentSubmerged = 75;
    m_OriginalHandlingData[35].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[35].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[35].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[35].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[35].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[35].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[35].Transmission.fEngineInertia = 6.000000f;
    m_OriginalHandlingData[35].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[35].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[35].fBrakeBias = 0.440000f;
    m_OriginalHandlingData[35].bABS = false;
    m_OriginalHandlingData[35].fSteeringLock = 40.000000f;
    m_OriginalHandlingData[35].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[35].fTractionBias = 0.510000f;
    m_OriginalHandlingData[35].fSuspensionForceLevel = 0.700000f;
    m_OriginalHandlingData[35].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[35].fSuspensionHighSpdDamping = 2.000000f;
    m_OriginalHandlingData[35].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[35].fSuspensionLowerLimit = -0.300000f;
    m_OriginalHandlingData[35].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[35].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[35].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[35].uiModelFlags = 0x0;
    m_OriginalHandlingData[35].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[35].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[35].uiMonetary = 10000;
    m_OriginalHandlingData[35].ucHeadLight = 1;
    m_OriginalHandlingData[35].ucTailLight = 1;
    m_OriginalHandlingData[35].ucAnimGroup = 0;

    m_OriginalHandlingData[36].iVehicleID = 36;
    m_OriginalHandlingData[36].fMass = 1600.000000f;
    m_OriginalHandlingData[36].fTurnMass = 3921.300049f;
    m_OriginalHandlingData[36].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[36].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.150000f);
    m_OriginalHandlingData[36].uiPercentSubmerged = 70;
    m_OriginalHandlingData[36].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[36].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[36].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[36].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[36].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[36].Transmission.fEngineAcceleration = 9.200000f;
    m_OriginalHandlingData[36].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[36].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[36].fBrakeDeceleration = 8.170000f;
    m_OriginalHandlingData[36].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[36].bABS = false;
    m_OriginalHandlingData[36].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[36].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[36].fTractionBias = 0.550000f;
    m_OriginalHandlingData[36].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[36].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[36].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[36].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[36].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[36].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[36].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[36].fCollisionDamageMultiplier = 0.640000f;
    m_OriginalHandlingData[36].uiModelFlags = 0x2800;
    m_OriginalHandlingData[36].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[36].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[36].uiMonetary = 19000;
    m_OriginalHandlingData[36].ucHeadLight = 1;
    m_OriginalHandlingData[36].ucTailLight = 1;
    m_OriginalHandlingData[36].ucAnimGroup = 0;

    m_OriginalHandlingData[37].iVehicleID = 37;
    m_OriginalHandlingData[37].fMass = 2000.000000f;
    m_OriginalHandlingData[37].fTurnMass = 4901.700195f;
    m_OriginalHandlingData[37].fDragCoeff = 2.400000f;
    m_OriginalHandlingData[37].vecCenterOfMass = CVector(0.000000f, 0.400000f, -0.100000f);
    m_OriginalHandlingData[37].uiPercentSubmerged = 85;
    m_OriginalHandlingData[37].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[37].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[37].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[37].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[37].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[37].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[37].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[37].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[37].fBrakeDeceleration = 5.500000f;
    m_OriginalHandlingData[37].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[37].bABS = false;
    m_OriginalHandlingData[37].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[37].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[37].fTractionBias = 0.520000f;
    m_OriginalHandlingData[37].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[37].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[37].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[37].fSuspensionUpperLimit = 0.430000f;
    m_OriginalHandlingData[37].fSuspensionLowerLimit = -0.110000f;
    m_OriginalHandlingData[37].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[37].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[37].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[37].uiModelFlags = 0x1;
    m_OriginalHandlingData[37].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[37].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[37].uiMonetary = 26000;
    m_OriginalHandlingData[37].ucHeadLight = 0;
    m_OriginalHandlingData[37].ucTailLight = 1;
    m_OriginalHandlingData[37].ucAnimGroup = 13;

    m_OriginalHandlingData[38].iVehicleID = 38;
    m_OriginalHandlingData[38].fMass = 100.000000f;
    m_OriginalHandlingData[38].fTurnMass = 24.100000f;
    m_OriginalHandlingData[38].fDragCoeff = 6.000000f;
    m_OriginalHandlingData[38].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.100000f);
    m_OriginalHandlingData[38].uiPercentSubmerged = 70;
    m_OriginalHandlingData[38].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[38].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[38].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[38].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[38].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[38].Transmission.fEngineAcceleration = 14.000000f;
    m_OriginalHandlingData[38].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[38].Transmission.fMaxVelocity = 75.000000f;
    m_OriginalHandlingData[38].fBrakeDeceleration = 5.500000f;
    m_OriginalHandlingData[38].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[38].bABS = false;
    m_OriginalHandlingData[38].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[38].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[38].fTractionBias = 0.490000f;
    m_OriginalHandlingData[38].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[38].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[38].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[38].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[38].fSuspensionLowerLimit = -0.080000f;
    m_OriginalHandlingData[38].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[38].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[38].fCollisionDamageMultiplier = 0.050000f;
    m_OriginalHandlingData[38].uiModelFlags = 0x440000;
    m_OriginalHandlingData[38].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[38].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[38].uiMonetary = 500;
    m_OriginalHandlingData[38].ucHeadLight = 0;
    m_OriginalHandlingData[38].ucTailLight = 1;
    m_OriginalHandlingData[38].ucAnimGroup = 0;

    m_OriginalHandlingData[39].iVehicleID = 39;
    m_OriginalHandlingData[39].fMass = 2500.000000f;
    m_OriginalHandlingData[39].fTurnMass = 5960.399902f;
    m_OriginalHandlingData[39].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[39].vecCenterOfMass = CVector(0.000000f, -0.800000f, 0.200000f);
    m_OriginalHandlingData[39].uiPercentSubmerged = 70;
    m_OriginalHandlingData[39].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[39].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[39].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[39].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[39].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[39].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[39].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[39].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[39].fBrakeDeceleration = 4.000000f;
    m_OriginalHandlingData[39].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[39].bABS = false;
    m_OriginalHandlingData[39].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[39].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[39].fTractionBias = 0.500000f;
    m_OriginalHandlingData[39].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[39].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[39].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[39].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[39].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[39].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[39].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[39].fCollisionDamageMultiplier = 1.250000f;
    m_OriginalHandlingData[39].uiModelFlags = 0x40000020;
    m_OriginalHandlingData[39].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[39].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[39].uiMonetary = 10000;
    m_OriginalHandlingData[39].ucHeadLight = 0;
    m_OriginalHandlingData[39].ucTailLight = 1;
    m_OriginalHandlingData[39].ucAnimGroup = 0;

    m_OriginalHandlingData[40].iVehicleID = 40;
    m_OriginalHandlingData[40].fMass = 8000.000000f;
    m_OriginalHandlingData[40].fTurnMass = 48273.300781f;
    m_OriginalHandlingData[40].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[40].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[40].uiPercentSubmerged = 90;
    m_OriginalHandlingData[40].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[40].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[40].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[40].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[40].Transmission.uiHandlingFlags = 4456448;
    m_OriginalHandlingData[40].Transmission.fEngineAcceleration = 5.200000f;
    m_OriginalHandlingData[40].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[40].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[40].fBrakeDeceleration = 5.700000f;
    m_OriginalHandlingData[40].fBrakeBias = 0.350000f;
    m_OriginalHandlingData[40].bABS = false;
    m_OriginalHandlingData[40].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[40].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[40].fTractionBias = 0.350000f;
    m_OriginalHandlingData[40].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[40].fSuspensionDamping = 0.040000f;
    m_OriginalHandlingData[40].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[40].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[40].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[40].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[40].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[40].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[40].uiModelFlags = 0x4000;
    m_OriginalHandlingData[40].uiHandlingFlags = 0x440000;
    m_OriginalHandlingData[40].fSeatOffsetDistance = 0.560000f;
    m_OriginalHandlingData[40].uiMonetary = 20000;
    m_OriginalHandlingData[40].ucHeadLight = 0;
    m_OriginalHandlingData[40].ucTailLight = 1;
    m_OriginalHandlingData[40].ucAnimGroup = 2;

    m_OriginalHandlingData[41].iVehicleID = 41;
    m_OriginalHandlingData[41].fMass = 5000.000000f;
    m_OriginalHandlingData[41].fTurnMass = 20000.000000f;
    m_OriginalHandlingData[41].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[41].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.350000f);
    m_OriginalHandlingData[41].uiPercentSubmerged = 80;
    m_OriginalHandlingData[41].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[41].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[41].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[41].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[41].Transmission.uiHandlingFlags = 19923013;
    m_OriginalHandlingData[41].Transmission.fEngineAcceleration = 18.000000f;
    m_OriginalHandlingData[41].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[41].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[41].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[41].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[41].bABS = false;
    m_OriginalHandlingData[41].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[41].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[41].fTractionBias = 0.550000f;
    m_OriginalHandlingData[41].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[41].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[41].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[41].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[41].fSuspensionLowerLimit = -0.300000f;
    m_OriginalHandlingData[41].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[41].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[41].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[41].uiModelFlags = 0x28;
    m_OriginalHandlingData[41].uiHandlingFlags = 0x1300045;
    m_OriginalHandlingData[41].fSeatOffsetDistance = 0.440000f;
    m_OriginalHandlingData[41].uiMonetary = 40000;
    m_OriginalHandlingData[41].ucHeadLight = 0;
    m_OriginalHandlingData[41].ucTailLight = 1;
    m_OriginalHandlingData[41].ucAnimGroup = 20;

    m_OriginalHandlingData[42].iVehicleID = 42;
    m_OriginalHandlingData[42].fMass = 1650.000000f;
    m_OriginalHandlingData[42].fTurnMass = 3851.399902f;
    m_OriginalHandlingData[42].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[42].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.050000f);
    m_OriginalHandlingData[42].uiPercentSubmerged = 75;
    m_OriginalHandlingData[42].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[42].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[42].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[42].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[42].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[42].Transmission.fEngineAcceleration = 8.800000f;
    m_OriginalHandlingData[42].Transmission.fEngineInertia = 8.000000f;
    m_OriginalHandlingData[42].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[42].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[42].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[42].bABS = false;
    m_OriginalHandlingData[42].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[42].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[42].fTractionBias = 0.510000f;
    m_OriginalHandlingData[42].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[42].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[42].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[42].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[42].fSuspensionLowerLimit = -0.190000f;
    m_OriginalHandlingData[42].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[42].fSuspensionAntiDiveMultiplier = 0.550000f;
    m_OriginalHandlingData[42].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[42].uiModelFlags = 0x0;
    m_OriginalHandlingData[42].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[42].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[42].uiMonetary = 35000;
    m_OriginalHandlingData[42].ucHeadLight = 0;
    m_OriginalHandlingData[42].ucTailLight = 1;
    m_OriginalHandlingData[42].ucAnimGroup = 0;

    m_OriginalHandlingData[43].iVehicleID = 43;
    m_OriginalHandlingData[43].fMass = 1900.000000f;
    m_OriginalHandlingData[43].fTurnMass = 4795.899902f;
    m_OriginalHandlingData[43].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[43].vecCenterOfMass = CVector(0.000000f, -0.300000f, 0.000000f);
    m_OriginalHandlingData[43].uiPercentSubmerged = 85;
    m_OriginalHandlingData[43].fTractionMultiplier = 0.970000f;
    m_OriginalHandlingData[43].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[43].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[43].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[43].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[43].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[43].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[43].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[43].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[43].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[43].bABS = false;
    m_OriginalHandlingData[43].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[43].fTractionLoss = 0.770000f;
    m_OriginalHandlingData[43].fTractionBias = 0.510000f;
    m_OriginalHandlingData[43].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[43].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[43].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[43].fSuspensionUpperLimit = 0.000000f;
    m_OriginalHandlingData[43].fSuspensionLowerLimit = -1.000000f;
    m_OriginalHandlingData[43].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[43].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[43].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[43].uiModelFlags = 0x1;
    m_OriginalHandlingData[43].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[43].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[43].uiMonetary = 26000;
    m_OriginalHandlingData[43].ucHeadLight = 0;
    m_OriginalHandlingData[43].ucTailLight = 1;
    m_OriginalHandlingData[43].ucAnimGroup = 13;

    m_OriginalHandlingData[44].iVehicleID = 44;
    m_OriginalHandlingData[44].fMass = 25500.000000f;
    m_OriginalHandlingData[44].fTurnMass = 139272.500000f;
    m_OriginalHandlingData[44].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[44].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[44].uiPercentSubmerged = 85;
    m_OriginalHandlingData[44].fTractionMultiplier = 0.580000f;
    m_OriginalHandlingData[44].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[44].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[44].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[44].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[44].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[44].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[44].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[44].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[44].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[44].bABS = false;
    m_OriginalHandlingData[44].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[44].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[44].fTractionBias = 0.460000f;
    m_OriginalHandlingData[44].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[44].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[44].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[44].fSuspensionUpperLimit = 0.470000f;
    m_OriginalHandlingData[44].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[44].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[44].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[44].fCollisionDamageMultiplier = 1.000000f;
    m_OriginalHandlingData[44].uiModelFlags = 0x0;
    m_OriginalHandlingData[44].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[44].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[44].uiMonetary = 10000;
    m_OriginalHandlingData[44].ucHeadLight = 0;
    m_OriginalHandlingData[44].ucTailLight = 1;
    m_OriginalHandlingData[44].ucAnimGroup = 0;

    m_OriginalHandlingData[45].iVehicleID = 45;
    m_OriginalHandlingData[45].fMass = 3800.000000f;
    m_OriginalHandlingData[45].fTurnMass = 30000.000000f;
    m_OriginalHandlingData[45].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[45].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.500000f);
    m_OriginalHandlingData[45].uiPercentSubmerged = 90;
    m_OriginalHandlingData[45].fTractionMultiplier = 0.450000f;
    m_OriginalHandlingData[45].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[45].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[45].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[45].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[45].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[45].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[45].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[45].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[45].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[45].bABS = false;
    m_OriginalHandlingData[45].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[45].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[45].fTractionBias = 0.500000f;
    m_OriginalHandlingData[45].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[45].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[45].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[45].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[45].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[45].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[45].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[45].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[45].uiModelFlags = 0x20002000;
    m_OriginalHandlingData[45].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[45].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[45].uiMonetary = 35000;
    m_OriginalHandlingData[45].ucHeadLight = 0;
    m_OriginalHandlingData[45].ucTailLight = 1;
    m_OriginalHandlingData[45].ucAnimGroup = 0;

    m_OriginalHandlingData[46].iVehicleID = 46;
    m_OriginalHandlingData[46].fMass = 1400.000000f;
    m_OriginalHandlingData[46].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[46].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[46].vecCenterOfMass = CVector(0.000000f, -0.300000f, -0.200000f);
    m_OriginalHandlingData[46].uiPercentSubmerged = 70;
    m_OriginalHandlingData[46].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[46].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[46].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[46].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[46].Transmission.uiHandlingFlags = 12616705;
    m_OriginalHandlingData[46].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[46].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[46].Transmission.fMaxVelocity = 240.000000f;
    m_OriginalHandlingData[46].fBrakeDeceleration = 11.000000f;
    m_OriginalHandlingData[46].fBrakeBias = 0.510000f;
    m_OriginalHandlingData[46].bABS = false;
    m_OriginalHandlingData[46].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[46].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[46].fTractionBias = 0.450000f;
    m_OriginalHandlingData[46].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[46].fSuspensionDamping = 0.130000f;
    m_OriginalHandlingData[46].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[46].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[46].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[46].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[46].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[46].fCollisionDamageMultiplier = 0.720000f;
    m_OriginalHandlingData[46].uiModelFlags = 0x40002004;
    m_OriginalHandlingData[46].uiHandlingFlags = 0xC08401;
    m_OriginalHandlingData[46].fSeatOffsetDistance = 0.170000f;
    m_OriginalHandlingData[46].uiMonetary = 95000;
    m_OriginalHandlingData[46].ucHeadLight = 1;
    m_OriginalHandlingData[46].ucTailLight = 1;
    m_OriginalHandlingData[46].ucAnimGroup = 1;

    m_OriginalHandlingData[47].iVehicleID = 47;
    m_OriginalHandlingData[47].fMass = 8500.000000f;
    m_OriginalHandlingData[47].fTurnMass = 48804.199219f;
    m_OriginalHandlingData[47].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[47].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.300000f);
    m_OriginalHandlingData[47].uiPercentSubmerged = 90;
    m_OriginalHandlingData[47].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[47].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[47].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[47].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[47].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[47].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[47].Transmission.fEngineInertia = 80.000000f;
    m_OriginalHandlingData[47].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[47].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[47].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[47].bABS = false;
    m_OriginalHandlingData[47].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[47].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[47].fTractionBias = 0.460000f;
    m_OriginalHandlingData[47].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[47].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[47].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[47].fSuspensionUpperLimit = 0.470000f;
    m_OriginalHandlingData[47].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[47].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[47].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[47].fCollisionDamageMultiplier = 0.430000f;
    m_OriginalHandlingData[47].uiModelFlags = 0x801;
    m_OriginalHandlingData[47].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[47].fSeatOffsetDistance = 0.620000f;
    m_OriginalHandlingData[47].uiMonetary = 10000;
    m_OriginalHandlingData[47].ucHeadLight = 0;
    m_OriginalHandlingData[47].ucTailLight = 1;
    m_OriginalHandlingData[47].ucAnimGroup = 2;

    m_OriginalHandlingData[48].iVehicleID = 48;
    m_OriginalHandlingData[48].fMass = 4500.000000f;
    m_OriginalHandlingData[48].fTurnMass = 18003.699219f;
    m_OriginalHandlingData[48].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[48].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[48].uiPercentSubmerged = 80;
    m_OriginalHandlingData[48].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[48].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[48].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[48].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[48].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[48].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[48].Transmission.fEngineInertia = 40.000000f;
    m_OriginalHandlingData[48].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[48].fBrakeDeceleration = 4.500000f;
    m_OriginalHandlingData[48].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[48].bABS = false;
    m_OriginalHandlingData[48].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[48].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[48].fTractionBias = 0.480000f;
    m_OriginalHandlingData[48].fSuspensionForceLevel = 1.800000f;
    m_OriginalHandlingData[48].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[48].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[48].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[48].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[48].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[48].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[48].fCollisionDamageMultiplier = 0.450000f;
    m_OriginalHandlingData[48].uiModelFlags = 0x4088;
    m_OriginalHandlingData[48].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[48].fSeatOffsetDistance = 0.350000f;
    m_OriginalHandlingData[48].uiMonetary = 22000;
    m_OriginalHandlingData[48].ucHeadLight = 0;
    m_OriginalHandlingData[48].ucTailLight = 1;
    m_OriginalHandlingData[48].ucAnimGroup = 0;

    m_OriginalHandlingData[49].iVehicleID = 49;
    m_OriginalHandlingData[49].fMass = 1000.000000f;
    m_OriginalHandlingData[49].fTurnMass = 1354.199951f;
    m_OriginalHandlingData[49].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[49].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[49].uiPercentSubmerged = 70;
    m_OriginalHandlingData[49].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[49].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[49].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[49].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[49].Transmission.uiHandlingFlags = 34820;
    m_OriginalHandlingData[49].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[49].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[49].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[49].fBrakeDeceleration = 13.000000f;
    m_OriginalHandlingData[49].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[49].bABS = false;
    m_OriginalHandlingData[49].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[49].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[49].fTractionBias = 0.500000f;
    m_OriginalHandlingData[49].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[49].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[49].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[49].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[49].fSuspensionLowerLimit = -0.130000f;
    m_OriginalHandlingData[49].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[49].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[49].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[49].uiModelFlags = 0x1100;
    m_OriginalHandlingData[49].uiHandlingFlags = 0x8804;
    m_OriginalHandlingData[49].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[49].uiMonetary = 9000;
    m_OriginalHandlingData[49].ucHeadLight = 1;
    m_OriginalHandlingData[49].ucTailLight = 1;
    m_OriginalHandlingData[49].ucAnimGroup = 0;

    m_OriginalHandlingData[50].iVehicleID = 50;
    m_OriginalHandlingData[50].fMass = 2000.000000f;
    m_OriginalHandlingData[50].fTurnMass = 5500.000000f;
    m_OriginalHandlingData[50].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[50].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[50].uiPercentSubmerged = 75;
    m_OriginalHandlingData[50].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[50].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[50].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[50].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[50].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[50].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[50].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[50].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[50].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[50].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[50].bABS = false;
    m_OriginalHandlingData[50].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[50].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[50].fTractionBias = 0.520000f;
    m_OriginalHandlingData[50].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[50].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[50].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[50].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[50].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[50].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[50].fSuspensionAntiDiveMultiplier = 0.200000f;
    m_OriginalHandlingData[50].fCollisionDamageMultiplier = 0.480000f;
    m_OriginalHandlingData[50].uiModelFlags = 0x20;
    m_OriginalHandlingData[50].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[50].fSeatOffsetDistance = 0.240000f;
    m_OriginalHandlingData[50].uiMonetary = 18000;
    m_OriginalHandlingData[50].ucHeadLight = 1;
    m_OriginalHandlingData[50].ucTailLight = 1;
    m_OriginalHandlingData[50].ucAnimGroup = 0;

    m_OriginalHandlingData[51].iVehicleID = 51;
    m_OriginalHandlingData[51].fMass = 1900.000000f;
    m_OriginalHandlingData[51].fTurnMass = 6333.299805f;
    m_OriginalHandlingData[51].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[51].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[51].uiPercentSubmerged = 80;
    m_OriginalHandlingData[51].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[51].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[51].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[51].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[51].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[51].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[51].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[51].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[51].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[51].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[51].bABS = false;
    m_OriginalHandlingData[51].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[51].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[51].fTractionBias = 0.460000f;
    m_OriginalHandlingData[51].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[51].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[51].fSuspensionHighSpdDamping = 2.000000f;
    m_OriginalHandlingData[51].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[51].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[51].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[51].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[51].fCollisionDamageMultiplier = 0.430000f;
    m_OriginalHandlingData[51].uiModelFlags = 0x1;
    m_OriginalHandlingData[51].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[51].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[51].uiMonetary = 20000;
    m_OriginalHandlingData[51].ucHeadLight = 0;
    m_OriginalHandlingData[51].ucTailLight = 3;
    m_OriginalHandlingData[51].ucAnimGroup = 13;

    m_OriginalHandlingData[52].iVehicleID = 52;
    m_OriginalHandlingData[52].fMass = 1600.000000f;
    m_OriginalHandlingData[52].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[52].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[52].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.050000f);
    m_OriginalHandlingData[52].uiPercentSubmerged = 75;
    m_OriginalHandlingData[52].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[52].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[52].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[52].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[52].Transmission.uiHandlingFlags = 276824066;
    m_OriginalHandlingData[52].Transmission.fEngineAcceleration = 8.800000f;
    m_OriginalHandlingData[52].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[52].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[52].fBrakeDeceleration = 6.200000f;
    m_OriginalHandlingData[52].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[52].bABS = false;
    m_OriginalHandlingData[52].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[52].fTractionLoss = 0.840000f;
    m_OriginalHandlingData[52].fTractionBias = 0.520000f;
    m_OriginalHandlingData[52].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[52].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[52].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[52].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[52].fSuspensionLowerLimit = -0.220000f;
    m_OriginalHandlingData[52].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[52].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[52].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[52].uiModelFlags = 0x0;
    m_OriginalHandlingData[52].uiHandlingFlags = 0x10800002;
    m_OriginalHandlingData[52].fSeatOffsetDistance = 0.230000f;
    m_OriginalHandlingData[52].uiMonetary = 20000;
    m_OriginalHandlingData[52].ucHeadLight = 1;
    m_OriginalHandlingData[52].ucTailLight = 1;
    m_OriginalHandlingData[52].ucAnimGroup = 0;

    m_OriginalHandlingData[53].iVehicleID = 53;
    m_OriginalHandlingData[53].fMass = 1900.000000f;
    m_OriginalHandlingData[53].fTurnMass = 4529.899902f;
    m_OriginalHandlingData[53].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[53].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[53].uiPercentSubmerged = 75;
    m_OriginalHandlingData[53].fTractionMultiplier = 0.670000f;
    m_OriginalHandlingData[53].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[53].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[53].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[53].Transmission.uiHandlingFlags = 276824064;
    m_OriginalHandlingData[53].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[53].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[53].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[53].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[53].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[53].bABS = false;
    m_OriginalHandlingData[53].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[53].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[53].fTractionBias = 0.520000f;
    m_OriginalHandlingData[53].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[53].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[53].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[53].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[53].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[53].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[53].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[53].fCollisionDamageMultiplier = 0.450000f;
    m_OriginalHandlingData[53].uiModelFlags = 0x0;
    m_OriginalHandlingData[53].uiHandlingFlags = 0x10800000;
    m_OriginalHandlingData[53].fSeatOffsetDistance = 0.230000f;
    m_OriginalHandlingData[53].uiMonetary = 20000;
    m_OriginalHandlingData[53].ucHeadLight = 2;
    m_OriginalHandlingData[53].ucTailLight = 1;
    m_OriginalHandlingData[53].ucAnimGroup = 0;

    m_OriginalHandlingData[54].iVehicleID = 54;
    m_OriginalHandlingData[54].fMass = 2500.000000f;
    m_OriginalHandlingData[54].fTurnMass = 7968.700195f;
    m_OriginalHandlingData[54].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[54].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[54].uiPercentSubmerged = 80;
    m_OriginalHandlingData[54].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[54].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[54].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[54].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[54].Transmission.uiHandlingFlags = 3145728;
    m_OriginalHandlingData[54].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[54].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[54].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[54].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[54].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[54].bABS = false;
    m_OriginalHandlingData[54].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[54].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[54].fTractionBias = 0.500000f;
    m_OriginalHandlingData[54].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[54].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[54].fSuspensionHighSpdDamping = 4.000000f;
    m_OriginalHandlingData[54].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[54].fSuspensionLowerLimit = -0.350000f;
    m_OriginalHandlingData[54].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[54].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[54].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[54].uiModelFlags = 0x8;
    m_OriginalHandlingData[54].uiHandlingFlags = 0x300000;
    m_OriginalHandlingData[54].fSeatOffsetDistance = 0.280000f;
    m_OriginalHandlingData[54].uiMonetary = 40000;
    m_OriginalHandlingData[54].ucHeadLight = 0;
    m_OriginalHandlingData[54].ucTailLight = 1;
    m_OriginalHandlingData[54].ucAnimGroup = 0;

    m_OriginalHandlingData[55].iVehicleID = 55;
    m_OriginalHandlingData[55].fMass = 1950.000000f;
    m_OriginalHandlingData[55].fTurnMass = 4712.500000f;
    m_OriginalHandlingData[55].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[55].vecCenterOfMass = CVector(0.000000f, 0.300000f, 0.000000f);
    m_OriginalHandlingData[55].uiPercentSubmerged = 70;
    m_OriginalHandlingData[55].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[55].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[55].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[55].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[55].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[55].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[55].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[55].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[55].fBrakeDeceleration = 3.500000f;
    m_OriginalHandlingData[55].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[55].bABS = false;
    m_OriginalHandlingData[55].fSteeringLock = 28.000000f;
    m_OriginalHandlingData[55].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[55].fTractionBias = 0.510000f;
    m_OriginalHandlingData[55].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[55].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[55].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[55].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[55].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[55].fSuspensionFrontRearBias = 0.580000f;
    m_OriginalHandlingData[55].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[55].fCollisionDamageMultiplier = 0.420000f;
    m_OriginalHandlingData[55].uiModelFlags = 0x40002000;
    m_OriginalHandlingData[55].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[55].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[55].uiMonetary = 19000;
    m_OriginalHandlingData[55].ucHeadLight = 1;
    m_OriginalHandlingData[55].ucTailLight = 3;
    m_OriginalHandlingData[55].ucAnimGroup = 0;

    m_OriginalHandlingData[56].iVehicleID = 56;
    m_OriginalHandlingData[56].fMass = 1700.000000f;
    m_OriginalHandlingData[56].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[56].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[56].vecCenterOfMass = CVector(0.000000f, 0.100000f, 0.000000f);
    m_OriginalHandlingData[56].uiPercentSubmerged = 70;
    m_OriginalHandlingData[56].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[56].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[56].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[56].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[56].Transmission.uiHandlingFlags = 268435462;
    m_OriginalHandlingData[56].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[56].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[56].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[56].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[56].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[56].bABS = false;
    m_OriginalHandlingData[56].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[56].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[56].fTractionBias = 0.530000f;
    m_OriginalHandlingData[56].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[56].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[56].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[56].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[56].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[56].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[56].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[56].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[56].uiModelFlags = 0x0;
    m_OriginalHandlingData[56].uiHandlingFlags = 0x10000006;
    m_OriginalHandlingData[56].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[56].uiMonetary = 19000;
    m_OriginalHandlingData[56].ucHeadLight = 1;
    m_OriginalHandlingData[56].ucTailLight = 1;
    m_OriginalHandlingData[56].ucAnimGroup = 0;

    m_OriginalHandlingData[57].iVehicleID = 57;
    m_OriginalHandlingData[57].fMass = 1400.000000f;
    m_OriginalHandlingData[57].fTurnMass = 2979.699951f;
    m_OriginalHandlingData[57].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[57].vecCenterOfMass = CVector(0.000000f, 0.200000f, -0.100000f);
    m_OriginalHandlingData[57].uiPercentSubmerged = 70;
    m_OriginalHandlingData[57].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[57].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[57].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[57].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[57].Transmission.uiHandlingFlags = 12582912;
    m_OriginalHandlingData[57].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[57].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[57].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[57].fBrakeDeceleration = 11.100000f;
    m_OriginalHandlingData[57].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[57].bABS = false;
    m_OriginalHandlingData[57].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[57].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[57].fTractionBias = 0.510000f;
    m_OriginalHandlingData[57].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[57].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[57].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[57].fSuspensionUpperLimit = 0.310000f;
    m_OriginalHandlingData[57].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[57].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[57].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[57].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[57].uiModelFlags = 0x0;
    m_OriginalHandlingData[57].uiHandlingFlags = 0xC00000;
    m_OriginalHandlingData[57].fSeatOffsetDistance = 0.240000f;
    m_OriginalHandlingData[57].uiMonetary = 45000;
    m_OriginalHandlingData[57].ucHeadLight = 1;
    m_OriginalHandlingData[57].ucTailLight = 1;
    m_OriginalHandlingData[57].ucAnimGroup = 0;

    m_OriginalHandlingData[58].iVehicleID = 58;
    m_OriginalHandlingData[58].fMass = 1850.000000f;
    m_OriginalHandlingData[58].fTurnMass = 3534.000000f;
    m_OriginalHandlingData[58].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[58].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[58].uiPercentSubmerged = 75;
    m_OriginalHandlingData[58].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[58].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[58].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[58].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[58].Transmission.uiHandlingFlags = 268435462;
    m_OriginalHandlingData[58].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[58].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[58].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[58].fBrakeDeceleration = 6.500000f;
    m_OriginalHandlingData[58].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[58].bABS = false;
    m_OriginalHandlingData[58].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[58].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[58].fTractionBias = 0.500000f;
    m_OriginalHandlingData[58].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[58].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[58].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[58].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[58].fSuspensionLowerLimit = -0.180000f;
    m_OriginalHandlingData[58].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[58].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[58].fCollisionDamageMultiplier = 0.190000f;
    m_OriginalHandlingData[58].uiModelFlags = 0x40;
    m_OriginalHandlingData[58].uiHandlingFlags = 0x10000006;
    m_OriginalHandlingData[58].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[58].uiMonetary = 26000;
    m_OriginalHandlingData[58].ucHeadLight = 1;
    m_OriginalHandlingData[58].ucTailLight = 1;
    m_OriginalHandlingData[58].ucAnimGroup = 0;

    m_OriginalHandlingData[59].iVehicleID = 59;
    m_OriginalHandlingData[59].fMass = 1500.000000f;
    m_OriginalHandlingData[59].fTurnMass = 3800.000000f;
    m_OriginalHandlingData[59].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[59].vecCenterOfMass = CVector(0.000000f, 0.200000f, 0.000000f);
    m_OriginalHandlingData[59].uiPercentSubmerged = 75;
    m_OriginalHandlingData[59].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[59].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[59].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[59].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[59].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[59].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[59].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[59].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[59].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[59].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[59].bABS = false;
    m_OriginalHandlingData[59].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[59].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[59].fTractionBias = 0.520000f;
    m_OriginalHandlingData[59].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[59].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[59].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[59].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[59].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[59].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[59].fSuspensionAntiDiveMultiplier = 0.200000f;
    m_OriginalHandlingData[59].fCollisionDamageMultiplier = 0.480000f;
    m_OriginalHandlingData[59].uiModelFlags = 0x20;
    m_OriginalHandlingData[59].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[59].fSeatOffsetDistance = 0.240000f;
    m_OriginalHandlingData[59].uiMonetary = 18000;
    m_OriginalHandlingData[59].ucHeadLight = 1;
    m_OriginalHandlingData[59].ucTailLight = 1;
    m_OriginalHandlingData[59].ucAnimGroup = 0;

    m_OriginalHandlingData[60].iVehicleID = 60;
    m_OriginalHandlingData[60].fMass = 1400.000000f;
    m_OriginalHandlingData[60].fTurnMass = 2200.000000f;
    m_OriginalHandlingData[60].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[60].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.200000f);
    m_OriginalHandlingData[60].uiPercentSubmerged = 75;
    m_OriginalHandlingData[60].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[60].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[60].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[60].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[60].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[60].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[60].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[60].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[60].fBrakeDeceleration = 11.000000f;
    m_OriginalHandlingData[60].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[60].bABS = false;
    m_OriginalHandlingData[60].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[60].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[60].fTractionBias = 0.500000f;
    m_OriginalHandlingData[60].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[60].fSuspensionDamping = 0.140000f;
    m_OriginalHandlingData[60].fSuspensionHighSpdDamping = 3.000000f;
    m_OriginalHandlingData[60].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[60].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[60].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[60].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[60].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[60].uiModelFlags = 0x40000800;
    m_OriginalHandlingData[60].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[60].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[60].uiMonetary = 35000;
    m_OriginalHandlingData[60].ucHeadLight = 1;
    m_OriginalHandlingData[60].ucTailLight = 1;
    m_OriginalHandlingData[60].ucAnimGroup = 19;

    m_OriginalHandlingData[61].iVehicleID = 61;
    m_OriginalHandlingData[61].fMass = 1900.000000f;
    m_OriginalHandlingData[61].fTurnMass = 5000.000000f;
    m_OriginalHandlingData[61].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[61].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[61].uiPercentSubmerged = 85;
    m_OriginalHandlingData[61].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[61].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[61].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[61].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[61].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[61].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[61].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[61].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[61].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[61].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[61].bABS = false;
    m_OriginalHandlingData[61].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[61].fTractionLoss = 0.870000f;
    m_OriginalHandlingData[61].fTractionBias = 0.510000f;
    m_OriginalHandlingData[61].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[61].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[61].fSuspensionHighSpdDamping = 2.000000f;
    m_OriginalHandlingData[61].fSuspensionUpperLimit = 0.400000f;
    m_OriginalHandlingData[61].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[61].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[61].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[61].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[61].uiModelFlags = 0x1;
    m_OriginalHandlingData[61].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[61].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[61].uiMonetary = 26000;
    m_OriginalHandlingData[61].ucHeadLight = 0;
    m_OriginalHandlingData[61].ucTailLight = 1;
    m_OriginalHandlingData[61].ucAnimGroup = 13;

    m_OriginalHandlingData[62].iVehicleID = 62;
    m_OriginalHandlingData[62].fMass = 1900.000000f;
    m_OriginalHandlingData[62].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[62].fDragCoeff = 2.600000f;
    m_OriginalHandlingData[62].vecCenterOfMass = CVector(0.000000f, -0.500000f, -0.400000f);
    m_OriginalHandlingData[62].uiPercentSubmerged = 85;
    m_OriginalHandlingData[62].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[62].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[62].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[62].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[62].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[62].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[62].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[62].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[62].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[62].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[62].bABS = false;
    m_OriginalHandlingData[62].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[62].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[62].fTractionBias = 0.460000f;
    m_OriginalHandlingData[62].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[62].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[62].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[62].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[62].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[62].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[62].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[62].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[62].uiModelFlags = 0x4000C000;
    m_OriginalHandlingData[62].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[62].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[62].uiMonetary = 26000;
    m_OriginalHandlingData[62].ucHeadLight = 0;
    m_OriginalHandlingData[62].ucTailLight = 1;
    m_OriginalHandlingData[62].ucAnimGroup = 0;

    m_OriginalHandlingData[63].iVehicleID = 63;
    m_OriginalHandlingData[63].fMass = 1000.000000f;
    m_OriginalHandlingData[63].fTurnMass = 1354.199951f;
    m_OriginalHandlingData[63].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[63].vecCenterOfMass = CVector(0.000000f, 0.400000f, -0.200000f);
    m_OriginalHandlingData[63].uiPercentSubmerged = 70;
    m_OriginalHandlingData[63].fTractionMultiplier = 1.000000f;
    m_OriginalHandlingData[63].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[63].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[63].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[63].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[63].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[63].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[63].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[63].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[63].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[63].bABS = false;
    m_OriginalHandlingData[63].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[63].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[63].fTractionBias = 0.500000f;
    m_OriginalHandlingData[63].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[63].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[63].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[63].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[63].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[63].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[63].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[63].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[63].uiModelFlags = 0x3300;
    m_OriginalHandlingData[63].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[63].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[63].uiMonetary = 9000;
    m_OriginalHandlingData[63].ucHeadLight = 1;
    m_OriginalHandlingData[63].ucTailLight = 1;
    m_OriginalHandlingData[63].ucAnimGroup = 0;

    m_OriginalHandlingData[64].iVehicleID = 64;
    m_OriginalHandlingData[64].fMass = 10000.000000f;
    m_OriginalHandlingData[64].fTurnMass = 35000.000000f;
    m_OriginalHandlingData[64].fDragCoeff = 20.000000f;
    m_OriginalHandlingData[64].vecCenterOfMass = CVector(0.000000f, -0.500000f, -0.500000f);
    m_OriginalHandlingData[64].uiPercentSubmerged = 90;
    m_OriginalHandlingData[64].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[64].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[64].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[64].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[64].Transmission.uiHandlingFlags = 3408416;
    m_OriginalHandlingData[64].Transmission.fEngineAcceleration = 14.000000f;
    m_OriginalHandlingData[64].Transmission.fEngineInertia = 150.000000f;
    m_OriginalHandlingData[64].Transmission.fMaxVelocity = 100.000000f;
    m_OriginalHandlingData[64].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[64].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[64].bABS = false;
    m_OriginalHandlingData[64].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[64].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[64].fTractionBias = 0.600000f;
    m_OriginalHandlingData[64].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[64].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[64].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[64].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[64].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[64].fSuspensionFrontRearBias = 0.350000f;
    m_OriginalHandlingData[64].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[64].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[64].uiModelFlags = 0x308;
    m_OriginalHandlingData[64].uiHandlingFlags = 0x340220;
    m_OriginalHandlingData[64].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[64].uiMonetary = 5000;
    m_OriginalHandlingData[64].ucHeadLight = 0;
    m_OriginalHandlingData[64].ucTailLight = 1;
    m_OriginalHandlingData[64].ucAnimGroup = 17;

    m_OriginalHandlingData[65].iVehicleID = 65;
    m_OriginalHandlingData[65].fMass = 2500.000000f;
    m_OriginalHandlingData[65].fTurnMass = 7604.200195f;
    m_OriginalHandlingData[65].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[65].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.350000f);
    m_OriginalHandlingData[65].uiPercentSubmerged = 80;
    m_OriginalHandlingData[65].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[65].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[65].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[65].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[65].Transmission.uiHandlingFlags = 1048580;
    m_OriginalHandlingData[65].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[65].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[65].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[65].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[65].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[65].bABS = false;
    m_OriginalHandlingData[65].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[65].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[65].fTractionBias = 0.540000f;
    m_OriginalHandlingData[65].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[65].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[65].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[65].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[65].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[65].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[65].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[65].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[65].uiModelFlags = 0x4020;
    m_OriginalHandlingData[65].uiHandlingFlags = 0x100004;
    m_OriginalHandlingData[65].fSeatOffsetDistance = 0.440000f;
    m_OriginalHandlingData[65].uiMonetary = 40000;
    m_OriginalHandlingData[65].ucHeadLight = 0;
    m_OriginalHandlingData[65].ucTailLight = 1;
    m_OriginalHandlingData[65].ucAnimGroup = 0;

    m_OriginalHandlingData[66].iVehicleID = 66;
    m_OriginalHandlingData[66].fMass = 3500.000000f;
    m_OriginalHandlingData[66].fTurnMass = 11156.200195f;
    m_OriginalHandlingData[66].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[66].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[66].uiPercentSubmerged = 80;
    m_OriginalHandlingData[66].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[66].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[66].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[66].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[66].Transmission.uiHandlingFlags = 5242880;
    m_OriginalHandlingData[66].Transmission.fEngineAcceleration = 8.800000f;
    m_OriginalHandlingData[66].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[66].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[66].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[66].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[66].bABS = false;
    m_OriginalHandlingData[66].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[66].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[66].fTractionBias = 0.520000f;
    m_OriginalHandlingData[66].fSuspensionForceLevel = 0.700000f;
    m_OriginalHandlingData[66].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[66].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[66].fSuspensionUpperLimit = 0.340000f;
    m_OriginalHandlingData[66].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[66].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[66].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[66].fCollisionDamageMultiplier = 0.300000f;
    m_OriginalHandlingData[66].uiModelFlags = 0x4020;
    m_OriginalHandlingData[66].uiHandlingFlags = 0x500000;
    m_OriginalHandlingData[66].fSeatOffsetDistance = 0.440000f;
    m_OriginalHandlingData[66].uiMonetary = 40000;
    m_OriginalHandlingData[66].ucHeadLight = 0;
    m_OriginalHandlingData[66].ucTailLight = 1;
    m_OriginalHandlingData[66].ucAnimGroup = 0;

    m_OriginalHandlingData[67].iVehicleID = 67;
    m_OriginalHandlingData[67].fMass = 1700.000000f;
    m_OriginalHandlingData[67].fTurnMass = 3435.399902f;
    m_OriginalHandlingData[67].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[67].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[67].uiPercentSubmerged = 70;
    m_OriginalHandlingData[67].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[67].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[67].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[67].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[67].Transmission.uiHandlingFlags = 268435456;
    m_OriginalHandlingData[67].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[67].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[67].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[67].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[67].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[67].bABS = false;
    m_OriginalHandlingData[67].fSteeringLock = 32.000000f;
    m_OriginalHandlingData[67].fTractionLoss = 0.860000f;
    m_OriginalHandlingData[67].fTractionBias = 0.500000f;
    m_OriginalHandlingData[67].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[67].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[67].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[67].fSuspensionUpperLimit = 0.310000f;
    m_OriginalHandlingData[67].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[67].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[67].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[67].fCollisionDamageMultiplier = 0.850000f;
    m_OriginalHandlingData[67].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[67].uiHandlingFlags = 0x10000000;
    m_OriginalHandlingData[67].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[67].uiMonetary = 9000;
    m_OriginalHandlingData[67].ucHeadLight = 0;
    m_OriginalHandlingData[67].ucTailLight = 0;
    m_OriginalHandlingData[67].ucAnimGroup = 0;

    m_OriginalHandlingData[68].iVehicleID = 68;
    m_OriginalHandlingData[68].fMass = 1600.000000f;
    m_OriginalHandlingData[68].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[68].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[68].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[68].uiPercentSubmerged = 70;
    m_OriginalHandlingData[68].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[68].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[68].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[68].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[68].Transmission.uiHandlingFlags = 268435457;
    m_OriginalHandlingData[68].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[68].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[68].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[68].fBrakeDeceleration = 5.400000f;
    m_OriginalHandlingData[68].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[68].bABS = false;
    m_OriginalHandlingData[68].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[68].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[68].fTractionBias = 0.520000f;
    m_OriginalHandlingData[68].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[68].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[68].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[68].fSuspensionUpperLimit = 0.320000f;
    m_OriginalHandlingData[68].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[68].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[68].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[68].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[68].uiModelFlags = 0x0;
    m_OriginalHandlingData[68].uiHandlingFlags = 0x10000001;
    m_OriginalHandlingData[68].fSeatOffsetDistance = 0.220000f;
    m_OriginalHandlingData[68].uiMonetary = 19000;
    m_OriginalHandlingData[68].ucHeadLight = 0;
    m_OriginalHandlingData[68].ucTailLight = 3;
    m_OriginalHandlingData[68].ucAnimGroup = 0;

    m_OriginalHandlingData[69].iVehicleID = 69;
    m_OriginalHandlingData[69].fMass = 1600.000000f;
    m_OriginalHandlingData[69].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[69].fDragCoeff = 1.400000f;
    m_OriginalHandlingData[69].vecCenterOfMass = CVector(0.000000f, 0.200000f, -0.400000f);
    m_OriginalHandlingData[69].uiPercentSubmerged = 70;
    m_OriginalHandlingData[69].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[69].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[69].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[69].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[69].Transmission.uiHandlingFlags = 12582912;
    m_OriginalHandlingData[69].Transmission.fEngineAcceleration = 10.400000f;
    m_OriginalHandlingData[69].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[69].Transmission.fMaxVelocity = 220.000000f;
    m_OriginalHandlingData[69].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[69].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[69].bABS = false;
    m_OriginalHandlingData[69].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[69].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[69].fTractionBias = 0.480000f;
    m_OriginalHandlingData[69].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[69].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[69].fSuspensionHighSpdDamping = 10.000000f;
    m_OriginalHandlingData[69].fSuspensionUpperLimit = 0.290000f;
    m_OriginalHandlingData[69].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[69].fSuspensionFrontRearBias = 0.600000f;
    m_OriginalHandlingData[69].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[69].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[69].uiModelFlags = 0x40002004;
    m_OriginalHandlingData[69].uiHandlingFlags = 0xC00000;
    m_OriginalHandlingData[69].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[69].uiMonetary = 45000;
    m_OriginalHandlingData[69].ucHeadLight = 1;
    m_OriginalHandlingData[69].ucTailLight = 1;
    m_OriginalHandlingData[69].ucAnimGroup = 0;

    m_OriginalHandlingData[70].iVehicleID = 70;
    m_OriginalHandlingData[70].fMass = 2000.000000f;
    m_OriginalHandlingData[70].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[70].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[70].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.600000f);
    m_OriginalHandlingData[70].uiPercentSubmerged = 80;
    m_OriginalHandlingData[70].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[70].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[70].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[70].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[70].Transmission.uiHandlingFlags = 3246080;
    m_OriginalHandlingData[70].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[70].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[70].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[70].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[70].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[70].bABS = false;
    m_OriginalHandlingData[70].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[70].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[70].fTractionBias = 0.500000f;
    m_OriginalHandlingData[70].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[70].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[70].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[70].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[70].fSuspensionLowerLimit = -0.310000f;
    m_OriginalHandlingData[70].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[70].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[70].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[70].uiModelFlags = 0x0;
    m_OriginalHandlingData[70].uiHandlingFlags = 0x318800;
    m_OriginalHandlingData[70].fSeatOffsetDistance = 0.380000f;
    m_OriginalHandlingData[70].uiMonetary = 40000;
    m_OriginalHandlingData[70].ucHeadLight = 0;
    m_OriginalHandlingData[70].ucTailLight = 1;
    m_OriginalHandlingData[70].ucAnimGroup = 22;

    m_OriginalHandlingData[71].iVehicleID = 71;
    m_OriginalHandlingData[71].fMass = 1000.000000f;
    m_OriginalHandlingData[71].fTurnMass = 2141.699951f;
    m_OriginalHandlingData[71].fDragCoeff = 2.400000f;
    m_OriginalHandlingData[71].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[71].uiPercentSubmerged = 50;
    m_OriginalHandlingData[71].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[71].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[71].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[71].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[71].Transmission.uiHandlingFlags = 12582912;
    m_OriginalHandlingData[71].Transmission.fEngineAcceleration = 10.400000f;
    m_OriginalHandlingData[71].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[71].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[71].fBrakeDeceleration = 11.000000f;
    m_OriginalHandlingData[71].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[71].bABS = false;
    m_OriginalHandlingData[71].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[71].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[71].fTractionBias = 0.500000f;
    m_OriginalHandlingData[71].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[71].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[71].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[71].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[71].fSuspensionLowerLimit = -0.120000f;
    m_OriginalHandlingData[71].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[71].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[71].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[71].uiModelFlags = 0xC0000000;
    m_OriginalHandlingData[71].uiHandlingFlags = 0xC00000;
    m_OriginalHandlingData[71].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[71].uiMonetary = 35000;
    m_OriginalHandlingData[71].ucHeadLight = 1;
    m_OriginalHandlingData[71].ucTailLight = 1;
    m_OriginalHandlingData[71].ucAnimGroup = 0;

    m_OriginalHandlingData[72].iVehicleID = 72;
    m_OriginalHandlingData[72].fMass = 5500.000000f;
    m_OriginalHandlingData[72].fTurnMass = 23489.599609f;
    m_OriginalHandlingData[72].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[72].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[72].uiPercentSubmerged = 80;
    m_OriginalHandlingData[72].fTractionMultiplier = 0.820000f;
    m_OriginalHandlingData[72].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[72].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[72].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[72].Transmission.uiHandlingFlags = 513;
    m_OriginalHandlingData[72].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[72].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[72].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[72].fBrakeDeceleration = 4.500000f;
    m_OriginalHandlingData[72].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[72].bABS = false;
    m_OriginalHandlingData[72].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[72].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[72].fTractionBias = 0.460000f;
    m_OriginalHandlingData[72].fSuspensionForceLevel = 0.900000f;
    m_OriginalHandlingData[72].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[72].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[72].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[72].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[72].fSuspensionFrontRearBias = 0.350000f;
    m_OriginalHandlingData[72].fSuspensionAntiDiveMultiplier = 0.600000f;
    m_OriginalHandlingData[72].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[72].uiModelFlags = 0x4009;
    m_OriginalHandlingData[72].uiHandlingFlags = 0x201;
    m_OriginalHandlingData[72].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[72].uiMonetary = 22000;
    m_OriginalHandlingData[72].ucHeadLight = 0;
    m_OriginalHandlingData[72].ucTailLight = 3;
    m_OriginalHandlingData[72].ucAnimGroup = 13;

    m_OriginalHandlingData[73].iVehicleID = 73;
    m_OriginalHandlingData[73].fMass = 3500.000000f;
    m_OriginalHandlingData[73].fTurnMass = 13865.799805f;
    m_OriginalHandlingData[73].fDragCoeff = 2.300000f;
    m_OriginalHandlingData[73].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[73].uiPercentSubmerged = 80;
    m_OriginalHandlingData[73].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[73].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[73].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[73].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[73].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[73].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[73].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[73].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[73].fBrakeDeceleration = 4.500000f;
    m_OriginalHandlingData[73].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[73].bABS = false;
    m_OriginalHandlingData[73].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[73].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[73].fTractionBias = 0.460000f;
    m_OriginalHandlingData[73].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[73].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[73].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[73].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[73].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[73].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[73].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[73].fCollisionDamageMultiplier = 0.530000f;
    m_OriginalHandlingData[73].uiModelFlags = 0x4088;
    m_OriginalHandlingData[73].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[73].fSeatOffsetDistance = 0.460000f;
    m_OriginalHandlingData[73].uiMonetary = 22000;
    m_OriginalHandlingData[73].ucHeadLight = 1;
    m_OriginalHandlingData[73].ucTailLight = 3;
    m_OriginalHandlingData[73].ucAnimGroup = 0;

    m_OriginalHandlingData[74].iVehicleID = 74;
    m_OriginalHandlingData[74].fMass = 1300.000000f;
    m_OriginalHandlingData[74].fTurnMass = 1900.000000f;
    m_OriginalHandlingData[74].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[74].vecCenterOfMass = CVector(0.000000f, 0.200000f, -0.300000f);
    m_OriginalHandlingData[74].uiPercentSubmerged = 85;
    m_OriginalHandlingData[74].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[74].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[74].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[74].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[74].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[74].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[74].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[74].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[74].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[74].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[74].bABS = false;
    m_OriginalHandlingData[74].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[74].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[74].fTractionBias = 0.500000f;
    m_OriginalHandlingData[74].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[74].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[74].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[74].fSuspensionUpperLimit = 0.320000f;
    m_OriginalHandlingData[74].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[74].fSuspensionFrontRearBias = 0.350000f;
    m_OriginalHandlingData[74].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[74].fCollisionDamageMultiplier = 0.300000f;
    m_OriginalHandlingData[74].uiModelFlags = 0x200840;
    m_OriginalHandlingData[74].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[74].fSeatOffsetDistance = 0.180000f;
    m_OriginalHandlingData[74].uiMonetary = 25000;
    m_OriginalHandlingData[74].ucHeadLight = 0;
    m_OriginalHandlingData[74].ucTailLight = 1;
    m_OriginalHandlingData[74].ucAnimGroup = 0;

    m_OriginalHandlingData[75].iVehicleID = 75;
    m_OriginalHandlingData[75].fMass = 2100.000000f;
    m_OriginalHandlingData[75].fTurnMass = 5146.700195f;
    m_OriginalHandlingData[75].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[75].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[75].uiPercentSubmerged = 75;
    m_OriginalHandlingData[75].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[75].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[75].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[75].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[75].Transmission.uiHandlingFlags = 276824576;
    m_OriginalHandlingData[75].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[75].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[75].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[75].fBrakeDeceleration = 6.200000f;
    m_OriginalHandlingData[75].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[75].bABS = false;
    m_OriginalHandlingData[75].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[75].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[75].fTractionBias = 0.520000f;
    m_OriginalHandlingData[75].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[75].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[75].fSuspensionHighSpdDamping = 3.000000f;
    m_OriginalHandlingData[75].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[75].fSuspensionLowerLimit = -0.240000f;
    m_OriginalHandlingData[75].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[75].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[75].fCollisionDamageMultiplier = 0.300000f;
    m_OriginalHandlingData[75].uiModelFlags = 0x100;
    m_OriginalHandlingData[75].uiHandlingFlags = 0x10800200;
    m_OriginalHandlingData[75].fSeatOffsetDistance = 0.280000f;
    m_OriginalHandlingData[75].uiMonetary = 500;
    m_OriginalHandlingData[75].ucHeadLight = 1;
    m_OriginalHandlingData[75].ucTailLight = 1;
    m_OriginalHandlingData[75].ucAnimGroup = 25;

    m_OriginalHandlingData[76].iVehicleID = 76;
    m_OriginalHandlingData[76].fMass = 1600.000000f;
    m_OriginalHandlingData[76].fTurnMass = 3866.699951f;
    m_OriginalHandlingData[76].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[76].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[76].uiPercentSubmerged = 75;
    m_OriginalHandlingData[76].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[76].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[76].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[76].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[76].Transmission.uiHandlingFlags = 8389120;
    m_OriginalHandlingData[76].Transmission.fEngineAcceleration = 10.400000f;
    m_OriginalHandlingData[76].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[76].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[76].fBrakeDeceleration = 6.200000f;
    m_OriginalHandlingData[76].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[76].bABS = false;
    m_OriginalHandlingData[76].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[76].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[76].fTractionBias = 0.520000f;
    m_OriginalHandlingData[76].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[76].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[76].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[76].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[76].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[76].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[76].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[76].fCollisionDamageMultiplier = 0.300000f;
    m_OriginalHandlingData[76].uiModelFlags = 0x0;
    m_OriginalHandlingData[76].uiHandlingFlags = 0x800200;
    m_OriginalHandlingData[76].fSeatOffsetDistance = 0.280000f;
    m_OriginalHandlingData[76].uiMonetary = 500;
    m_OriginalHandlingData[76].ucHeadLight = 1;
    m_OriginalHandlingData[76].ucTailLight = 1;
    m_OriginalHandlingData[76].ucAnimGroup = 0;

    m_OriginalHandlingData[77].iVehicleID = 77;
    m_OriginalHandlingData[77].fMass = 1400.000000f;
    m_OriginalHandlingData[77].fTurnMass = 2800.000000f;
    m_OriginalHandlingData[77].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[77].vecCenterOfMass = CVector(0.000000f, -0.200000f, -0.240000f);
    m_OriginalHandlingData[77].uiPercentSubmerged = 70;
    m_OriginalHandlingData[77].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[77].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[77].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[77].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[77].Transmission.uiHandlingFlags = 2129920;
    m_OriginalHandlingData[77].Transmission.fEngineAcceleration = 10.400000f;
    m_OriginalHandlingData[77].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[77].Transmission.fMaxVelocity = 230.000000f;
    m_OriginalHandlingData[77].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[77].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[77].bABS = false;
    m_OriginalHandlingData[77].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[77].fTractionLoss = 0.860000f;
    m_OriginalHandlingData[77].fTractionBias = 0.480000f;
    m_OriginalHandlingData[77].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[77].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[77].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[77].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[77].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[77].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[77].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[77].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[77].uiModelFlags = 0x40002004;
    m_OriginalHandlingData[77].uiHandlingFlags = 0x208000;
    m_OriginalHandlingData[77].fSeatOffsetDistance = 0.400000f;
    m_OriginalHandlingData[77].uiMonetary = 105000;
    m_OriginalHandlingData[77].ucHeadLight = 0;
    m_OriginalHandlingData[77].ucTailLight = 0;
    m_OriginalHandlingData[77].ucAnimGroup = 1;

    m_OriginalHandlingData[78].iVehicleID = 78;
    m_OriginalHandlingData[78].fMass = 2200.000000f;
    m_OriginalHandlingData[78].fTurnMass = 5000.000000f;
    m_OriginalHandlingData[78].fDragCoeff = 1.800000f;
    m_OriginalHandlingData[78].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.100000f);
    m_OriginalHandlingData[78].uiPercentSubmerged = 75;
    m_OriginalHandlingData[78].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[78].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[78].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[78].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[78].Transmission.uiHandlingFlags = 272629760;
    m_OriginalHandlingData[78].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[78].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[78].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[78].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[78].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[78].bABS = false;
    m_OriginalHandlingData[78].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[78].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[78].fTractionBias = 0.460000f;
    m_OriginalHandlingData[78].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[78].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[78].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[78].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[78].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[78].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[78].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[78].fCollisionDamageMultiplier = 0.300000f;
    m_OriginalHandlingData[78].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[78].uiHandlingFlags = 0x10400000;
    m_OriginalHandlingData[78].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[78].uiMonetary = 35000;
    m_OriginalHandlingData[78].ucHeadLight = 0;
    m_OriginalHandlingData[78].ucTailLight = 1;
    m_OriginalHandlingData[78].ucAnimGroup = 0;

    m_OriginalHandlingData[79].iVehicleID = 79;
    m_OriginalHandlingData[79].fMass = 3500.000000f;
    m_OriginalHandlingData[79].fTurnMass = 13865.799805f;
    m_OriginalHandlingData[79].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[79].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[79].uiPercentSubmerged = 80;
    m_OriginalHandlingData[79].fTractionMultiplier = 0.620000f;
    m_OriginalHandlingData[79].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[79].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[79].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[79].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[79].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[79].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[79].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[79].fBrakeDeceleration = 4.500000f;
    m_OriginalHandlingData[79].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[79].bABS = false;
    m_OriginalHandlingData[79].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[79].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[79].fTractionBias = 0.460000f;
    m_OriginalHandlingData[79].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[79].fSuspensionDamping = 0.110000f;
    m_OriginalHandlingData[79].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[79].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[79].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[79].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[79].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[79].fCollisionDamageMultiplier = 0.530000f;
    m_OriginalHandlingData[79].uiModelFlags = 0x88;
    m_OriginalHandlingData[79].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[79].fSeatOffsetDistance = 0.460000f;
    m_OriginalHandlingData[79].uiMonetary = 22000;
    m_OriginalHandlingData[79].ucHeadLight = 0;
    m_OriginalHandlingData[79].ucTailLight = 3;
    m_OriginalHandlingData[79].ucAnimGroup = 0;

    m_OriginalHandlingData[80].iVehicleID = 80;
    m_OriginalHandlingData[80].fMass = 3800.000000f;
    m_OriginalHandlingData[80].fTurnMass = 20000.000000f;
    m_OriginalHandlingData[80].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[80].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[80].uiPercentSubmerged = 90;
    m_OriginalHandlingData[80].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[80].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[80].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[80].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[80].Transmission.uiHandlingFlags = 16777217;
    m_OriginalHandlingData[80].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[80].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[80].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[80].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[80].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[80].bABS = false;
    m_OriginalHandlingData[80].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[80].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[80].fTractionBias = 0.400000f;
    m_OriginalHandlingData[80].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[80].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[80].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[80].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[80].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[80].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[80].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[80].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[80].uiModelFlags = 0x20206008;
    m_OriginalHandlingData[80].uiHandlingFlags = 0x1000001;
    m_OriginalHandlingData[80].fSeatOffsetDistance = 0.350000f;
    m_OriginalHandlingData[80].uiMonetary = 35000;
    m_OriginalHandlingData[80].ucHeadLight = 0;
    m_OriginalHandlingData[80].ucTailLight = 1;
    m_OriginalHandlingData[80].ucAnimGroup = 2;

    m_OriginalHandlingData[81].iVehicleID = 81;
    m_OriginalHandlingData[81].fMass = 5000.000000f;
    m_OriginalHandlingData[81].fTurnMass = 28000.000000f;
    m_OriginalHandlingData[81].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[81].vecCenterOfMass = CVector(0.000000f, 0.500000f, -0.400000f);
    m_OriginalHandlingData[81].uiPercentSubmerged = 90;
    m_OriginalHandlingData[81].fTractionMultiplier = 0.950000f;
    m_OriginalHandlingData[81].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[81].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[81].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[81].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[81].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[81].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[81].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[81].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[81].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[81].bABS = false;
    m_OriginalHandlingData[81].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[81].fTractionLoss = 0.650000f;
    m_OriginalHandlingData[81].fTractionBias = 0.400000f;
    m_OriginalHandlingData[81].fSuspensionForceLevel = 0.700000f;
    m_OriginalHandlingData[81].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[81].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[81].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[81].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[81].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[81].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[81].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[81].uiModelFlags = 0x20200008;
    m_OriginalHandlingData[81].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[81].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[81].uiMonetary = 35000;
    m_OriginalHandlingData[81].ucHeadLight = 0;
    m_OriginalHandlingData[81].ucTailLight = 1;
    m_OriginalHandlingData[81].ucAnimGroup = 2;

    m_OriginalHandlingData[82].iVehicleID = 82;
    m_OriginalHandlingData[82].fMass = 1400.000000f;
    m_OriginalHandlingData[82].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[82].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[82].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.100000f);
    m_OriginalHandlingData[82].uiPercentSubmerged = 75;
    m_OriginalHandlingData[82].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[82].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[82].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[82].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[82].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[82].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[82].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[82].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[82].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[82].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[82].bABS = false;
    m_OriginalHandlingData[82].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[82].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[82].fTractionBias = 0.500000f;
    m_OriginalHandlingData[82].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[82].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[82].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[82].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[82].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[82].fSuspensionFrontRearBias = 0.580000f;
    m_OriginalHandlingData[82].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[82].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[82].uiModelFlags = 0x0;
    m_OriginalHandlingData[82].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[82].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[82].uiMonetary = 35000;
    m_OriginalHandlingData[82].ucHeadLight = 0;
    m_OriginalHandlingData[82].ucTailLight = 1;
    m_OriginalHandlingData[82].ucAnimGroup = 0;

    m_OriginalHandlingData[83].iVehicleID = 83;
    m_OriginalHandlingData[83].fMass = 1400.000000f;
    m_OriginalHandlingData[83].fTurnMass = 3267.800049f;
    m_OriginalHandlingData[83].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[83].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.100000f);
    m_OriginalHandlingData[83].uiPercentSubmerged = 75;
    m_OriginalHandlingData[83].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[83].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[83].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[83].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[83].Transmission.uiHandlingFlags = 272629760;
    m_OriginalHandlingData[83].Transmission.fEngineAcceleration = 8.800000f;
    m_OriginalHandlingData[83].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[83].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[83].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[83].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[83].bABS = false;
    m_OriginalHandlingData[83].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[83].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[83].fTractionBias = 0.520000f;
    m_OriginalHandlingData[83].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[83].fSuspensionDamping = 0.130000f;
    m_OriginalHandlingData[83].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[83].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[83].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[83].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[83].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[83].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[83].uiModelFlags = 0x400000;
    m_OriginalHandlingData[83].uiHandlingFlags = 0x10400000;
    m_OriginalHandlingData[83].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[83].uiMonetary = 35000;
    m_OriginalHandlingData[83].ucHeadLight = 0;
    m_OriginalHandlingData[83].ucTailLight = 1;
    m_OriginalHandlingData[83].ucAnimGroup = 0;

    m_OriginalHandlingData[84].iVehicleID = 84;
    m_OriginalHandlingData[84].fMass = 1700.000000f;
    m_OriginalHandlingData[84].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[84].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[84].vecCenterOfMass = CVector(0.000000f, 0.300000f, 0.000000f);
    m_OriginalHandlingData[84].uiPercentSubmerged = 70;
    m_OriginalHandlingData[84].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[84].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[84].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[84].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[84].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[84].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[84].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[84].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[84].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[84].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[84].bABS = false;
    m_OriginalHandlingData[84].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[84].fTractionLoss = 0.860000f;
    m_OriginalHandlingData[84].fTractionBias = 0.540000f;
    m_OriginalHandlingData[84].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[84].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[84].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[84].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[84].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[84].fSuspensionFrontRearBias = 0.540000f;
    m_OriginalHandlingData[84].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[84].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[84].uiModelFlags = 0x40400004;
    m_OriginalHandlingData[84].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[84].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[84].uiMonetary = 19000;
    m_OriginalHandlingData[84].ucHeadLight = 1;
    m_OriginalHandlingData[84].ucTailLight = 1;
    m_OriginalHandlingData[84].ucAnimGroup = 1;

    m_OriginalHandlingData[85].iVehicleID = 85;
    m_OriginalHandlingData[85].fMass = 5500.000000f;
    m_OriginalHandlingData[85].fTurnMass = 33187.898438f;
    m_OriginalHandlingData[85].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[85].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[85].uiPercentSubmerged = 90;
    m_OriginalHandlingData[85].fTractionMultiplier = 0.580000f;
    m_OriginalHandlingData[85].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[85].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[85].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[85].Transmission.uiHandlingFlags = 262656;
    m_OriginalHandlingData[85].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[85].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[85].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[85].fBrakeDeceleration = 3.170000f;
    m_OriginalHandlingData[85].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[85].bABS = false;
    m_OriginalHandlingData[85].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[85].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[85].fTractionBias = 0.500000f;
    m_OriginalHandlingData[85].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[85].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[85].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[85].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[85].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[85].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[85].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[85].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[85].uiModelFlags = 0x404008;
    m_OriginalHandlingData[85].uiHandlingFlags = 0x40200;
    m_OriginalHandlingData[85].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[85].uiMonetary = 5000;
    m_OriginalHandlingData[85].ucHeadLight = 0;
    m_OriginalHandlingData[85].ucTailLight = 1;
    m_OriginalHandlingData[85].ucAnimGroup = 0;

    m_OriginalHandlingData[86].iVehicleID = 86;
    m_OriginalHandlingData[86].fMass = 3500.000000f;
    m_OriginalHandlingData[86].fTurnMass = 12000.000000f;
    m_OriginalHandlingData[86].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[86].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.250000f);
    m_OriginalHandlingData[86].uiPercentSubmerged = 80;
    m_OriginalHandlingData[86].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[86].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[86].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[86].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[86].Transmission.uiHandlingFlags = 18121216;
    m_OriginalHandlingData[86].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[86].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[86].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[86].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[86].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[86].bABS = false;
    m_OriginalHandlingData[86].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[86].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[86].fTractionBias = 0.460000f;
    m_OriginalHandlingData[86].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[86].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[86].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[86].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[86].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[86].fSuspensionFrontRearBias = 0.250000f;
    m_OriginalHandlingData[86].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[86].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[86].uiModelFlags = 0x240001;
    m_OriginalHandlingData[86].uiHandlingFlags = 0x1148200;
    m_OriginalHandlingData[86].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[86].uiMonetary = 20000;
    m_OriginalHandlingData[86].ucHeadLight = 0;
    m_OriginalHandlingData[86].ucTailLight = 3;
    m_OriginalHandlingData[86].ucAnimGroup = 13;

    m_OriginalHandlingData[87].iVehicleID = 87;
    m_OriginalHandlingData[87].fMass = 1700.000000f;
    m_OriginalHandlingData[87].fTurnMass = 4166.399902f;
    m_OriginalHandlingData[87].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[87].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[87].uiPercentSubmerged = 70;
    m_OriginalHandlingData[87].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[87].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[87].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[87].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[87].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[87].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[87].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[87].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[87].fBrakeDeceleration = 8.170000f;
    m_OriginalHandlingData[87].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[87].bABS = false;
    m_OriginalHandlingData[87].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[87].fTractionLoss = 0.840000f;
    m_OriginalHandlingData[87].fTractionBias = 0.530000f;
    m_OriginalHandlingData[87].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[87].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[87].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[87].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[87].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[87].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[87].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[87].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[87].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[87].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[87].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[87].uiMonetary = 19000;
    m_OriginalHandlingData[87].ucHeadLight = 1;
    m_OriginalHandlingData[87].ucTailLight = 1;
    m_OriginalHandlingData[87].ucAnimGroup = 0;

    m_OriginalHandlingData[88].iVehicleID = 88;
    m_OriginalHandlingData[88].fMass = 1200.000000f;
    m_OriginalHandlingData[88].fTurnMass = 2000.000000f;
    m_OriginalHandlingData[88].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[88].vecCenterOfMass = CVector(0.000000f, 0.150000f, -0.100000f);
    m_OriginalHandlingData[88].uiPercentSubmerged = 70;
    m_OriginalHandlingData[88].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[88].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[88].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[88].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[88].Transmission.uiHandlingFlags = 2;
    m_OriginalHandlingData[88].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[88].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[88].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[88].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[88].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[88].bABS = false;
    m_OriginalHandlingData[88].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[88].fTractionLoss = 0.860000f;
    m_OriginalHandlingData[88].fTractionBias = 0.500000f;
    m_OriginalHandlingData[88].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[88].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[88].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[88].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[88].fSuspensionLowerLimit = -0.080000f;
    m_OriginalHandlingData[88].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[88].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[88].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[88].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[88].uiHandlingFlags = 0x2;
    m_OriginalHandlingData[88].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[88].uiMonetary = 9000;
    m_OriginalHandlingData[88].ucHeadLight = 0;
    m_OriginalHandlingData[88].ucTailLight = 0;
    m_OriginalHandlingData[88].ucAnimGroup = 0;

    m_OriginalHandlingData[89].iVehicleID = 89;
    m_OriginalHandlingData[89].fMass = 4000.000000f;
    m_OriginalHandlingData[89].fTurnMass = 10000.000000f;
    m_OriginalHandlingData[89].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[89].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[89].uiPercentSubmerged = 85;
    m_OriginalHandlingData[89].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[89].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[89].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[89].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[89].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[89].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[89].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[89].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[89].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[89].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[89].bABS = false;
    m_OriginalHandlingData[89].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[89].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[89].fTractionBias = 0.540000f;
    m_OriginalHandlingData[89].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[89].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[89].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[89].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[89].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[89].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[89].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[89].fCollisionDamageMultiplier = 0.160000f;
    m_OriginalHandlingData[89].uiModelFlags = 0x4001;
    m_OriginalHandlingData[89].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[89].fSeatOffsetDistance = 0.320000f;
    m_OriginalHandlingData[89].uiMonetary = 40000;
    m_OriginalHandlingData[89].ucHeadLight = 0;
    m_OriginalHandlingData[89].ucTailLight = 1;
    m_OriginalHandlingData[89].ucAnimGroup = 13;

    m_OriginalHandlingData[90].iVehicleID = 90;
    m_OriginalHandlingData[90].fMass = 1800.000000f;
    m_OriginalHandlingData[90].fTurnMass = 4350.000000f;
    m_OriginalHandlingData[90].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[90].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[90].uiPercentSubmerged = 70;
    m_OriginalHandlingData[90].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[90].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[90].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[90].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[90].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[90].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[90].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[90].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[90].fBrakeDeceleration = 5.400000f;
    m_OriginalHandlingData[90].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[90].bABS = false;
    m_OriginalHandlingData[90].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[90].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[90].fTractionBias = 0.520000f;
    m_OriginalHandlingData[90].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[90].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[90].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[90].fSuspensionUpperLimit = 0.320000f;
    m_OriginalHandlingData[90].fSuspensionLowerLimit = -0.140000f;
    m_OriginalHandlingData[90].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[90].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[90].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[90].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[90].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[90].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[90].uiMonetary = 19000;
    m_OriginalHandlingData[90].ucHeadLight = 0;
    m_OriginalHandlingData[90].ucTailLight = 3;
    m_OriginalHandlingData[90].ucAnimGroup = 0;

    m_OriginalHandlingData[91].iVehicleID = 91;
    m_OriginalHandlingData[91].fMass = 1000.000000f;
    m_OriginalHandlingData[91].fTurnMass = 1354.199951f;
    m_OriginalHandlingData[91].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[91].vecCenterOfMass = CVector(0.000000f, -0.200000f, -0.350000f);
    m_OriginalHandlingData[91].uiPercentSubmerged = 70;
    m_OriginalHandlingData[91].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[91].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[91].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[91].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[91].Transmission.uiHandlingFlags = 17039396;
    m_OriginalHandlingData[91].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[91].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[91].Transmission.fMaxVelocity = 60.000000f;
    m_OriginalHandlingData[91].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[91].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[91].bABS = false;
    m_OriginalHandlingData[91].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[91].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[91].fTractionBias = 0.500000f;
    m_OriginalHandlingData[91].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[91].fSuspensionDamping = 0.140000f;
    m_OriginalHandlingData[91].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[91].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[91].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[91].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[91].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[91].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[91].uiModelFlags = 0x1300;
    m_OriginalHandlingData[91].uiHandlingFlags = 0x1040024;
    m_OriginalHandlingData[91].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[91].uiMonetary = 9000;
    m_OriginalHandlingData[91].ucHeadLight = 1;
    m_OriginalHandlingData[91].ucTailLight = 1;
    m_OriginalHandlingData[91].ucAnimGroup = 0;

    m_OriginalHandlingData[92].iVehicleID = 92;
    m_OriginalHandlingData[92].fMass = 2000.000000f;
    m_OriginalHandlingData[92].fTurnMass = 5000.000000f;
    m_OriginalHandlingData[92].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[92].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[92].uiPercentSubmerged = 70;
    m_OriginalHandlingData[92].fTractionMultiplier = 0.900000f;
    m_OriginalHandlingData[92].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[92].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[92].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[92].Transmission.uiHandlingFlags = 20185093;
    m_OriginalHandlingData[92].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[92].Transmission.fEngineInertia = 90.000000f;
    m_OriginalHandlingData[92].Transmission.fMaxVelocity = 70.000000f;
    m_OriginalHandlingData[92].fBrakeDeceleration = 15.000000f;
    m_OriginalHandlingData[92].fBrakeBias = 0.200000f;
    m_OriginalHandlingData[92].bABS = false;
    m_OriginalHandlingData[92].fSteeringLock = 50.000000f;
    m_OriginalHandlingData[92].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[92].fTractionBias = 0.500000f;
    m_OriginalHandlingData[92].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[92].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[92].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[92].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[92].fSuspensionLowerLimit = -0.050000f;
    m_OriginalHandlingData[92].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[92].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[92].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[92].uiModelFlags = 0x201310;
    m_OriginalHandlingData[92].uiHandlingFlags = 0x1340005;
    m_OriginalHandlingData[92].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[92].uiMonetary = 9000;
    m_OriginalHandlingData[92].ucHeadLight = 1;
    m_OriginalHandlingData[92].ucTailLight = 1;
    m_OriginalHandlingData[92].ucAnimGroup = 28;

    m_OriginalHandlingData[93].iVehicleID = 93;
    m_OriginalHandlingData[93].fMass = 8500.000000f;
    m_OriginalHandlingData[93].fTurnMass = 48804.199219f;
    m_OriginalHandlingData[93].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[93].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.200000f);
    m_OriginalHandlingData[93].uiPercentSubmerged = 90;
    m_OriginalHandlingData[93].fTractionMultiplier = 0.880000f;
    m_OriginalHandlingData[93].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[93].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[93].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[93].Transmission.uiHandlingFlags = 32;
    m_OriginalHandlingData[93].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[93].Transmission.fEngineInertia = 80.000000f;
    m_OriginalHandlingData[93].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[93].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[93].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[93].bABS = false;
    m_OriginalHandlingData[93].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[93].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[93].fTractionBias = 0.460000f;
    m_OriginalHandlingData[93].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[93].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[93].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[93].fSuspensionUpperLimit = 0.470000f;
    m_OriginalHandlingData[93].fSuspensionLowerLimit = -0.110000f;
    m_OriginalHandlingData[93].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[93].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[93].fCollisionDamageMultiplier = 0.430000f;
    m_OriginalHandlingData[93].uiModelFlags = 0x12C008;
    m_OriginalHandlingData[93].uiHandlingFlags = 0x20;
    m_OriginalHandlingData[93].fSeatOffsetDistance = 1.200000f;
    m_OriginalHandlingData[93].uiMonetary = 10000;
    m_OriginalHandlingData[93].ucHeadLight = 0;
    m_OriginalHandlingData[93].ucTailLight = 1;
    m_OriginalHandlingData[93].ucAnimGroup = 20;

    m_OriginalHandlingData[94].iVehicleID = 94;
    m_OriginalHandlingData[94].fMass = 1600.000000f;
    m_OriginalHandlingData[94].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[94].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[94].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.150000f);
    m_OriginalHandlingData[94].uiPercentSubmerged = 75;
    m_OriginalHandlingData[94].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[94].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[94].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[94].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[94].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[94].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[94].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[94].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[94].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[94].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[94].bABS = false;
    m_OriginalHandlingData[94].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[94].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[94].fTractionBias = 0.500000f;
    m_OriginalHandlingData[94].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[94].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[94].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[94].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[94].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[94].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[94].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[94].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[94].uiModelFlags = 0x40002800;
    m_OriginalHandlingData[94].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[94].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[94].uiMonetary = 35000;
    m_OriginalHandlingData[94].ucHeadLight = 1;
    m_OriginalHandlingData[94].ucTailLight = 1;
    m_OriginalHandlingData[94].ucAnimGroup = 19;

    m_OriginalHandlingData[95].iVehicleID = 95;
    m_OriginalHandlingData[95].fMass = 1800.000000f;
    m_OriginalHandlingData[95].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[95].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[95].vecCenterOfMass = CVector(0.000000f, -0.400000f, -0.200000f);
    m_OriginalHandlingData[95].uiPercentSubmerged = 70;
    m_OriginalHandlingData[95].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[95].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[95].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[95].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[95].Transmission.uiHandlingFlags = 37814272;
    m_OriginalHandlingData[95].Transmission.fEngineAcceleration = 9.200000f;
    m_OriginalHandlingData[95].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[95].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[95].fBrakeDeceleration = 6.500000f;
    m_OriginalHandlingData[95].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[95].bABS = false;
    m_OriginalHandlingData[95].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[95].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[95].fTractionBias = 0.560000f;
    m_OriginalHandlingData[95].fSuspensionForceLevel = 0.500000f;
    m_OriginalHandlingData[95].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[95].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[95].fSuspensionUpperLimit = 0.000000f;
    m_OriginalHandlingData[95].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[95].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[95].fSuspensionAntiDiveMultiplier = 0.600000f;
    m_OriginalHandlingData[95].fCollisionDamageMultiplier = 0.410000f;
    m_OriginalHandlingData[95].uiModelFlags = 0x40002004;
    m_OriginalHandlingData[95].uiHandlingFlags = 0x2410000;
    m_OriginalHandlingData[95].fSeatOffsetDistance = 0.210000f;
    m_OriginalHandlingData[95].uiMonetary = 30000;
    m_OriginalHandlingData[95].ucHeadLight = 1;
    m_OriginalHandlingData[95].ucTailLight = 1;
    m_OriginalHandlingData[95].ucAnimGroup = 1;

    m_OriginalHandlingData[96].iVehicleID = 96;
    m_OriginalHandlingData[96].fMass = 1950.000000f;
    m_OriginalHandlingData[96].fTurnMass = 4712.500000f;
    m_OriginalHandlingData[96].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[96].vecCenterOfMass = CVector(0.000000f, 0.100000f, 0.000000f);
    m_OriginalHandlingData[96].uiPercentSubmerged = 70;
    m_OriginalHandlingData[96].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[96].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[96].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[96].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[96].Transmission.uiHandlingFlags = 33619968;
    m_OriginalHandlingData[96].Transmission.fEngineAcceleration = 16.000000f;
    m_OriginalHandlingData[96].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[96].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[96].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[96].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[96].bABS = false;
    m_OriginalHandlingData[96].fSteeringLock = 28.000000f;
    m_OriginalHandlingData[96].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[96].fTractionBias = 0.500000f;
    m_OriginalHandlingData[96].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[96].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[96].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[96].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[96].fSuspensionLowerLimit = -0.140000f;
    m_OriginalHandlingData[96].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[96].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[96].fCollisionDamageMultiplier = 0.420000f;
    m_OriginalHandlingData[96].uiModelFlags = 0x40002000;
    m_OriginalHandlingData[96].uiHandlingFlags = 0x2010000;
    m_OriginalHandlingData[96].fSeatOffsetDistance = 0.360000f;
    m_OriginalHandlingData[96].uiMonetary = 19000;
    m_OriginalHandlingData[96].ucHeadLight = 1;
    m_OriginalHandlingData[96].ucTailLight = 3;
    m_OriginalHandlingData[96].ucAnimGroup = 0;

    m_OriginalHandlingData[97].iVehicleID = 97;
    m_OriginalHandlingData[97].fMass = 1500.000000f;
    m_OriginalHandlingData[97].fTurnMass = 2500.000000f;
    m_OriginalHandlingData[97].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[97].vecCenterOfMass = CVector(0.000000f, -0.200000f, 0.100000f);
    m_OriginalHandlingData[97].uiPercentSubmerged = 70;
    m_OriginalHandlingData[97].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[97].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[97].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[97].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[97].Transmission.uiHandlingFlags = 302055424;
    m_OriginalHandlingData[97].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[97].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[97].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[97].fBrakeDeceleration = 8.170000f;
    m_OriginalHandlingData[97].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[97].bABS = false;
    m_OriginalHandlingData[97].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[97].fTractionLoss = 0.840000f;
    m_OriginalHandlingData[97].fTractionBias = 0.530000f;
    m_OriginalHandlingData[97].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[97].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[97].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[97].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[97].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[97].fSuspensionFrontRearBias = 0.440000f;
    m_OriginalHandlingData[97].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[97].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[97].uiModelFlags = 0x40202000;
    m_OriginalHandlingData[97].uiHandlingFlags = 0x12010000;
    m_OriginalHandlingData[97].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[97].uiMonetary = 19000;
    m_OriginalHandlingData[97].ucHeadLight = 1;
    m_OriginalHandlingData[97].ucTailLight = 1;
    m_OriginalHandlingData[97].ucAnimGroup = 0;

    m_OriginalHandlingData[98].iVehicleID = 98;
    m_OriginalHandlingData[98].fMass = 5500.000000f;
    m_OriginalHandlingData[98].fTurnMass = 65000.000000f;
    m_OriginalHandlingData[98].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[98].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[98].uiPercentSubmerged = 90;
    m_OriginalHandlingData[98].fTractionMultiplier = 0.580000f;
    m_OriginalHandlingData[98].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[98].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[98].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[98].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[98].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[98].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[98].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[98].fBrakeDeceleration = 3.170000f;
    m_OriginalHandlingData[98].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[98].bABS = false;
    m_OriginalHandlingData[98].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[98].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[98].fTractionBias = 0.500000f;
    m_OriginalHandlingData[98].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[98].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[98].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[98].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[98].fSuspensionLowerLimit = 0.000000f;
    m_OriginalHandlingData[98].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[98].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[98].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[98].uiModelFlags = 0x8;
    m_OriginalHandlingData[98].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[98].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[98].uiMonetary = 5000;
    m_OriginalHandlingData[98].ucHeadLight = 0;
    m_OriginalHandlingData[98].ucTailLight = 1;
    m_OriginalHandlingData[98].ucAnimGroup = 2;

    m_OriginalHandlingData[99].iVehicleID = 99;
    m_OriginalHandlingData[99].fMass = 5500.000000f;
    m_OriginalHandlingData[99].fTurnMass = 65000.000000f;
    m_OriginalHandlingData[99].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[99].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[99].uiPercentSubmerged = 90;
    m_OriginalHandlingData[99].fTractionMultiplier = 0.580000f;
    m_OriginalHandlingData[99].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[99].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[99].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[99].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[99].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[99].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[99].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[99].fBrakeDeceleration = 3.170000f;
    m_OriginalHandlingData[99].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[99].bABS = false;
    m_OriginalHandlingData[99].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[99].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[99].fTractionBias = 0.500000f;
    m_OriginalHandlingData[99].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[99].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[99].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[99].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[99].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[99].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[99].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[99].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[99].uiModelFlags = 0x8;
    m_OriginalHandlingData[99].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[99].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[99].uiMonetary = 5000;
    m_OriginalHandlingData[99].ucHeadLight = 0;
    m_OriginalHandlingData[99].ucTailLight = 1;
    m_OriginalHandlingData[99].ucAnimGroup = 2;

    m_OriginalHandlingData[100].iVehicleID = 100;
    m_OriginalHandlingData[100].fMass = 1800.000000f;
    m_OriginalHandlingData[100].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[100].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[100].vecCenterOfMass = CVector(0.000000f, 0.300000f, 0.000000f);
    m_OriginalHandlingData[100].uiPercentSubmerged = 70;
    m_OriginalHandlingData[100].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[100].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[100].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[100].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[100].Transmission.uiHandlingFlags = 2;
    m_OriginalHandlingData[100].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[100].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[100].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[100].fBrakeDeceleration = 5.400000f;
    m_OriginalHandlingData[100].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[100].bABS = false;
    m_OriginalHandlingData[100].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[100].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[100].fTractionBias = 0.500000f;
    m_OriginalHandlingData[100].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[100].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[100].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[100].fSuspensionUpperLimit = 0.320000f;
    m_OriginalHandlingData[100].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[100].fSuspensionFrontRearBias = 0.560000f;
    m_OriginalHandlingData[100].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[100].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[100].uiModelFlags = 0x0;
    m_OriginalHandlingData[100].uiHandlingFlags = 0x2;
    m_OriginalHandlingData[100].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[100].uiMonetary = 19000;
    m_OriginalHandlingData[100].ucHeadLight = 0;
    m_OriginalHandlingData[100].ucTailLight = 3;
    m_OriginalHandlingData[100].ucAnimGroup = 0;

    m_OriginalHandlingData[101].iVehicleID = 101;
    m_OriginalHandlingData[101].fMass = 1200.000000f;
    m_OriginalHandlingData[101].fTurnMass = 2500.000000f;
    m_OriginalHandlingData[101].fDragCoeff = 1.800000f;
    m_OriginalHandlingData[101].vecCenterOfMass = CVector(0.000000f, -0.150000f, -0.200000f);
    m_OriginalHandlingData[101].uiPercentSubmerged = 70;
    m_OriginalHandlingData[101].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[101].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[101].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[101].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[101].Transmission.uiHandlingFlags = 2113536;
    m_OriginalHandlingData[101].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[101].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[101].Transmission.fMaxVelocity = 230.000000f;
    m_OriginalHandlingData[101].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[101].fBrakeBias = 0.580000f;
    m_OriginalHandlingData[101].bABS = false;
    m_OriginalHandlingData[101].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[101].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[101].fTractionBias = 0.480000f;
    m_OriginalHandlingData[101].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[101].fSuspensionDamping = 0.130000f;
    m_OriginalHandlingData[101].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[101].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[101].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[101].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[101].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[101].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[101].uiModelFlags = 0xC0002004;
    m_OriginalHandlingData[101].uiHandlingFlags = 0x204000;
    m_OriginalHandlingData[101].fSeatOffsetDistance = 0.150000f;
    m_OriginalHandlingData[101].uiMonetary = 105000;
    m_OriginalHandlingData[101].ucHeadLight = 0;
    m_OriginalHandlingData[101].ucTailLight = 0;
    m_OriginalHandlingData[101].ucAnimGroup = 1;

    m_OriginalHandlingData[102].iVehicleID = 102;
    m_OriginalHandlingData[102].fMass = 1600.000000f;
    m_OriginalHandlingData[102].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[102].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[102].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[102].uiPercentSubmerged = 70;
    m_OriginalHandlingData[102].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[102].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[102].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[102].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[102].Transmission.uiHandlingFlags = 268468228;
    m_OriginalHandlingData[102].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[102].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[102].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[102].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[102].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[102].bABS = false;
    m_OriginalHandlingData[102].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[102].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[102].fTractionBias = 0.520000f;
    m_OriginalHandlingData[102].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[102].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[102].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[102].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[102].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[102].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[102].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[102].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[102].uiModelFlags = 0x40280000;
    m_OriginalHandlingData[102].uiHandlingFlags = 0x10008004;
    m_OriginalHandlingData[102].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[102].uiMonetary = 19000;
    m_OriginalHandlingData[102].ucHeadLight = 1;
    m_OriginalHandlingData[102].ucTailLight = 1;
    m_OriginalHandlingData[102].ucAnimGroup = 0;

    m_OriginalHandlingData[103].iVehicleID = 103;
    m_OriginalHandlingData[103].fMass = 1700.000000f;
    m_OriginalHandlingData[103].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[103].fDragCoeff = 2.700000f;
    m_OriginalHandlingData[103].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.050000f);
    m_OriginalHandlingData[103].uiPercentSubmerged = 75;
    m_OriginalHandlingData[103].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[103].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[103].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[103].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[103].Transmission.uiHandlingFlags = 1064964;
    m_OriginalHandlingData[103].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[103].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[103].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[103].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[103].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[103].bABS = false;
    m_OriginalHandlingData[103].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[103].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[103].fTractionBias = 0.500000f;
    m_OriginalHandlingData[103].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[103].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[103].fSuspensionHighSpdDamping = 3.000000f;
    m_OriginalHandlingData[103].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[103].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[103].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[103].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[103].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[103].uiModelFlags = 0x200040;
    m_OriginalHandlingData[103].uiHandlingFlags = 0x104004;
    m_OriginalHandlingData[103].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[103].uiMonetary = 26000;
    m_OriginalHandlingData[103].ucHeadLight = 0;
    m_OriginalHandlingData[103].ucTailLight = 1;
    m_OriginalHandlingData[103].ucAnimGroup = 0;

    m_OriginalHandlingData[104].iVehicleID = 104;
    m_OriginalHandlingData[104].fMass = 1700.000000f;
    m_OriginalHandlingData[104].fTurnMass = 4108.299805f;
    m_OriginalHandlingData[104].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[104].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[104].uiPercentSubmerged = 85;
    m_OriginalHandlingData[104].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[104].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[104].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[104].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[104].Transmission.uiHandlingFlags = 7340032;
    m_OriginalHandlingData[104].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[104].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[104].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[104].fBrakeDeceleration = 6.200000f;
    m_OriginalHandlingData[104].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[104].bABS = false;
    m_OriginalHandlingData[104].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[104].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[104].fTractionBias = 0.510000f;
    m_OriginalHandlingData[104].fSuspensionForceLevel = 1.700000f;
    m_OriginalHandlingData[104].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[104].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[104].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[104].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[104].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[104].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[104].fCollisionDamageMultiplier = 0.230000f;
    m_OriginalHandlingData[104].uiModelFlags = 0x20;
    m_OriginalHandlingData[104].uiHandlingFlags = 0x700000;
    m_OriginalHandlingData[104].fSeatOffsetDistance = 0.270000f;
    m_OriginalHandlingData[104].uiMonetary = 25000;
    m_OriginalHandlingData[104].ucHeadLight = 0;
    m_OriginalHandlingData[104].ucTailLight = 1;
    m_OriginalHandlingData[104].ucAnimGroup = 0;

    m_OriginalHandlingData[105].iVehicleID = 105;
    m_OriginalHandlingData[105].fMass = 1700.000000f;
    m_OriginalHandlingData[105].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[105].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[105].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.050000f);
    m_OriginalHandlingData[105].uiPercentSubmerged = 75;
    m_OriginalHandlingData[105].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[105].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[105].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[105].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[105].Transmission.uiHandlingFlags = 8388608;
    m_OriginalHandlingData[105].Transmission.fEngineAcceleration = 8.800000f;
    m_OriginalHandlingData[105].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[105].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[105].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[105].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[105].bABS = false;
    m_OriginalHandlingData[105].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[105].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[105].fTractionBias = 0.520000f;
    m_OriginalHandlingData[105].fSuspensionForceLevel = 0.450000f;
    m_OriginalHandlingData[105].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[105].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[105].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[105].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[105].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[105].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[105].fCollisionDamageMultiplier = 0.450000f;
    m_OriginalHandlingData[105].uiModelFlags = 0x0;
    m_OriginalHandlingData[105].uiHandlingFlags = 0x800000;
    m_OriginalHandlingData[105].fSeatOffsetDistance = 0.180000f;
    m_OriginalHandlingData[105].uiMonetary = 20000;
    m_OriginalHandlingData[105].ucHeadLight = 2;
    m_OriginalHandlingData[105].ucTailLight = 1;
    m_OriginalHandlingData[105].ucAnimGroup = 0;

    m_OriginalHandlingData[106].iVehicleID = 106;
    m_OriginalHandlingData[106].fMass = 1800.000000f;
    m_OriginalHandlingData[106].fTurnMass = 4350.000000f;
    m_OriginalHandlingData[106].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[106].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[106].uiPercentSubmerged = 70;
    m_OriginalHandlingData[106].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[106].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[106].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[106].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[106].Transmission.uiHandlingFlags = 2;
    m_OriginalHandlingData[106].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[106].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[106].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[106].fBrakeDeceleration = 5.400000f;
    m_OriginalHandlingData[106].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[106].bABS = false;
    m_OriginalHandlingData[106].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[106].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[106].fTractionBias = 0.490000f;
    m_OriginalHandlingData[106].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[106].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[106].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[106].fSuspensionUpperLimit = 0.320000f;
    m_OriginalHandlingData[106].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[106].fSuspensionFrontRearBias = 0.540000f;
    m_OriginalHandlingData[106].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[106].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[106].uiModelFlags = 0x0;
    m_OriginalHandlingData[106].uiHandlingFlags = 0x2;
    m_OriginalHandlingData[106].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[106].uiMonetary = 19000;
    m_OriginalHandlingData[106].ucHeadLight = 0;
    m_OriginalHandlingData[106].ucTailLight = 3;
    m_OriginalHandlingData[106].ucAnimGroup = 0;

    m_OriginalHandlingData[107].iVehicleID = 107;
    m_OriginalHandlingData[107].fMass = 1600.000000f;
    m_OriginalHandlingData[107].fTurnMass = 3300.000000f;
    m_OriginalHandlingData[107].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[107].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[107].uiPercentSubmerged = 70;
    m_OriginalHandlingData[107].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[107].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[107].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[107].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[107].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[107].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[107].Transmission.fEngineInertia = 7.000000f;
    m_OriginalHandlingData[107].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[107].fBrakeDeceleration = 5.400000f;
    m_OriginalHandlingData[107].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[107].bABS = false;
    m_OriginalHandlingData[107].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[107].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[107].fTractionBias = 0.540000f;
    m_OriginalHandlingData[107].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[107].fSuspensionDamping = 0.140000f;
    m_OriginalHandlingData[107].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[107].fSuspensionUpperLimit = 0.320000f;
    m_OriginalHandlingData[107].fSuspensionLowerLimit = -0.140000f;
    m_OriginalHandlingData[107].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[107].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[107].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[107].uiModelFlags = 0x0;
    m_OriginalHandlingData[107].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[107].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[107].uiMonetary = 19000;
    m_OriginalHandlingData[107].ucHeadLight = 0;
    m_OriginalHandlingData[107].ucTailLight = 3;
    m_OriginalHandlingData[107].ucAnimGroup = 0;

    m_OriginalHandlingData[108].iVehicleID = 108;
    m_OriginalHandlingData[108].fMass = 1700.000000f;
    m_OriginalHandlingData[108].fTurnMass = 4166.399902f;
    m_OriginalHandlingData[108].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[108].vecCenterOfMass = CVector(0.000000f, 0.150000f, 0.000000f);
    m_OriginalHandlingData[108].uiPercentSubmerged = 70;
    m_OriginalHandlingData[108].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[108].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[108].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[108].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[108].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[108].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[108].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[108].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[108].fBrakeDeceleration = 8.170000f;
    m_OriginalHandlingData[108].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[108].bABS = false;
    m_OriginalHandlingData[108].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[108].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[108].fTractionBias = 0.520000f;
    m_OriginalHandlingData[108].fSuspensionForceLevel = 0.700000f;
    m_OriginalHandlingData[108].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[108].fSuspensionHighSpdDamping = 3.000000f;
    m_OriginalHandlingData[108].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[108].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[108].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[108].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[108].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[108].uiModelFlags = 0x40000004;
    m_OriginalHandlingData[108].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[108].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[108].uiMonetary = 19000;
    m_OriginalHandlingData[108].ucHeadLight = 1;
    m_OriginalHandlingData[108].ucTailLight = 1;
    m_OriginalHandlingData[108].ucAnimGroup = 1;

    m_OriginalHandlingData[109].iVehicleID = 109;
    m_OriginalHandlingData[109].fMass = 1600.000000f;
    m_OriginalHandlingData[109].fTurnMass = 3550.000000f;
    m_OriginalHandlingData[109].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[109].vecCenterOfMass = CVector(0.000000f, 0.300000f, 0.000000f);
    m_OriginalHandlingData[109].uiPercentSubmerged = 70;
    m_OriginalHandlingData[109].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[109].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[109].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[109].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[109].Transmission.uiHandlingFlags = 1;
    m_OriginalHandlingData[109].Transmission.fEngineAcceleration = 6.800000f;
    m_OriginalHandlingData[109].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[109].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[109].fBrakeDeceleration = 5.400000f;
    m_OriginalHandlingData[109].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[109].bABS = false;
    m_OriginalHandlingData[109].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[109].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[109].fTractionBias = 0.520000f;
    m_OriginalHandlingData[109].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[109].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[109].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[109].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[109].fSuspensionLowerLimit = -0.120000f;
    m_OriginalHandlingData[109].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[109].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[109].fCollisionDamageMultiplier = 0.540000f;
    m_OriginalHandlingData[109].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[109].uiHandlingFlags = 0x1;
    m_OriginalHandlingData[109].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[109].uiMonetary = 19000;
    m_OriginalHandlingData[109].ucHeadLight = 0;
    m_OriginalHandlingData[109].ucTailLight = 3;
    m_OriginalHandlingData[109].ucAnimGroup = 0;

    m_OriginalHandlingData[110].iVehicleID = 110;
    m_OriginalHandlingData[110].fMass = 1800.000000f;
    m_OriginalHandlingData[110].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[110].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[110].vecCenterOfMass = CVector(0.000000f, 0.200000f, -0.100000f);
    m_OriginalHandlingData[110].uiPercentSubmerged = 75;
    m_OriginalHandlingData[110].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[110].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[110].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[110].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[110].Transmission.uiHandlingFlags = 4194305;
    m_OriginalHandlingData[110].Transmission.fEngineAcceleration = 8.800000f;
    m_OriginalHandlingData[110].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[110].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[110].fBrakeDeceleration = 9.000000f;
    m_OriginalHandlingData[110].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[110].bABS = false;
    m_OriginalHandlingData[110].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[110].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[110].fTractionBias = 0.490000f;
    m_OriginalHandlingData[110].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[110].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[110].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[110].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[110].fSuspensionLowerLimit = -0.080000f;
    m_OriginalHandlingData[110].fSuspensionFrontRearBias = 0.540000f;
    m_OriginalHandlingData[110].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[110].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[110].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[110].uiHandlingFlags = 0x400001;
    m_OriginalHandlingData[110].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[110].uiMonetary = 35000;
    m_OriginalHandlingData[110].ucHeadLight = 0;
    m_OriginalHandlingData[110].ucTailLight = 1;
    m_OriginalHandlingData[110].ucAnimGroup = 0;

    m_OriginalHandlingData[111].iVehicleID = 111;
    m_OriginalHandlingData[111].fMass = 2600.000000f;
    m_OriginalHandlingData[111].fTurnMass = 8666.700195f;
    m_OriginalHandlingData[111].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[111].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[111].uiPercentSubmerged = 80;
    m_OriginalHandlingData[111].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[111].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[111].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[111].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[111].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[111].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[111].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[111].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[111].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[111].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[111].bABS = false;
    m_OriginalHandlingData[111].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[111].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[111].fTractionBias = 0.460000f;
    m_OriginalHandlingData[111].fSuspensionForceLevel = 1.800000f;
    m_OriginalHandlingData[111].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[111].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[111].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[111].fSuspensionLowerLimit = -0.180000f;
    m_OriginalHandlingData[111].fSuspensionFrontRearBias = 0.250000f;
    m_OriginalHandlingData[111].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[111].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[111].uiModelFlags = 0x1;
    m_OriginalHandlingData[111].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[111].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[111].uiMonetary = 20000;
    m_OriginalHandlingData[111].ucHeadLight = 0;
    m_OriginalHandlingData[111].ucTailLight = 3;
    m_OriginalHandlingData[111].ucAnimGroup = 13;

    m_OriginalHandlingData[112].iVehicleID = 112;
    m_OriginalHandlingData[112].fMass = 3000.000000f;
    m_OriginalHandlingData[112].fTurnMass = 6000.000000f;
    m_OriginalHandlingData[112].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[112].vecCenterOfMass = CVector(0.000000f, 0.350000f, 0.000000f);
    m_OriginalHandlingData[112].uiPercentSubmerged = 80;
    m_OriginalHandlingData[112].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[112].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[112].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[112].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[112].Transmission.uiHandlingFlags = 5260288;
    m_OriginalHandlingData[112].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[112].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[112].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[112].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[112].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[112].bABS = false;
    m_OriginalHandlingData[112].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[112].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[112].fTractionBias = 0.400000f;
    m_OriginalHandlingData[112].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[112].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[112].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[112].fSuspensionUpperLimit = 0.240000f;
    m_OriginalHandlingData[112].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[112].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[112].fSuspensionAntiDiveMultiplier = 0.500000f;
    m_OriginalHandlingData[112].fCollisionDamageMultiplier = 0.300000f;
    m_OriginalHandlingData[112].uiModelFlags = 0x20200020;
    m_OriginalHandlingData[112].uiHandlingFlags = 0x504400;
    m_OriginalHandlingData[112].fSeatOffsetDistance = 0.440000f;
    m_OriginalHandlingData[112].uiMonetary = 40000;
    m_OriginalHandlingData[112].ucHeadLight = 0;
    m_OriginalHandlingData[112].ucTailLight = 1;
    m_OriginalHandlingData[112].ucAnimGroup = 0;

    m_OriginalHandlingData[113].iVehicleID = 113;
    m_OriginalHandlingData[113].fMass = 1500.000000f;
    m_OriginalHandlingData[113].fTurnMass = 3500.000000f;
    m_OriginalHandlingData[113].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[113].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.200000f);
    m_OriginalHandlingData[113].uiPercentSubmerged = 75;
    m_OriginalHandlingData[113].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[113].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[113].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[113].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[113].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[113].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[113].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[113].Transmission.fMaxVelocity = 180.000000f;
    m_OriginalHandlingData[113].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[113].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[113].bABS = false;
    m_OriginalHandlingData[113].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[113].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[113].fTractionBias = 0.500000f;
    m_OriginalHandlingData[113].fSuspensionForceLevel = 0.650000f;
    m_OriginalHandlingData[113].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[113].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[113].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[113].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[113].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[113].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[113].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[113].uiModelFlags = 0x40282804;
    m_OriginalHandlingData[113].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[113].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[113].uiMonetary = 35000;
    m_OriginalHandlingData[113].ucHeadLight = 1;
    m_OriginalHandlingData[113].ucTailLight = 1;
    m_OriginalHandlingData[113].ucAnimGroup = 1;

    m_OriginalHandlingData[114].iVehicleID = 114;
    m_OriginalHandlingData[114].fMass = 5000.000000f;
    m_OriginalHandlingData[114].fTurnMass = 20000.000000f;
    m_OriginalHandlingData[114].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[114].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.350000f);
    m_OriginalHandlingData[114].uiPercentSubmerged = 80;
    m_OriginalHandlingData[114].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[114].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[114].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[114].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[114].Transmission.uiHandlingFlags = 19923013;
    m_OriginalHandlingData[114].Transmission.fEngineAcceleration = 18.000000f;
    m_OriginalHandlingData[114].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[114].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[114].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[114].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[114].bABS = false;
    m_OriginalHandlingData[114].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[114].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[114].fTractionBias = 0.550000f;
    m_OriginalHandlingData[114].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[114].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[114].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[114].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[114].fSuspensionLowerLimit = -0.300000f;
    m_OriginalHandlingData[114].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[114].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[114].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[114].uiModelFlags = 0x28;
    m_OriginalHandlingData[114].uiHandlingFlags = 0x1300045;
    m_OriginalHandlingData[114].fSeatOffsetDistance = 0.440000f;
    m_OriginalHandlingData[114].uiMonetary = 40000;
    m_OriginalHandlingData[114].ucHeadLight = 0;
    m_OriginalHandlingData[114].ucTailLight = 1;
    m_OriginalHandlingData[114].ucAnimGroup = 20;

    m_OriginalHandlingData[115].iVehicleID = 115;
    m_OriginalHandlingData[115].fMass = 5000.000000f;
    m_OriginalHandlingData[115].fTurnMass = 20000.000000f;
    m_OriginalHandlingData[115].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[115].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.350000f);
    m_OriginalHandlingData[115].uiPercentSubmerged = 80;
    m_OriginalHandlingData[115].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[115].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[115].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[115].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[115].Transmission.uiHandlingFlags = 19923013;
    m_OriginalHandlingData[115].Transmission.fEngineAcceleration = 18.000000f;
    m_OriginalHandlingData[115].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[115].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[115].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[115].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[115].bABS = false;
    m_OriginalHandlingData[115].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[115].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[115].fTractionBias = 0.550000f;
    m_OriginalHandlingData[115].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[115].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[115].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[115].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[115].fSuspensionLowerLimit = -0.300000f;
    m_OriginalHandlingData[115].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[115].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[115].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[115].uiModelFlags = 0x28;
    m_OriginalHandlingData[115].uiHandlingFlags = 0x1300045;
    m_OriginalHandlingData[115].fSeatOffsetDistance = 0.440000f;
    m_OriginalHandlingData[115].uiMonetary = 40000;
    m_OriginalHandlingData[115].ucHeadLight = 0;
    m_OriginalHandlingData[115].ucTailLight = 1;
    m_OriginalHandlingData[115].ucAnimGroup = 20;

    m_OriginalHandlingData[116].iVehicleID = 116;
    m_OriginalHandlingData[116].fMass = 1400.000000f;
    m_OriginalHandlingData[116].fTurnMass = 2998.300049f;
    m_OriginalHandlingData[116].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[116].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.300000f);
    m_OriginalHandlingData[116].uiPercentSubmerged = 75;
    m_OriginalHandlingData[116].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[116].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[116].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[116].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[116].Transmission.uiHandlingFlags = 67108865;
    m_OriginalHandlingData[116].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[116].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[116].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[116].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[116].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[116].bABS = false;
    m_OriginalHandlingData[116].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[116].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[116].fTractionBias = 0.470000f;
    m_OriginalHandlingData[116].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[116].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[116].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[116].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[116].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[116].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[116].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[116].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[116].uiModelFlags = 0xC0002800;
    m_OriginalHandlingData[116].uiHandlingFlags = 0x4000001;
    m_OriginalHandlingData[116].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[116].uiMonetary = 35000;
    m_OriginalHandlingData[116].ucHeadLight = 1;
    m_OriginalHandlingData[116].ucTailLight = 1;
    m_OriginalHandlingData[116].ucAnimGroup = 0;

    m_OriginalHandlingData[117].iVehicleID = 117;
    m_OriginalHandlingData[117].fMass = 1500.000000f;
    m_OriginalHandlingData[117].fTurnMass = 3600.000000f;
    m_OriginalHandlingData[117].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[117].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.050000f);
    m_OriginalHandlingData[117].uiPercentSubmerged = 75;
    m_OriginalHandlingData[117].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[117].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[117].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[117].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[117].Transmission.uiHandlingFlags = 67108864;
    m_OriginalHandlingData[117].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[117].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[117].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[117].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[117].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[117].bABS = false;
    m_OriginalHandlingData[117].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[117].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[117].fTractionBias = 0.500000f;
    m_OriginalHandlingData[117].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[117].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[117].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[117].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[117].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[117].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[117].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[117].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[117].uiModelFlags = 0xC0002804;
    m_OriginalHandlingData[117].uiHandlingFlags = 0x4000000;
    m_OriginalHandlingData[117].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[117].uiMonetary = 35000;
    m_OriginalHandlingData[117].ucHeadLight = 1;
    m_OriginalHandlingData[117].ucTailLight = 1;
    m_OriginalHandlingData[117].ucAnimGroup = 1;

    m_OriginalHandlingData[118].iVehicleID = 118;
    m_OriginalHandlingData[118].fMass = 1400.000000f;
    m_OriginalHandlingData[118].fTurnMass = 3400.000000f;
    m_OriginalHandlingData[118].fDragCoeff = 2.400000f;
    m_OriginalHandlingData[118].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.100000f);
    m_OriginalHandlingData[118].uiPercentSubmerged = 75;
    m_OriginalHandlingData[118].fTractionMultiplier = 0.800000f;
    m_OriginalHandlingData[118].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[118].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[118].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[118].Transmission.uiHandlingFlags = 67108866;
    m_OriginalHandlingData[118].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[118].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[118].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[118].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[118].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[118].bABS = false;
    m_OriginalHandlingData[118].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[118].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[118].fTractionBias = 0.500000f;
    m_OriginalHandlingData[118].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[118].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[118].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[118].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[118].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[118].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[118].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[118].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[118].uiModelFlags = 0x2800;
    m_OriginalHandlingData[118].uiHandlingFlags = 0x4000002;
    m_OriginalHandlingData[118].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[118].uiMonetary = 35000;
    m_OriginalHandlingData[118].ucHeadLight = 1;
    m_OriginalHandlingData[118].ucTailLight = 1;
    m_OriginalHandlingData[118].ucAnimGroup = 0;

    m_OriginalHandlingData[119].iVehicleID = 119;
    m_OriginalHandlingData[119].fMass = 1800.000000f;
    m_OriginalHandlingData[119].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[119].fDragCoeff = 2.100000f;
    m_OriginalHandlingData[119].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.100000f);
    m_OriginalHandlingData[119].uiPercentSubmerged = 75;
    m_OriginalHandlingData[119].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[119].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[119].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[119].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[119].Transmission.uiHandlingFlags = 67108864;
    m_OriginalHandlingData[119].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[119].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[119].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[119].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[119].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[119].bABS = false;
    m_OriginalHandlingData[119].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[119].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[119].fTractionBias = 0.500000f;
    m_OriginalHandlingData[119].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[119].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[119].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[119].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[119].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[119].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[119].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[119].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[119].uiModelFlags = 0x2800;
    m_OriginalHandlingData[119].uiHandlingFlags = 0x4000000;
    m_OriginalHandlingData[119].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[119].uiMonetary = 35000;
    m_OriginalHandlingData[119].ucHeadLight = 1;
    m_OriginalHandlingData[119].ucTailLight = 1;
    m_OriginalHandlingData[119].ucAnimGroup = 0;

    m_OriginalHandlingData[120].iVehicleID = 120;
    m_OriginalHandlingData[120].fMass = 1500.000000f;
    m_OriginalHandlingData[120].fTurnMass = 3500.000000f;
    m_OriginalHandlingData[120].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[120].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.150000f);
    m_OriginalHandlingData[120].uiPercentSubmerged = 75;
    m_OriginalHandlingData[120].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[120].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[120].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[120].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[120].Transmission.uiHandlingFlags = 67108865;
    m_OriginalHandlingData[120].Transmission.fEngineAcceleration = 11.200000f;
    m_OriginalHandlingData[120].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[120].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[120].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[120].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[120].bABS = false;
    m_OriginalHandlingData[120].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[120].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[120].fTractionBias = 0.500000f;
    m_OriginalHandlingData[120].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[120].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[120].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[120].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[120].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[120].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[120].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[120].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[120].uiModelFlags = 0x40002804;
    m_OriginalHandlingData[120].uiHandlingFlags = 0x4000001;
    m_OriginalHandlingData[120].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[120].uiMonetary = 35000;
    m_OriginalHandlingData[120].ucHeadLight = 1;
    m_OriginalHandlingData[120].ucTailLight = 1;
    m_OriginalHandlingData[120].ucAnimGroup = 1;

    m_OriginalHandlingData[121].iVehicleID = 121;
    m_OriginalHandlingData[121].fMass = 100.000000f;
    m_OriginalHandlingData[121].fTurnMass = 24.100000f;
    m_OriginalHandlingData[121].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[121].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[121].uiPercentSubmerged = 70;
    m_OriginalHandlingData[121].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[121].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[121].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[121].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[121].Transmission.uiHandlingFlags = 64;
    m_OriginalHandlingData[121].Transmission.fEngineAcceleration = 14.000000f;
    m_OriginalHandlingData[121].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[121].Transmission.fMaxVelocity = 75.000000f;
    m_OriginalHandlingData[121].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[121].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[121].bABS = false;
    m_OriginalHandlingData[121].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[121].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[121].fTractionBias = 0.490000f;
    m_OriginalHandlingData[121].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[121].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[121].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[121].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[121].fSuspensionLowerLimit = -0.140000f;
    m_OriginalHandlingData[121].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[121].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[121].fCollisionDamageMultiplier = 0.050000f;
    m_OriginalHandlingData[121].uiModelFlags = 0x0;
    m_OriginalHandlingData[121].uiHandlingFlags = 0x40;
    m_OriginalHandlingData[121].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[121].uiMonetary = 500;
    m_OriginalHandlingData[121].ucHeadLight = 0;
    m_OriginalHandlingData[121].ucTailLight = 1;
    m_OriginalHandlingData[121].ucAnimGroup = 0;

    m_OriginalHandlingData[122].iVehicleID = 122;
    m_OriginalHandlingData[122].fMass = 1400.000000f;
    m_OriginalHandlingData[122].fTurnMass = 2998.300049f;
    m_OriginalHandlingData[122].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[122].vecCenterOfMass = CVector(0.000000f, 0.200000f, -0.100000f);
    m_OriginalHandlingData[122].uiPercentSubmerged = 75;
    m_OriginalHandlingData[122].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[122].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[122].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[122].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[122].Transmission.uiHandlingFlags = 67108865;
    m_OriginalHandlingData[122].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[122].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[122].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[122].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[122].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[122].bABS = false;
    m_OriginalHandlingData[122].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[122].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[122].fTractionBias = 0.500000f;
    m_OriginalHandlingData[122].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[122].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[122].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[122].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[122].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[122].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[122].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[122].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[122].uiModelFlags = 0x2804;
    m_OriginalHandlingData[122].uiHandlingFlags = 0x4000001;
    m_OriginalHandlingData[122].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[122].uiMonetary = 35000;
    m_OriginalHandlingData[122].ucHeadLight = 1;
    m_OriginalHandlingData[122].ucTailLight = 1;
    m_OriginalHandlingData[122].ucAnimGroup = 1;

    m_OriginalHandlingData[123].iVehicleID = 123;
    m_OriginalHandlingData[123].fMass = 1800.000000f;
    m_OriginalHandlingData[123].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[123].fDragCoeff = 2.300000f;
    m_OriginalHandlingData[123].vecCenterOfMass = CVector(0.000000f, -0.300000f, 0.000000f);
    m_OriginalHandlingData[123].uiPercentSubmerged = 75;
    m_OriginalHandlingData[123].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[123].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[123].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[123].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[123].Transmission.uiHandlingFlags = 302055424;
    m_OriginalHandlingData[123].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[123].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[123].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[123].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[123].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[123].bABS = false;
    m_OriginalHandlingData[123].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[123].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[123].fTractionBias = 0.520000f;
    m_OriginalHandlingData[123].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[123].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[123].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[123].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[123].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[123].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[123].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[123].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[123].uiModelFlags = 0x0;
    m_OriginalHandlingData[123].uiHandlingFlags = 0x12010000;
    m_OriginalHandlingData[123].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[123].uiMonetary = 35000;
    m_OriginalHandlingData[123].ucHeadLight = 1;
    m_OriginalHandlingData[123].ucTailLight = 1;
    m_OriginalHandlingData[123].ucAnimGroup = 0;

    m_OriginalHandlingData[124].iVehicleID = 124;
    m_OriginalHandlingData[124].fMass = 1500.000000f;
    m_OriginalHandlingData[124].fTurnMass = 2500.000000f;
    m_OriginalHandlingData[124].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[124].vecCenterOfMass = CVector(0.000000f, -0.600000f, 0.100000f);
    m_OriginalHandlingData[124].uiPercentSubmerged = 70;
    m_OriginalHandlingData[124].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[124].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[124].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[124].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[124].Transmission.uiHandlingFlags = 33619968;
    m_OriginalHandlingData[124].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[124].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[124].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[124].fBrakeDeceleration = 8.170000f;
    m_OriginalHandlingData[124].fBrakeBias = 0.520000f;
    m_OriginalHandlingData[124].bABS = false;
    m_OriginalHandlingData[124].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[124].fTractionLoss = 0.840000f;
    m_OriginalHandlingData[124].fTractionBias = 0.550000f;
    m_OriginalHandlingData[124].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[124].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[124].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[124].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[124].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[124].fSuspensionFrontRearBias = 0.300000f;
    m_OriginalHandlingData[124].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[124].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[124].uiModelFlags = 0x200000;
    m_OriginalHandlingData[124].uiHandlingFlags = 0x2010000;
    m_OriginalHandlingData[124].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[124].uiMonetary = 19000;
    m_OriginalHandlingData[124].ucHeadLight = 1;
    m_OriginalHandlingData[124].ucTailLight = 1;
    m_OriginalHandlingData[124].ucAnimGroup = 0;

    m_OriginalHandlingData[125].iVehicleID = 125;
    m_OriginalHandlingData[125].fMass = 1000.000000f;
    m_OriginalHandlingData[125].fTurnMass = 2500.300049f;
    m_OriginalHandlingData[125].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[125].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.300000f);
    m_OriginalHandlingData[125].uiPercentSubmerged = 80;
    m_OriginalHandlingData[125].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[125].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[125].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[125].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[125].Transmission.uiHandlingFlags = 3179520;
    m_OriginalHandlingData[125].Transmission.fEngineAcceleration = 14.000000f;
    m_OriginalHandlingData[125].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[125].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[125].fBrakeDeceleration = 6.100000f;
    m_OriginalHandlingData[125].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[125].bABS = false;
    m_OriginalHandlingData[125].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[125].fTractionLoss = 0.880000f;
    m_OriginalHandlingData[125].fTractionBias = 0.550000f;
    m_OriginalHandlingData[125].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[125].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[125].fSuspensionHighSpdDamping = 5.000000f;
    m_OriginalHandlingData[125].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[125].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[125].fSuspensionFrontRearBias = 0.350000f;
    m_OriginalHandlingData[125].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[125].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[125].uiModelFlags = 0x40001B04;
    m_OriginalHandlingData[125].uiHandlingFlags = 0x308400;
    m_OriginalHandlingData[125].fSeatOffsetDistance = 0.600000f;
    m_OriginalHandlingData[125].uiMonetary = 15000;
    m_OriginalHandlingData[125].ucHeadLight = 1;
    m_OriginalHandlingData[125].ucTailLight = 2;
    m_OriginalHandlingData[125].ucAnimGroup = 1;

    m_OriginalHandlingData[126].iVehicleID = 126;
    m_OriginalHandlingData[126].fMass = 5500.000000f;
    m_OriginalHandlingData[126].fTurnMass = 33187.898438f;
    m_OriginalHandlingData[126].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[126].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[126].uiPercentSubmerged = 90;
    m_OriginalHandlingData[126].fTractionMultiplier = 0.580000f;
    m_OriginalHandlingData[126].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[126].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[126].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[126].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[126].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[126].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[126].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[126].fBrakeDeceleration = 3.170000f;
    m_OriginalHandlingData[126].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[126].bABS = false;
    m_OriginalHandlingData[126].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[126].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[126].fTractionBias = 0.500000f;
    m_OriginalHandlingData[126].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[126].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[126].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[126].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[126].fSuspensionLowerLimit = 0.000000f;
    m_OriginalHandlingData[126].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[126].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[126].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[126].uiModelFlags = 0x8;
    m_OriginalHandlingData[126].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[126].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[126].uiMonetary = 5000;
    m_OriginalHandlingData[126].ucHeadLight = 0;
    m_OriginalHandlingData[126].ucTailLight = 1;
    m_OriginalHandlingData[126].ucAnimGroup = 0;

    m_OriginalHandlingData[127].iVehicleID = 127;
    m_OriginalHandlingData[127].fMass = 5500.000000f;
    m_OriginalHandlingData[127].fTurnMass = 33187.898438f;
    m_OriginalHandlingData[127].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[127].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[127].uiPercentSubmerged = 90;
    m_OriginalHandlingData[127].fTractionMultiplier = 0.580000f;
    m_OriginalHandlingData[127].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[127].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[127].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[127].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[127].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[127].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[127].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[127].fBrakeDeceleration = 3.170000f;
    m_OriginalHandlingData[127].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[127].bABS = false;
    m_OriginalHandlingData[127].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[127].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[127].fTractionBias = 0.500000f;
    m_OriginalHandlingData[127].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[127].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[127].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[127].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[127].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[127].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[127].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[127].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[127].uiModelFlags = 0x8;
    m_OriginalHandlingData[127].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[127].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[127].uiMonetary = 5000;
    m_OriginalHandlingData[127].ucHeadLight = 0;
    m_OriginalHandlingData[127].ucTailLight = 1;
    m_OriginalHandlingData[127].ucAnimGroup = 21;

    m_OriginalHandlingData[128].iVehicleID = 128;
    m_OriginalHandlingData[128].fMass = 300.000000f;
    m_OriginalHandlingData[128].fTurnMass = 150.000000f;
    m_OriginalHandlingData[128].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[128].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.150000f);
    m_OriginalHandlingData[128].uiPercentSubmerged = 110;
    m_OriginalHandlingData[128].fTractionMultiplier = 0.900000f;
    m_OriginalHandlingData[128].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[128].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[128].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[128].Transmission.uiHandlingFlags = 3179009;
    m_OriginalHandlingData[128].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[128].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[128].Transmission.fMaxVelocity = 90.000000f;
    m_OriginalHandlingData[128].fBrakeDeceleration = 15.000000f;
    m_OriginalHandlingData[128].fBrakeBias = 0.200000f;
    m_OriginalHandlingData[128].bABS = false;
    m_OriginalHandlingData[128].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[128].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[128].fTractionBias = 0.480000f;
    m_OriginalHandlingData[128].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[128].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[128].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[128].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[128].fSuspensionLowerLimit = -0.040000f;
    m_OriginalHandlingData[128].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[128].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[128].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[128].uiModelFlags = 0x40001B00;
    m_OriginalHandlingData[128].uiHandlingFlags = 0x308201;
    m_OriginalHandlingData[128].fSeatOffsetDistance = 0.380000f;
    m_OriginalHandlingData[128].uiMonetary = 15000;
    m_OriginalHandlingData[128].ucHeadLight = 1;
    m_OriginalHandlingData[128].ucTailLight = 2;
    m_OriginalHandlingData[128].ucAnimGroup = 18;

    m_OriginalHandlingData[129].iVehicleID = 129;
    m_OriginalHandlingData[129].fMass = 800.000000f;
    m_OriginalHandlingData[129].fTurnMass = 500.000000f;
    m_OriginalHandlingData[129].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[129].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.300000f);
    m_OriginalHandlingData[129].uiPercentSubmerged = 80;
    m_OriginalHandlingData[129].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[129].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[129].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[129].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[129].Transmission.uiHandlingFlags = 19955713;
    m_OriginalHandlingData[129].Transmission.fEngineAcceleration = 4.800000f;
    m_OriginalHandlingData[129].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[129].Transmission.fMaxVelocity = 60.000000f;
    m_OriginalHandlingData[129].fBrakeDeceleration = 6.100000f;
    m_OriginalHandlingData[129].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[129].bABS = false;
    m_OriginalHandlingData[129].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[129].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[129].fTractionBias = 0.480000f;
    m_OriginalHandlingData[129].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[129].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[129].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[129].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[129].fSuspensionLowerLimit = -0.050000f;
    m_OriginalHandlingData[129].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[129].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[129].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[129].uiModelFlags = 0x40001B00;
    m_OriginalHandlingData[129].uiHandlingFlags = 0x1308001;
    m_OriginalHandlingData[129].fSeatOffsetDistance = 0.380000f;
    m_OriginalHandlingData[129].uiMonetary = 15000;
    m_OriginalHandlingData[129].ucHeadLight = 1;
    m_OriginalHandlingData[129].ucTailLight = 2;
    m_OriginalHandlingData[129].ucAnimGroup = 28;

    m_OriginalHandlingData[130].iVehicleID = 130;
    m_OriginalHandlingData[130].fMass = 10000.000000f;
    m_OriginalHandlingData[130].fTurnMass = 50000.000000f;
    m_OriginalHandlingData[130].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[130].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.600000f);
    m_OriginalHandlingData[130].uiPercentSubmerged = 80;
    m_OriginalHandlingData[130].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[130].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[130].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[130].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[130].Transmission.uiHandlingFlags = 19922949;
    m_OriginalHandlingData[130].Transmission.fEngineAcceleration = 14.000000f;
    m_OriginalHandlingData[130].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[130].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[130].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[130].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[130].bABS = false;
    m_OriginalHandlingData[130].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[130].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[130].fTractionBias = 0.500000f;
    m_OriginalHandlingData[130].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[130].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[130].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[130].fSuspensionUpperLimit = 0.400000f;
    m_OriginalHandlingData[130].fSuspensionLowerLimit = -0.400000f;
    m_OriginalHandlingData[130].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[130].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[130].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[130].uiModelFlags = 0x6028;
    m_OriginalHandlingData[130].uiHandlingFlags = 0x1300005;
    m_OriginalHandlingData[130].fSeatOffsetDistance = 0.290000f;
    m_OriginalHandlingData[130].uiMonetary = 40000;
    m_OriginalHandlingData[130].ucHeadLight = 0;
    m_OriginalHandlingData[130].ucTailLight = 1;
    m_OriginalHandlingData[130].ucAnimGroup = 2;

    m_OriginalHandlingData[131].iVehicleID = 131;
    m_OriginalHandlingData[131].fMass = 800.000000f;
    m_OriginalHandlingData[131].fTurnMass = 632.700012f;
    m_OriginalHandlingData[131].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[131].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.300000f);
    m_OriginalHandlingData[131].uiPercentSubmerged = 80;
    m_OriginalHandlingData[131].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[131].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[131].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[131].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[131].Transmission.uiHandlingFlags = 19956225;
    m_OriginalHandlingData[131].Transmission.fEngineAcceleration = 4.800000f;
    m_OriginalHandlingData[131].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[131].Transmission.fMaxVelocity = 60.000000f;
    m_OriginalHandlingData[131].fBrakeDeceleration = 6.100000f;
    m_OriginalHandlingData[131].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[131].bABS = false;
    m_OriginalHandlingData[131].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[131].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[131].fTractionBias = 0.460000f;
    m_OriginalHandlingData[131].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[131].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[131].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[131].fSuspensionUpperLimit = 0.340000f;
    m_OriginalHandlingData[131].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[131].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[131].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[131].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[131].uiModelFlags = 0x40005A00;
    m_OriginalHandlingData[131].uiHandlingFlags = 0x1308201;
    m_OriginalHandlingData[131].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[131].uiMonetary = 15000;
    m_OriginalHandlingData[131].ucHeadLight = 1;
    m_OriginalHandlingData[131].ucTailLight = 2;
    m_OriginalHandlingData[131].ucAnimGroup = 0;

    m_OriginalHandlingData[132].iVehicleID = 132;
    m_OriginalHandlingData[132].fMass = 1700.000000f;
    m_OriginalHandlingData[132].fTurnMass = 4166.399902f;
    m_OriginalHandlingData[132].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[132].vecCenterOfMass = CVector(0.000000f, 0.100000f, 0.100000f);
    m_OriginalHandlingData[132].uiPercentSubmerged = 70;
    m_OriginalHandlingData[132].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[132].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[132].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[132].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[132].Transmission.uiHandlingFlags = 33628416;
    m_OriginalHandlingData[132].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[132].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[132].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[132].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[132].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[132].bABS = false;
    m_OriginalHandlingData[132].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[132].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[132].fTractionBias = 0.460000f;
    m_OriginalHandlingData[132].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[132].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[132].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[132].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[132].fSuspensionLowerLimit = -0.140000f;
    m_OriginalHandlingData[132].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[132].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[132].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[132].uiModelFlags = 0x220000;
    m_OriginalHandlingData[132].uiHandlingFlags = 0x2012100;
    m_OriginalHandlingData[132].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[132].uiMonetary = 19000;
    m_OriginalHandlingData[132].ucHeadLight = 1;
    m_OriginalHandlingData[132].ucTailLight = 1;
    m_OriginalHandlingData[132].ucAnimGroup = 0;

    m_OriginalHandlingData[133].iVehicleID = 133;
    m_OriginalHandlingData[133].fMass = 1700.000000f;
    m_OriginalHandlingData[133].fTurnMass = 4166.399902f;
    m_OriginalHandlingData[133].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[133].vecCenterOfMass = CVector(0.000000f, -0.100000f, 0.100000f);
    m_OriginalHandlingData[133].uiPercentSubmerged = 70;
    m_OriginalHandlingData[133].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[133].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[133].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[133].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[133].Transmission.uiHandlingFlags = 33619968;
    m_OriginalHandlingData[133].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[133].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[133].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[133].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[133].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[133].bABS = false;
    m_OriginalHandlingData[133].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[133].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[133].fTractionBias = 0.520000f;
    m_OriginalHandlingData[133].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[133].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[133].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[133].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[133].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[133].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[133].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[133].fCollisionDamageMultiplier = 0.520000f;
    m_OriginalHandlingData[133].uiModelFlags = 0x220000;
    m_OriginalHandlingData[133].uiHandlingFlags = 0x2010000;
    m_OriginalHandlingData[133].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[133].uiMonetary = 19000;
    m_OriginalHandlingData[133].ucHeadLight = 1;
    m_OriginalHandlingData[133].ucTailLight = 1;
    m_OriginalHandlingData[133].ucAnimGroup = 0;

    m_OriginalHandlingData[134].iVehicleID = 134;
    m_OriginalHandlingData[134].fMass = 5500.000000f;
    m_OriginalHandlingData[134].fTurnMass = 33187.898438f;
    m_OriginalHandlingData[134].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[134].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[134].uiPercentSubmerged = 90;
    m_OriginalHandlingData[134].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[134].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[134].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[134].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[134].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[134].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[134].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[134].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[134].fBrakeDeceleration = 3.500000f;
    m_OriginalHandlingData[134].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[134].bABS = false;
    m_OriginalHandlingData[134].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[134].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[134].fTractionBias = 0.400000f;
    m_OriginalHandlingData[134].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[134].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[134].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[134].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[134].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[134].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[134].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[134].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[134].uiModelFlags = 0x4008;
    m_OriginalHandlingData[134].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[134].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[134].uiMonetary = 5000;
    m_OriginalHandlingData[134].ucHeadLight = 0;
    m_OriginalHandlingData[134].ucTailLight = 1;
    m_OriginalHandlingData[134].ucAnimGroup = 2;

    m_OriginalHandlingData[135].iVehicleID = 135;
    m_OriginalHandlingData[135].fMass = 2500.000000f;
    m_OriginalHandlingData[135].fTurnMass = 6000.000000f;
    m_OriginalHandlingData[135].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[135].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[135].uiPercentSubmerged = 80;
    m_OriginalHandlingData[135].fTractionMultiplier = 0.620000f;
    m_OriginalHandlingData[135].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[135].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[135].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[135].Transmission.uiHandlingFlags = 17412;
    m_OriginalHandlingData[135].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[135].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[135].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[135].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[135].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[135].bABS = false;
    m_OriginalHandlingData[135].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[135].fTractionLoss = 0.890000f;
    m_OriginalHandlingData[135].fTractionBias = 0.500000f;
    m_OriginalHandlingData[135].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[135].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[135].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[135].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[135].fSuspensionLowerLimit = -0.210000f;
    m_OriginalHandlingData[135].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[135].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[135].fCollisionDamageMultiplier = 0.350000f;
    m_OriginalHandlingData[135].uiModelFlags = 0x20;
    m_OriginalHandlingData[135].uiHandlingFlags = 0x4404;
    m_OriginalHandlingData[135].fSeatOffsetDistance = 0.440000f;
    m_OriginalHandlingData[135].uiMonetary = 40000;
    m_OriginalHandlingData[135].ucHeadLight = 0;
    m_OriginalHandlingData[135].ucTailLight = 1;
    m_OriginalHandlingData[135].ucAnimGroup = 0;

    m_OriginalHandlingData[136].iVehicleID = 136;
    m_OriginalHandlingData[136].fMass = 2200.000000f;
    m_OriginalHandlingData[136].fTurnMass = 6000.000000f;
    m_OriginalHandlingData[136].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[136].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[136].uiPercentSubmerged = 75;
    m_OriginalHandlingData[136].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[136].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[136].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[136].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[136].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[136].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[136].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[136].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[136].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[136].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[136].bABS = false;
    m_OriginalHandlingData[136].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[136].fTractionLoss = 0.920000f;
    m_OriginalHandlingData[136].fTractionBias = 0.500000f;
    m_OriginalHandlingData[136].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[136].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[136].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[136].fSuspensionUpperLimit = 0.270000f;
    m_OriginalHandlingData[136].fSuspensionLowerLimit = -0.220000f;
    m_OriginalHandlingData[136].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[136].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[136].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[136].uiModelFlags = 0x0;
    m_OriginalHandlingData[136].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[136].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[136].uiMonetary = 35000;
    m_OriginalHandlingData[136].ucHeadLight = 0;
    m_OriginalHandlingData[136].ucTailLight = 1;
    m_OriginalHandlingData[136].ucAnimGroup = 0;

    m_OriginalHandlingData[137].iVehicleID = 137;
    m_OriginalHandlingData[137].fMass = 1900.000000f;
    m_OriginalHandlingData[137].fTurnMass = 6333.299805f;
    m_OriginalHandlingData[137].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[137].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.150000f);
    m_OriginalHandlingData[137].uiPercentSubmerged = 80;
    m_OriginalHandlingData[137].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[137].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[137].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[137].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[137].Transmission.uiHandlingFlags = 2;
    m_OriginalHandlingData[137].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[137].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[137].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[137].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[137].fBrakeBias = 0.800000f;
    m_OriginalHandlingData[137].bABS = false;
    m_OriginalHandlingData[137].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[137].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[137].fTractionBias = 0.460000f;
    m_OriginalHandlingData[137].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[137].fSuspensionDamping = 0.070000f;
    m_OriginalHandlingData[137].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[137].fSuspensionUpperLimit = 0.350000f;
    m_OriginalHandlingData[137].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[137].fSuspensionFrontRearBias = 0.450000f;
    m_OriginalHandlingData[137].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[137].fCollisionDamageMultiplier = 0.430000f;
    m_OriginalHandlingData[137].uiModelFlags = 0x4001;
    m_OriginalHandlingData[137].uiHandlingFlags = 0x2;
    m_OriginalHandlingData[137].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[137].uiMonetary = 20000;
    m_OriginalHandlingData[137].ucHeadLight = 0;
    m_OriginalHandlingData[137].ucTailLight = 3;
    m_OriginalHandlingData[137].ucAnimGroup = 13;

    m_OriginalHandlingData[138].iVehicleID = 138;
    m_OriginalHandlingData[138].fMass = 800.000000f;
    m_OriginalHandlingData[138].fTurnMass = 632.700012f;
    m_OriginalHandlingData[138].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[138].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[138].uiPercentSubmerged = 80;
    m_OriginalHandlingData[138].fTractionMultiplier = 0.850000f;
    m_OriginalHandlingData[138].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[138].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[138].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[138].Transmission.uiHandlingFlags = 3179008;
    m_OriginalHandlingData[138].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[138].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[138].Transmission.fMaxVelocity = 170.000000f;
    m_OriginalHandlingData[138].fBrakeDeceleration = 6.100000f;
    m_OriginalHandlingData[138].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[138].bABS = false;
    m_OriginalHandlingData[138].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[138].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[138].fTractionBias = 0.460000f;
    m_OriginalHandlingData[138].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[138].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[138].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[138].fSuspensionUpperLimit = 0.340000f;
    m_OriginalHandlingData[138].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[138].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[138].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[138].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[138].uiModelFlags = 0x1A00;
    m_OriginalHandlingData[138].uiHandlingFlags = 0x308200;
    m_OriginalHandlingData[138].fSeatOffsetDistance = 0.380000f;
    m_OriginalHandlingData[138].uiMonetary = 15000;
    m_OriginalHandlingData[138].ucHeadLight = 1;
    m_OriginalHandlingData[138].ucTailLight = 2;
    m_OriginalHandlingData[138].ucAnimGroup = 0;

    m_OriginalHandlingData[139].iVehicleID = 139;
    m_OriginalHandlingData[139].fMass = 3800.000000f;
    m_OriginalHandlingData[139].fTurnMass = 30000.000000f;
    m_OriginalHandlingData[139].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[139].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.500000f);
    m_OriginalHandlingData[139].uiPercentSubmerged = 90;
    m_OriginalHandlingData[139].fTractionMultiplier = 0.450000f;
    m_OriginalHandlingData[139].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[139].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[139].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[139].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[139].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[139].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[139].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[139].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[139].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[139].bABS = false;
    m_OriginalHandlingData[139].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[139].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[139].fTractionBias = 0.500000f;
    m_OriginalHandlingData[139].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[139].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[139].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[139].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[139].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[139].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[139].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[139].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[139].uiModelFlags = 0x20002000;
    m_OriginalHandlingData[139].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[139].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[139].uiMonetary = 35000;
    m_OriginalHandlingData[139].ucHeadLight = 0;
    m_OriginalHandlingData[139].ucTailLight = 1;
    m_OriginalHandlingData[139].ucAnimGroup = 0;

    m_OriginalHandlingData[140].iVehicleID = 140;
    m_OriginalHandlingData[140].fMass = 1800.000000f;
    m_OriginalHandlingData[140].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[140].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[140].vecCenterOfMass = CVector(0.000000f, 0.200000f, 0.150000f);
    m_OriginalHandlingData[140].uiPercentSubmerged = 75;
    m_OriginalHandlingData[140].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[140].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[140].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[140].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[140].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[140].Transmission.fEngineAcceleration = 8.400000f;
    m_OriginalHandlingData[140].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[140].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[140].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[140].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[140].bABS = false;
    m_OriginalHandlingData[140].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[140].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[140].fTractionBias = 0.520000f;
    m_OriginalHandlingData[140].fSuspensionForceLevel = 0.900000f;
    m_OriginalHandlingData[140].fSuspensionDamping = 0.130000f;
    m_OriginalHandlingData[140].fSuspensionHighSpdDamping = 3.000000f;
    m_OriginalHandlingData[140].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[140].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[140].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[140].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[140].fCollisionDamageMultiplier = 0.560000f;
    m_OriginalHandlingData[140].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[140].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[140].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[140].uiMonetary = 35000;
    m_OriginalHandlingData[140].ucHeadLight = 0;
    m_OriginalHandlingData[140].ucTailLight = 1;
    m_OriginalHandlingData[140].ucAnimGroup = 0;

    m_OriginalHandlingData[141].iVehicleID = 141;
    m_OriginalHandlingData[141].fMass = 5500.000000f;
    m_OriginalHandlingData[141].fTurnMass = 33187.898438f;
    m_OriginalHandlingData[141].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[141].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[141].uiPercentSubmerged = 90;
    m_OriginalHandlingData[141].fTractionMultiplier = 0.580000f;
    m_OriginalHandlingData[141].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[141].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[141].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[141].Transmission.uiHandlingFlags = 512;
    m_OriginalHandlingData[141].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[141].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[141].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[141].fBrakeDeceleration = 3.170000f;
    m_OriginalHandlingData[141].fBrakeBias = 0.400000f;
    m_OriginalHandlingData[141].bABS = false;
    m_OriginalHandlingData[141].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[141].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[141].fTractionBias = 0.500000f;
    m_OriginalHandlingData[141].fSuspensionForceLevel = 1.400000f;
    m_OriginalHandlingData[141].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[141].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[141].fSuspensionUpperLimit = 0.450000f;
    m_OriginalHandlingData[141].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[141].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[141].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[141].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[141].uiModelFlags = 0x8;
    m_OriginalHandlingData[141].uiHandlingFlags = 0x200;
    m_OriginalHandlingData[141].fSeatOffsetDistance = 0.450000f;
    m_OriginalHandlingData[141].uiMonetary = 5000;
    m_OriginalHandlingData[141].ucHeadLight = 0;
    m_OriginalHandlingData[141].ucTailLight = 1;
    m_OriginalHandlingData[141].ucAnimGroup = 0;

    m_OriginalHandlingData[142].iVehicleID = 142;
    m_OriginalHandlingData[142].fMass = 1400.000000f;
    m_OriginalHandlingData[142].fTurnMass = 2998.300049f;
    m_OriginalHandlingData[142].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[142].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.100000f);
    m_OriginalHandlingData[142].uiPercentSubmerged = 75;
    m_OriginalHandlingData[142].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[142].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[142].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[142].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[142].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[142].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[142].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[142].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[142].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[142].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[142].bABS = false;
    m_OriginalHandlingData[142].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[142].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[142].fTractionBias = 0.500000f;
    m_OriginalHandlingData[142].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[142].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[142].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[142].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[142].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[142].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[142].fSuspensionAntiDiveMultiplier = 0.300000f;
    m_OriginalHandlingData[142].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[142].uiModelFlags = 0x40002804;
    m_OriginalHandlingData[142].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[142].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[142].uiMonetary = 35000;
    m_OriginalHandlingData[142].ucHeadLight = 1;
    m_OriginalHandlingData[142].ucTailLight = 1;
    m_OriginalHandlingData[142].ucAnimGroup = 0;

    m_OriginalHandlingData[143].iVehicleID = 143;
    m_OriginalHandlingData[143].fMass = 5500.000000f;
    m_OriginalHandlingData[143].fTurnMass = 23489.599609f;
    m_OriginalHandlingData[143].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[143].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.300000f);
    m_OriginalHandlingData[143].uiPercentSubmerged = 80;
    m_OriginalHandlingData[143].fTractionMultiplier = 0.720000f;
    m_OriginalHandlingData[143].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[143].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[143].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[143].Transmission.uiHandlingFlags = 513;
    m_OriginalHandlingData[143].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[143].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[143].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[143].fBrakeDeceleration = 4.500000f;
    m_OriginalHandlingData[143].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[143].bABS = false;
    m_OriginalHandlingData[143].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[143].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[143].fTractionBias = 0.460000f;
    m_OriginalHandlingData[143].fSuspensionForceLevel = 0.600000f;
    m_OriginalHandlingData[143].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[143].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[143].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[143].fSuspensionLowerLimit = -0.240000f;
    m_OriginalHandlingData[143].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[143].fSuspensionAntiDiveMultiplier = 0.600000f;
    m_OriginalHandlingData[143].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[143].uiModelFlags = 0x40000009;
    m_OriginalHandlingData[143].uiHandlingFlags = 0x201;
    m_OriginalHandlingData[143].fSeatOffsetDistance = 0.360000f;
    m_OriginalHandlingData[143].uiMonetary = 22000;
    m_OriginalHandlingData[143].ucHeadLight = 0;
    m_OriginalHandlingData[143].ucTailLight = 3;
    m_OriginalHandlingData[143].ucAnimGroup = 13;

    m_OriginalHandlingData[144].iVehicleID = 144;
    m_OriginalHandlingData[144].fMass = 1400.000000f;
    m_OriginalHandlingData[144].fTurnMass = 3000.000000f;
    m_OriginalHandlingData[144].fDragCoeff = 2.800000f;
    m_OriginalHandlingData[144].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[144].uiPercentSubmerged = 80;
    m_OriginalHandlingData[144].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[144].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[144].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[144].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[144].Transmission.uiHandlingFlags = 12582912;
    m_OriginalHandlingData[144].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[144].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[144].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[144].fBrakeDeceleration = 11.000000f;
    m_OriginalHandlingData[144].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[144].bABS = false;
    m_OriginalHandlingData[144].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[144].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[144].fTractionBias = 0.490000f;
    m_OriginalHandlingData[144].fSuspensionForceLevel = 1.700000f;
    m_OriginalHandlingData[144].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[144].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[144].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[144].fSuspensionLowerLimit = -0.120000f;
    m_OriginalHandlingData[144].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[144].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[144].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[144].uiModelFlags = 0x2000;
    m_OriginalHandlingData[144].uiHandlingFlags = 0xC00000;
    m_OriginalHandlingData[144].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[144].uiMonetary = 35000;
    m_OriginalHandlingData[144].ucHeadLight = 1;
    m_OriginalHandlingData[144].ucTailLight = 1;
    m_OriginalHandlingData[144].ucAnimGroup = 0;

    m_OriginalHandlingData[145].iVehicleID = 145;
    m_OriginalHandlingData[145].fMass = 3800.000000f;
    m_OriginalHandlingData[145].fTurnMass = 30000.000000f;
    m_OriginalHandlingData[145].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[145].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.500000f);
    m_OriginalHandlingData[145].uiPercentSubmerged = 90;
    m_OriginalHandlingData[145].fTractionMultiplier = 0.450000f;
    m_OriginalHandlingData[145].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[145].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[145].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[145].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[145].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[145].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[145].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[145].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[145].fBrakeBias = 0.300000f;
    m_OriginalHandlingData[145].bABS = false;
    m_OriginalHandlingData[145].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[145].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[145].fTractionBias = 0.500000f;
    m_OriginalHandlingData[145].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[145].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[145].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[145].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[145].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[145].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[145].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[145].fCollisionDamageMultiplier = 0.250000f;
    m_OriginalHandlingData[145].uiModelFlags = 0x20002000;
    m_OriginalHandlingData[145].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[145].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[145].uiMonetary = 35000;
    m_OriginalHandlingData[145].ucHeadLight = 0;
    m_OriginalHandlingData[145].ucTailLight = 1;
    m_OriginalHandlingData[145].ucAnimGroup = 0;

    m_OriginalHandlingData[146].iVehicleID = 146;
    m_OriginalHandlingData[146].fMass = 100.000000f;
    m_OriginalHandlingData[146].fTurnMass = 50.000000f;
    m_OriginalHandlingData[146].fDragCoeff = 20.000000f;
    m_OriginalHandlingData[146].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.200000f);
    m_OriginalHandlingData[146].uiPercentSubmerged = 70;
    m_OriginalHandlingData[146].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[146].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[146].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[146].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[146].Transmission.uiHandlingFlags = 16777216;
    m_OriginalHandlingData[146].Transmission.fEngineAcceleration = 20.000000f;
    m_OriginalHandlingData[146].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[146].Transmission.fMaxVelocity = 60.000000f;
    m_OriginalHandlingData[146].fBrakeDeceleration = 5.500000f;
    m_OriginalHandlingData[146].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[146].bABS = false;
    m_OriginalHandlingData[146].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[146].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[146].fTractionBias = 0.490000f;
    m_OriginalHandlingData[146].fSuspensionForceLevel = 3.000000f;
    m_OriginalHandlingData[146].fSuspensionDamping = 0.300000f;
    m_OriginalHandlingData[146].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[146].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[146].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[146].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[146].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[146].fCollisionDamageMultiplier = 0.050000f;
    m_OriginalHandlingData[146].uiModelFlags = 0x0;
    m_OriginalHandlingData[146].uiHandlingFlags = 0x1000000;
    m_OriginalHandlingData[146].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[146].uiMonetary = 500;
    m_OriginalHandlingData[146].ucHeadLight = 0;
    m_OriginalHandlingData[146].ucTailLight = 1;
    m_OriginalHandlingData[146].ucAnimGroup = 0;

    m_OriginalHandlingData[147].iVehicleID = 147;
    m_OriginalHandlingData[147].fMass = 1600.000000f;
    m_OriginalHandlingData[147].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[147].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[147].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.100000f);
    m_OriginalHandlingData[147].uiPercentSubmerged = 75;
    m_OriginalHandlingData[147].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[147].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[147].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[147].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[147].Transmission.uiHandlingFlags = 270532616;
    m_OriginalHandlingData[147].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[147].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[147].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[147].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[147].fBrakeBias = 0.530000f;
    m_OriginalHandlingData[147].bABS = false;
    m_OriginalHandlingData[147].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[147].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[147].fTractionBias = 0.500000f;
    m_OriginalHandlingData[147].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[147].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[147].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[147].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[147].fSuspensionLowerLimit = -0.120000f;
    m_OriginalHandlingData[147].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[147].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[147].fCollisionDamageMultiplier = 0.240000f;
    m_OriginalHandlingData[147].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[147].uiHandlingFlags = 0x10200008;
    m_OriginalHandlingData[147].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[147].uiMonetary = 25000;
    m_OriginalHandlingData[147].ucHeadLight = 0;
    m_OriginalHandlingData[147].ucTailLight = 1;
    m_OriginalHandlingData[147].ucAnimGroup = 0;

    m_OriginalHandlingData[148].iVehicleID = 148;
    m_OriginalHandlingData[148].fMass = 1600.000000f;
    m_OriginalHandlingData[148].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[148].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[148].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.150000f);
    m_OriginalHandlingData[148].uiPercentSubmerged = 75;
    m_OriginalHandlingData[148].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[148].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[148].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[148].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[148].Transmission.uiHandlingFlags = 270532616;
    m_OriginalHandlingData[148].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[148].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[148].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[148].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[148].fBrakeBias = 0.530000f;
    m_OriginalHandlingData[148].bABS = false;
    m_OriginalHandlingData[148].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[148].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[148].fTractionBias = 0.520000f;
    m_OriginalHandlingData[148].fSuspensionForceLevel = 1.100000f;
    m_OriginalHandlingData[148].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[148].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[148].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[148].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[148].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[148].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[148].fCollisionDamageMultiplier = 0.240000f;
    m_OriginalHandlingData[148].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[148].uiHandlingFlags = 0x10200008;
    m_OriginalHandlingData[148].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[148].uiMonetary = 25000;
    m_OriginalHandlingData[148].ucHeadLight = 0;
    m_OriginalHandlingData[148].ucTailLight = 1;
    m_OriginalHandlingData[148].ucAnimGroup = 0;

    m_OriginalHandlingData[149].iVehicleID = 149;
    m_OriginalHandlingData[149].fMass = 1600.000000f;
    m_OriginalHandlingData[149].fTurnMass = 4500.000000f;
    m_OriginalHandlingData[149].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[149].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.100000f);
    m_OriginalHandlingData[149].uiPercentSubmerged = 75;
    m_OriginalHandlingData[149].fTractionMultiplier = 0.750000f;
    m_OriginalHandlingData[149].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[149].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[149].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[149].Transmission.uiHandlingFlags = 270532616;
    m_OriginalHandlingData[149].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[149].Transmission.fEngineInertia = 10.000000f;
    m_OriginalHandlingData[149].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[149].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[149].fBrakeBias = 0.530000f;
    m_OriginalHandlingData[149].bABS = false;
    m_OriginalHandlingData[149].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[149].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[149].fTractionBias = 0.520000f;
    m_OriginalHandlingData[149].fSuspensionForceLevel = 0.900000f;
    m_OriginalHandlingData[149].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[149].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[149].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[149].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[149].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[149].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[149].fCollisionDamageMultiplier = 0.240000f;
    m_OriginalHandlingData[149].uiModelFlags = 0x40000000;
    m_OriginalHandlingData[149].uiHandlingFlags = 0x10200008;
    m_OriginalHandlingData[149].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[149].uiMonetary = 25000;
    m_OriginalHandlingData[149].ucHeadLight = 0;
    m_OriginalHandlingData[149].ucTailLight = 1;
    m_OriginalHandlingData[149].ucAnimGroup = 0;

    m_OriginalHandlingData[150].iVehicleID = 150;
    m_OriginalHandlingData[150].fMass = 2500.000000f;
    m_OriginalHandlingData[150].fTurnMass = 5500.000000f;
    m_OriginalHandlingData[150].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[150].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.200000f);
    m_OriginalHandlingData[150].uiPercentSubmerged = 85;
    m_OriginalHandlingData[150].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[150].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[150].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[150].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[150].Transmission.uiHandlingFlags = 3180544;
    m_OriginalHandlingData[150].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[150].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[150].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[150].fBrakeDeceleration = 6.200000f;
    m_OriginalHandlingData[150].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[150].bABS = false;
    m_OriginalHandlingData[150].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[150].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[150].fTractionBias = 0.550000f;
    m_OriginalHandlingData[150].fSuspensionForceLevel = 0.700000f;
    m_OriginalHandlingData[150].fSuspensionDamping = 0.060000f;
    m_OriginalHandlingData[150].fSuspensionHighSpdDamping = 1.000000f;
    m_OriginalHandlingData[150].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[150].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[150].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[150].fSuspensionAntiDiveMultiplier = 0.250000f;
    m_OriginalHandlingData[150].fCollisionDamageMultiplier = 0.230000f;
    m_OriginalHandlingData[150].uiModelFlags = 0x284020;
    m_OriginalHandlingData[150].uiHandlingFlags = 0x308800;
    m_OriginalHandlingData[150].fSeatOffsetDistance = 0.270000f;
    m_OriginalHandlingData[150].uiMonetary = 25000;
    m_OriginalHandlingData[150].ucHeadLight = 0;
    m_OriginalHandlingData[150].ucTailLight = 1;
    m_OriginalHandlingData[150].ucAnimGroup = 0;

    m_OriginalHandlingData[151].iVehicleID = 151;
    m_OriginalHandlingData[151].fMass = 1600.000000f;
    m_OriginalHandlingData[151].fTurnMass = 3800.000000f;
    m_OriginalHandlingData[151].fDragCoeff = 2.700000f;
    m_OriginalHandlingData[151].vecCenterOfMass = CVector(0.000000f, 0.200000f, 0.000000f);
    m_OriginalHandlingData[151].uiPercentSubmerged = 75;
    m_OriginalHandlingData[151].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[151].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[151].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[151].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[151].Transmission.uiHandlingFlags = 1064964;
    m_OriginalHandlingData[151].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[151].Transmission.fEngineInertia = 20.000000f;
    m_OriginalHandlingData[151].Transmission.fMaxVelocity = 165.000000f;
    m_OriginalHandlingData[151].fBrakeDeceleration = 8.500000f;
    m_OriginalHandlingData[151].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[151].bABS = false;
    m_OriginalHandlingData[151].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[151].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[151].fTractionBias = 0.520000f;
    m_OriginalHandlingData[151].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[151].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[151].fSuspensionHighSpdDamping = 2.000000f;
    m_OriginalHandlingData[151].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[151].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[151].fSuspensionFrontRearBias = 0.400000f;
    m_OriginalHandlingData[151].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[151].fCollisionDamageMultiplier = 0.200000f;
    m_OriginalHandlingData[151].uiModelFlags = 0x40200040;
    m_OriginalHandlingData[151].uiHandlingFlags = 0x104004;
    m_OriginalHandlingData[151].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[151].uiMonetary = 26000;
    m_OriginalHandlingData[151].ucHeadLight = 0;
    m_OriginalHandlingData[151].ucTailLight = 1;
    m_OriginalHandlingData[151].ucAnimGroup = 0;

    m_OriginalHandlingData[152].iVehicleID = 152;
    m_OriginalHandlingData[152].fMass = 5000.000000f;
    m_OriginalHandlingData[152].fTurnMass = 10000.000000f;
    m_OriginalHandlingData[152].fDragCoeff = 2.500000f;
    m_OriginalHandlingData[152].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[152].uiPercentSubmerged = 85;
    m_OriginalHandlingData[152].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[152].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[152].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[152].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[152].Transmission.uiHandlingFlags = 16777216;
    m_OriginalHandlingData[152].Transmission.fEngineAcceleration = 9.600000f;
    m_OriginalHandlingData[152].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[152].Transmission.fMaxVelocity = 110.000000f;
    m_OriginalHandlingData[152].fBrakeDeceleration = 6.400000f;
    m_OriginalHandlingData[152].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[152].bABS = false;
    m_OriginalHandlingData[152].fSteeringLock = 27.000000f;
    m_OriginalHandlingData[152].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[152].fTractionBias = 0.460000f;
    m_OriginalHandlingData[152].fSuspensionForceLevel = 0.700000f;
    m_OriginalHandlingData[152].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[152].fSuspensionHighSpdDamping = 1.000000f;
    m_OriginalHandlingData[152].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[152].fSuspensionLowerLimit = -0.180000f;
    m_OriginalHandlingData[152].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[152].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[152].fCollisionDamageMultiplier = 0.060000f;
    m_OriginalHandlingData[152].uiModelFlags = 0x880010;
    m_OriginalHandlingData[152].uiHandlingFlags = 0x1000000;
    m_OriginalHandlingData[152].fSeatOffsetDistance = 0.320000f;
    m_OriginalHandlingData[152].uiMonetary = 40000;
    m_OriginalHandlingData[152].ucHeadLight = 0;
    m_OriginalHandlingData[152].ucTailLight = 1;
    m_OriginalHandlingData[152].ucAnimGroup = 13;

    m_OriginalHandlingData[153].iVehicleID = 153;
    m_OriginalHandlingData[153].fMass = 1500.000000f;
    m_OriginalHandlingData[153].fTurnMass = 3400.000000f;
    m_OriginalHandlingData[153].fDragCoeff = 2.000000f;
    m_OriginalHandlingData[153].vecCenterOfMass = CVector(0.000000f, 0.100000f, -0.200000f);
    m_OriginalHandlingData[153].uiPercentSubmerged = 85;
    m_OriginalHandlingData[153].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[153].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[153].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[153].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[153].Transmission.uiHandlingFlags = 2097152;
    m_OriginalHandlingData[153].Transmission.fEngineAcceleration = 9.200000f;
    m_OriginalHandlingData[153].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[153].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[153].fBrakeDeceleration = 7.000000f;
    m_OriginalHandlingData[153].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[153].bABS = false;
    m_OriginalHandlingData[153].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[153].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[153].fTractionBias = 0.500000f;
    m_OriginalHandlingData[153].fSuspensionForceLevel = 1.200000f;
    m_OriginalHandlingData[153].fSuspensionDamping = 0.120000f;
    m_OriginalHandlingData[153].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[153].fSuspensionUpperLimit = 0.300000f;
    m_OriginalHandlingData[153].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[153].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[153].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[153].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[153].uiModelFlags = 0x40002800;
    m_OriginalHandlingData[153].uiHandlingFlags = 0x200000;
    m_OriginalHandlingData[153].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[153].uiMonetary = 35000;
    m_OriginalHandlingData[153].ucHeadLight = 1;
    m_OriginalHandlingData[153].ucTailLight = 1;
    m_OriginalHandlingData[153].ucAnimGroup = 0;

    m_OriginalHandlingData[154].iVehicleID = 154;
    m_OriginalHandlingData[154].fMass = 1500.000000f;
    m_OriginalHandlingData[154].fTurnMass = 4000.000000f;
    m_OriginalHandlingData[154].fDragCoeff = 2.200000f;
    m_OriginalHandlingData[154].vecCenterOfMass = CVector(0.000000f, 0.300000f, -0.150000f);
    m_OriginalHandlingData[154].uiPercentSubmerged = 85;
    m_OriginalHandlingData[154].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[154].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[154].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[154].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[154].Transmission.uiHandlingFlags = 2097152;
    m_OriginalHandlingData[154].Transmission.fEngineAcceleration = 10.400000f;
    m_OriginalHandlingData[154].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[154].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[154].fBrakeDeceleration = 6.000000f;
    m_OriginalHandlingData[154].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[154].bABS = false;
    m_OriginalHandlingData[154].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[154].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[154].fTractionBias = 0.520000f;
    m_OriginalHandlingData[154].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[154].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[154].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[154].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[154].fSuspensionLowerLimit = -0.240000f;
    m_OriginalHandlingData[154].fSuspensionFrontRearBias = 0.590000f;
    m_OriginalHandlingData[154].fSuspensionAntiDiveMultiplier = 0.400000f;
    m_OriginalHandlingData[154].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[154].uiModelFlags = 0x2800;
    m_OriginalHandlingData[154].uiHandlingFlags = 0x200000;
    m_OriginalHandlingData[154].fSeatOffsetDistance = 0.250000f;
    m_OriginalHandlingData[154].uiMonetary = 35000;
    m_OriginalHandlingData[154].ucHeadLight = 1;
    m_OriginalHandlingData[154].ucTailLight = 1;
    m_OriginalHandlingData[154].ucAnimGroup = 0;

    m_OriginalHandlingData[155].iVehicleID = 155;
    m_OriginalHandlingData[155].fMass = 1000.000000f;
    m_OriginalHandlingData[155].fTurnMass = 1354.199951f;
    m_OriginalHandlingData[155].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[155].vecCenterOfMass = CVector(0.000000f, 0.400000f, -0.200000f);
    m_OriginalHandlingData[155].uiPercentSubmerged = 70;
    m_OriginalHandlingData[155].fTractionMultiplier = 1.000000f;
    m_OriginalHandlingData[155].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[155].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[155].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[155].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[155].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[155].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[155].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[155].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[155].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[155].bABS = false;
    m_OriginalHandlingData[155].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[155].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[155].fTractionBias = 0.500000f;
    m_OriginalHandlingData[155].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[155].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[155].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[155].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[155].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[155].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[155].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[155].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[155].uiModelFlags = 0x3100;
    m_OriginalHandlingData[155].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[155].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[155].uiMonetary = 9000;
    m_OriginalHandlingData[155].ucHeadLight = 1;
    m_OriginalHandlingData[155].ucTailLight = 1;
    m_OriginalHandlingData[155].ucAnimGroup = 0;

    m_OriginalHandlingData[156].iVehicleID = 156;
    m_OriginalHandlingData[156].fMass = 1000.000000f;
    m_OriginalHandlingData[156].fTurnMass = 1354.199951f;
    m_OriginalHandlingData[156].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[156].vecCenterOfMass = CVector(0.000000f, 0.400000f, -0.200000f);
    m_OriginalHandlingData[156].uiPercentSubmerged = 70;
    m_OriginalHandlingData[156].fTractionMultiplier = 1.000000f;
    m_OriginalHandlingData[156].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[156].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[156].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[156].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[156].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[156].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[156].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[156].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[156].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[156].bABS = false;
    m_OriginalHandlingData[156].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[156].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[156].fTractionBias = 0.500000f;
    m_OriginalHandlingData[156].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[156].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[156].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[156].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[156].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[156].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[156].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[156].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[156].uiModelFlags = 0x3100;
    m_OriginalHandlingData[156].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[156].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[156].uiMonetary = 9000;
    m_OriginalHandlingData[156].ucHeadLight = 1;
    m_OriginalHandlingData[156].ucTailLight = 1;
    m_OriginalHandlingData[156].ucAnimGroup = 0;

    m_OriginalHandlingData[157].iVehicleID = 157;
    m_OriginalHandlingData[157].fMass = 1000.000000f;
    m_OriginalHandlingData[157].fTurnMass = 2500.000000f;
    m_OriginalHandlingData[157].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[157].vecCenterOfMass = CVector(0.000000f, 0.400000f, -0.200000f);
    m_OriginalHandlingData[157].uiPercentSubmerged = 70;
    m_OriginalHandlingData[157].fTractionMultiplier = 1.000000f;
    m_OriginalHandlingData[157].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[157].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[157].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[157].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[157].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[157].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[157].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[157].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[157].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[157].bABS = false;
    m_OriginalHandlingData[157].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[157].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[157].fTractionBias = 0.500000f;
    m_OriginalHandlingData[157].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[157].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[157].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[157].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[157].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[157].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[157].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[157].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[157].uiModelFlags = 0x3100;
    m_OriginalHandlingData[157].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[157].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[157].uiMonetary = 9000;
    m_OriginalHandlingData[157].ucHeadLight = 1;
    m_OriginalHandlingData[157].ucTailLight = 1;
    m_OriginalHandlingData[157].ucAnimGroup = 0;

    m_OriginalHandlingData[158].iVehicleID = 158;
    m_OriginalHandlingData[158].fMass = 5500.000000f;
    m_OriginalHandlingData[158].fTurnMass = 23489.599609f;
    m_OriginalHandlingData[158].fDragCoeff = 3.000000f;
    m_OriginalHandlingData[158].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[158].uiPercentSubmerged = 80;
    m_OriginalHandlingData[158].fTractionMultiplier = 0.820000f;
    m_OriginalHandlingData[158].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[158].Transmission.ucEngineType = 'D';
    m_OriginalHandlingData[158].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[158].Transmission.uiHandlingFlags = 513;
    m_OriginalHandlingData[158].Transmission.fEngineAcceleration = 5.600000f;
    m_OriginalHandlingData[158].Transmission.fEngineInertia = 25.000000f;
    m_OriginalHandlingData[158].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[158].fBrakeDeceleration = 4.500000f;
    m_OriginalHandlingData[158].fBrakeBias = 0.600000f;
    m_OriginalHandlingData[158].bABS = false;
    m_OriginalHandlingData[158].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[158].fTractionLoss = 0.700000f;
    m_OriginalHandlingData[158].fTractionBias = 0.460000f;
    m_OriginalHandlingData[158].fSuspensionForceLevel = 0.900000f;
    m_OriginalHandlingData[158].fSuspensionDamping = 0.080000f;
    m_OriginalHandlingData[158].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[158].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[158].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[158].fSuspensionFrontRearBias = 0.350000f;
    m_OriginalHandlingData[158].fSuspensionAntiDiveMultiplier = 0.600000f;
    m_OriginalHandlingData[158].fCollisionDamageMultiplier = 0.400000f;
    m_OriginalHandlingData[158].uiModelFlags = 0x4009;
    m_OriginalHandlingData[158].uiHandlingFlags = 0x201;
    m_OriginalHandlingData[158].fSeatOffsetDistance = 0.360000f;
    m_OriginalHandlingData[158].uiMonetary = 22000;
    m_OriginalHandlingData[158].ucHeadLight = 0;
    m_OriginalHandlingData[158].ucTailLight = 3;
    m_OriginalHandlingData[158].ucAnimGroup = 13;

    m_OriginalHandlingData[159].iVehicleID = 159;
    m_OriginalHandlingData[159].fMass = 400.000000f;
    m_OriginalHandlingData[159].fTurnMass = 400.000000f;
    m_OriginalHandlingData[159].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[159].vecCenterOfMass = CVector(0.000000f, -0.400000f, 0.000000f);
    m_OriginalHandlingData[159].uiPercentSubmerged = 70;
    m_OriginalHandlingData[159].fTractionMultiplier = 0.600000f;
    m_OriginalHandlingData[159].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[159].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[159].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[159].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[159].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[159].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[159].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[159].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[159].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[159].bABS = false;
    m_OriginalHandlingData[159].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[159].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[159].fTractionBias = 0.500000f;
    m_OriginalHandlingData[159].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[159].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[159].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[159].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[159].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[159].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[159].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[159].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[159].uiModelFlags = 0x3100;
    m_OriginalHandlingData[159].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[159].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[159].uiMonetary = 9000;
    m_OriginalHandlingData[159].ucHeadLight = 1;
    m_OriginalHandlingData[159].ucTailLight = 1;
    m_OriginalHandlingData[159].ucAnimGroup = 0;

    m_OriginalHandlingData[160].iVehicleID = 160;
    m_OriginalHandlingData[160].fMass = 1000.000000f;
    m_OriginalHandlingData[160].fTurnMass = 1354.199951f;
    m_OriginalHandlingData[160].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[160].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[160].uiPercentSubmerged = 70;
    m_OriginalHandlingData[160].fTractionMultiplier = 1.000000f;
    m_OriginalHandlingData[160].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[160].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[160].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[160].Transmission.uiHandlingFlags = 4;
    m_OriginalHandlingData[160].Transmission.fEngineAcceleration = 8.000000f;
    m_OriginalHandlingData[160].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[160].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[160].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[160].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[160].bABS = false;
    m_OriginalHandlingData[160].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[160].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[160].fTractionBias = 0.500000f;
    m_OriginalHandlingData[160].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[160].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[160].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[160].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[160].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[160].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[160].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[160].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[160].uiModelFlags = 0x3100;
    m_OriginalHandlingData[160].uiHandlingFlags = 0x4;
    m_OriginalHandlingData[160].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[160].uiMonetary = 9000;
    m_OriginalHandlingData[160].ucHeadLight = 1;
    m_OriginalHandlingData[160].ucTailLight = 1;
    m_OriginalHandlingData[160].ucAnimGroup = 0;

    m_OriginalHandlingData[161].iVehicleID = 161;
    m_OriginalHandlingData[161].fMass = 1000.000000f;
    m_OriginalHandlingData[161].fTurnMass = 1354.199951f;
    m_OriginalHandlingData[161].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[161].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[161].uiPercentSubmerged = 70;
    m_OriginalHandlingData[161].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[161].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[161].Transmission.ucEngineType = 'E';
    m_OriginalHandlingData[161].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[161].Transmission.uiHandlingFlags = 34820;
    m_OriginalHandlingData[161].Transmission.fEngineAcceleration = 6.000000f;
    m_OriginalHandlingData[161].Transmission.fEngineInertia = 30.000000f;
    m_OriginalHandlingData[161].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[161].fBrakeDeceleration = 13.000000f;
    m_OriginalHandlingData[161].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[161].bABS = false;
    m_OriginalHandlingData[161].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[161].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[161].fTractionBias = 0.500000f;
    m_OriginalHandlingData[161].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[161].fSuspensionDamping = 0.090000f;
    m_OriginalHandlingData[161].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[161].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[161].fSuspensionLowerLimit = -0.130000f;
    m_OriginalHandlingData[161].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[161].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[161].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[161].uiModelFlags = 0x1100;
    m_OriginalHandlingData[161].uiHandlingFlags = 0x8804;
    m_OriginalHandlingData[161].fSeatOffsetDistance = 0.260000f;
    m_OriginalHandlingData[161].uiMonetary = 9000;
    m_OriginalHandlingData[161].ucHeadLight = 1;
    m_OriginalHandlingData[161].ucTailLight = 1;
    m_OriginalHandlingData[161].ucAnimGroup = 0;

    m_OriginalHandlingData[162].iVehicleID = 162;
    m_OriginalHandlingData[162].fMass = 500.000000f;
    m_OriginalHandlingData[162].fTurnMass = 161.699997f;
    m_OriginalHandlingData[162].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[162].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[162].uiPercentSubmerged = 103;
    m_OriginalHandlingData[162].fTractionMultiplier = 1.600000f;
    m_OriginalHandlingData[162].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[162].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[162].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[162].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[162].Transmission.fEngineAcceleration = 20.000000f;
    m_OriginalHandlingData[162].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[162].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[162].fBrakeDeceleration = 15.000000f;
    m_OriginalHandlingData[162].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[162].bABS = false;
    m_OriginalHandlingData[162].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[162].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[162].fTractionBias = 0.480000f;
    m_OriginalHandlingData[162].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[162].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[162].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[162].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[162].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[162].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[162].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[162].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[162].uiModelFlags = 0x1002000;
    m_OriginalHandlingData[162].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[162].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[162].uiMonetary = 10000;
    m_OriginalHandlingData[162].ucHeadLight = 1;
    m_OriginalHandlingData[162].ucTailLight = 1;
    m_OriginalHandlingData[162].ucAnimGroup = 4;

    m_OriginalHandlingData[163].iVehicleID = 163;
    m_OriginalHandlingData[163].fMass = 350.000000f;
    m_OriginalHandlingData[163].fTurnMass = 119.599998f;
    m_OriginalHandlingData[163].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[163].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.100000f);
    m_OriginalHandlingData[163].uiPercentSubmerged = 103;
    m_OriginalHandlingData[163].fTractionMultiplier = 1.800000f;
    m_OriginalHandlingData[163].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[163].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[163].Transmission.ucNumberOfGears = 3;
    m_OriginalHandlingData[163].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[163].Transmission.fEngineAcceleration = 12.000000f;
    m_OriginalHandlingData[163].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[163].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[163].fBrakeDeceleration = 14.000000f;
    m_OriginalHandlingData[163].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[163].bABS = false;
    m_OriginalHandlingData[163].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[163].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[163].fTractionBias = 0.480000f;
    m_OriginalHandlingData[163].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[163].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[163].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[163].fSuspensionUpperLimit = 0.120000f;
    m_OriginalHandlingData[163].fSuspensionLowerLimit = -0.170000f;
    m_OriginalHandlingData[163].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[163].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[163].fCollisionDamageMultiplier = 0.110000f;
    m_OriginalHandlingData[163].uiModelFlags = 0x1000000;
    m_OriginalHandlingData[163].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[163].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[163].uiMonetary = 10000;
    m_OriginalHandlingData[163].ucHeadLight = 1;
    m_OriginalHandlingData[163].ucTailLight = 1;
    m_OriginalHandlingData[163].ucAnimGroup = 5;

    m_OriginalHandlingData[164].iVehicleID = 164;
    m_OriginalHandlingData[164].fMass = 500.000000f;
    m_OriginalHandlingData[164].fTurnMass = 195.000000f;
    m_OriginalHandlingData[164].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[164].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[164].uiPercentSubmerged = 103;
    m_OriginalHandlingData[164].fTractionMultiplier = 1.600000f;
    m_OriginalHandlingData[164].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[164].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[164].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[164].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[164].Transmission.fEngineAcceleration = 20.000000f;
    m_OriginalHandlingData[164].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[164].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[164].fBrakeDeceleration = 14.000000f;
    m_OriginalHandlingData[164].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[164].bABS = false;
    m_OriginalHandlingData[164].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[164].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[164].fTractionBias = 0.480000f;
    m_OriginalHandlingData[164].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[164].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[164].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[164].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[164].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[164].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[164].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[164].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[164].uiModelFlags = 0x1000000;
    m_OriginalHandlingData[164].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[164].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[164].uiMonetary = 10000;
    m_OriginalHandlingData[164].ucHeadLight = 1;
    m_OriginalHandlingData[164].ucTailLight = 1;
    m_OriginalHandlingData[164].ucAnimGroup = 7;

    m_OriginalHandlingData[165].iVehicleID = 165;
    m_OriginalHandlingData[165].fMass = 500.000000f;
    m_OriginalHandlingData[165].fTurnMass = 200.000000f;
    m_OriginalHandlingData[165].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[165].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[165].uiPercentSubmerged = 103;
    m_OriginalHandlingData[165].fTractionMultiplier = 1.500000f;
    m_OriginalHandlingData[165].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[165].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[165].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[165].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[165].Transmission.fEngineAcceleration = 20.000000f;
    m_OriginalHandlingData[165].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[165].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[165].fBrakeDeceleration = 15.000000f;
    m_OriginalHandlingData[165].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[165].bABS = false;
    m_OriginalHandlingData[165].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[165].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[165].fTractionBias = 0.480000f;
    m_OriginalHandlingData[165].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[165].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[165].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[165].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[165].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[165].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[165].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[165].fCollisionDamageMultiplier = 0.110000f;
    m_OriginalHandlingData[165].uiModelFlags = 0x1000000;
    m_OriginalHandlingData[165].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[165].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[165].uiMonetary = 10000;
    m_OriginalHandlingData[165].ucHeadLight = 1;
    m_OriginalHandlingData[165].ucTailLight = 1;
    m_OriginalHandlingData[165].ucAnimGroup = 4;

    m_OriginalHandlingData[166].iVehicleID = 166;
    m_OriginalHandlingData[166].fMass = 400.000000f;
    m_OriginalHandlingData[166].fTurnMass = 200.000000f;
    m_OriginalHandlingData[166].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[166].vecCenterOfMass = CVector(0.000000f, 0.080000f, -0.090000f);
    m_OriginalHandlingData[166].uiPercentSubmerged = 103;
    m_OriginalHandlingData[166].fTractionMultiplier = 1.800000f;
    m_OriginalHandlingData[166].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[166].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[166].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[166].Transmission.uiHandlingFlags = 2;
    m_OriginalHandlingData[166].Transmission.fEngineAcceleration = 24.000000f;
    m_OriginalHandlingData[166].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[166].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[166].fBrakeDeceleration = 15.000000f;
    m_OriginalHandlingData[166].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[166].bABS = false;
    m_OriginalHandlingData[166].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[166].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[166].fTractionBias = 0.480000f;
    m_OriginalHandlingData[166].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[166].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[166].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[166].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[166].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[166].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[166].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[166].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[166].uiModelFlags = 0x1002000;
    m_OriginalHandlingData[166].uiHandlingFlags = 0x2;
    m_OriginalHandlingData[166].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[166].uiMonetary = 10000;
    m_OriginalHandlingData[166].ucHeadLight = 1;
    m_OriginalHandlingData[166].ucTailLight = 1;
    m_OriginalHandlingData[166].ucAnimGroup = 4;

    m_OriginalHandlingData[167].iVehicleID = 167;
    m_OriginalHandlingData[167].fMass = 500.000000f;
    m_OriginalHandlingData[167].fTurnMass = 240.000000f;
    m_OriginalHandlingData[167].fDragCoeff = 4.500000f;
    m_OriginalHandlingData[167].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[167].uiPercentSubmerged = 103;
    m_OriginalHandlingData[167].fTractionMultiplier = 1.500000f;
    m_OriginalHandlingData[167].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[167].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[167].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[167].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[167].Transmission.fEngineAcceleration = 20.000000f;
    m_OriginalHandlingData[167].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[167].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[167].fBrakeDeceleration = 15.000000f;
    m_OriginalHandlingData[167].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[167].bABS = false;
    m_OriginalHandlingData[167].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[167].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[167].fTractionBias = 0.460000f;
    m_OriginalHandlingData[167].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[167].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[167].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[167].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[167].fSuspensionLowerLimit = -0.160000f;
    m_OriginalHandlingData[167].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[167].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[167].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[167].uiModelFlags = 0x1002000;
    m_OriginalHandlingData[167].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[167].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[167].uiMonetary = 10000;
    m_OriginalHandlingData[167].ucHeadLight = 1;
    m_OriginalHandlingData[167].ucTailLight = 1;
    m_OriginalHandlingData[167].ucAnimGroup = 4;

    m_OriginalHandlingData[168].iVehicleID = 168;
    m_OriginalHandlingData[168].fMass = 500.000000f;
    m_OriginalHandlingData[168].fTurnMass = 200.000000f;
    m_OriginalHandlingData[168].fDragCoeff = 4.500000f;
    m_OriginalHandlingData[168].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[168].uiPercentSubmerged = 103;
    m_OriginalHandlingData[168].fTractionMultiplier = 1.400000f;
    m_OriginalHandlingData[168].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[168].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[168].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[168].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[168].Transmission.fEngineAcceleration = 20.000000f;
    m_OriginalHandlingData[168].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[168].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[168].fBrakeDeceleration = 15.000000f;
    m_OriginalHandlingData[168].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[168].bABS = false;
    m_OriginalHandlingData[168].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[168].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[168].fTractionBias = 0.480000f;
    m_OriginalHandlingData[168].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[168].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[168].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[168].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[168].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[168].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[168].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[168].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[168].uiModelFlags = 0x1000000;
    m_OriginalHandlingData[168].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[168].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[168].uiMonetary = 10000;
    m_OriginalHandlingData[168].ucHeadLight = 1;
    m_OriginalHandlingData[168].ucTailLight = 1;
    m_OriginalHandlingData[168].ucAnimGroup = 4;

    m_OriginalHandlingData[169].iVehicleID = 169;
    m_OriginalHandlingData[169].fMass = 800.000000f;
    m_OriginalHandlingData[169].fTurnMass = 600.000000f;
    m_OriginalHandlingData[169].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[169].vecCenterOfMass = CVector(0.000000f, 0.100000f, 0.000000f);
    m_OriginalHandlingData[169].uiPercentSubmerged = 103;
    m_OriginalHandlingData[169].fTractionMultiplier = 1.400000f;
    m_OriginalHandlingData[169].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[169].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[169].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[169].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[169].Transmission.fEngineAcceleration = 16.000000f;
    m_OriginalHandlingData[169].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[169].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[169].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[169].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[169].bABS = false;
    m_OriginalHandlingData[169].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[169].fTractionLoss = 0.850000f;
    m_OriginalHandlingData[169].fTractionBias = 0.480000f;
    m_OriginalHandlingData[169].fSuspensionForceLevel = 0.650000f;
    m_OriginalHandlingData[169].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[169].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[169].fSuspensionUpperLimit = 0.090000f;
    m_OriginalHandlingData[169].fSuspensionLowerLimit = -0.110000f;
    m_OriginalHandlingData[169].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[169].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[169].fCollisionDamageMultiplier = 0.240000f;
    m_OriginalHandlingData[169].uiModelFlags = 0x41002000;
    m_OriginalHandlingData[169].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[169].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[169].uiMonetary = 10000;
    m_OriginalHandlingData[169].ucHeadLight = 1;
    m_OriginalHandlingData[169].ucTailLight = 1;
    m_OriginalHandlingData[169].ucAnimGroup = 8;

    m_OriginalHandlingData[170].iVehicleID = 170;
    m_OriginalHandlingData[170].fMass = 400.000000f;
    m_OriginalHandlingData[170].fTurnMass = 300.000000f;
    m_OriginalHandlingData[170].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[170].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.200000f);
    m_OriginalHandlingData[170].uiPercentSubmerged = 70;
    m_OriginalHandlingData[170].fTractionMultiplier = 0.700000f;
    m_OriginalHandlingData[170].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[170].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[170].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[170].Transmission.uiHandlingFlags = 517;
    m_OriginalHandlingData[170].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[170].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[170].Transmission.fMaxVelocity = 160.000000f;
    m_OriginalHandlingData[170].fBrakeDeceleration = 8.000000f;
    m_OriginalHandlingData[170].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[170].bABS = false;
    m_OriginalHandlingData[170].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[170].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[170].fTractionBias = 0.490000f;
    m_OriginalHandlingData[170].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[170].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[170].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[170].fSuspensionUpperLimit = 0.150000f;
    m_OriginalHandlingData[170].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[170].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[170].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[170].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[170].uiModelFlags = 0x281300;
    m_OriginalHandlingData[170].uiHandlingFlags = 0x205;
    m_OriginalHandlingData[170].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[170].uiMonetary = 9000;
    m_OriginalHandlingData[170].ucHeadLight = 1;
    m_OriginalHandlingData[170].ucTailLight = 1;
    m_OriginalHandlingData[170].ucAnimGroup = 12;

    m_OriginalHandlingData[171].iVehicleID = 171;
    m_OriginalHandlingData[171].fMass = 100.000000f;
    m_OriginalHandlingData[171].fTurnMass = 39.000000f;
    m_OriginalHandlingData[171].fDragCoeff = 7.000000f;
    m_OriginalHandlingData[171].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[171].uiPercentSubmerged = 103;
    m_OriginalHandlingData[171].fTractionMultiplier = 1.600000f;
    m_OriginalHandlingData[171].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[171].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[171].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[171].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[171].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[171].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[171].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[171].fBrakeDeceleration = 19.000000f;
    m_OriginalHandlingData[171].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[171].bABS = false;
    m_OriginalHandlingData[171].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[171].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[171].fTractionBias = 0.480000f;
    m_OriginalHandlingData[171].fSuspensionForceLevel = 0.800000f;
    m_OriginalHandlingData[171].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[171].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[171].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[171].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[171].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[171].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[171].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[171].uiModelFlags = 0x41000000;
    m_OriginalHandlingData[171].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[171].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[171].uiMonetary = 10000;
    m_OriginalHandlingData[171].ucHeadLight = 1;
    m_OriginalHandlingData[171].ucTailLight = 1;
    m_OriginalHandlingData[171].ucAnimGroup = 9;

    m_OriginalHandlingData[172].iVehicleID = 172;
    m_OriginalHandlingData[172].fMass = 100.000000f;
    m_OriginalHandlingData[172].fTurnMass = 39.000000f;
    m_OriginalHandlingData[172].fDragCoeff = 6.000000f;
    m_OriginalHandlingData[172].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[172].uiPercentSubmerged = 103;
    m_OriginalHandlingData[172].fTractionMultiplier = 1.600000f;
    m_OriginalHandlingData[172].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[172].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[172].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[172].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[172].Transmission.fEngineAcceleration = 7.200000f;
    m_OriginalHandlingData[172].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[172].Transmission.fMaxVelocity = 120.000000f;
    m_OriginalHandlingData[172].fBrakeDeceleration = 19.000000f;
    m_OriginalHandlingData[172].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[172].bABS = false;
    m_OriginalHandlingData[172].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[172].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[172].fTractionBias = 0.480000f;
    m_OriginalHandlingData[172].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[172].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[172].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[172].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[172].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[172].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[172].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[172].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[172].uiModelFlags = 0x41000000;
    m_OriginalHandlingData[172].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[172].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[172].uiMonetary = 10000;
    m_OriginalHandlingData[172].ucHeadLight = 1;
    m_OriginalHandlingData[172].ucTailLight = 1;
    m_OriginalHandlingData[172].ucAnimGroup = 11;

    m_OriginalHandlingData[173].iVehicleID = 173;
    m_OriginalHandlingData[173].fMass = 100.000000f;
    m_OriginalHandlingData[173].fTurnMass = 60.000000f;
    m_OriginalHandlingData[173].fDragCoeff = 5.000000f;
    m_OriginalHandlingData[173].vecCenterOfMass = CVector(0.000000f, 0.050000f, -0.090000f);
    m_OriginalHandlingData[173].uiPercentSubmerged = 103;
    m_OriginalHandlingData[173].fTractionMultiplier = 1.600000f;
    m_OriginalHandlingData[173].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[173].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[173].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[173].Transmission.uiHandlingFlags = 2;
    m_OriginalHandlingData[173].Transmission.fEngineAcceleration = 10.000000f;
    m_OriginalHandlingData[173].Transmission.fEngineInertia = 15.000000f;
    m_OriginalHandlingData[173].Transmission.fMaxVelocity = 140.000000f;
    m_OriginalHandlingData[173].fBrakeDeceleration = 19.000000f;
    m_OriginalHandlingData[173].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[173].bABS = false;
    m_OriginalHandlingData[173].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[173].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[173].fTractionBias = 0.480000f;
    m_OriginalHandlingData[173].fSuspensionForceLevel = 0.850000f;
    m_OriginalHandlingData[173].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[173].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[173].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[173].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[173].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[173].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[173].fCollisionDamageMultiplier = 0.150000f;
    m_OriginalHandlingData[173].uiModelFlags = 0x41000000;
    m_OriginalHandlingData[173].uiHandlingFlags = 0x2;
    m_OriginalHandlingData[173].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[173].uiMonetary = 10000;
    m_OriginalHandlingData[173].ucHeadLight = 1;
    m_OriginalHandlingData[173].ucTailLight = 1;
    m_OriginalHandlingData[173].ucAnimGroup = 10;

    m_OriginalHandlingData[174].iVehicleID = 174;
    m_OriginalHandlingData[174].fMass = 800.000000f;
    m_OriginalHandlingData[174].fTurnMass = 403.299988f;
    m_OriginalHandlingData[174].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[174].vecCenterOfMass = CVector(0.000000f, 0.100000f, 0.000000f);
    m_OriginalHandlingData[174].uiPercentSubmerged = 103;
    m_OriginalHandlingData[174].fTractionMultiplier = 1.200000f;
    m_OriginalHandlingData[174].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[174].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[174].Transmission.ucNumberOfGears = 4;
    m_OriginalHandlingData[174].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[174].Transmission.fEngineAcceleration = 16.000000f;
    m_OriginalHandlingData[174].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[174].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[174].fBrakeDeceleration = 10.000000f;
    m_OriginalHandlingData[174].fBrakeBias = 0.550000f;
    m_OriginalHandlingData[174].bABS = false;
    m_OriginalHandlingData[174].fSteeringLock = 35.000000f;
    m_OriginalHandlingData[174].fTractionLoss = 0.820000f;
    m_OriginalHandlingData[174].fTractionBias = 0.510000f;
    m_OriginalHandlingData[174].fSuspensionForceLevel = 0.650000f;
    m_OriginalHandlingData[174].fSuspensionDamping = 0.200000f;
    m_OriginalHandlingData[174].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[174].fSuspensionUpperLimit = 0.090000f;
    m_OriginalHandlingData[174].fSuspensionLowerLimit = -0.110000f;
    m_OriginalHandlingData[174].fSuspensionFrontRearBias = 0.550000f;
    m_OriginalHandlingData[174].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[174].fCollisionDamageMultiplier = 0.240000f;
    m_OriginalHandlingData[174].uiModelFlags = 0x1002000;
    m_OriginalHandlingData[174].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[174].fSeatOffsetDistance = 0.000000f;
    m_OriginalHandlingData[174].uiMonetary = 10000;
    m_OriginalHandlingData[174].ucHeadLight = 1;
    m_OriginalHandlingData[174].ucTailLight = 1;
    m_OriginalHandlingData[174].ucAnimGroup = 6;

    m_OriginalHandlingData[175].iVehicleID = 175;
    m_OriginalHandlingData[175].fMass = 2200.000000f;
    m_OriginalHandlingData[175].fTurnMass = 29333.300781f;
    m_OriginalHandlingData[175].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[175].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[175].uiPercentSubmerged = 14;
    m_OriginalHandlingData[175].fTractionMultiplier = 2.300000f;
    m_OriginalHandlingData[175].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[175].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[175].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[175].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[175].Transmission.fEngineAcceleration = 0.680000f;
    m_OriginalHandlingData[175].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[175].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[175].fBrakeDeceleration = 0.050000f;
    m_OriginalHandlingData[175].fBrakeBias = 0.010000f;
    m_OriginalHandlingData[175].bABS = false;
    m_OriginalHandlingData[175].fSteeringLock = 24.000000f;
    m_OriginalHandlingData[175].fTractionLoss = 15.000000f;
    m_OriginalHandlingData[175].fTractionBias = 0.580000f;
    m_OriginalHandlingData[175].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[175].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[175].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[175].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[175].fSuspensionLowerLimit = 0.100000f;
    m_OriginalHandlingData[175].fSuspensionFrontRearBias = 0.000000f;
    m_OriginalHandlingData[175].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[175].fCollisionDamageMultiplier = 0.330000f;
    m_OriginalHandlingData[175].uiModelFlags = 0x8000000;
    m_OriginalHandlingData[175].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[175].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[175].uiMonetary = 40000;
    m_OriginalHandlingData[175].ucHeadLight = 0;
    m_OriginalHandlingData[175].ucTailLight = 1;
    m_OriginalHandlingData[175].ucAnimGroup = 0;

    m_OriginalHandlingData[176].iVehicleID = 176;
    m_OriginalHandlingData[176].fMass = 2200.000000f;
    m_OriginalHandlingData[176].fTurnMass = 20210.699219f;
    m_OriginalHandlingData[176].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[176].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[176].uiPercentSubmerged = 22;
    m_OriginalHandlingData[176].fTractionMultiplier = 2.500000f;
    m_OriginalHandlingData[176].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[176].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[176].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[176].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[176].Transmission.fEngineAcceleration = 1.000000f;
    m_OriginalHandlingData[176].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[176].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[176].fBrakeDeceleration = 0.040000f;
    m_OriginalHandlingData[176].fBrakeBias = 0.010000f;
    m_OriginalHandlingData[176].bABS = false;
    m_OriginalHandlingData[176].fSteeringLock = 20.000000f;
    m_OriginalHandlingData[176].fTractionLoss = 15.000000f;
    m_OriginalHandlingData[176].fTractionBias = 0.650000f;
    m_OriginalHandlingData[176].fSuspensionForceLevel = 1.300000f;
    m_OriginalHandlingData[176].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[176].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[176].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[176].fSuspensionLowerLimit = 0.500000f;
    m_OriginalHandlingData[176].fSuspensionFrontRearBias = 2.000000f;
    m_OriginalHandlingData[176].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[176].fCollisionDamageMultiplier = 0.390000f;
    m_OriginalHandlingData[176].uiModelFlags = 0x8000400;
    m_OriginalHandlingData[176].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[176].fSeatOffsetDistance = 0.700000f;
    m_OriginalHandlingData[176].uiMonetary = 30000;
    m_OriginalHandlingData[176].ucHeadLight = 0;
    m_OriginalHandlingData[176].ucTailLight = 1;
    m_OriginalHandlingData[176].ucAnimGroup = 0;

    m_OriginalHandlingData[177].iVehicleID = 177;
    m_OriginalHandlingData[177].fMass = 5000.000000f;
    m_OriginalHandlingData[177].fTurnMass = 25520.800781f;
    m_OriginalHandlingData[177].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[177].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[177].uiPercentSubmerged = 15;
    m_OriginalHandlingData[177].fTractionMultiplier = -1.500000f;
    m_OriginalHandlingData[177].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[177].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[177].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[177].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[177].Transmission.fEngineAcceleration = 0.280000f;
    m_OriginalHandlingData[177].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[177].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[177].fBrakeDeceleration = 0.020000f;
    m_OriginalHandlingData[177].fBrakeBias = 0.020000f;
    m_OriginalHandlingData[177].bABS = false;
    m_OriginalHandlingData[177].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[177].fTractionLoss = 15.000000f;
    m_OriginalHandlingData[177].fTractionBias = 0.450000f;
    m_OriginalHandlingData[177].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[177].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[177].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[177].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[177].fSuspensionLowerLimit = 0.100000f;
    m_OriginalHandlingData[177].fSuspensionFrontRearBias = 0.000000f;
    m_OriginalHandlingData[177].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[177].fCollisionDamageMultiplier = 0.380000f;
    m_OriginalHandlingData[177].uiModelFlags = 0x8000000;
    m_OriginalHandlingData[177].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[177].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[177].uiMonetary = 25000;
    m_OriginalHandlingData[177].ucHeadLight = 0;
    m_OriginalHandlingData[177].ucTailLight = 1;
    m_OriginalHandlingData[177].ucAnimGroup = 0;

    m_OriginalHandlingData[178].iVehicleID = 178;
    m_OriginalHandlingData[178].fMass = 3000.000000f;
    m_OriginalHandlingData[178].fTurnMass = 17312.500000f;
    m_OriginalHandlingData[178].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[178].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[178].uiPercentSubmerged = 15;
    m_OriginalHandlingData[178].fTractionMultiplier = -4.000000f;
    m_OriginalHandlingData[178].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[178].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[178].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[178].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[178].Transmission.fEngineAcceleration = 0.200000f;
    m_OriginalHandlingData[178].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[178].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[178].fBrakeDeceleration = 0.020000f;
    m_OriginalHandlingData[178].fBrakeBias = 0.000000f;
    m_OriginalHandlingData[178].bABS = false;
    m_OriginalHandlingData[178].fSteeringLock = 20.000000f;
    m_OriginalHandlingData[178].fTractionLoss = 25.000000f;
    m_OriginalHandlingData[178].fTractionBias = 0.500000f;
    m_OriginalHandlingData[178].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[178].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[178].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[178].fSuspensionUpperLimit = 15.000000f;
    m_OriginalHandlingData[178].fSuspensionLowerLimit = 0.100000f;
    m_OriginalHandlingData[178].fSuspensionFrontRearBias = 0.000000f;
    m_OriginalHandlingData[178].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[178].fCollisionDamageMultiplier = 0.230000f;
    m_OriginalHandlingData[178].uiModelFlags = 0x8000000;
    m_OriginalHandlingData[178].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[178].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[178].uiMonetary = 70000;
    m_OriginalHandlingData[178].ucHeadLight = 0;
    m_OriginalHandlingData[178].ucTailLight = 1;
    m_OriginalHandlingData[178].ucAnimGroup = 0;

    m_OriginalHandlingData[179].iVehicleID = 179;
    m_OriginalHandlingData[179].fMass = 2200.000000f;
    m_OriginalHandlingData[179].fTurnMass = 29333.300781f;
    m_OriginalHandlingData[179].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[179].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[179].uiPercentSubmerged = 42;
    m_OriginalHandlingData[179].fTractionMultiplier = 3.000000f;
    m_OriginalHandlingData[179].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[179].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[179].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[179].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[179].Transmission.fEngineAcceleration = 1.200000f;
    m_OriginalHandlingData[179].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[179].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[179].fBrakeDeceleration = 0.020000f;
    m_OriginalHandlingData[179].fBrakeBias = 0.000000f;
    m_OriginalHandlingData[179].bABS = false;
    m_OriginalHandlingData[179].fSteeringLock = 24.000000f;
    m_OriginalHandlingData[179].fTractionLoss = 15.000000f;
    m_OriginalHandlingData[179].fTractionBias = 0.650000f;
    m_OriginalHandlingData[179].fSuspensionForceLevel = 0.450000f;
    m_OriginalHandlingData[179].fSuspensionDamping = 5.000000f;
    m_OriginalHandlingData[179].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[179].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[179].fSuspensionLowerLimit = 0.050000f;
    m_OriginalHandlingData[179].fSuspensionFrontRearBias = 0.000000f;
    m_OriginalHandlingData[179].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[179].fCollisionDamageMultiplier = 0.330000f;
    m_OriginalHandlingData[179].uiModelFlags = 0x8000400;
    m_OriginalHandlingData[179].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[179].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[179].uiMonetary = 60000;
    m_OriginalHandlingData[179].ucHeadLight = 0;
    m_OriginalHandlingData[179].ucTailLight = 1;
    m_OriginalHandlingData[179].ucAnimGroup = 0;

    m_OriginalHandlingData[180].iVehicleID = 180;
    m_OriginalHandlingData[180].fMass = 2200.000000f;
    m_OriginalHandlingData[180].fTurnMass = 29333.300781f;
    m_OriginalHandlingData[180].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[180].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[180].uiPercentSubmerged = 10;
    m_OriginalHandlingData[180].fTractionMultiplier = 2.200000f;
    m_OriginalHandlingData[180].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[180].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[180].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[180].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[180].Transmission.fEngineAcceleration = 0.560000f;
    m_OriginalHandlingData[180].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[180].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[180].fBrakeDeceleration = 0.050000f;
    m_OriginalHandlingData[180].fBrakeBias = 0.010000f;
    m_OriginalHandlingData[180].bABS = false;
    m_OriginalHandlingData[180].fSteeringLock = 24.000000f;
    m_OriginalHandlingData[180].fTractionLoss = 12.000000f;
    m_OriginalHandlingData[180].fTractionBias = 0.450000f;
    m_OriginalHandlingData[180].fSuspensionForceLevel = 1.800000f;
    m_OriginalHandlingData[180].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[180].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[180].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[180].fSuspensionLowerLimit = 0.100000f;
    m_OriginalHandlingData[180].fSuspensionFrontRearBias = 0.000000f;
    m_OriginalHandlingData[180].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[180].fCollisionDamageMultiplier = 0.330000f;
    m_OriginalHandlingData[180].uiModelFlags = 0x8000400;
    m_OriginalHandlingData[180].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[180].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[180].uiMonetary = 73000;
    m_OriginalHandlingData[180].ucHeadLight = 0;
    m_OriginalHandlingData[180].ucTailLight = 1;
    m_OriginalHandlingData[180].ucAnimGroup = 0;

    m_OriginalHandlingData[181].iVehicleID = 181;
    m_OriginalHandlingData[181].fMass = 1200.000000f;
    m_OriginalHandlingData[181].fTurnMass = 6525.000000f;
    m_OriginalHandlingData[181].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[181].vecCenterOfMass = CVector(0.000000f, -0.300000f, 0.000000f);
    m_OriginalHandlingData[181].uiPercentSubmerged = 14;
    m_OriginalHandlingData[181].fTractionMultiplier = 2.000000f;
    m_OriginalHandlingData[181].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[181].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[181].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[181].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[181].Transmission.fEngineAcceleration = 0.640000f;
    m_OriginalHandlingData[181].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[181].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[181].fBrakeDeceleration = 0.050000f;
    m_OriginalHandlingData[181].fBrakeBias = 0.010000f;
    m_OriginalHandlingData[181].bABS = false;
    m_OriginalHandlingData[181].fSteeringLock = 24.000000f;
    m_OriginalHandlingData[181].fTractionLoss = 4.200000f;
    m_OriginalHandlingData[181].fTractionBias = 0.700000f;
    m_OriginalHandlingData[181].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[181].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[181].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[181].fSuspensionUpperLimit = 3.200000f;
    m_OriginalHandlingData[181].fSuspensionLowerLimit = 0.100000f;
    m_OriginalHandlingData[181].fSuspensionFrontRearBias = 2.500000f;
    m_OriginalHandlingData[181].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[181].fCollisionDamageMultiplier = 0.180000f;
    m_OriginalHandlingData[181].uiModelFlags = 0x8000000;
    m_OriginalHandlingData[181].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[181].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[181].uiMonetary = 10000;
    m_OriginalHandlingData[181].ucHeadLight = 0;
    m_OriginalHandlingData[181].ucTailLight = 1;
    m_OriginalHandlingData[181].ucAnimGroup = 0;

    m_OriginalHandlingData[182].iVehicleID = 182;
    m_OriginalHandlingData[182].fMass = 800.000000f;
    m_OriginalHandlingData[182].fTurnMass = 1483.300049f;
    m_OriginalHandlingData[182].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[182].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[182].uiPercentSubmerged = 16;
    m_OriginalHandlingData[182].fTractionMultiplier = 3.500000f;
    m_OriginalHandlingData[182].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[182].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[182].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[182].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[182].Transmission.fEngineAcceleration = 0.480000f;
    m_OriginalHandlingData[182].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[182].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[182].fBrakeDeceleration = 0.070000f;
    m_OriginalHandlingData[182].fBrakeBias = 0.010000f;
    m_OriginalHandlingData[182].bABS = false;
    m_OriginalHandlingData[182].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[182].fTractionLoss = 3.500000f;
    m_OriginalHandlingData[182].fTractionBias = 1.000000f;
    m_OriginalHandlingData[182].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[182].fSuspensionDamping = 4.500000f;
    m_OriginalHandlingData[182].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[182].fSuspensionUpperLimit = 3.500000f;
    m_OriginalHandlingData[182].fSuspensionLowerLimit = 0.100000f;
    m_OriginalHandlingData[182].fSuspensionFrontRearBias = 0.700000f;
    m_OriginalHandlingData[182].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[182].fCollisionDamageMultiplier = 0.120000f;
    m_OriginalHandlingData[182].uiModelFlags = 0x8000000;
    m_OriginalHandlingData[182].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[182].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[182].uiMonetary = 5000;
    m_OriginalHandlingData[182].ucHeadLight = 0;
    m_OriginalHandlingData[182].ucTailLight = 1;
    m_OriginalHandlingData[182].ucAnimGroup = 0;

    m_OriginalHandlingData[183].iVehicleID = 183;
    m_OriginalHandlingData[183].fMass = 5000.000000f;
    m_OriginalHandlingData[183].fTurnMass = 155520.796875f;
    m_OriginalHandlingData[183].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[183].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[183].uiPercentSubmerged = 10;
    m_OriginalHandlingData[183].fTractionMultiplier = -3.500000f;
    m_OriginalHandlingData[183].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[183].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[183].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[183].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[183].Transmission.fEngineAcceleration = 0.200000f;
    m_OriginalHandlingData[183].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[183].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[183].fBrakeDeceleration = 0.040000f;
    m_OriginalHandlingData[183].fBrakeBias = 0.030000f;
    m_OriginalHandlingData[183].bABS = false;
    m_OriginalHandlingData[183].fSteeringLock = 38.000000f;
    m_OriginalHandlingData[183].fTractionLoss = 25.000000f;
    m_OriginalHandlingData[183].fTractionBias = 0.400000f;
    m_OriginalHandlingData[183].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[183].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[183].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[183].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[183].fSuspensionLowerLimit = 0.000000f;
    m_OriginalHandlingData[183].fSuspensionFrontRearBias = 1.000000f;
    m_OriginalHandlingData[183].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[183].fCollisionDamageMultiplier = 0.380000f;
    m_OriginalHandlingData[183].uiModelFlags = 0x8000000;
    m_OriginalHandlingData[183].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[183].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[183].uiMonetary = 99000;
    m_OriginalHandlingData[183].ucHeadLight = 0;
    m_OriginalHandlingData[183].ucTailLight = 1;
    m_OriginalHandlingData[183].ucAnimGroup = 0;

    m_OriginalHandlingData[184].iVehicleID = 184;
    m_OriginalHandlingData[184].fMass = 3000.000000f;
    m_OriginalHandlingData[184].fTurnMass = 40000.000000f;
    m_OriginalHandlingData[184].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[184].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[184].uiPercentSubmerged = 35;
    m_OriginalHandlingData[184].fTractionMultiplier = 2.000000f;
    m_OriginalHandlingData[184].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[184].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[184].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[184].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[184].Transmission.fEngineAcceleration = 1.200000f;
    m_OriginalHandlingData[184].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[184].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[184].fBrakeDeceleration = 0.020000f;
    m_OriginalHandlingData[184].fBrakeBias = 0.000000f;
    m_OriginalHandlingData[184].bABS = false;
    m_OriginalHandlingData[184].fSteeringLock = 24.000000f;
    m_OriginalHandlingData[184].fTractionLoss = 15.000000f;
    m_OriginalHandlingData[184].fTractionBias = 0.500000f;
    m_OriginalHandlingData[184].fSuspensionForceLevel = 0.750000f;
    m_OriginalHandlingData[184].fSuspensionDamping = 4.000000f;
    m_OriginalHandlingData[184].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[184].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[184].fSuspensionLowerLimit = 0.300000f;
    m_OriginalHandlingData[184].fSuspensionFrontRearBias = 1.500000f;
    m_OriginalHandlingData[184].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[184].fCollisionDamageMultiplier = 0.450000f;
    m_OriginalHandlingData[184].uiModelFlags = 0x8000400;
    m_OriginalHandlingData[184].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[184].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[184].uiMonetary = 48000;
    m_OriginalHandlingData[184].ucHeadLight = 0;
    m_OriginalHandlingData[184].ucTailLight = 1;
    m_OriginalHandlingData[184].ucAnimGroup = 0;

    m_OriginalHandlingData[185].iVehicleID = 185;
    m_OriginalHandlingData[185].fMass = 2200.000000f;
    m_OriginalHandlingData[185].fTurnMass = 20210.699219f;
    m_OriginalHandlingData[185].fDragCoeff = 1.000000f;
    m_OriginalHandlingData[185].vecCenterOfMass = CVector(0.000000f, -1.000000f, 0.000000f);
    m_OriginalHandlingData[185].uiPercentSubmerged = 22;
    m_OriginalHandlingData[185].fTractionMultiplier = 1.500000f;
    m_OriginalHandlingData[185].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[185].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[185].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[185].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[185].Transmission.fEngineAcceleration = 0.600000f;
    m_OriginalHandlingData[185].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[185].Transmission.fMaxVelocity = 190.000000f;
    m_OriginalHandlingData[185].fBrakeDeceleration = 0.030000f;
    m_OriginalHandlingData[185].fBrakeBias = 0.010000f;
    m_OriginalHandlingData[185].bABS = false;
    m_OriginalHandlingData[185].fSteeringLock = 24.000000f;
    m_OriginalHandlingData[185].fTractionLoss = 15.000000f;
    m_OriginalHandlingData[185].fTractionBias = 0.650000f;
    m_OriginalHandlingData[185].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[185].fSuspensionDamping = 3.000000f;
    m_OriginalHandlingData[185].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[185].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[185].fSuspensionLowerLimit = 0.500000f;
    m_OriginalHandlingData[185].fSuspensionFrontRearBias = 2.000000f;
    m_OriginalHandlingData[185].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[185].fCollisionDamageMultiplier = 0.390000f;
    m_OriginalHandlingData[185].uiModelFlags = 0x8000400;
    m_OriginalHandlingData[185].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[185].fSeatOffsetDistance = 0.700000f;
    m_OriginalHandlingData[185].uiMonetary = 30000;
    m_OriginalHandlingData[185].ucHeadLight = 0;
    m_OriginalHandlingData[185].ucTailLight = 1;
    m_OriginalHandlingData[185].ucAnimGroup = 0;

    m_OriginalHandlingData[186].iVehicleID = 186;
    m_OriginalHandlingData[186].fMass = 5000.000000f;
    m_OriginalHandlingData[186].fTurnMass = 27083.300781f;
    m_OriginalHandlingData[186].fDragCoeff = 12.000000f;
    m_OriginalHandlingData[186].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[186].uiPercentSubmerged = 9;
    m_OriginalHandlingData[186].fTractionMultiplier = 0.830000f;
    m_OriginalHandlingData[186].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[186].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[186].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[186].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[186].Transmission.fEngineAcceleration = 0.680000f;
    m_OriginalHandlingData[186].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[186].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[186].fBrakeDeceleration = 0.010000f;
    m_OriginalHandlingData[186].fBrakeBias = 0.050000f;
    m_OriginalHandlingData[186].bABS = false;
    m_OriginalHandlingData[186].fSteeringLock = 24.000000f;
    m_OriginalHandlingData[186].fTractionLoss = 45.000000f;
    m_OriginalHandlingData[186].fTractionBias = 0.500000f;
    m_OriginalHandlingData[186].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[186].fSuspensionDamping = 0.750000f;
    m_OriginalHandlingData[186].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[186].fSuspensionUpperLimit = 0.100000f;
    m_OriginalHandlingData[186].fSuspensionLowerLimit = 0.000000f;
    m_OriginalHandlingData[186].fSuspensionFrontRearBias = 2.000000f;
    m_OriginalHandlingData[186].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[186].fCollisionDamageMultiplier = 0.050000f;
    m_OriginalHandlingData[186].uiModelFlags = 0x4000400;
    m_OriginalHandlingData[186].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[186].fSeatOffsetDistance = 1.000000f;
    m_OriginalHandlingData[186].uiMonetary = 10000;
    m_OriginalHandlingData[186].ucHeadLight = 0;
    m_OriginalHandlingData[186].ucTailLight = 1;
    m_OriginalHandlingData[186].ucAnimGroup = 0;

    m_OriginalHandlingData[187].iVehicleID = 187;
    m_OriginalHandlingData[187].fMass = 1900.000000f;
    m_OriginalHandlingData[187].fTurnMass = 4795.899902f;
    m_OriginalHandlingData[187].fDragCoeff = 20.000000f;
    m_OriginalHandlingData[187].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.200000f);
    m_OriginalHandlingData[187].uiPercentSubmerged = 85;
    m_OriginalHandlingData[187].fTractionMultiplier = 0.050000f;
    m_OriginalHandlingData[187].Transmission.ucDriveType = 'R';
    m_OriginalHandlingData[187].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[187].Transmission.ucNumberOfGears = 5;
    m_OriginalHandlingData[187].Transmission.uiHandlingFlags = 4194368;
    m_OriginalHandlingData[187].Transmission.fEngineAcceleration = 0.800000f;
    m_OriginalHandlingData[187].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[187].Transmission.fMaxVelocity = 150.000000f;
    m_OriginalHandlingData[187].fBrakeDeceleration = 1.000000f;
    m_OriginalHandlingData[187].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[187].bABS = false;
    m_OriginalHandlingData[187].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[187].fTractionLoss = 1.000000f;
    m_OriginalHandlingData[187].fTractionBias = 0.500000f;
    m_OriginalHandlingData[187].fSuspensionForceLevel = 0.500000f;
    m_OriginalHandlingData[187].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[187].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[187].fSuspensionUpperLimit = 0.330000f;
    m_OriginalHandlingData[187].fSuspensionLowerLimit = -0.250000f;
    m_OriginalHandlingData[187].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[187].fSuspensionAntiDiveMultiplier = 0.020000f;
    m_OriginalHandlingData[187].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[187].uiModelFlags = 0x301;
    m_OriginalHandlingData[187].uiHandlingFlags = 0x400040;
    m_OriginalHandlingData[187].fSeatOffsetDistance = 0.650000f;
    m_OriginalHandlingData[187].uiMonetary = 26000;
    m_OriginalHandlingData[187].ucHeadLight = 0;
    m_OriginalHandlingData[187].ucTailLight = 1;
    m_OriginalHandlingData[187].ucAnimGroup = 23;

    m_OriginalHandlingData[188].iVehicleID = 188;
    m_OriginalHandlingData[188].fMass = 5000.000000f;
    m_OriginalHandlingData[188].fTurnMass = 27083.300781f;
    m_OriginalHandlingData[188].fDragCoeff = 10.000000f;
    m_OriginalHandlingData[188].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[188].uiPercentSubmerged = 75;
    m_OriginalHandlingData[188].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[188].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[188].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[188].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[188].Transmission.uiHandlingFlags = 4194336;
    m_OriginalHandlingData[188].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[188].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[188].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[188].fBrakeDeceleration = 1.500000f;
    m_OriginalHandlingData[188].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[188].bABS = false;
    m_OriginalHandlingData[188].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[188].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[188].fTractionBias = 0.500000f;
    m_OriginalHandlingData[188].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[188].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[188].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[188].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[188].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[188].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[188].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[188].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[188].uiModelFlags = 0x4008108;
    m_OriginalHandlingData[188].uiHandlingFlags = 0x400020;
    m_OriginalHandlingData[188].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[188].uiMonetary = 45000;
    m_OriginalHandlingData[188].ucHeadLight = 0;
    m_OriginalHandlingData[188].ucTailLight = 1;
    m_OriginalHandlingData[188].ucAnimGroup = 14;

    m_OriginalHandlingData[189].iVehicleID = 189;
    m_OriginalHandlingData[189].fMass = 10000.000000f;
    m_OriginalHandlingData[189].fTurnMass = 80000.000000f;
    m_OriginalHandlingData[189].fDragCoeff = 14.000000f;
    m_OriginalHandlingData[189].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[189].uiPercentSubmerged = 75;
    m_OriginalHandlingData[189].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[189].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[189].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[189].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[189].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[189].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[189].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[189].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[189].fBrakeDeceleration = 1.500000f;
    m_OriginalHandlingData[189].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[189].bABS = false;
    m_OriginalHandlingData[189].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[189].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[189].fTractionBias = 0.500000f;
    m_OriginalHandlingData[189].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[189].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[189].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[189].fSuspensionUpperLimit = 1.000000f;
    m_OriginalHandlingData[189].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[189].fSuspensionFrontRearBias = 0.350000f;
    m_OriginalHandlingData[189].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[189].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[189].uiModelFlags = 0x24000008;
    m_OriginalHandlingData[189].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[189].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[189].uiMonetary = 45000;
    m_OriginalHandlingData[189].ucHeadLight = 0;
    m_OriginalHandlingData[189].ucTailLight = 1;
    m_OriginalHandlingData[189].ucAnimGroup = 13;

    m_OriginalHandlingData[190].iVehicleID = 190;
    m_OriginalHandlingData[190].fMass = 5000.000000f;
    m_OriginalHandlingData[190].fTurnMass = 27083.300781f;
    m_OriginalHandlingData[190].fDragCoeff = 15.000000f;
    m_OriginalHandlingData[190].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[190].uiPercentSubmerged = 75;
    m_OriginalHandlingData[190].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[190].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[190].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[190].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[190].Transmission.uiHandlingFlags = 4194336;
    m_OriginalHandlingData[190].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[190].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[190].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[190].fBrakeDeceleration = 1.500000f;
    m_OriginalHandlingData[190].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[190].bABS = false;
    m_OriginalHandlingData[190].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[190].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[190].fTractionBias = 0.500000f;
    m_OriginalHandlingData[190].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[190].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[190].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[190].fSuspensionUpperLimit = 0.550000f;
    m_OriginalHandlingData[190].fSuspensionLowerLimit = -0.050000f;
    m_OriginalHandlingData[190].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[190].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[190].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[190].uiModelFlags = 0x4008000;
    m_OriginalHandlingData[190].uiHandlingFlags = 0x400020;
    m_OriginalHandlingData[190].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[190].uiMonetary = 45000;
    m_OriginalHandlingData[190].ucHeadLight = 0;
    m_OriginalHandlingData[190].ucTailLight = 1;
    m_OriginalHandlingData[190].ucAnimGroup = 14;

    m_OriginalHandlingData[191].iVehicleID = 191;
    m_OriginalHandlingData[191].fMass = 5000.000000f;
    m_OriginalHandlingData[191].fTurnMass = 20000.000000f;
    m_OriginalHandlingData[191].fDragCoeff = 14.000000f;
    m_OriginalHandlingData[191].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[191].uiPercentSubmerged = 75;
    m_OriginalHandlingData[191].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[191].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[191].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[191].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[191].Transmission.uiHandlingFlags = 4194336;
    m_OriginalHandlingData[191].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[191].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[191].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[191].fBrakeDeceleration = 1.500000f;
    m_OriginalHandlingData[191].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[191].bABS = false;
    m_OriginalHandlingData[191].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[191].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[191].fTractionBias = 0.500000f;
    m_OriginalHandlingData[191].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[191].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[191].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[191].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[191].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[191].fSuspensionFrontRearBias = 0.900000f;
    m_OriginalHandlingData[191].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[191].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[191].uiModelFlags = 0x4008100;
    m_OriginalHandlingData[191].uiHandlingFlags = 0x400020;
    m_OriginalHandlingData[191].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[191].uiMonetary = 45000;
    m_OriginalHandlingData[191].ucHeadLight = 0;
    m_OriginalHandlingData[191].ucTailLight = 1;
    m_OriginalHandlingData[191].ucAnimGroup = 14;

    m_OriginalHandlingData[192].iVehicleID = 192;
    m_OriginalHandlingData[192].fMass = 15000.000000f;
    m_OriginalHandlingData[192].fTurnMass = 81250.000000f;
    m_OriginalHandlingData[192].fDragCoeff = 8.000000f;
    m_OriginalHandlingData[192].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[192].uiPercentSubmerged = 75;
    m_OriginalHandlingData[192].fTractionMultiplier = 0.550000f;
    m_OriginalHandlingData[192].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[192].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[192].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[192].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[192].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[192].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[192].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[192].fBrakeDeceleration = 1.500000f;
    m_OriginalHandlingData[192].fBrakeBias = 0.150000f;
    m_OriginalHandlingData[192].bABS = false;
    m_OriginalHandlingData[192].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[192].fTractionLoss = 0.800000f;
    m_OriginalHandlingData[192].fTractionBias = 0.700000f;
    m_OriginalHandlingData[192].fSuspensionForceLevel = 4.000000f;
    m_OriginalHandlingData[192].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[192].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[192].fSuspensionUpperLimit = 1.000000f;
    m_OriginalHandlingData[192].fSuspensionLowerLimit = -0.000000f;
    m_OriginalHandlingData[192].fSuspensionFrontRearBias = 0.300000f;
    m_OriginalHandlingData[192].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[192].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[192].uiModelFlags = 0x400C108;
    m_OriginalHandlingData[192].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[192].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[192].uiMonetary = 45000;
    m_OriginalHandlingData[192].ucHeadLight = 0;
    m_OriginalHandlingData[192].ucTailLight = 1;
    m_OriginalHandlingData[192].ucAnimGroup = 26;

    m_OriginalHandlingData[193].iVehicleID = 193;
    m_OriginalHandlingData[193].fMass = 9000.000000f;
    m_OriginalHandlingData[193].fTurnMass = 48750.000000f;
    m_OriginalHandlingData[193].fDragCoeff = 20.000000f;
    m_OriginalHandlingData[193].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[193].uiPercentSubmerged = 75;
    m_OriginalHandlingData[193].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[193].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[193].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[193].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[193].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[193].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[193].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[193].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[193].fBrakeDeceleration = 1.500000f;
    m_OriginalHandlingData[193].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[193].bABS = false;
    m_OriginalHandlingData[193].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[193].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[193].fTractionBias = 0.500000f;
    m_OriginalHandlingData[193].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[193].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[193].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[193].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[193].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[193].fSuspensionFrontRearBias = 0.800000f;
    m_OriginalHandlingData[193].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[193].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[193].uiModelFlags = 0x4008100;
    m_OriginalHandlingData[193].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[193].fSeatOffsetDistance = 0.600000f;
    m_OriginalHandlingData[193].uiMonetary = 45000;
    m_OriginalHandlingData[193].ucHeadLight = 0;
    m_OriginalHandlingData[193].ucTailLight = 1;
    m_OriginalHandlingData[193].ucAnimGroup = 27;

    m_OriginalHandlingData[194].iVehicleID = 194;
    m_OriginalHandlingData[194].fMass = 25000.000000f;
    m_OriginalHandlingData[194].fTurnMass = 438750.000000f;
    m_OriginalHandlingData[194].fDragCoeff = 10.000000f;
    m_OriginalHandlingData[194].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[194].uiPercentSubmerged = 75;
    m_OriginalHandlingData[194].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[194].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[194].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[194].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[194].Transmission.uiHandlingFlags = 4194336;
    m_OriginalHandlingData[194].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[194].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[194].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[194].fBrakeDeceleration = 1.000000f;
    m_OriginalHandlingData[194].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[194].bABS = false;
    m_OriginalHandlingData[194].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[194].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[194].fTractionBias = 0.500000f;
    m_OriginalHandlingData[194].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[194].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[194].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[194].fSuspensionUpperLimit = 0.400000f;
    m_OriginalHandlingData[194].fSuspensionLowerLimit = -0.300000f;
    m_OriginalHandlingData[194].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[194].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[194].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[194].uiModelFlags = 0x400C108;
    m_OriginalHandlingData[194].uiHandlingFlags = 0x400020;
    m_OriginalHandlingData[194].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[194].uiMonetary = 45000;
    m_OriginalHandlingData[194].ucHeadLight = 0;
    m_OriginalHandlingData[194].ucTailLight = 1;
    m_OriginalHandlingData[194].ucAnimGroup = 29;

    m_OriginalHandlingData[195].iVehicleID = 195;
    m_OriginalHandlingData[195].fMass = 60000.000000f;
    m_OriginalHandlingData[195].fTurnMass = 9000000.000000f;
    m_OriginalHandlingData[195].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[195].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[195].uiPercentSubmerged = 75;
    m_OriginalHandlingData[195].fTractionMultiplier = 1.500000f;
    m_OriginalHandlingData[195].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[195].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[195].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[195].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[195].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[195].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[195].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[195].fBrakeDeceleration = 1.000000f;
    m_OriginalHandlingData[195].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[195].bABS = false;
    m_OriginalHandlingData[195].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[195].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[195].fTractionBias = 0.850000f;
    m_OriginalHandlingData[195].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[195].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[195].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[195].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[195].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[195].fSuspensionFrontRearBias = 0.300000f;
    m_OriginalHandlingData[195].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[195].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[195].uiModelFlags = 0x2400C008;
    m_OriginalHandlingData[195].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[195].fSeatOffsetDistance = 0.700000f;
    m_OriginalHandlingData[195].uiMonetary = 45000;
    m_OriginalHandlingData[195].ucHeadLight = 0;
    m_OriginalHandlingData[195].ucTailLight = 1;
    m_OriginalHandlingData[195].ucAnimGroup = 15;

    m_OriginalHandlingData[196].iVehicleID = 196;
    m_OriginalHandlingData[196].fMass = 40000.000000f;
    m_OriginalHandlingData[196].fTurnMass = 3000000.000000f;
    m_OriginalHandlingData[196].fDragCoeff = 4.000000f;
    m_OriginalHandlingData[196].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[196].uiPercentSubmerged = 75;
    m_OriginalHandlingData[196].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[196].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[196].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[196].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[196].Transmission.uiHandlingFlags = 4456448;
    m_OriginalHandlingData[196].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[196].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[196].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[196].fBrakeDeceleration = 1.000000f;
    m_OriginalHandlingData[196].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[196].bABS = false;
    m_OriginalHandlingData[196].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[196].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[196].fTractionBias = 0.500000f;
    m_OriginalHandlingData[196].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[196].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[196].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[196].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[196].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[196].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[196].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[196].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[196].uiModelFlags = 0x24114108;
    m_OriginalHandlingData[196].uiHandlingFlags = 0x440000;
    m_OriginalHandlingData[196].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[196].uiMonetary = 45000;
    m_OriginalHandlingData[196].ucHeadLight = 0;
    m_OriginalHandlingData[196].ucTailLight = 1;
    m_OriginalHandlingData[196].ucAnimGroup = 0;

    m_OriginalHandlingData[197].iVehicleID = 197;
    m_OriginalHandlingData[197].fMass = 5000.000000f;
    m_OriginalHandlingData[197].fTurnMass = 27083.300781f;
    m_OriginalHandlingData[197].fDragCoeff = 12.000000f;
    m_OriginalHandlingData[197].vecCenterOfMass = CVector(0.000000f, 0.300000f, 0.000000f);
    m_OriginalHandlingData[197].uiPercentSubmerged = 75;
    m_OriginalHandlingData[197].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[197].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[197].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[197].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[197].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[197].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[197].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[197].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[197].fBrakeDeceleration = 1.500000f;
    m_OriginalHandlingData[197].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[197].bABS = false;
    m_OriginalHandlingData[197].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[197].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[197].fTractionBias = 0.500000f;
    m_OriginalHandlingData[197].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[197].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[197].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[197].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[197].fSuspensionLowerLimit = -0.050000f;
    m_OriginalHandlingData[197].fSuspensionFrontRearBias = 0.200000f;
    m_OriginalHandlingData[197].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[197].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[197].uiModelFlags = 0x4000000;
    m_OriginalHandlingData[197].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[197].fSeatOffsetDistance = 0.500000f;
    m_OriginalHandlingData[197].uiMonetary = 45000;
    m_OriginalHandlingData[197].ucHeadLight = 0;
    m_OriginalHandlingData[197].ucTailLight = 1;
    m_OriginalHandlingData[197].ucAnimGroup = 13;

    m_OriginalHandlingData[198].iVehicleID = 198;
    m_OriginalHandlingData[198].fMass = 2500.000000f;
    m_OriginalHandlingData[198].fTurnMass = 6041.700195f;
    m_OriginalHandlingData[198].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[198].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[198].uiPercentSubmerged = 75;
    m_OriginalHandlingData[198].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[198].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[198].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[198].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[198].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[198].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[198].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[198].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[198].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[198].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[198].bABS = false;
    m_OriginalHandlingData[198].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[198].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[198].fTractionBias = 0.500000f;
    m_OriginalHandlingData[198].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[198].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[198].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[198].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[198].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[198].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[198].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[198].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[198].uiModelFlags = 0x2004000;
    m_OriginalHandlingData[198].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[198].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[198].uiMonetary = 25000;
    m_OriginalHandlingData[198].ucHeadLight = 0;
    m_OriginalHandlingData[198].ucTailLight = 1;
    m_OriginalHandlingData[198].ucAnimGroup = 0;

    m_OriginalHandlingData[199].iVehicleID = 199;
    m_OriginalHandlingData[199].fMass = 3000.000000f;
    m_OriginalHandlingData[199].fTurnMass = 7250.000000f;
    m_OriginalHandlingData[199].fDragCoeff = 0.100000f;
    m_OriginalHandlingData[199].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[199].uiPercentSubmerged = 5;
    m_OriginalHandlingData[199].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[199].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[199].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[199].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[199].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[199].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[199].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[199].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[199].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[199].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[199].bABS = false;
    m_OriginalHandlingData[199].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[199].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[199].fTractionBias = 0.500000f;
    m_OriginalHandlingData[199].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[199].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[199].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[199].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[199].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[199].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[199].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[199].fCollisionDamageMultiplier = 0.600000f;
    m_OriginalHandlingData[199].uiModelFlags = 0x2004000;
    m_OriginalHandlingData[199].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[199].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[199].uiMonetary = 28000;
    m_OriginalHandlingData[199].ucHeadLight = 0;
    m_OriginalHandlingData[199].ucTailLight = 1;
    m_OriginalHandlingData[199].ucAnimGroup = 0;

    m_OriginalHandlingData[200].iVehicleID = 200;
    m_OriginalHandlingData[200].fMass = 5000.000000f;
    m_OriginalHandlingData[200].fTurnMass = 29270.800781f;
    m_OriginalHandlingData[200].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[200].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[200].uiPercentSubmerged = 75;
    m_OriginalHandlingData[200].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[200].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[200].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[200].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[200].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[200].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[200].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[200].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[200].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[200].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[200].bABS = false;
    m_OriginalHandlingData[200].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[200].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[200].fTractionBias = 0.500000f;
    m_OriginalHandlingData[200].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[200].fSuspensionDamping = 0.150000f;
    m_OriginalHandlingData[200].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[200].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[200].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[200].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[200].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[200].fCollisionDamageMultiplier = 0.750000f;
    m_OriginalHandlingData[200].uiModelFlags = 0x2004000;
    m_OriginalHandlingData[200].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[200].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[200].uiMonetary = 45000;
    m_OriginalHandlingData[200].ucHeadLight = 0;
    m_OriginalHandlingData[200].ucTailLight = 1;
    m_OriginalHandlingData[200].ucAnimGroup = 0;

    m_OriginalHandlingData[201].iVehicleID = 201;
    m_OriginalHandlingData[201].fMass = 3500.000000f;
    m_OriginalHandlingData[201].fTurnMass = 8458.299805f;
    m_OriginalHandlingData[201].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[201].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[201].uiPercentSubmerged = 75;
    m_OriginalHandlingData[201].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[201].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[201].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[201].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[201].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[201].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[201].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[201].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[201].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[201].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[201].bABS = false;
    m_OriginalHandlingData[201].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[201].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[201].fTractionBias = 0.500000f;
    m_OriginalHandlingData[201].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[201].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[201].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[201].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[201].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[201].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[201].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[201].fCollisionDamageMultiplier = 0.650000f;
    m_OriginalHandlingData[201].uiModelFlags = 0x2004000;
    m_OriginalHandlingData[201].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[201].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[201].uiMonetary = 50000;
    m_OriginalHandlingData[201].ucHeadLight = 0;
    m_OriginalHandlingData[201].ucTailLight = 1;
    m_OriginalHandlingData[201].ucAnimGroup = 0;

    m_OriginalHandlingData[202].iVehicleID = 202;
    m_OriginalHandlingData[202].fMass = 4500.000000f;
    m_OriginalHandlingData[202].fTurnMass = 26343.699219f;
    m_OriginalHandlingData[202].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[202].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[202].uiPercentSubmerged = 75;
    m_OriginalHandlingData[202].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[202].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[202].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[202].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[202].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[202].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[202].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[202].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[202].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[202].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[202].bABS = false;
    m_OriginalHandlingData[202].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[202].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[202].fTractionBias = 0.500000f;
    m_OriginalHandlingData[202].fSuspensionForceLevel = 2.000000f;
    m_OriginalHandlingData[202].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[202].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[202].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[202].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[202].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[202].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[202].fCollisionDamageMultiplier = 0.650000f;
    m_OriginalHandlingData[202].uiModelFlags = 0x2004000;
    m_OriginalHandlingData[202].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[202].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[202].uiMonetary = 52000;
    m_OriginalHandlingData[202].ucHeadLight = 0;
    m_OriginalHandlingData[202].ucTailLight = 1;
    m_OriginalHandlingData[202].ucAnimGroup = 0;

    m_OriginalHandlingData[203].iVehicleID = 203;
    m_OriginalHandlingData[203].fMass = 10000.000000f;
    m_OriginalHandlingData[203].fTurnMass = 150000.000000f;
    m_OriginalHandlingData[203].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[203].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[203].uiPercentSubmerged = 75;
    m_OriginalHandlingData[203].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[203].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[203].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[203].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[203].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[203].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[203].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[203].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[203].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[203].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[203].bABS = false;
    m_OriginalHandlingData[203].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[203].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[203].fTractionBias = 0.500000f;
    m_OriginalHandlingData[203].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[203].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[203].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[203].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[203].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[203].fSuspensionFrontRearBias = 0.850000f;
    m_OriginalHandlingData[203].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[203].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[203].uiModelFlags = 0x200C000;
    m_OriginalHandlingData[203].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[203].fSeatOffsetDistance = 0.400000f;
    m_OriginalHandlingData[203].uiMonetary = 99000;
    m_OriginalHandlingData[203].ucHeadLight = 0;
    m_OriginalHandlingData[203].ucTailLight = 1;
    m_OriginalHandlingData[203].ucAnimGroup = 14;

    m_OriginalHandlingData[204].iVehicleID = 204;
    m_OriginalHandlingData[204].fMass = 15000.000000f;
    m_OriginalHandlingData[204].fTurnMass = 200000.000000f;
    m_OriginalHandlingData[204].fDragCoeff = 0.100000f;
    m_OriginalHandlingData[204].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[204].uiPercentSubmerged = 30;
    m_OriginalHandlingData[204].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[204].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[204].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[204].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[204].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[204].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[204].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[204].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[204].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[204].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[204].bABS = false;
    m_OriginalHandlingData[204].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[204].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[204].fTractionBias = 0.500000f;
    m_OriginalHandlingData[204].fSuspensionForceLevel = 1.000000f;
    m_OriginalHandlingData[204].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[204].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[204].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[204].fSuspensionLowerLimit = -0.200000f;
    m_OriginalHandlingData[204].fSuspensionFrontRearBias = 0.900000f;
    m_OriginalHandlingData[204].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[204].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[204].uiModelFlags = 0x2004000;
    m_OriginalHandlingData[204].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[204].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[204].uiMonetary = 25000;
    m_OriginalHandlingData[204].ucHeadLight = 0;
    m_OriginalHandlingData[204].ucTailLight = 1;
    m_OriginalHandlingData[204].ucAnimGroup = 0;

    m_OriginalHandlingData[205].iVehicleID = 205;
    m_OriginalHandlingData[205].fMass = 20000.000000f;
    m_OriginalHandlingData[205].fTurnMass = 48333.300781f;
    m_OriginalHandlingData[205].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[205].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[205].uiPercentSubmerged = 75;
    m_OriginalHandlingData[205].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[205].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[205].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[205].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[205].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[205].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[205].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[205].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[205].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[205].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[205].bABS = false;
    m_OriginalHandlingData[205].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[205].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[205].fTractionBias = 0.500000f;
    m_OriginalHandlingData[205].fSuspensionForceLevel = 0.600000f;
    m_OriginalHandlingData[205].fSuspensionDamping = 0.050000f;
    m_OriginalHandlingData[205].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[205].fSuspensionUpperLimit = 0.500000f;
    m_OriginalHandlingData[205].fSuspensionLowerLimit = -0.100000f;
    m_OriginalHandlingData[205].fSuspensionFrontRearBias = 0.300000f;
    m_OriginalHandlingData[205].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[205].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[205].uiModelFlags = 0x22004000;
    m_OriginalHandlingData[205].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[205].fSeatOffsetDistance = 0.300000f;
    m_OriginalHandlingData[205].uiMonetary = 25000;
    m_OriginalHandlingData[205].ucHeadLight = 0;
    m_OriginalHandlingData[205].ucTailLight = 1;
    m_OriginalHandlingData[205].ucAnimGroup = 0;

    m_OriginalHandlingData[206].iVehicleID = 206;
    m_OriginalHandlingData[206].fMass = 10000.000000f;
    m_OriginalHandlingData[206].fTurnMass = 96666.703125f;
    m_OriginalHandlingData[206].fDragCoeff = 0.050000f;
    m_OriginalHandlingData[206].vecCenterOfMass = CVector(0.000000f, 0.000000f, -1.000000f);
    m_OriginalHandlingData[206].uiPercentSubmerged = 75;
    m_OriginalHandlingData[206].fTractionMultiplier = 0.650000f;
    m_OriginalHandlingData[206].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[206].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[206].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[206].Transmission.uiHandlingFlags = 4194304;
    m_OriginalHandlingData[206].Transmission.fEngineAcceleration = 6.400000f;
    m_OriginalHandlingData[206].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[206].Transmission.fMaxVelocity = 200.000000f;
    m_OriginalHandlingData[206].fBrakeDeceleration = 5.000000f;
    m_OriginalHandlingData[206].fBrakeBias = 0.450000f;
    m_OriginalHandlingData[206].bABS = false;
    m_OriginalHandlingData[206].fSteeringLock = 30.000000f;
    m_OriginalHandlingData[206].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[206].fTractionBias = 0.500000f;
    m_OriginalHandlingData[206].fSuspensionForceLevel = 1.500000f;
    m_OriginalHandlingData[206].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[206].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[206].fSuspensionUpperLimit = 0.200000f;
    m_OriginalHandlingData[206].fSuspensionLowerLimit = -0.150000f;
    m_OriginalHandlingData[206].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[206].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[206].fCollisionDamageMultiplier = 0.500000f;
    m_OriginalHandlingData[206].uiModelFlags = 0x2004000;
    m_OriginalHandlingData[206].uiHandlingFlags = 0x400000;
    m_OriginalHandlingData[206].fSeatOffsetDistance = 0.700000f;
    m_OriginalHandlingData[206].uiMonetary = 99000;
    m_OriginalHandlingData[206].ucHeadLight = 0;
    m_OriginalHandlingData[206].ucTailLight = 1;
    m_OriginalHandlingData[206].ucAnimGroup = 0;

    m_OriginalHandlingData[207].iVehicleID = 207;
    m_OriginalHandlingData[207].fMass = 100.000000f;
    m_OriginalHandlingData[207].fTurnMass = 50.000000f;
    m_OriginalHandlingData[207].fDragCoeff = 120.000000f;
    m_OriginalHandlingData[207].vecCenterOfMass = CVector(0.000000f, 0.000000f, 0.000000f);
    m_OriginalHandlingData[207].uiPercentSubmerged = 99;
    m_OriginalHandlingData[207].fTractionMultiplier = 0.200000f;
    m_OriginalHandlingData[207].Transmission.ucDriveType = 'F';
    m_OriginalHandlingData[207].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[207].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[207].Transmission.uiHandlingFlags = 32;
    m_OriginalHandlingData[207].Transmission.fEngineAcceleration = 0.400000f;
    m_OriginalHandlingData[207].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[207].Transmission.fMaxVelocity = 75.000000f;
    m_OriginalHandlingData[207].fBrakeDeceleration = 0.500000f;
    m_OriginalHandlingData[207].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[207].bABS = false;
    m_OriginalHandlingData[207].fSteeringLock = 45.000000f;
    m_OriginalHandlingData[207].fTractionLoss = 0.900000f;
    m_OriginalHandlingData[207].fTractionBias = 0.500000f;
    m_OriginalHandlingData[207].fSuspensionForceLevel = 0.600000f;
    m_OriginalHandlingData[207].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[207].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[207].fSuspensionUpperLimit = 0.250000f;
    m_OriginalHandlingData[207].fSuspensionLowerLimit = -0.000000f;
    m_OriginalHandlingData[207].fSuspensionFrontRearBias = 0.800000f;
    m_OriginalHandlingData[207].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[207].fCollisionDamageMultiplier = 0.080000f;
    m_OriginalHandlingData[207].uiModelFlags = 0x4000000;
    m_OriginalHandlingData[207].uiHandlingFlags = 0x20;
    m_OriginalHandlingData[207].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[207].uiMonetary = 300;
    m_OriginalHandlingData[207].ucHeadLight = 0;
    m_OriginalHandlingData[207].ucTailLight = 1;
    m_OriginalHandlingData[207].ucAnimGroup = 0;

    m_OriginalHandlingData[208].iVehicleID = 208;
    m_OriginalHandlingData[208].fMass = 100.000000f;
    m_OriginalHandlingData[208].fTurnMass = 24.100000f;
    m_OriginalHandlingData[208].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[208].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[208].uiPercentSubmerged = 70;
    m_OriginalHandlingData[208].fTractionMultiplier = 1.100000f;
    m_OriginalHandlingData[208].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[208].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[208].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[208].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[208].Transmission.fEngineAcceleration = 14.000000f;
    m_OriginalHandlingData[208].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[208].Transmission.fMaxVelocity = 75.000000f;
    m_OriginalHandlingData[208].fBrakeDeceleration = 5.500000f;
    m_OriginalHandlingData[208].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[208].bABS = false;
    m_OriginalHandlingData[208].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[208].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[208].fTractionBias = 0.500000f;
    m_OriginalHandlingData[208].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[208].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[208].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[208].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[208].fSuspensionLowerLimit = -0.080000f;
    m_OriginalHandlingData[208].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[208].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[208].fCollisionDamageMultiplier = 0.050000f;
    m_OriginalHandlingData[208].uiModelFlags = 0x2000000;
    m_OriginalHandlingData[208].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[208].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[208].uiMonetary = 800;
    m_OriginalHandlingData[208].ucHeadLight = 0;
    m_OriginalHandlingData[208].ucTailLight = 1;
    m_OriginalHandlingData[208].ucAnimGroup = 0;

    m_OriginalHandlingData[209].iVehicleID = 209;
    m_OriginalHandlingData[209].fMass = 100.000000f;
    m_OriginalHandlingData[209].fTurnMass = 24.100000f;
    m_OriginalHandlingData[209].fDragCoeff = 0.200000f;
    m_OriginalHandlingData[209].vecCenterOfMass = CVector(0.000000f, 0.000000f, -0.100000f);
    m_OriginalHandlingData[209].uiPercentSubmerged = 70;
    m_OriginalHandlingData[209].fTractionMultiplier = 1.100000f;
    m_OriginalHandlingData[209].Transmission.ucDriveType = '4';
    m_OriginalHandlingData[209].Transmission.ucEngineType = 'P';
    m_OriginalHandlingData[209].Transmission.ucNumberOfGears = 1;
    m_OriginalHandlingData[209].Transmission.uiHandlingFlags = 0;
    m_OriginalHandlingData[209].Transmission.fEngineAcceleration = 14.000000f;
    m_OriginalHandlingData[209].Transmission.fEngineInertia = 5.000000f;
    m_OriginalHandlingData[209].Transmission.fMaxVelocity = 75.000000f;
    m_OriginalHandlingData[209].fBrakeDeceleration = 5.500000f;
    m_OriginalHandlingData[209].fBrakeBias = 0.500000f;
    m_OriginalHandlingData[209].bABS = false;
    m_OriginalHandlingData[209].fSteeringLock = 25.000000f;
    m_OriginalHandlingData[209].fTractionLoss = 0.750000f;
    m_OriginalHandlingData[209].fTractionBias = 0.500000f;
    m_OriginalHandlingData[209].fSuspensionForceLevel = 1.600000f;
    m_OriginalHandlingData[209].fSuspensionDamping = 0.100000f;
    m_OriginalHandlingData[209].fSuspensionHighSpdDamping = 0.000000f;
    m_OriginalHandlingData[209].fSuspensionUpperLimit = 0.280000f;
    m_OriginalHandlingData[209].fSuspensionLowerLimit = -0.080000f;
    m_OriginalHandlingData[209].fSuspensionFrontRearBias = 0.500000f;
    m_OriginalHandlingData[209].fSuspensionAntiDiveMultiplier = 0.000000f;
    m_OriginalHandlingData[209].fCollisionDamageMultiplier = 0.100000f;
    m_OriginalHandlingData[209].uiModelFlags = 0x2000000;
    m_OriginalHandlingData[209].uiHandlingFlags = 0x0;
    m_OriginalHandlingData[209].fSeatOffsetDistance = 0.200000f;
    m_OriginalHandlingData[209].uiMonetary = 500;
    m_OriginalHandlingData[209].ucHeadLight = 0;
    m_OriginalHandlingData[209].ucTailLight = 1;
    m_OriginalHandlingData[209].ucAnimGroup = 0;

    m_OriginalHandlingData[210] = m_OriginalHandlingData[69];            // HT_HOTRINA = HT_HOTRING
    m_OriginalHandlingData[210].iVehicleID = 210;

    m_OriginalHandlingData[211] = m_OriginalHandlingData[69];            // HT_HOTRINB = HT_HOTRING
    m_OriginalHandlingData[211].iVehicleID = 211;

    m_OriginalHandlingData[212] = m_OriginalHandlingData[103];            // HT_SADLSHIT = HT_SADLER
    m_OriginalHandlingData[212].iVehicleID = 212;

    m_OriginalHandlingData[213] = m_OriginalHandlingData[52];            // HT_GLENSHIT = HT_GLENDALE
    m_OriginalHandlingData[213].iVehicleID = 213;

    m_OriginalHandlingData[214] = m_OriginalHandlingData[163];            // HT_FAGGIO = HT_PIZZABOY
    m_OriginalHandlingData[214].iVehicleID = 214;

    m_OriginalHandlingData[215] = m_OriginalHandlingData[7];            // HT_FIRELA = HT_FIRETRUK
    m_OriginalHandlingData[215].iVehicleID = 215;

    m_OriginalHandlingData[216] = m_OriginalHandlingData[65];            // HT_RNCHLURE = HT_RANCHER
    m_OriginalHandlingData[216].iVehicleID = 216;

    m_OriginalHandlingData[217] = m_OriginalHandlingData[126];            // HT_FREIBOX = HT_FREIFLAT
    m_OriginalHandlingData[217].iVehicleID = 217;

    m_OriginalHandlingData[218] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[218].iVehicleID = 218;

    m_OriginalHandlingData[219] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[219].iVehicleID = 219;

    m_OriginalHandlingData[220] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[220].iVehicleID = 220;

    m_OriginalHandlingData[221] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[221].iVehicleID = 221;

    m_OriginalHandlingData[222] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[222].iVehicleID = 222;

    m_OriginalHandlingData[223] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[223].iVehicleID = 223;

    m_OriginalHandlingData[224] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[224].iVehicleID = 224;

    m_OriginalHandlingData[225] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[225].iVehicleID = 225;

    m_OriginalHandlingData[226] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[226].iVehicleID = 226;

    m_OriginalHandlingData[227] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[227].iVehicleID = 227;

    m_OriginalHandlingData[228] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[228].iVehicleID = 228;

    m_OriginalHandlingData[229] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[229].iVehicleID = 229;

    m_OriginalHandlingData[230] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[230].iVehicleID = 230;

    m_OriginalHandlingData[231] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[231].iVehicleID = 231;

    m_OriginalHandlingData[232] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[232].iVehicleID = 232;

    m_OriginalHandlingData[233] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[233].iVehicleID = 233;

    m_OriginalHandlingData[234] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[234].iVehicleID = 234;

    m_OriginalHandlingData[235] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[235].iVehicleID = 235;

    m_OriginalHandlingData[236] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[236].iVehicleID = 236;

    m_OriginalHandlingData[237] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[237].iVehicleID = 237;

    m_OriginalHandlingData[238] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[238].iVehicleID = 238;

    m_OriginalHandlingData[239] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[239].iVehicleID = 239;

    m_OriginalHandlingData[240] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[240].iVehicleID = 240;

    m_OriginalHandlingData[241] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[241].iVehicleID = 241;

    m_OriginalHandlingData[242] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[242].iVehicleID = 242;

    m_OriginalHandlingData[243] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[243].iVehicleID = 243;

    m_OriginalHandlingData[244] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[244].iVehicleID = 244;

    m_OriginalHandlingData[245] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[245].iVehicleID = 245;

    m_OriginalHandlingData[246] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[246].iVehicleID = 246;

    m_OriginalHandlingData[247] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[247].iVehicleID = 247;

    m_OriginalHandlingData[248] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[248].iVehicleID = 248;

    m_OriginalHandlingData[249] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[249].iVehicleID = 249;

    m_OriginalHandlingData[250] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[250].iVehicleID = 250;

    m_OriginalHandlingData[251] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[251].iVehicleID = 251;

    m_OriginalHandlingData[252] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[252].iVehicleID = 252;

    m_OriginalHandlingData[253] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[253].iVehicleID = 253;

    m_OriginalHandlingData[254] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[254].iVehicleID = 254;

    m_OriginalHandlingData[255] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[255].iVehicleID = 255;

    m_OriginalHandlingData[256] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[256].iVehicleID = 256;

    m_OriginalHandlingData[257] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[257].iVehicleID = 257;

    m_OriginalHandlingData[258] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[258].iVehicleID = 258;

    m_OriginalHandlingData[259] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[259].iVehicleID = 259;

    m_OriginalHandlingData[260] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[260].iVehicleID = 260;

    m_OriginalHandlingData[261] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[261].iVehicleID = 261;

    m_OriginalHandlingData[262] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[262].iVehicleID = 262;

    m_OriginalHandlingData[263] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[263].iVehicleID = 263;

    m_OriginalHandlingData[264] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[264].iVehicleID = 264;

    m_OriginalHandlingData[265] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[265].iVehicleID = 265;

    m_OriginalHandlingData[266] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[266].iVehicleID = 266;

    m_OriginalHandlingData[267] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[267].iVehicleID = 267;

    m_OriginalHandlingData[268] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[268].iVehicleID = 268;

    m_OriginalHandlingData[269] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[269].iVehicleID = 269;

    m_OriginalHandlingData[270] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[270].iVehicleID = 270;

    m_OriginalHandlingData[271] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[271].iVehicleID = 271;

    m_OriginalHandlingData[272] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[272].iVehicleID = 272;

    m_OriginalHandlingData[273] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[273].iVehicleID = 273;

    m_OriginalHandlingData[274] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[274].iVehicleID = 274;

    m_OriginalHandlingData[275] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[275].iVehicleID = 275;

    m_OriginalHandlingData[276] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[276].iVehicleID = 276;

    m_OriginalHandlingData[277] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[277].iVehicleID = 277;

    m_OriginalHandlingData[278] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[278].iVehicleID = 278;

    m_OriginalHandlingData[279] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[279].iVehicleID = 279;

    m_OriginalHandlingData[280] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[280].iVehicleID = 280;

    m_OriginalHandlingData[281] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[281].iVehicleID = 281;

    m_OriginalHandlingData[282] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[282].iVehicleID = 282;

    m_OriginalHandlingData[283] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[283].iVehicleID = 283;

    m_OriginalHandlingData[284] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[284].iVehicleID = 284;

    m_OriginalHandlingData[285] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[285].iVehicleID = 285;

    m_OriginalHandlingData[286] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[286].iVehicleID = 286;

    m_OriginalHandlingData[287] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[287].iVehicleID = 287;

    m_OriginalHandlingData[288] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[288].iVehicleID = 288;

    m_OriginalHandlingData[289] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[289].iVehicleID = 289;

    m_OriginalHandlingData[290] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[290].iVehicleID = 290;

    m_OriginalHandlingData[291] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[291].iVehicleID = 291;

    m_OriginalHandlingData[292] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[292].iVehicleID = 292;

    m_OriginalHandlingData[293] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[293].iVehicleID = 293;

    m_OriginalHandlingData[294] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[294].iVehicleID = 294;

    m_OriginalHandlingData[295] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[295].iVehicleID = 295;

    m_OriginalHandlingData[296] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[296].iVehicleID = 296;

    m_OriginalHandlingData[297] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[297].iVehicleID = 297;

    m_OriginalHandlingData[298] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[298].iVehicleID = 298;

    m_OriginalHandlingData[299] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[299].iVehicleID = 299;

    m_OriginalHandlingData[300] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[300].iVehicleID = 300;

    m_OriginalHandlingData[301] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[301].iVehicleID = 301;

    m_OriginalHandlingData[302] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[302].iVehicleID = 302;

    m_OriginalHandlingData[303] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[303].iVehicleID = 303;

    m_OriginalHandlingData[304] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[304].iVehicleID = 304;

    m_OriginalHandlingData[305] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[305].iVehicleID = 305;

    m_OriginalHandlingData[306] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[306].iVehicleID = 306;

    m_OriginalHandlingData[307] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[307].iVehicleID = 307;

    m_OriginalHandlingData[308] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[308].iVehicleID = 308;

    m_OriginalHandlingData[309] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[309].iVehicleID = 309;

    m_OriginalHandlingData[310] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[310].iVehicleID = 310;

    m_OriginalHandlingData[311] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[311].iVehicleID = 311;

    m_OriginalHandlingData[312] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[312].iVehicleID = 312;

    m_OriginalHandlingData[313] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[313].iVehicleID = 313;

    m_OriginalHandlingData[314] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[314].iVehicleID = 314;

    m_OriginalHandlingData[315] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[315].iVehicleID = 315;

    m_OriginalHandlingData[316] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[316].iVehicleID = 316;

    m_OriginalHandlingData[317] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[317].iVehicleID = 317;

    m_OriginalHandlingData[318] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[318].iVehicleID = 318;

    m_OriginalHandlingData[319] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[319].iVehicleID = 319;

    m_OriginalHandlingData[320] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[320].iVehicleID = 320;

    m_OriginalHandlingData[321] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[321].iVehicleID = 321;

    m_OriginalHandlingData[322] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[322].iVehicleID = 322;

    m_OriginalHandlingData[323] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[323].iVehicleID = 323;

    m_OriginalHandlingData[324] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[324].iVehicleID = 324;

    m_OriginalHandlingData[325] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[325].iVehicleID = 325;

    m_OriginalHandlingData[326] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[326].iVehicleID = 326;

    m_OriginalHandlingData[327] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[327].iVehicleID = 327;

    m_OriginalHandlingData[328] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[328].iVehicleID = 328;

    m_OriginalHandlingData[329] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[329].iVehicleID = 329;

    m_OriginalHandlingData[330] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[330].iVehicleID = 330;

    m_OriginalHandlingData[331] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[331].iVehicleID = 331;

    m_OriginalHandlingData[332] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[332].iVehicleID = 332;

    m_OriginalHandlingData[333] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[333].iVehicleID = 333;

    m_OriginalHandlingData[334] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[334].iVehicleID = 334;

    m_OriginalHandlingData[335] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[335].iVehicleID = 335;

    m_OriginalHandlingData[336] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[336].iVehicleID = 336;

    m_OriginalHandlingData[337] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[337].iVehicleID = 337;

    m_OriginalHandlingData[338] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[338].iVehicleID = 338;

    m_OriginalHandlingData[339] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[339].iVehicleID = 339;

    m_OriginalHandlingData[340] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[340].iVehicleID = 340;

    m_OriginalHandlingData[341] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[341].iVehicleID = 341;

    m_OriginalHandlingData[342] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[342].iVehicleID = 342;

    m_OriginalHandlingData[343] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[343].iVehicleID = 343;

    m_OriginalHandlingData[344] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[344].iVehicleID = 344;

    m_OriginalHandlingData[345] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[345].iVehicleID = 345;

    m_OriginalHandlingData[346] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[346].iVehicleID = 346;

    m_OriginalHandlingData[347] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[347].iVehicleID = 347;

    m_OriginalHandlingData[348] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[348].iVehicleID = 348;

    m_OriginalHandlingData[349] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[349].iVehicleID = 349;

    m_OriginalHandlingData[350] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[350].iVehicleID = 350;

    m_OriginalHandlingData[351] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[351].iVehicleID = 351;

    m_OriginalHandlingData[352] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[352].iVehicleID = 352;

    m_OriginalHandlingData[353] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[353].iVehicleID = 353;

    m_OriginalHandlingData[354] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[354].iVehicleID = 354;

    m_OriginalHandlingData[355] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[355].iVehicleID = 355;

    m_OriginalHandlingData[356] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[356].iVehicleID = 356;

    m_OriginalHandlingData[357] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[357].iVehicleID = 357;

    m_OriginalHandlingData[358] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[358].iVehicleID = 358;

    m_OriginalHandlingData[359] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[359].iVehicleID = 359;

    m_OriginalHandlingData[360] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[360].iVehicleID = 360;

    m_OriginalHandlingData[361] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[361].iVehicleID = 361;

    m_OriginalHandlingData[362] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[362].iVehicleID = 362;

    m_OriginalHandlingData[363] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[363].iVehicleID = 363;

    m_OriginalHandlingData[364] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[364].iVehicleID = 364;

    m_OriginalHandlingData[365] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[365].iVehicleID = 365;

    m_OriginalHandlingData[366] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[366].iVehicleID = 366;

    m_OriginalHandlingData[367] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[367].iVehicleID = 367;

    m_OriginalHandlingData[368] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[368].iVehicleID = 368;

    m_OriginalHandlingData[369] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[369].iVehicleID = 369;

    m_OriginalHandlingData[370] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[370].iVehicleID = 370;

    m_OriginalHandlingData[371] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[371].iVehicleID = 371;

    m_OriginalHandlingData[372] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[372].iVehicleID = 372;

    m_OriginalHandlingData[373] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[373].iVehicleID = 373;

    m_OriginalHandlingData[374] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[374].iVehicleID = 374;

    m_OriginalHandlingData[375] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[375].iVehicleID = 375;

    m_OriginalHandlingData[376] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[376].iVehicleID = 376;

    m_OriginalHandlingData[377] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[377].iVehicleID = 377;

    m_OriginalHandlingData[378] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[378].iVehicleID = 378;

    m_OriginalHandlingData[379] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[379].iVehicleID = 379;

    m_OriginalHandlingData[380] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[380].iVehicleID = 380;

    m_OriginalHandlingData[381] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[381].iVehicleID = 381;

    m_OriginalHandlingData[382] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[382].iVehicleID = 382;

    m_OriginalHandlingData[383] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[383].iVehicleID = 383;

    m_OriginalHandlingData[384] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[384].iVehicleID = 384;

    m_OriginalHandlingData[385] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[385].iVehicleID = 385;

    m_OriginalHandlingData[386] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[386].iVehicleID = 386;

    m_OriginalHandlingData[387] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[387].iVehicleID = 387;

    m_OriginalHandlingData[388] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[388].iVehicleID = 388;

    m_OriginalHandlingData[389] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[389].iVehicleID = 389;

    m_OriginalHandlingData[390] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[390].iVehicleID = 390;

    m_OriginalHandlingData[391] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[391].iVehicleID = 391;

    m_OriginalHandlingData[392] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[392].iVehicleID = 392;

    m_OriginalHandlingData[393] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[393].iVehicleID = 393;

    m_OriginalHandlingData[394] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[394].iVehicleID = 394;

    m_OriginalHandlingData[395] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[395].iVehicleID = 395;

    m_OriginalHandlingData[396] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[396].iVehicleID = 396;

    m_OriginalHandlingData[397] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[397].iVehicleID = 397;

    m_OriginalHandlingData[398] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[398].iVehicleID = 398;

    m_OriginalHandlingData[399] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[399].iVehicleID = 399;

    m_OriginalHandlingData[400] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[400].iVehicleID = 400;

    m_OriginalHandlingData[401] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[401].iVehicleID = 401;

    m_OriginalHandlingData[402] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[402].iVehicleID = 402;

    m_OriginalHandlingData[403] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[403].iVehicleID = 403;

    m_OriginalHandlingData[404] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[404].iVehicleID = 404;

    m_OriginalHandlingData[405] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[405].iVehicleID = 405;

    m_OriginalHandlingData[406] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[406].iVehicleID = 406;

    m_OriginalHandlingData[407] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[407].iVehicleID = 407;

    m_OriginalHandlingData[408] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[408].iVehicleID = 408;

    m_OriginalHandlingData[409] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[409].iVehicleID = 409;

    m_OriginalHandlingData[410] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[410].iVehicleID = 410;

    m_OriginalHandlingData[411] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[411].iVehicleID = 411;

    m_OriginalHandlingData[412] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[412].iVehicleID = 412;

    m_OriginalHandlingData[413] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[413].iVehicleID = 413;

    m_OriginalHandlingData[414] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[414].iVehicleID = 414;

    m_OriginalHandlingData[415] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[415].iVehicleID = 415;

    m_OriginalHandlingData[416] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[416].iVehicleID = 416;

    m_OriginalHandlingData[417] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[417].iVehicleID = 417;

    m_OriginalHandlingData[418] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[418].iVehicleID = 418;

    m_OriginalHandlingData[419] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[419].iVehicleID = 419;

    m_OriginalHandlingData[420] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[420].iVehicleID = 420;

    m_OriginalHandlingData[421] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[421].iVehicleID = 421;

    m_OriginalHandlingData[422] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[422].iVehicleID = 422;

    m_OriginalHandlingData[423] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[423].iVehicleID = 423;

    m_OriginalHandlingData[424] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[424].iVehicleID = 424;

    m_OriginalHandlingData[425] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[425].iVehicleID = 425;

    m_OriginalHandlingData[426] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[426].iVehicleID = 426;

    m_OriginalHandlingData[427] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[427].iVehicleID = 427;

    m_OriginalHandlingData[428] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[428].iVehicleID = 428;

    m_OriginalHandlingData[429] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[429].iVehicleID = 429;

    m_OriginalHandlingData[430] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[430].iVehicleID = 430;

    m_OriginalHandlingData[431] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[431].iVehicleID = 431;

    m_OriginalHandlingData[432] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[432].iVehicleID = 432;

    m_OriginalHandlingData[433] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[433].iVehicleID = 433;

    m_OriginalHandlingData[434] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[434].iVehicleID = 434;

    m_OriginalHandlingData[435] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[435].iVehicleID = 435;

    m_OriginalHandlingData[436] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[436].iVehicleID = 436;

    m_OriginalHandlingData[437] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[437].iVehicleID = 437;

    m_OriginalHandlingData[438] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[438].iVehicleID = 438;

    m_OriginalHandlingData[439] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[439].iVehicleID = 439;

    m_OriginalHandlingData[440] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[440].iVehicleID = 440;

    m_OriginalHandlingData[441] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[441].iVehicleID = 441;

    m_OriginalHandlingData[442] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[442].iVehicleID = 442;

    m_OriginalHandlingData[443] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[443].iVehicleID = 443;

    m_OriginalHandlingData[444] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[444].iVehicleID = 444;

    m_OriginalHandlingData[445] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[445].iVehicleID = 445;

    m_OriginalHandlingData[446] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[446].iVehicleID = 446;

    m_OriginalHandlingData[447] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[447].iVehicleID = 447;

    m_OriginalHandlingData[448] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[448].iVehicleID = 448;

    m_OriginalHandlingData[449] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[449].iVehicleID = 449;

    m_OriginalHandlingData[450] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[450].iVehicleID = 450;

    m_OriginalHandlingData[451] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[451].iVehicleID = 451;

    m_OriginalHandlingData[452] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[452].iVehicleID = 452;

    m_OriginalHandlingData[453] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[453].iVehicleID = 453;

    m_OriginalHandlingData[454] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[454].iVehicleID = 454;

    m_OriginalHandlingData[455] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[455].iVehicleID = 455;

    m_OriginalHandlingData[456] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[456].iVehicleID = 456;

    m_OriginalHandlingData[457] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[457].iVehicleID = 457;

    m_OriginalHandlingData[458] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[458].iVehicleID = 458;

    m_OriginalHandlingData[459] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[459].iVehicleID = 459;

    m_OriginalHandlingData[460] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[460].iVehicleID = 460;

    m_OriginalHandlingData[461] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[461].iVehicleID = 461;

    m_OriginalHandlingData[462] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[462].iVehicleID = 462;

    m_OriginalHandlingData[463] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[463].iVehicleID = 463;

    m_OriginalHandlingData[464] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[464].iVehicleID = 464;

    m_OriginalHandlingData[465] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[465].iVehicleID = 465;

    m_OriginalHandlingData[466] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[466].iVehicleID = 466;

    m_OriginalHandlingData[467] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[467].iVehicleID = 467;

    m_OriginalHandlingData[468] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[468].iVehicleID = 468;

    m_OriginalHandlingData[469] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[469].iVehicleID = 469;

    m_OriginalHandlingData[470] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[470].iVehicleID = 470;

    m_OriginalHandlingData[471] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[471].iVehicleID = 471;

    m_OriginalHandlingData[472] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[472].iVehicleID = 472;

    m_OriginalHandlingData[473] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[473].iVehicleID = 473;

    m_OriginalHandlingData[474] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[474].iVehicleID = 474;

    m_OriginalHandlingData[475] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[475].iVehicleID = 475;

    m_OriginalHandlingData[476] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[476].iVehicleID = 476;

    m_OriginalHandlingData[477] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[477].iVehicleID = 477;

    m_OriginalHandlingData[478] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[478].iVehicleID = 478;

    m_OriginalHandlingData[479] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[479].iVehicleID = 479;

    m_OriginalHandlingData[480] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[480].iVehicleID = 480;

    m_OriginalHandlingData[481] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[481].iVehicleID = 481;

    m_OriginalHandlingData[482] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[482].iVehicleID = 482;

    m_OriginalHandlingData[483] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[483].iVehicleID = 483;

    m_OriginalHandlingData[484] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[484].iVehicleID = 484;

    m_OriginalHandlingData[485] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[485].iVehicleID = 485;

    m_OriginalHandlingData[486] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[486].iVehicleID = 486;

    m_OriginalHandlingData[487] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[487].iVehicleID = 487;

    m_OriginalHandlingData[488] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[488].iVehicleID = 488;

    m_OriginalHandlingData[489] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[489].iVehicleID = 489;

    m_OriginalHandlingData[490] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[490].iVehicleID = 490;

    m_OriginalHandlingData[491] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[491].iVehicleID = 491;

    m_OriginalHandlingData[492] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[492].iVehicleID = 492;

    m_OriginalHandlingData[493] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[493].iVehicleID = 493;

    m_OriginalHandlingData[494] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[494].iVehicleID = 494;

    m_OriginalHandlingData[495] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[495].iVehicleID = 495;

    m_OriginalHandlingData[496] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[496].iVehicleID = 496;

    m_OriginalHandlingData[497] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[497].iVehicleID = 497;

    m_OriginalHandlingData[498] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[498].iVehicleID = 498;

    m_OriginalHandlingData[499] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[499].iVehicleID = 499;

    m_OriginalHandlingData[500] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[500].iVehicleID = 500;

    m_OriginalHandlingData[501] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[501].iVehicleID = 501;

    m_OriginalHandlingData[502] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[502].iVehicleID = 502;

    m_OriginalHandlingData[503] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[503].iVehicleID = 503;

    m_OriginalHandlingData[504] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[504].iVehicleID = 504;

    m_OriginalHandlingData[505] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[505].iVehicleID = 505;

    m_OriginalHandlingData[506] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[506].iVehicleID = 506;

    m_OriginalHandlingData[507] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[507].iVehicleID = 507;

    m_OriginalHandlingData[508] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[508].iVehicleID = 508;

    m_OriginalHandlingData[509] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[509].iVehicleID = 509;

    m_OriginalHandlingData[510] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[510].iVehicleID = 510;

    m_OriginalHandlingData[511] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[511].iVehicleID = 511;

    m_OriginalHandlingData[512] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[512].iVehicleID = 512;

    m_OriginalHandlingData[513] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[513].iVehicleID = 513;

    m_OriginalHandlingData[514] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[514].iVehicleID = 514;

    m_OriginalHandlingData[515] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[515].iVehicleID = 515;

    m_OriginalHandlingData[516] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[516].iVehicleID = 516;

    m_OriginalHandlingData[517] = m_OriginalHandlingData[69];
    m_OriginalHandlingData[517].iVehicleID = 517; 



    
}
