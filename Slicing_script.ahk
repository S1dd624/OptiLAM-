#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

;^!r::
Send, {Lwin down}r{Lwin up}
Sleep, 500
Send, "C:\Program Files\INRIA\IceSL\bin\IceSL-slicer.exe" -s "C:\OptiLAM\bin\icesl_script.lua"
Send, {Enter}
return