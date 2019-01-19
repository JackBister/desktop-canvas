#!lua
require("build/conanbuildinfo")

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

	project "Main"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"

		files { "src/**.h", "src/**.c", "src/**.cpp" }
		buildlog "build/build.log"
		debugargs { "--watch example/dist/app.js" }
		debugdir "."
		includedirs { "src" }
		objdir "build"
		targetdir "build"

		-- Appveyor VS does not like to compile websocketpp without this flag(?)
		defines { "_HAS_AUTO_PTR_ETC" }

		sysincludedirs { conan_includedirs_asio, conan_includedirs_sdl2, conan_includedirs_sdl2_ttf, conan_includedirs_websocketpp }

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
			libdirs { conan_libdirs_sdl2, conan_libdirs_sdl2_ttf }
			optimize "Off"
			symbols "On"

		filter "Release"
			libdirs { conan_libdirs_sdl2, conan_libdirs_sdl2_ttf }
			optimize "Full"

		filter { "system:windows", "Debug" }
			links { conan_libs_sdl2, conan_libs_sdl2_ttf }

		filter { "system:windows", "Release" }
			links { conan_libs_sdl2, conan_libs_sdl2_ttf }

		filter { "system:linux", "Debug" }
			links { conan_libs_sdl2, conan_libs_sdl2_ttf, "stdc++fs" }

		filter { "system:linux", "Release" }
			links { conan_libs_sdl2, conan_libs_sdl2_ttf, "stdc++fs" }

		filter { "files:src/**_android.*" }
			flags {"ExcludeFromBuild"}
