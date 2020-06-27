#include "SpellSerialization.h"

namespace SpellSerializer {
	// reads a serialized form id and returns its pointer
	template <typename T>
	T* _deserializeNextForm(SKSESerializationInterface* a_intfc) {
		T* ret = NULL;
		UInt32 id;
		a_intfc->ReadRecordData(&id, sizeof(id));
		if (a_intfc->ResolveFormId(id, &id)) {
			ret = dynamic_cast<T*>(LookupFormByID(id));
		}
		return ret;
	}
	// reads a serialized form id and returns its pointer
	bool _serializeEffectItem(SKSESerializationInterface* a_intfc, MagicItem::EffectItem* effect) {
		a_intfc->WriteRecordData(&(effect->magnitude), sizeof(float));
		a_intfc->WriteRecordData(&(effect->area), sizeof(UInt32));
		a_intfc->WriteRecordData(&(effect->duration), sizeof(UInt32));
		a_intfc->WriteRecordData(&(effect->pad0C), sizeof(UInt32));
		a_intfc->WriteRecordData(&(effect->cost), sizeof(float));
		a_intfc->WriteRecordData(&(effect->unk1C), sizeof(UInt32));

		return a_intfc->WriteRecordData(&(effect->mgef->formID), sizeof(UInt32));
	}

	MagicItem::EffectItem* _deserializeEffectItem(SKSESerializationInterface* a_intfc) {
		MagicItem::EffectItem* effect = new MagicItem::EffectItem();
		a_intfc->ReadRecordData(&(effect->magnitude), sizeof(float));
		a_intfc->ReadRecordData(&(effect->area), sizeof(UInt32));
		a_intfc->ReadRecordData(&(effect->duration), sizeof(UInt32));
		a_intfc->ReadRecordData(&(effect->pad0C), sizeof(UInt32));
		a_intfc->ReadRecordData(&(effect->cost), sizeof(float));
		a_intfc->ReadRecordData(&(effect->unk1C), sizeof(UInt32));

		effect->mgef = _deserializeNextForm<EffectSetting>(a_intfc);

		_MESSAGE("Deserialized effect %s", effect->mgef->fullName.name);
		return effect;
	}

	bool serializeSpell(SKSESerializationInterface* a_intfc, SpellItem* spell) {
		// serialize form id so we can find the spell again during load
		_MESSAGE("%d", spell->formID);
		a_intfc->WriteRecordData(&(spell->formID), sizeof(UInt32));

		// serialize name in case it was modified
		std::size_t len = strlen(spell->fullName.name.c_str());
		a_intfc->WriteRecordData(&len, sizeof(len));
		a_intfc->WriteRecordData(spell->fullName.name.c_str(), len);

		// serialize other stuff (formid)
		a_intfc->WriteRecordData(&(spell->keyword), sizeof(BGSKeywordForm));
		a_intfc->WriteRecordData(&(spell->hostile), sizeof(UInt32));
		bool hasTemplate = spell->effectTemplate;
		a_intfc->WriteRecordData(&(hasTemplate), sizeof(UInt32));
		if (hasTemplate)
			a_intfc->WriteRecordData(&(spell->effectTemplate->formID), sizeof(UInt32)); // usually null, except for when used with combine spells
		a_intfc->WriteRecordData(&(spell->equipType.equipSlot->formID), sizeof(UInt32));
		a_intfc->WriteRecordData(&(spell->dispObj.worldStatic->formID), sizeof(UInt32));

		// serialize the spell's effects
		_MESSAGE("saving %d effects", spell->effectItemList.count);
		if (a_intfc->WriteRecordData(&(spell->effectItemList.count), sizeof(UInt32))) {
			for (int i = 0; i < spell->effectItemList.count; ++i) {
				_serializeEffectItem(a_intfc, spell->effectItemList[i]);
			}
		}

		// serialize the data struct
		a_intfc->WriteRecordData(&(spell->data.unk00), sizeof(SpellItem::Data::Data0));
		a_intfc->WriteRecordData(&(spell->data.type), sizeof(UInt32));
		a_intfc->WriteRecordData(&(spell->data.castTime), sizeof(float));
		a_intfc->WriteRecordData(&(spell->data.castType), sizeof(UInt32));

		a_intfc->WriteRecordData(&(spell->data.unk14), sizeof(UInt32)); // one of these is cast type, should figure out which
		a_intfc->WriteRecordData(&(spell->data.unk18), sizeof(UInt32));
		a_intfc->WriteRecordData(&(spell->data.unk1C), sizeof(UInt32));

		a_intfc->WriteRecordData(&(spell->data.spellPerk->formID), sizeof(UInt32));

		_MESSAGE("successfully saved spell %s", spell->fullName.name);
		return true;
	}

	bool deserializeSpell(SKSESerializationInterface* a_intfc, SpellItem* spell) {

		std::size_t len;
		a_intfc->ReadRecordData(&len, sizeof(len));
		char buf[257] = { '\0' };
		a_intfc->ReadRecordData(buf, len);
		spell->fullName.name = buf;

		a_intfc->ReadRecordData(&(spell->keyword), sizeof(BGSKeywordForm));
		a_intfc->ReadRecordData(&(spell->hostile), sizeof(UInt32));
		bool hasTemplate = false;
		a_intfc->ReadRecordData(&(hasTemplate), sizeof(UInt32));
		if (hasTemplate)
			spell->effectTemplate = _deserializeNextForm<EffectSetting>(a_intfc); // usually null

		spell->equipType.equipSlot = _deserializeNextForm<BGSEquipSlot>(a_intfc);
		spell->dispObj.worldStatic = _deserializeNextForm<TESObjectSTAT>(a_intfc);

		UInt32 num_effects;
		if (a_intfc->ReadRecordData(&(num_effects), sizeof(UInt32))) {
			spell->effectItemList.Clear();
			for (int i = 0; i < num_effects; ++i) {
				spell->effectItemList.Push(_deserializeEffectItem(a_intfc));
			}
			_MESSAGE("Loaded %d effects", spell->effectItemList.count);
		}

		a_intfc->ReadRecordData(&(spell->data.unk00), sizeof(SpellItem::Data::Data0));
		a_intfc->ReadRecordData(&(spell->data.type), sizeof(UInt32));
		a_intfc->ReadRecordData(&(spell->data.castTime), sizeof(float));
		a_intfc->ReadRecordData(&(spell->data.castType), sizeof(UInt32));

		a_intfc->ReadRecordData(&(spell->data.unk14), sizeof(UInt32));
		a_intfc->ReadRecordData(&(spell->data.unk18), sizeof(UInt32));
		a_intfc->ReadRecordData(&(spell->data.unk1C), sizeof(UInt32));
		spell->data.spellPerk = _deserializeNextForm<BGSPerk>(a_intfc);


		_MESSAGE("successfully loaded spell %s", spell->fullName.name);
		return true;
	}
}