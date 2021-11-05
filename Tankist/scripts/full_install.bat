rem Всё должно собираться static runtime

rem *** Create links ***

set start_dir=%CD%
cd install
call create_links.bat
cd %start_dir%

rem *** Clone and build Urho3D ***

cd install\Urho3D
call assembly.bat full all
cd %start_dir%

rem *** Build third party ***

cd install\ThirdParty
call make_build_all.bat
cd %start_dir%

rem *** Build Tankist ***
cd Windows
call make.bat build all
cd %start_dir%

call copy_dll.bat
