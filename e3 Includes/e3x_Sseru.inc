| --------------------------------------------------------------------------------------|
| -- e3x_Sseru v1.0	( extended functionality designed to work with e3.mac )	           -|
| -- Written by Smaht							                                       -|
| --------------------------------------------------------------------------------------|
| -- Contains extended and miscellaneous EVENTs and functions for e3.mac.	           -|
| -- Specific to pulling Praesertum's in Sseru.	                                       -|
| --------------------------------------------------------------------------------------|

| --------------------------------------------------------------------------------------|
| -- Pull Praesertum Matpa                                                             -|
| --------------------------------------------------------------------------------------|

#event pullMatpa "#*#Pull Matpa"
SUB EVENT_pullMatpa

	| ----------  If you are not Oomph or are not in Sseru, exit
	/if (!${Me.CleanName.Equal[Oomph]} || !${Zone.ShortName.Equal[sseru]}) /return

	| ---------- Declare some variables
	/declare sseruDebug bool local TRUE
	/declare sseruDebugChannel string local bc
	/declare pullTarget string local Praesertum Matpa
	|/declare pullTarget string local Praesertum Vantorus
	/declare pullTargetID ${Spawn[npc ${pullTarget}].ID}

	| ---------- If Praesertum Matpa is not up, exit
	/if (!${pullTargetID}) {
		/if (${sseruDebug}) /docommand /${sseruDebugChannel} No target named ${pullTarget}! - Exiting ...
		/return NO_SPAWN
	}

	| ---------- Get to the starting location
	/moveto loc -579 -664
	:SseruSetup
	/delay 5
	/if (${Me.Moving}) /goto :SseruSetup

	| ---------- Ensure you have a pet up
	:SseruNoPet
	/if (!${Me.Pet.ID}) {
		/call summonPet
		/delay 1s
		/if (${sseruDebug}) /docommand /${sseruDebugChannel} No Pet! - Summoning ....
		/goto :SseruNoPet
	}

	| ---------- Move to pre-pull location
	/moveto loc -716 -664
	:SseruPrePullOne
	/delay 5
	/if (${Me.Moving}) /goto :SseruPrePullOne

	/moveto loc -797 -675
	:SseruPrePullFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruPrePullFinal

	| ---------- Target the pullTarget
	/target id ${pullTargetID}
	/delay 2s ${Target.ID}==${pullTargetID}
	/if (${sseruDebug}) /docommand /${sseruDebugChannel} Distance to ${pullTarget} is ${Target.Distance}

	| ---------- Ensure your pet is within X yards
	:SseruPetCheck
	/delay 5
	/if (${Me.Pet.Distance} > 20) /goto :SseruPetCheck

	| ---------- Wait until Matpa is in the correct position
	|pull when within 44 units for no adds
	| 51.92 was okay ...
	| 53.07 was okay ...
	| 52.47 was okay ...

	| ---------- Send in the pet
	/pet hold off
	/pet attack

	| ---------- Move to post-pull location
	/moveto loc -716 -664
	:SseruPostPullOne
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullOne

	/moveto loc -579 -664
	:SseruPostPullTwo
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullTwo

	/moveto loc -565 -695
	:SseruPostPullFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullFinal

	| ---------- Wait for your pet to hit the target
	:SseruPetEngage
	/delay 5
	/if (${Me.TargetOfTarget.ID} != ${Me.Pet.ID}) /goto :SseruPetEngage

	| ---------- Back off pet and return to normal mode
	/pet hold on
	/pet back off
	/delay 3
	/pet back off
	/delay 3

	| ---------- Nuke as pullTarget is coming in
	/face fast
	:SseruNuke
	/delay 2
	/if (${Me.SpellReady[Trushar's Frost]}) {
		/casting "Trushar's Frost|gem3" -maxtries|2
		/goto :SSeruNuke
	}

	| ---------- Wait for pullTarget to close the distance
	:SseruTargetWait
	/delay 3
	/if (${Target.Distance} > 30) /goto :SseruTargetWait

	| ---------- Move to final location
	/moveto loc -565 -643
	:SseruFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruFinal
	/face fast

/return

| --------------------------------------------------------------------------------------|
| -- Pull Praesertum Rhugol                                                            -|
| --------------------------------------------------------------------------------------|

#event pullRhugol "#*#Pull Rhugol"
SUB EVENT_pullRhugol

	| ----------  If you are not Oomph or are not in Sseru, exit
	/if (!${Me.CleanName.Equal[Oomph]} || !${Zone.ShortName.Equal[sseru]}) /return

	| ---------- Declare some variables
	/declare sseruDebug bool local TRUE
	/declare sseruDebugChannel string local bc
	/declare pullTarget string local Praesertum Rhugol
	/declare pullTargetID ${Spawn[npc ${pullTarget}].ID}

	| ---------- If Praesertum Rhugol is not up, exit
	/if (!${pullTargetID}) {
		/if (${sseruDebug}) /docommand /${sseruDebugChannel} No target named ${pullTarget}! - Exiting ...
		/return NO_SPAWN
	}

	| ---------- Get to the starting location
	/moveto loc 0 -395
	:SseruSetup
	/delay 5
	/if (${Me.Moving}) /goto :SseruSetup

	| ---------- Ensure you have a pet up
	:SseruNoPet
	/if (!${Me.Pet.ID}) {
		/call summonPet
		/delay 1s
		/if (${sseruDebug}) /docommand /${sseruDebugChannel} No Pet! - Summoning ....
		/goto :SseruNoPet
	}

	| ---------- Move to pre-pull location
	/moveto loc 0 -510
	:SseruPrePullOne
	/delay 5
	/if (${Me.Moving}) /goto :SseruPrePullOne

	/moveto loc 12 -600
	:SseruPrePullFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruPrePullFinal

	| ---------- Target the pullTarget
	/target id ${pullTargetID}
	/delay 2s ${Target.ID}==${pullTargetID}
	/if (${sseruDebug}) /docommand /${sseruDebugChannel} Distance to ${pullTarget} is ${Target.Distance}

	| ---------- Ensure your pet is within X yards
	:SseruPetCheck
	/delay 5
	/if (${Me.Pet.Distance} > 20) /goto :SseruPetCheck

	| ---------- Wait until Rhugol is in the correct position
	| 107 units
	| 96.86 was okay ...
	| 99.39 was okay ...
	| 103.90 was okay ...

	| ---------- Send in the pet
	/pet hold off
	/pet attack

	| ---------- Move to post-pull location
	/moveto loc 0 -510
	:SseruPostPullOne
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullOne

	/moveto loc 0 -395
	:SseruPostPullTwo
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullTwo

	/moveto loc 32 -360
	:SseruPostPullFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullFinal

	| ---------- Wait for your pet to hit the target
	:SseruPetEngage
	/delay 5
	/if (${Me.TargetOfTarget.ID} != ${Me.Pet.ID}) /goto :SseruPetEngage

	| ---------- Back off pet and return to normal mode
	/pet hold on
	/pet back off
	/delay 3
	/pet back off
	/delay 3

	| ---------- Nuke as pullTarget is coming in
	/face fast
	:SseruNuke
	/delay 2
	/if (${Me.SpellReady[Trushar's Frost]}) {
		/casting "Trushar's Frost|gem3" -maxtries|2
		/goto :SSeruNuke
	}

	| ---------- Wait for pullTarget to close the distance
	:SseruTargetWait
	/delay 3
	/if (${Target.Distance} > 30) /goto :SseruTargetWait

	| ---------- Move to final location
	/moveto loc -20 -364
	:SseruFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruFinal
	/face fast

/return

| --------------------------------------------------------------------------------------|
| -- Pull Praesertum Bikun                                                             -|
| --------------------------------------------------------------------------------------|

#event pullBikun "#*#Pull Bikun"
SUB EVENT_pullBikun

	| ----------  If you are not Oomph or are not in Sseru, exit
	/if (!${Me.CleanName.Equal[Oomph]} || !${Zone.ShortName.Equal[sseru]}) /return

	| ---------- Declare some variables
	/declare sseruDebug bool local TRUE
	/declare sseruDebugChannel string local bc
	/declare pullTarget string local Praesertum Bikun
	/declare pullTargetID ${Spawn[npc ${pullTarget}].ID}

	| ---------- If Praesertum Bikun is not up, exit
	/if (!${pullTargetID}) {
		/if (${sseruDebug}) /docommand /${sseruDebugChannel} No target named ${pullTarget}! - Exiting ...
		/return NO_SPAWN
	}

	| ---------- Get to the starting location
	/moveto loc 0 -77
	:SseruSetup
	/delay 5
	/if (${Me.Moving}) /goto :SseruSetup

	| ---------- Ensure you have a pet up
	:SseruNoPet
	/if (!${Me.Pet.ID}) {
		/call summonPet
		/delay 1s
		/if (${sseruDebug}) /docommand /${sseruDebugChannel} No Pet! - Summoning ....
		/goto :SseruNoPet
	}

	| ---------- Move to pre-pull location
	/moveto loc 0 40
	:SseruPrePullOne
	/delay 5
	/if (${Me.Moving}) /goto :SseruPrePullOne

	/moveto loc -10 125
	:SseruPrePullFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruPrePullFinal

	| ---------- Target the pullTarget
	/target id ${pullTargetID}
	/delay 2s ${Target.ID}==${pullTargetID}
	/if (${sseruDebug}) /docommand /${sseruDebugChannel} Distance to ${pullTarget} is ${Target.Distance}

	| ---------- Ensure your pet is within X yards
	:SseruPetCheck
	/delay 5
	/if (${Me.Pet.Distance} > 20) /goto :SseruPetCheck

	| ---------- Wait until Bikun is in the correct position
	| XYZ units
	| 115units was okay
	| 118 was okay

	| ---------- Send in the pet
	/pet hold off
	/pet attack

	| ---------- Move to post-pull location
	/moveto loc 0 40
	:SseruPostPullOne
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullOne

	/moveto loc 0 -77
	:SseruPostPullTwo
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullTwo

	/moveto loc -35 -100
	:SseruPostPullFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruPostPullFinal

	| ---------- Wait for your pet to hit the target
	:SseruPetEngage
	/delay 5
	/if (${Me.TargetOfTarget.ID} != ${Me.Pet.ID}) /goto :SseruPetEngage

	| ---------- Back off pet and return to normal mode
	/pet hold on
	/pet back off
	/delay 3
	/pet back off
	/delay 3

	| ---------- Nuke as pullTarget is coming in
	/face fast
	:SseruNuke
	/delay 2
	/if (${Me.SpellReady[Trushar's Frost]}) {
		/casting "Trushar's Frost|gem3" -maxtries|2
		/goto :SSeruNuke
	}

	| ---------- Wait for pullTarget to close the distance
	:SseruTargetWait
	/delay 3
	/if (${Target.Distance} > 30) /goto :SseruTargetWait

	| ---------- Move to final location
	/moveto loc 23 -102
	:SseruFinal
	/delay 5
	/if (${Me.Moving}) /goto :SseruFinal
	/face fast

/return

| --------------------------------------------------------------------------------------|
| -- Move from Matpa to Rhugol                                                         -|
| --------------------------------------------------------------------------------------|

#event moveToRhugol "#*#Come With Me To Rhugol"
SUB EVENT_moveToRhugol

	| ---------- Move to starting location
	/moveto loc -526 -661
	:Start
	/delay 5
	/if (${Me.Moving}) /goto :Start

	| ---------- Get everyone following
	/bc Follow
	/delay 2s

	| ---------- Move to appropriate waypoints
	/moveto loc -384 -513
	:WPOne
	/delay 5
	/if (${Me.Moving}) /goto :WPOne

	/moveto loc -166 -441
	:WPTwo
	/delay 5
	/if (${Me.Moving}) /goto :WPTwo

	/moveto loc -162 -283
	:WPThree
	/delay 5
	/if (${Me.Moving}) /goto :WPThree

	/moveto loc 10 -308
	:WPFour
	/delay 5
	/if (${Me.Moving}) /goto :WPFour

	/moveto loc -1 -363
	:WPFive
	/delay 5
	/if (${Me.Moving}) /goto :WPFive

	/moveto loc -84 -379
	:WPSix
	/delay 5
	/if (${Me.Moving}) /goto :WPSix

	/moveto loc -107 -433
	:WPSeven
	/delay 5
	/if (${Me.Moving}) /goto :WPSeven

	/moveto loc -11 -430
	:WPEight
	/delay 5
	/if (${Me.Moving}) /goto :WPEight

	/moveto loc -1 -358
	:WPNine
	/delay 5
	/if (${Me.Moving}) /goto :WPNine

	| ---------- Clean up trash
	/call trashCleanup

	| ---------- Stop following
	/bc Stop
	/delay 1s

/return

| --------------------------------------------------------------------------------------|
| -- Move from Rhugol to Bikun                                                         -|
| --------------------------------------------------------------------------------------|

#event moveToBikun "#*#Come With Me To Bikun"
SUB EVENT_moveToBikun

	/moveto loc 0 -331
	:Start
	/delay 5
	/if (${Me.Moving}) /goto :Start

	| ---------- Get everyone following
	/bc Follow
	/delay 2s

	| ---------- Move to appropriate waypoints
	/moveto loc -62 -260
	:WPOne
	/delay 5
	/if (${Me.Moving}) /goto :WPOne

	/moveto loc -62 -189
	:WPTwo
	/delay 5
	/if (${Me.Moving}) /goto :WPTwo

	/moveto loc 0 -155
	:WPThree
	/delay 5
	/if (${Me.Moving}) /goto :WPThree

	/moveto loc 0 -100
	:WPFour
	/delay 5
	/if (${Me.Moving}) /goto :WPFour

	| ---------- Clean up trash
|	/call trashCleanup

	| ---------- Stop following
	/bc Stop
	/delay 1s

/return

| --------------------------------------------------------------------------------------|
| -- Move from Bikun to Matpa                                                          -|
| --------------------------------------------------------------------------------------|

#event moveToMatpa "#*#Come With Me To Matpa"
SUB EVENT_moveToMatpa

	| ---------- Move to starting location
	/moveto loc 0 -150
	:Start
	/delay 5
	/if (${Me.Moving}) /goto :Start

	| ---------- Get everyone following
	/bc Follow
	/delay 2s

	| ---------- Move to appropriate waypoints
	/moveto loc -144 -266
	:WPOne
	/delay 5
	/if (${Me.Moving}) /goto :WPOne

	/moveto loc -221 -485
	:WPTwo
	/delay 5
	/if (${Me.Moving}) /goto :WPTwo

	/moveto loc -372 -502
	:WPThree
	/delay 5
	/if (${Me.Moving}) /goto :WPThree

	/moveto loc -525 -665
	:WPFour
	/delay 5
	/if (${Me.Moving}) /goto :WPFour

	| ---------- Clean up trash
|	/call trashCleanup

	| ---------- Stop following
	/bc Stop
	/delay 1s

/return


| --------------------------------------------------------------------------------------|
| -- Main event                                                                        -|
| --------------------------------------------------------------------------------------|

| mem trushar's frost in gem3
| mem pet spell and pet buffs
| buff others (smrt/virtue everyone/kazad everyone/sd everyone/sv pet/haste,buff,run speed)

SUB trashCleanup

	/declare sseruDebug bool local TRUE
	/declare sseruDebugChannel string local bc
	/declare mobRadius int local 75
	/declare mobsInRange int local ${SpawnCount[npc radius ${mobRadius}]}

	/if (${sseruDebug}) /docommand /${sseruDebugChannel} mobsInRange = ${mobsInRange}

	| ---------- If mobsInRange is <= 1, then exit function.  Nothing to do
	/if (${mobsInRange} <= 1) /return

	/declare i int local
	/declare targetToCheck int local

	| ---------- Loop through all mobsInRange looking for ones in combat (with a PC targeted)
	/for i 1 to ${mobsInRange}

		| ---------- Set targetToCheck to the ID of the mob to check on this loop
		/varset targetToCheck ${NearestSpawn[${i}, npc radius ${mobRadius}].ID}

		| ------------ Exit if targetToCheck would be of undesireable type
		/if (${Select[${Spawn[${targetToCheck}].Type},PC,Untargetable,Mount,Corpse,Chest,Trigger,Trap,Timer,Item]}) {
			/if (${sseruDebug}) /docommand /${sseruDebugChannel} Mob Type is ${Spawn[${targetToCheck}].Type} - skipping
			/next i
		}

		| ---------- Get it targeted and wait for HoTT to populate
		/target id ${targetToCheck}
		/delay 1s ${Target.ID}==${targetToCheck}
		/delay 5

		| ---------- Debug Info
		/if (${sseruDebug}) {
			/docommand /${sseruDebugChannel} {i} = ${i}
			/docommand /${sseruDebugChannel} {targetToCheck} = ${targetToCheck}
			/docommand /${sseruDebugChannel} {Me.TargetOfTarget.Type.Equal[PC]} = ${Me.TargetOfTarget.Type.Equal[PC]}
			/docommand /${sseruDebugChannel} {Me.TargetOfTarget.Name.Find[lictor]} = ${Me.TargetOfTarget.Name.Find[lictor]}
		}

		| ---------- If it's got a target that is a PC, and it's NOT a lichtor, call for assist, and kill it, continuing only when it's dead
		/if (${Me.TargetOfTarget.Type.Equal[PC]} && !${Me.TargetOfTarget.Name.Find[lictor]}) {

			| ---------- Debug Info
			/if (${sseruDebug}) {
				/docommand /${sseruDebugChannel} Attacking target with ID = ${targetToCheck}
			}

			| ---------- Call for DPS on spawn and prepare to attack yourself
			/bc Assist on ${targetToCheck}
			/stick id ${targetToCheck}

			| ---------- Begin attacking, when you loose your target it's dead
			/attack on
			/pet attack
			:SseruContinueAttacking
			/delay 5
			/if (${Target.ID}==${targetToCheck}) /goto :SseruContinueAttacking
		}

	/next i
	

/return