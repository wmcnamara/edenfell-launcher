workspace "EdenfellLauncher"
    configurations { "Debug", "Release" }

project "EdenfellLauncher"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    cppdialect "C++17"

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
       
        "premake5.lua",
    }

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

    includedirs {}
    libdirs {}
    links {}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"