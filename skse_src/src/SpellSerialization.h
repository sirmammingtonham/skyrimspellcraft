#pragma once
#include "skse64/PluginAPI.h"
#include "skse64/GameData.h"
#include "skse64/GameObjects.h"

namespace SpellSerializer {
	// reads a serialized form id and returns its pointer
	template <typename T> T* _deserializeNextForm(SKSESerializationInterface* a_intfc);

	// serializes a spell's effect item (so we can serialize the spell's effects)
	bool _serializeEffectItem(SKSESerializationInterface* a_intfc, MagicItem::EffectItem* effect);

	// deserializes a spell's effect item
	MagicItem::EffectItem* _deserializeEffectItem(SKSESerializationInterface* a_intfc);

	// fully serializes a spell
	bool serializeSpell(SKSESerializationInterface* a_intfc, SpellItem* spell);

	// reads a serialized spell and remodifies it
	bool deserializeSpell(SKSESerializationInterface* a_intfc, SpellItem* spell);
}