#!lua

function isModuleAvailable(name)
	if package.loaded[name] then
		return true
	else
		for _, searcher in ipairs(package.searchers or package.loaders) do
			local loader = searcher(name)
			if type(loader) == 'function' then
				package.preload[name] = loader
				return true
			end
		end
		return false
	end
end

function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

if isModuleAvailable("premake-compilationunit/compilationunit") then
	require("premake-compilationunit/compilationunit")
end

solution "desktop-canvas"

	configurations { "Debug", "Release" }
	platforms { "ARM", "x86", "x64" }

	filter {"system:windows", "action:vs*"}
		local sysversion = os.winSdkVersion()
		if sysversion ~= nil then
			systemversion(sysversion .. ".0")
		end

	local platformdirectory = "installed/%{cfg.platform}-%{cfg.system}/"
	local staticPlatformDirectory = "installed/%{cfg.platform}-%{cfg.system}-static/"

	project "Main"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"

		exceptionhandling "Off"

		files { "src/**.h", "src/**.c", "src/**.cpp" }
		buildlog "build/build.log"
		debugargs { "--watch example/dist/app.js" }
		debugdir "."
		includedirs { "src" }
		objdir "build"
		targetdir "build"

		sysincludedirs { "include",  staticPlatformDirectory .. "include", platformdirectory .. "include" }

		local androidHome = os.getenv("ANDROID_HOME")
		if androidHome ~= nil then
			sysincludedirs { androidHome .. "/ndk-bundle/sysroot/usr/include" }
		end

		flags { "MultiProcessorCompile" }

		if isModuleAvailable("premake-compilationunit/compilationunit") then
			compilationunitenabled (true)
		end

		filter { "system:linux" }
			linkoptions { "-pthread" }

		filter "Debug"
			libdirs { staticPlatformDirectory .. "debug/lib", platformdirectory .. "debug/lib" }
			optimize "Off"
			symbols "On"

		filter "Release"
			libdirs { staticPlatformDirectory .. "lib", platformdirectory .. "lib" }
			optimize "Full"

		filter { "system:windows", "Debug" }
			links { "SDL2d", "SDL2_ttf", "uWS", "libuv", "ssleay32", "zlibd" }

		filter { "system:windows", "Release" }
			links { "SDL2", "SDL2_ttf", "uWS", "libuv", "ssleay32", "zlib" }

		filter { "system:linux", "Debug" }
			links { "SDL2d", "SDL2_ttf", "uWS", "libuv", "ssl", "z", "crypto", "dl", "freetyped", "bz2d", "png", "stdc++fs" }

		filter { "system:linux", "Release" }
			links { "SDL2", "SDL2_ttf", "uWS", "libuv", "ssl", "z", "crypto", "dl", "freetype", "bz2", "png", "stdc++fs" }

		filter { "files:src/**_android.*" }
			flags {"ExcludeFromBuild"}
