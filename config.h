#include <X11/XF86keysym.h>

static int showsystray                   = 1;         /* 是否显示托盘栏 */
static const int newclientathead         = 0;         /* 定义新窗口在栈顶还是栈底 */
static const unsigned int borderpx       = 2;         /* 窗口边框大小 */
static const unsigned int systraypinning = 1;         /* 托盘跟随的显示器 0代表不指定显示器 */
static const unsigned int systrayspacing = 1;         /* 托盘间距 */
static const unsigned int systrayspadding = 5;        /* 托盘和状态栏的间隙 */
static int gappi                         = 12;        /* 窗口与窗口 缝隙大小 */
static int gappo                         = 12;        /* 窗口与边缘 缝隙大小 */
static const int _gappo                  = 12;        /* 窗口与窗口 缝隙大小 不可变 用于恢复时的默认值 */
static const int _gappi                  = 12;        /* 窗口与边缘 缝隙大小 不可变 用于恢复时的默认值 */
static const int vertpad                 = 5;         /* vertical padding of bar */
static const int sidepad                 = 5;         /* horizontal padding of bar */
static const int overviewgappi           = 24;        /* overview时 窗口与边缘 缝隙大小 */
static const int overviewgappo           = 60;        /* overview时 窗口与窗口 缝隙大小 */
static const int showbar                 = 1;         /* 是否显示状态栏 */
static const int topbar                  = 1;         /* 指定状态栏位置 0底部 1顶部 */
static const float mfact                 = 0.6;       /* 主工作区 大小比例 */
static const int   nmaster               = 1;         /* 主工作区 窗口数量 */
static const unsigned int snap           = 10;        /* 边缘依附宽度 */
static const unsigned int baralpha       = 0xc0;      /* 状态栏透明度 */
static const unsigned int borderalpha    = 0xdd;      /* 边框透明度 */
static const char *fonts[]               = { "JetBrainsMono Nerd Font:style=medium:size=14", "monospace:size=14" };
static const char *colors[][3]           = {          /* 颜色设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { "#bbbbbb", "#333333", "#444444" },
    [SchemeSel] = { "#ffffff", "#37474F", "#42A5F5" },
    [SchemeSelGlobal] = { "#ffffff", "#37474F", "#FFC0CB" },
    [SchemeHid] = { "#dddddd", NULL, NULL },
    [SchemeSystray] = { NULL, "#7799AA", NULL },
    [SchemeUnderline] = { "#7799AA", NULL, NULL }, 
    [SchemeNormTag] = { "#bbbbbb", "#333333", NULL },
    [SchemeSelTag] = { "#eeeeee", "#333333", NULL },
    [SchemeBarEmpty] = { NULL, "#111111", NULL },
};
static const unsigned int alphas[][3]    = {          /* 透明度设置 ColFg, ColBg, ColBorder */ 
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSel] = { OPAQUE, baralpha, borderalpha },
    [SchemeSelGlobal] = { OPAQUE, baralpha, borderalpha },
    [SchemeNormTag] = { OPAQUE, baralpha, borderalpha }, 
    [SchemeSelTag] = { OPAQUE, baralpha, borderalpha },
    [SchemeBarEmpty] = { NULL, 0xa0a, NULL },
    [SchemeStatusText] = { OPAQUE, 0x88, NULL },
};

/* 自定义脚本位置 */
static const char *autostartscript = "$SCRIPTS/autostart.sh";
static const char *statusbarscript = "$DWM/statusbar/statusbar.sh";

/* 自定义 scratchpad instance */
static const char scratchpadname[] = "scratchpad";

//自定义浏览器指令
static const char *browsercmd[]  = { "brave", NULL };

/*自定义volumetoggle */
static const char *volumetoggle[] = { "/home/masterj/scripts/volume_toggle.sh", NULL};

//自定义dmenu
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char dmenufont[]       = {"Source code pro:size=16"};
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };


/* 自定义tag名称 */
/* 自定义特定实例的显示状态 */
//            ﮸  ﭮ 切
// 对应的tag序号以及快捷键:   0:1  1:2  2:3  3:4  4:5  5:9  6:c  7:m  8:0  9:w 10:l
static const char *tags[] = { "", "", "", "", "","","","","", "", "", "ﬄ", "﬐", "" };
static const Rule rules[] = {
    /* class                 instance              title             tags mask     isfloating  isglobal    isnoborder monitor */
    {"obs",                  NULL,                 NULL,             1 << 8,       0,          0,          0,        -1 },
    {"chrome",               NULL,                 NULL,             1 << 9,       0,          0,          0,        -1 },
  //{"Chromium",             NULL,                 NULL,             1 << 6,       0,          0,          0,        -1 },
    {"music",                NULL,                 NULL,             1 << 10,       1,          0,          1,        -1 },
    { NULL,                 "qq",                  NULL,             1 << 11,       0,          0,          1,        -1 },
    { NULL,                 "wechat.exe",          NULL,             1 << 12,       0,          0,          0,        -1 },
    { NULL,                 "wxwork.exe",          NULL,             1 << 13,      0,          0,          0,        -1 },
    { NULL,                  NULL,                "broken",          0,            1,          0,          0,        -1 },
    { "图片查看",           "图片查看",           "图片查看",        0,            1,          0,          0,        -1 },
    { "图片预览",           "图片预览",           "图片预览",        0,            1,          0,          0,        -1 },
    { NULL,                  NULL,                "crx_",            0,            1,          0,          0,        -1 },
    {"flameshot",            NULL,                 NULL,             0,            1,          0,          0,        -1 },
    {"wemeetapp",            NULL,                 NULL,             TAGMASK,      1,          1,          0,        -1 }, // 腾讯会议在切换tag时有诡异bug导致退出 变成global来规避该问题
    {"float",                NULL,                 NULL,             0,            1,          0,          0,        -1 }, // 特殊class client默认浮动
    {"noborder",             NULL,                 NULL,             0,            1,          0,          1,        -1 }, // 特殊class client默认无边框
    {"global",               NULL,                 NULL,             TAGMASK,      1,          1,          0,        -1 }, // 特殊class client全局于所有tag
};
static const char *overviewtag = "OVERVIEW";
static const Layout overviewlayout = { "舘",  overview };

/* 自定义布局 */
static const Layout layouts[] = {
    { "﬿",  tile },         /* 主次栈 */
    { "﩯",  magicgrid },    /* 网格 */
};

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG, cmd) \
    { MODKEY,              KEY, view,       {.ui = 1 << TAG, .v = cmd} }, \
    { MODKEY|ShiftMask,    KEY, tag,        {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,  KEY, toggleview, {.ui = 1 << TAG} }, \

static Key keys[] = {
    /* modifier            key              function          argument */
    { MODKEY,              XK_equal,        togglesystray,    {0} },                     /* super +            |  切换 托盘栏显示状态 */

	{ MODKEY,              XK_w,            spawn,          {.v = browsercmd } },
    { MODKEY,              XK_d,            spawn,          {.v = dmenucmd } },

    { MODKEY,              XK_j,            focusstack,         {.i = +1} },               /* super tab          |  本tag内切换聚焦窗口 */
    { MODKEY,              XK_k,            focusstack,         {.i = -1} },               /* super shift tab    |  本tag内切换聚焦窗口 */

    { MODKEY,              XK_u,            viewtoleft,       {0} },                     /* super left         |  聚焦到左边的tag */
    { MODKEY,              XK_i,            viewtoright,      {0} },                     /* super right        |  聚焦到右边的tag */
    { MODKEY|ShiftMask,    XK_u,            tagtoleft,        {0} },                     /* super shift left   |  将本窗口移动到左边tag */
    { MODKEY|ShiftMask,    XK_i,            tagtoright,       {0} },                     /* super shift right  |  将本窗口移动到右边tag */

    { MODKEY,              XK_0,            toggleoverview,   {0} },                     /* super 0            |  显示所有tag 或 跳转到聚焦窗口的tag */

    { MODKEY,              XK_comma,        setmfact,         {.f = -0.05} },            /* super ,            |  缩小主工作区 */
    { MODKEY,              XK_period,       setmfact,         {.f = +0.05} },            /* super .            |  放大主工作区 */

    { MODKEY,              XK_o,            hidewin,          {0} },                     /* super h            |  隐藏 窗口 */
    { MODKEY|ShiftMask,    XK_o,            restorewin,       {0} },                     /* super shift h      |  取消隐藏 窗口 */

    { MODKEY|ShiftMask,    XK_Return,       zoom,             {0} },                     /* super shift enter  |  将当前聚焦窗口置为主窗口 */

    { MODKEY,              XK_t,            togglefloating,   {0} },                     /* super t            |  开启/关闭 聚焦目标的float模式 */
    { MODKEY|ShiftMask,    XK_t,            toggleallfloating,{0} },                     /* super shift t      |  开启/关闭 全部目标的float模式 */
    { MODKEY,              XK_f,            fullscreen,       {0} },                     /* super f            |  开启/关闭 全屏 */
    { MODKEY|ShiftMask,    XK_f,            togglebar,        {0} },                     /* super shift f      |  开启/关闭 状态栏 */
    { MODKEY,              XK_g,            toggleglobal,     {0} },                     /* super g            |  开启/关闭 全局 */
    { MODKEY,              XK_e,            incnmaster,       {.i = +1} },               /* super e            |  改变主工作区窗口数量 (1 2中切换) */

    { MODKEY,              XK_b,            focusmon,         {.i = +1} },               /* super b            |  光标移动到另一个显示器 */
    { MODKEY|ShiftMask,    XK_b,            tagmon,           {.i = +1} },               /* super shift b      |  将聚焦窗口移动到另一个显示器 */

    { MODKEY,              XK_q,            killclient,       {0} },                     /* super q            |  关闭窗口 */
    { MODKEY|ShiftMask,    XK_q,            forcekillclient,  {0} },                     /* super ctrl q       |  强制关闭窗口(处理某些情况下无法销毁的窗口) */
    { MODKEY|ControlMask,  XK_F12,          quit,             {0} },                     /* super ctrl f12     |  退出dwm */

	{ MODKEY|ShiftMask,    XK_space,        selectlayout,     {.v = &layouts[1]} },      /* super shift space  |  切换到网格布局 */
	{ MODKEY,              XK_p,            showonlyorall,    {0} },                     /* super o            |  切换 只显示一个窗口 / 全部显示 */

    { MODKEY|ControlMask,  XK_equal,        setgap,           {.i = -6} },               /* super ctrl +       |  窗口增大 */
    { MODKEY|ControlMask,  XK_minus,        setgap,           {.i = +6} },               /* super ctrl -       |  窗口减小 */
    { MODKEY|ControlMask,  XK_space,        setgap,           {.i = 0} },                /* super ctrl space   |  窗口重置 */

    { MODKEY|ControlMask,  XK_k,            movewin,          {.ui = UP} },              /* super ctrl up      |  移动窗口 */
    { MODKEY|ControlMask,  XK_j,            movewin,          {.ui = DOWN} },            /* super ctrl down    |  移动窗口 */
    { MODKEY|ControlMask,  XK_h,            movewin,          {.ui = LEFT} },            /* super ctrl left    |  移动窗口 */
    { MODKEY|ControlMask,  XK_l,            movewin,          {.ui = RIGHT} },           /* super ctrl right   |  移动窗口 */

    { MODKEY|Mod1Mask,     XK_k,            resizewin,        {.ui = V_REDUCE} },        /* super alt up       |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_j,            resizewin,        {.ui = V_EXPAND} },        /* super alt down     |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_h,            resizewin,        {.ui = H_REDUCE} },        /* super alt left     |  调整窗口 */
    { MODKEY|Mod1Mask,     XK_l,            resizewin,        {.ui = H_EXPAND} },        /* super alt right    |  调整窗口 */


	{ 0,             	   0x1008ff12,  spawn,          {.v = volumetoggle } },// F1
	{ 0,             	   0x1008ff11,  spawn,          SHCMD("~/scripts/set_vol.sh down") },// F2
	{ 0,             	   0x1008ff13, 	spawn,          SHCMD("~/scripts/set_vol.sh up") },// F3

    /* spawn + SHCMD 执行对应命令(已下部分建议完全自己重新定义) */
    { MODKEY,              XK_s,      togglescratch, SHCMD("st -t scratchpad -c float") },                      /* super s          | 打开scratch终端        */
    { MODKEY,              XK_Return, spawn, SHCMD("st") },                                                     /* super enter      | 打开st终端             */
    { MODKEY,              XK_minus,  spawn, SHCMD("st -c global") },                                           /* super +          | 打开全局st终端         */
    { MODKEY,              XK_space,  spawn, SHCMD("st -c float") },                                            /* super space      | 打开浮动st终端         */
    { MODKEY|ShiftMask,    XK_s,      spawn, SHCMD("boomer")},
    { 0,                   XK_F12,    spawn, SHCMD("~/scripts/screenshot") },                                   /* super shift a    | 截图                   */
    //{ MODKEY|ShiftMask,    XK_q,      spawn, SHCMD("kill -9 $(xprop | grep _NET_WM_PID | awk '{print $3}')") }, /* super shift q    | 选中某个窗口并强制kill */
    { ShiftMask|ControlMask, XK_c,    spawn, SHCMD("xclip -o | xclip -selection c") },                          /* super shift c    | 进阶复制               */

    /* super key : 跳转到对应tag (可附加一条命令 若目标目录无窗口，则执行该命令) */
    /* super shift key : 将聚焦窗口移动到对应tag */
    /* key tag cmd */

    TAGKEYS(XK_1, 0,  0)
    TAGKEYS(XK_2, 1,  0)
    TAGKEYS(XK_3, 2,  0)
    TAGKEYS(XK_4, 3,  0)
    TAGKEYS(XK_5, 4,  0)
    TAGKEYS(XK_6, 5,  0)
    TAGKEYS(XK_7, 6,  0)
    TAGKEYS(XK_8, 7,  0)
    TAGKEYS(XK_9, 8,  "obs")
    TAGKEYS(XK_n, 9,  "google-chrome-stable")
    TAGKEYS(XK_m, 10,  "netease-cloud-music")
    TAGKEYS(XK_z, 11,  "linuxqq")
    TAGKEYS(XK_x, 12,  "/opt/apps/com.qq.weixin.deepin/files/run.sh")
    TAGKEYS(XK_c, 13, "")
};
static Button buttons[] = {
    /* click               event mask       button            function       argument  */
    /* 点击窗口标题栏操作 */
    { ClkWinTitle,         0,               Button1,          hideotherwins, {0} },                                   // 左键        |  点击标题     |  隐藏其他窗口仅保留该窗口
    { ClkWinTitle,         0,               Button3,          togglewin,     {0} },                                   // 右键        |  点击标题     |  切换窗口显示状态
    /* 点击窗口操作 */
    { ClkClientWin,        MODKEY,          Button1,          movemouse,     {0} },                                   // super+左键  |  拖拽窗口     |  拖拽窗口
    { ClkClientWin,        MODKEY,          Button3,          resizemouse,   {0} },                                   // super+右键  |  拖拽窗口     |  改变窗口大小
    /* 点击tag操作 */
    { ClkTagBar,           0,               Button1,          view,          {0} },                                   // 左键        |  点击tag      |  切换tag
	{ ClkTagBar,           0,               Button3,          toggleview,    {0} },                                   // 右键        |  点击tag      |  切换是否显示tag
    { ClkTagBar,           MODKEY,          Button1,          tag,           {0} },                                   // super+左键  |  点击tag      |  将窗口移动到对应tag
    { ClkTagBar,           0,               Button4,          viewtoleft,    {0} },                                   // 鼠标滚轮上  |  tag          |  向前切换tag
	{ ClkTagBar,           0,               Button5,          viewtoright,   {0} },                                   // 鼠标滚轮下  |  tag          |  向后切换tag
    /* 点击状态栏操作 */
    { ClkStatusText,       0,               Button1,          clickstatusbar,{0} },                                   // 左键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal L
    { ClkStatusText,       0,               Button2,          clickstatusbar,{0} },                                   // 中键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal M
    { ClkStatusText,       0,               Button3,          clickstatusbar,{0} },                                   // 右键        |  点击状态栏   |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal R
    { ClkStatusText,       0,               Button4,          clickstatusbar,{0} },                                   // 鼠标滚轮上  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal U
    { ClkStatusText,       0,               Button5,          clickstatusbar,{0} },                                   // 鼠标滚轮下  |  状态栏       |  根据状态栏的信号执行 ~/scripts/dwmstatusbar.sh $signal D
                                                                                                                      //
    /* 点击bar空白处 */
    { ClkBarEmpty,         0,               Button1,          spawn, SHCMD("~/scripts/call_rofi.sh window") },        // 左键        |  bar空白处    |  rofi 执行 window
    { ClkBarEmpty,         0,               Button3,          spawn, SHCMD("~/scripts/call_rofi.sh drun") },          // 右键        |  bar空白处    |  rofi 执行 drun
};
