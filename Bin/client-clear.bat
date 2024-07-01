@echo off
set sBinDir=Bin
FOR /r %cd% %%a in ("*.exp" "*.lib" "*.pdb" "*_d.dll") do (
  del "%%a"
  echo File: %%a
) 
exit
