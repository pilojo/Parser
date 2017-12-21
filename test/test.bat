::Batch testing file
::Created by Daniel Brenot and John Pilon for testing Parser.exe
@echo off

"./compiled/Parser.exe" ass3e.pls > ./output/ass3e.out 2> ./error/ass3e.out
"./compiled/Parser.exe" ass3r.pls > ./output/ass3r.out 2> ./error/ass3r.out
"./compiled/Parser.exe" ass3w1.pls > ./output/ass3w1.out 2> ./error/ass3w1.out
"./compiled/Parser.exe" ass3w2.pls > ./output/ass3w2.out 2> ./error/ass3w2.out
"./compiled/Parser.exe" ass3w3.pls > ./output/ass3w3.out 2> ./error/ass3w3.out
"./compiled/Parser.exe" ass3w4.pls > ./output/ass3w4.out 2> ./error/ass3w4.out

::Additional Scanner test files
"./compiled/Parser.exe" a2empty.pls > ./output/a2empty.out 2> ./error/a2empty.out
"./compiled/Parser.exe" a2error.pls > ./output/a2error.out 2> ./error/a2error.out
"./compiled/Parser.exe" ass2r.pls > ./output/ass2r.out 2> ./error/ass2r.out
"./compiled/Parser.exe" ass2w.pls > ./output/ass2w.out 2> ./error/ass2w.out

fc /B "%cd%\output\ass3e.out" "%cd%\expected\ass3e.out" >nul
if %errorlevel%==1 (echo ass3e.out: Fail)
if %errorlevel%==0 (echo ass3e.out: Pass)
fc /B "%cd%\output\ass3r.out" "%cd%\expected\ass3r.out" >nul
if %errorlevel%==1 (echo ass3r.out: Fail)
if %errorlevel%==0 (echo ass3r.out: Pass)
fc /B "%cd%\output\ass3w1.out" "%cd%\expected\ass3w1.out" >nul
if %errorlevel%==1 (echo ass3w1.out: Fail)
if %errorlevel%==0 (echo ass3w1.out: Pass)
fc /B "%cd%\output\ass3w2.out" "%cd%\expected\ass3w2.out" >nul
if %errorlevel%==1 (echo ass3w2.out: Fail)
if %errorlevel%==0 (echo ass3w2.out: Pass)
fc /B "%cd%\output\ass3w3.out" "%cd%\expected\ass3w3.out" >nul
if %errorlevel%==1 (echo ass3w3.out: Fail)
if %errorlevel%==0 (echo ass3w3.out: Pass)
fc /B "%cd%\output\ass3w4.out" "%cd%\expected\ass3w4.out" >nul
if %errorlevel%==1 (echo ass3w4.out: Fail)
if %errorlevel%==0 (echo ass3w4.out: Pass)
pause