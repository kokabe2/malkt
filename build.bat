@echo off

set PROJECT_NAME=malkt
for /F "tokens=*" %%A in ('"git describe HEAD --abbrev=0"') do set PROJECT_VER=%%A

cd %~dp0

set COMPILER=ccrx
set OUTPUT_DIR=bin\%COMPILER%@%PROJECT_VER%
mkdir %OUTPUT_DIR% > NUL 2>&1
%COMPILER% -output=obj -lang=c99 -nostuff -isa=rxv3 -debug -signed_char ^
           -include=v1/,external/ ^
           src/inbox/utkernel/inbox.c ^
           src/isr/utkernel/basic_isr.c ^
           src/isr/isr.c ^
           src/memory_pool/utkernel/memory_pool.c ^
           src/task/utkernel/task.c ^
           src/time/utkernel/time_unit.c ^
           src/timer/utkernel/interval_timer.c ^
           src/timer/utkernel/one_shot_timer.c ^
           src/timer/utkernel/timer.c ^
           src/uptime/utkernel/uptime.c
if %errorlevel% neq 0 exit /b

rlink -form=lib -output=%OUTPUT_DIR%/%PROJECT_NAME%.lib *.obj
del *.obj
