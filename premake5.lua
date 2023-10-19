workspace "Boomerang"
    architecture "x64"
    startproject "Boomerang"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}

group "Dependencies"

group ""

project "Boomerang"
    location "Boomerang"
    kind "ConsoleApp"
    staticruntime "on"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- pchheader "hvepch.h"
    -- pchsource "helios/src/hvepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c",
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "BOOM_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "Boomerang_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "Boomerang_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "Boomerang_DIST"
        runtime "Release"
        optimize "on"