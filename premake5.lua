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
    IncludeDirs["GoogleTest"] = "vendor/gtest/googletest/include"

    project "GoogleTest"
    location "vendor/gtest/googletest"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files 
    { 
        "%{prj.location}/src/gtest-all.cc", 
        "%{prj.location}/src/*.h"
    }

    includedirs 
    { 
        "%{prj.location}",
        "%{prj.location}/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter "configurations:Release"
        runtime "Release"
        optimize "on"

project "Boomerang"
    location "Boomerang"
    kind "ConsoleApp"
    staticruntime "on"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c",
        "%{prj.name}/tests/**.h",
        "%{prj.name}/tests/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        IncludeDirs["GoogleTest"]
    }

    links
    {
        "GoogleTest"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "BOOM_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "BOOM_INCLUDE_TESTS", "Boomerang_DEBUG" }
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
