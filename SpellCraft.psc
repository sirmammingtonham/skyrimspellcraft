; inspired by grimytools, adds a bunch of functions that allow more fine-grained control of spells in skyrim
Scriptname SpellCraft

; setters
bool Function SetSpellNthMagicEffect(Spell akSpell, MagicEffect akEffect, int index) Global Native 
bool Function SetSpellNthMagicEffectMagnitude(Spell akSpell, Float mag, int index) Global Native 
bool Function SetSpellNthMagicEffectDuration(Spell akSpell, Int dur, int index) Global Native 
bool Function SetSpellNthMagicEffectArea(Spell akSpell, Int area, int index) Global Native 
bool Function SetSpellNthMagicEffectCost(Spell akSpell, Float cost, int index) Global Native 
bool Function SetSpellType(Spell akSpell, Int akType ) Global Native 
bool Function SetSpellCastType(Spell akSpell, Int akType ) Global Native 
bool Function SetSpellCastTime(Spell akSpell, Float akTime ) Global Native 
bool Function SetSpellDelivery(Spell akSpell, Int akType ) Global Native 
bool Function SetSpellPerk(Spell akSpell, Perk akPerk ) Global Native
bool Function SetSpellCost(Spell akSpell, Int akCost ) Global Native 
bool Function SetSpellName(Spell akSpell, String name ) Global Native 

; getters
Float Function GetSpellNthMagicEffectCost(Spell akSpell, int index) Global Native
Int Function GetSpellType(Spell akSpell ) Global Native 
; Returns the type of the spell. IE Spell, Ability, Lesser Power, etc.
; 0 = spell, 1= disease, 2 = power, 3 = lesser power, 4 = ability
; 5 = poison, 6 = addition, 7 = voice (shout?)
Int Function GetSpellCastType(Spell akSpell ) Global Native
; Returns the cast type, IE Concentration, fire forget, etc
; 1 = fire and forget, 2 = concentration, 0 = constant effect
Int Function GetSpellCastTime(Spell akSpell) Global Native
Perk Function GetSpellPerk(Spell akSpell) Global Native
; Int Function GetSpellDelivery(Spell akSpell ) Global Native // not implemented

; fancy functions lol
; appends a new MagicEffect to a spell
bool Function SpellAddEffect(Spell akSpell, MagicEffect akEffect, Float magnitude, int duration, int area, Float cost) Global Native

; basically replaces spellA with a copy of spellB
bool Function CopySpell(Spell spellA, Spell spellB) Global Native

; deletes all spells from an actor, and the actor's base if {removeBase} is true (doesn't work as intended)
bool Function RemoveAllActorSpells(Actor akActor, bool removeBase=false) Global Native

; returns an array of all spells from an actor, including their base
; you can specify a specific magic school, or leave {akSchool} empty for all spells
; you can also specify the spell type (see above)
Spell[] Function GetAllActorSpells(Actor akActor, String akSchool="", int type=0) Global Native

; function that allows you to filter duplicates and specific spells from an array
; returns an array with no duplicately named spells, and without any spells whose names are specified in {tags}
Spell[] Function FilterSpellArray(Spell[] spells, String[] tags) Global Native

; fun function that replaces {base} spell with a new spell that combines all the effects of {spells}
bool Function CombineSpells(Spell base, Spell[] spells, String name) Global Native