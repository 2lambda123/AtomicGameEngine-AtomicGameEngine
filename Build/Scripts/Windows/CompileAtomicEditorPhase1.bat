call "%VS140COMNTOOLS%..\..\VC\bin\amd64\vcvars64.bat"
cmake ..\\..\\..\\ -DATOMIC_DEV_BUILD=0 -G "Visual Studio 14 2015 Win64"
msbuild /m Atomic.sln /t:AtomicNETNative /p:Configuration=%1 /p:Platform=x64
