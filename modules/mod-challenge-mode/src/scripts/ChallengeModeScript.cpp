#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "InstanceSaveMgr.h"
#include "Player.h"
#include "Map.h"
#include "WorldSession.h"
#include "Group.h"
#include "AzthUtils.h"
#include "AZTH.h"

class AzthUtils;

#define MAX_HIGHER_LEVEL 3

class ChallengeModeMisc : public MiscScript
{
public:
    ChallengeModeMisc() : MiscScript("ChallengeModeMisc") { }

    void OnAfterLootTemplateProcess(Loot* /* loot */, LootTemplate const* /* tab */, LootStore const& /* store */, Player* /* lootOwner */, bool /* personal */, bool /* noEmptyError */, uint16 /* lootMode */) override
    {
        // if (!sConfigMgr->GetBoolDefault("Azth.Multiplies.Drop.Enable", false))
        //     return;

        //Dangerous since it can drops multiple quest items
        //[AZTH-DISABLED] give another loot process if done with correct level
        // if (sAzthUtils->isEligibleForBonusByArea(lootOwner) && (&store == &LootTemplates_Gameobject || &store == &LootTemplates_Creature))
        // {
        //     sAZTH->AddAZTHLoot(loot);
        //     tab->Process(*loot, store, lootMode, lootOwner);
        // }
    }
};

class ChallengeModePlayer : public PlayerScript
{
public:
    ChallengeModePlayer() : PlayerScript("ChallengeModePlayer") { }

    bool OnBeforeAchiComplete(Player* player, AchievementEntry const* /* achievement */) override
    {
        WorldLocation pos = WorldLocation(player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
        uint32 posLvl=sAzthUtils->getPositionLevel(true, player->GetMap(), pos);

        if (!player->GetMap()->IsDungeon() && !player->GetMap()->IsRaid())
            return true;

        uint32 level = player->getLevel();
        if (posLvl > level && posLvl - level == MAX_HIGHER_LEVEL) {
            return false;
        }

        return true;
    }
};

class ChallengeModeGlobal : public GlobalScript
{
public:

    ChallengeModeGlobal() : GlobalScript("ChallengeModeGlobal") {
    }

    void OnItemRoll(Player const* player, LootStoreItem const */* item */, float &chance, Loot &/* loot */, LootStore const& /* store */) override
    {
        WorldLocation pos = WorldLocation(player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
        uint32 posLvl=sAzthUtils->getPositionLevel(true, player->GetMap(), pos);

        uint32 level = player->getLevel();

        if (!player->GetMap()->IsDungeon() && !player->GetMap()->IsRaid())
            return;

        if (posLvl > level && posLvl - level == MAX_HIGHER_LEVEL) {
            chance = 0;
            return;
        }

        // [AZTH-DISABLED]
        // this check assume that sAzthUtils->isEligibleForBonusByArea(player) has been already checked
        // if (!sAZTH->GetAZTHLoot(&loot))
        //     return;

        // if ((loot.quest_items.size() + loot.items.size()) >= MAX_NR_LOOT_ITEMS)
        // {
        //     chance = 0;
        //     return;
        // }

        // if (chance >= 100.0f || chance <= 0)
        //     return;

        // if (!player->GetMap()->IsDungeon() && !player->GetMap()->IsRaid())
        //     return;

        // if (item->mincount >= 0)
        // {
        //     ItemTemplate const* i = sObjectMgr->GetItemTemplate(item->itemid);

        //     if (i && i->Quality < ITEM_QUALITY_RARE)
        //         return;
        // }

        // if (&store != &LootTemplates_Gameobject && &store != &LootTemplates_Creature)
        //     return;

        // if (chance < 20.f)
        //     chance += 20.f;

        // sAZTH->DeleteAZTHLoot(&loot);
    }

};

void AddSC_challengemode() {
    new ChallengeModeMisc();
    new ChallengeModeGlobal();
    new ChallengeModePlayer();
}