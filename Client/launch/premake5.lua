project "Client Launcher"
	language "C++"
	kind "WindowedApp"
	targetname "Multi Theft Auto"
	targetdir(buildpath("."))
	debugdir(buildpath("."))

	includedirs {
		"../../Shared/sdk",
		"../sdk",
	}

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

	entrypoint "WinMainCRTStartup"

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["Resources/*"] = {"*.rc", "**.ico", "**.xml", "**.manifest"},
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"*.h",
		"*.cpp"
	}

	filter "system:windows"
		staticruntime "On"
		files {
			"NEU/gameicon_NEU.ico",
			"NEU/Multi Theft Auto.gdf.xml",
			"launch.rc",
			"Multi Theft Auto.rc",
			"Multi Theft Auto.manifest",
			"resource/mtaicon.ico"
		}
		if _ACTION and _ACTION >= "vs2010" then
            require "vstudio"
            premake.override(premake.vstudio.vc2010.elements, "link", function(base, prj)
                local calls = base(prj)
                table.insert(calls, function() premake.vstudio.vc2010.element("UACExecutionLevel", nil, "RequireAdministrator") end)
                return calls
            end)
        end

	filter "architecture:not x86"
		flags { "ExcludeFromBuild" }

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
