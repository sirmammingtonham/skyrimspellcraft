#pragma once
#include "skse64/PapyrusNativeFunctions.h"

#include "skse64/GameData.h"
#include "skse64/GameObjects.h"
#include "skse64/GameReferences.h"
#include "skse64/GameRTTI.h"

// read GameObjects.h if you wanna see how i found the variables to modify for these functions
namespace SpellCraft
{
	// Warning, these functions are not thoroughly tested
	// Remember to practice proper safety when using them

	bool SpellAddEffect(StaticFunctionTag* base, SpellItem* akSpell, EffectSetting* akEffect, float mag, UInt32 dur, UInt32 area, float cost);

	bool SetSpellNthMagicEffect(StaticFunctionTag* base, SpellItem* akSpell, EffectSetting* akEffect, UInt32 index);
	bool SetSpellNthMagicEffectMag(StaticFunctionTag* base, SpellItem* akSpell, float mag, UInt32 index);
	bool SetSpellNthMagicEffectDur(StaticFunctionTag* base, SpellItem* akSpell, UInt32 dur, UInt32 index);
	bool SetSpellNthMagicEffectArea(StaticFunctionTag* base, SpellItem* akSpell, UInt32 area, UInt32 index);
	bool SetSpellNthMagicEffectCost(StaticFunctionTag* base, SpellItem* akSpell, float cost, UInt32 index);

	float GetSpellNthMagicEffectCost(StaticFunctionTag* base, SpellItem* akSpell, UInt32 index);
	
	bool SetSpellType(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akType);
	bool SetSpellCastType(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akType);
	bool SetSpellCastTime(StaticFunctionTag* base, SpellItem* akSpell, float akTime);
	bool SetSpellDelivery(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akType);
	bool SetSpellPerk(StaticFunctionTag* base, SpellItem* akSpell, BGSPerk* akPerk);
	bool SetSpellCost(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akCost);
	bool SetSpellName(StaticFunctionTag* base, SpellItem* akSpell, BSFixedString name);

	UInt32 GetSpellType(StaticFunctionTag* base, SpellItem* akSpell);
	UInt32 GetSpellCastType(StaticFunctionTag* base, SpellItem* akSpell);
	UInt32 GetSpellCastTime(StaticFunctionTag* base, SpellItem* akSpell);
	BGSPerk* GetSpellPerk(StaticFunctionTag* base, SpellItem* akSpell);

	bool CopySpell(StaticFunctionTag* base, SpellItem* spellA, SpellItem* spellB);
	bool RemoveAllActorSpells(StaticFunctionTag* base, Actor* akActor, bool removeBase);
	VMResultArray<SpellItem*> GetAllActorSpells(StaticFunctionTag* base, Actor* akActor, BSFixedString akSchool, UInt32 type);
	VMResultArray<SpellItem*> FilterSpellArray(StaticFunctionTag* base, VMArray<SpellItem*> spells, VMArray<BSFixedString> tags);

	bool CombineSpells(StaticFunctionTag* base, SpellItem* base_spell, VMArray<SpellItem*> spells, BSFixedString name);

	bool RegisterFuncs(VMClassRegistry* registry);
}
