#include "SpellCraft.h"

template <> void UnpackValue(VMArray<SpellItem*>* dst, VMValue* src) {
	UnpackArray(dst, src, GetTypeIDFromFormTypeID(SpellItem::kTypeID, (*g_skyrimVM)->GetClassRegistry()) | VMValue::kType_Identifier);
}

namespace SpellCraft {
	// adds effect to spell
	bool SpellAddEffect(StaticFunctionTag* base, SpellItem* akSpell, EffectSetting* akEffect, float mag, UInt32 dur, UInt32 area, float cost) {
		if (!akSpell || !akEffect)
			return false;

		SpellItem::EffectItem* new_effect = new SpellItem::EffectItem();
		new_effect->mgef = akEffect;
		new_effect->magnitude = mag;
		new_effect->duration = dur;
		new_effect->area = area;
		new_effect->cost = cost;

		akSpell->effectItemList.Push(new_effect);
		return true;
	}

	// bunch of setters for magical effects
	bool SetSpellNthMagicEffect(StaticFunctionTag* base, SpellItem* akSpell, EffectSetting* akEffect, UInt32 index) {
		if (!akSpell || !akEffect || index >= akSpell->effectItemList.count)
			return false;
		akSpell->effectItemList[index]->mgef = akEffect;
		return true;
	}
	bool SetSpellNthMagicEffectMag(StaticFunctionTag* base, SpellItem* akSpell, float mag, UInt32 index) {
		if (!akSpell || index >= akSpell->effectItemList.count)
			return false;
		akSpell->effectItemList[index]->magnitude = mag;
		return true;
	}
	bool SetSpellNthMagicEffectDur(StaticFunctionTag* base, SpellItem* akSpell, UInt32 dur, UInt32 index) {
		if (!akSpell || index >= akSpell->effectItemList.count)
			return false;
		akSpell->effectItemList[index]->duration = dur;
		return true;
	}
	bool SetSpellNthMagicEffectArea(StaticFunctionTag* base, SpellItem* akSpell, UInt32 area, UInt32 index) {
		if (!akSpell || index >= akSpell->effectItemList.count)
			return false;
		akSpell->effectItemList[index]->area = area;
		return true;
	}
	bool SetSpellNthMagicEffectCost(StaticFunctionTag* base, SpellItem* akSpell, float cost, UInt32 index) {
		if (!akSpell || index >= akSpell->effectItemList.count)
			return false;
		akSpell->effectItemList[index]->cost = cost;
		return true;
	}

	// getter for magical effect cost
	float GetSpellNthMagicEffectCost(StaticFunctionTag* base, SpellItem* akSpell, UInt32 index) {
		if (!akSpell || index >= akSpell->effectItemList.count)
			return -1.0;
		return akSpell->effectItemList[index]->cost;
	}

	// bunch of setters for spell variables
	bool SetSpellType(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akType) {
		if (!akSpell)
			return false;
		akSpell->data.type = akType;
		return true;
	}
	bool SetSpellCastType(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akType) {
		if (!akSpell)
			return false;
		akSpell->data.type = akType;
		return true;
	}
	bool SetSpellCastTime(StaticFunctionTag* base, SpellItem* akSpell, float akTime) {
		if (!akSpell)
			return false;
		akSpell->data.castTime = akTime;
		return true;
	}
	bool SetSpellDelivery(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akType) {
		if (!akSpell)
			return false;
		akSpell->data.type = akType;
		return true;
	}
	bool SetSpellPerk(StaticFunctionTag* base, SpellItem* akSpell, BGSPerk* akPerk) {
		if (!akSpell)
			return false;
		akSpell->data.spellPerk= akPerk;
		return true;
	}
	bool SetSpellCost(StaticFunctionTag* base, SpellItem* akSpell, UInt32 akCost) {
		if (!akSpell)
			return false;
		akSpell->data.unk00.cost = akCost;
		return true;
	}
	bool SetSpellName(StaticFunctionTag* base, SpellItem* akSpell, BSFixedString name) {
		if (!akSpell)
			return false;
		akSpell->fullName.name = name;
		return true;
	}

	// bunch of getters for spell variables
	UInt32 GetSpellType(StaticFunctionTag* base, SpellItem* akSpell) {
		return akSpell ? akSpell->data.type : UINT32_MAX;
	}
	UInt32 GetSpellCastType(StaticFunctionTag* base, SpellItem* akSpell) {
		return akSpell ? akSpell->data.castType : UINT32_MAX;
	}
	UInt32 GetSpellCastTime(StaticFunctionTag* base, SpellItem* akSpell) {
		return akSpell ? akSpell->data.castTime : UINT32_MAX;
	}
	BGSPerk* GetSpellPerk(StaticFunctionTag* base, SpellItem* akSpell) {
		return akSpell ? akSpell->data.spellPerk : NULL;
	}

	// copies all the data from {spellB} to {spellA} (essentially makes a copy of spellB through spellA)
	bool _copySpell(SpellItem* spellA, SpellItem* spellB) {
		if (!spellA || !spellB)
			return false;
		// copy magicitem vars
		spellA->keyword = spellB->keyword;
		spellA->hostile = spellB->hostile;
		spellA->effectTemplate = spellB->effectTemplate;
		// copy spellitem vars
		spellA->equipType = spellB->equipType;
		spellA->dispObj = spellB->dispObj;
		tArray<MagicItem::EffectItem*> new_list = tArray<MagicItem::EffectItem*>();
		new_list.Grow(128);
		spellA->effectItemList = new_list;
		// copy spellitem data struct
		spellA->data = spellB->data;
		return true;
	}

	bool CopySpell(StaticFunctionTag* base, SpellItem* spellA, SpellItem* spellB) {
		return _copySpell(spellA, spellB);
	}

	// doesnt work, need more digging to find out why, probably because the game doesnt update after this
	bool RemoveAllActorSpells(StaticFunctionTag* base, Actor* akActor, bool removeBase) {
		if (!akActor)
			return false;
		Actor::SpellArray empty = Actor::SpellArray();
		akActor->addedSpells = empty;
		if (removeBase) {
			TESNPC* akActorBase = DYNAMIC_CAST(akActor->baseForm, TESForm, TESNPC);
			TESSpellList::Data* empty_dat = new TESSpellList::Data();
			akActorBase->spellList.data = empty_dat;
		}
		return true;
	}

	//void SetAssociatedSkill(EffectSetting* thisEffect, BSFixedString school)
	//{
	//	if (thisEffect) {
	//		UInt32 actorValue = ActorValueList::ResolveActorValueByName(school.data);
	//		thisEffect->properties.school = actorValue;
	//	}
	//}

	// helper function that checks if a spell is in a specific school (for GetAllActorSpells)
	bool _checkSpell(SpellItem* akSpell, BSFixedString akSchool, UInt32 type) {
		if (akSpell->data.type != type)
			return false;
		if (akSchool.data[0] == '\0') {
			return true;
		}
		UInt32 actorValue = ActorValueList::ResolveActorValueByName(akSchool.data);

		// only check the first effect because its the main one and it avoids counting stuff like
		// flames as illusion because of secondary effects
		return akSpell->effectItemList[0]->mgef->properties.school == actorValue;
	}

	// returns an array of all spells an actor has, including the actor's base
	// also accepts a specific school (empty for any school) and a specific spell type (i.e. spell, shout, etc)
	VMResultArray<SpellItem*> GetAllActorSpells(StaticFunctionTag* base, Actor* akActor, BSFixedString akSchool, UInt32 type) {
		VMResultArray<SpellItem*> retArr;
		if (!akActor)
			return retArr;
		SpellItem* cur;
		for (int i=0; i<akActor->addedSpells.Length(); ++i) {
			cur = akActor->addedSpells.Get(i);
			if (_checkSpell(cur, akSchool, type)) {
				retArr.push_back(cur);
				// _MESSAGE("Found spell %s", cur->fullName.name);
			}
		}
		for (int i=0; i<akActor->race->spellList.GetSpellCount(); ++i) {
			cur = akActor->race->spellList.GetNthSpell(i);
			if (_checkSpell(cur, akSchool, type)) {
				retArr.push_back(cur);
				// _MESSAGE("Found spell %s", cur->fullName.name);
			}
		}
		TESNPC* akActorBase = DYNAMIC_CAST(akActor->baseForm, TESForm, TESNPC);
		for (int i=0; i<akActorBase->spellList.GetSpellCount(); ++i) {
			cur = akActorBase->spellList.GetNthSpell(i);
			if (_checkSpell(cur, akSchool, type)) {
				retArr.push_back(cur);
				// _MESSAGE("Found spell %s", cur->fullName.name);
			}
		}
		return retArr;
	}

	// filters an array of spells, removing spells with duplicate names and any names in {tags}
	// much faster than trying to do this in papyrus lol
	VMResultArray<SpellItem*> FilterSpellArray(StaticFunctionTag* base, VMArray<SpellItem*> spells, VMArray<BSFixedString> tags) {
		VMResultArray<SpellItem*> filtered_list;
		SpellItem* curSpell = NULL;
		BSFixedString str;
		std::set<const char*> seen;
		const char* name;
		bool add = true;
		for (UInt32 i = 0; i < spells.Length(); ++i) {
			spells.Get(&curSpell, i);
			name = curSpell->fullName.GetName();
			add = true;
			for (UInt32 j = 0; j < tags.Length(); ++j) {
				tags.Get(&str, j);
				if (strcmp(name, str.data) == 0) {
					add = false;
					break;
				}
			}
			if (add && seen.find(name) == seen.end()) {
				filtered_list.push_back(curSpell);
			}
			seen.insert(name);
		}
		return filtered_list;
	}

	// Allows you to combine an array of spells
	// Modifies {akBase} to combine all the effects of {akSpells}
	// mostly used for my own mod
	bool CombineSpells(StaticFunctionTag* base, SpellItem* akBase, VMArray<SpellItem*> akSpells, BSFixedString akName) {
		SpellItem* new_spell = NULL;
		akSpells.Get(&new_spell, 0);
		if (!akBase || !new_spell) {
			_MESSAGE("NO SPELLS TO COMBINE");
			return false;
		}
		// _MESSAGE("FIRST SPELL = %s", new_spell->fullName.name);
		_copySpell(akBase, new_spell); //->CopyFrom(new_spell);
		_MESSAGE("BASE SPELL = %s", new_spell->fullName.name);
		akBase->fullName.name = akName;
		// _MESSAGE("%d", akBase->effectItemList.capacity);
		// _MESSAGE("%d", akSpells.Length());
		for (UInt32 i=0; i < akSpells.Length(); ++i) {
			akSpells.Get(&new_spell, i);
			if (!new_spell) {
				break;
			}
			for (UInt32 j=0; j < new_spell->effectItemList.count; ++j) {
				akBase->effectItemList.Push(new_spell->effectItemList[j]);
				_MESSAGE("added effect %s", new_spell->effectItemList[j]->mgef->fullName.GetName());
			}
		}
		// _MESSAGE("%d", akBase->effectItemList.capacity);
		// _MESSAGE("Combo effects (%d):", akBase->effectItemList.count);
		// for (UInt32 i=0; i < akBase->effectItemList.count; ++i) {
			// _MESSAGE("%s, duration: %d", akBase->effectItemList[i]->mgef->fullName.GetName(), akBase->effectItemList[i]->duration);
		// }
		//_MESSAGE("%s", akBase->effectTemplate->fullName.GetName());
		// _MESSAGE("Finished listing effects");
		return true;
	}

	bool RegisterFuncs(VMClassRegistry* registry) {
		//registry->RegisterFunction(
		//	new NativeFunction0<StaticFunctionTag, float>("MyTest", "All41", SpellCraft::MyTest, registry));
		//registry->RegisterFunction(
		//	new NativeFunction1 <StaticFunctionTag, UInt32, SpellItem*>("GetCastType", "All41", SpellCraft::GetCastType, registry));
		//registry->RegisterFunction(
		//	new NativeFunction1 <SpellItem, BSFixedString, EffectSetting*>("PlsWork", "Spell", SpellCraft::PlsWork, registry));
		//registry->RegisterFunction(
		//	new NativeFunction1 <SpellItem, bool, EffectSetting*>("AddEffect", "Spell", SpellCraft::AddEffect, registry));

		registry->RegisterFunction(
			new NativeFunction6 <StaticFunctionTag, bool, SpellItem*, EffectSetting*, float, UInt32, UInt32, float>("SpellAddEffect", "SpellCraft", SpellCraft::SpellAddEffect, registry));

		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, bool, SpellItem*, EffectSetting*, UInt32>("SetSpellNthMagicEffect", "SpellCraft", SpellCraft::SetSpellNthMagicEffect, registry));
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, bool, SpellItem*, float, UInt32>("SetSpellNthMagicEffectMagnitude", "SpellCraft", SpellCraft::SetSpellNthMagicEffectMag, registry));
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, bool, SpellItem*, UInt32, UInt32>("SetSpellNthMagicEffectDuration", "SpellCraft", SpellCraft::SetSpellNthMagicEffectDur, registry));
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, bool, SpellItem*, UInt32, UInt32>("SetSpellNthMagicEffectArea", "SpellCraft", SpellCraft::SetSpellNthMagicEffectArea, registry));
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, bool, SpellItem*, float, UInt32>("SetSpellNthMagicEffectCost", "SpellCraft", SpellCraft::SetSpellNthMagicEffectCost, registry));
		
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, float, SpellItem*, UInt32>("GetSpellNthMagicEffectCost", "SpellCraft", SpellCraft::GetSpellNthMagicEffectCost, registry));
		
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, UInt32>("SetSpellType", "SpellCraft", SpellCraft::SetSpellType, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, UInt32>("SetSpellCastType", "SpellCraft", SpellCraft::SetSpellCastType, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, float>("SetSpellCastTime", "SpellCraft", SpellCraft::SetSpellCastTime, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, UInt32>("SetSpellDelivery", "SpellCraft", SpellCraft::SetSpellDelivery, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, BGSPerk*>("SetSpellPerk", "SpellCraft", SpellCraft::SetSpellPerk, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, UInt32>("SetSpellCost", "SpellCraft", SpellCraft::SetSpellCost, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, BSFixedString>("SetSpellName", "SpellCraft", SpellCraft::SetSpellName, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, UInt32, SpellItem*>("GetSpellType", "SpellCraft", SpellCraft::GetSpellType, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, UInt32, SpellItem*>("GetSpellCastType", "SpellCraft", SpellCraft::GetSpellCastType, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, UInt32, SpellItem*>("GetSpellCastTime", "SpellCraft", SpellCraft::GetSpellCastTime, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, BGSPerk*, SpellItem*>("GetSpellPerk", "SpellCraft", SpellCraft::GetSpellPerk, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, SpellItem*, SpellItem*>("CopySpell", "SpellCraft", SpellCraft::CopySpell, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, Actor*, bool>("RemoveAllActorSpells", "SpellCraft", SpellCraft::RemoveAllActorSpells, registry));
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, VMResultArray<SpellItem*>, Actor*, BSFixedString, UInt32>("GetAllActorSpells", "SpellCraft", SpellCraft::GetAllActorSpells, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, VMResultArray<SpellItem*>, VMArray<SpellItem*>, VMArray<BSFixedString>>("FilterSpellArray", "SpellCraft", SpellCraft::FilterSpellArray, registry));
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, bool, SpellItem*, VMArray<SpellItem*>, BSFixedString>("CombineSpells", "SpellCraft", SpellCraft::CombineSpells, registry));
		return true;
	}
}
