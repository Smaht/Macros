|--------------------------------------------------------------------|
|- e3_Macro_Setup.inc ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ										-|
| -- Modified by Smaht of EQTitan to work with extended (e3x) functionality
| -- e3x syntax best viewed with Haskell
|--------------------------------------------------------------------|
|- Contains setup functions for e3.mac								-|
|- Contains additional functions for e3x (extended)					-|
|--------------------------------------------------------------------|

SUB e3_Setup(modeSelect)
/if (${Debug}) /echo |- e3_Setup ==>

	/declare Debug bool outer FALSE
	/if (${modeSelect.Equal[Debug]}) /varset Debug TRUE

	| The file path for e3 Data.ini will still need to be updated in corresponding includes because you must use /noparse to write variables to inis.
	/declare MacroData_Ini string outer e3 Macro Inis\e3 Data.ini
	/declare i int local

	/call check_Plugins
	
	/echo Loading e3 v${macroVersion}...

	| create a macro data
	/if (!${Ini[${MacroData_Ini}].Length}) {
		/echo Welcome to e3! preforming first time setup...
		/call make_macroDataIni
	}

| Import file paths ======================================================================================================= 

	| General Settings
	/if (!${Ini[${MacroData_Ini},File Paths,General Settings].Length}) {
		/echo ERROR: Could not find designated file path for [General Settings].  Please review review settings in [${MacroData_Ini} > File Paths].
		/endmacro
	} else {
		/declare genSettings_Ini string outer ${Ini[${MacroData_Ini},File Paths,General Settings]}
	}

	| Advanced Settings
	/if (!${Ini[${MacroData_Ini},File Paths,Advanced Settings].Length}) {
		/echo ERROR: Could not find designated file path for [Advanced Settings].  Please review review settings in [${MacroData_Ini} > File Paths].
		/endmacro
	} else {
		/declare advSettings_Ini string outer ${Ini[${MacroData_Ini},File Paths,Advanced Settings]}
	}
	
	| Bot Settings
	/if (!${Ini[${MacroData_Ini},File Paths,Bot Settings].Length}) {
		/echo ERROR: Could not find designated file path for [Bot Settings].  Please review review settings in [${MacroData_Ini} > File Paths].
		/endmacro
	} else {
		/declare Character_Ini string outer ${Ini[${MacroData_Ini},File Paths,Bot Settings]}
	}
	
| Create Ini files ======================================================================================================== 
	/if (${Bool[${Ini[${genSettings_Ini}]}]} && !${Ini[${MacroData_Ini},e3 Build,Version].Equal[${macroVersion}]}) /echo New version of e3 detected. Hold on to your nuggets!
	
	| Macro Setting Inis
	/if (!${Ini[${MacroData_Ini},e3 Build,Version].Equal[${macroVersion}]} || !${Bool[${Ini[${genSettings_Ini}]}]} || !${Ini[${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Bot_Ini version].Equal[${macroVersion}]}) /call EVENT_make_MacroSettingsIni
	
	| Macro Aliases
	/if (!${Ini[${MacroData_Ini},e3 Build,Version].Equal[${macroVersion}]} || !${Ini[${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Bot_Ini version].Equal[${macroVersion}]}) /call EVENT_updateAliases

	| Generate Character_Ini.
	/if (!${Ini[${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Bot_Ini version].Equal[${macroVersion}]} || !${Bool[${Ini[${Character_Ini}]}]}) /call EVENT_make_CharacterSettingsIni

	| Update macro version in e3 Data.ini
	/if (!${Ini[${MacroData_Ini},e3 Build,Version].Equal[${macroVersion}]}) /call WriteToIni "${MacroData_Ini},e3 Build,Version" "${macroVersion}" 1
	
| Import ini settings ===================================================================================================== 
	
	/echo Importing user settings...
	
	| Debug
	/if (!${modeSelect.Equal[Debug]}) /call IniToVar "${advSettings_Ini},Debug,Debug Mode (On/Off)" Debug bool outer
	
	| Chat Channel
	/declare ChatToggle string outer /bc
	/if (${Ini[${Macro_Ini},General,Macro Output Channel].Length}) /call IniToVar "${Macro_Ini},General,Macro Output Channel" ChatToggle string outer

	/call IniToArray "${advSettings_Ini},${Me.Class} Included Setups,Include#" macroSetups
	/call IniToArray "${advSettings_Ini},${Me.Class.ShortName} Functions,${Me.Class.ShortName} Function#" mainLoop_Array
	
| Setup Includes ========================================================================================================== 

	/for i 1 to ${macroSetups.Size}
		/if (${macroSetups[${i}].Equal[class]}) /varset macroSetups[${i}] ${Me.Class.ShortName}
		/if (${Bool[${macroSetups[${i}]}]}) /call ${macroSetups[${i}]}_Setup
	/next i

| Complete a pending exchange ============================================================================================= 
	/if (${Ini[${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Pending Exchange].Length} && ${Ini[${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Pending Exchange].NotEqual[NONE]} && !${itemExchange_Timer}) /call completePendingExchange
	
| ========================================================================================================================= 
	
	/echo e3 loaded.
	/echo
	
	/echo ** For updates, issues, suggestions, or donations, visit 'http://code.google.com/p/e3-macro-builds/'.
	/echo ** For help using e3, use /e3help to learn about different basic functions and /comannds.
	/if (${NetBots.Client.Find[Tackleberry]}) /echo Though you may find this message a bit 'extreme' so is bitching about a few lines of text which can easily be ignored.
	
	/echo
	
	/if (${Debug}) {
		/echo
		/echo
		/echo *** Debug_Mode ON ***
		/echo The macro has paused so you can review setup information, type /mqpause to continue.
		/echo
		/echo
		/mqpause
	}

/if (${Debug}) /echo <== e3_Setup -|	
/RETURN



|-------------------------
|- Creates a file settings file to change file paths, and store various pieces of data
|-------------------------
SUB make_macroDataIni
/if (${Debug}) /echo |- make_macroDataIni ==>

	/noparse /ini "e3 Macro Inis\e3 Data.ini" "e3 Build" "Version"
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "Bot Settings" "e3 Bot Inis\${MacroQuest.Server}-${Me.CleanName}.ini"
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "General Settings" "e3 Macro Inis\General Settings.ini"
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "Advanced Settings" "e3 Macro Inis\Advanced Settings.ini"

	| ---------- e3x file locations
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "Deployment Locations" "e3 Macro Inis\Locations - Deployment.ini"
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "Engagement Locations" "e3 Macro Inis\Locations - Engagement.ini"
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "Typical Buffs" "e3 Macro Inis\Typical Buffs.ini"
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "Interesting Mobs" "e3 Macro Inis\Interesting Mobs.ini"
	/noparse /ini "e3 Macro Inis\e3 Data.ini" "File Paths" "CH Events" "e3 Macro Inis\e3x_CH.ini"

/if (${Debug}) /echo <== make_macroDataIni -|
/RETURN



|--------------------------------------------------------------------------------|
|- Generates a general Macro Settings file, to be used by multiple includes.	-|
|--------------------------------------------------------------------------------|
#EVENT make_MacroSettingsIni "[MQ2] Make Macro ini"
SUB EVENT_make_MacroSettingsIni
/if (${Debug}) /echo |- make_MacroSettingsIni ==>

	/echo Creating macro settings files...

	/call WriteToIni "${genSettings_Ini},General,Macro Output Channel" /bc
	
	/call WriteToIni "${advSettings_Ini},Debug,Debug Mode (On/Off)" Off
	
	/call ${Me.Class.ShortName}_MacroSettings

	| Import the array of Included Setups
	/call IniToArray "${advSettings_Ini},${Me.Class} Included Setups,Include#" macroSetups
	
	| Call each setup
	/declare i int local
	
	/for i 1 to ${macroSetups.Size}
		/if (${macroSetups[${i}].Equal[class]}) /next i
		/if (${Bool[${macroSetups[${i}]}]}) /call ${macroSetups[${i}]}_MacroSettings
	/next i

/if (${Debug}) /echo <== make_MacroSettingsIni -|
/RETURN

	

|--------------------------------------------------------------------------------|
|- Generates a general Macro Settings file, to be used by multiple includes.	-|
|--------------------------------------------------------------------------------|
#EVENT make_CharacterSettingsIni "[MQ2] Make bot ini"
SUB EVENT_make_CharacterSettingsIni
/if (${Debug}) /echo |- make_CharacterSettingsIni ==>

	/echo Creating ${Me.CleanName}'${If[${Me.CleanName.Right[1].Equal[s]},,s]} settings file...
	
	/call IniToArray "${advSettings_Ini},${Me.Class} Included Setups,Include#" macroSetups
	
	/declare i int local
	
	/for i 1 to ${macroSetups.Size}
		/if (${macroSetups[${i}].Equal[class]}) /varset macroSetups[${i}] ${Me.Class.ShortName}
		/if (${Bool[${macroSetups[${i}]}]}) /call ${macroSetups[${i}]}_CharacterSettings
	/next i
	
	/call WriteToIni "${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Bot_Ini version" "${macroVersion}" 1

/if (${Debug}) /echo <== make_CharacterSettingsIni -|
/RETURN	



|------------------------------------------------------------|
|- Deletes and Recreates aliases associated with e3.mac.	-|
|------------------------------------------------------------|
#EVENT updateAliases "[MQ2] Update macro aliases"
SUB EVENT_updateAliases
/if (${Debug}) /echo |- EVENT_updateAliases ==>

	/echo Updating macro aliases....
	/echo (WARNING: Your clients will lock up for a few moments.  Stand fast, friend...)
	/echo Aliases may not work properly until you restart MQ2 on ALL clients.
	
	| Core aliases
	/squelch /alias /updateAliases /echo Update macro aliases
	/squelch /alias /makeCharIni /echo Make Character Ini
	/squelch /alias /makeSettingsIni /echo Make General Settings Ini
	/squelch /alias /makeAdvSettingsIni /echo Make Advanced Settings Ini
	/squelch /alias /e3help /echo Help
	/squelch /alias /debugon /varset Debug 1
	/squelch /alias /debugoff /varset Debug 0

	/call IniToArray "${advSettings_Ini},${Me.Class} Included Setups,Include#" macroSetups
	
	/declare i int local
	
	/for i 1 to ${macroSetups.Size}
		/if (${macroSetups[${i}].Equal[class]}) {
			/varset macroSetups[${i}] ${Me.Class.ShortName}
			/next i
		}
		/if (${Bool[${macroSetups[${i}]}]}) /call ${macroSetups[${i}]}_Aliases
	/next i

	/call e3x_ExtraUtilities_Aliases
	/call e3x_Class_Aliases
	/call VetAAs_Aliases

/if (${Debug}) /echo <== EVENT_updateAliases -|
/RETURN



|----------------------------------------------------|
|- Checks for and enables required macro plugins.	-|
|----------------------------------------------------|
SUB check_Plugins
/echo Checking plugins...

| REQUIRED PLUGINS ==>


	| REQUIRED PLUGINS ==>
	
| -----------------------------------------------------------------------------------------------------------------------------------Check MQ2EQBC.
	/if (!${Plugin[MQ2EQBC].Name.Length}) {
		/echo Plugin MQ2EQBC is not loaded, attempting to resolve...
		/plugin MQ2EQBC
		/delay 10s !${Select[${EQBC},NULL]}
		/if (${Select[${EQBC},NULL]}) {
			/echo ***WARNING*** Could not load MQ2EQBC, macro functionality may be limited.
			/echo Starting in Solo Mode...
		}
	}

| -------------------------------------------------------------------------------------------Auto-Connect to EQBCS.
	/if (${Bool[${EQBC}]}) {
		/if (!${EQBC.Connected}) {
			/bccmd connect
			/delay 100 ${EQBC.Connected}
			/delay 5
			/if (!${EQBC.Connected}) /echo ***WARNING*** Could not connect to EQBCS! Please open EQBCS and try again.  Macro functionality may be limited...
		}


	| --------------------------------------------------------------------------------------------------------------------------------Check MQ2NetBots.
		/if (!${Plugin[MQ2NetBots].Name.Length}) {
			/echo Plugin MQ2NetBots is not loaded, attempting to resolve...
			/plugin MQ2NetBots
			/delay 3s ${NetBots}
			/if (!${NetBots}) {
				/echo ***WARNING*** Could not load MQ2NetBots! Macro functionality may be limited.
			}
		}
		
	| -------------------------------------------------------------------------------------------Auto-Connect to NetBots.
		/if (${NetBots}) {
			/squelch /netbots on grab=on send=on
		}
		
	| -----------------------------------------------------------------------------------------------------------------------------Check MQ2NetAdvPath.
		/if (!${Plugin[MQ2NetAdvPath].Name.Length}) {
			/echo Plugin MQ2NetAdvPath is not loaded, attempting to resolve...
			/plugin MQ2NetAdvPath
			/delay 3s ${NetAdvPath}
			/if (!${NetAdvPath}) {
				/echo ***WARNING*** Could not load MQ2NetAdvPath. Please ensure you're using a copy of MQ2 which was compiled with the MQ2NetAdvPath plugin and try again.
			}
		}
	}
	
| ------------------------------------------------------------------------------------------------------------------------------Check MQ2MoveUtils.
	/if (!${Plugin[MQ2MoveUtils].Name.Length}) {
		/echo Plugin MQ2MoveUtils is not loaded, attempting to resolve...
		/plugin MQ2MoveUtils
		/delay 3s !${Select[${Stick.Status},NULL]}
		/if (${Select[${Stick.Status},NULL]}) {
			/echo ***WARNING*** Could not load MQ2MoveUtils! Macro functionality may be limited.
			/if (!${NetAdvPath}) {
				/echo Follow and Assist stick DISABLED.
			} else {
				/echo Assist and NPC stick DISABLED.
				/echo Follow restricted to NetBots.
			}
		}
	}	

| -------------------------------------------------------------------------------------------------------------------------------Check MQ2Exchange.
	/if (!${Plugin[MQ2Exchange].Name.Length}) {
		/echo Plugin MQ2Exchange is not loaded, attempting to resolve...
		/plugin MQ2Exchange
		/delay 3s ${Plugin[MQ2Exchange].Name.Length}
		/if (!${Plugin[MQ2Exchange].Name.Length}) {
			/echo ***WARNING*** Could not load MQ2Exchange! Macro functionality may be limited.
			/echo Item swapping is DISABLED.
		}
	}

| ----------------------------------------------------------------------------------------------------------------------------------Check MQ2Twist.
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/if (!${Plugin[MQ2Twist].Name.Length}) {
			/echo Plugin Unable to detect MQ2Twist, attempting to resolve...
			/plugin MQ2Twist
			/delay 3s !${Select[${Twist},NULL]}
			/if (${Select[${Twist},NULL]}) {
				/echo ***WARNING*** Could not load MQ2Twist! Macro functionality may be limited.
				/echo Bard melodies DISABLED.
			}
		}	
	}

| <== REQUIRED PLUGINS

| ----------------------------------------------------------------------------------------------------------------------------Check MQ2ItemDisplay.
	/if (!${Plugin[mq2itemdisplay].Name.Length}) /plugin MQ2ItemDisplay

| ------------------------------------------------------------------------------------------------------------------------------------Check MQ2HUD.
|	/if (!${Plugin[mq2hud].Name.Length}) /plugin MQ2HUD	
	
| <== REQUIRED PLUGINS
/RETURN