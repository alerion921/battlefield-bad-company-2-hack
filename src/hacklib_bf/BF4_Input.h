#ifndef BF4_INPUT_H
#define BF4_INPUT_H


namespace BF4
{
    enum InputConceptId
    {
        MoveFB = 0,
        MoveLR = 1,
        MoveForward = 2,
        MoveBackward = 3,
        MoveLeft = 4,
        MoveRight = 5,
        Yaw = 6,
        Pitch = 7,
        Roll = 8,
        Fire = 10,
        Reload = 13,
        Crawl = 17,
        Interact = 34,
        EnterVehicle = 57,
        Brake = 58,
        Spot = 84,
    };

    class IMouse
    {
    public:
        virtual void init();
        virtual void release();
        virtual void preFrameUpdate(float);
        virtual void update(float);
        virtual void updateBetweenFrames(float);
        virtual void function5();
        virtual void function6();
        virtual void function7();
        virtual void function8();
        virtual void function9();
        virtual void function10();
        virtual void enableCursorMode(bool, int);
    };

    class InputCache
    {
    public:
        BYTE m_disableCache;
        char _pad_1[3];
        float m_conceptCache[123];
        float m_juiceConceptCache[123];
        float m_prevJuiceConceptCache[123];
        void* m_recordData;
        void* m_replayData;
        void* m_juiceController;
    };

    class BorderInputNode
    {
        IMPLMEMBER(bool, ForceReadCache, 0x4);
        IMPLMEMBER(InputCache*, InputCache, 0x8);
        IMPLMEMBER(bool, ForceRead2, 0x10);
        IMPLMEMBER(IMouse*, Mouse, 0x40);
    };
}

#endif


/*

class IMouse
{
public:
virtual void init(); //
virtual void release(); //
virtual void preFrameUpdate(float); // oh, hi!
virtual void update(float); //
virtual void updateBetweenFrames(float); //
virtual void function5(); //
virtual void function6(); //
virtual void function7(); //
virtual void function8(); //
virtual void function9(); //
virtual void function10(); //
virtual void enableCursorMode(bool, int); //

char unknown4[4]; //0x0004
};

class InputCache
{
public:
BYTE m_disableCache; //0x0000
char _pad_1[3]; //0x0001
float m_conceptCache[123]; //0x0004
float m_juiceConceptCache[123]; //0x007F
float m_prevJuiceConceptCache[123]; //0x00FA
void* m_recordData; //0x0175
void* m_replayData; //0x0179
void* m_juiceController; //0x017D
};

class BorderInputNode
{
public:
virtual void function0();
virtual void function1();

InputCache* m_inputCache; //0x0004
BYTE m_forceReadCache; //0x0008
char _pad_9[3]; //0x0009
__int32 m_padRange_firstId; //0x000C
__int32 m_padRange_lastId; //0x0010
__int32 m_motionRange_firstId; //0x0014
__int32 m_motionRange_lastId; //0x0018
__int32 m_mouseRange_firstId; //0x001C
__int32 m_mouseRange_lastId; //0x0020
__int32 m_keyboardRange_firstId; //0x0024
__int32 m_keyboardRange_lastId; //0x0028
__int32 m_messageRange_firstId; //0x002C
__int32 m_messageRange_lastId; //0x0030
void* m_pad; //0x0034
void* m_motionController; //0x0038
void** m_keyboard; //0x003C
IMouse* m_mouse; //0x0040
};



enum InputConceptIdentifiers
{
ConceptMoveFB=0,
ConceptMoveLR=1,
ConceptMoveForward=2,
ConceptMoveBackward=3,
ConceptMoveLeft=4,
ConceptMoveRight=5,
ConceptYaw=6,
ConceptPitch=7,
ConceptRoll=8,
ConceptRecenterCamera=9,
ConceptFire=10,
ConceptAltFire=11,
ConceptFireCountermeasure=12,
ConceptReload=13,
ConceptZoom=14,
ConceptToggleCamera=15,
ConceptSprint=16,
ConceptCrawl=17,
ConceptToggleWeaponLight=18,
ConceptSelectPartyMember0=19,
ConceptSelectPartyMember1=20,
ConceptSelectPartyMember2=21,
ConceptSelectPartyMember3=22,
ConceptLockTarget=23,
ConceptCrosshairMoveX=24,
ConceptCrosshairMoveY=25,
ConceptTacticalMenu=26,
ConceptConversationSelect=27,
ConceptConversationSkip=28,
ConceptConversationChangeSelection=29,
ConceptJump=30,
ConceptCrouch=31,
ConceptCrouchOnHold=32,
ConceptProne=33,
ConceptInteract=34,
ConceptPickUp=35,
ConceptDrop=36,
ConceptBreathControl=37,
ConceptParachute=38,
ConceptSwitchInventoryItem=39,
ConceptSelectInventoryItem1=40,
ConceptSelectInventoryItem2=41,
ConceptSelectInventoryItem3=42,
ConceptSelectInventoryItem4=43,
ConceptSelectInventoryItem5=44,
ConceptSelectInventoryItem6=45,
ConceptSelectInventoryItem7=46,
ConceptSelectInventoryItem8=47,
ConceptSelectInventoryItem9=48,
ConceptSwitchToPrimaryWeapon=49,
ConceptSwitchToGrenadeLauncher=50,
ConceptSwitchToStaticGadget=51,
ConceptSwitchToDynamicGadget1=52,
ConceptSwitchToDynamicGadget2=53,
ConceptMeleeAttack=54,
ConceptThrowGrenade=55,
ConceptCycleFireMode=56,
ConceptChangeVehicle=57,
ConceptBrake=58,
ConceptHandBrake=59,
ConceptClutch=60,
ConceptGearUp=61,
ConceptGearDown=62,
ConceptGearSwitch=63,
ConceptNextPosition=64,
ConceptSelectPosition1=65,
ConceptSelectPosition2=66,
ConceptSelectPosition3=67,
ConceptSelectPosition4=68,
ConceptSelectPosition5=69,
ConceptSelectPosition6=70,
ConceptSelectPosition7=71,
ConceptSelectPosition8=72,
ConceptCameraPitch=73,
ConceptCameraYaw=74,
ConceptMapZoom=75,
ConceptMapInnerZoom=76,
ConceptMapSize=77,
ConceptMapThreeDimensional=78,
ConceptScoreboard=79,
ConceptScoreboardAndMenuCombo=80,
ConceptMenu=81,
ConceptSpawnMenu=82,
ConceptCancel=83,
ConceptCommMenu1=84,
ConceptCommMenu2=85,
ConceptCommMenu3=86,
ConceptAccept=87,
ConceptDecline=88,
ConceptSelect=89,
ConceptBack=90,
ConceptActivate=91,
ConceptDeactivate=92,
ConceptEdit=93,
ConceptView=94,
ConceptParentNavigateLeft=95,
ConceptParentNavigateRight=96,
ConceptMenuZoomIn=97,
ConceptMenuZoomOut=98,
ConceptPanX=99,
ConceptPanY=100,
ConceptBattledashToggle=101,
ConceptVoiceFunction1=102,
ConceptSquadVoice=103,
ConceptSayAllChat=104,
ConceptTeamChat=105,
ConceptSquadChat=106,
ConceptSquadLeaderChat=107,
ConceptToggleChat=108,
ConceptQuicktimeInteractDrag=109,
ConceptQuicktimeFire=110,
ConceptQuicktimeBlock=111,
ConceptQuicktimeFastMelee=112,
ConceptQuicktimeJumpClimb=113,
ConceptQuicktimeCrouchDuck=114,
ConceptFreeCameraMoveUp=115,
ConceptFreeCameraMoveDown=116,
ConceptFreeCameraMoveLR=117,
ConceptFreeCameraMoveFB=118,
ConceptFreeCameraMoveUD=119,
ConceptFreeCameraRotateX=120,
ConceptFreeCameraRotateY=121,
ConceptFreeCameraRotateZ=122,
ConceptFreeCameraIncreaseSpeed=123,
ConceptFreeCameraDecreaseSpeed=124,
ConceptFreeCameraFOVModifier=125,
ConceptFreeCameraChangeFOV=126,
ConceptFreeCameraSwitchSpeed=127,
ConceptFreeCameraTurboSpeed=128,
ConceptFreeCameraActivator1=129,
ConceptFreeCameraActivator2=130,
ConceptFreeCameraActivator3=131,
ConceptFreeCameraMayaInputActivator=132,
ConceptTargetedCameraDistance=133,
ConceptTargetedCameraRotateX=134,
ConceptTargetedCameraRotateY=135,
ConceptTargetedCameraChangeSpeed=136,
ConceptLThumb=137,
ConceptRThumb=138,
ConceptLeftStickX=139,
ConceptLeftStickY=140,
ConceptRightStickX=141,
ConceptRightStickY=142,
ConceptButtonDPadLeft=143,
ConceptButtonDPadRight=144,
ConceptButtonDPadUp=145,
ConceptButtonDPadDown=146,
ConceptButtonA=147,
ConceptButtonB=148,
ConceptButtonX=149,
ConceptButtonY=150,
ConceptButtonSelect=151,
ConceptButtonStart=152,
ConceptButtonL1=153,
ConceptButtonL2=154,
ConceptButtonR1=155,
ConceptButtonR2=156,
ConceptButtonLeftThumb=157,
ConceptButtonRightThumb=158,
ConceptToggleMinimapType=159,
ConceptDeployZoom=160,
ConceptMenuDigitalUp=161,
ConceptMenuDigitalDown=162,
ConceptMenuDigitalLeft=163,
ConceptMenuDigitalRight=164,
ConceptRightStickUp=165,
ConceptRightStickDown=166,
ConceptRightStickLeft=167,
ConceptRightStickRight=168,
ConceptMultipleSelect=169,
ConceptMoveCameraUp=170,
ConceptMoveCameraDown=171,
ConceptMoveCameraLeft=172,
ConceptMoveCameraRight=173,
ConceptSpectatorViewPrev=174,
ConceptSpectatorViewNext=175,
ConceptSpectatorTargetPrev=176,
ConceptSpectatorTargetNext=177,
ConceptSpectatorViewTableTop=178,
ConceptSpectatorViewFirstPerson=179,
ConceptSpectatorViewThirdPerson=180,
ConceptSpectatorViewFreeCam=181,
ConceptSpectatorViewPlayer1=182,
ConceptSpectatorViewPlayer2=183,
//ConceptSpectatorViewPlayer3=184
//ConceptSpectatorViewPlayer4=185
//ConceptSpectatorViewPlayer5=186
//ConceptSpectatorViewPlayer6=187
//ConceptSpectatorViewPlayer7=188
//ConceptSpectatorViewPlayer8=189
//ConceptSpectatorViewPlayer9=190
//ConceptSpectatorViewPlayer10=191
//ConceptSpectatorViewPlayer11=192
//ConceptSpectatorViewPlayer12=193
//ConceptSpectatorViewPlayer13=194
//ConceptSpectatorViewPlayer14=195
//ConceptSpectatorViewPlayer15=196
//ConceptSpectatorViewPlayer16=197
//ConceptSpectatorViewOptions=198
//ConceptSpectatorHudVisibility=199
//ConceptAnalogZoom=200
//ConceptSpectatorTargetPrevInSquad=201
//ConceptSpectatorTargetNextInSquad=202
//ConceptSpectatorTargetPrevOnTeam=203
//ConceptSpectatorTargetNextOnTeam=204
//ConceptSpectatorFreeCameraIncreaseSpeed=205
//ConceptSpectatorFreeCameraDecreaseSpeed=206
//ConceptSpectatorFreeCameraSwitchSpeed=207
//ConceptSpectatorFreeCameraMoveLR=208
//ConceptSpectatorFreeCameraMoveUD=209
//ConceptSpectatorFreeCameraMoveFB=210
//ConceptSpectatorFreeCameraRotateX=211
//ConceptSpectatorFreeCameraRotateY=212
//ConceptSpectatorFreeCameraRotateZ=213
//ConceptSpectatorFreeCameraTurboSpeed=214
//ConceptSpectatorFreeCameraFOVModifier=215
//ConceptSpectatorFreeCameraChangeFOV=216
//ConceptSpectatorSquadLeft=217
//ConceptSpectatorSquadRight=218
//ConceptSpectatorSquadUp=219
//ConceptSpectatorSquadDown=220
//ConceptSpectatorSquadActivate=221
//ConceptUndefined=222
//ConceptSize=223
};

*/