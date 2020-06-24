# Skyrim Spell Craft Functions SE 🧙‍
[SKSE64](https://skse.silverlock.org/) plugin that adds more papyrus functions for the manipulation of spells in Skyrim Special Edition.
https://www.nexusmods.com/skyrimspecialedition/mods/37493

## Usage
Check [SpellCraft.psc](SpellCraft.psc) for usage.

## Example
The following papyrus snippet creates a spell effect that will add all spells from a target actor to the caster:
```
Event OnEffectStart(Actor AkTarget, Actor akCaster)
	Spell[] stolen_spells = SpellCraft.GetAllActorSpells(AkTarget, "")
	int i = 0
	while i < stolen_spells.Length
		akCaster.AddSpell(stolen_spells[i])
		i += 1
	endWhile
EndEvent
```

## Demo
Clips from my mod [All For One Mage](https://www.nexusmods.com/skyrimspecialedition/mods/37515):

![bruh](clips/combo.gif)

## Mods that require this
* [All For One Mage](https://www.nexusmods.com/skyrimspecialedition/mods/37515)

### Notes
Not an actual mod! Just a modders' resource. Not extensively tested either so submit an [issue](https://github.com/sirmammingtonham/skyrimspellcraft/issues) if you notice any bugs.

(created from zero experience in skyrim modding with the help of this [tutorial](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/wiki) from @Ryan-rsm-McKenzie)
