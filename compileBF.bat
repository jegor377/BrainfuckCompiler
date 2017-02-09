@echo off
bf %1 %2.asm
if %errorlevel% neq 1 (
	nasm -f win32 %2.asm -o %2.o
	ld %2.o -o %2.exe C:\WINDOWS\system32\msvcrt.dll
	del %2.o
)
del %2.asm