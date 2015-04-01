call "..\..\glitch\tools\LibToDefExporter\configDefExporter.bat"

SET CONFIGURATION="Win_Debug_DEF"
SET ZLIB_LIB_FILE="..\lib\msvc2008\Win_Debug_DEF\zlib_d.lib"

%MSBUILD_EXE% "..\projects\msvc2008\zlib_2008.vcproj" /maxcpucount:4 /p:Configuration=%CONFIGURATION% /t:build

call %LIB_TO_DEF_EXPORTER% -in %ZLIB_LIB_FILE% -out "..\..\zlib_config\zlib_DEBUG.def"