| --------------------------------------------------------------------------------------|
| -- e3x_Utilities v1.0	( extended functionality designed to work with e3.mac )	       -|
| -- Written by Smaht							                                       -|
| --------------------------------------------------------------------------------------|
| -- Contains extended and miscellaneous EVENTs and functions for e3.mac.	           -|
| --------------------------------------------------------------------------------------|

| --------------------------------------------------------------------------------------|
| -- Attendance: determines if netbot clients are near, feetwet or drowning            -|
| --------------------------------------------------------------------------------------|

#EVENT attendance "<#1#> Attendance"

SUB EVENT_attendance(line, ChatSender)

| ---------------- If I made the attendance call, continue, otherwise exit
/if (${Me.CleanName.Equal[${ChatSender}]}) {

	/declare e3xUtilitiesDebug bool local FALSE
	/declare shortDistList string local
	/declare medDistList string local
	/declare feetwetList string local
	/declare drowningList string local
	/declare outOfZoneList string local

	| ---------------- Distances against which to check
	/declare shortDistance int local 30
	/declare medDistance int local 85

	/declare i int local

	| ---------------- Set targetlist to the NetBots.Client list
	/for i 1 to ${NetBots.Counts}

    	| ---------------- For any NetBots.Client, if InZone and (Distance3D > shortDistance) and !Me add to shortDistList
		/if (${NetBots[${NetBots.Client.Arg[${i}]}].InZone} && (${Spawn[${NetBots.Client.Arg[${i}]}].Distance3D} > ${shortDistance}) && !${NetBots.Client.Arg[${i}].Equal[${Me.Name}]}) /varset shortDistList ${shortDistList} ${NetBots.Client.Arg[${i}]}

		| ---------------- For any NetBots.Client, if !InZone add to outOfZoneList
		/if (!${NetBots[${NetBots.Client.Arg[${i}]}].InZone}) /varset outOfZoneList ${outOfZoneList} ${NetBots.Client.Arg[${i}]}

		| ---------------- Debug info
		/if (${e3xUtilitiesDebug} || ${Debug}) {
			/echo shortdistList : ${NetBots.Client.Arg[${i}]} : ${Spawn[${NetBots.Client.Arg[${i}]}].Distance3D} : InZone=${NetBots[${NetBots.Client.Arg[${i}]}].InZone}
			/echo Current list for shortDistList is: ${shortDistList}
    		/echo outOfZoneList : ${NetBots.Client.Arg[${i}]} : InZone=${NetBots[${NetBots.Client.Arg[${i}]}].InZone}
			/echo Current list for outOfZoneList is: ${outOfZoneList}
		}

        /next i

    | ---------------- Report outOfZoneList
    /if (${outOfZoneList.Length}) /bc [+r+] Out of Zone: ${outOfZoneList}

	| ---------------- If no one in the list, report everyone present and goto underwater checks
	/if (!${shortDistList.Length}) /multiline ; /bc [+r+] Everyone present.;/goto :underwater

	| ---------------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) /echo shortdistList length is ${Math.Calc[${shortDistList.Count[ ]} + 1]}

	/varset i 1
    /for i 1 to ${Math.Calc[${shortDistList.Count[ ]} + 1]}

		| ---------------- For any shortDistList member, if (Distance3D > medDistance) add to medDistList
		/if (${Spawn[${shortDistList.Arg[${i}]}].Distance3D} > ${medDistance}) /varset medDistList ${medDistList} ${shortDistList.Arg[${i}]}

		| ---------------- Debug info
		/if (${e3xUtilitiesDebug} || ${Debug}) {
			/echo medDistList : ${shortDistList.Arg[${i}]} : ${Spawn[${shortDistList.Arg[${i}]}].Distance3D}
			/echo Current list for medDistList is: ${medDistList}
		}

		/next i

	| ---------------- Report shortDistList members and medDistList members if they exist
	/bc The following are not present: ${shortDistList}
	/if (${medDistList.Length}) /bc The following are further than the rest: ${medDistList}

	:underwater

	| ---------------- If I am feetwet skip, otherwise figure out who is in the water
	/if (!${Me.FeetWet}) {

		/varset i 1
    	/for i 1 to ${NetBots.Counts}

    		/if (${Spawn[${NetBots.Client.Arg[${i}]}].FeetWet}) /varset feetwetList ${feetwetList} ${NetBots.Client.Arg[${i}]}

    		| ---------------- Debug info
			/if (${e3xUtilitiesDebug} || ${Debug}) {
				/echo FeetWet: ${NetBots.Client.Arg[${i}]} : ${Spawn[${NetBots.Client.Arg[${i}]}].FeetWet}
				/echo Current list for feetwetList is: ${feetwetList}
			}

			/next i

		/if (${feetwetList.Length}) /bc [+r+] The following are in the drink! ${feetwetList}
	}

	| ---------------- If anyone is drowning, report it

	/noparse /squelch /bcaa //if (${Window[BreathWindow].Open}) /bc [+r+] DROWNING!
}

/return

| --------------------------------------------------------------------------------------|
| -- Command group leaders to all request a specific group buff.	                   -|
| --------------------------------------------------------------------------------------|

#event BuffAllSpecific "<#*#> BuffAS: #1# #2#"
SUB EVENT_BuffAllSpecific(line, fromWho, whatBuff)

	|---------------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) {
		/echo $line is: ${line}
		/echo $fromWho is: ${fromWho}
		/echo $whatBuff is: ${whatBuff}
	}

	/if (${Select[${Group.Leader},${Me}]}) /t ${fromWho} ${whatBuff}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Item to Cursor:  Finds item and places it on your cursor (stacked if stackable)					 -|
| --------------------------------------------------------------------------------------------------------|

SUB itemToCursor(itemName)

	| ---------------- Declare some variables
	/declare e int local
	/declare x int local
	/declare miscTimer timer local 3s

	| ---------------- Open the inventory, really really make sure it's open
	:openInventory
	/if (!${Window[InventoryWindow].Open}) /nomodkey /keypress i
	/delay 5 ${Window[InventoryWindow].Open}
	/if (!${Window[InventoryWindow].Open} && ${miscTimer}) /goto :openInventory
		
	| ---------------- Loop looking for item.  Picks it up and puts in on the cursor				
	/for x 1 to 8  
					
		| ---------------- Checek the packslots
		/if (!${Me.Inventory[pack${x}].Container} && ${itemName.Equal[${Me.Inventory[pack${x}]}]}) {
			
			/nomodkey /shift /itemnotify pack${x} leftmouseup

		} else {
										
			| ---------------- Otherwise check in each pack
			/for e 1 to ${Me.Inventory[pack${x}].Container}
				/if (${itemName.Equal[${Me.Inventory[pack${x}].Item[${e}]}]}) {
					/if (!${Window[pack${x}].Open}) /itemnotify pack${x} rightmouseup
					/delay 1s ${Window[pack${x}].Open}
					/nomodkey /shift /itemnotify in pack${x} ${e} leftmouseup
				}
			/next e
		}
	/next x

	| ---------------- Set ${Macro.Return} appropriately
	/if (${Cursor.ID}) {
		/return ITEM_FOUND
		} else {
			/return ITEM_NOT_FOUND
		}
	
/return


| --------------------------------------------------------------------------------------------------------|
| -- Loot on event																						 -|
| --------------------------------------------------------------------------------------------------------|

#event lootOn "#*#Loot On"
SUB EVENT_lootOn
	
	| ---------------- Record loot state prior to making changes
	/declare initialLootState bool local ${Auto_Loot}

	/varset Auto_Loot TRUE
	/echo test loot

	| ---------------- Report changes in /bc
	/bc Loot State was ${initialLootState} and is now ${Auto_Loot}.  FreeInventory = ${Me.FreeInventory}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Loot off event																			 			 -|
| --------------------------------------------------------------------------------------------------------|

#event lootOff "#*#Loot Off"
SUB EVENT_lootOff

	| ---------------- Record loot state prior to making changes
	/declare initialLootState bool local ${Auto_Loot}

	/varset Auto_Loot FALSE

	| ---------------- Report changes in /bc
	/bc Loot State was ${initialLootState} and is now ${Auto_Loot}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Check for buff																						 -|
| --------------------------------------------------------------------------------------------------------|

#event checkForBuff "#*#Check for buff: #1#"
SUB EVENT_checkForBuff(line, buffSpell)

	| ---------------- Report in /bc if you DON'T have ${buffSpell}
	/if (!${Me.Buff[${buffSpell}].ID}) /bc I am missing ${buffSpell}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Look for debuff																					 -|
| --------------------------------------------------------------------------------------------------------|

#event checkForDebuff "#*#Check for debuff: #1#"
SUB EVENT_checkForDebuff(line, debuffSpell)

	| ---------------- Report in /bc if you DO have ${debuffSpell}
	/if (${Me.Buff[${debuffSpell}].ID}) /bc I have contracted ${debuffSpell}!

/return

| --------------------------------------------------------------------------------------------------------|
| -- Mount up																							 -|
| --------------------------------------------------------------------------------------------------------|

#event mountUp "#*#Mount Up!"
SUB EVENT_mountUp

	| ---------------- If you have the mount item, use it if not already mounted
	/declare mountItem string local Black Rope Bridle

	/if (${FindItem[${mountItem}].ID} && !${Me.Buff[Summon Horse].ID}) {
			/call e3_Cast ${Me.ID} "${mountItem}"
	}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Dismount upon entering a zone																		 -|
| --------------------------------------------------------------------------------------------------------|

#event dismountOnZone "#*#LOADING, PLEASE WAIT#*#"
#event dismountOnZone "#*#Dismount!"
SUB EVENT_dismountOnZone

	|/dismount
	/call EVENT_RemoveBuff NULL ${Me.CleanName} "Summon Horse"

	|---------------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) /echo 00: Tried to dismount

/return

| --------------------------------------------------------------------------------------------------------|
| -- Build Sewers Expedition Group (Grimy Turepta)														 -|
| --------------------------------------------------------------------------------------------------------|

#event sewersGroup "#*#Build Sewers Group"
SUB EVENT_sewersGroup

|	/windowstate DynamicZoneWnd open

	/declare sewersGroupMember1 string local Smaht
	/declare sewersGroupMember2 string local Iddyhitty
	/declare sewersGroupMember3 string local Tinee
	/declare sewersGroupMember4 string local Kwazyhatchet
	/declare sewersGroupMember5 string local Kwazybludgeon

	/dzadd ${sewersGroupMember1}
	/dzadd ${sewersGroupMember2}
	/dzadd ${sewersGroupMember3}
	/dzadd ${sewersGroupMember4}
	/dzadd ${sewersGroupMember5}
	/delay 10

	/delay 10
	/bca //notify ConfirmationDialogBox Yes_Button leftmouseup

	/windowstate DynamicZoneWnd close

/return


| --------------------------------------------------------------------------------------------------------|
| -- EQBC Reconnect			565:565:565																			 -|
| --------------------------------------------------------------------------------------------------------|

#event eqbcReconnect "#*#did not recieve expected#*#"
#event eqbcReconnect "#*#recon now#*#"
#event eqbcReconnect "#*#:-1:#*#"
SUB EVENT_eqbcReconnect

	/delay 2s
	/bccmd Reconnect

/return

| --------------------------------------------------------------------------------------------------------|
| -- Pre-boss fight bot deployment																		 -|
| -- Usage: /bc Deploy: Quarm ("/deploy Quarm" in MQ2.ini file)											 -|
| -- Moves each character class to it's pre-fight deployment location									 -|
| -- Where locations are defined in the file "Locations - Deployment.ini"								 -|
| --------------------------------------------------------------------------------------------------------|

#EVENT Deploy "<#1#> Deploy: #2#"
Sub EVENT_Deploy(line, deployActor, deployTarget)

	| ---------- Announce into /bc confirmation of the appropriate target.  Smrt because it's prettier
	/if (${Me.Name.Equal[Smrt]}) /bc [+r+] Deploying for the encounter against ${deployTarget}

	| ---------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) /echo Deploy_00: deployTarget is ${deployTarget}
		
	| ---------- Define a variable for use in this, and the engagement, subroutine (ie who are we fighting)
	/if (!${Defined[deployRaidTarget]}) {
		/declare deployRaidTarget string outer ${deployTarget}
	} else {
		
		| ---------- If the deployTarget is different than the currently defined deployRaidTarget, cleanup old variables before beginning
		|/if (!${deployTarget.Equal[${deployRaidTarget}]}) /call deployCleanup
		
		/varset deployRaidTarget ${deployTarget}
	}

	| ---------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) /echo Deploy_10: deployRaidTarget is ${deployRaidTarget}

	| ---------- Echo out engagement information as contained in the file "Engagement Info.ini"
	/if (${Me.Name.Equal[${deployActor}]}) /call EVENT_macroHelp "NULL" "${deployTarget}"

	| ---------- Define the location of deployment locations
	/if (!${Defined[deployLocations_Ini]}) /declare deployLocations_Ini string outer ${Ini[${MacroData_Ini},File Paths,Deployment Locations]}

	| ---------- For this raid target, for your class, get the deploymennt location from the deployLocations_Ini
	/call IniToVar "${deployLocations_Ini},${deployRaidTarget},${Me.Class.ShortName}" deployRaidTargetLocation string outer

	| ---------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) {
		/echo Deploy_20: deployRaidTarget is ${deployRaidTarget}
		/echo Deploy_21: deployLocations_Ini is ${deployLocations_Ini}
		/echo Deploy_22: deployRaidTargetLocation is ${deployRaidTargetLocation}
	}

	| ---------- Move to the deployRaidTargetLocation
	/if (!${deployRaidTargetLocation.Length}) {
		/echo No setting for deployRaidTargetLocation for ${Me.Class.ShortName}
	} else {
		/moveto loc ${deployRaidTargetLocation}
	}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Mid-boss fight bot engagement of characters														 -|
| -- Usage: /bc Engage: Quarm ("/engage Quarm" in MQ2.ini file)											 -|
| -- Moves each character class to it's mid-fight engagement location									 -|
| -- Where locations are defined in the file "Locations - Engagement.ini"								 -|
| --------------------------------------------------------------------------------------------------------|

#EVENT Engage "<#*#> Engage Target!"
Sub EVENT_Engage(line)

	/echo Moving to engagement positions for ${deployRaidTarget}

	| ---------- Define the location of engagement locations
	/if (!${Defined[engagementLocations_Ini]}) /declare engagementLocations_Ini string outer ${Ini[${MacroData_Ini},File Paths,Engagement Locations]}

	| ---------- For this raid target, for your class, get the engagement location from the engagementLocations_Ini
	/call IniToVar "${engagementLocations_Ini},${deployRaidTarget},${Me.Class.ShortName}" engageRaidTargetLocation string outer

	| ---------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) {
		/echo Engage_00: deployRaidTarget is ${deployRaidTarget}
		/echo Engage_01: engagementLocations_Ini is ${engagementLocations_Ini}
		/echo Engage_02: engageRaidTargetLocation is ${engageRaidTargetLocation}
	}

	| ---------- Move to the engageRaidTargetLocation
	/if (!${engageRaidTargetLocation.Length}) {
		/echo No setting for engageRaidTargetLocation for ${Me.Class.ShortName}
	} else {

		| ---------- /moveto new location.  Pause 1s to have time to actually begin moving if you are in fact going to move
		/moveto loc ${engageRaidTargetLocation}
		/delay 10

		| ---------- Wait until you are no longer moving, then re-target the ${deployRaidTarget} and face them
		:stillMoving
		/if (${Me.Moving}) {
			/delay 5
			/goto :stillMoving
		}

		/target ${deployRaidTarget}
		/face fast
	}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Cleanup functions assocaited with Deploy and Engage functions										 -|
| --------------------------------------------------------------------------------------------------------|

Sub deployCleanup

	/if (${Defined[deployRaidTarget]}) /deletevar deployRaidTarget
	/if (${Defined[deployLocations_Ini]}) /deletevar deployLocations_Ini
	/if (${Defined[deployRaidTargetLocation]}) /deletevar deployRaidTargetLocation
	/if (${Defined[engageRaidTargetLocation]}) /deletevar engageRaidTargetLocation

/return

| --------------------------------------------------------------------------------------------------------|
| -- End all deployment/engagement functions and cleanup assocaited variables							 -|
| --------------------------------------------------------------------------------------------------------|

#EVENT endEngagement "<#*#> End Engagement!"
Sub EVENT_endEngagement(line)

	/call deployCleanup

/return

| --------------------------------------------------------------------------------------------------------|
| -- Report status on typical buffs per characters														 -|
| --------------------------------------------------------------------------------------------------------|

#EVENT checkTypicalBuffs "<#*#> Check for Typical Buffs"
SUB EVENT_checkTypicalBuffs(line)

	| ---------- Declare file path for the location of the Typical Buffs.ini
	/declare TypicalBuffs_Ini string local ${Ini[${MacroData_Ini},File Paths,Typical Buffs]}

	| ---------- Create an array of typicalBuffs for the class in quweation
	/if (${Ini[${TypicalBuffs_Ini},${Me.Class},Buff#1].Length}) /call IniToArray "${TypicalBuffs_Ini},${Me.Class},Buff#" typicalBuffs

	| ---------- For each item in the array, check to see if if it's missing. Report as necessary
	/declare i int local
	/for i 1 to ${typicalBuffs.Size}

		/if (!${Me.Buff[${typicalBuffs[${i}]}].ID}) /bc [+g+] Missing Buff #${i}: ${typicalBuffs[${i}]}!

	/next i

/return

| --------------------------------------------------------------------------------------|
| -- Checks an ini file (Interesting Mobs.ini) for interesting mobs in the current zone-|
| --------------------------------------------------------------------------------------|

#EVENT interestingMobs "<#1#> Check for Interesting Mobs#*#"
SUB EVENT_interestingMobs(line, cimActor)

	| ---------- Exit if you are not the one who called for it
	/if (!${Me.Name.Equal[${cimActor}]}) /return

	| ---------- Declare file path for the location of the Typical Buffs.ini
	/declare InterestingMobs_Ini string local ${Ini[${MacroData_Ini},File Paths,Interesting Mobs]}

	| ---------- Create an array of Interesting Mobs for the zone in question
	/if (${Ini[${InterestingMobs_Ini},${Zone.ShortName},Interesting Mob#1].Length}) /call IniToArray "${InterestingMobs_Ini},${Zone.ShortName},Interesting Mob#" interestingMobs

	| ---------- Declare some variables for temporary use
	/declare im_Name string local
	/declare im_Loot string local
	/declare im_Epic string local
	/declare im_Loc string local
	/declare im_Color string local
	/declare im_Final string local

	| ---------- For each item in the array, check to see if if it's missing. Report as necessary
	/declare i int local
	/for i 1 to ${interestingMobs.Size}

		| --------- Set some variables
		/call argueString NPC "${interestingMobs[${i}]}"
		/varset im_Name ${Macro.Return}
	
		/call argueString Loot "${interestingMobs[${i}]}"
		/varset im_Loot ${Macro.Return}

		/call argueString Loc "${interestingMobs[${i}]}"
		/varset im_Loc ${Macro.Return}

		/call argueString Epic "${interestingMobs[${i}]}"
		/varset im_Epic ${Macro.Return}

		| --------- Determine text colour
		/if (${Spawn[${im_Name}].ID}) {
			/varset im_Color g
			/if (!${im_Epic.Equal[NULL]}) /varset im_Color p
		} else {
			/varset im_Color y
		}

		| ---------- Write the final string
		/varset im_Final /bc [+${im_Color}+] IM #${i} // NPC: ${im_Name}
		/if (${Spawn[${im_Name}].ID}) /varset im_Final ${im_Final} ID: ${Spawn[${im_Name}].ID}
		/if (!${im_Epic.Equal[NULL]}) /varset im_Final ${im_Final} Epic: ${im_Epic}
		/if (!${im_Loot.Equal[NULL]}) /varset im_Final ${im_Final} Loot: ${im_Loot}
		/if (!${im_Loc.Equal[NULL]}) /varset im_Final ${im_Final} Loc: ${im_Loc}

		/docommand ${im_Final}

	/next i

/return

| --------------------------------------------------------------------------------------------------------|
| -- Creates a comma-seperated string from a one-dimensional array (MUST be a one-dimensional array) 	 -|
| -- To be replaced if I ever come up with a simple way to determine if a value is contained anywhere	 -|
| -- within the one-dimensional array			 														 -|
| --------------------------------------------------------------------------------------------------------|

SUB ArrayToString(arrayName, stringName)

	/declare e3xUtilitiesDebug bool local FALSE

	| ---------------- Debug info
	/if (${Debug} || ${offtankDebug}) {
		/echo 00: e3xUtilitiesDebug set to ${e3xUtilitiesDebug}
		/echo 01: arrayName set to ${arrayName}
		/echo 02: arrayName.Size set to ${${arrayName}.Size}
		/echo 02: stringName set to ${arrayName}
	}

	| ---------------- Exit because the arrayName is not defined
	/if (!${Defined[${arrayName}]}) {

		| ---------------- Debug info
		/if (${Debug} || ${offtankDebug}) /echo 10: arrayName set to ${arrayName} not found (undefined)
		/return FAILED
	}

	/declare i int local
	/declare ${stringName} string outer

	| ---------------- Loop through arrayName and write each element to stringName
	/for i 1 to ${${arrayName}.Size}
		/if (${i} == 1) {
			/varset ${stringName} ${${arrayName}[${i}]}

			| ---------------- Debug info
			/if (${Debug} || ${offtankDebug}) {
				/echo 20: i set to ${i}
				/echo 21: arrayName[${i}] set to ${${arrayName}[${i}]}
				/echo 22: stringName (${stringName}) set to ${${stringName}}
			}

		} else {
			/varset ${stringName} ${${stringName}},${${arrayName}[${i}]}

			| ---------------- Debug info
			/if (${Debug} || ${offtankDebug}) {
				/echo 30: i set to ${i}
				/echo 31: arrayName[${i}] set to ${${arrayName}[${i}]}
				/echo 32: stringName (${stringName}) set to ${${stringName}}
			}
		}

	/next i

/return

| --------------------------------------------------------------------------------------------------------|
| -- Determines if a bot is dead by the number of items of gear they have on 							 -|
| -- 5 or more empty spots is "Dead"																	 -|
| --------------------------------------------------------------------------------------------------------|

Sub CheckDead

	/declare CheckDeadDebug bool local TRUE
	/declare CheckDeadDebugChannel string local echo

	| ---------- Debug info
	/if (${CheckDeadDebug}) {
	 	/docommand /${CheckDeadDebugChannel} ${Me.CleanName}: Starting logs for CheckDead
	}
	
	/declare i int local
	/declare emptySlotCount int local 0

	/for i 1 to 20

	 	/if (!${InvSlot[${i}].Item.Name.Length}) {

	 		/varset emptySlotCount ${Math.Calc[${emptySlotCount} + 1]}

	 		| ---------- Debug info
			/if (${CheckDeadDebug}) {
				/docommand /${CheckDeadDebugChannel} ${Me.CleanName}: {i} = ${i}
				/docommand /${CheckDeadDebugChannel} ${Me.CleanName}: {emptySlotCount} = ${emptySlotCount}
				/docommand /${CheckDeadDebugChannel} ${Me.CleanName}: {InvSlot[${i}].Item} = ${InvSlot[${i}].Item}
			}

	 	}

	/next i

	/if (${emptySlotCount} > 5) {

		/bc I am dead
		/return DEAD

	} else {

		/return NOTDEAD

	}

/return

| --------------------------------------------------------------------------------------------------------|
| -- Delete's an item from inventory - MAKE DAMN SURE YOU ACTUALLY WANT TO DO THIS						 -|
| --------------------------------------------------------------------------------------------------------|

#EVENT removeItemFromInventory "<#*#> Delete thingamajig: #1#"
SUB EVENT_removeItemFromInventory(line, itemToDelete)

	| ---------- Call the item to cursor
	/call itemToCursor "${itemToDelete}"

	| ---------- Wait for something to hit cursor
	/delay 5s ${Cursor.ID}

	| ---------- If the item name on the cursor matches the item, destroy it.  Else autoinventory
	/if (${itemToDelete.Equal[${Cursor.Name}]}) {
		/destroy
	} else {
		/autoinventory
	}

	/if (${FindItemCount[=${itemToDelete}]}) /bc [+y+] Still have one or more ${itemToDelete} on me!

	| ---------- Cleanup
	/call closeBags
	/if (${Window[InventoryWindow].Open}) /keypress i

/return

| --------------------------------------------------------------------------------------------------------|
| -- Sets up aliases for e3x																			 -|
| --------------------------------------------------------------------------------------------------------|

SUB e3x_ExtraUtilities_Aliases
	/if (${Debug}) /echo |- e3x_ExtraUtilities_Aliases ==>

		/noparse /squelch /alias /BSG /echo Build Sewers Group
		/noparse /squelch /alias /BuffAS /bc BuffAS:
		/noparse /squelch /alias /cfb /bc [+y+] Check for buff:
		/noparse /squelch /alias /cfd /bc [+y+] Check for debuff:
		/noparse /squelch /alias /CIM /bc Check for Interesting Mobs in ${Zone}
		/noparse /squelch /alias /CTB /bc Check for Typical Buffs
		/noparse /squelch /alias /deploy /bc Deploy:
		/noparse /squelch /alias /dis /bcaa //disband
		/noparse /squelch /alias /EE /bc End Engagement!
		/noparse /squelch /alias /Engage /bc Engage Target!
		/noparse /squelch /alias /lootOff /echo Loot Off
		/noparse /squelch /alias /lootOn /echo Loot On
		/noparse /squelch /alias /PortTo /bc Port to
		/noparse /squelch /alias /PP /bc Play Puretones:
		/noparse /squelch /alias /PPStat /noparse /bct bards //if (${Me.CombatAbilityReady[Puretone Discipline]}) /bc [+y+] Puretones ready!
		/noparse /squelch /alias /Sew /bc Invite me to Sewers Expedition
		/noparse /squelch /alias /Atar /bcaa //target id ${Target.ID}
		/noparse /squelch /alias /gis /bcaa //say group is ready
		/noparse /squelch /alias /iGis /say group is ready
		/noparse /squelch /alias /iQVIC /say I wish to travel
		/noparse /squelch /alias /QVIC /bcaa //say I wish to travel
		/noparse /squelch /alias /itt /say fire water undead air earth graveyard
		/noparse /squelch /alias /tt /bcaa //say fire water undead air earth graveyard
		/noparse /squelch /alias /ChOff /bc CH Mode Disabled
		/noparse /squelch /alias /ChOn /bc CH Mode Enabled:
		/noparse /squelch /alias /CureOff /bc Cure Disable
		/noparse /squelch /alias /CureOn /bc Cure Enable:
		/noparse /squelch /alias /Ikkinz1 /say Sanctuary of Righteousness
		/noparse /squelch /alias /Ikkinz2 /say Sanctuary of the Glorified
		/noparse /squelch /alias /Ikkinz3 /say Sanctuary of the Transcendent
		/noparse /squelch /alias /iYes /notify ConfirmationDialogBox Yes_Button leftmouseup
		/noparse /squelch /alias /Yes /bca //notify ConfirmationDialogBox Yes_Button leftmouseup

	/if (${Debug}) /echo <== e3x_ExtraUtilities_Aliases -|
/return