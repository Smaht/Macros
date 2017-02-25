|----------------------------------------------------------------------------|
|- e3_Classes_Magician.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Magician specific functions related to,							-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|



#event petWeapons "#1# tells you, 'Pet Weapons'"
#event petWeapons "<#1#> Pet Weapons"
#event petWeapons "#1# tells you, 'Pet Toys'"
#event petWeapons "<#1#> Pet Toys"
sub event_petWeapons(string line, string ChatSender)
/if (${Debug} || ${Debug_MAG}) /echo |- EVENT_petWeapons ==>

	/if (${petItems.Size}) {
	
		| -Verify the event
		/call verifyEvent "${ChatSender}" "${line}"
		/if (${Defined[ChatSender]}) /varset ChatSender ${Macro.Return.Arg[1,_]}
		|    |-----------Valid User----------|
		/if (!${Bool[${Macro.Return.Arg[2,_]}]}) {

			/call IniToVar "${genSettings_Ini},Buff Requests,Allow Buff Requests (On/Off)" allow_BuffRequests bool local
			
			| If the user is not valid, check allow_BuffRequests
			/if (${Macro.Return}) {
		
				| If allow_BuffRequests=TRUE, check restricked PCs
				/if (!${Select[${ChatSender},${Ini[${Macro_Ini},Buff Requests,Restricted PCs]}]}) /goto :allowAccess
			}
		} else {
		
			:allowAccess
			
			/if (!${SpawnCount[pc ${ChatSender}]}) {
				/if (${Debug} || ${Debug_MAG}) /echo |- EVENT_petWeapons -| [${ChatSender}] is not in ${Zone}.
			} else {
				
				/if (${Spawn[pc ${ChatSender}].Distance} > 200) {
					/tell ${ChatSender} You're too far away.
				} else {
				
					/if (!${Spawn[pc ${ChatSender}].LineOfSight}) {
						/tell ${ChatSender} I can't see you.
					} else {

						/if (!${Spawn[pet ${Spawn[pc ${ChatSender}].Pet}].ID}) {
							/tell ${ChatSender} You don't have a pet, jackass! 
						} else {
						
							/if (${ChatSender.NotEqual[${Me}]}) /tell ${ChatSender} Incoming pet weapons.  Please stay still for a few moments.
							
							| Declare local variables
							/declare i int local
							/declare x int local
							/declare e int local
							/declare retryTimer timer local
							/declare miscTimer timer local
							/declare summonedItem string local
							
							| Target the pet, and move to it
							/call TrueTarget ${Spawn[pet ${Spawn[pc ${ChatSender}].Pet}].ID}
							/call MoveTo -SpawnID|${Spawn[pet ${Spawn[pc ${ChatSender}].Pet}].ID}
							
							/for i 1 to ${petItems.Size}
							
							/if (${Target.Distance} < 20) {
								/call e3_Cast 0 "${petItems[${i}]}/GiveUpTimer|${If[${Me.Gem[${petItems[${i}].Arg[1,/]}]},50,150]}"
								/delay 20 ${Cursor.ID}

								/if (${Cursor.ID}) {
									
									/varset summonedItem ${Cursor.Name}
									/varset retryTimer 50
									
									:OpenTrade_Loop
									
									/click left target
									/delay 10 ${Window[GiveWnd].Open}
									
									/if (!${Window[GiveWnd].Open}) {
										/if (${retryTimer}) {
											/goto :OpenTrade_Loop
										} else {
											/echo Failed to open trade with ${Target.CleanName}.
										}
									} else {
										
										/varset retryTimer 30
										:WaitAccept_Loop
										
										/notify GiveWnd GVW_Give_Button LeftMouseUp
										/delay 5 !${Window[GiveWnd].Open}
										
										/if (${Window[GiveWnd].Open}) {
											/if (${retryTimer}) {
												/goto :WaitAccept_Loop
											} else {
												/echo Failed to open trade with ${Target.CleanName}.
											}
										}
									}
								}
								
								/if (${FindItemCount[=${summonedItem}]}) {
									
									/if (${Debug} || ${Debug_MAG}) /echo |- EVENT_petWeapons -| Dropped [${summonedItem}] in my inventory.
								
									| Open InventoryWindow.
									/varset miscTimer 3s
									
									:openInventory
									/if (!${Window[InventoryWindow].Open}) /nomodkey /keypress i
									/delay 5 ${Window[InventoryWindow].Open}
									/if (!${Window[InventoryWindow].Open} && ${miscTimer}) /goto :openInventory
						
									| Search packslots.
									/varset e 0
									/for x 1 to 8  
									
										| If the item in the packslot is the summoned item, pick it up.
										/if (!${Me.Inventory[pack${x}].Container} && ${summonedItem.Equal[${Me.Inventory[pack${x}]}]}) {
											/if (${Debug} || ${Debug_MAG}) /echo |- EVENT_petWeapons -| Picking up [${summonedItem}] form pack${x}
											/itemnotify pack${x} leftmouseup
										} else {
										
											| If the item in the packslot is a container, search it.
											/for e 1 to ${Me.Inventory[pack${x}].Container}
												/if (${summonedItem.Equal[${Me.Inventory[pack${x}].Item[${e}]}]}) {
													/if (!${Window[pack${x}].Open}) /itemnotify pack${x} rightmouseup
													/delay 1s ${Window[pack${x}].Open}
													/if (${Debug} || ${Debug_MAG}) /echo |- EVENT_petWeapons -| Picking up [${summonedItem}] form pack${x} slot${e}
													/itemnotify in pack${x} ${e} leftmouseup
												}
											/next e
										}
									/next x
									
									/if (${retryTimer}) {
										/goto :OpenTrade_Loop
									} else {
										/if (${Cursor.Name.Equal[${summonedItem}]}) /destroy 
									}
								}
							}
							
							/next i
						}
					}
				}
			}
		}
	}

/if (${Debug} || ${Debug_MAG}) /echo <== EVENT_petWeapons -|
/return



#event distributePetWeapons "#1# tells you, 'Hand out Pet Weapons'"
#event distributePetWeapons "<#1#> Hand out Pet Weapons"
#event distributePetWeapons "#1# tells you, 'Distribute Pet Weapons'"
#event distributePetWeapons "<#1#> Distribute Pet Weapons"
sub event_distributePetWeapons(line, ChatSender)
/if (${Debug} || ${Debug_MAG}) /echo |- event_distributePetWeapons ==>

	/if (${petItems.Size}) {
	
		| -Verify the event
		/call verifyEvent "${ChatSender}" "${line}"
		/if (${Defined[ChatSender]}) /varset ChatSender ${Macro.Return.Arg[1,_]}
		|    |-----------Valid User----------|
		/if (!${Bool[${Macro.Return.Arg[2,_]}]}) {

			/call IniToVar "${genSettings_Ini},Buff Requests,Allow Buff Requests (On/Off)" allow_BuffRequests bool local
			
			| If the user is not valid, check allow_BuffRequests
			/if (${Macro.Return}) {
		
				| If allow_BuffRequests=TRUE, check restricked PCs
				/if (!${Select[${ChatSender},${Ini[${Macro_Ini},Buff Requests,Restricted PCs]}]}) /goto :allowAccess
			}
		} else {
		
			:allowAccess
			
			/if (!${SpawnCount[pc ${ChatSender}]}) {
				/if (${Debug} || ${Debug_MAG}) /echo |- EVENT_petWeapons -| [${ChatSender}] is not in ${Zone}.
			} else {
				
				/if (${Spawn[pc ${ChatSender}].Distance} > 200) {
					/tell ${ChatSender} You're too far away.
				} else {
				
					/if (!${Spawn[pc ${ChatSender}].LineOfSight}) {
						/tell ${ChatSender} I can't see you.
					} else {
					
					
						/if (${ChatSender.NotEqual[${Me}]}) /tell ${ChatSender} Handing out pet weapons...
					
						/declare originalLoc string local ${Me.Loc.Replace[ ,]}
						/declare startingLoc string local ${Spawn[pc ${ChatSender}].Loc.Replace[ ,]}
						
						/declare nearbyPetOwners string local
						/declare petOwner string local
						/declare petID int local
						
						/declare i int local
						
					
						| Move to the starting point
						/call MoveTo -Loc|${startingLoc}
					
						| Make a list of all the pets nearby
						/for i 1 to ${SpawnCount[radius 100]}
						
							/if (${NearestSpawn[${i}].Type.Equal[PC]} && ${Spawn[pc ${NearestSpawn[${i}]}].Pet.ID} && ${Spawn[id ${Spawn[pc ${NearestSpawn[${i}]}].Pet.ID}].LineOfSight}) /varset nearbyPetOwners ${nearbyPetOwners}${NearestSpawn[${i}]},
				
						/next i
					
						| Pass out weapons to each person on the nearbyPetOwners list
						
						/varset i 1
						
						:next_PetOwner
						
						| set petOwner and petID, then send the owner a msg.
						/varset petOwner ${nearbyPetOwners.Arg[${i},,]}
					
						/call EVENT_petWeapons "${Line}" "${petOwner}"

						/varcalc i ${i} + 1
						/if (${Bool[${nearbyPetOwners.Arg[${i},,]}]}) /goto :next_PetOwner
						
						| Move to the starting point
						/call MoveTo -Loc|${originalLoc}
					}
				}
			}
		}
	}

/if (${Debug} || ${Debug_MAG}) /echo <== event_distributePetWeapons -|
/return



SUB check_PetWeapons
/if (${Debug} || ${Debug_MAG}) /echo |- check_petWeapons ==>

	/if (${autoWeapons} && ${Zone.ID}) {
		
		/if (${BuffCheck} && !${Me.Moving} && !${combatTimer} && !${activeTimer} && !${rebuffTimer}) {
		
			/declare i int local
			
			/for i 1 to ${NetBots.Counts}
				
				/if (${NetBots[${NetBots.Client[${i}]}].Zone} && ${NetBots[${NetBots.Client[${i}]}].PetID}) {
					
					/if (${Spawn[id ${NetBots[${NetBots.Client[${i}]}].PetID}].CleanName.Equal[${Ini[${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Auto-Pet Weapons-${NetBots.Client[${i}]}]}]}) {
						/if (${Debug} || ${Debug_MAG}) /echo |- check_petWeapons -| I have already given weapons to [${Spawn[id ${NetBots[${NetBots.Client[${i}]}].PetID}].CleanName}].
					} else {

						| /docommand ${ChatToggle} Handing out pet weapons to [${Spawn[id ${NetBots[${NetBots.Client[${i}]}].PetID}].CleanName}(${NetBots.Client[${i}]})], please wait a few moments...
						/call EVENT_petWeapons PLACEHOLDER ${NetBots.Client[${i}]}
						/call WriteToIni "${MacroData_Ini},${MacroQuest.Server}-${Me.CleanName},Auto-Pet Weapons-${NetBots.Client[${i}]}" "${Spawn[id ${NetBots[${NetBots.Client[${i}]}].PetID}].CleanName}" 1
					}
				}

			/next i
		}
	}

/if (${Debug} || ${Debug_MAG}) /echo <== check_petWeapons -|
/RETURN

Sub MAG_Setup
/if (${Debug} || ${Debug_MAG}) /echo |- MAG_Setup ==>
	
	/call IniToVar "${Character_Ini},Magician,Auto-Pet Weapons (On/Off)" autoWeapons bool outer
	/call IniToArray "${Character_Ini},Magician,Summoned Pet Item#" petItems

/if (${Debug} || ${Debug_MAG}) /echo <== MAG_Setup -|
/return



Sub MAG_Background_Events

/return



SUB MAG_MacroSettings
/if (${Debug} || ${Debug_MAG}) /echo |- MAG_MacroSettings ==>

	| Adv Settings
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#1" background 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#2" basics 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#3" casting 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#4" begForBuffs 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#5" buffCheck 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#6" buy 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#7" pet 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#8" assist 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#9" class 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#10" corpseSummon 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#11" loot 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#12" sell 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#13" smoothMoves 1
	/call WriteToIni "${advSettings_Ini},Magician Included Setups,Include#14" Wait4Rez 1
	/delay 15

	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#1" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#2" "check_petHeal" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#3" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#4" "check_Debuffs" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#5" "check_Rain" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#6" "check_Nukes" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#7" "check_Buffs" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#8" "check_Pets" 1
	/call WriteToIni "${advSettings_Ini},MAG Functions,MAG Function#9" "check_PetWeapons" 1

/if (${Debug} || ${Debug_MAG}) /echo <== MAG_MacroSettings -|
/RETURN



SUB MAG_CharacterSettings
/if (${Debug} || ${Debug_MAG}) /echo |- MAG_CharacterSettings ==>

	/call WriteToIni "${Character_Ini},Magician,Auto-Pet Weapons (On/Off)"
	/call WriteToIni "${Character_Ini},Magician,Summoned Pet Item#1"

/if (${Debug} || ${Debug_MAG}) /echo <== MAG_CharacterSettings -|
/RETURN

| --------------------------------------------------------------------------------------------------------|
| -- Begin class specific routines developped by Smaht and integrated into e3x							 -|
| --------------------------------------------------------------------------------------------------------|

| --------------------------------------------------------------------------------------------------------|
| -- Magician: Summons ranged ammunition when requested													 -|
| --------------------------------------------------------------------------------------------------------|

#event summonRangedWeapons "#*# Ranged Weapons: #1#"
SUB EVENT_summonRangedWeapons(line, rangedTargetID)

	| ---------------- Set summoner.  Put your magician's name here
	/declare magSummoner string local ${classLeader}

	| ---------------- If you aren't named magSummoner, exit function
	/if (!${Me.CleanName.Equal[${magSummoner}]}) /return

	| ---------------- Otherwise get started
	/declare e3xMagicianDebug bool local FALSE

	| ---------------- Debug info
	/if (${e3xMagicianDebug} || ${Debug}) /echo 00: rangedTargetID set to ${rangedTargetID} (Name:${Spawn[${rangedTargetID}].CleanName})

	/declare summonedSpell string local
	/declare summonedItem string local

	| ---------------- Set summonedItem to 
	/if (${Select[${Spawn[${rangedTargetID}].Class.ShortName},PAL,RNG,SHD,WAR]}) {
	
		/varset summonedSpell Quiver of Marr
		/varset summonedItem Summoned: Arrow of Marr

		| ---------------- Debug info
		/if (${e3xMagicianDebug} || ${Debug}) {
			/echo 10: summonedSpell set to ${summonedSpell}
			/echo 11: summonedItem set to ${summonedItem}
		}
	} else /if (${Select[${Spawn[${rangedTargetID}].Class.ShortName},BRD,BST,MNK,ROG]}) {
	
		/varset summonedSpell Pouch of Quellious
		/varset summonedItem Summoned: Shuriken of Quellious

		| ---------------- Debug info
		/if (${e3xMagicianDebug} || ${Debug}) {
			/echo 20: summonedSpell set to ${summonedSpell}
			/echo 21: summonedItem set to ${summonedItem}
		}
	} else /if (${Select[${Spawn[${rangedTargetID}].Class.ShortName},ENC,MAG,WIZ,NEC,CLR,DRU,SHM]}) {
	
		/varset summonedSpell Summon Jewelry Bag
		/varset summonedItem Phantom Satchel

		| ---------------- Debug info
		/if (${e3xMagicianDebug} || ${Debug}) {
			/echo 20: summonedSpell set to ${summonedSpell}
			/echo 21: summonedItem set to ${summonedItem}
		}
	} else {
		| ---------------- Report an unhandled class type
		/bc [+r+] ${Spawn[${rangedTargetID}].CleanName} (ID: ${rangedTargetID}) - Unhandled Class (${Spawn[${rangedTargetID}].Class.ShortName}) for Ranged Weapon Summoning
		/return UNHANDLED_CLASS
	}

	| ---------------- Skip summoning of item if you already have some
	/if (${e3xMagicianDebug} || ${Debug}) /echo 30: Count of ${summonedItem} is ${FindItemCount[=${summonedItem}]}
	/if (${FindItemCount[=${summonedItem}]} > 0) /goto :distributeRanged

	| ---------------- Memorize and cast summonedItem
	/if (!${Me.Gem[${summonedSpell}]}) {
		/memorize "${summonedSpell}|gem8"
		/delay 8s

		/if (${e3xMagicianDebug} || ${Debug}) /echo 40: Memorized ${summonedSpell}
	}

	/casting "${summonedSpell}|gem8" -maxtries|4
	/delay 8s
	/if (${e3xMagicianDebug} || ${Debug}) /echo 50: Cast ${summonedSpell}

	| ---------- If it's a Summoned Jewelery Bag leave the item on the cursor
	/if (${summonedSpell.Equal[Summon Jewelry Bag]}) /goto :distributeRanged

	| ---------------- Deal with items on your cursor
	:cursorItems
	/if (${Cursor.ID}) {
		/autoinv
		/delay 5
		/goto :cursorItems
	}

	:distributeRanged
	
	| ---------------- Give items to the rangedTargetID
	/declare originalLoc string local ${Me.Loc.Replace[ ,]}
	/declare targetLoc string local ${Spawn[${rangedTargetID}].Loc.Replace[ ,]}

	| ---------------- Move to the target
	/call MoveTo -Loc|${targetLoc}

	| ---------------- Find the item, get it on your cursor
	:itemsToTrade
	/call itemToCursor "${summonedItem}"

	/echo ${Macro.Return}

	| ---------------- Ensure correct target and complete trade
	/call TrueTarget ${rangedTargetID}
	/click left target
	/delay 20 ${Window[TradeWnd].Open}
	/notify TradeWnd TRDW_Trade_Button leftmouseup

	| ---------------- If items of correct type remain, do another trade
	/if (${FindItemCount[=${summonedItem}]}) /goto :itemsToTrade

	| ---------------- Move to the original loc
	/call MoveTo -Loc|${originalLoc}
	
	| ---------------- Announce done, and cleanup
	/bc [+r+] ${summonedItem}s summoned and given to ${Spawn[${rangedTargetID}].CleanName}
	/call closeBags
	/if (${Window[InventoryWindow].Open}) /keypress i

	| ---------- Have the char equip the ranged items
	/bct ${Spawn[${rangedTargetID}].CleanName} //exchange "${summonedItem}" ammo

/return

| --------------------------------------------------------------------------------------------------------|
| -- Magician: Casts Call of the Hero on the provided target											 -|
| --------------------------------------------------------------------------------------------------------|

Sub coh_SingleTarget(cohTarget)

	| ---------- Memorize "Call of the Hero"
	/if (!${Me.Gem[Call of the Hero]}) {
		/memorize "Call of the Hero|${DefaultGem}"
	}

	| ---------- Disband self and target
	/squelch /bct ${Me.CleanName} //disband
	/squelch /bct ${Me.CleanName} //raiddisband
	/squelch /bct ${cohTarget} //disband
	/squelch /bct ${cohTarget} //raiddisband
	/squelch /delay 1s
	/squelch /bct ${cohTarget} //disband
	/squelch /bct ${cohTarget} //raiddisband
	/delay 1s

	| ---------- Invite target, delay until grouped
	/squelch /bct ${Me.CleanName} //invite ${cohTarget}
	/delay 10s ${cohTarget.Equal[${Group.Member[1]}]}

	| ---------- Delay until "Call of the Hero" is ready, then cast
	/delay 20s ${Me.SpellReady[Call of the Hero]}
	/call e3_Cast ${Group.Member[1].ID} "Call of the Hero"
	/delay 12s !${Me.Casting.ID}

	| ---------- Disband self
	/squelch /bct ${Me.CleanName} //disband

/Return

| --------------------------------------------------------------------------------------------------------|
| -- Magician: Casts Call of the Hero on the provided target											 -|
| --------------------------------------------------------------------------------------------------------|

#event coh_ListOfTargets "<#*#> #1# CotH the following: #2#"
Sub EVENT_coh_ListOfTargets(line, cohCaster, cohListOfTargets)

	| ---------- Exit if you are not the {cohCaster}
	/if (!${Me.Name.Equal[${cohCaster}]}) /return

 	| ---------- If the list is empty, exit
 	/if (!${cohListOfTargets.Length}) {
 		/bc [+r+] List is empty. Exiting
 		/return
 	}

 	| ---------- Determine the length of cohListOfTargets
	/declare cohListOfTargetsCount int local
	/varcalc cohListOfTargetsCount ${cohListOfTargets.Count[,]}+1

	| ---------- Echo the list
	/bc [+r+] Call of the Hero target list is as follows: ${cohListOfTargets}

	| ---------- Loop through the list summoning each list member
	/declare i int local 1
	/for i 1 to ${cohListOfTargetsCount}

		/bc (${i}/${cohListOfTargetsCount}) Casting on ${cohListOfTargets.Arg[${i},,]}
		/call coh_SingleTarget ${cohListOfTargets.Arg[${i},,]}
		
	/next i

	| ---------- Report and exit
	/bc [+r+] CotH Loop Complete.  Exiting
	/if (${FindItemCount[Pearl]} > 50) /bc [+r+] I have ${FindItemCount[Pearl]} pearls remaining!

/return

| --------------------------------------------------------------------------------------------------------|
| -- Magician: Summons a single person																	 -|
| --------------------------------------------------------------------------------------------------------|

#event CoH_OOR "<#*#> CotH"
sub EVENT_CoH_OOR

	| ---------- Exit if you aren't the classLeader
	/if (!${Me.Name.Equal[${classLeader}]}) /return

	| ---------- Debug information
	/declare e3xMagicianDebug bool local FALSE

	| ---------- Declare a few variables
	/declare cohDistance int local 200

	| ---------- Determine the count of magicians
	/declare x int local 1
	/for x 1 to ${NetBots.Counts}

		| ---------- Add to the mage list IF:  is a mage AND greater than level 54 AND is in the same zone AND is within cohDistance of the controller
 		/if (${NetBots[${NetBots.Client.Arg[${x}]}].Class.ShortName.Equal[MAG]} && (${NetBots[${NetBots.Client.Arg[${x}]}].Level} > 54) && (${NetBots[${NetBots.Client.Arg[${x}]}].Zone} == ${Zone.ID}) && (${Spawn[${NetBots.Client.Arg[${x}]}].Distance3D} < ${cohDistance})) /call BuildArray mageList ${NetBots.Client.Arg[${x}]} outer

 		| ---------- Debug info
 		/if (${e3xMagicianDebug} || ${Debug}) {
 			/echo Mag100: x = ${x}
 			/echo Mag101: NetBots Client = ${NetBots.Client.Arg[${x}]}
 		}
	
 	/next x

 	| ---------- Declare a list for each mage found
 	/declare i int local 1
 	/for i 1 to ${mageList.Size}

 		/declare mage_${i}_List string local

 		| ---------- Debug info
 		/if (${e3xMagicianDebug} || ${Debug}) /echo Mag110: Declared mage_${i}_List

 	/next i

 	| ---------- Write each list for each mage
 	/declare j int local 1
 	/declare k int local 1
 	/for j 1 to ${NetBots.Counts}

 		| ---------- If the NetBots client in question is InZone AND is !Me AND is greater than {cohDistance} away (200), write them into the summon list
 		/if (${NetBots[${NetBots.Client.Arg[${j}]}].InZone} && !${NetBots.Client.Arg[${j}].Equal[${Me.Name}]} && (${Spawn[${NetBots.Client.Arg[${j}]}].Distance3D} > ${cohDistance})) {

 			| ---------- If the mage_X_List is empty, set it to {NetBots.Client.Arg[j]}, otherwise append {NetBots.Client.Arg[j]} to the list
			/if (!${mage_${k}_List.Length}) {
				/varset mage_${k}_List ${NetBots.Client.Arg[${j}]}
			} else {
				/varset mage_${k}_List ${mage_${k}_List},${NetBots.Client.Arg[${j}]}
			}

 			| ---------- Increase {k} to move the next entry to the next list
 			/if (${k} < ${mageList.Size}) {
 				/varcalc k ${k} + 1
  			} else {
 				/varset k 1
 			}
 		}

		/next j

	| ---------- If the first list has items, call out existing magicians requesting summons, otherwise exit
	/if (${mage_1_List.Length}) {

		| ---------- Call out to existing magicians requesting summons
		/declare l int local 1
		/for l 1 to ${mageList.Size}

			/bc ${mageList[${l}]} CotH the following: ${mage_${l}_List}

		/next l

	} else {

		/bc [+r+] CotH Loop Complete.  Exiting
	}

	| ---------- Cleanup
	/deletevar mageList

/return
