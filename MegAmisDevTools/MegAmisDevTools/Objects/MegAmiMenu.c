// ---------------------------------------------------------------------
// RSDK Project: MegAmi's Dev Tools
// Original Game: Sonic Mania
// Object Description: MegAmiMenu Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "GameAPI/Game.h"
#include "MegAmiMenu.h"
#include "Player.h"

ObjectMegAmiMenu *MegAmiMenu;

void MegAmiMenu_Update(void)
{
    RSDK_THIS(MegAmiMenu);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    if (player->classID != Player->classID) {
        destroyEntity(self);
    }
    else {
        if (ControllerInfo[player->controllerID].keySelect.press)
            destroyEntity(self);
        else
            StateMachine_Run(self->state);
    }

    if (self->classID != MegAmiMenu->classID && SceneInfo->state & ENGINESTATE_PAUSED)
        RSDK.SetEngineState(ENGINESTATE_REGULAR);
}

void MegAmiMenu_LateUpdate(void) {}

void MegAmiMenu_StaticUpdate(void) {}

void MegAmiMenu_Draw(void)
{
    RSDK_THIS(MegAmiMenu);

    EntityPlayer *parentplayer = (EntityPlayer *)self->parent;
    EntityPlayer *camplayer    = RSDK_GET_ENTITY(SceneInfo->currentScreenID, Player);
    
    if (RSDK.GetEntitySlot(parentplayer) == RSDK.GetEntitySlot(camplayer)) {
        RSDK.DrawRect(16, 12, 150, 100, 0xFF0000, 0xFF, INK_NONE, true);

        Vector2 drawPos;

        // Draw Selection Arrow
        drawPos.x = TO_FIXED(22);
        drawPos.y = TO_FIXED(21 + (self->mainSelection * VERT_SPACING));
        RSDK.DrawText(&self->animator, &drawPos, &self->arrow, 0, self->arrow.length, ALIGN_LEFT, 0, 0, 0, true);

        // Initialize Option Text Position
        drawPos.x = TO_FIXED(32);
        drawPos.y = TO_FIXED(21);

        // Draw Option Text
        RSDK.DrawText(&self->animator, &drawPos, &self->p1char, 0, self->p1char.length, ALIGN_LEFT, 0, 0, 0, true);
        drawPos.y += TO_FIXED(VERT_SPACING);
        RSDK.DrawText(&self->animator, &drawPos, &self->p2char, 0, self->p2char.length, ALIGN_LEFT, 0, 0, 0, true);
        drawPos.y += TO_FIXED(VERT_SPACING);
        RSDK.DrawText(&self->animator, &drawPos, &self->shield, 0, self->shield.length, ALIGN_LEFT, 0, 0, 0, true);
        drawPos.y += TO_FIXED(VERT_SPACING);
        RSDK.DrawText(&self->animator, &drawPos, &self->super, 0, self->super.length, ALIGN_LEFT, 0, 0, 0, true);
        drawPos.y += TO_FIXED(VERT_SPACING);
        RSDK.DrawText(&self->animator, &drawPos, &self->inv, 0, self->inv.length, ALIGN_LEFT, 0, 0, 0, true);
        drawPos.y += TO_FIXED(VERT_SPACING);
        RSDK.DrawText(&self->animator, &drawPos, &self->exit, 0, self->exit.length, ALIGN_LEFT, 0, 0, 0, true);

        if (self->stateDraw)
            RSDK.DrawRect(16, 12, 150, 100, 0x000000, 0x80, INK_ALPHA, true);

        StateMachine_Run(self->stateDraw);
    }
}

void MegAmiMenu_Create(void *data)
{
    RSDK_THIS(MegAmiMenu);

    ObjectUIWidgets *uiwidgets = Mod.FindObject("UIWidgets");
    ObjectZone *zone           = Mod.FindObject("Zone");

    RSDK.SetSpriteAnimation(uiwidgets->fontFrames, 0, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->drawGroup = zone->hudDrawGroup;

        self->parent         = (Entity *)data;
        EntityPlayer *player = (EntityPlayer *)self->parent;

        // Initialize Strings
        RSDK.InitString(&self->p1char, "CHANGE CHARACTER", false);
        RSDK.InitString(&self->p2char, "CHANGE SIDEKICK", false);
        RSDK.InitString(&self->shield, "CHANGE SHIELD", false);
        if (player->superState != SUPERSTATE_SUPER)
            RSDK.InitString(&self->super, "TURN SUPER", false);
        else
            RSDK.InitString(&self->super, "REVERT SUPER", false);
        if (!MegAmiMenu->playerInv)
            RSDK.InitString(&self->inv, "ENABLE INVINCIBILITY", false);
        else
            RSDK.InitString(&self->inv, "DISABLE INVINCIBILITY", false);
        RSDK.InitString(&self->exit, "EXIT", false);
        RSDK.InitString(&self->sonic, "SONIC", false);
        RSDK.InitString(&self->tails, "TAILS", false);
        RSDK.InitString(&self->knux, "KNUCKLES", false);
#if MANIA_USE_PLUS
        RSDK.InitString(&self->mighty, "MIGHTY", false);
        RSDK.InitString(&self->ray, "RAY", false);
#endif
        RSDK.InitString(&self->none, "NONE", false);
        RSDK.InitString(&self->blue, "BLUE SHIELD", false);
        RSDK.InitString(&self->bubble, "BUBBLE SHIELD", false);
        RSDK.InitString(&self->fire, "FIRE SHIELD", false);
        RSDK.InitString(&self->lightning, "LIGHTNING SHIELD", false);
        RSDK.InitString(&self->arrow, ">", false);

        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->p1char);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->p2char);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->shield);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->super);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->inv);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->exit);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->sonic);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->tails);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->knux);
#if MANIA_USE_PLUS
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->mighty);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->ray);
#endif
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->none);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->blue);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->bubble);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->fire);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->lightning);
        RSDK.SetSpriteString(uiwidgets->fontFrames, 0, &self->arrow);

        self->state     = MegAmiMenu_State_Main;
        self->stateDraw = StateMachine_None;

        RSDK.SetEngineState(ENGINESTATE_PAUSED);
    }
}

void MegAmiMenu_StageLoad(void) { MegAmiMenu->sfxFail = RSDK.GetSfx("Stage/Fail.wav"); }

void MegAmiMenu_State_Main(void)
{
    RSDK_THIS(MegAmiMenu);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    bool32 confirmPress =
        ControllerInfo[player->controllerID].keyStart.press
        || (API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyB.press : ControllerInfo[player->controllerID].keyA.press);
    bool32 backPress = API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyA.press : ControllerInfo[player->controllerID].keyB.press;

    if (ControllerInfo[player->controllerID].keyDown.press) {
        if (++self->mainSelection > MEGAMIMENU_EXIT)
            self->mainSelection = MEGAMIMENU_P1CHAR;
    }
    else if (ControllerInfo[player->controllerID].keyUp.press) {
        if (--self->mainSelection < MEGAMIMENU_P1CHAR)
            self->mainSelection = MEGAMIMENU_EXIT;
    }
    else if (confirmPress) {
        switch (self->mainSelection) {
            default: break;
            case MEGAMIMENU_P1CHAR:
                self->state        = MegAmiMenu_State_P1Char;
                self->stateDraw    = MegAmiMenu_State_DrawChar;
                self->subSelection = 0;
                break;
            case MEGAMIMENU_P2CHAR:
                if (!player->sidekick || !globals->gameMode == MODE_COMPETITION) {
                    self->state        = MegAmiMenu_State_P2Char;
                    self->stateDraw    = MegAmiMenu_State_DrawChar;
                    self->subSelection = 0;
                }
                else
                    RSDK.PlaySfx(MegAmiMenu->sfxFail, false, 255);
                break;
            case MEGAMIMENU_SHIELD:
                self->state        = MegAmiMenu_State_Shield;
                self->stateDraw    = MegAmiMenu_State_DrawShield;
                self->subSelection = 0;
                break;
            case MEGAMIMENU_SUPER:
                if (player->superState != SUPERSTATE_SUPER) {
                    Player_GiveRings(player, 50, false);
                    Player_TryTransform(player, 0x7F);
                }
                else
                    player->superState = SUPERSTATE_FADEOUT;
                destroyEntity(self);
                break;
            case MEGAMIMENU_INV:
                MegAmiMenu->playerInv = MegAmiMenu->playerInv ? false : true;
                destroyEntity(self);
                break;
            case MEGAMIMENU_EXIT: destroyEntity(self); break;
        }
    }
    else if (backPress) {
        destroyEntity(self);
    }
}

void MegAmiMenu_State_P1Char(void)
{
    RSDK_THIS(MegAmiMenu);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    bool32 confirmPress =
        ControllerInfo[player->controllerID].keyStart.press
        || (API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyB.press : ControllerInfo[player->controllerID].keyA.press);
    bool32 backPress = API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyA.press : ControllerInfo[player->controllerID].keyB.press;

    if (ControllerInfo[player->controllerID].keyDown.press) {
#if MANIA_USE_PLUS
        if (++self->subSelection > (API.CheckDLC(DLC_PLUS) ? 4 : 2))
#else
        if (++self->subSelection > 2)
#endif
            self->subSelection = 0;
    }
    else if (ControllerInfo[player->controllerID].keyUp.press) {
        if (--self->subSelection < 0)
#if MANIA_USE_PLUS
            self->subSelection = (API.CheckDLC(DLC_PLUS) ? 4 : 2);
#else
            self->subSelection = 2;
#endif
    }
    else if (confirmPress) {
        Player_ChangeCharacter(player, 1 << self->subSelection);
        destroyEntity(self);
    }
    else if (backPress) {
        self->state     = MegAmiMenu_State_Main;
        self->stateDraw = StateMachine_None;
    }
}

void MegAmiMenu_State_P2Char(void)
{
    RSDK_THIS(MegAmiMenu);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    bool32 confirmPress =
        ControllerInfo[player->controllerID].keyStart.press
        || (API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyB.press : ControllerInfo[player->controllerID].keyA.press);
    bool32 backPress = API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyA.press : ControllerInfo[player->controllerID].keyB.press;

    if (ControllerInfo[player->controllerID].keyDown.press) {
#if MANIA_USE_PLUS
        if (++self->subSelection > (API.CheckDLC(DLC_PLUS) ? 5 : 3))
#else
        if (++self->subSelection > 3)
#endif
            self->subSelection = 0;
    }
    else if (ControllerInfo[player->controllerID].keyUp.press) {
        if (--self->subSelection < 0)
#if MANIA_USE_PLUS
            self->subSelection = (API.CheckDLC(DLC_PLUS) ? 5 : 3);
#else
            self->subSelection = 3;
#endif
    }
    else if (confirmPress) {
        EntityPlayer *sidekick = RSDK_GET_ENTITY(RSDK.GetEntitySlot(player) + 1, Player);
        if (sidekick->classID == Player->classID) {
            // P2 already exists, just change the character
            if (self->subSelection == 0) {
                Player_ChangeCharacter(sidekick, ID_NONE);
                destroyEntity(sidekick);

                // Handle new player count
                EntityShield *shield = RSDK_GET_ENTITY(RSDK.GetEntitySlot(player) + Player->playerCount, Shield);
                if (shield->classID)
                    RSDK.CopyEntity(sidekick, shield, true);
#if MANIA_USE_PLUS
                Player->playerCount = globals->gameMode == MODE_ENCORE ? 2 : RSDK.GetEntityCount(Player->classID, false);
#else
                Player->playerCount = RSDK.GetEntityCount(Player->classID, false);
#endif
            }
            else
                Player_ChangeCharacter(sidekick, 1 << (self->subSelection - 1));
        }
        else {
            if (self->subSelection > 0) {
                // P2 doesn't exist, spawn them
                ObjectZone *zone = Mod.FindObject("Zone");
                ObjectDust *Dust = Mod.FindObject("Dust");

                if (sidekick->classID) // If this is a powerup
                    RSDK.CopyEntity(RSDK.GetEntity(RSDK.GetEntitySlot(sidekick) + 1), sidekick, true);

                sidekick->classID = Player->classID;
                Player->jumpInTimer = 0;
                sidekick->playerID  = 1;

                EntityDust *entdust  = CREATE_ENTITY(Dust, INT_TO_VOID(1), sidekick->position.x, sidekick->position.y);
                entdust->visible     = false;
                entdust->active      = ACTIVE_NEVER;
                entdust->isPermanent = true;
                entdust->position.y  = TO_FIXED(ScreenInfo->position.y - 128);

                Player_ChangeCharacter(sidekick, 1 << (self->subSelection - 1));
                sidekick->velocity.x = 0;
                sidekick->velocity.y = 0;
                sidekick->groundVel  = 0;
                sidekick->position.x = -TO_FIXED(64);
                sidekick->position.y = -TO_FIXED(64);
                sidekick->angle      = 0x80;
                if (sidekick->characterID == ID_TAILS) {
                    sidekick->state = Player_State_FlyToPlayer;
                }
                else {
                    sidekick->state            = Player_State_ReturnToPlayer;
                    sidekick->abilityValues[0] = ((ScreenInfo->position.y + ScreenInfo->size.y + 16) << 16) - player->position.y;
                    sidekick->drawFX |= FX_SCALE;
                    sidekick->scale.x    = 0x400;
                    sidekick->scale.y    = 0x400;
                    sidekick->velocity.y = CLAMP(sidekick->abilityValues[0] / -12, -0xE0000, -0x68000);
                }
                sidekick->abilityPtrs[0]   = entdust;
                sidekick->abilityValues[0] = 0;
                sidekick->nextAirState     = StateMachine_None;
                sidekick->nextGroundState  = StateMachine_None;
                sidekick->stateInput       = Player_Input_P2_Delay;
                sidekick->tileCollisions   = TILECOLLISION_NONE;
                sidekick->interaction      = false;
                sidekick->drawGroup        = zone->playerDrawGroup[1];
                sidekick->drownTimer       = 0;
                sidekick->active           = ACTIVE_NORMAL;
                sidekick->collisionPlane   = 0;
                sidekick->collisionMode    = CMODE_FLOOR;
                sidekick->collisionLayers  = zone->collisionLayers;
                sidekick->controllerID     = CONT_P2;
                sidekick->sidekick         = true;
                sidekick->drawFX           = FX_FLIP | FX_ROTATE;
                sidekick->visible          = true;

#if MANIA_USE_PLUS
                Player->playerCount = globals->gameMode == MODE_ENCORE ? 2 : RSDK.GetEntityCount(Player->classID, false);
#else
                Player->playerCount = RSDK.GetEntityCount(Player->classID, false);
#endif
            }
        }
        destroyEntity(self);
    }
    else if (backPress) {
        self->state     = MegAmiMenu_State_Main;
        self->stateDraw = StateMachine_None;
    }
}

void MegAmiMenu_State_Shield(void)
{
    RSDK_THIS(MegAmiMenu);

    EntityPlayer *player = (EntityPlayer *)self->parent;
    bool32 confirmPress =
        ControllerInfo[player->controllerID].keyStart.press
        || (API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyB.press : ControllerInfo[player->controllerID].keyA.press);
    bool32 backPress = API_GetConfirmButtonFlip() ? ControllerInfo[player->controllerID].keyA.press : ControllerInfo[player->controllerID].keyB.press;

    if (ControllerInfo[player->controllerID].keyDown.press) {
        if (++self->subSelection > SHIELD_LIGHTNING)
            self->subSelection = SHIELD_NONE;
    }
    else if (ControllerInfo[player->controllerID].keyUp.press) {
        if (--self->subSelection < SHIELD_NONE)
            self->subSelection = SHIELD_LIGHTNING;
    }
    else if (confirmPress) {
        player->shield = self->subSelection;
        Player_ApplyShield(player);
        if (player->shield == SHIELD_NONE) {
            ObjectShield *Shield = Mod.FindObject("Shield");
            destroyEntity(RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield));
        }
        destroyEntity(self);
    }
    else if (backPress) {
        self->state     = MegAmiMenu_State_Main;
        self->stateDraw = StateMachine_None;
    }
}

void MegAmiMenu_State_DrawChar(void)
{
    RSDK_THIS(MegAmiMenu);

    RSDK.DrawRect(169, 12, 80, 100, 0xFF0000, 0xFF, INK_NONE, true);

    Vector2 drawPos;

    // Draw Selection Arrow
    drawPos.x = TO_FIXED(175);
    drawPos.y = TO_FIXED(21 + (self->subSelection * VERT_SPACING));
    RSDK.DrawText(&self->animator, &drawPos, &self->arrow, 0, self->arrow.length, ALIGN_LEFT, 0, 0, 0, true);

    // Initialize Option Text Position
    drawPos.x = TO_FIXED(185);
    drawPos.y = TO_FIXED(21);

    // Draw Option Text
    if (self->mainSelection == MEGAMIMENU_P2CHAR) {
        RSDK.DrawText(&self->animator, &drawPos, &self->none, 0, self->none.length, ALIGN_LEFT, 0, 0, 0, true);
        drawPos.y += TO_FIXED(VERT_SPACING);
    }
    RSDK.DrawText(&self->animator, &drawPos, &self->sonic, 0, self->sonic.length, ALIGN_LEFT, 0, 0, 0, true);
    drawPos.y += TO_FIXED(VERT_SPACING);
    RSDK.DrawText(&self->animator, &drawPos, &self->tails, 0, self->tails.length, ALIGN_LEFT, 0, 0, 0, true);
    drawPos.y += TO_FIXED(VERT_SPACING);
    RSDK.DrawText(&self->animator, &drawPos, &self->knux, 0, self->knux.length, ALIGN_LEFT, 0, 0, 0, true);
#if MANIA_USE_PLUS
    if (API.CheckDLC(DLC_PLUS)) {
        drawPos.y += TO_FIXED(VERT_SPACING);
        RSDK.DrawText(&self->animator, &drawPos, &self->mighty, 0, self->mighty.length, ALIGN_LEFT, 0, 0, 0, true);
        drawPos.y += TO_FIXED(VERT_SPACING);
        RSDK.DrawText(&self->animator, &drawPos, &self->ray, 0, self->ray.length, ALIGN_LEFT, 0, 0, 0, true);
    }
#endif
}

void MegAmiMenu_State_DrawShield(void)
{
    RSDK_THIS(MegAmiMenu);

    RSDK.DrawRect(169, 12, 130, 80, 0xFF0000, 0xFF, INK_NONE, true);

    Vector2 drawPos;

    // Draw Selection Arrow
    drawPos.x = TO_FIXED(175);
    drawPos.y = TO_FIXED(21 + (self->subSelection * VERT_SPACING));
    RSDK.DrawText(&self->animator, &drawPos, &self->arrow, 0, self->arrow.length, ALIGN_LEFT, 0, 0, 0, true);

    // Initialize Option Text Position
    drawPos.x = TO_FIXED(185);
    drawPos.y = TO_FIXED(21);

    // Draw Option Text
    RSDK.DrawText(&self->animator, &drawPos, &self->none, 0, self->none.length, ALIGN_LEFT, 0, 0, 0, true);
    drawPos.y += TO_FIXED(VERT_SPACING);
    RSDK.DrawText(&self->animator, &drawPos, &self->blue, 0, self->blue.length, ALIGN_LEFT, 0, 0, 0, true);
    drawPos.y += TO_FIXED(VERT_SPACING);
    RSDK.DrawText(&self->animator, &drawPos, &self->bubble, 0, self->bubble.length, ALIGN_LEFT, 0, 0, 0, true);
    drawPos.y += TO_FIXED(VERT_SPACING);
    RSDK.DrawText(&self->animator, &drawPos, &self->fire, 0, self->fire.length, ALIGN_LEFT, 0, 0, 0, true);
    drawPos.y += TO_FIXED(VERT_SPACING);
    RSDK.DrawText(&self->animator, &drawPos, &self->lightning, 0, self->lightning.length, ALIGN_LEFT, 0, 0, 0, true);
}

#if RETRO_INCLUDE_EDITOR
void MegAmiMenu_EditorDraw(void) {}

void MegAmiMenu_EditorLoad(void) {}
#endif

void MegAmiMenu_Serialize(void) {}
