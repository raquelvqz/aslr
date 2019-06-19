@ECHO off

FOR /L %%A IN (1,1,100) DO (
  a.exe
)

@shutdown /r /f /t 0