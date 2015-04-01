
zlib_solution = function()
	if  _ACTION=="vs2008" then
		solution "zlib_2008"
	elseif _ACTION=="vs2010" then
		solution "zlib_2010"
	else
		solution "zlib"
	end
	
	if _ACTION=="xcode3" then
		configurations { "debug",
						 "release"
						}
	else
		location ( getZlibLocation("../zlib/projects/") )
		configurations { 
						 "Win_Debug",
						 "Win_Release",
						 "Win_Debug_DEF",
						 "Win_Debug_DLL",
						 "Win_Release_DEF",
						 "Win_Release_DLL"
						 }
	end
	
	-- Platforms
	if _ACTION=="xcode3" then
        if _OPTIONS["iphone"] then
            platforms { "iPhone" }
        else
            platforms { "x32" }
        end
	end
	
	zlib_project()

end

zlib_project = function()

	if  _ACTION=="vs2008" then
		project "zlib_2008"
	elseif _ACTION=="vs2010" then
		project "zlib_2010"
	else
		project "zlib"
	end
	
	kind "StaticLib"
	language "C++"
	targetname "zlib"
	
	-- Options
	location ( getZlibLocation("../zlib/projects/") )
	platform = _ACTION
	
	-- Sources
	files { "../zlib/src/**.c",
			"../zlib/include/**.h" }
			
	if _ACTION == "xcode3" then -- Xcode3 specific stuff
	
		-- Config files
		files { "../zlib/make/premake_config/**"}
		excludes { "../zlib/make/premake_config/propertysheets/**"}
		
		-- Include dirs
		includedirs { "../zlib/include" }
		
		-- Global configurations
		targetdir ("../glitchConfig/bin/zlib/premake_" .. platform  .. "/$(CONFIGURATION)")
		objdir ("../glitchConfig/build/zlib/premake_" .. platform)
		
		-- Configurations
		-- Debug
		configuration "debug"
			xcodeconfigreferenceproject "debug.xcconfig"
			
		-- Release
		configuration "release"	
			xcodeconfigreferenceproject "release.xcconfig"
	
	else -- Vs specific stuff

		uuid "56ADAFAD-9F28-3C48-BE79-138F621E52CB"
		if _ACTION == "vs2008" then
			targetdir ("../zlib/lib/msvc2008/$(ConfigurationName)")
			objdir ("$(PlatformName)")
		else
			targetdir ("../zlib/lib/msvc2010/$(Configuration)")
			objdir ("$(PlatformName)")
		end

		-- Configurations
		configuration "Win_Debug"
			vsconfigreference "../zlib/make/premake_config/propertysheets/zlib - Debug"
		
		configuration "Win_Debug_DEF"
			targetname "zlib_d"
			vsconfigreference "../zlib/make/premake_config/propertysheets/zlib - Debug - DEF"
			
		configuration "Win_Debug_DLL"
			targetname "zlib_d"
			vsconfigreference "../zlib/make/premake_config/propertysheets/zlib - Debug - DLL"
			kind "SharedLib"
			
		configuration "Win_Release"
			vsconfigreference "../zlib/make/premake_config/propertysheets/zlib - Release"
			
		configuration "Win_Release_DEF"
			targetname "zlib"
			vsconfigreference "../zlib/make/premake_config/propertysheets/zlib - Release - DEF"
			
		configuration "Win_Release_DLL"
			targetname "zlib"
			vsconfigreference "../zlib/make/premake_config/propertysheets/zlib - Release - DLL"
			kind "SharedLib"
	end		
end

getZlibLocation = function(basePath)
	outputPath = basePath
	
	-- Options
	if _ACTION=="xcode3" then
		if  _OPTIONS["iphone"] then
			outputPath = outputPath .. "xcode"
		else
			outputPath = outputPath .."xcode-macosx"
		end
	else
		if  _ACTION=="vs2008" then
			outputPath = outputPath .. "msvc2008"
		elseif _ACTION=="vs2010" then
			outputPath = outputPath .. "msvc2010"
		else
			outputPath = outputPath .."premake_" .. _ACTION
		end
	end

	return outputPath
end
