@SETLOCAL

@REM Set the PYTHON path variable to your python command, like C:\Python33\python.exe
@IF "%PYTHON%" == "" echo Warning: PYTHON environment path not set.

@IF "%DLLPATH_X86%" == "" SET DLLPATH_X86=%CD%\..\dlls\32bit
@IF "%DLLPATH_X64%" == "" SET DLLPATH_X64=%CD%\..\dlls\64bit
@IF "%PYTHONPATH%" == "" SET PYTHONPATH=%CHDIR%
@IF "%PYTHON27_X86%" == "" SET PYTHON27_X86=c:\Python27-x86\python.exe
@IF "%PYTHON27_X64%" == "" SET PYTHON27_X64=c:\Python27-x64\python.exe
@IF "%PYTHON32_X86%" == "" SET PYTHON32_X86=c:\Python32-x86\python.exe
@IF "%PYTHON32_X64%" == "" SET PYTHON32_X64=c:\Python32-x64\python.exe
@IF "%PYTHON33_X86%" == "" SET PYTHON33_X86=c:\Python33-x86\python.exe
@IF "%PYTHON33_X64%" == "" SET PYTHON33_X64=c:\Python33-x64\python.exe
@IF "%PYTHON%" == "" SET PYTHON=%PYTHON27_X64%
@IF "%PYPY18%" == "" SET PYPY18=c:\pypy-1.8\pypy.exe
@IF "%PYPY19%" == "" SET PYPY19=c:\pypy-1.9\pypy.exe
@IF "%PYPY20%" == "" SET PYPY20=c:\pypy-2.0\pypy.exe
@IF "%IRONPYTHON27_X86%" == "" SET IRONPYTHON27_X86=c:\IronPython-2.7.3\ipy.exe
@IF "%IRONPYTHON27_X64%" == "" SET IRONPYTHON27_X64=c:\IronPython-2.7.3\ipy64.exe

@SET INTERP_X64=%PYTHON27_X64%;%PYTHON32_X64%;%PYTHON33_X64%;%IRONPYTHON27_X64%
@SET INTERP_X86=%PYTHON27_X86%;%PYTHON32_X86%;%PYTHON33_X86%;%PYPY18%;^
 %PYPY19%;%PYPY20%;%IRONPYTHON27_X86%
@SET INTERPRETERS=%INTERP_X86%;%INTERP_X64%

@IF "%~1" == "" GOTO :all
@GOTO :%~1

:all
@CALL :clean
@CALL :build
@GOTO :eof

:dist
@ECHO Creating dist...
@CALL :clean
@CALL :docs
@%PYTHON% setup.py sdist --format gztar
@%PYTHON% setup.py sdist --format zip
@GOTO :eof

:bdist
@CALL :clean
@CALL :docs
@ECHO Creating bdist...
@FOR %%A in (%INTERPRETERS%) do %%A setup.py bdist --format=msi
@GOTO :eof

:build
@ECHO Running build
@%PYTHON% setup.py build
@ECHO Build finished, invoke 'make install' to install.
@GOTO :eof

:install
@ECHO Installing...
@%PYTHON% setup.py install
@GOTO :eof

:clean
@RMDIR /S /Q build
@RMDIR /S /Q dist
@FOR /d /r . %%d in (__pycache__) do @IF EXIST "%%d" RMDIR /S /Q "%%d"
@DEL /S /Q MANIFEST
@DEL /S /Q *.pyc
    
@GOTO :eof

:docs
@IF "%SPHINXBUILD%" == "" SET SPHINXBUILD=C:\Python27-x64\Scripts\sphinx-build.exe
@ECHO Creating docs package
@RMDIR /S /Q doc\html
@CD doc
@CALL make html
@MOVE /Y _build\html html
@RMDIR /S /Q _build
@CALL make clean
@CD ..
@GOTO :eof

:release
@CALL :dist
@GOTO :eof

:testall
@FOR /F "tokens=1 delims=" %%A in ('CHDIR') do @SET PYTHONPATH=%%A && @SET IRONPYTHONPATH=%%A
@FOR %%A in (%INTERPRETERS%) do @%%A -B -m dynrules.test.util.runtests
@GOTO :eof

@REM Do not run these in production environments. They are for testing purposes
@REM only!

:buildall
@FOR %%A in (%INTERPRETERS%) do @CALL :clean & @%%A setup.py build
@CALL :clean
@GOTO :eof

:installall
@FOR %%A in (%INTERPRETERS%) do @CALL :clean & @%%A setup.py install
@CALL :clean
@GOTO :eof

:testall2
@FOR %%A in (%INTERPRETERS%) do @%%A -B -c "import dynrules.test; dynrules.test.run()"
@GOTO :eof

:purge_installs
@echo Deleting data...
@RMDIR /S /Q C:\Python27-x86\Lib\site-packages\dynrules
@RMDIR /S /Q C:\Python27-x64\Lib\site-packages\dynrules
@RMDIR /S /Q C:\Python32-x86\Lib\site-packages\dynrules
@RMDIR /S /Q C:\Python32-x64\Lib\site-packages\dynrules
@RMDIR /S /Q C:\Python33-x86\Lib\site-packages\dynrules
@RMDIR /S /Q C:\Python33-x64\Lib\site-packages\dynrules
@RMDIR /S /Q C:\pypy-1.8\site-packages\dynrules
@RMDIR /S /Q C:\pypy-1.9\site-packages\dynrules
@RMDIR /S /Q C:\pypy-2.0\site-packages\dynrules
@RMDIR /S /Q C:\IronPython-2.7.3\Lib\site-packages\dynrules
@echo done
@GOTO :eof

@ENDLOCAL
