:: Ce script g�n�re la documentation HTML � partir de la doc int�gr�e au code.
:: Pour qu'il fonctionne, il n�cessite que l'utilitaire Doxygen soit install� sur le poste.
:: Auteur : J�r�me Conus
:: Date   : 06.09.2022

@ECHO OFF

::Avant de changer de dossier, on m�morise le dossier actuel
set OLDDIR=%CD%

:: Pour que doxygen fonctionne, il faut que le dossier de travail soit celui qui contient le Doxyfile
cd ..\src
"C:\Program Files\doxygen\bin\doxygen" Doxyfile

:: R�tablissement du dossier d'origine
cd %OLDDIR%
