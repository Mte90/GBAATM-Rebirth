@define ROTATION_FLAG 0x100
@define SIZE_DOUBLE 0x200
@define MODE_NORMAL 0x0
@define MODE_TRANSPERANT 0x400
@define MODE_WINDOWED 0x800
@define MOSAIC 0x1000
@define COLOR_16 0x0000
@define COLOR_256 0x2000
@define SQUARE 0x0
@define TALL 0x8000
@define WIDE 0x4000

@define HORIZONTAL_FLIP 0x1000
@define VERTICAL_FLIP 0x2000
@define SIZE_8 0x0
@define SIZE_16 0x4000
@define SIZE_32 0x8000
@define SIZE_64 0xC000
@define OAM 0x07000000 
@define OBJPAL 0x5000200
@define CHARMEM 0x6010000
@define CHARMEM_MODE3 0x6012BFD

@macro Init_Sprites
ldr r0,=OAM
ldr r1,=128
ldr r2,=(300<<16)
initlooplabel
str r2,[r0]+8!
subs r1,r1,1
bne initlooplabel
@endm

@macro MoveSprite Sprite, X, Y
ldr r2,=(OAM+(Sprite*8))  ;get the starting point of sprite's OAM Atts.
ldr r3,[r2]           ; load r3 with attributes 0&1.
mov r3,r3, lsr#16     ; shift so we only have att#0 in r3.
ldr r4,[r2]           ; load r4 with attributes 0&1.
mov r4,r4, lsl#16     ; shift so we only have att#1 in r4.
mov r4,r4, lsr#16     ; get r4's value in the lower bits.
and r3,r3,#0xFF00     ; clear the old y value.
and r4,r4,#0xFE00    ; clear the old x value.
orr r3,r3,#X       ; set new y value.
orr r4,r4,#Y       ; set new x value.
mov r3,r3, lsl#16  ; get ready to add them.
orr r4,r4,r3       ; add them together so we have both in r4.
str r4,[r2]        ; store in OAM. we're done! :)
@endm

@macro SetNumber Sprite, Number
ldr r2,=(OAM + (Sprite*8))+6
ldr r3,=Number
strh r3,[r2]              
@endm


;some defines for the rotation data.

@define PA_0 0x7000006
@define PB_0 0x700000E
@define PC_0 0x7000015
@define PD_0 0x700001C

@define PA_1 0x7000026
@define PB_1 0x700002E
@define PC_1 0x7000036
@define PD_1 0x700003E

@define PA_2 0x7000046
@define PB_2 0x700004E
@define PC_2 0x7000055
@define PD_2 0x700005C

@define PA_3 0x7000066
@define PB_3 0x700006E
@define PC_3 0x7000076
@define PD_3 0x700007E

@define PA_4 0x7000086
@define PB_4 0x700008E
@define PC_4 0x7000095
@define PD_4 0x700009C

@define PA_5 0x70000A6
@define PB_5 0x70000AE
@define PC_5 0x70000B6
@define PD_5 0x70000BE

@define PA_6 0x70000C6
@define PB_6 0x70000CE
@define PC_6 0x70000D5
@define PD_6 0x70000DC

@define PA_7 0x70000E6
@define PB_7 0x70000EE
@define PC_7 0x70000F6
@define PD_7 0x70000FE

@define PA_8 0x7000106
@define PB_8 0x700010E
@define PC_8 0x7000115
@define PD_8 0x700011C

@define PA_9 0x7000126
@define PB_9 0x700012E
@define PC_9 0x7000136
@define PD_9 0x700013E

@define PA_10 0x7000146
@define PB_10 0x700014E
@define PC_10 0x7000155
@define PD_10 0x700015C

@define PA_11 0x7000166
@define PB_11 0x700016E
@define PC_11 0x7000176
@define PD_11 0x700017E

@define PA_12 0x7000186
@define PB_12 0x700018E
@define PC_12 0x7000195
@define PD_12 0x700019C

@define PA_13 0x70001A6
@define PB_13 0x70001AE
@define PC_13 0x70001B6
@define PD_13 0x70001BE

@define PA_14 0x70001C6
@define PB_14 0x70001CE
@define PC_14 0x70001D5
@define PD_14 0x70001DC

@define PA_15 0x70001E6
@define PB_15 0x70001EE
@define PC_15 0x70001F6
@define PD_15 0x70001FE

@define PA_16 0x7000206
@define PB_16 0x700020E
@define PC_16 0x7000215
@define PD_16 0x700021C

@define PA_17 0x7000226
@define PB_17 0x700022E
@define PC_17 0x7000236
@define PD_17 0x700023E

@define PA_18 0x7000246
@define PB_18 0x700024E
@define PC_18 0x7000255
@define PD_18 0x700025C

@define PA_19 0x7000266
@define PB_19 0x700026E
@define PC_19 0x7000266
@define PD_19 0x700026E

;; not quite done yet, but the pattern isn't too hard to figure out if you want to
;; make the rest on your own. :)

