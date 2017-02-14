|----------------------------------------------------------------|
|- GoD_Alerts.inc ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ									-|
| -- Modified by Smaht of EQTitan to work with extended (e3x) functionality
|----------------------------------------------------------------|
|- Contains various alerts.										-|
|----------------------------------------------------------------|

|----------------------------------------------------------------|
|- Planes of Power and GoD Flag Received						-|
| -- Smaht: Added GoD checks
|----------------------------------------------------------------|

#event charFlag "You receive a character flag#*#"
#event charFlag "You have gained a temporary flag#*#"
Sub event_charFlag
	/bc I have recieved a character flag!
/return

|----------------------------------------------------------------|
|- Plane of Tactics Stampe Event								-|
|----------------------------------------------------------------|

#event tactics_stampede "#*#You hear the pounding of hooves#*#"
Sub event_tactics_stampede
	/if (${Zone.ShortName.Equal[potactics]}) {
		/gsay STAMPEDE!
		/beep
	}
/return

|----------------------------------------------------------------|
|- Uqua Events													-|
|----------------------------------------------------------------|

#event Define_UquaKey "#*#The #1# must unlock the door to the next room.#*#"
SUB event_Define_UquaKey(string line, string KeyToUse)
	/rs >>^<< The ${KeyToUse} unlocks the door >>^<<
	/popup >>^<< The ${KeyToUse} unlocks the door >>^<<
/return

#event setClass_Warrior "#*#Brute force and brawn#*#"
#event setClass_Shaman "#*#Cringes at the appearance of talismans#*#"
#event setClass_Beastlord "#*#Deep gashes of feral savagery#*#"
#event setClass_Necromancer "#*#Doom of death#*#"
#event setClass_Cleric "#*#Dread of celestial spirit#*#"
#event setClass_Shadowknight "#*#Dreads the strike of dead#*#"
#event setClass_Monk "#*#Focused tranquility#*#"
#event setClass_Bard "#*#Foreboding melody#*#"
#event setClass_Paladin "#*#Holy blade#*#"
#event setClass_Rogue "#*#Ignores anything behind it#*#"
#event setClass_Enchanter "#*#Mind and body vulnerable#*#"
#event setClass_Wizard "#*#Falters when struck with the power of the elements#*#"
#event setClass_Berserker "#*#Shies from heavy blades#*#"
#event setClass_Magician "#*#Summoned elements#*#"
#event setClass_Druid "#*#The creature seems weak in the face of the power of nature#*#"
#event setClass_Ranger "#*#True shots and fast blades#*#"

SUB event_setClass_Warrior
	/popup Spawn must be killed by a >>^<< WARRIOR >>^<<
	/rs Spawn must be killed by a >>^<< WARRIOR >>^<<
/return

SUB event_setClass_Shaman
	/popup Spawn must be killed by a >>^<< SHAMAN >>^<< 
	/rs Spawn must be killed by a >>^<< SHAMAN >>^<< 
/return

SUB event_setClass_Beastlord
	/popup Spawn must be killed by a >>^<< BEASTLORD >>^<< 
	/rs Spawn must be killed by a >>^<< BEASTLORD >>^<< 
/return

SUB event_setClass_Necromancer
	/popup Spawn must be killed by a >>^<< NECROMANCER >>^<< 
	/rs Spawn must be killed by a >>^<< NECROMANCER >>^<< 
/return

SUB event_setClass_Cleric
	/popup Spawn must be killed by a >>^<< CLERIC >>^<< 
	/rs Spawn must be killed by a >>^<< CLERIC >>^<< 
/return

SUB event_setClass_Shadowknight
	/popup Spawn must be killed by a >>^<< SHADOWKNIGHT >>^<< 
	/rs Spawn must be killed by a >>^<< SHADOWKNIGHT >>^<< 
/return

SUB event_setClass_Monk
	/popup Spawn must be killed by a >>^<< MONK >>^<< 
	/rs Spawn must be killed by a >>^<< MONK >>^<< 
/return

SUB event_setClass_Bard
	/popup Spawn must be killed by a >>^<< BARD >>^<< 
	/rs Spawn must be killed by a >>^<< BARD >>^<< 
/return

SUB event_setClass_Paladin
	/popup Spawn must be killed by a >>^<< PALADIN >>^<< 
	/rs Spawn must be killed by a >>^<< PALADIN >>^<< 
/return

SUB event_setClass_Rogue
	/popup Spawn must be killed by a >>^<< ROGUE >>^<< 
	/rs Spawn must be killed by a >>^<< ROGUE >>^<< 
/return

SUB event_setClass_Enchanter
	/popup Spawn must be killed by a >>^<< ENCHANTER >>^<< 
	/rs Spawn must be killed by a >>^<< ENCHANTER >>^<< 
/return

SUB event_setClass_Wizard
	/popup Spawn must be killed by a >>^<< WIZARD >>^<< 
	/rs Spawn must be killed by a >>^<< WIZARD >>^<< 
/return

SUB event_setClass_Berserker
	/popup Spawn must be killed by a >>^<< BERSERKER >>^<< 
	/rs Spawn must be killed by a >>^<< BERSERKER >>^<< 
/return

SUB event_setClass_Magician
	/popup Spawn must be killed by a >>^<< MAGICIAN >>^<< 
	/rs Spawn must be killed by a >>^<< MAGICIAN >>^<< 
/return

SUB event_setClass_Druid
	/popup Spawn must be killed by a >>^<< DRUID >>^<< 
	/rs Spawn must be killed by a >>^<< DRUID >>^<< 
/return

SUB event_setClass_Ranger
	/popup Spawn must be killed by a >>^<< RANGER >>^<< 
	/rs Spawn must be killed by a >>^<< RANGER >>^<< 
/return

#event startClass_Priests "#*#The creature cannot stand up to the power of healers#*#"
#event startClass_Hybrids "#*#The creature appears weak to the combined effort of might and magic#*#"
#event startClass_Casters "#*#The creature will perish under the strength of intelligent magic#*#"
#event startClass_Melees "#*#The creature appears weak to the combined effort of strength and cunning#*#"

SUB event_startClass_Priests
	/if (!${Defined[murauderKill]}) {
		/declare murauderKill int outer 4
	} else /if (${Defined[murauderKill]}) {
		/varset murauderKill 4
	}
	/rs Spawn must be killed by a >>^<< PRIEST >>^<< 
/return

SUB event_startClass_Hybrids
	/if (!${Defined[murauderKill]}) {
		/declare murauderKill int outer 2
	} else /if (${Defined[murauderKill]}) {
		/varset murauderKill 2
	}
	/rs Spawn must be killed by a >>^<< HYBRID >>^<< 
/return

SUB event_startClass_Casters
	/if (!${Defined[murauderKill]}) {
		/declare murauderKill int outer 1
	} else /if (${Defined[murauderKill]}) {
		/varset murauderKill 1
	}
	/rs Spawn must be killed by a >>^<< CASTER >>^<< 
/return

SUB event_startClass_Melees
	/if (!${Defined[murauderKill]}) {
		/declare murauderKill int outer 3
	} else /if (${Defined[murauderKill]}) {
		/varset murauderKill 3
	}
	/rs Spawn must be killed by a >>^<< MELEE >>^<< 
/return

#event MobSet "#*#shudders as if somehow changing.#*#"
SUB event_MobSet
	/if (${murauderKill} < 4) {
		/if (${Defined[murauderKill]}) /varcalc murauderKill ${murauderKill} + 1
	} else {
		/if (${Defined[murauderKill]}) /varset murauderKill 1
	}
	/if (${murauderKill} == 1) {
		/rs Vrex Xalkaks Marauder must be killed by a  >>^<< CASTER >>^<< 
	} else /if (${murauderKill} == 2) {
		/rs Vrex Xalkaks Marauder must be killed by a >>^<< HYBRID >>^<< 
	} else /if (${murauderKill} == 3) {
		/rs Vrex Xalkaks Marauder must be killed by a >>^<< PRIEST >>^<< 
	} else /if (${murauderKill} == 4) {
		/rs Vrex Xalkaks Marauder must be killed by a >>^<< MELEE >>^<< 
	}
/return

| ----------------------------------------------------------------|
| -- Smaht: Tipt Riddle Answer Events							 -|
| -- Answers to the Tipt riddles								 -|
| ----------------------------------------------------------------|

#event setRiddle1 "#*#Lovely and round, I shine with pale light, grown in darkness, a lady's delight.#*#"
#event setRiddle2 "#*#I scribble forms of the finest letter, and repel elements of the harshest weather. I am an arrow-aimer and a dust-breaker.#*#"
#event setRiddle3 "#*#When young, I am sweet in the sun. When middle-aged, I make you happy. When old, I am valued more than ever.#*#"
#event setRiddle4 "#*#My mouth bears whiskers but no teeth. I have scales but I do not weigh. You can bait me with delicious food but please, no drink today.#*#"
#event setRiddle5 "#*#My life is measured in hours. I serve by being devoured. Thin, I am quick, Fat I am slow. Wind is my foe.#*#"
#event setRiddle6 "#*#It floats on water, light as can be. A thousand men can't lift it free.#*#"
#event setRiddle7 "#*#I'm offered to the loved, and also to the dead. I come in many varied hues, most notably red. My pricks are known to pierce the skin, often resulting in wounds that bled.#*#"
#event setRiddle8 "#*#What can be held in the left hand, but not in the right hand.#*#"
#event setRiddle9 "#*#I'm always hungry, I must always be fed, The finger I lick will soon turn red.#*#"
#event setRiddle10 "#*#Who makes has no need of it, who buys has no use of it, who uses it can neither see nor feel it.#*#"
#event setRiddle11 "#*#I help to mature your spirits. When moistened I fulfill my purpose. Should I dry out, my task will fail and my quarry may be worthless.#*#"
#event setRiddle12 "#*#I build up castles. I tear down mountains. I make some men blind, I help others to see. What am I?#*#"
#event setRiddle13 "#*#When I am filled I can point the way, When I am empty Nothing moves me, I have two skins One without and one within.#*#"
#event setRiddle14 "#*#I drive men mad for love of me, Easily beaten, Never free.#*#"
#event setRiddle15 "#*#Searing 'cross the pitch-black skies, I scream in celebration, yet moments later, my outburst through, I am naught but imagination.#*#"

SUB event_setRiddle1
	/popup Answer is a >>^<< PEARL >>^<< (8th from the left)
	/g Answer is a >>^<< PEARL >>^<< (8th from the left)
/return

SUB event_setRiddle2
	/popup Answer is a >>^<< FEATHER >>^<< (9th from the left)
	/g Answer is a >>^<< FEATHER >>^<< (9th from the left)
/return

SUB event_setRiddle3
	/popup Answer is a >>^<< WINE >>^<< (6th from the right)
	/g Answer is a >>^<< WINE >>^<< (6th from the right)
/return

SUB event_setRiddle4
	/popup Answer is a >>^<< CATFISH >>^<< (5th from the right)
	/g Answer is a >>^<< CATFISH >>^<< (5th from the right)
/return

SUB event_setRiddle5
	/popup Answer is a >>^<< CANDLE >>^<< (4th from the right)
	/g Answer is a >>^<< CANDLE >>^<< (4th from the right)
/return

SUB event_setRiddle6
	/popup Answer is a >>^<< BUBBLE >>^<< (3rd from the right)
	/g Answer is a >>^<< BUBBLE >>^<< (3rd from the right)
/return

SUB event_setRiddle7
	/popup Answer is a >>^<< ROSE >>^<< (2nd from the right)
	/g Answer is a >>^<< ROSE >>^<< (2nd from the right)
/return

SUB event_setRiddle8
	/popup Answer is a >>^<< RIGHT HAND >>^<< (1st from the right)
	/g Answer is a >>^<< RIGHT HAND >>^<< (1st from the right)
/return

SUB event_setRiddle9
	/popup Answer is a >>^<< FIRE >>^<< (7th from the left)
	/g Answer is a >>^<< FIRE >>^<< (7th from the left)
/return

SUB event_setRiddle10
	/popup Answer is a >>^<< COFFIN >>^<< (6th from the left)
	/g Answer is a >>^<< COFFIN >>^<< (6th from the left)
/return

SUB event_setRiddle11
	/popup Answer is a >>^<< CORK >>^<< (5th from the left)
	/g Answer is a >>^<< CORK >>^<< (5th from the left)
/return

SUB event_setRiddle12
	/popup Answer is a >>^<< SAND >>^<< (4th from the left)
	/g Answer is a >>^<< SAND >>^<< (4th from the left)
/return

SUB event_setRiddle13
	/popup Answer is a >>^<< GLOVE >>^<< (3rd from the left)
	/g Answer is a >>^<< GLOVE >>^<< (3rd from the left)
/return

SUB event_setRiddle14
	/popup Answer is a >>^<< GOLD >>^<< (2nd from the left)
	/g Answer is a >>^<< GOLD >>^<< (2nd from the left)
/return

SUB event_setRiddle15
	/popup Answer is a >>^<< FIREWORK >>^<< (1st from the left)
	/g Answer is a >>^<< FIREWORK >>^<< (1st from the left)
/return

| ----------------------------------------------------------------|
| -- Smaht: Vxed Events											 -|
| -- Counter to keep track of mob kills							 -|
| ----------------------------------------------------------------|

sub vxed_Counter

	/declare e3xEventDebug bool local FALSE

	|---------------- Define counter variable if it doesn't exist
	/if (!${vxedMobCounter} > 0) /declare vxedMobCounter int outer 0

	|---------------- Debug info
	/if (${e3xEventDebug} || ${Debug}) /echo 300: vxedMobCounter set to ${vxedMobCounter}

	|---------------- You just killed something, increase counter
	/varcalc vxedMobCounter ${vxedMobCounter} + 1

	|---------------- Debug info
	/if (${e3xEventDebug} || ${Debug}) /echo 310: vxedMobCounter set to ${vxedMobCounter}

	|---------------- Ech out the count
	/echo Current mob count is: ${vxedMobCounter} (of a total of 51-54)
	/echo Remaining mob count is: ${Int[${Math.Calc[54 - ${vxedMobCounter}]}]} (of a potential max total of 54)

	|---------------- Popup for getting close
	/if ((${vxedMobCounter} == 39) || (${vxedMobCounter} == 44) || (${vxedMobCounter} == 49)) /popup ${Int[${Math.Calc[54 - ${vxedMobCounter}]}]} mobs remaining!

/return

| ----------------------------------------------------------------|
| -- Smaht: Cursecaller event									 -|
| ----------------------------------------------------------------|

#event cursed "#*#The thoughts of a cursed Trusik invade your mind#*#"
Sub event_cursed

	| ---------- Exit if in the wrong zone
	/if (!${Zone.ShortName.Equal[inktuta]}) /return
    
    | ---------- If south of the cursecaller room, run the cursecaller pattern  
 	/if (${Me.Y} < -780) {

    /echo backoff
    /target clear
    /shout CURSECALLER AFTER ME!!!!!!

    /mac Cursecaller
	}
	
 /return
 
| ----------------------------------------------------------------|
| -- Smaht: event_fulmination									 -|
| ----------------------------------------------------------------|

#event fulmination "#*#Your body is assaulted by a blast of pure magical energy#*#"
Sub event_fulmination
   
    /bc [+y+] Fulmination!
    /if (${Math.Rand[2]} == 1) {
    	/t iddyditty Antidote
    } else {
    	/t iddygitty Antidote
	}

 /return

| ----------------------------------------------------------------|
| -- Smaht: Tacvi event:  Pirik - Balance begins				 -|
| ----------------------------------------------------------------|

#event tacvi_Pirik_Start "#*#Kvxe enters a state of battle meditation#*#"
Sub event_tacvi_Pirik_Start

| ---------- For now, only these two should be in this loop
/if (!${Select[${Me.CleanName},Kwazyclddub,Kwazyclddeaver,Smadddht]}) /return
/bc [+g+] Rocking it!

	| ---------- Define groups for attacking
	/declare tacviPirikDefenseDPS string local none
	/declare tacviPirikFuryDPS string local Kwazyaxe,Kwazybludgeon,Kwazyclub,Tupper,Clickboom,Clickfoom,Tinee,Tipee,Quacked,Longview
	/declare tacviPirikRageDPS string local none
	/declare tacviPirikSpeedDPS string local Kwazyhatchet,Kwazycleaver,Kwazylance,Kwazyspear,Flopp,Clickdoom,Timee,Longvision,Whacked,Oomph
	/declare tacviMeleeList string local BRD,BER,BST,MNK,PAL,RNG,ROG,SHD
	/declare tacviPirikMeleeStartLocation string local -638, 353
	/declare tacviDebug bool outer TRUE
	/declare tacviDebugChannel string outer mqlog

	| ---------- Define the % at which to attack to as 3% higher than Pirikis current HPs
	/target ${Spawn[npc Zun`Muram Kvxe Pirik]}
	/delay 1s ${Target.ID}
	/declare tacviPirikDPSPct int local ${Int[${Math.Calc[${Target.PctHPs} + 3]}]}

	| ---------- Debug info
	/if (${tacviDebug}) {
		/docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_100: {Target.CleanName} = ${Target.CleanName} (Should be Pirik)
		/docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_101: {Target.ID} = ${Target.ID}
		/docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_102: {tacviPirikDPSPct} = ${tacviPirikDPSPct}
	}

	| ---------- Call for DPS on a target if you are on the appropriate list. 
	/if (${Select[${Me.CleanName},${tacviPirikSpeedDPS}]}) {

		| ---------- Debug info
		/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_200: Calling for DPS on "Balance of Speed"
		
		/call tacvi_DPS_Target "Balance of Speed" ${tacviPirikDPSPct}

	} else /if (${Select[${Me.CleanName},${tacviPirikFuryDPS}]}) {

		| ---------- Debug info
		/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_201: Calling for DPS on "Balance of Fury"

		/call tacvi_DPS_Target "Balance of Fury" ${tacviPirikDPSPct}

	} else /if (${Select[${Me.CleanName},${tacviPirikRageDPS}]}) {

		| ---------- Debug info
		/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_202: Calling for DPS on "Balance of Rage"

		/call tacvi_DPS_Target "Balance of Rage" ${tacviPirikDPSPct}

	} else /if (${Select[${Me.CleanName},${tacviPirikDefenseDPS}]}) {

		| ---------- Debug info
		/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_203: Calling for DPS on "Balance of Defense"

		/call tacvi_DPS_Target "Balance of Defense" ${tacviPirikDPSPct}
	}

 	| ---------- If you are melee return to behind the target
	/if (${Select[${Me.Class.ShortName},${tacviMeleeList}]}) {

		| ---------- Debug info
		/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_300: I am melee!  Moving to starting location (${tacviPirikMeleeStartLocation})

		/moveto loc ${tacviPirikMeleeStartLocation}

		| ---------- Wait until you are no longer moving, then re-target the ${deployRaidTarget} and face them
		:stillMoving
		/if (${Me.Moving}) {
			/delay 5
			/goto :stillMoving
		}
	} else {

		| ---------- Debug info
		/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_301: I am not a melee!  Not moving anywhere

	}

	| ---------- Debug info
	/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: event_tacvi_Pirik_Start_400: Ready to re-engage Pirik

	| ---------- Cleanup
	/if (${Defined[tacviDebug]}) /deletevar tacviDebug
	/if (${Defined[tacviDebugChannel]}) /deletevar tacviDebugChannel

/return

| ----------------------------------------------------------------|
| -- Smaht: Tacvi event: Pirik - Balance ends					 -|
| ----------------------------------------------------------------|

#event tacvi_Pirik_End "#*#Kvxe's body trembles as he fails to gather power from the balanced scales#*#"
#event tacvi_Pirik_End "#*#Your failure to balance the scales has added to Kvxe's already impressive skills#*#"
Sub event_tacvi_Pirik_End	

/return PIRIK_STOP

| ----------------------------------------------------------------|
| -- Smaht: Tacvi event: Pirik - DPS to Balance					 -|
| ----------------------------------------------------------------|

Sub tacvi_DPS_Target(string balanceTarget, int balancePct)

	| ---------- Get your target
	/target ${Spawn[npc ${balanceTarget}]}
	/delay 2s ${Target.ID}

	| ---------- Debug info
	/if (${tacviDebug}) {
		/docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_100: {balanceTarget} = ${balanceTarget}
		/docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_101: {balancePct} = ${balancePct}
		/docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_102: {Spawn[npc ${balanceTarget}]} = ${Spawn[npc ${balanceTarget}]}
		/docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_103: {Spawn[npc ${balanceTarget}].ID} = ${Spawn[npc ${balanceTarget}].ID}
		/docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_104: {Target.ID} = ${Target.ID} (Should match the Spawn.ID above)
	}

	| ---------- Engage on the target
	/call event_Assist "NULL" "Smaht" ${Target.ID}

	| ---------- Begin looping.  Exit if the target drops below the target PCT, or if the balance portion of the event ends
	:loop

	| ---------- Debug info
	/if (${tacviDebug}) /docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_200: {Macro.Return} = ${Macro.Return}

	/doevents event_tacvi_Pirik_End

	| ---------- Debug info
	/if (${tacviDebug}) {
		/docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_300: Results of event_tacvi_Pirik_End are "${Macro.Return}"
		/docommand /${tacviDebugChannel} ${Me.CleanName}: tacvi_DPS_Target_301: {Target.PctHPs} = ${Target.PctHPs}
	}

	| ---------- Stop attacking and return if {Target.PctHPs} <= {balancePct}
	/if ((${Target.PctHPs} <= ${balancePct}) || ${Macro.Return.Equal[PIRIK_STOP]}) {
		/call Event_BackOff "NULL" "${Me.CleanName}"
		/return
	}
	/delay 5
	/goto :loop

/return
