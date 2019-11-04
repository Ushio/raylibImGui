workspace "raylibMinimumVS"
    location "build"
    configurations { "Debug", "Release" }
    startproject "raylibMinimumVS"

architecture "x86_64"

project "raylib"
    kind "SharedLib"
    language "C++"
    targetdir "bin/"
    systemversion "latest"
    flags { "MultiProcessorCompile", "NoPCH" }

    -- src
    files { "libs/raylib/src/*.h", "libs/raylib/src/*.c" }
    includedirs { "libs/raylib/src/" }
    includedirs { "libs/raylib/src/external/glfw/include" }

    defines {"GRAPHICS_API_OPENGL_33", "PLATFORM_DESKTOP", "BUILD_LIBTYPE_SHARED"}
    links { "winmm" }
    
    filter {"Debug"}
        buildoptions "/MDd"
        targetname ("raylib_d")
        optimize "Off"
    filter {"Release"}
        buildoptions "/MD"
        targetname ("raylib")
        optimize "Full"
    filter{}

    -- strcopy etc
    disablewarnings { "4996"} 

project "raylibMinimumVS"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/"
    systemversion "latest"
    flags { "MultiProcessorCompile", "NoPCH" }

    -- Src
    files { "src/**.h", "src/**.hpp", "src/**.cpp" }
    defines { "GRAPHICS_API_OPENGL_33", "PLATFORM_DESKTOP"}

    -- raylib
    dependson { "raylib" }

    filter {"Debug"}
        links { "raylib_d" }
    filter {"Release"}
        links { "raylib" }
    filter{}

    includedirs { "libs/raylib/src/" }
    libdirs { "bin" }

    symbols "On"

    filter {"Debug"}
        buildoptions "/MDd"
        targetname ("raylibMinimumVS_Debug")
        optimize "Off"
    filter {"Release"}
        buildoptions "/MD"
        targetname ("raylibMinimumVS")
        optimize "Full"
    filter{}
