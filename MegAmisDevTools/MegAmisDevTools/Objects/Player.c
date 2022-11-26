#include "GameAPI/Game.h"
#include "Player.h"
#include "MegAmiMenu.h"

ObjectPlayer *Player;

void Player_Update(void)
{
    RSDK_THIS(Player);

    // Handle Invincibility Toggle
    if (MegAmiMenu->playerInv && self->blinkTimer >= 0 && self->blinkTimer <= 2)
        self->blinkTimer = 2;

    if (self->outerbox && !self->innerbox)
        self->innerbox = RSDK.GetHitbox(&self->animator, 1);

    Mod.Super(Player->classID, SUPER_UPDATE, NULL);
}

bool32 Player_Input_P1_Hook(bool32 skippedState)
{
    RSDK_THIS(Player);

    ObjectAnnouncer *announcer       = Mod.FindObject("Announcer");
    ObjectLottoMachine *lottoMachine = Mod.FindObject("LottoMachine");
    if (!announcer)
        return false;

    if (self->controllerID <= CONT_P4) {
        if (globals->gameMode != MODE_COMPETITION || announcer->finishedCountdown) {
            if (!lottoMachine || !((1 << self->playerID) & lottoMachine->activePlayers)) {
                if (ControllerInfo[self->controllerID].keySelect.press) {
                    CREATE_ENTITY(MegAmiMenu, self, self->position.x, self->position.y);
                    ControllerInfo[self->controllerID].keySelect.press = false;
                }
            }
        }
    }

    return false;
}