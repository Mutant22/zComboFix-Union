#include <Union/Hook.h>

namespace GOTHIC_NAMESPACE
{
void __fastcall zComboFix__hook_oCNpc_EV_AttackForward_HitCombo_Npc(Union::Registers& reg)
{
    if (!reg.ecx) { return; };
    oCAniCtrl_Human* aniCtrl = reinterpret_cast<oCAniCtrl_Human*>(reg.ecx);

    if (!aniCtrl->npc) { return; };

    //If Npc is in combo animation
    if (aniCtrl->IsInCombo()) {
        //Some fight styles don't have combos - if we would enable combo - Npc would freeze in their animation
        //Check if combo can be enabled
        if (aniCtrl->comboNr + 1 < aniCtrl->comboMax) {
            //Enable combo :)
            aniCtrl->canEnableNextCombo = aniCtrl->canEnableNextCombo | 1;
        }
    }
}

//init
void zComboFix_G12_PatchFightCombos()
{
    static int once = 0;
    if (!once) {

        //In engine there is an incorrect condition check (< instead of >) ... we need to swap EAX - EDX comparison to fix it
        //3b c2 CMP EAX, EDX
        //3b d0 CMP EDX, EAX

        char* addr_oCNpc__EV_AttackForward_ComboCheck = nullptr;
        if (GetGameVersion() == Engine_G1) {
            //006aae8f
            addr_oCNpc__EV_AttackForward_ComboCheck = reinterpret_cast<char*>(0x006aae8f);
        } else if (GetGameVersion() == Engine_G2A) {
            //0074ff77
            addr_oCNpc__EV_AttackForward_ComboCheck = reinterpret_cast<char*>(0x0074ff77);
        } else {
            // unsupported game
            once = 1;
            return;
        }
        DWORD protection = PAGE_READWRITE;
        VirtualProtect(addr_oCNpc__EV_AttackForward_ComboCheck + 1, 1, protection, &protection);
        *(addr_oCNpc__EV_AttackForward_ComboCheck + 1) = 0xD0;
        VirtualProtect(addr_oCNpc__EV_AttackForward_ComboCheck + 1, 1, protection, &protection);

        //Additionally we need to add canEnableNextCombo flag to aniCtrl if aniCtrl is in combo animation ... this is handled by hook below
        //hook memory address inside oCNpc::EV_AttackForward, where oCAniCtrl_Human::HitCombo is called
        if (GetGameVersion() == Engine_G1) {
            //006aae99
            auto partialHook = Union::CreatePartialHook((void*)0x006aae99, &zComboFix__hook_oCNpc_EV_AttackForward_HitCombo_Npc);
        } else if (GetGameVersion() == Engine_G2A) {
            //0074ff81
            auto partialHook = Union::CreatePartialHook((void*)0x0074ff81, &zComboFix__hook_oCNpc_EV_AttackForward_HitCombo_Npc);
        }

        once = 1;
    }
}
} //namespace
