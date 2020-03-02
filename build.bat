@echo off

set PROJECT_NAME=malkt
for /F "tokens=*" %%A in ('"git describe HEAD --abbrev=0"') do set PROJECT_VER=%%A

cd %~dp0

set COMPILER=ccrx
set OUTPUT_DIR=bin\%COMPILER%@%PROJECT_VER%
mkdir %OUTPUT_DIR% > NUL 2>&1
%COMPILER% -output=obj -lang=c99 -nostuff -isa=rxv3 ^
           -include=v1/,external/ ^
           src/utkernel/inbox/inbox.c ^
           src/utkernel/isr/isr.c ^
           src/utkernel/memory_pool/memory_pool.c ^
           src/utkernel/task/task.c ^
           src/utkernel/timer/interval_timer.c ^
           src/utkernel/timer/one_shot_timer.c ^
           src/utkernel/timer/timer.c ^
           src/utkernel/uptime/uptime.c
if %errorlevel% neq 0 exit /b

rlink -form=lib -output=%OUTPUT_DIR%/%PROJECT_NAME%.lib *.obj
del *.obj
