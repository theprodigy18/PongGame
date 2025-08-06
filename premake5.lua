-- ========================================
-- WORKSPACE("PongGame")
-- ========================================
workspace "PongGame"
	architecture "x64"
	configurations { "Debug", "Release" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "action:vs*"
		buildoptions { "/Zc:__cplusplus" }

	filter {"action:vs*", "configurations:Release"}
		staticruntime "on"

	filter "system:windows"
		defines {"PLATFORM_WINDOWS"}
	
	filter {}


outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ========================================
-- PROJECT("SharedLib")
-- ========================================
project "SharedLib"
	location "SharedLib"
	kind "SharedLib"
	language "C"
	cdialect "C11"

	targetdir ("bin/" .. outdir .. "/data")
	objdir ("bin-int/" .. outdir .. "/data")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.c"

	files 
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.c"
	}
	includedirs
	{
		"%{prj.name}/include",
		"%{prj.name}/vendor"
	}
	links
	{
		"user32",
		"gdi32",
		"opengl32"
	}

	defines {"DLL_EXPORTS"}
	
	postbuildcommands
	{
		"{COPY} %{cfg.targetdir}/data/SharedLib.dll %{cfg.targetdir}/"
	}
		

-- ========================================
-- PROJECT("PongGame")
-- ========================================
project "PongGame"
	location "PongGame"
	kind "ConsoleApp"
	language "C"
	cdialect "C11"

	targetdir ("bin/" .. outdir)
	objdir ("bin-int/" .. outdir)

	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.c"
	}
	includedirs
	{
		"SharedLib"
	}
	links
	{
		"SharedLib"
	}